#include <iostream> 
#include <fstream>
#include <string>
#include <limits>
#include "functions.h"

using namespace std;

// compile statement : g++ -std=c++11 Assn2.cpp functions.cpp ShapeTwoD.cpp Circle.cpp Square.cpp Rectangle.cpp Cross.cpp -o csci251_a2.app

int main()
{
	int userInput = 0;
	//  initializeTestShapes();  hardcoded test cases for testing!
	
	while (userInput != 5) 
	{
		displayMenu();
		cin >> userInput;
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore newline from enter choice 
		
		// handle non-integer input
		if (cin.fail()) {
			cin.clear(); 	// clear error state
			// discard invalid input
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
			cout << "\nNot an option. Select again.\n";
			continue;
		}
		
		cout << endl;
		
		// handle integer out of range
		if (userInput < 1 || userInput > 5) {
			cout << "Not an option. Select again.\n";
			continue;
		}
		
		// valid input
		switch (userInput) 
		{
			case 1: option1(); 
					break;
			case 2: option2(); 
					break;
			case 3: option3();
					promptUser(); 
					break;
			case 4: option4();
					break;
			case 5: exitProgram();
					break;
			
		}
	}
	
	return 0;
}

