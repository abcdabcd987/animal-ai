#include "board.h"
#include "animal.h"

const Board::LandType Board::land[ROWS][COLS] = {
    {NORMAL, NORMAL, FRIEND_TRAP, FRIEND_CAVE, FRIEND_TRAP, NORMAL, NORMAL},
    {NORMAL, NORMAL, NORMAL     , FRIEND_TRAP, NORMAL     , NORMAL, NORMAL},
    {NORMAL, NORMAL, NORMAL     , NORMAL     , NORMAL     , NORMAL, NORMAL},
    {NORMAL, WATER , WATER      , NORMAL     , WATER      , WATER , NORMAL},
    {NORMAL, WATER , WATER      , NORMAL     , WATER      , WATER , NORMAL},
    {NORMAL, WATER , WATER      , NORMAL     , WATER      , WATER , NORMAL},
    {NORMAL, NORMAL, NORMAL     , NORMAL     , NORMAL     , NORMAL, NORMAL},
    {NORMAL, NORMAL, NORMAL     , ENEMY_TRAP , NORMAL     , NORMAL, NORMAL},
    {NORMAL, NORMAL, ENEMY_TRAP , ENEMY_CAVE , ENEMY_TRAP , NORMAL, NORMAL}
};

Board::Board() {
    for (int row = 0; row < ROWS; ++row)
        for (int col = 0; col < COLS; ++col)
            _animal[row][col] = nullptr;
    
    _friend[Animal::TIGER   ] = new Animal(this, 0, 0, Animal::TIGER   , true);
    _friend[Animal::LION    ] = new Animal(this, 0, 6, Animal::LION    , true);
    _friend[Animal::CAT     ] = new Animal(this, 1, 1, Animal::CAT     , true);
    _friend[Animal::DOG     ] = new Animal(this, 1, 5, Animal::DOG     , true);
    _friend[Animal::ELEPHANT] = new Animal(this, 2, 0, Animal::ELEPHANT, true);
    _friend[Animal::WOLF    ] = new Animal(this, 2, 2, Animal::WOLF    , true);
    _friend[Animal::LEOPARD ] = new Animal(this, 2, 4, Animal::LEOPARD , true);
    _friend[Animal::MOUSE   ] = new Animal(this, 2, 6, Animal::MOUSE   , true);
    
    _enemy[Animal::TIGER   ] = new Animal(this, 8, 6, Animal::TIGER   , false);
    _enemy[Animal::LION    ] = new Animal(this, 8, 0, Animal::LION    , false);
    _enemy[Animal::CAT     ] = new Animal(this, 7, 5, Animal::CAT     , false);
    _enemy[Animal::DOG     ] = new Animal(this, 7, 1, Animal::DOG     , false);
    _enemy[Animal::ELEPHANT] = new Animal(this, 6, 6, Animal::ELEPHANT, false);
    _enemy[Animal::WOLF    ] = new Animal(this, 6, 4, Animal::WOLF    , false);
    _enemy[Animal::LEOPARD ] = new Animal(this, 6, 2, Animal::LEOPARD , false);
    _enemy[Animal::MOUSE   ] = new Animal(this, 6, 0, Animal::MOUSE   , false);

    for (int i = 0; i < ANIMALS; ++i) {
        _animal[_friend[i]->x()][_friend[i]->y()] = _friend[i];
        _animal[_enemy[i]->x()][_enemy[i]->y()] = _enemy[i];
    }
}
