#include "vgl.h"
#include "LoadShaders.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "functionalities.h"
using namespace std;

const GLint NumBuffers = 2;
GLuint Buffers[NumBuffers];
const GLuint NumVertices = 6;

//Will be used to store the location of our model-view matrix in VRAM
GLuint location;

//Will be used to create animation (rotate the square)
GLfloat alpha = 0;

GLfloat theta = 0;

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
		// We use two triangles and connect them together to get a square. We're ONLY allowed to use triangles in this course.

		//First triangle
		{ -0.45, 0.45 },
		{ 0.45, 0.45 },
		{ 0.45, -0.45 },

		//Second triangle
		{ 0.45, -0.45 },
		{ -0.45, -0.45 },
		{ -0.45, 0.45 }

	};

	GLfloat colorData[NumVertices][3] = {
		{ 1,0,0 }, // colors for vertices of the first triangle
		{ 0,1,0 },
		{ 0,0,1 },

		{ 1,0,0 }, // colors for vertices of the second triangle
		{ 0,1,0 },
		{ 0,0,1 }

	};

	//We allocate two buffers in VRAM: One for vertex data and the other for colors
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
	// 2: They must be considered as couples since we have 2D vertices (X, Y)
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
	//-------------------------------------------------------------------------
	//---------------We are done initializing the second buffer----------------
	//-------------------------------------------------------------------------


	//Retreiving the memory address of the "model_view" matrix in VRAM (in vertex shader)
	location = glGetUniformLocation(program, "model_view");
}
//---------------Initialization accomplished :-)




//---------------------------------------------------------------------
//
// display function. All drawings happen here in the "display" function
//
//---------------------------------------------------------------------
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);	//Clears the screen and ready to draw


	//Building our transformation matrices using glm library

	//Next, we'd like to rtate the translated geometry about Z-axis
	// 45 degrees will be = 3.141f/4.0f
	glm::mat4 model_view_matrix = glm::rotate(glm::mat4(1.0), alpha, glm::vec3(0, 0, 1));   // Rotation in Global axis

	//We'd like to translate our geometry 0.7 units along the X-axis
	model_view_matrix = glm::translate(model_view_matrix, glm::vec3(0.7, 0.0, 0.0));

	model_view_matrix = glm::rotate(model_view_matrix, theta, glm::vec3(0, 0, 1));          // Rotation in Local Axis

	//Finally, we'll shrink the geometry to 70% of the original dimensions in X, Y directions
	model_view_matrix = glm::scale(model_view_matrix, glm::vec3(0.4, 0.4, 0.0));

	//Passing the built model-view_matrix to the vertex shader. Recall: variable 'location' contains the memory address of the model_view
	//matrix that is used in our vertex shader.
	glUniformMatrix4fv(location, 1, GL_FALSE, &model_view_matrix[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, 6);	//Connect the vertices using "GL_TRIANGLES" modes.
										//Read more about draw modes here:
										//https://www.glprogramming.com/red/chapter02.html
										//https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glDrawArrays.xml

	glFlush();							//Flush the rendered contents on the screen.
}

//This function gets called for every frame. This will be used to animate the world 
void idle()
{
	float omega = 1;	// let omega  be the angular speed for Global Axes rotation
	float omega2 = 5;   // let omega2 be the angular speed for Local Axes rotation

	float timeSinceStart = glutGet(GLUT_ELAPSED_TIME) / 1000.f;		//Retreives the time in seconds

	alpha = omega * timeSinceStart;		//We use Theta = omega * time to calculate the amount of rotation of the geometry in this frame
	theta = omega2 * timeSinceStart;

	glutPostRedisplay();	//This is the explicit call to display function: display()
}