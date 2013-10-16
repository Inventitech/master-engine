/*
 * window.cpp
 *
 *  Created on: Oct 16, 2013
 *      Author: nihathrael
 */

#include "window.h"

Window::Window(int xSize, int ySize, const char* title) {
	this->xSize = xSize;
	this->ySize = ySize;
	this->title = title;
	this->fullScreen = fullScreen;
	setup();
}

Window::~Window() {
	glfwDestroyWindow(window);
}

bool Window::setup() {
	// Open a window and create its OpenGL context
	window = glfwCreateWindow(this->xSize, this->ySize, this->title, NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
//	glfwSetWindowTitle(window, "Master Engine!111");

	return true;
}


