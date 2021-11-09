#include <cstdlib> // srand, rand functions
#include <ctime>   // for time function
#include "vgl.h"
#include "LoadShaders.h"
#include "functionalities.h"
using namespace std;


const GLint NumBuffers = 2; 
GLuint Buffers[NumBuffers]; // buffers for Vertices index "0" and Colors index "1"

const int typeOfGraphic = 2; // "typeOfGraphic" variable defines if the draw is 2D or 3D
const int colorScheme = 3; // "colorScheme" tells that it will have 3 numbers

const int N = 100; // "N" represents the number of Triangles
const GLuint numVertices = N * 3; // number of vertices: 3 vertices per triangle
const GLuint numOfConnections = numVertices; // number of vertices connections that glDrawArrays will do


// This function's task is to return a random number between 0 and 1
float getRandomNumber() {
	return (float)rand() / RAND_MAX;
}

// This function's task is to return a random number either positive or negative
float getPosNegNumber() {
	float randNumber = getRandomNumber();
	return (rand() % 2) ? randNumber : randNumber * -1;
}

// This function's task is to randomly generates the location of the vertices and random colors
void genRandVerticesAndColors(GLfloat vertices[][typeOfGraphic], GLfloat colorData[][colorScheme]) {
	for (int i = 0; i < numVertices; i++) {
		// random vertice generated
		vertices[i][0] = getPosNegNumber();
		vertices[i][1] = getPosNegNumber();

		// random color for the vertice generated
		colorData[i][0] = getRandomNumber();
		colorData[i][1] = getRandomNumber();
		colorData[i][2] = getRandomNumber();
	}
}



//---------------------------------------------------------------------
//
// initialization: Setting up our rendering pipeline. Loading Shaders. Creating buffers in VRAM. 
//
//---------------------------------------------------------------------
void init(void)
{
	// "srand" seeds the "rand" to make sure that 
	// the number generated will always be random
	srand((unsigned)time(NULL));

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);	//Loading the shaders from files: triangles.vert & triangles.frag
	glUseProgram(program);	//Compiling and running the shaders. And now, my Pipeline is set up :-)


	// Buffers for vertices and colors
	GLfloat vertices[numVertices][typeOfGraphic] = {};
	GLfloat colorData[numVertices][colorScheme] = {};


	// Randomly generates the location of the vertices and random colors
	genRandVerticesAndColors(vertices, colorData);

	//We allocate two buffers in VRAM: One for vertex data and the other for colors
	//Once created, we use Buffers[0] to point to the first, and Buffers[1] to point to the second.
	glGenBuffers(2, Buffers);

	//-----------------------------------------
	//Initializing the first buffer: Buffers[0] -> VERTICES BUFFER
	//-----------------------------------------

	//Selecting Buffers[0]
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);

	//Pushing the vertices data into the buffer (transmission from RAM to VRAM)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//During the run-time, the buffer data should be transferred to the "vPosition" variable in the vertex shader
	glBindAttribLocation(program, 0, "vPosition");

	//We specify the format of the data in the buffer: 
	// GL_FLOAT: They are float as we have used a GLfloat[] array up in the code
	// "typeOfGraphic": must be considered as couples since we have 2D vertices for this Lab
	glVertexAttribPointer(0, typeOfGraphic, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//Enable the buffer
	glEnableVertexAttribArray(0);
	//-------------------------------------------------------------------------
	//---------------We are done initializing the first buffer-----------------
	//-------------------------------------------------------------------------



	//-----------------------------------------
	//Initializing the second buffer: Buffers[1] -> COLORS BUFFER
	//-----------------------------------------

	//Selecting Buffers[1]
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);

	//Pushing the color data into the buffer (transmission from RAM to VRAM)
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);

	//During the run-time, the buffer data should be transferred to the "vertexColor" variable in the vertex shader
	glBindAttribLocation(program, 1, "vertexColor");

	//We specify the format of the data in the buffer: 
	// "GL_FLOAT": They are float as we have used a GLfloat[] array up in the code
	// "colorScheme": They must be considered as triplets since we use 3 values for each color(red, green, blue)
	glVertexAttribPointer(1, colorScheme, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//Enable the buffer
	glEnableVertexAttribArray(1);
}
//-------------------------------------------------------------------------
//---------------We are done initializing the second buffer----------------
//-------------------------------------------------------------------------
//---------------Initialization accomplished :-)



//---------------------------------------------------------------------
//
// display function. All drawings happen here in the "display" function
//
//---------------------------------------------------------------------
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);	//Clears the screen and ready to draw

	//glDrawArrays(GL_TRIANGLES, 0, NumOfConnections);	//Connect the vertices using "GL_TRIANGLES" modes.
										//Read more about draw modes here:
										//https://www.glprogramming.com/red/chapter02.html
										//https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glDrawArrays.xml

	for (size_t i = 0; i < numOfConnections; i += 3)
	{
		glDrawArrays(GL_LINE_LOOP, i, 3);
	}

	glFlush();							//Flush the rendered contents on the screen.
}