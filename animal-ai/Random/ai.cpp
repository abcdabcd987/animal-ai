#include "ai.h"
#include <vector>
#include <cstdlib>

const std::string AI::_name = "α - Rad0m";

Operation AI::makeDecision() {
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
            Animal *target = _board.animal(tar);
            if (!target)
                continue;
            if (!toEat || target->type() > toEat->type()) {
                toEat = target;
                op.source = animal->position();
                op.direction = d;
            }
        }
    }
    
    if (toEat)
        return op;
    
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

void AI::update(const int id, const Point &src, const Point &tar) {
    Animal* source = _board.animal(src);
    Animal* target = source->move(tar);
    if (target)
        delete target;
}