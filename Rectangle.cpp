#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iostream>

#include "Rectangle.h"
#include "point.h"

using namespace std;

Rectangle::Rectangle(string name, bool containsWarpSpace, Point points[4])
    : ShapeTwoD(name, containsWarpSpace) {
    for (int i = 0; i < 4; ++i)
        vertices[i] = points[i];
}

double Rectangle::computeArea() {
    // Initialize min and max with first vertex
    double minX = vertices[0].x;
    double maxX = vertices[0].x;
    double minY = vertices[0].y;
    double maxY = vertices[0].y;

    // Find min and max x and y among all vertices
    for (int i = 1; i < 4; ++i) {
        if (vertices[i].x < minX) minX = vertices[i].x;
        if (vertices[i].x > maxX) maxX = vertices[i].x;
        if (vertices[i].y < minY) minY = vertices[i].y;
        if (vertices[i].y > maxY) maxY = vertices[i].y;
    }

    double width = maxX - minX;
    double height = maxY - minY;

    area = width * height;
    return area;
}

// =====================================
std::vector<Point> Rectangle::orderVertices(const std::vector<Point>& vertices) {
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


std::vector<Point> Rectangle::buildPerimeter(const std::vector<Point>& 
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


bool Rectangle::isPointOnShape(int x, int y) {

    auto rawVertices = getVertices();
    auto ordered = orderVertices(rawVertices);
    auto bbox = buildBoundingBox();
    auto onPerimeter = buildPerimeter(ordered, bbox);

    for (const auto& pt : onPerimeter) {
        if (pt.x == x && pt.y == y) return true;
    }
    return false;
}

bool Rectangle::isPointInShape(int x, int y) {
    auto rawVertices = getVertices();
    auto ordered = orderVertices(rawVertices);
    auto bbox = buildBoundingBox();
    // Get the list of points on the perimeter
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





vector<Point> Rectangle::getVertices() const {
    return vector<Point>(vertices, vertices + 4);
}

