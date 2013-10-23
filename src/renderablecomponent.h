/*
 * renderablecomponent.h
 *
 *  Created on: Oct 22, 2013
 *      Author: nihathrael
 */

#ifndef RENDERABLECOMPONENT_H_
#define RENDERABLECOMPONENT_H_

#include "mesh.h"
#include "window.h"
#include "glm/glm.hpp"

class RenderableComponent {
	Mesh* mesh;
	glm::vec3 position;

public:
	RenderableComponent(Mesh* mesh, glm::vec3 position);
	virtual ~RenderableComponent();
	void render(ShaderProgram* shaderProgram, Window* window);
};

#endif /* RENDERABLECOMPONENT_H_ */
