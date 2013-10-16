/*
 * window.h
 *
 *  Created on: Oct 16, 2013
 *      Author: nihathrael
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
	GLFWwindow* window;
	const char* title;
	bool setup();
public:
	int xSize;
	int ySize;
	Window(int xSize, int ySize, const char* title);
	GLFWwindow* getGLFWWindow() { return this->window; };
	virtual ~Window();
};

#endif /* WINDOW_H_ */
