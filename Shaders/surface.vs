#version 330

uniform mat4 V;
uniform mat4 P;

layout(location = 0) in vec3 inVertex;


void main()
{
	gl_Position = P * V * vec4(inVertex, 1.0);
}
