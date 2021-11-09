
#include "functionalities.h"


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);

	//The dimensions of the window
	glutInitWindowSize(512, 512);

	//The title for the window
	glutCreateWindow("Crazy Triangles - Arthur Dibe");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	//Defining our 'display', 'idle', 'mouse' and 'keyboard' functions.
	glutDisplayFunc(display);

	//The following function makes the OpenGL to go through an infinite loop and waits for any event from keyboard, mouse, etc.
	glutMainLoop();
}
