#version 300 es
precision mediump float;

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec3 vNormal;


uniform mat4 viewModel;
uniform mat4 projection;

out vec4 color;
out vec3 normal;
out vec3 vertex;

void main()
{
    vertex=vec3(viewModel * vec4(vPosition, 1.0));
    normal = vec3(viewModel * vec4(vNormal, 0.0));
    color = vColor;
    gl_Position = projection * viewModel * vec4(vPosition.xyz, 1.0);
}
