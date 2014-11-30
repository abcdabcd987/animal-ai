#ifndef ai_h
#define ai_h

#include "helper.h"
#include "board.h"
#include "animal.h"
#include <string>

class AI {
    int _id;
    Board _board;
    static const std::string _name;
    
public:
    AI(const int id): _id(id), _board() {}
    const std::string& name() const { return _name; }
    Operation makeDecision();
    void update(const int id, const Point& src, const Point& tar);
};

#endif
