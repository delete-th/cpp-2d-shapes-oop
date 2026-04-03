#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include "point.h"
#include <utility>


class ShapeTwoD 
{
	protected:
		std::string name;
		bool containsWarpSpace;
		double area = 0.0;
      
	public:
		// constructor
      	ShapeTwoD(std::string name, bool containsWarpSpace);
      	
      	// destructor
      	virtual ~ShapeTwoD();
      	
      	std::string getName();
      	bool getContainsWarpSpace();
        double getArea() const { return area; }
    

    	virtual std::string toString();
    	virtual std::vector<Point> getVertices() const { return {}; }
      	
		virtual double computeArea();
		virtual bool isPointInShape(int x, int y);
		virtual bool isPointOnShape(int x, int y);
		
		virtual std::vector<Point> orderVertices
		(const std::vector<Point>& vertices);
		
		virtual std::vector<Point> buildPerimeter
		(const std::vector<Point>& orderedVertices,
         const std::vector<Point>& box);
		

      	void setName(std::string name);
      	void setContainsWarpSpace(bool containsWarpSpace);
      	void setArea(double a) { area = a; }
      	
      	std::vector<Point> buildBoundingBox() const;
      	
      	
};



#endif
