#include "ai.h"
#include <vector>
#include <cstdlib>
#include <algorithm>

const std::string AI::_name = "β - Stupid";

Operation AI::_try_eat() const {
    Operation op;
    Animal* toEat = nullptr;
    for (int i = 0; i < ANIMALS; ++i) {
        Animal *animal = _board.friendAnimal(i);
        if (!animal)
            continue;
        for (int d = 0; d < DIRS; ++d) {
            const Point tar(animal->canMove(d));
            if (tar == Animal::INVALID_MOVE)
                continue;
            if (_board.landType(tar) == Board::ENEMY_CAVE) {
                return Operation(animal->position(), d);
            }
            Animal *target = _board.animal(tar);
            if (!target || !animal->canEat(target))
                continue;
            if (!toEat || target->type() > toEat->type()) {
                toEat = target;
                op.source = animal->position();
                op.direction = d;
            }
        }
    }
    return toEat ? op : INVALID_OPERATION;
}

Operation AI::_try_init() {
    if (init.finish)
        return INVALID_OPERATION;
    if (init.lion == 0) {
        ++init.lion;
        return Operation(_board.friendAnimal(Animal::LION)->position(), 0);
    }
    if (!init.elephant) {
        init.elephant = true;
        return Operation(_board.friendAnimal(Animal::ELEPHANT)->position(), 2);
    }
    if (init.tiger == 0) {
        ++init.tiger;
        return Operation(_board.friendAnimal(Animal::TIGER)->position(), 0);
    }
    if (!init.mouse) {
        init.mouse = true;
        return Operation(_board.friendAnimal(Animal::MOUSE)->position(), 0);
    }
    if (init.lion != 2) {
        ++init.lion;
        return Operation(_board.friendAnimal(Animal::LION)->position(), 0);
    }
    if (init.tiger != 2) {
        ++init.tiger;
        return Operation(_board.friendAnimal(Animal::TIGER)->position(), 0);
    }
    if (!init.cat) {
        init.cat = true;
        return Operation(_board.friendAnimal(Animal::CAT)->position(), 3);
    }
    if (!init.dog) {
        init.dog = true;
        return Operation(_board.friendAnimal(Animal::DOG)->position(), 3);
    }
    if (!init.wolf) {
        init.wolf = true;
        return Operation(_board.friendAnimal(Animal::WOLF)->position(), 3);
    }
    init.finish = true;
    return INVALID_OPERATION;
}

int AI::_evaluateFriend() const {
    static const int animalRate[8] = { 6, 1, 2, 3, 8, 12, 15, 14 };
    int dist = 15;
    for (int i = 0; i < ANIMALS; ++i) {
        Animal *animal = _board.friendAnimal(i);
        if (animal)
            dist = std::min(dist, animal->manhattanDistance(Board::ENEMY_CAVE_POS));
    }
    
    int ani = 0;
    for (int i = 0; i < ANIMALS; ++i) {
        Animal *animal = _board.enemyAnimal(i);
        if (animal)
            ani += animalRate[i];
    }
    
    return dist * 7 + ani * 3;
}

const int AI::searchOrder[8] = { 6, 5, 1, 7, 4, 3, 2, 0 };

Operation AI::_move_lion_tiger() {
    static const int kind[2] = { Animal::LION, Animal::TIGER };
    Operation op(INVALID_OPERATION);
    int eval = _evaluateFriend();
    bool hasBetter = false;
    for (int i = 0; i < 2; ++i) {
        Animal* animal = _board.friendAnimal(kind[i]);
        if (!animal)
            continue;
        const Point oldpos = animal->position();
        for (int d = 0; d < 4; ++d) {
            const Point pos = animal->canMove(d);
            if (pos == Animal::INVALID_MOVE)
                continue;
            
            Animal* target = animal->move(pos);
            const int new_eval = _evaluateFriend();
            if (new_eval < eval) {
                eval = new_eval;
                op = Operation(oldpos, d);
                hasBetter = true;
            }
            //if (!hasBetter)
            //    op = Operation(oldpos, d);
            
            if (target) {
                _board.enemyAnimal(target->type(), target);
                target->position(pos);
            }
            _board.animal(pos, target);
            _board.animal(oldpos, animal);
            _board.friendAnimal(animal->type(), animal);
            animal->position(oldpos);
        }
    }
    return op;
}

Operation AI::_try_random() const {
    std::vector<Animal*> v;
    for (int i = 0; i < ANIMALS; ++i) {
        Animal *animal = _board.friendAnimal(i);
        if (animal)
            v.push_back(animal);
    }
    
    while (!v.empty()) {
        const unsigned index = rand()%v.size();
        Animal *animal = v[index];
        for (int d = 0; d < DIRS; ++d) {
            const Point target(animal->canMove(d));
            if (target != Animal::INVALID_MOVE) {
                const Operation op(animal->position(), d);
                return op;
            }
        }
        v.erase(v.begin()+index);
    }
    return INVALID_OPERATION;
}

Operation AI::makeDecision() {
    Operation op;
    
    op = _try_eat();
    if (op != INVALID_OPERATION) {
        fprintf(stderr, "Step %d: _try_eat\n", _steps);
        op.print();
        return op;
    }
    
    op = _try_init();
    if (op != INVALID_OPERATION) {
        fprintf(stderr, "Step %d: _try_init\n", _steps);
        op.print();
        return op;
    }
    
    op = _move_lion_tiger();
    if (op != INVALID_OPERATION) {
        fprintf(stderr, "Step %d: _move_lion_tiger\n", _steps);
        op.print();
        return op;
    }
    
    op = _try_random();
    if (op != INVALID_OPERATION) {
        fprintf(stderr, "Step %d: _try_random\n", _steps);
        op.print();
        return op;
    }
    
    fprintf(stderr, "Step %d: invalid\n", _steps);
    return INVALID_OPERATION;
}

void AI::_print_map() const {
    static const char* mapper[2] = {"mcwdptle", "MCWDPTLE"};
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            Animal* animal = _board.animal(Point(row, col));
            if (!animal) {
                fprintf(stderr, ".");
            } else {
                fprintf(stderr, "%c", mapper[animal->isFriend()][animal->type()]);
            }
        }
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n");
}

void AI::update(const int id, const Point &src, const Point &tar) {
    Animal* source = _board.animal(src);
    Animal* target = source->move(tar);
    if (target)
        delete target;
    _print_map();
    ++_steps;
}