#ifndef OBJTEXT_H
#define OBJTEXT_H

#include "vgl.h"

const GLuint NumVertices = 28;

//Since we use texture mapping, to simplify the task of texture mapping, 
	//and to clarify the demonstration of texture mapping, we consider 4 vertices per face.
	//Overall, we will have 24 vertices and we have 4 vertices to render the sky (a large square).
	//Therefore, we'll have 28 vertices in total.
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

#endif