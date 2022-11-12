#include "Renderer.h"
#include <iostream>

Renderer::Renderer(Window& parent) : OGLRenderer(parent)
{
	triangle = Mesh::GenerateTriagnle();

	basicShader = new Shader("Tutoria1Vertex.glsl", "Tutorial1Fragment.glsl");

	if (!basicShader->LoadSuccess())
	{
		std::cout << "Shader Load failed!" << std::endl;
		return;
	}

	init = true;
}

Renderer::~Renderer(void)
{
	delete triangle;
	delete basicShader;
}

void Renderer::RenderScene()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	BindShader(basicShader);
	triangle->Draw();
}