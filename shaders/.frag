#version 300 es
precision mediump float;

in vec4 color;
out vec4 fragColor;

void main()
{
    //    fragColor = vec4(vec3(gl_FragCoord.z), 1.0);
    fragColor=color;
}
