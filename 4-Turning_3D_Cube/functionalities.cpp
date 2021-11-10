#include "vgl.h"
#include "LoadShaders.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "Cube.h"
#include "functionalities.h"

using namespace std;

const GLint NumBuffers = 2;
GLuint Buffers[NumBuffers];

GLuint model_view_matrix_location; // to store the location of our model-view matrix in VRAM
GLuint camera_matrix_location;     // to store the location of our camera matrix in VRAM
GLuint projection_matrix_location; // to store the location of our projection matrix in VRAM

GLfloat alpha = 0; // to create animation (rotate the square)

GLfloat degreeAngle = 90.0;
GLfloat theta = degreeAngle * 0.0174533; // theta will be used to reposition the location of 2 other sides of the cube

// Camera parameter values
glm::vec3 cam_pos = glm::vec3(0.0, 0.0, 2.5);

// Camera looking point (used for direction of the camera)
glm::vec3 cam_dir = glm::vec3(0.0, 0.0, 0.0);

// Camera Up-Vector
glm::vec3 cam_up_vector = glm::vec3(0.0, 1.0, 0.0);


// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------

// Function that draws a cube. 
void drawCube(glm::mat4 model_view_matrix)
{
	glDrawArrays(GL_TRIANGLES, 0, 6);	 // --------------- FIRST SQUARE
	glDrawArrays(GL_TRIANGLES, 6, 6);	 // --------------- SECOND SQUARE

	// Turn 90 degrees to draw the other right and left 2 squares
	model_view_matrix = glm::rotate(model_view_matrix, theta, glm::vec3(0, 1, 0));
	glUniformMatrix4fv(model_view_matrix_location, 1, GL_FALSE, &model_view_matrix[0][0]);

	glDrawArrays(GL_TRIANGLES, 12, 6); // --------------- THIRD SQUARE
	glDrawArrays(GL_TRIANGLES, 18, 6); // --------------- FOURTH SQUARE

	// Turn 90 degrees to draw the other up and down 2 squares
	model_view_matrix = glm::rotate(model_view_matrix, theta, glm::vec3(1, 0, 0));
	glUniformMatrix4fv(model_view_matrix_location, 1, GL_FALSE, &model_view_matrix[0][0]);

	glDrawArrays(GL_TRIANGLES, 24, 6); // --------------- FIFTH SQUARE
	glDrawArrays(GL_TRIANGLES, 30, 6); // --------------- SIXTH SQUARE
}


//---------------------------------------------------------------------
//
// initialization: Setting up our rendering pipeline. Loading Shaders. Creating buffers in VRAM. 
//
//---------------------------------------------------------------------
void init(void)
{
	glEnable(GL_DEPTH_TEST);	//Enabling the depth test so that closer objects to camera obscure further objects

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);	//Loading the shaders from files: triangles.vert & triangles.frag
	glUseProgram(program);	//Compiling and running the shaders. And now, my Pipeline is set up :-)


	//We allocate two buffers in VRAM: One for vertex data and the other for colors
	//Once created, we use Buffers[0] to point to the first, and Buffers[1] to point to the second.
	glGenBuffers(2, Buffers);

	//-----------------------------------------
	//Initializing the first buffer: Buffers[0]
	//-----------------------------------------

	//Selecting Buffers[0]
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);

	//Pushing the vertices data into the buffer (transmission from RAM to VRAM)
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	//During the run-time, the buffer data should be transferred to the "vPosition" variable in the vertex shader
	glBindAttribLocation(program, 0, "vPosition");

	//We specify the format of the data in the buffer: 
	// GL_FLOAT: They are float as we have used a GLfloat[] array up in the code
	// drawType => 2: They must be considered as couples since we have 2D vertices
	//             3: 3D
	GLint drawType = 3;
	glVertexAttribPointer(0, drawType, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColors), cubeColors, GL_STATIC_DRAW);

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


	// Retreiving the memory address of the "model_view" matrix in VRAM (in vertex shader)
	model_view_matrix_location = glGetUniformLocation(program, "model_view");

	// Retreiving the memory address of the "camera" matrix in VRAM (in vertex shader)
	camera_matrix_location = glGetUniformLocation(program, "camera");

	// Retreiving the memory address of the "projection" matrix in VRAM (in vertex shader)
	projection_matrix_location = glGetUniformLocation(program, "projection");
}
//---------------Initialization accomplished :-)



//---------------------------------------------------------------------
//
// display function. All drawings happen here in the "display" function
//
//---------------------------------------------------------------------
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT); //Clears the screen and ready to draw
	glClear(GL_DEPTH_BUFFER_BIT); //Clearing the depth buffer

	//We'd like to translate our geometry 1.0 units along the X-axis
	glm::mat4 model_view_matrix = glm::mat4(1.0);
	model_view_matrix = glm::rotate(model_view_matrix, alpha, glm::vec3(0, 1, 0)); // keep Rotating in Y Local Axis


	//Passing the built model-view_matrix to the vertex shader. 
	//Recall: variable 'location' contains the memory address of the model_view matrix that is used in our vertex shader.
	glUniformMatrix4fv(model_view_matrix_location, 1, GL_FALSE, &model_view_matrix[0][0]);

	//--------- building our camera matrix using glm library.
	//Setting up my camera in the scene: location, direction and up_vector.
	glm::mat4 camera_mat = glm::lookAt(cam_pos, cam_dir, cam_up_vector);
	glUniformMatrix4fv(camera_matrix_location, 1, GL_FALSE, &camera_mat[0][0]);

	//Finally, developing our projection matrix using glm library
	glm::mat4 projection_mat = glm::frustum(-0.03, +0.03, -0.03, +0.03, 0.1, 10.0);
	glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE, &projection_mat[0][0]);


	drawCube(model_view_matrix);

	glFlush();							//Flush the rendered contents on the screen.
}



// -- This function gets called for every frame. This will be used to animate the world 
void idle()
{
	float omega = 1.5;	// let omega  be the angular speed for Global Axes rotation

	float timeSinceStart = glutGet(GLUT_ELAPSED_TIME) / 1000.f;		//Retreives the time in seconds

	alpha = omega * timeSinceStart;		//We use Theta = omega * time to calculate the amount of rotation of the geometry in this frame

	glutPostRedisplay();	//This is the explicit call to display function: display()
}


void keyboard(unsigned char key, int x, int y)
{
	//Handler function for keyboard events
	//To be developed in next lectures

	switch (key)
	{
	case 'i':
		cam_pos.z -= 0.05;
		break;
	case 'o':
		cam_pos.z += 0.05;
		break;
	case 'w':
		cam_pos.y += 0.05;
		break;
	case 's':
		cam_pos.y -= 0.05;
		break;
	case 'a':
		cam_pos.x -= 0.05;
		break;
	case 'd':
		cam_pos.x += 0.05;
		break;
	}
	glutPostRedisplay();
}