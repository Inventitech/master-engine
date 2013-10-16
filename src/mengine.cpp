#include <stdio.h>
#include <stdlib.h>

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "triangle.h"
#include "window.h"

using namespace glm;

GLuint program;
GLint attribute_coord3d, attribute_v_color;
GLuint vbo_triangle, vbo_triangle_colors;
GLint uniform_m_transform;
Window* window;

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

bool initGL() {

	glfwSetErrorCallback(error_callback);
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	window = new Window(640,480,"Master Engine!111");

	// Initialize GLEW
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		return false;
	}
	if (!GLEW_VERSION_2_0) {
		fprintf(stderr,
				"Error: your graphic card does not support OpenGL 2.0\n");
		return false;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window->getGLFWWindow(), GLFW_STICKY_KEYS, GL_TRUE);

	glfwSetKeyCallback(window->getGLFWWindow(), key_callback);
	return true;
}

bool initData() {

	program = LoadShaders("src/shaders/vertexshader.glsl",
			"src/shaders/fragmentshader.glsl");

	struct attributes triangle_attributes[] = { { { 0.0, 0.8, 0.0 }, { 1.0, 1.0,
			0.0 } }, { { -0.8, -0.8, 0.0 }, { 0.0, 0.0, 1.0 } }, { { 0.8, -0.8,
			0.0 }, { 1.0, 0.0, 0.0 } } };

	// Setup vertex shader attribute
	const char* attribute_name = "coord3d";
	attribute_coord3d = glGetAttribLocation(program, attribute_name);
	if (attribute_coord3d == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return false;
	}

	attribute_name = "m_transform";
	uniform_m_transform = glGetUniformLocation(program, attribute_name);
	if (uniform_m_transform == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", attribute_name);
		return 0;
	}

	// Setup vertex shader attribute
	attribute_name = "v_color";
	attribute_v_color = glGetAttribLocation(program, attribute_name);
	if (attribute_v_color == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return false;
	}

	glGenBuffers(1, &vbo_triangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_attributes),
			triangle_attributes,
			GL_STATIC_DRAW);

	return true;
}

void render() {
	// Enable alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	double elapsedTime = glfwGetTime();
	float move = sinf(elapsedTime  * (2 * 3.14) / 5); // -1<->+1 every 5 seconds
	float angle = elapsedTime  * 10;  // 45° per second
	glm::vec3 axis_z(0, 0, 1);
	glm::mat4 m_transform = glm::rotate(glm::mat4(1.0f), angle, axis_z) *  glm::translate(glm::mat4(1.0f),
			glm::vec3(move, 0.0, 0.0))
			;
	glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE,
			glm::value_ptr(m_transform));

	/* Clear the background as white */
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);

	glEnableVertexAttribArray(attribute_v_color);
	glEnableVertexAttribArray(attribute_coord3d);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);

	/* Describe our colors array to OpenGL (it can't guess its format automatically) */
	glVertexAttribPointer(attribute_v_color,   // attribute
			3,                   // number of elements per vertex, here (x,y,z)
			GL_FLOAT,            // the type of each element
			GL_FALSE,            // take our values as-is
			sizeof(struct attributes),  // next coord3d appears every 6 floats
			(GLvoid*) offsetof(struct attributes, v_color) // offset of first element
					);

	/* Describe our vertices array to OpenGL (it can't guess its format automatically) */
	glVertexAttribPointer(attribute_coord3d,   // attribute
			3,                   // number of elements per vertex, here (x,y,z)
			GL_FLOAT,            // the type of each element
			GL_FALSE,            // take our values as-is
			sizeof(struct attributes),  // next coord3d appears every 6 floats
			0                    // offset of first element
			);

	/* Push each element in buffer_vertices to the vertex shader */
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(attribute_v_color);
	glDisableVertexAttribArray(attribute_coord3d);

}

void freeMemory() {
	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo_triangle);
	delete window;
}

int main(void) {

	if (!initGL()) {
		return EXIT_FAILURE;
	}

	if (!initData()) {
		return EXIT_FAILURE;
	}

	while (!glfwWindowShouldClose(window->getGLFWWindow())) {
		render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window->getGLFWWindow());
		glfwPollEvents();
	}


	freeMemory();
	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
