#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <numeric>
#include <limits>

#include "point.h"
#include "ShapeTwoD.h"

using namespace std;

// filename: ShapeTwoD.cpp
// content: ShapeTwoD method implementations

// constructor
ShapeTwoD::ShapeTwoD(string name, bool containsWarpSpace)
{
	this->name = name;
	this->containsWarpSpace = containsWarpSpace;
}

// destructor
ShapeTwoD::~ShapeTwoD() { }


// getters
string ShapeTwoD::getName()
{
	return name;
}

bool ShapeTwoD::getContainsWarpSpace()
{
	return containsWarpSpace;
}

// to string
string ShapeTwoD::toString() {
    stringstream ss;
    ss << "Name : " << name << "\n";
    ss << "Special Type : " << (containsWarpSpace ? "WS" : "NS") << "\n";
    ss << "Area : " << getArea() << " units square" << "\n";
    ss << "Vertices :\n";

    vector<Point> vertices = getVertices();
    for (size_t i = 0; i < vertices.size(); ++i) {
        ss << "Point [" << i << "] : (" << vertices[i].x << "," << vertices[i].y << ")\n";
    }

    return ss.str();
}

// methods  
double ShapeTwoD::computeArea() { return 0.0; }
bool ShapeTwoD::isPointInShape(int x, int y) { return false; }
bool ShapeTwoD::isPointOnShape(int x, int y) { return false; }


// setters  	
void ShapeTwoD::setName(string name)
{
	this->name = name;
}

void ShapeTwoD::setContainsWarpSpace(bool containsWarpSpace)
{
	this->containsWarpSpace = containsWarpSpace;
}

// extra methods
// gets max x and max y of a shape, builds a box, and gets all the 
// points from that box, stores into a vector
std::vector<Point> ShapeTwoD::buildBoundingBox() const {
	const auto& vertices = getVertices();
    int minX = std::numeric_limits<int>::max();
    int maxX = std::numeric_limits<int>::min();
    int minY = std::numeric_limits<int>::max();
    int maxY = std::numeric_limits<int>::min();

    for (const auto& p : vertices) {
    	if (p.x < minX) minX = p.x;
        if (p.x > maxX) maxX = p.x;
        if (p.y < minY) minY = p.y;
        if (p.y > maxY) maxY = p.y;
    }

    std::vector<Point> box;
    for (int x = minX; x <= maxX; ++x) {
    	for (int y = minY; y <= maxY; ++y) {
        	box.push_back({x, y});
        }
    }
        return box;
    }

std::vector<Point> ShapeTwoD::orderVertices(const std::vector<Point>& vertices){
    // Placeholder or base behavior
    return vertices;
}

std::vector<Point> ShapeTwoD::buildPerimeter(const std::vector<Point>& ordered, const std::vector<Point>& box) {
    // Placeholder or base behavior
    return {};
}

