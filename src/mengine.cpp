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
#include "shaderprogram.h"

using namespace glm;

GLint attribute_coord3d, attribute_v_color;
Window* window;
ShaderProgram* shaderProgram;
GLuint vbo_cube_vertices, vbo_cube_colors;
GLuint ibo_cube_elements;
GLint uniform_mvp;

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

#pragma GCC diagnostic ignored "-Wunused-parameter"
static void window_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

bool initGL() {

	glfwSetErrorCallback(error_callback);
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	window = new Window(640, 480, "Master Engine!111");

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

	glfwSetWindowSizeCallback(window->getGLFWWindow(), window_callback);

	return true;
}

bool initData() {

	shaderProgram = new ShaderProgram("src/shaders/vertexshader.glsl",
			"src/shaders/fragmentshader.glsl");

	GLfloat cube_vertices[] =
			{
					// front
					-1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0,
					1.0,
					// back
					-1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0,
					1.0, -1.0, };
	glGenBuffers(1, &vbo_cube_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices,
	GL_STATIC_DRAW);

	GLfloat cube_colors[] = {
	// front colors
			1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0,
			// back colors
			1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, };

	glGenBuffers(1, &vbo_cube_colors);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors,
	GL_STATIC_DRAW);

	GLushort cube_elements[] = {
	// front
			0, 1, 2, 2, 3, 0,
			// top
			1, 5, 6, 6, 2, 1,
			// back
			7, 6, 5, 5, 4, 7,
			// bottom
			4, 0, 3, 3, 7, 4,
			// left
			4, 5, 1, 1, 0, 4,
			// right
			3, 2, 6, 6, 7, 3, };
	glGenBuffers(1, &ibo_cube_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements,
	GL_STATIC_DRAW);

	// Setup vertex shader attribute
	const char* attribute_name = "coord3d";
	attribute_coord3d = glGetAttribLocation(shaderProgram->getProgram(),
			attribute_name);
	if (attribute_coord3d == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return false;
	}

	GLuint program = shaderProgram->getProgram();

	// Setup vertex shader attribute
	attribute_name = "v_color";
	attribute_v_color = glGetAttribLocation(program, attribute_name);
	if (attribute_v_color == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return false;
	}

	const char* uniform_name;
	uniform_name = "mvp";
	uniform_mvp = glGetUniformLocation(program, uniform_name);
	if (uniform_mvp == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
	}

	return true;
}

void render() {
	float angle = glfwGetTime() / 10.0 * 45;  // 45° per second
	glm::vec3 axis_y(0, 1, 0);
	glm::mat4 anim = glm::rotate(glm::mat4(1.0f), angle, axis_y);

	glm::mat4 model = glm::translate(glm::mat4(1.0f),
			glm::vec3(0.0, 0.0, -4.0));
	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0),
			glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 projection = glm::perspective(45.0f,
			1.0f * window->xSize / window->ySize, 0.1f, 10.0f);
	glm::mat4 mvp = projection * view * model * anim;

	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

	// Enable alpha
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Clear the background as white */
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgram->getProgram());

	glEnableVertexAttribArray(attribute_v_color);
	glEnableVertexAttribArray(attribute_coord3d);

	// Describe our vertices array to OpenGL (it can't guess its format automatically)
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glVertexAttribPointer(attribute_coord3d, // attribute
			3,                 // number of elements per vertex, here (x,y,z)
			GL_FLOAT,          // the type of each element
			GL_FALSE,          // take our values as-is
			0,                 // no extra data between each position
			0                  // offset of first element
			);

	glEnableVertexAttribArray(attribute_v_color);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
	glVertexAttribPointer(attribute_v_color, // attribute
			3,                 // number of elements per vertex, here (R,G,B)
			GL_FLOAT,          // the type of each element
			GL_FALSE,          // take our values as-is
			0,                 // no extra data between each position
			0                  // offset of first element
			);

	/* Push each element in buffer_vertices to the vertex shader */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(attribute_coord3d);
	glDisableVertexAttribArray(attribute_v_color);

}

void freeMemory() {
	delete shaderProgram;
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
