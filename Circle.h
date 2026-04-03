#ifndef CIRCLE_H
#define CIRCLE_H

#include "ShapeTwoD.h"
#include "point.h"


class Circle : public ShapeTwoD {
private:
    Point center;
    double radius;

public:
    Circle(std::string name, bool containsWarpSpace, Point center, double radius);

    double computeArea() override;
    bool isPointInShape(int x, int y) override;
    bool isPointOnShape(int x, int y) override;
    std::string toString() override;
    

    Point getCenter();
    double getRadius();
    
    std::vector<Point> getVertices() const override;
};

#endif

