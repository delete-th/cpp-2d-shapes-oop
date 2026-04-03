# cpp-2d-shapes-oop

A menu-driven CLI application in C++ that models 2D geometric shapes using object-oriented programming. Supports input, display, area computation, and point-containment checks across multiple shape types.

## Features

- Supports four shape types: **Circle**, **Square**, **Rectangle**, and **Cross**
- Detects whether a shape contains **warp space** (a concave interior region)
- Computes **area** for each shape
- Checks whether a given point is **inside** or **on the boundary** of a shape
- Orders vertices and builds shape perimeters
- Constructs **bounding boxes** for any shape

## Concepts Demonstrated

- Inheritance and polymorphism via a `ShapeTwoD` base class
- Virtual functions and method overriding
- Dynamic dispatch with runtime polymorphism
- Modular class design with `.h` / `.cpp` separation

## Build & Run

```bash
g++ -std=c++11 Assn2.cpp functions.cpp ShapeTwoD.cpp Circle.cpp Square.cpp Rectangle.cpp Cross.cpp -o shapes_app
./shapes_app
```

## File Structure

```
├── Assn2.cpp         # Entry point and menu loop
├── functions.h/cpp   # Menu and option logic
├── ShapeTwoD.h/cpp   # Abstract base class
├── Circle.h/cpp      # Circle shape
├── Square.h/cpp      # Square shape
├── Rectangle.h/cpp   # Rectangle shape
├── Cross.h/cpp       # Cross shape
└── point.h           # Point struct used by shapes
```
