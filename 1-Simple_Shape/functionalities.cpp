#include "vgl.h"
#include "LoadShaders.h"
#include "functionalities.h"
using namespace std;

const GLint NumBuffers = 2;
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 9;
const GLuint NumOfConnections = NumVertices;


//---------------------------------------------------------------------
//
// initialization: Setting up our rendering pipeline. Loading Shaders. Creating buffers in VRAM. 
//
//---------------------------------------------------------------------
void init(void)
{

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);	//Loading the shaders from files: triangles.vert & triangles.frag
	glUseProgram(program);	//Compiling and running the shaders. And now, my Pipeline is set up :-)

	GLfloat vertices[NumVertices][2] = {
		// ---- BLUE TRIANGLE 
		{ -0.5, 0.2 },
		{ -0.7, -0.2 },
		{ -0.3, -0.2 },


		// ---------- RED BUTTERFLY ----------
		// Left Half Triangle
		{ 0.5, 0.0 },
		{ 0.3, 0.2 },
		{ 0.3, -0.2 },

		// Right Half Triangle
		{ 0.5, 0.0 },
		{ 0.7, 0.2 },
		{ 0.7, -0.2 }

		// --------------------------------
	};

	GLfloat colorData[NumVertices][3] = {
		{ 0,0,1 }, // Triangle - Blue color
		{ 0,0,1 },
		{ 0,0,1 },

		{ 1,0,0 }, // Butterfly - Red color - LEFT half triangle 
		{ 1,0,0 },
		{ 1,0,0 },

		{ 1,0,0 }, // Butterfly - Red color - RIGHT half triangle
		{ 1,0,0 },
		{ 1,0,0 }
	};

	//Allocate two buffers in VRAM: One for vertex data and the other for colors
	//Once created, we use Buffers[0] to point to the first, and Buffers[1] to point to the second.
	glGenBuffers(2, Buffers);

	//-----------------------------------------
	//Initializing the first buffer: Buffers[0]
	//-----------------------------------------

	//Selecting Buffers[0]
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);

	//Pushing the vertices data into the buffer (transmission from RAM to VRAM)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//During the run-time, the buffer data should be transferred to the "vPosition" variable in the vertex shader
	glBindAttribLocation(program, 0, "vPosition");

	//We specify the format of the data in the buffer: 
	// GL_FLOAT: They are float as we have used a GLfloat[] array up in the code
	// 2: They must be considered as couples since we have 2D vertices
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//Enable the buffer
	glEnableVertexAttribArray(0);
	//-------------------------------------------------------------------------
	//---------------We are done initializing the first buffer-----------------
	//-------------------------------------------------------------------------



	//-----------------------------------------
	//Initializing the second buffer: Buffers[1]
	//-----------------------------------------

	//Selecting Buffers[1]
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);

	//Pushing the color data into the buffer (transmission from RAM to VRAM)
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);

	//During the run-time, the buffer data should be transferred to the "vertexColor" variable in the vertex shader
	glBindAttribLocation(program, 1, "vertexColor");

	//We specify the format of the data in the buffer: 
	// GL_FLOAT: They are float as we have used a GLfloat[] array up in the code
	// 3: They must be considered as triplets since we use 3 values for each color(red, green, blue)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

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

	glDrawArrays(GL_TRIANGLES, 0, NumOfConnections);	//Connect the vertices using "GL_TRIANGLES" modes.

	glFlush();							//Flush the rendered contents on the screen.
}