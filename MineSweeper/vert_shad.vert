#version 330
layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTextureCoords;

out vec2 xTextureCoords;

uniform mat4 model;


void main()
{
	gl_Position = model * vec4(aPosition, 0, 1);
	xTextureCoords = aTextureCoords;
}
