
#include "functionalities.h"

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);

	//The dimensions of the window
	glutInitWindowSize(900, 900);

	//The title for the window
	glutCreateWindow("Toggle Views - Arthur Dibe");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	//Defining our 'display', 'idle', 'mouse' and 'keyboard' functions.
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(mouse);

	//The following function makes the OpenGL to go through an infinite loop and waits for any event from keyboard, mouse, etc.
	glutMainLoop();
	
}
