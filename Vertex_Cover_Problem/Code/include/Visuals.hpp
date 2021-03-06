#ifndef VISUALS_HPP
#define VISUALS_HPP

#include "Graph.hpp"
#include <GL/glut.h>

#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#define PI 3.1415927

// Drawing Functions
void drawGraph(graph::Graph &g);

// OpenGL Handling Functions
void renderScene(void);
void resize(int, int);
void initializeGL(void);

// Event Handling Functions
void mouseClick(int button, int state, int x, int y);
void mouseMotion(int x, int y);
void keyboardDown(unsigned char key, int x, int y);

#endif // VISUALS_HPP
