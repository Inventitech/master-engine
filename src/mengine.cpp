#include <stdio.h>
#include <stdlib.h>

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SOIL/SOIL.h>

#include "window.h"
#include "shaderprogram.h"
#include "sound/soundmanager.h"
#include "utils/framecounter.h"
#include "utils/utilities.h"
#include "mesh.h"
#include "objloader.h"
#include "renderablecomponent.h"
#include "utils/glutils.h"

using namespace glm;

Window* window;
ShaderProgram* shaderProgram;
SoundManager* soundManager;
Mesh* monkey;
RenderableComponent* renderComponent;
ObjLoader* objLoader;

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



bool initData() {
	shaderProgram = new ShaderProgram("src/shaders/vertexshader.glsl",
			"src/shaders/fragmentshader.glsl");

	soundManager = new SoundManager();
	sound = soundManager->createSound("resources/itssofluffy.mp3");

	objLoader = new ObjLoader();

	monkey = new Mesh();
	objLoader->load_obj("resources/teddy.obj", monkey);
	monkey->upload();
	renderComponent = new RenderableComponent(monkey);

	if (!shaderProgram->bind()) {
		return EXIT_FAILURE;
	}

	if (showFps) {
		frameCounter = FrameCounter(glfwGetTime());
	}
	return true;
}

void render() {
	soundManager->update();
	renderComponent->render(shaderProgram, window);

	if (showFps) {
		frameCounter.countNewFrame();
	}
}

void freeMemory() {
	soundManager->deleteSound(sound);
	delete soundManager;
	delete shaderProgram;
	delete window;
	delete monkey;
	delete objLoader;
	delete renderComponent;
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
