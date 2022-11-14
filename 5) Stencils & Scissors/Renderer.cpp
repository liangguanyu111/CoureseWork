#include "Renderer.h"
#include <iostream>

Renderer::Renderer(Window& parent) : OGLRenderer(parent)
{
	meshes[1] = Mesh::GenerateTriagnle();
	meshes[0] = Mesh::GenerateQuad();
	//camera = new Camera();

	textures[0] = SOIL_load_OGL_texture(TEXTUREDIR"brick.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

	textures[1] = SOIL_load_OGL_texture(TEXTUREDIR"stainedglass.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

	if (!textures[0] || !textures[1])
	{
		std::cout << "Texture Load failed!" << std::endl;
		return;
	}

	SetTextureRepeating(textures[0], true);
	SetTextureRepeating(textures[1], true);

	matrixShader = new Shader("Tutoria3Vertex.glsl", "Tutorial3Fragment.glsl");

	if (!matrixShader->LoadSuccess())
	{
		std::cout << "Shader Load failed!" << std::endl;
		return;
	}

	usingScissor = false;
	usingStencil = false;


	position[0] = Vector3(0, 0, -5);
	position[1] = Vector3(0, 0, -5);

	usingDepth = false;
	usingAlpha = false;
	blendMode = 0;
	modifyObject = true;

	projMatrix = Matrix4::Perspective(1.0f, 100.0f, (float)width / (float)height, 45.0f);

	init = true;
	filtering = true;
	repeating = false;

	//SwitchToOrthographic();
}

Renderer::~Renderer(void)
{
	delete meshes[0];
	delete meshes[1];

	delete matrixShader;
	//delete camera;

	glDeleteTextures(2, textures);
}

void Renderer::RenderScene()
{

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

	BindShader(matrixShader);
	//UpdateTextureMatrix(rotation);
	UpdateShaderMatrices();

	if (usingScissor)
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor((float)width / 2.5f, (float)height / 2.5f,(float)width/5.0f,(float)height/5.0f);
	}

	glUniform1i(glGetUniformLocation(matrixShader->GetProgram(), "diffuseTex"), 0);


	glActiveTexture(GL_TEXTURE0);
	for (unsigned int i = 0; i < 2; ++i)
	{
		glUniformMatrix4fv(glGetUniformLocation(matrixShader->GetProgram(), "modelMatrix"), 1, false, (float*)&Matrix4::Translation(position[i]));

		glBindTexture(GL_TEXTURE_2D, textures[i]);
		meshes[i]->Draw();
	}

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

void Renderer::ToggleScissor()
{
	usingScissor = !usingScissor;
}
void Renderer::ToggleStencil()
{
	usingStencil = !usingStencil;
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


void Renderer::ToggleObject()
{
	modifyObject = !modifyObject;
}

void Renderer::MoveObject(float by)
{
	position[(int)modifyObject].z += by;
}
void Renderer::ToggleDepth()
{
	usingDepth = !usingDepth;
	usingDepth ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

void Renderer::ToggleAlphaBlend()
{
	usingAlpha = !usingAlpha;
	usingAlpha ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
}


void Renderer::ToggleBlendMode()
{
	blendMode = (blendMode + 1) % 4;

	switch ((blendMode))
	{
	case(0):glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
	case(1):glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);	break;
	case(2):glBlendFunc(GL_ONE, GL_ZERO);	break;
	case(3):glBlendFunc(GL_SRC_ALPHA, GL_ONE); break;
	}
}