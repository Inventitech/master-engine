#version 120
uniform mat4 m, v, p;
void main(void) {
	mat4 trdui = m * v * p;
  	gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}