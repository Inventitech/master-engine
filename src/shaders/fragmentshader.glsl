#version 120
uniform mat4 m, v, p;
varying vec3 position;
void main(void) {
	mat4 trdui = m * v * p;
  	gl_FragColor = vec4(mod(abs(position.x),3), mod(abs(position.y),3), mod(abs(position.z),3), 0.3);
  	//gl_FragColor = vec4(0.5, 1.0, 1.0, 0.5);
}