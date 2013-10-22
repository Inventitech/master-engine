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

class RenderableComponent {
	Mesh* mesh;

public:
	RenderableComponent(Mesh* mesh);
	virtual ~RenderableComponent();
	void render(ShaderProgram* shaderProgram, Window* window);
};

#endif /* RENDERABLECOMPONENT_H_ */
