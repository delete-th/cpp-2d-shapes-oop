#ifndef CROSS_H
#define CROSS_H

#include "ShapeTwoD.h"
#include "point.h"

#pragma once

#include <set>      // for std::string
#include <utility>      // for std::vector


class Cross : public ShapeTwoD {
private:
    Point vertices[12];

 
public:
    Cross(std::string name, bool containsWarpSpace, Point points[12]);

    double computeArea() override;
    bool isPointInShape(int x, int y) override;
    bool isPointOnShape(int x, int y) override;
    
    std::vector<Point> orderVertices
		(const std::vector<Point>& vertices) override;
    
    std::vector<Point> buildPerimeter(const std::vector<Point>& 
    			orderedVertices, const std::vector<Point>& box) override;

    std::vector<Point> getVertices() const override;
};

#endif

