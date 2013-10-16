#include "window.h"

Window::Window(int xSize, int ySize, const char* title) :
		title(title), xSize(xSize), ySize(ySize) {
	setup();
}

Window::~Window() {
	glfwDestroyWindow(window);
}

/**
 * Sets up an OpenGL-enabled window with GLFW. If the window could not be created, returns false.
 */
bool Window::setup() {
	// Open a window and create its OpenGL context
	window = glfwCreateWindow(this->xSize, this->ySize, this->title, NULL,
	NULL);
	if (window == NULL) {
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	return true;
}

