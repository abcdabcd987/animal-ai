#ifndef helper_h
#define helper_h
#include <cstdio>

const int DIRS = 4;
const char DIR_CHAR[DIRS] = {'U', 'L', 'R', 'D'};

struct Point {
    short x, y;
    Point(): x(0), y(0) {}
    Point(const int X, const int Y): x(X), y(Y) {}
    Point& operator+=(const Point& rhs) {
        x += rhs.x, y += rhs.y;
        return *this;
    }
};

inline bool operator==(const Point& lhs, const Point& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator!=(const Point& lhs, const Point& rhs) {
    return lhs.x != rhs.x || lhs.y != rhs.y;
}

inline Point operator+(Point lhs, const Point& rhs) {
    return lhs += rhs;
}

struct Operation {
    Point source;
    union {
        int direction;
        Point target;
    };
    Operation(): source(), direction(0) {}
    Operation(const Point& src, const int dir): source(src), direction(dir) {}
    Operation(const Point& src, const Point& tar): source(src), target(tar) {}
    void print() const {
        fprintf(stderr, "(%d, %d) %c\n", source.x, source.y, DIR_CHAR[direction]);
    }
};

inline bool operator!=(const Operation& lhs, const Operation& rhs) {
    return lhs.source != rhs.source || lhs.direction != rhs.direction || lhs.target != rhs.target;
}

const Point DIR1[DIRS] = {Point(1, 0), Point(0, -1), Point(0, 1), Point(-1, 0)};
const Point DIR2[DIRS] = {Point(4, 0), Point(0, -3), Point(0, 3), Point(-4, 0)};
const int ROWS = 9;
const int COLS = 7;
const int ANIMALS = 8;
const Operation INVALID_OPERATION(Point(10086, 10086), 0);

#endif
