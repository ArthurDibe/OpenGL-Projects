#version 430 core
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec2 vTexCoord;

uniform mat4 model_matrix;
uniform mat4 camera_matrix;
uniform mat4 projection_matrix;

out vec2 texCoord;

void main()
{
	gl_Position = projection_matrix * camera_matrix * model_matrix * vPosition;
	texCoord = vTexCoord;
}
