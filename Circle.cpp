#include <sstream>
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

#include "Circle.h"
#include "point.h"

using namespace std;

Circle::Circle(string name, bool containsWarpSpace, Point center, double radius)
    : ShapeTwoD(name, containsWarpSpace), center(center), radius(radius) {}

double Circle::computeArea() {
    area = M_PI * radius * radius;
    return area;
}


bool Circle::isPointOnShape(int x, int y) {
    return
        (x == center.x && y == center.y + radius) ||  // North
        (x == center.x && y == center.y - radius) ||  // South
        (x == center.x + radius && y == center.y) ||  // East
        (x == center.x - radius && y == center.y);    // West
}


bool Circle::isPointInShape(int x, int y) {
    int dx = x - center.x;
    int dy = y - center.y;
    double dist = std::sqrt(dx*dx + dy*dy);

    return dist < radius;
}


Point Circle::getCenter() {
    return center;
}

double Circle::getRadius() {
    return radius;
}

string Circle::toString() {
    stringstream ss;
    ss << "Name : " << name << "\n";
    ss << "Special Type : " << (containsWarpSpace ? "WS" : "NS") << "\n";
    ss << "Area : " << fixed << setprecision(2) << getArea()
                    << " units square" << "\n";
    ss << "Center : (" << center.x << "," << center.y << ")\n";
    ss << "Radius : " << radius << "\n";
    return ss.str();
}

// return north, east, west, south points of circle as vertices
// this function is for buildBoundingBox();
vector<Point> Circle::getVertices() const {
    // Return approximate 4-point bounding box (or empty)
    vector<Point> approx;
	approx.push_back({static_cast<int>(center.x - radius), center.y});
	approx.push_back({static_cast<int>(center.x + radius), center.y});
	approx.push_back({center.x, static_cast<int>(center.y - radius)});
	approx.push_back({center.x, static_cast<int>(center.y + radius)});

    return approx;
}


