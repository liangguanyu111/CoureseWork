#include "Renderer.h"
#include <iostream>

Renderer::Renderer(Window& parent) : OGLRenderer(parent)
{
	triangle = Mesh::GenerateTriagnle();
	camera = new Camera();
	matrixShader = new Shader("Tutoria2Vertex.glsl", "Tutorial2Fragment.glsl");

	if (!matrixShader->LoadSuccess())
	{
		std::cout << "Shader Load failed!" << std::endl;
		return;
	}

	init = true;

	SwitchToOrthographic();
}

Renderer::~Renderer(void)
{
	delete triangle;
	delete matrixShader;
	delete camera;
}

void Renderer::RenderScene()
{

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	BindShader(matrixShader);

	//ShaderSetting
	glUniformMatrix4fv(glGetUniformLocation(matrixShader->GetProgram(), "projMatrix"), 1, false, projMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(matrixShader->GetProgram(), "viewMatrix"), 1, false, viewMatrix.values);

	for (int i = 0; i < 3; i++)
	{
		Vector3 temPos = position;
		temPos.x += (i * 500.0f);
		temPos.y -= (i * 100.0f);
		temPos.z -= (i * 100.0f);

		modelMatrix = Matrix4::Translation(temPos) * Matrix4::Rotation(rotation, Vector3(0, 1, 0)) * Matrix4::Scale(Vector3(scale, scale, scale));

		glUniformMatrix4fv(glGetUniformLocation(matrixShader->GetProgram(), "modelMatrix"), 1, false,modelMatrix.values);

		triangle->Draw();
	}

}

void Renderer::SwitchToPerspective()
{
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
}

void Renderer::SwitchToOrthographic()
{
	projMatrix = Matrix4::Orthographic(-1.0f, 10000.0f, width/2.0f,-width / 2.0f, height/2.0f,-height / 2.0f);
}


void Renderer::UpdateScene(float dt)
{
	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();
}