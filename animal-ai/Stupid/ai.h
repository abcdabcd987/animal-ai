#ifndef ai_h
#define ai_h

#include "helper.h"
#include "board.h"
#include "animal.h"
#include <string>

class AI {
    int _id;
    int _steps = 1;
    Board _board;
    static const std::string _name;
    
    void _print_map() const;
    
    struct {
        bool finish = false;
        int lion = 0;
        int tiger = 0;
        bool mouse = false;
        bool cat = false;
        bool dog = false;
        bool wolf = false;
        bool elephant = false;
    } init;
    
    int _evaluateFriend() const;
    Operation _try_eat() const;
    Operation _try_init();
    Operation _move_lion_tiger();
    Operation _try_random() const;
    
    static const int searchOrder[8];
    
public:
    AI(const int id): _id(id), _board() { _print_map(); }
    const std::string& name() const { return _name; }
    Operation makeDecision();
    void update(const int id, const Point& src, const Point& tar);
};

#endif
