#pragma once
#include <GLFW/glfw3.h>

struct Timer
{
	GLfloat currTime = (GLfloat)glfwGetTime();
	GLfloat lastTime = 0;

	void start() { lastTime = currTime; }

	void stop() { currTime = (GLfloat)glfwGetTime(); }

	GLfloat getElapsedTime() const { return currTime - lastTime; }
};
