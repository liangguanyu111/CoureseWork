#version 330 core


out vec4 fragColour;

uniform sampler2D sprite;

in Vertex
{
	vec2 texCoord;
	vec4 colour;
}IN;


void main()
{
    fragColour = (texture(sprite, IN.texCoord) *IN.colour);
}