#include <stdio.h>
#include <stdlib.h>

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>

#include "triangle.h"
#include "window.h"
#include "shaderprogram.h"
#include "soundmanager.h"
#include "FrameCounter.h"
#include "utilities.h"

using namespace glm;

GLint attribute_coord3d, attribute_v_color, attribute_texcoord;
Window* window;
ShaderProgram* shaderProgram;
SoundManager* soundManager;
GLuint vbo_cube_vertices, vbo_cube_colors, vbo_cube_texcoords;
GLuint ibo_cube_elements;
GLint uniform_m;
GLuint texture_id, texture_id2;
GLint uniform_mytexture;
FMOD::Sound* sound;

bool showFps = false;
FrameCounter frameCounter;

#pragma GCC diagnostic ignored "-Wunused-parameter"
static void key_callback(GLFWwindow* window, int key, int scancode, int action,
		int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		if (!soundManager->isPlaying()) {
			soundManager->playSound(sound);
		} else {
			soundManager->togglePause();
		}
	}

}

bool initGL() {
	glfwInit();

	window = new Window(640, 480, "Master Engine!111");
	glfwSetKeyCallback(window->getGLFWWindow(), key_callback);

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

	return true;
}

/**
 * Binds the specified attribute name to the given shader program.
 */
bool bindAttribute(GLuint program, const char* attributeName,
		GLint& attribute) {
	attribute = glGetAttribLocation(program, attributeName);
	if (attribute == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attributeName);
		return false;
	}
	return true;
}

bool bindUniform(GLuint program, const char* uniformName, GLint& uniform) {
	uniform = glGetUniformLocation(program, uniformName);
	if (uniform == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniformName);
		return false;
	}
	return true;

}

bool initData() {
	shaderProgram = new ShaderProgram("src/shaders/vertexshader.glsl",
			"src/shaders/fragmentshader.glsl");

	soundManager = new SoundManager();
	sound = soundManager->createSound("resources/itssofluffy.mp3");

	GLfloat cube_vertices[] = {
	// front
			-1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0,
			// top
			-1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0,
			// back
			1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0,
			// bottom
			-1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0,
			// left
			-1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0,
			// right
			1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, };
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
			4, 5, 6, 6, 7, 4,
			// back
			8, 9, 10, 10, 11, 8,
			// bottom
			12, 13, 14, 14, 15, 12,
			// left
			16, 17, 18, 18, 19, 16,
			// right
			20, 21, 22, 22, 23, 20, };
	glGenBuffers(1, &ibo_cube_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements,
	GL_STATIC_DRAW);

	GLfloat cube_texcoords[2 * 4 * 6] = {
	// front
			0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, };
	for (int i = 1; i < 6; i++)
		memcpy(&cube_texcoords[i * 4 * 2], &cube_texcoords[0],
				2 * 4 * sizeof(GLfloat));
	glGenBuffers(1, &vbo_cube_texcoords);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_texcoords), cube_texcoords,
	GL_STATIC_DRAW);

	texture_id = SOIL_load_OGL_texture("resources/texture.jpg", SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB
					| SOIL_FLAG_COMPRESS_TO_DXT);

	texture_id2 = SOIL_load_OGL_texture("resources/texture2.jpg",
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB
					| SOIL_FLAG_COMPRESS_TO_DXT);

	GLuint program = shaderProgram->getProgram();
	bool bindSuccessfull = true;
	bindSuccessfull &= bindAttribute(program, "coord3d", attribute_coord3d);
	bindSuccessfull &= bindAttribute(program, "texcoord", attribute_texcoord);
	bindSuccessfull &= bindUniform(program, "mvp", uniform_m);
	bindSuccessfull &= bindUniform(program, "mytexture", uniform_mytexture);

	if (!bindSuccessfull) {
		return EXIT_FAILURE;
	}

	if (showFps) {
		frameCounter = FrameCounter(glfwGetTime());
	}
	return true;
}

void render() {
	soundManager->update();
	double currentTime = glfwGetTime();

	float angle = currentTime / 1 * 45;  // 45° per second
	glm::vec3 axis_y(0, 1, 0);
	//glm::mat4 anim = glm::rotate(glm::mat4(1.0f), angle, axis_y);
	glm::mat4 anim = glm::rotate(glm::mat4(1.0f), angle * 3.0f,
			glm::vec3(1, 0, 0)) *  // X axis
			glm::rotate(glm::mat4(1.0f), angle * 2.0f, glm::vec3(0, 1, 0)) * // Y axis
			glm::rotate(glm::mat4(1.0f), angle * 4.0f, glm::vec3(0, 0, 1)); // Z axis

	glm::mat4 model = glm::translate(glm::mat4(1.0f),
			glm::vec3(0.0, 0.0, -4.0));
	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0),
			glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 projection = glm::perspective(45.0f,
			1.0f * window->xSize / window->ySize, 0.1f, 10.0f);
	glm::mat4 mvp = projection * view * model * anim;

	glUniformMatrix4fv(uniform_m, 1, GL_FALSE, glm::value_ptr(mvp));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glUniform1i(uniform_mytexture, /*GL_TEXTURE*/0);

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
	glEnableVertexAttribArray(attribute_texcoord);

	// Describe our vertices array to OpenGL (it can't guess its format automatically)
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glVertexAttribPointer(attribute_coord3d, // attribute
			3,                 // number of elements per vertex, here (x,y,z)
			GL_FLOAT,          // the type of each element
			GL_FALSE,          // take our values as-is
			0,                 // no extra data between each position
			0                  // offset of first element
			);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoords);
	glVertexAttribPointer(attribute_texcoord, // attribute
			2,                  // number of elements per vertex, here (x,y)
			GL_FLOAT,           // the type of each element
			GL_FALSE,           // take our values as-is
			0,                  // no extra data between each position
			0                   // offset of first element
			);

	/* Push each element in buffer_vertices to the vertex shader */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	int indexex = size / sizeof(GLushort);
	glDrawElements(GL_TRIANGLES, indexex, GL_UNSIGNED_SHORT, (GLvoid*) (0));

	glDisableVertexAttribArray(attribute_coord3d);
	glDisableVertexAttribArray(attribute_v_color);
	glDisableVertexAttribArray(attribute_texcoord);

	if (showFps) {
		frameCounter.countNewFrame(currentTime);
	}
}

void freeMemory() {
	glDeleteTextures(1, &texture_id);
	glDeleteTextures(1, &texture_id2);
	soundManager->deleteSound(sound);
	delete soundManager;
	delete shaderProgram;
	delete window;
}

int main(int argc, char** argv) {
	if(parseCommandLineArgs(argc,argv, showFps)) {
		return EXIT_SUCCESS;
	}

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
