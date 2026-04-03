#ifndef POINT_H
#define POINT_H

// struct for that describes point coordinates

struct Point {
    int x, y;
    
    
  	Point(int x=0, int y=0): x(x), y(y) {}
  	

};

inline bool operator==(const Point& a, const Point& b) {
    return a.x == b.x && a.y == b.y;
}

inline bool operator!=(const Point& a, const Point& b) {
    return !(a == b);
}

#endif
