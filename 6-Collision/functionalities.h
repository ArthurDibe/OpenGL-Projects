#ifndef FUNCTIONALITIES_H
#define FUNCTIONALITIES_H

#include "vgl.h"


float randomFloat(float a, float b);
void setUpPowerPacks();
void init(void);
void drawCube(float scale, GLuint* selectedTexture);
void draw_level();
void updateScene(); // Update the Scene objects' position
void display(void);
void keyboard(unsigned char key, int x, int y);
void mouse(int x, int y);
void idle();

#endif