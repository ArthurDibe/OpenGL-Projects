#ifndef FUNCTIONALITIES_H
#define FUNCTIONALITIES_H

#include "vgl.h"
#include "glm\glm.hpp"

void init(void);
void display(void);
void drawLevel(glm::mat4 model_view_matrix);
void drawCube(glm::mat4 model_view_matrix, GLfloat pos_x, GLfloat pos_y, GLfloat pos_z, GLfloat rotation_z);
void keyboard(unsigned char key, int x, int y);
void mouse(int x, int y);
void idle();

#endif