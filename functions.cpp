#include <iostream>
#include <string>
#include <fstream> 
#include <vector>     // used for temporary storages
#include <algorithm>  // for transform() to convert input to lowercase
#include <iomanip>    // used for setting decimal places
#include <cctype>     // for ::tolower character conversion
#include <cmath>      // for abs()
#include <limits>


#include "functions.h"
#include "point.h"
#include "ShapeTwoD.h"  // base class
#include "Circle.h"     // Circle subclass
#include "Square.h"     // Square subclass
#include "Rectangle.h"  // Rectangle subclass
#include "Cross.h"      // Cross subclass

using namespace std;

// Full Name: Thea Gardiola Saguid
// UOW Number : 8691836
// Assignment 2

// some function prototypes
void promptUser();
void exitProgram();

// displays Menu
// simple print statements
void displayMenu()
{
	cout << "Student ID: 8691836" << endl;
	cout << "Student Name: Thea Gardiola Saguid" << endl;
	cout << "------------------------------------" << endl;
	cout << "Welcome to Assn2 program!" << endl;
	cout << endl;
	
	cout << "1)   Input sensor data" << endl;
	cout << "2)   Compute area (for all records)" << endl;
	cout << "3)   Print shapes report" << endl;
	cout << "4)   Sort shape data" << endl;
	cout << "5)   Quit" << endl;

	cout << endl;
	cout << "Please enter your choice: ";

}


// ==== some global variables ====
const int maxShapes = 100;  // max number of shapes input
ShapeTwoD* shapeStorage[maxShapes]; // global array storage
int shapeCount = 0; // shape counter

// == test cases ==
/*
void initializeTestShapes() {

    // Circle
    shapeStorage[shapeCount++] = new Circle("Circle", false, Point{3, 3}, 2);

    // Rectangle 1
    Point rect1[4] = { Point{6, 4}, Point{6, 1}, Point{11, 1}, Point{11, 4} };
    shapeStorage[shapeCount++] = new Rectangle("Rectangle", false, rect1);

    // Rectangle 2
    Point rect2[4] = { Point{13, 4}, Point{13, 1}, Point{12, 4}, Point{12, 1} };
    shapeStorage[shapeCount++] = new Rectangle("Rectangle", false, rect2);

    // Square
    Point square1[4] = { Point{4, 9}, Point{4, 6}, Point{1, 9}, Point{1, 6} };
    shapeStorage[shapeCount++] = new Square("Square", true, square1);

	// Cross 1
	Point cross1[12] = {
		{5,7}, {5,8}, {6,6}, {6,7},
		{6,8}, {6,9}, {7,6}, {7,7},
		{7,8}, {7,9}, {8,7}, {8,8}
	};
	shapeStorage[shapeCount++] = new Cross("Cross", true, cross1);

	// Cross 2
	Point cross2[12] = {
		{8,7}, {8,9}, {14,7}, {14,9},
		{12,7}, {12,9}, {12,5}, {12,11},
		{10,5}, {10,7}, {10,9}, {10,11}
	};
	shapeStorage[shapeCount++] = new Cross("Cross", false, cross2);

	// Cross 3
	Point cross3[12] = {
		{1,12}, {1,14}, {2,14}, {2,15},
		{3,14}, {3,15}, {4,11}, {4,12},
		{5,11}, {5,12}, {6,12}, {6,14}
	};
	shapeStorage[shapeCount++] = new Cross("Cross", true, cross3);

	// Cross 4
	Point cross4[12] = {
		{3,5}, {3,6}, {3,8}, {3,11},
		{1,6}, {1,8}, {4,5}, {4,6},
		{5,8}, {5,11}, {6,6}, {6,8}
	};
	shapeStorage[shapeCount++] = new Cross("Cross", true, cross4);
	
		// Cross 5
	Point cross5[12] = {
		{7,14}, {7,15}, {8,15}, {8,17},
		{10,13}, {10,14}, {10,15}, {10,17},
		{11,13}, {11,14}, {12,14}, {12,15}
	};
	shapeStorage[shapeCount++] = new Cross("Cross", true, cross5);

}
*/



// helper function to trim and lowercase input
string sanitizeInput(string input) {
    // trim leading whitespace
    size_t start = input.find_first_not_of(" \t\n\r\f\v");
    if (start == string::npos)
        return ""; // input was all whitespace

    // trim trailing whitespace
    size_t end = input.find_last_not_of(" \t\n\r\f\v");
    input = input.substr(start, end - start + 1);

    // convert to lowercase
    transform(input.begin(), input.end(), input.begin(), ::tolower);
    return input;
}

// helper function to store shapes into shapeStorage
void storeShape(ShapeTwoD* shape) 
{
	if (shapeCount < maxShapes) {
		shapeStorage[shapeCount++] = shape;
	} else {
		cout << "Storage full. Cannot add more shapes." << endl;
		delete shape;
	}
}

// helper function to determine valid square
bool isValidSquare(Point vertices[4]) {
    // Copy points into vector for sorting
    std::vector<Point> pts(vertices, vertices + 4);

    // Sort by x ascending, then y ascending
    std::sort(pts.begin(), pts.end(), [](const Point& a, const Point& b) {
        return (a.x == b.x) ? (a.y < b.y) : (a.x < b.x);
    });

    // After sort:
    // pts[0] and pts[1] are the left two points (lowest x)
    // pts[2] and pts[3] are the right two points (highest x)

    // We need to distinguish top-left and bottom-left between pts[0] and pts[1]
    Point topLeft = (pts[0].y < pts[1].y) ? pts[0] : pts[1];
    Point bottomLeft = (pts[0].y < pts[1].y) ? pts[1] : pts[0];

    // Similarly for right points
    Point topRight = (pts[2].y < pts[3].y) ? pts[2] : pts[3];
    Point bottomRight = (pts[2].y < pts[3].y) ? pts[3] : pts[2];

    // Check horizontal sides lengths
    int topWidth = topRight.x - topLeft.x;
    int bottomWidth = bottomRight.x - bottomLeft.x;

    // Check vertical sides lengths
    int leftHeight = bottomLeft.y - topLeft.y;
    int rightHeight = bottomRight.y - topRight.y;

    // Check sides positive length
    if (topWidth <= 0 || leftHeight <= 0)
        return false;

    // Check opposite sides equal
    if (topWidth != bottomWidth || leftHeight != rightHeight)
        return false;

    // Check all sides equal (square)
    if (topWidth != leftHeight)
        return false;

    // Check points form right angles by verifying horizontal and vertical alignments
    if (topLeft.y != topRight.y) return false;
    if (bottomLeft.y != bottomRight.y) return false;
    if (topLeft.x != bottomLeft.x) return false;
    if (topRight.x != bottomRight.x) return false;

    return true;
}


void option1 () {
	
    string inputName;
    string shapeType;
    vector<Point> tempVertices;
    int x, y;
    double radius = 0.0;
    Point center;

    // ask for shape name
    while (true) {
        cout << "Please enter name of shape: ";
        getline(cin, inputName);
        inputName = sanitizeInput(inputName);

		if (inputName.empty()) {
		    cout << "Input cannot be empty. Please try again.\n";
		    continue;
		}

        if (inputName == "circle" || inputName == "square" ||
            inputName == "rectangle" || inputName == "cross") {
            break;
        } else {
            cout << "Invalid shape. Please enter: circle, square, rectangle, or cross.\n";
            cout << endl;
        }
    }

    // ask for special type
    cout << "Please enter special type: ";
	getline(cin, shapeType);
	shapeType = sanitizeInput(shapeType);

	while (shapeType != "ws" && shapeType != "ns") {
		cout << "This is not a valid special type. (Input NS or WS)" << endl;
		cout << "Please enter special type: ";
		getline(cin, shapeType);
		shapeType = sanitizeInput(shapeType);
	}
	bool containsWarpSpace = (shapeType == "ws");

    // ask for shape-specific data
    cout << endl;
    if (inputName == "circle") {
        cout << "Please enter x-ordinate of center point: ";
        cin >> center.x;
        cin.ignore();
        cout << "Please enter y-ordinate of center point: ";
        cin >> center.y;
        cin.ignore();
        cout << endl;
        
        cout << "Please enter radius (units): ";
        cin >> radius;
        cin.ignore(); // clear newline left by cin
        cout << endl;

        Circle* c = new Circle("Circle", containsWarpSpace, center, radius);
        storeShape(c);
        
    } else {
        int vertexCount = 0;
        if (inputName == "square") vertexCount = 4;
        else if (inputName == "rectangle") vertexCount = 4;
        else if (inputName == "cross") vertexCount = 12;

        for (int i = 0; i < vertexCount; ++i) {
            Point p;
            cout << "Please enter x-ordinate of pt." << i + 1 << " : ";
            cin >> p.x;
            cin.ignore();
            cout << "Please enter y-ordinate of pt." << i + 1 << " : ";
            cin >> p.y;
            cin.ignore();
            tempVertices.push_back(p);
            cout << endl;
        }

		Point* vertexArray = new Point[tempVertices.size()];
			for (size_t i = 0; i < tempVertices.size(); ++i) {
   				vertexArray[i] = tempVertices[i];
			}
		
        if (inputName == "square") {
        	if (!isValidSquare(vertexArray)) {
        		cout << "Error: The entered points do not form a valid square." << endl;
        		cout << "Square unsuccessfully stored." << endl;
        		cout << endl; 
        		delete [] vertexArray;
        		return;
        	}
            Square* s = new Square("Square", containsWarpSpace, vertexArray);
            storeShape(s);
        } else if (inputName == "rectangle") {
            Rectangle* r = new Rectangle("Rectangle", containsWarpSpace, 	
            							 vertexArray);
            storeShape(r);
        } else if (inputName == "cross") {
            Cross* cr = new Cross("Cross", containsWarpSpace, vertexArray);
            storeShape(cr);
        }
    }
	
    cout << "Record successfully stored. Going back to main menu ..." << endl;
    cout << endl;
}


void option2 ()
{
    for (int i = 0; i < shapeCount; ++i) {
        if (shapeStorage[i]) {
            shapeStorage[i]->setArea(shapeStorage[i]->computeArea());  
            // computes and stores internally
            // shapes[i]->area now has the computed area
        }
    }
    
    cout << "Computation completed! ( " << shapeCount
         << " records were updated )" << endl;
    cout << endl;
}

// stores the onPerimeter and inShape points 
void computeShapePoints(ShapeTwoD* shape, std::vector<Point>& onPerimeter, std::vector<Point>& inShape) {
    onPerimeter.clear();
    inShape.clear();

	// polymorphically calls method of shape
	// bounding box gets the max x and max y of a shape and lists all
	// the coordinates in that box, including the perimeter of the box
    auto bbox = shape->buildBoundingBox();

    for (const auto& pt : bbox) {
        if (shape->isPointOnShape(pt.x, pt.y)) {
            onPerimeter.push_back(pt);
        } else if (shape->isPointInShape(pt.x, pt.y)) {
            inShape.push_back(pt);
        }
    }
	
	// polymorphically calls method of shape
    vector<Point> vertices = shape->getVertices();

	// since circle has different vertices/format of input, its points get
	// processed differently
    if (shape->getName() != "Circle") {
        auto vertexExists = [&](const Point& p) {
            return std::any_of(vertices.begin(), vertices.end(),
                [&](const Point& v) { return v.x == p.x && v.y == p.y; });
        };
        onPerimeter.erase(std::remove_if(onPerimeter.begin(),
            onPerimeter.end(),
            [&](const Point& p) { return vertexExists(p); }),
            onPerimeter.end());
    }

    if (shape->getName() == "Circle") {
        Circle* circ = dynamic_cast<Circle*>(shape);
        if (circ) {
            Point center = circ->getCenter();
            inShape.erase(std::remove_if(inShape.begin(), inShape.end(),
                [&](const Point& p) { return p.x == center.x && p.y == center.y; }),
                inShape.end());
        }
    }
}



void option3() {
    if (shapeCount == 0) {
        cout << "No shapes have been recorded yet." << endl << endl;
        return;
    }

    cout << "Total no. of records available = " << shapeCount << endl << endl;

    for (int i = 0; i < shapeCount; ++i) {
        cout << "Shape[" << i << "]" << endl;
        cout << shapeStorage[i]->toString() << endl;

        vector<Point> onPerimeter, inShape;
        computeShapePoints(shapeStorage[i], onPerimeter, inShape);

        cout << "Points on perimeter : ";
        if (onPerimeter.empty()) cout << "none !";
        else {
            for (const auto& p : onPerimeter)
                cout << "(" << p.x << "," << p.y << ") ";
        }
        cout << endl << endl;

        cout << "Points within shape : ";
        if (inShape.empty()) cout << "none !";
        else {
            for (const auto& p : inShape)
                cout << "(" << p.x << "," << p.y << ") ";
        }
        cout << endl << endl << endl;
    }
}



void option4() {
    if (shapeCount == 0) {
        cout << "No shapes have been recorded yet." << endl << endl;
        return;
    }

    cout << "     a)    Sort by area (ascending)" << endl;
    cout << "     b)    Sort by area (descending)" << endl;
    cout << "     c)    Sort by special type and area" << endl;
   	cout << endl;
    cout << "Please select sort option ('q' to go main menu): ";

    string userAnswer;
    string message;
    getline(cin, userAnswer);
    userAnswer = sanitizeInput(userAnswer);

    if (userAnswer == "q") return;
    
    cout << endl << endl;

    // Copy shape pointers into a temp vector
    vector<ShapeTwoD*> tempShapes(shapeStorage, shapeStorage + shapeCount);

	// if user answer a, sort by area ascending
    if (userAnswer == "a") {
        sort(tempShapes.begin(), tempShapes.end(),
            [](ShapeTwoD* a, ShapeTwoD* b) {
                return a->computeArea() < b->computeArea();
            });
        message = "Sort by area (smallest to largest) ...";
    
    // if user answer b, sort by area descending
    } else if (userAnswer == "b") {
        sort(tempShapes.begin(), tempShapes.end(),
            [](ShapeTwoD* a, ShapeTwoD* b) {
                return a->computeArea() > b->computeArea();
            });
        message = "Sort by area (largest to smallest) ...";
        
    // if user answer c, sort by special type and area
    } else if (userAnswer == "c") {
        sort(tempShapes.begin(), tempShapes.end(),
            [](ShapeTwoD* a, ShapeTwoD* b) {
                bool aWS = a->getContainsWarpSpace();;
                bool bWS = b->getContainsWarpSpace();;

                if (aWS != bWS) {
                    return aWS;  // WS types first
                }
                return a->computeArea() > b->computeArea();
            });
            message = "Sort by special type (WS to NS) and area (smallest to largest) ...";
    } else {
        cout << "Invalid option." << endl;
        return;
    }

	// display
	cout << message << endl;
	cout << endl;
	for (size_t i = 0; i < tempShapes.size(); ++i) {
		cout << "Shape[" << i << "]" << endl;
		cout << tempShapes[i]->toString() << endl;

		vector<Point> onPerimeter, inShape;
		computeShapePoints(tempShapes[i], onPerimeter, inShape);

		cout << "Points on perimeter : ";
		if (onPerimeter.empty()) cout << "none !";
		else {
		    for (const auto& p : onPerimeter)
		        cout << "(" << p.x << "," << p.y << ") ";
		}
		cout << endl << endl;

		cout << "Points within shape : ";
		if (inShape.empty()) cout << "none !";
		else {
		    for (const auto& p : inShape)
		        cout << "(" << p.x << "," << p.y << ") ";
		}
		cout << endl << endl << endl;
	}

}



// ===== extra functions =====
// prompt user to press <enter> 
void promptUser()
{
	cout << endl;
	cout << "Press <enter> to go back to main menu ... ";
	cin.get();
	cout << endl;	
}


void exitProgram() 
{
	// cleanup
	if (shapeStorage != nullptr)
	{
		for (int i = 0; i < shapeCount; i++) {
			delete shapeStorage[i];
		}
		shapeCount = 0;
	}
	
	cout << "Memory deallocated." << endl;
	cout << "Exiting program ... \n";
	exit(0);
}

