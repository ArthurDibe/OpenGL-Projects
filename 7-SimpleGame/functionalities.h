#ifndef FUNCTIONALITIES_H
#define FUNCTIONALITIES_H


#include "glm\glm.hpp"

extern glm::vec3 cam_pos;
extern glm::vec3 looking_dir_vector;

float randomFloat(float a, float b);
void setUpPowerPacks();
void setUpEnemies();
void init(void);
void drawCube(float scale, GLuint* selectedTexture);
void draw_level();
void updateScene(); // Update the Scene objects' position
void generateProjectile(GLchar objType, glm::vec3 dir_vector);
void display(void);
bool checkForGameEnd();
void endGameResults();
void resetModelViewState();
void updateCameraState();
void keyboard(unsigned char key, int x, int y);
void mouse(int x, int y);
void idle();



#endif