#include "window.h"

Window::Window(int xSize, int ySize, const char* title) :
		title(title), xSize(xSize), ySize(ySize) {
	setup();
}

Window::~Window() {
	glfwDestroyWindow(window);
}

/**
 * Sets up an OpeanGL-enabled window with GLFW. If the window could not be created, returns false.
 */
bool Window::setup() {
	window = glfwCreateWindow(this->xSize, this->ySize, this->title, NULL,
	NULL);
	if (window == NULL) {
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window); // create OpenGL context

	return true;
}

