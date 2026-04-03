#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "ShapeTwoD.h"
#include "point.h"


class Rectangle : public ShapeTwoD {
private:
    Point vertices[4];

public:
    Rectangle(std::string name, bool containsWarpSpace, Point points[4]);

    double computeArea() override;
    bool isPointInShape(int x, int y) override;
    bool isPointOnShape(int x, int y) override;

 	std::vector<Point> getVertices() const override;
 	
 	std::vector<Point> orderVertices
		(const std::vector<Point>& vertices) override;
		
    std::vector<Point> buildPerimeter
		(const std::vector<Point>& orderedVertices,
         const std::vector<Point>& box) override;
};

#endif

