#ifndef animal_h
#define animal_h

#include "helper.h"
#include "board.h"

class Animal {
public:
    enum AnimalType { MOUSE, CAT, WOLF, DOG, LEOPARD, TIGER, LION, ELEPHANT };
    static const Point INVALID_MOVE;
    static const Point DIE;

private:
    Board* _board;
    Point _pos;
    const AnimalType _type;
    bool _is_friend;
    
    bool _move_target_check(const Point& target) const;
    
public:
    Animal(Board* board, const int x, const int y, const AnimalType type, const bool isfriend):
        _board(board), _pos(Point(x, y)), _type(type), _is_friend(isfriend)
    {}
    
    short x() const { return _pos.x; }
    short y() const { return _pos.y; }
    const Point& position() const { return _pos; }
    AnimalType type() const { return _type; }
    bool isFriend() const { return _is_friend; }
    
    Board::LandType landType() const { return Board::land[_pos.x][_pos.y]; }
    
    
    bool canEat(const Animal* target) const;
    Point canMove(const int direction) const;
    Animal* move(const Point& target);
};

inline bool Animal::canEat(const Animal* target) const {
    if (_is_friend == target->_is_friend)
        return false;
    if ((target->_is_friend && target->landType() == Board::ENEMY_TRAP) ||
        (!target->_is_friend && target->landType() == Board::FRIEND_TRAP))
        return true;
    if (_type == MOUSE)
        return target->_type == ELEPHANT && landType() != Board::WATER;
    if (_type == ELEPHANT)
        return target->_type != MOUSE;
    return _type >= target->_type;
}

inline bool Animal::_move_target_check(const Point& target) const {
    Animal* tar = _board->animal(target);
    if (!tar)
        return true;
    return canEat(tar);
}

inline Point Animal::canMove(const int direction) const {
    Point pos1(_pos + DIR1[direction]);
    if (!Board::inBoard(pos1))
        return INVALID_MOVE;
    Board::LandType land = Board::landType(pos1);
    if (land == Board::WATER) {
        if (_type == TIGER || _type == LION) {
            Point pos2(_pos + DIR2[direction]);
            if (!Board::inBoard(pos2) || !_move_target_check(pos2))
                return INVALID_MOVE;
            for (; pos1 != pos2; pos1 += DIR1[direction])
                if (_board->animal(pos1))
                    return INVALID_MOVE;
            return pos2;
        }
        return INVALID_MOVE;
    }
    if ((_is_friend && land == Board::FRIEND_CAVE) ||
        (!_is_friend && land == Board::ENEMY_CAVE) ||
        !_move_target_check(pos1))
        return INVALID_MOVE;
    return pos1;
}

inline Animal* Animal::move(const Point& target) {
    Animal* animal = _board->animal(target);
    
    _board->animal(target, this);
    _board->animal(_pos, nullptr);
    _pos = target;
    
    if (animal) {
        if (animal->_is_friend) {
            _board->friendAnimal(animal->_type, nullptr);
        } else {
            _board->enemyAnimal(animal->_type, nullptr);
        }
        animal->_pos = DIE;
        return animal;
    }
    return nullptr;
}

#endif