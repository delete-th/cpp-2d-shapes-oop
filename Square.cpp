#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "Square.h"
#include "point.h"

using namespace std;

// constructor
Square::Square(string name, bool containsWarpSpace, Point points[4])
    : ShapeTwoD(name, containsWarpSpace) {
    for (int i = 0; i < 4; ++i)
        vertices[i] = points[i];
}

double Square::computeArea() {
    double side = 0.0;

    // find a pair of adjacent vertices with different x or y
    for (int i = 1; i < 4; ++i) {
        if (vertices[0].x != vertices[i].x) {
            side = std::abs(vertices[0].x - vertices[i].x);
            break;
        } else if (vertices[0].y != vertices[i].y) {
            side = std::abs(vertices[0].y - vertices[i].y);
            break;
        }
    }

    area = side * side;
    return area;
}

// ====================
std::vector<Point> Square::orderVertices(const std::vector<Point>& vertices) {
    if (vertices.size() != 4) {
        std::cerr << "Error: Square should have 4 vertices.\n";
        return {};
    }

    std::vector<Point> ordered(4);
    
    // Step 1: sort points to find bottom-left
    auto compare = [](const Point& a, const Point& b) {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    };

    Point bottomLeft = *std::min_element(vertices.begin(), 
      									 vertices.end(), compare);

    // Step 2: identify other corners
    std::vector<Point> copy = vertices;
    copy.erase(std::remove(copy.begin(), 
               copy.end(), bottomLeft), copy.end());

    Point topLeft, topRight, bottomRight;

    for (const Point& p : copy) {
        if (p.x == bottomLeft.x) topLeft = p;
        else if (p.y == bottomLeft.y) bottomRight = p;
        else topRight = p;
    }

    // Clockwise order: bottomLeft -> topLeft -> topRight -> bottomRight
    ordered[0] = bottomLeft;
    ordered[1] = topLeft;
    ordered[2] = topRight;
    ordered[3] = bottomRight;

    return ordered;
}


std::vector<Point> Square::buildPerimeter(const std::vector<Point>& 
									      orderedVertices,
                                          const std::vector<Point>& box)
{
    std::vector<Point> onPerimeter;

    for (size_t i = 0; i < orderedVertices.size(); ++i) {
        Point start = orderedVertices[i];
        Point end = orderedVertices[(i + 1) % orderedVertices.size()]; // wrap around

        if (start.x == end.x) {
            // vertical edge
            int yStart = std::min(start.y, end.y);
            int yEnd = std::max(start.y, end.y);
            for (int y = yStart; y <= yEnd; ++y) {
                onPerimeter.push_back({start.x, y});
            }
        } else if (start.y == end.y) {
            // horizontal edge
            int xStart = std::min(start.x, end.x);
            int xEnd = std::max(start.x, end.x);
            for (int x = xStart; x <= xEnd; ++x) {
                onPerimeter.push_back({x, start.y});
            }
        }
    }

    return onPerimeter;
}



bool Square::isPointOnShape(int x, int y) {

    auto rawVertices = getVertices();
    auto ordered = orderVertices(rawVertices);
    auto bbox = buildBoundingBox();
    auto onPerimeter = buildPerimeter(ordered, bbox);

    for (const auto& pt : onPerimeter) {
        if (pt.x == x && pt.y == y) return true;
    }
    return false;
}

bool Square::isPointInShape(int x, int y) {
    auto rawVertices = getVertices();
    auto ordered = orderVertices(rawVertices);
    auto bbox = buildBoundingBox();
    auto onPerimeter = buildPerimeter(ordered, bbox);

    // If the point is on the perimeter, it's not considered inside
    for (const auto& pt : onPerimeter) {
        if (pt.x == x && pt.y == y)
            return false;
    }

    // If the point is within the bounding box (but not on perimeter), it's inside
    for (const auto& pt : bbox) {
        if (pt.x == x && pt.y == y)
            return true;
    }

    // Otherwise, the point is outside the shape
    return false;
}



vector<Point> Square::getVertices() const {
    return vector<Point>(vertices, vertices + 4);
}
