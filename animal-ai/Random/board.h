#ifndef board_h
#define board_h

#include "helper.h"

class Animal;

class Board
{
public:
    enum LandType { NORMAL, WATER, FRIEND_TRAP, ENEMY_TRAP, FRIEND_CAVE, ENEMY_CAVE };
    static const LandType land[ROWS][COLS];
    static bool inBoard(const Point& pos) {
        return 0 <= pos.x && pos.x < ROWS && 0 <= pos.y && pos.y < COLS;
    }
    static LandType landType(const Point& pos) {
        return land[pos.x][pos.y];
    }
    
private:
    Animal* _animal[ROWS][COLS];
    Animal* _friend[ANIMALS];
    Animal* _enemy[ANIMALS];
    
public:
    Board();
    
    Animal* animal(const Point& pos) const { return _animal[pos.x][pos.y]; }
    Animal* friendAnimal(const int animal) const { return _friend[animal]; }
    Animal* enemyAnimal(const int animal) const { return _enemy[animal]; }
    void animal(const Point& pos, Animal* value) { _animal[pos.x][pos.y] = value; }
    void friendAnimal(const int animal, Animal* value) { _friend[animal] = value; }
    void enemyAnimal(const int animal, Animal* value) { _enemy[animal] = value; }
};

#endif