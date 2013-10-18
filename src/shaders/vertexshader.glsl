#version 120
attribute vec4 coord3d;
attribute vec3 normal;
uniform mat4 m, v, p;
varying vec3 position;
 
void main(void) {
	mat4 mvp = p * v * m;
  	gl_Position = mvp * coord3d;
  	position = vec3(coord3d.x, coord3d.y, coord3d.z);
}