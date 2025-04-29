#version 300 es
precision mediump float;

uniform vec3 lightPos;

// Light model
in vec4 color;
in vec3 normal;
in vec3 vertex;

out vec4 fragColor;

void main()
{
    vec3 to_light;
    vec3 vertex_normal;
    float cos_angle;

    // Calculate a vector from the fragment location to the light source
    to_light = lightPos - vertex;
    to_light = normalize(to_light);

    // The vertex's normal vector is being interpolated across the primitive
    // which can make it un-normalized. So normalize the vertex's normal vector.
    vertex_normal = normalize(normal);

    // Calculate the cosine of the angle between the vertex's normal vector
    // and the vector going to the light.
    cos_angle = dot(vertex_normal, to_light);
    cos_angle = clamp(cos_angle, 0.0, 1.0);


    fragColor=vec4(vec3(color) * cos_angle, color.a);
}
