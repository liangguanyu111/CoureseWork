#include "Renderer.h"
#include <iostream>

Renderer::Renderer(Window& parent) : OGLRenderer(parent)
{
	triangle = Mesh::GenerateTriagnle();
	//camera = new Camera();

	texture = SOIL_load_OGL_texture(TEXTUREDIR"brick.tga",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,0);

	if (!texture)
	{
		std::cout << "Texture Load failed!" << std::endl;
		return;
	}

	matrixShader = new Shader("Tutoria3Vertex.glsl", "Tutorial3Fragment.glsl");

	if (!matrixShader->LoadSuccess())
	{
		std::cout << "Shader Load failed!" << std::endl;
		return;
	}

	init = true;
	filtering = true;
	repeating = false;

	//SwitchToOrthographic();
}

Renderer::~Renderer(void)
{
	delete triangle;
	delete matrixShader;
	//delete camera;

	glDeleteTextures(1, &texture);
}

void Renderer::RenderScene()
{

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	BindShader(matrixShader);
	//UpdateTextureMatrix(rotation);

	glUniform1i(glGetUniformLocation(matrixShader->GetProgram(), "diffuseTex"), 0);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	//ShaderSetting
	glUniformMatrix4fv(glGetUniformLocation(matrixShader->GetProgram(), "projMatrix"), 1, false, projMatrix.values);
	glUniformMatrix4fv(glGetUniformLocation(matrixShader->GetProgram(), "viewMatrix"), 1, false, viewMatrix.values);
	modelMatrix = Matrix4::Rotation(rotation, Vector3(0, 1, 0)) * Matrix4::Scale(Vector3(scale, scale, scale));

	glUniformMatrix4fv(glGetUniformLocation(matrixShader->GetProgram(), "modelMatrix"), 1, false, modelMatrix.values);

	triangle->Draw();
	/*
	for (int i = 0; i < 3; i++)
	{
		Vector3 temPos = position;
		temPos.x += (i * 500.0f);
		temPos.y -= (i * 100.0f);
		temPos.z -= (i * 100.0f);

		modelMatrix = Matrix4::Translation(temPos) * Matrix4::Rotation(rotation, Vector3(0, 1, 0)) * Matrix4::Scale(Vector3(scale, scale, scale));

		glUniformMatrix4fv(glGetUniformLocation(matrixShader->GetProgram(), "modelMatrix"), 1, false, modelMatrix.values);

		triangle->Draw();
	}*/

}

void Renderer::SwitchToPerspective()
{
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
}

void Renderer::SwitchToOrthographic()
{
	projMatrix = Matrix4::Orthographic(-1.0f, 10000.0f, width / 2.0f, -width / 2.0f, height / 2.0f, -height / 2.0f);
}


void Renderer::UpdateScene(float dt)
{
	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();
}

void Renderer::UpdateTextureMatrix(float value)
{
	Matrix4 push = Matrix4::Translation(Vector3(-0.5f, -0.5f, 0));
	Matrix4 pop = Matrix4::Translation(Vector3(0.5f, 0.5f, 0));
	Matrix4 rotation = Matrix4::Rotation(value, Vector3(0, 0, 1));

	textureMatrix = pop * rotation * push;

	glUniformMatrix4fv(glGetUniformLocation(matrixShader->GetProgram(), "textureMatrix"), 1, false, textureMatrix.values);
}

void Renderer::ToggleRepeating()
{
	repeating = !repeating;
	SetTextureRepeating(texture, repeating);
}

void Renderer::ToggleFiltering()
{
	filtering = !filtering;
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,filtering ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering ? GL_LINEAR : GL_CLAMP);
	glBindTexture(GL_TEXTURE_2D, 0);
}
