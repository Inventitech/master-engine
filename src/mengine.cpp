#include <stdio.h>
#include <stdlib.h>

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.h"

using namespace glm;

GLuint program;
GLint attribute_coord3d;

#pragma GCC diagnostic ignored "-Wunused-parameter"
static void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
static void key_callback(GLFWwindow* window, int key, int scancode, int action,
		int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void render() {
	/* Clear the background as white */
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);
	glEnableVertexAttribArray(attribute_coord3d);
	GLfloat triangle_vertices[] = { -0.5, 0.0, 0.0, -0.8, -0.8, 0.0, 0.5, -0.5,
			0.0, 0.5, 0.0, 0.0, -0.5, 0.0, 0.0, 0.5, -0.5, 0.0 };
	/* Describe our vertices array to OpenGL (it can't guess its format automatically) */
	glVertexAttribPointer(attribute_coord3d, // attribute
			3,                 // number of elements per vertex, here (x,y)
			GL_FLOAT,          // the type of each element
			GL_FALSE,          // take our values as-is
			0,                 // no extra data between each position
			triangle_vertices  // pointer to the C array
			);

	/* Push each element in buffer_vertices to the vertex shader */
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(attribute_coord3d);

}

int main(void) {
	glfwSetErrorCallback(error_callback);
	glfwInit();

	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	// Open a window and create its OpenGL context
	GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		return -1;
	}
	if (!GLEW_VERSION_2_0) {
		fprintf(stderr,
				"Error: your graphic card does not support OpenGL 2.0\n");
		return 1;
	}

	glfwSetWindowTitle(window, "Master Engine!111");

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glfwSetKeyCallback(window, key_callback);

	program = LoadShaders("src/shaders/vertexshader.glsl",
			"src/shaders/fragmentshader.glsl");

	// Setup vertex shader attribute
	const char* attribute_name = "coord3d";
	attribute_coord3d = glGetAttribLocation(program, attribute_name);
	if (attribute_coord3d == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}

	while (!glfwWindowShouldClose(window)) {
		render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);

	glDeleteProgram(program);
	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
