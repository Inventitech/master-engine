#include "window.h"

Window::Window(int xSize, int ySize, const char* title) :
		title(title), xSize(xSize), ySize(ySize) {
	setup();
}

Window::~Window() {
	glfwDestroyWindow(window);
}


#pragma GCC diagnostic ignored "-Wunused-parameter"
static void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
static void window_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

/*
 * Sets up an OpeanGL-enabled window with GLFW. Furthermore, it sets an error
 * a window resize, and a key callback. Returns false, if the window could
 * not be created.
 */
bool Window::setup() {
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	window = glfwCreateWindow(this->xSize, this->ySize, this->title, NULL,
	NULL);
	if (window == NULL) {
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window); // create OpenGL context

	glfwSetErrorCallback(error_callback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetWindowSizeCallback(window, window_callback);

	return true;
}

