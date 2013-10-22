/*
 * renderablecomponent.cpp
 *
 *  Created on: Oct 22, 2013
 *      Author: nihathrael
 */

#include "renderablecomponent.h"
#include "objloader.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

/** Constructor */
RenderableComponent::RenderableComponent(Mesh* mesh) {
	this->mesh = mesh;
}

RenderableComponent::~RenderableComponent() {
}

void RenderableComponent::render(ShaderProgram* shaderProgram, Window* window) {
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

	glUniformMatrix4fv(shaderProgram->getUniformM(), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(shaderProgram->getUniformV(), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(shaderProgram->getUniformP(), 1, GL_FALSE, glm::value_ptr(projection));
	mesh->render(shaderProgram);
}

