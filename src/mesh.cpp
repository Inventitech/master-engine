/*
 * mesh.cpp
 *
 *  Created on: Oct 17, 2013
 *      Author: nihathrael
 */

#include "mesh.h"

Mesh::Mesh() :
		vbo_vertices(0), vbo_normals(0), ibo_elements(0), object2world(
				glm::mat4(1)) {
}

Mesh::~Mesh() {
	if (vbo_vertices != 0)
		glDeleteBuffers(1, &vbo_vertices);
	if (vbo_normals != 0)
		glDeleteBuffers(1, &vbo_normals);
	if (ibo_elements != 0)
		glDeleteBuffers(1, &ibo_elements);
}

