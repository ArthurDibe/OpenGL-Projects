#ifndef CUBE_H
#define CUBE_H

#include "vgl.h"

const GLuint NumVertices = 6 * 6; // 6 SIDES, 6 vertices each cube's side [total 36 vertices] 
// We use two triangles and connect them together to get a square.
GLfloat cube[NumVertices][3] = {

	// ----------- FIRST Square ---------------------
	// First triangle
	{ -0.5, 0.5, -0.5},
	{ 0.5, 0.5, -0.5},
	{ 0.5, -0.5, -0.5},

	// Second triangle
	{ 0.5, -0.5, -0.5},
	{ -0.5, -0.5, -0.5},
	{ -0.5, 0.5, -0.5},


	// ----------- SECOND Square ---------------------
	// First triangle
	{ -0.5, 0.5 ,0.5},
	{ 0.5, 0.5 ,0.5},
	{ 0.5, -0.5 ,0.5},

	// Second triangle
	{ 0.5, -0.5 , 0.5},
	{ -0.5, -0.5 , 0.5},
	{ -0.5, 0.5 , 0.5},

	// ----------- THIRD Square ---------------------
	// First triangle
	{ -0.5, 0.5, -0.5},
	{ 0.5, 0.5, -0.5},
	{ 0.5, -0.5, -0.5},

	// Second triangle
	{ 0.5, -0.5, -0.5},
	{ -0.5, -0.5, -0.5},
	{ -0.5, 0.5, -0.5},


	// ----------- FOURTH Square ---------------------
	// First triangle
	{ -0.5, 0.5 ,0.5},
	{ 0.5, 0.5 ,0.5},
	{ 0.5, -0.5 ,0.5},

	// Second triangle
	{ 0.5, -0.5 , 0.5},
	{ -0.5, -0.5 , 0.5},
	{ -0.5, 0.5 , 0.5},

	// ----------- FIFTH Square ---------------------
	// First triangle
	{ -0.5, 0.5, -0.5},
	{ 0.5, 0.5, -0.5},
	{ 0.5, -0.5, -0.5},

	// Second triangle
	{ 0.5, -0.5, -0.5},
	{ -0.5, -0.5, -0.5},
	{ -0.5, 0.5, -0.5},


	// ----------- SIXTH Square ---------------------
	// First triangle
	{ -0.5, 0.5 ,0.5},
	{ 0.5, 0.5 ,0.5},
	{ 0.5, -0.5 ,0.5},

	// Second triangle
	{ 0.5, -0.5 , 0.5},
	{ -0.5, -0.5 , 0.5},
	{ -0.5, 0.5 , 0.5},


};

GLfloat cubeColors[NumVertices][3] = {
	// --------------- FIRST Square color ---------------------------
	{ 1,0,0 }, // RED - color for vertices of the triangle 1
	{ 1,0,0 },
	{ 1,0,0 },

	{ 1,0,0 }, // RED - color for vertices of the triangle 2
	{ 1,0,0 },
	{ 1,0,0 },

	// --------------- SECOND Square color ---------------------------
	{ 0,1,0 }, // GREEN - color for vertices of the triangle 3
	{ 0,1,0 },
	{ 0,1,0 },

	{ 0,1,0 }, // GREEN - color for vertices of the triangle 4
	{ 0,1,0 },
	{ 0,1,0 },

	// --------------- THIRD Square color ---------------------------
	{ 0,0,1 }, // BLUE - color for vertices of the triangle 5
	{ 0,0,1 },
	{ 0,0,1 },

	{ 0,0,1 }, // BLUE - color for vertices of the triangle 6
	{ 0,0,1 },
	{ 0,0,1 },

	// --------------- FOURTH Square color ---------------------------
	{ 0,1,1 }, // LIGHT BLUE - color for vertices of the triangle 7
	{ 0,1,1 },
	{ 0,1,1 },

	{ 0,1,1 }, // LIGHT BLUE - color for vertices of the triangle 8
	{ 0,1,1 },
	{ 0,1,1 },

	// --------------- FIFTH Square color ---------------------------
	{ 1,0,1 }, //  - color for vertices of the triangle 7
	{ 1,0,1 },
	{ 1,0,1 },

	{ 1,0,1 }, //  - color for vertices of the triangle 8
	{ 1,0,1 },
	{ 1,0,1 },

	// --------------- SIXTH Square color ---------------------------
	{ 1,1,0 }, //  - color for vertices of the triangle 7
	{ 1,1,0 },
	{ 1,1,0 },

	{ 1,1,0 }, //  - color for vertices of the triangle 8
	{ 1,1,0 },
	{ 1,1,0 },

};

#endif
