#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>         // std::chrono::seconds

#include "vgl.h"
#include "LoadShaders.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\rotate_vector.hpp"
#include "..\SOIL\src\SOIL.h"
using namespace std::chrono;


float randomFloat(float a, float b);
void setUpPowerPacks();

//void setOneTank(char tankType, GLint l_program, glm::vec3 specificLocation);
void init(void);
void drawCube(float scale, GLuint* selectedTexture);
void displayWelcomeMessage();
void draw_level();
void updateScene(); // Update the Scene objects' position
//void generateProjectile(char objType, glm::vec3 start_location, glm::vec3 dir_vector);
bool checkForGameEnd();
void endGameResults();
void resetModelViewState();
void updateCameraState();



struct TextureInfo {
	int width;
	int height;
	unsigned char* imageData;
};


struct PlayerInfo {
	int life;
	int powerPacks;
	int enemyTouch;
	int enemyKills;
};

extern glm::vec3 cam_pos;
extern glm::vec3 looking_dir_vector;

const GLint NumBuffers = 8;
int numberOfTanks = 2;
int numberOfPowerPacks = 0;
const int Num_Obstacles = 1;

GLint playerLife = 1;
GLint enemyKills = 0;
PlayerInfo playerInfo = { playerLife, 0, 0, enemyKills };

high_resolution_clock::time_point enemyTime;
high_resolution_clock::time_point bulletTime;

bool checkIfWaitedTimeSeconds(high_resolution_clock::time_point startTime,GLint timeWait)
{

	auto timeSpawn = duration_cast<duration<double>>(high_resolution_clock::now() - startTime);
	return timeSpawn >= duration<double>(timeWait);
}

class GameObject
{
	GLchar type;  // 'e' for enemy, 'b' for bullet, 'p' for power
	glm::vec3 location;
	glm::vec3 look_at; // The 3D point in space that the must look
	glm::vec3 moving_direction;
	GLfloat colliderRadius;
	GLfloat movementSpeed;
	GLuint program;
	high_resolution_clock::time_point start_time;

public:
	// type, collisionRadius, playerLocation, looking_direction
	GameObject(GLchar l_type, GLfloat l_colliderRadius, glm::vec3 l_startLocation, glm::vec3 l_looking_dir_vec, GLfloat l_movementSpeed, GLuint l_program = 0, high_resolution_clock::time_point l_time = high_resolution_clock::now())
	{
		this->type = l_type;
		this->colliderRadius = l_colliderRadius;
		this->location = l_startLocation;
		this->look_at = l_looking_dir_vec;

		movementSpeed = l_movementSpeed;
		this->moving_direction = glm::vec3(0, 0, 0);

		this->program = l_program;

		this->start_time = l_time;
	}

	glm::vec3 getLocation() { return this->location; }

	bool checkIsBullet() { return 'b' == this->type; }

	bool checkIsEnemy() { return 'e' == this->type; }

	bool checkIsTankHero() { return 't' == this->type; }

	bool checkIsEnemyBullet() { return 'f' == this->type; }

	bool checkIsPowerPack() { return 'p' == this->type; }

	GLint getProgram() { return this->program; }

	glm::vec3 getLookAt() { return this->look_at; }

	void setLookAt(glm::vec3 newLookAt) { this->look_at = newLookAt; }

	bool isTwoOrMoreSeconds() 
	{
		auto timeSpawn = duration_cast<duration<double>>(high_resolution_clock::now() - this->start_time);
		return timeSpawn >= duration<double>(2);
	}

	bool isCollidingWithPlayer() {

		return (abs(this->location.x - cam_pos.x) <= this->colliderRadius) &&
			(abs(this->location.y - cam_pos.y) <= this->colliderRadius);
	}


	bool isCollidingWith(glm::vec3 otherLocation) {
		return (abs(this->location.x - otherLocation.x) <= this->colliderRadius) &&
			(abs(this->location.y - otherLocation.y) <= this->colliderRadius);
	}

	void incrementPosition() {
		//this->location += (glm::vec3(this->movementSpeed, this->movementSpeed, 0) * this->look_at);
		this->location += this->look_at * this->movementSpeed;
	}

	//void setNewLocationEnemyAI(GLfloat l_deltaTime) {
	void incrementEnemyLocation() {
		//glm::vec3 targetLocation = this->look_at - this->location;
		//this->location += (glm::vec3(this->movementSpeed, this->movementSpeed, 0) * glm::normalize(targetLocation));
		this->location += (glm::vec3(this->movementSpeed, this->movementSpeed, 0) * glm::normalize(this->look_at));
	}
};



//////////////////////////////////////////////////////////////////////////
//				START of Vertices and Texture Mapping
/////////////////////////////////////////////////////////////////////////

const GLuint NumVertices = 28;

GLfloat vertices[NumVertices][3] = {

	{ -100.0, -100.0, 0.0 }, //Plane to walk on and a sky
	{ 100.0, -100.0, 0.0 },
	{ 100.0, 100.0, 0.0 },
	{ -100.0, 100.0, 0.0 },

	{ -0.45, -0.45 ,0.01 }, // bottom face
	{ 0.45, -0.45 ,0.01 },
	{ 0.45, 0.45 ,0.01 },
	{ -0.45, 0.45 ,0.01 },

	{ -0.45, -0.45 ,0.9 }, //top face
	{ 0.45, -0.45 ,0.9 },
	{ 0.45, 0.45 ,0.9 },
	{ -0.45, 0.45 ,0.9 },

	{ 0.45, -0.45 , 0.01 }, //left face
	{ 0.45, 0.45 , 0.01 },
	{ 0.45, 0.45 ,0.9 },
	{ 0.45, -0.45 ,0.9 },

	{ -0.45, -0.45, 0.01 }, //right face
	{ -0.45, 0.45 , 0.01 },
	{ -0.45, 0.45 ,0.9 },
	{ -0.45, -0.45 ,0.9 },

	{ -0.45, 0.45 , 0.01 }, //front face
	{ 0.45, 0.45 , 0.01 },
	{ 0.45, 0.45 ,0.9 },
	{ -0.45, 0.45 ,0.9 },

	{ -0.45, -0.45 , 0.01 }, //back face
	{ 0.45, -0.45 , 0.01 },
	{ 0.45, -0.45 ,0.9 },
	{ -0.45, -0.45 ,0.9 },
};

//These are the texture coordinates for the second texture
GLfloat textureCoordinates[28][2] = { // TODO: TEXTURE
	// -------------------- grass texture map
	0.0f, 0.0f,
	200.0f, 0.0f,
	200.0f, 200.0f,
	0.0f, 200.0f,

	// -------------------- brick texture map
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,

	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,

	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,

	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,

	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,

	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
};



enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer };
enum Attrib_IDs { vPosition = 0 };




GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
GLuint location;
GLuint cam_mat_location;
GLuint proj_mat_location;
GLuint texture[NumBuffers];	// <------------ TEXTURE -> Array of pointers to texture data in VRAM.


// Height of camera (player) from the level
float height = 0.8f;
float top_height = 23.0f;
float initial_pos_x = -5.0f;
float initial_pos_y = -5.0f;
//Player motion speed for movement and pitch/yaw
float travel_speed = 100.0f;		//Motion speed
float mouse_sensitivity = 0.01f;	//Pitch/Yaw speed


GLfloat degreeAngle = 90.0;
GLfloat theta = degreeAngle * 0.0174533; // theta will be used to reposition the location of 2 other sides of the cube



//Used for tracking mouse cursor position on screen
int x0 = 0;
int y_0 = 0;

//Transformation matrices and camera vectors
glm::mat4 model_view;
glm::vec3 unit_z_vector = glm::vec3(0, 0, 1);	//Assigning a meaningful name to (0,0,1) :-)
glm::vec3 cam_pos = glm::vec3(0.0f, 0.0f, height);
glm::vec3 forward_vector = glm::vec3(1, 1, 0);	//Forward vector is parallel to the level at all times (No pitch)

//The direction which the camera is looking, at any instance
glm::vec3 looking_dir_vector = glm::vec3(1, 1, 0);
glm::vec3 up_vector = unit_z_vector;
glm::vec3 side_vector = glm::cross(up_vector, forward_vector);


//Used to measure time between two frames
int oldTimeSinceStart = 0;
int deltaTime;

//Creating and rendering bunch of objects on the scene to interact with
float obstacle_data[Num_Obstacles][3];

GLfloat alpha = 0; // Rationale: value that is an angle that keeps being calculated for every frame in Idle function

std::vector<GameObject> sceneGraph;



//Helper function to generate a random float number within a range
float randomFloat(float a, float b)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

// Setup power packs initial locations
void setUpPowerPacks()
{
	for (size_t i = 0; i < numberOfPowerPacks; i++)
	{
		GLfloat randX = randomFloat(-20, 20);
		GLfloat randY = randomFloat(-20, 20);

		glm::vec3 powerPos = glm::vec3(randX, randY, 0.0); // Random Position for power

		sceneGraph.push_back(GameObject('p', 2, powerPos, cam_pos, 0.0009)); // look_at -> to the camera position
	}
}

void setOneTank(char tankType, GLint l_program, glm::vec3 specificLocation)
{
	GLfloat randX, randY, distanceRadius = 30;
	glm::vec3 tankDirection;

	// random Direction
	randX = randomFloat(-distanceRadius, distanceRadius);
	randY = randomFloat(-distanceRadius, distanceRadius);
	tankDirection = glm::vec3(randX, randY, 0.0); // Random Direction for power

	// random Speed
	GLfloat randomSpeed = randomFloat(0.0009, 0.002);

	sceneGraph.push_back(GameObject(tankType, 2, specificLocation, tankDirection, randomSpeed, l_program)); // look_at -> to the camera position
}


// inititializing buffers, coordinates, setting up pipeline, etc.
void init(void)
{
	glEnable(GL_DEPTH_TEST);

	//Normalizing all vectors
	up_vector = glm::normalize(up_vector);
	forward_vector = glm::normalize(forward_vector);
	looking_dir_vector = glm::normalize(looking_dir_vector);
	side_vector = glm::normalize(side_vector);

	//Randomizing the position and scale of obstacles
	for (int i = 0; i < Num_Obstacles; i++)
	{
		obstacle_data[i][0] = randomFloat(-50, 50) + 20; //X
		obstacle_data[i][1] = randomFloat(-50, 50) + 20; //Y
		obstacle_data[i][2] = randomFloat(0.1, 10.0); //Scale
	}

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);	//My Pipeline is set up



	// TEXTURE
	//Creating our texture: 
	//This texture is loaded from file. To do this, we use the SOIL (Simple OpenGL Imaging Library) library.
	//When using the SOIL_load_image() function, make sure the you are using correct patrameters, or else, your image will NOT be loaded properly, or will not be loaded at all.
	TextureInfo texturesInfo[NumBuffers];
	texturesInfo[0].imageData = SOIL_load_image("grass.png", &texturesInfo[0].width, &texturesInfo[0].height, 0, SOIL_LOAD_RGB);
	texturesInfo[1].imageData = SOIL_load_image("wall.jpg", &texturesInfo[1].width, &texturesInfo[1].height, 0, SOIL_LOAD_RGB);
	texturesInfo[2].imageData = SOIL_load_image("energy.jpg", &texturesInfo[2].width, &texturesInfo[2].height, 0, SOIL_LOAD_RGB);
	texturesInfo[3].imageData = SOIL_load_image("fire.jpg", &texturesInfo[3].width, &texturesInfo[3].height, 0, SOIL_LOAD_RGB);
	texturesInfo[4].imageData = SOIL_load_image("demon.jpg", &texturesInfo[4].width, &texturesInfo[4].height, 0, SOIL_LOAD_RGB);
	texturesInfo[5].imageData = SOIL_load_image("ammo.png", &texturesInfo[5].width, &texturesInfo[5].height, 0, SOIL_LOAD_RGB);
	texturesInfo[6].imageData = SOIL_load_image("skyNight.jpg", &texturesInfo[6].width, &texturesInfo[6].height, 0, SOIL_LOAD_RGB);
	texturesInfo[7].imageData = SOIL_load_image("grass2.png", &texturesInfo[7].width, &texturesInfo[7].height, 0, SOIL_LOAD_RGB);


	// ------------- SET SHADERS
	glGenBuffers(NumBuffers, Buffers);

	// Set the VERTICES shader
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindAttribLocation(program, 0, "vPosition");
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	// set the TEXTURES shaders
	for (size_t i = 1; i < NumBuffers; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinates), textureCoordinates, GL_STATIC_DRAW);
		glBindAttribLocation(program, 1, "vTexCoord");
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glEnableVertexAttribArray(1);
	}
	// ---------------------------------------------

	location = glGetUniformLocation(program, "model_matrix");
	cam_mat_location = glGetUniformLocation(program, "camera_matrix");
	proj_mat_location = glGetUniformLocation(program, "projection_matrix");

	///////////////////////TEXTURE SET UP////////////////////////

	//Allocating buffers in VRAM
	glGenTextures(NumBuffers, texture); // <-------------------------- TEXTURES

	// Set the textures' Info
	for (size_t i = 0; i < NumBuffers; i++)
	{
		//Set the type of the allocated buffer as "TEXTURE_2D"
		glBindTexture(GL_TEXTURE_2D, texture[i]);

		//Loading the second texture into the second allocated buffer:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texturesInfo[i].width, texturesInfo[i].height, 0, GL_RGB, GL_UNSIGNED_BYTE, texturesInfo[i].imageData);

		//Setting up parameters for the texture that recently pushed into VRAM
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}


	//setUpPowerPacks();
	//setUpTanks(program); // enemies
	enemyTime = high_resolution_clock::now();
	bulletTime = high_resolution_clock::now();
	setOneTank('t', 0, glm::vec3(0,0,0));
	setOneTank('e', 0, glm::vec3(5,5,0));
	//displayWelcomeMessage();

}

void displayWelcomeMessage()
{
	std::cout << "///////////////////////////////////////////////////" << std::endl;
	std::cout << "///////////////    GAME STARTED    ////////////////" << std::endl;
	std::cout << "///////////////////////////////////////////////////" << std::endl;
	std::cout << std::endl;
	std::cout << "---------   GAME RULES   ------------------------." << std::endl;
	std::cout << "| - YOU WIN     ->   Kill 10 Tanks               |" << std::endl;
	std::cout << "| - YOU LOOSE   ->   Collide with a Tank         |" << std::endl;
	std::cout << "|                                                |" << std::endl;
	std::cout << "| - Press \"f\" ->   Fire Bullet to Kill           |" << std::endl;
	std::cout << "-------------------------------------------------." << std::endl;
	std::cout << std::endl;
	std::cout << "PLAYER's START LIFE: " << playerInfo.life << std::endl;
}


//Helper function to draw a cube
void drawCube(float scale, GLuint* selectedTexture)
{
	model_view = glm::scale(model_view, glm::vec3(scale, scale, scale));
	glUniformMatrix4fv(location, 1, GL_FALSE, &model_view[0][0]);

	//Select the texture designated
	glBindTexture(GL_TEXTURE_2D, *selectedTexture);
	glDrawArrays(GL_QUADS, 4, 24);
}


void drawTank(GameObject tank, float scale, GLuint* tankTexture, GLuint* wheelTexture)
{
	glm::mat4 backup_body = model_view;
	model_view = glm::mat4(1.0);

	model_view = glm::translate(model_view, tank.getLocation()); // move tank to new position
	model_view = glm::scale(model_view, glm::vec3(2, 1, 1)); // scale for tank body

	// draw tank body
	drawCube(2.0, tankTexture);
		
	// render every children and their new position
	for (size_t i = 0; i < 4; i++)
	{
		glm::mat4 backup_wheel = model_view;
		glm::vec3 wheelInitialPos;
		GLfloat wheelDistance = 0.5;
		switch (i) {
		case 0:
			wheelInitialPos = glm::vec3(-wheelDistance, wheelDistance, 0);
			break;
		case 1:
			wheelInitialPos = glm::vec3(-wheelDistance, -wheelDistance, 0);
			break;
		case 2:
			wheelInitialPos = glm::vec3(wheelDistance, -wheelDistance, 0);
			break;
		case 3:
			wheelInitialPos = glm::vec3(wheelDistance, wheelDistance, 0);
			break;
		};
		
		// rotate wheel
		glm::vec3 unit_z_vector = glm::vec3(0, 0, 1);
		model_view = glm::translate(model_view, wheelInitialPos);
		model_view = glm::rotate(model_view, alpha / 2, unit_z_vector);
		drawCube(0.5, wheelTexture);
		model_view = backup_wheel;
	}

	model_view = backup_body;
}


//Renders level
void draw_level()
{
	//Select the first texture (grass.png) when drawing the first geometry (floor)
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glDrawArrays(GL_QUADS, 0, 4);

	//Rendering obstacles obstacles
	for (int i = 0; i < Num_Obstacles; i++)
	{
		model_view = glm::translate(model_view, glm::vec3(obstacle_data[i][0], obstacle_data[i][1], 0.0));
		glUniformMatrix4fv(location, 1, GL_FALSE, &model_view[0][0]);
		drawCube(obstacle_data[i][2], &texture[1]); // <------
		model_view = glm::mat4(1.0);
	}

	model_view = glm::translate(model_view, glm::vec3(0, 0, -50.0));
	drawCube(800, &texture[6]);    // night sky
	model_view = glm::mat4(1.0);
}

void generateProjectile(char objType, glm::vec3 start_location, glm::vec3 dir_vector)
{
	glm::vec3 bulletStartLocation = start_location * glm::vec3(1, 1, 0.0);
	GLfloat collisionRadius = 2;
	GLfloat bullSpeed = 0.0005;

	sceneGraph.push_back(GameObject(objType, collisionRadius, bulletStartLocation, dir_vector, bullSpeed));
}



// Rationale: the code bellow will draw all the objects in the scene that are listed in the gameScene vector
//            and each object have your own specific position
// texture[0] - grass
// texture[1] - wall
// texture[2] - energy
// texture[3] - fire (players bullet)
// texture[4] - demon
// texture[5] - ammo (deamons bullet)
// texture[6] - Night Sky
void updateScene()
{
	std::vector<int> indexesToErase;
	bool playerBulletCollides = false;

	if (checkIfWaitedTimeSeconds(enemyTime, 3)) // Spawn Enemy Tanks every 3 seconds
	{
		enemyTime = high_resolution_clock::now();

		// random Spawn location
		GLfloat distanceRadius = 30;
		GLfloat randX = randomFloat(-distanceRadius, distanceRadius);
		GLfloat randY = randomFloat(-distanceRadius, distanceRadius);
		glm::vec3 tankLocation = glm::vec3(randX, randY, 0.0);

		setOneTank('e', 0, tankLocation);
	}

	for (size_t i = 0; i < sceneGraph.size(); i++)
	{
		model_view = glm::translate(model_view, sceneGraph[i].getLocation()); // move the object to its own location x and y (z will be 0)
		glUniformMatrix4fv(location, 1, GL_FALSE, &model_view[0][0]);

		model_view = glm::rotate(model_view, alpha / 8, unit_z_vector);

		if (sceneGraph[i].checkIsBullet()) // <------- [ BULLET PLAYER ]
		{
			drawCube(1, &texture[2]);
			model_view = glm::mat4(1.0);

			for (size_t j = 0; j < sceneGraph.size() && !playerBulletCollides; j++)
			{
				if (i != j && sceneGraph[j].checkIsEnemy())
				{
					if (sceneGraph[i].isCollidingWith(sceneGraph[j].getLocation()))  // erase if collision happen
					{
						indexesToErase.push_back(i); // will erase player's bullet
						indexesToErase.push_back(j); // will erase enemy
						playerBulletCollides = true;

						playerInfo.enemyKills++;
						std::cout << "----------> TANK HERO [ Killed Enemy ]: " << playerInfo.enemyKills << std::endl;

					}
				}
			}
			if (!playerBulletCollides) sceneGraph[i].incrementPosition(); // increment bullet position
			
		}
		else if (sceneGraph[i].checkIsTankHero()) // <------- [ TANK HERO ]
		{
			drawTank(sceneGraph[i], 2, &texture[5], &texture[2]);
			model_view = glm::mat4(1.0);

			if (checkIfWaitedTimeSeconds(bulletTime, 5)) // Spawn Bullet Hero Tank every 5 seconds
			{
				bulletTime = high_resolution_clock::now();
				bool continueLoop = true;
				for (size_t j = 0; j < sceneGraph.size() && continueLoop; j++)
				{
					if (sceneGraph[j].checkIsEnemy()) 
					{
						sceneGraph[i].setLookAt(sceneGraph[j].getLocation()); // make bullet Hero to aim the first enemy in the list
						continueLoop = false;
					}
				}
				generateProjectile('b', sceneGraph[i].getLocation(), sceneGraph[i].getLookAt()); // spawn the hero's bullet
			}

		} 
		else // <------- [ ENEMY ] 
		{
			drawTank(sceneGraph[i], 2, &texture[4], &texture[3]);
			model_view = glm::mat4(1.0);

			// Update ENEMY Location
			sceneGraph[i].incrementEnemyLocation();

		} 

	}


	// Erase overlaped objects from the scene
	for (auto& index : indexesToErase)
	{
		sceneGraph.erase(sceneGraph.begin() + index);
	}


}



void display(void)
{
	resetModelViewState();

	updateCameraState();

	draw_level();

	updateScene();

	glFlush();

	if (checkForGameEnd())
	{
		endGameResults();
		glutDestroyWindow(glutGetWindow()); // close the window once GAME OVER
	}
}

// Check if the game needs to end (checks through the player info state)
bool checkForGameEnd()
{
	GLint noLife = 0;
	GLint numEnemiesKilled = 10;
	return playerInfo.life <= noLife || playerInfo.enemyKills == numEnemiesKilled;
}

void endGameResults()
{
	std::cout << std::endl;
	std::cout << "-------------------> GAME OVER <--------------------" << std::endl;
	std::cout << "____________________________________________________" << std::endl;
	std::cout << "|                    OVER VIEW                     |" << std::endl;
	std::cout << "| Player Life: " << playerInfo.life << " HP" << std::endl;
	std::cout << "| Player Got: " << playerInfo.powerPacks << " Power Packs" << std::endl;
	std::cout << "| Player Killed: " << playerInfo.enemyKills << " Enemies" << std::endl;
	std::cout << "| Player Was Touched: " << playerInfo.enemyTouch << " times by Enemies" << std::endl;
	std::cout << "|__________________________________________________|" << std::endl;
	std::cout << std::endl;
}

void resetModelViewState()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	model_view = glm::mat4(1.0);
	glUniformMatrix4fv(location, 1, GL_FALSE, &model_view[0][0]);
}

void updateCameraState()
{
	//The 3D point in space that the camera is looking
	glm::vec3 look_at = cam_pos + looking_dir_vector;

	glm::mat4 camera_matrix = glm::lookAt(cam_pos, look_at, up_vector);
	glUniformMatrix4fv(cam_mat_location, 1, GL_FALSE, &camera_matrix[0][0]);

	glm::mat4 proj_matrix = glm::frustum(-0.01f, +0.01f, -0.01f, +0.01f, 0.01f, 1000.0f);
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, &proj_matrix[0][0]);
}


void keyboard(unsigned char key, int x, int y)
{
	if (key == 'a')
	{
		//Moving camera along opposit direction of side vector
		cam_pos += side_vector * travel_speed * ((float)deltaTime) / 1000.0f;
	}
	if (key == 'd')
	{
		//Moving camera along side vector
		cam_pos -= side_vector * travel_speed * ((float)deltaTime) / 1000.0f;
	}
	if (key == 'w')
	{
		//Moving camera along forward vector. To be more realistic, we use X=V.T equation in physics
		cam_pos += forward_vector * travel_speed * ((float)deltaTime) / 1000.0f;
	}
	if (key == 's')
	{
		//Moving camera along backward (negative forward) vector. To be more realistic, we use X=V.T equation in physics
		cam_pos -= forward_vector * travel_speed * ((float)deltaTime) / 1000.0f;
	}


	// Fire object in a specific direction
	if (key == 'f')
	{
		generateProjectile('b', cam_pos, looking_dir_vector); // 'b' - means player bullet
	}

	// Change to top view
	if (key == 't') {
		cam_pos = glm::vec3(cam_pos.x, cam_pos.y, top_height); // make the height of the camera to be very heigh at the center of the screen
		forward_vector = glm::rotate(forward_vector, theta, side_vector); // rotate the camera forward vector about 90 degrees the side_vector
	}

	// Change to the perspective view
	if (key == 'p') {
		cam_pos = glm::vec3(cam_pos.x, cam_pos.y, height);  // put back the camera position and height as the initial
		forward_vector = glm::rotate(forward_vector, -theta, side_vector); // turn back the camera rotation about the side vector to be as initial
	}
}

//Controlling Pitch with vertical mouse movement
void mouse(int x, int y)
{
	//Controlling Yaw with horizontal mouse movement
	int delta_x = x - x0;

	//The following vectors must get updated during a yaw movement
	forward_vector = glm::rotate(forward_vector, -delta_x * mouse_sensitivity, unit_z_vector);
	looking_dir_vector = glm::rotate(looking_dir_vector, -delta_x * mouse_sensitivity, unit_z_vector);
	side_vector = glm::rotate(side_vector, -delta_x * mouse_sensitivity, unit_z_vector);
	up_vector = glm::rotate(up_vector, -delta_x * mouse_sensitivity, unit_z_vector);
	x0 = x;

	//The following vectors must get updated during a pitch movement
	int delta_y = y - y_0;
	glm::vec3 tmp_up_vec = glm::rotate(up_vector, delta_y * mouse_sensitivity, side_vector);
	glm::vec3 tmp_looking_dir = glm::rotate(looking_dir_vector, delta_y * mouse_sensitivity, side_vector);

	//The dot product is used to prevent the user from over-pitch (pitching 360 degrees)
	//The dot product is equal to cos(theta), where theta is the angle between looking_dir and forward vector
	GLfloat dot_product = glm::dot(tmp_looking_dir, forward_vector);

	//If the angle between looking_dir and forward vector is between (-90 and 90) degress 
	if (dot_product > 0)
	{
		up_vector = glm::rotate(up_vector, delta_y * mouse_sensitivity, side_vector);
		looking_dir_vector = glm::rotate(looking_dir_vector, delta_y * mouse_sensitivity, side_vector);
	}
	y_0 = y;
}

void idle()
{
	//Calculating the delta time between two frames
	//We will use this delta time when moving forward (in keyboard function)
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = timeSinceStart - oldTimeSinceStart;
	oldTimeSinceStart = timeSinceStart;


	// Rationale: the code bellow is being used to calculate for each frame the new value for the angle alpha, 
	//            in order to keep rotating the objects that the user adds to the scene
	GLfloat omega = 10.0f;	// let omega  be the angular speed for Global Axes rotation
	GLfloat rotatetimeSinceStart = glutGet(GLUT_ELAPSED_TIME) / 1000.f;		//Retreives the time in seconds
	alpha = omega * rotatetimeSinceStart;		// We use alpha = omega * time to calculate the amount of rotation of the geometry in this frame
	// ---------

	glutPostRedisplay();
}



int main(int argc, char** argv)
{
	srand(time(0));  // ensure that every sngle time will be randomized 
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(1024, 1024);
	glutCreateWindow("Game Assignment - Arthur Dibe");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	glutKeyboardFunc(keyboard);

	glutIdleFunc(idle);

	glutPassiveMotionFunc(mouse);

	glutMainLoop();

}