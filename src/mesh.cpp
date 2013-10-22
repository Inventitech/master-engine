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

void Mesh::upload() {
	glGenBuffers(1, &vbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER,
			this->vertices.size() * sizeof(this->vertices[0]),
			this->vertices.data(),
			GL_STATIC_DRAW);

	glGenBuffers(1, &vbo_normals);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
	glBufferData(GL_ARRAY_BUFFER,
			this->normals.size() * sizeof(this->normals[0]),
			this->normals.data(),
			GL_STATIC_DRAW);

	glGenBuffers(1, &ibo_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			this->elements.size() * sizeof(this->elements[0]),
			this->elements.data(),
			GL_STATIC_DRAW);
}

void Mesh::render(ShaderProgram* shaderProgram) {
	GLuint program = shaderProgram->getProgram();

	// Enable alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Clear the background as white */
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgram->getProgram());

	glEnableVertexAttribArray(shaderProgram->getAttributeCoord3d());

	// Describe our vertices array to OpenGL (it can't guess its format automatically)
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glVertexAttribPointer(shaderProgram->getAttributeCoord3d(), // attribute
			4,                 // number of elements per vertex, here (x,y,z,w)
			GL_FLOAT,          // the type of each element
			GL_FALSE,          // take our values as-is
			0,                 // no extra data between each position
			0                  // offset of first element
			);

	// Describe our vertices array to OpenGL (it can't guess its format automatically)
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
	glVertexAttribPointer(shaderProgram->getAttributeNormal(), // attribute
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

	glDisableVertexAttribArray(shaderProgram->getAttributeCoord3d());
}
