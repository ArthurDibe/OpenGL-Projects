#version 430 core
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vertexColor;

out vec4 myColor;

void main()
{
	myColor = vertexColor;
	gl_Position = vPosition;

}
