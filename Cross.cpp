#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <iostream> 



#include "Cross.h"
#include "point.h"

using namespace std;

// constructor
Cross::Cross(string name, bool containsWarpSpace, Point points[12])
    : ShapeTwoD(name, containsWarpSpace) {
    for (int i = 0; i < 12; ++i)
        vertices[i] = points[i];
}

// uses shoelace formula and the clockwise ordered vertices to compute area
static double computeShoelaceArea(const std::vector<Point>& pts) {
    double area = 0.0;
    int n = pts.size();

    for (int i = 0; i < n; ++i) {
        const Point& p1 = pts[i];
        const Point& p2 = pts[(i + 1) % n];
        area += (p1.x * p2.y - p2.x * p1.y);
    }

    return std::abs(area) / 2.0;
}

// Finds the index of the next vertex point in a specific direction from the current point.
// The direction is specified by whether the movement is vertical/horizontal and ascending/descending.
// Only unused points (i.e., not yet visited) are considered.
static int findNextPoint(const std::vector<Point>& pts, const Point& current,
                  bool vertical, bool ascending,
                  const std::vector<bool>& used) {
    int idx = -1; // Default to -1 if no valid next point is found
    if (vertical) {
        // Looking for a point vertically aligned with the current point (same x)
        if (ascending) {
            int minY = std::numeric_limits<int>::max();
            for (int i = 0; i < (int)pts.size(); ++i) {
                if (!used[i] && pts[i].x == current.x && pts[i].y 
                    > current.y && pts[i].y < minY) {
                    minY = pts[i].y;
                    idx = i;
                }
            }
        } else {
        	// Find the closest unused point below the current point (lower y)
            int maxY = std::numeric_limits<int>::min();
            for (int i = 0; i < (int)pts.size(); ++i) {
                if (!used[i] && pts[i].x == current.x && pts[i].y 
                    < current.y && pts[i].y > maxY) {
                    maxY = pts[i].y;
                    idx = i;
                }
            }
        }
    } else {
         // Looking for a point horizontally aligned with 
         // the current point (same y)
         
        if (ascending) {
            int minX = std::numeric_limits<int>::max();
            for (int i = 0; i < (int)pts.size(); ++i) {
                if (!used[i] && pts[i].y == current.y && pts[i].x 
                    > current.x && pts[i].x < minX) {
                    minX = pts[i].x;
                    idx = i;
                }
            }
        } else {
        	// Find the closest unused point to the left (lower x)
        	
            int maxX = std::numeric_limits<int>::min();
            for (int i = 0; i < (int)pts.size(); ++i) {
                if (!used[i] && pts[i].y == current.y && pts[i].x 
                    < current.x && pts[i].x > maxX) {
                    maxX = pts[i].x;
                    idx = i;
                }
            }
        }
    }
    // Return the index of the chosen point, or -1 if none found
    return idx;
}


// Orders the 12 vertices of a cross shape in a consistent traversal sequence
// so that the resulting polygon can be used for area and boundary calculations.
std::vector<Point> Cross::orderVertices(const std::vector<Point>& vertices) {
	
    if (vertices.size() != 12) {
        std::cerr << "Error: Expected 12 vertices.\n";
        return {};
    }

	// Track which points have been used in the ordered list
    std::vector<bool> used(12, false);
    std::vector<Point> ordered;

    // Step 1: Find starting point = lowest x with lowest y at that x
    int startIdx = 0;
    int minX = std::numeric_limits<int>::max();
    int minY = std::numeric_limits<int>::max();
    for (int i = 0; i < 12; ++i) {
        if (vertices[i].x < minX || (vertices[i].x == minX && vertices[i].y < minY)) {
            minX = vertices[i].x;
            minY = vertices[i].y;
            startIdx = i;
        }
    }
    
	// Add the starting point to the ordered list
    ordered.push_back(vertices[startIdx]);
    used[startIdx] = true;
	
	// Step 2: Follow a hardcoded traversal pattern for cross shapes
    // The cross consists of 12 vertices and 11 edges forming a loop
    // Each step is defined by (isVertical, isAscending)
    bool directions[11][2] = {
        {true, true},    // vertical asc
        {false, true},   // horizontal asc
        {true, true},    // vertical asc
        {false, true},   // horizontal asc
        {true, false},   // vertical desc
        {false, true},   // horizontal asc
        {true, false},   // vertical desc
        {false, false},  // horizontal desc
        {true, false},   // vertical desc
        {false, false},  // horizontal desc
        {true, true}     // vertical asc (to close)
    };

	// Step 3: Traverse through the directions to collect all 12 points in order
    for (int step = 0; step < 11; ++step) {
        const Point& current = ordered.back();
        
        // Find the next point along the specified axis and direction
        int nextIdx = findNextPoint(vertices, current, directions[step][0], directions[step][1], used);
        if (nextIdx == -1) {
            std::cerr << "Failed to find next point at step " << step << "\n";
            return {};
        }
        ordered.push_back(vertices[nextIdx]);
        used[nextIdx] = true;
    }

	 // Return the full ordered list of vertices
    return ordered;
}

// computeArea function for the Cross shape
double Cross::computeArea() {
	auto rawVertices = getVertices();
    auto ordered = orderVertices(rawVertices);


    return computeShoelaceArea(ordered);
}



// === for inShape and onShape point computation ===
static bool pointInPolygon(int x, int y, const std::vector<Point>& ordered) {
    int count = 0;	// Number of times a ray from the point crosses polygon edges
    int n = ordered.size();	// Total number of vertices in the polygon

    for (int i = 0; i < n; ++i) {
        Point a = ordered[i];
        Point b = ordered[(i + 1) % n];

        // Check if the edge (a,b) crosses the horizontal ray extending to the 
        //right from (x, y)
        // This occurs when the y-coordinate of the point is strictly between a.y 
        //and b.y
        if ((a.y <= y && b.y > y) || (b.y <= y && a.y > y)) {
            // Compute x-intersection
            float xIntersect = a.x + (float)(y - a.y) * (b.x - a.x) / (b.y - a.y);
            if (xIntersect > x)
                count++;
        }
    }

    // Inside if odd number of crossings
    return count % 2 == 1;
}


bool Cross::isPointInShape(int x, int y) {
	auto rawVertices = getVertices();
	auto ordered = orderVertices(rawVertices);
	auto box = buildBoundingBox();
	auto onPerimeter = buildPerimeter(ordered, box);

    // Exclude perimeter
    for (const auto& pt : onPerimeter) {
        if (pt.x == x && pt.y == y)
            return false;
    }

    // Must be inside polygon
    return pointInPolygon(x, y, ordered);
}


 // Checks if point is on line segment between a and b (axis aligned)
static bool isOnEdge(const Point& p, const Point& a, const Point& b) {
    if (a.x == b.x) { // vertical edge
        if (p.x == a.x) {
            int minY = std::min(a.y, b.y);
            int maxY = std::max(a.y, b.y);
            return p.y >= minY && p.y <= maxY;
        }
    } else if (a.y == b.y) { // horizontal edge
        if (p.y == a.y) {
            int minX = std::min(a.x, b.x);
            int maxX = std::max(a.x, b.x);
            return p.x >= minX && p.x <= maxX;
        }
    }
    return false;
}


// Constructs the full list of perimeter points along the edges of the cross shape by extracting and ordering relevant points from the bounding box.
std::vector<Point> Cross::buildPerimeter(const std::vector<Point>& orderedVertices, const std::vector<Point>& box) {
    std::vector<Point> perimeterPoints;

    int n = (int)orderedVertices.size();
    for (int i = 0; i < n; ++i) {
        const Point& a = orderedVertices[i];
        const Point& b = orderedVertices[(i + 1) % n];

        // Collect points on edge from bounding box, including endpoints
        std::vector<Point> edgePoints;

        for (const Point& p : box) {
            if (isOnEdge(p, a, b)) {
                edgePoints.push_back(p);
            }
        }

        // Sort edge points along edge direction (ascending)
        if (a.x == b.x) {
            // vertical edge, sort by y ascending
            std::sort(edgePoints.begin(), edgePoints.end(), 
                      [](const Point& p1, const Point& p2) {
                return p1.y < p2.y;
            });
        } else {
            // horizontal edge, sort by x ascending
            std::sort(edgePoints.begin(), edgePoints.end(), 
                      [](const Point& p1, const Point& p2) {
                return p1.x < p2.x;
            });
        }

        // Avoid duplicating the first point of the edge except for the first edge
        if (!perimeterPoints.empty() && !edgePoints.empty()) {
            if (perimeterPoints.back().x == edgePoints.front().x && 
                perimeterPoints.back().y == edgePoints.front().y) {
                edgePoints.erase(edgePoints.begin());
            }
        }

        perimeterPoints.insert(perimeterPoints.end(), 
        					   edgePoints.begin(), edgePoints.end());
    }

    return perimeterPoints;
}


bool Cross::isPointOnShape(int x, int y) {
	auto rawVertices = getVertices();
	auto ordered = orderVertices(rawVertices);
	auto box = buildBoundingBox();
	auto perimeterPoints = buildPerimeter(ordered, box);
	
    for (const auto& p : perimeterPoints) {
        if (p.x == x && p.y == y) {
            return true;
        }
    }
    return false;
}


// copies vertices in array into vector
vector<Point> Cross::getVertices() const {
    return vector<Point>(vertices, vertices + 12);
}


