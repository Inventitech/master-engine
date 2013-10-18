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
#include "mesh.h"
#include "objloader.h"

using namespace glm;

GLint attribute_coord3d, attribute_normal, attribute_texcoord;
Window* window;
ShaderProgram* shaderProgram;
SoundManager* soundManager;
Mesh* monkey;
ObjLoader* objLoader;
GLuint vbo_vertices, vbo_normals;
GLuint ibo_elements;
GLint uniform_m, uniform_v, uniform_p;
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

	objLoader = new ObjLoader();

	monkey = new Mesh();
	objLoader->load_obj("resources/teddy.obj", monkey);
	fprintf(stdout, "Mesh vertices: %d\n", monkey->vertices.size());
	fprintf(stdout, "Mesh elements: %d\n", monkey->elements.size());

	glGenBuffers(1, &vbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER,
			monkey->vertices.size() * sizeof(monkey->vertices[0]),
			monkey->vertices.data(),
			GL_STATIC_DRAW);

	glGenBuffers(1, &vbo_normals);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
	glBufferData(GL_ARRAY_BUFFER,
			monkey->normals.size() * sizeof(monkey->normals[0]),
			monkey->normals.data(),
			GL_STATIC_DRAW);

	glGenBuffers(1, &ibo_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			monkey->elements.size() * sizeof(monkey->elements[0]),
			monkey->elements.data(),
			GL_STATIC_DRAW);

	GLuint program = shaderProgram->getProgram();
	bool bindSuccessfull = true;
	bindSuccessfull &= bindAttribute(program, "coord3d", attribute_coord3d);
	bindSuccessfull &= bindAttribute(program, "normal", attribute_normal);
	bindSuccessfull &= bindUniform(program, "m", uniform_m);
	bindSuccessfull &= bindUniform(program, "v", uniform_v);
	bindSuccessfull &= bindUniform(program, "p", uniform_p);

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

	glm::mat4 model = glm::translate(glm::mat4(1.0f),
			glm::vec3(0.0, -10.0, -30.0));
	  float angle = currentTime * 2;  // base 15° per second
	  glm::mat4 anim = \
	    glm::rotate(glm::mat4(1.0f), angle*3.0f, glm::vec3(1, 0, 0)) *  // X axis
	    glm::rotate(glm::mat4(1.0f), angle*2.0f, glm::vec3(0, 1, 0)) *  // Y axis
	    glm::rotate(glm::mat4(1.0f), angle*4.0f, glm::vec3(0, 0, 1));   // Z axis
	model = model * anim;
	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 4.0),   // eye
	glm::vec3(0.0, 0.0, 0.0),   // direction
	glm::vec3(0.0, 1.0, 0.0));  // up
	glm::mat4 projection = glm::perspective(45.0f,
			1.0f * window->xSize / window->ySize, 0.1f, 1000.0f);

	glUniformMatrix4fv(uniform_m, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniform_v, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(uniform_p, 1, GL_FALSE, glm::value_ptr(projection));

	// Enable alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Clear the background as white */
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgram->getProgram());

	glEnableVertexAttribArray(attribute_coord3d);

	// Describe our vertices array to OpenGL (it can't guess its format automatically)
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glVertexAttribPointer(attribute_coord3d, // attribute
			4,                 // number of elements per vertex, here (x,y,z,w)
			GL_FLOAT,          // the type of each element
			GL_FALSE,          // take our values as-is
			0,                 // no extra data between each position
			0                  // offset of first element
			);

	// Describe our vertices array to OpenGL (it can't guess its format automatically)
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
	glVertexAttribPointer(attribute_normal, // attribute
			3,                 // number of elements per vertex, here (x,y,z,w)
			GL_FLOAT,          // the type of each element
			GL_FALSE,          // take our values as-is
			0,                 // no extra data between each position
			0                  // offset of first element
			);

	/* Push each element in buffer_vertices to the vertex shader */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	int indexex = size / sizeof(GLushort);
	glDrawElements(GL_TRIANGLES, indexex, GL_UNSIGNED_SHORT, (GLvoid*) (0));

	glDisableVertexAttribArray(attribute_coord3d);

	if (showFps) {
		frameCounter.countNewFrame(currentTime);
	}
}

void freeMemory() {
	soundManager->deleteSound(sound);
	delete soundManager;
	delete shaderProgram;
	delete window;
	delete monkey;
	delete objLoader;
}

int main(int argc, char** argv) {
	if (parseCommandLineArgs(argc, argv, showFps)) {
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
