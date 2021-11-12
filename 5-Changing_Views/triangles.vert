#version 430 core
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vertexColor;

uniform mat4 model_view;
uniform mat4 camera;
uniform mat4 projection;

out vec4 myColor;

void main()
{
	myColor = vertexColor;
	gl_Position = projection * camera * model_view * vPosition;
}
