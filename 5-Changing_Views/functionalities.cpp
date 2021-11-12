#include "vgl.h"
#include "LoadShaders.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\rotate_vector.hpp"
#include "cube.h";
#include "functionalities.h"


const GLint NumBuffers = 2;

GLuint Buffers[NumBuffers];

//Will be used to store the location of our model-view matrix in VRAM
GLuint model_view_matrix_location;

//Will be used to store the location of our camera matrix in VRAM
GLuint camera_matrix_location;

//Will be used to store the location of our projection matrix in VRAM
GLuint projection_matrix_location;

//Will be used to create animation (rotate the square)
GLfloat alpha = 0;

GLfloat degreeAngle = 90.0;
GLfloat theta = degreeAngle * 0.0174533; // theta will be used to reposition the location of 2 other sides of the cube

//Player motion speed
float height = 0.8f;
float yaw_speed = 0.1f;
float travel_speed = 60.0f;
float mouse_sensitivity = 0.01f;

//Used for tracking mouse cursor position on screen
int x0 = 0;
int y_0 = 0;

float top_height = 20.0f;
float initial_pos_x = -5.0f;
float initial_pos_y = -5.0f;

//Camera parameter values
glm::vec3 cam_pos = glm::vec3(initial_pos_x, initial_pos_y, height);
glm::vec3 forward_vector = glm::normalize(glm::vec3(1, 1, 0));
glm::vec3 up_vector = glm::vec3(0.0, 0.0, 1.0);
glm::vec3 side_vector = glm::normalize(glm::cross(up_vector, forward_vector));

//Used to measure time between two frames
int oldTimeSinceStart = 0;
int deltaTime;

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
	//--------------- done initializing the first buffer -----------------
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


	//Retreiving the memory address of the "model_view" matrix in VRAM (in vertex shader)
	model_view_matrix_location = glGetUniformLocation(program, "model_view");

	//Retreiving the memory address of the "camera" matrix in VRAM (in vertex shader)
	camera_matrix_location = glGetUniformLocation(program, "camera");

	//Retreiving the memory address of the "projection" matrix in VRAM (in vertex shader)
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
	glClear(GL_COLOR_BUFFER_BIT);	//Clears the screen and ready to draw

	//Clearing the depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	//Building our transformation matrices using glm library

	//NOTE: 
	//THE ORDER OF TRANSFOEMATION DOES MATTER. A rotation followed by a translation- 
	//might return a different outcome than a translation followed by a rotation.

	//We'd like to translate our geometry 0.5 units along the X-axis
	glm::mat4 model_view_matrix = glm::mat4(1.0);


	//Passing the built model-view_matrix to the vertex shader. Recall: variable 'location' contains the memory address of the model_view
	//matrix that is used in our vertex shader.
	glUniformMatrix4fv(model_view_matrix_location, 1, GL_FALSE, &model_view_matrix[0][0]);

	glm::vec3 look_at = glm::vec3(cam_pos.x + forward_vector.x, cam_pos.y + forward_vector.y, height);

	glm::mat4 camera_mat = glm::lookAt(cam_pos, look_at, up_vector);
	glUniformMatrix4fv(camera_matrix_location, 1, GL_FALSE, &camera_mat[0][0]);



	//Finally, developing our projection matrix using glm library
	//glm::mat4 projection_mat = glm::frustum(-0.1, +0.1, -0.1, +0.1, 0.1, 10.0);
	glm::mat4 projection_mat = glm::frustum(-0.03, +0.03, -0.03, +0.03, 0.1, 100.0);
	glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE, &projection_mat[0][0]);


	//model_view_matrix = glm::translate(model_view_matrix, glm::vec3(0.0, 0.0, 0.3));
	drawLevel(model_view_matrix);


	glFlush();							//Flush the rendered contents on the screen.
}

void drawLevel(glm::mat4 model_view_matrix)
{
	// Draw the BLOCKS ---------------------------------------------------------------------- 
	drawCube(model_view_matrix, 0.0, 0.0, 0.0, 0);
	drawCube(model_view_matrix, 2.0, 2.0, 0.0, 0.5);
	drawCube(model_view_matrix, -2.0, -2.0, 0.0, 0.3);
	drawCube(model_view_matrix, 2.0, 0.0, 0.0, 0.9);
	drawCube(model_view_matrix, 0.0, 2.0, 0.0, 1.2);


	// Draw the GROUND FLOOR ----------------------------------------------------------------
	model_view_matrix = glm::rotate(model_view_matrix, theta * 2, glm::vec3(0, 1, 0));
	glUniformMatrix4fv(model_view_matrix_location, 1, GL_FALSE, &model_view_matrix[0][0]);
	model_view_matrix = glm::scale(model_view_matrix, glm::vec3(30, 30, 1));
	glUniformMatrix4fv(model_view_matrix_location, 1, GL_FALSE, &model_view_matrix[0][0]);
	drawCube(model_view_matrix, 0.0, 0.0, 1.0, 2);
	// --------------------------------------------------------------------------------------
}

// Parameters: model_view_matrix, 
void drawCube(glm::mat4 model_view_matrix, GLfloat pos_x, GLfloat pos_y, GLfloat pos_z, GLfloat rotation_z)
{
	// -------------------------------
	// locate the start point to draw and the start rotation in Z axis
	// move the cube along x,y
	model_view_matrix = glm::translate(model_view_matrix, glm::vec3(pos_x, pos_y, pos_z));
	glUniformMatrix4fv(model_view_matrix_location, 1, GL_FALSE, &model_view_matrix[0][0]);

	// Rotate the cube about Z axis
	model_view_matrix = glm::rotate(model_view_matrix, theta * rotation_z, glm::vec3(0, 0, 1));
	glUniformMatrix4fv(model_view_matrix_location, 1, GL_FALSE, &model_view_matrix[0][0]);

	// ---------------------------------------------------------------
	// After positioning the start location and the rotation in Z axis
	// Then start drawing the cube
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

void keyboard(unsigned char key, int x, int y)
{
	//Handler function for keyboard events
	//To be developed in next lectures

	switch (key)
	{
	case 'w':
		cam_pos += forward_vector * 0.1f; //travel_speed * ((float)deltaTime) / 1000.0f;
		break;

	case 'a':
		cam_pos += side_vector * 0.1f; //travel_speed * ((float)deltaTime) / 1000.0f;travel_speed * ((float)deltaTime) / 1000.0f;
		break;

	case 's':
		cam_pos -= forward_vector * 0.1f; //travel_speed * ((float)deltaTime) / 1000.0f;travel_speed * ((float)deltaTime) / 1000.0f;
		break;

	case 'd':
		cam_pos -= side_vector * 0.1f; //travel_speed * ((float)deltaTime) / 1000.0f;travel_speed * ((float)deltaTime) / 1000.0f;
		break;

	case 't':
		cam_pos = glm::vec3(0.0, 0.0, top_height); // make the height of the camera to be very heigh at the center of the screen
		forward_vector = glm::rotate(forward_vector, theta, side_vector); // rotate the camera forward vector about 90 degrees the side_vector
		break;

	case 'p':
		cam_pos = glm::vec3(initial_pos_x, initial_pos_y, height);  // put back the camera position and height as the initial
		forward_vector = glm::rotate(forward_vector, -theta, side_vector); // turn back the camera rotation about the side vector to be as initial
		break;
	}

	glutPostRedisplay();
}

void mouse(int x, int y)
{
	int delta_x = x - x0;
	forward_vector = glm::rotate(forward_vector, -delta_x * mouse_sensitivity, up_vector);
	side_vector = glm::cross(up_vector, forward_vector);

	x0 = x;
	glutPostRedisplay();
}

//This function gets called for every frame. This will be used to animate the world 
void idle()
{
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = timeSinceStart - oldTimeSinceStart;
	oldTimeSinceStart = timeSinceStart;
}

