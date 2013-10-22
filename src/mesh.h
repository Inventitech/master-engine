/*
 * mesh.h
 *
 *  Created on: Oct 17, 2013
 *      Author: nihathrael
 */

#ifndef MESH_H_
#define MESH_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "shaderprogram.h"

class Mesh {
private:
  GLuint vbo_vertices, vbo_normals, ibo_elements;

public:
  std::vector<glm::vec4> vertices;
  std::vector<glm::vec3> normals;
  std::vector<GLushort> elements;
  glm::mat4 object2world;

  Mesh();
  ~Mesh();
  void upload();
  void render(ShaderProgram* shaderProgram);
};

#endif /* MESH_H_ */
