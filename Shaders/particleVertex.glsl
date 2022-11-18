#version 330 core


uniform mat4 projMatrix;
uniform vec2 offset;
uniform vec4 color;

in vec3 position;
in vec2 texCoord;

out Vertex
{
	vec2 texCoord;
	vec4 colour;
}OUT;


void main()
{
    float scale = 10.0f;
    OUT.texCoord = texCoord;
    OUT.colour = color;
    gl_Position = projMatrix * vec4((position * scale) + offset, 0.0, 1.0);
}