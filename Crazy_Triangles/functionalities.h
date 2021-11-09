#ifndef INIT_H
#define INIT_H

#include "vgl.h"

// "typeOfGraphic" variable defines if the draw is 2D or 3D
//		- if number 2, then it means 2D graphic
//		- if number 3, then it means 3D graphic
extern const int typeOfGraphic;

// "colorScheme" tells that it will have 3 numbers
//		and each number represents RGB respectively
extern const int colorScheme;


extern const int N; // "N" represents the number of Triangles
extern const GLuint numVertices; // number of vertices
extern const GLuint numOfConnections; // number of connections


float getRandomNumber(); // This function`s task is to return a random number between 0 and 1

float getPosNegNumber(); // This function`s task is to return a random number either positive or negative

// This function's task is to randomly generates the location of the vertices and random colors
//void genRandVerticesAndColors(GLfloat vertices[][typeOfGraphic], GLfloat colorData[][colorScheme]);
void genRandVerticesAndColors(GLfloat** vertices, GLfloat** colorData);


void init(void);
void display(void);

#endif