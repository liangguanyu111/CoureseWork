#include "SecondRenderer.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/Camera.h"
#include "../nclgl/Light.h"
#include "../nclgl/MeshAnimation.h"
#include "../nclgl/MeshMaterial.h"
#include "tree.h"
#include "Snow.h"
SecondRenderer::SecondRenderer(Window& parent) : OGLRenderer(parent) 
{

}

void SecondRenderer::Init()
{

	CubeRobotshader = new Shader("Tutoria6Vertex.glsl", "Tutorial6Fragment.glsl");


	quad = Mesh::GenerateQuad();
	heightMap = new HeightMap(TEXTUREDIR"noise.png");

	waterTex = SOIL_load_OGL_texture(TEXTUREDIR"water.TGA", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	earthTex = SOIL_load_OGL_texture(TEXTUREDIR"clay_02_dif.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	earthBump = SOIL_load_OGL_texture(TEXTUREDIR"clay_02_nrm.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	snowTex = SOIL_load_OGL_texture(TEXTUREDIR"Snow.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR"west.jpg", TEXTUREDIR"east.jpg",
		TEXTUREDIR"up.jpg", TEXTUREDIR"down.jpg",
		TEXTUREDIR"south.jpg", TEXTUREDIR"north.jpg", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	tree = new Tree(Vector3(4000.0f, 150.0f, 1500.0f),75,90);
	SetTrees();

	if (!earthTex || !earthBump || !cubeMap||!waterTex||!grassTex||!snowTex)
	{
		std::cout << "Texture load failed" << std::endl;
		return;
	}

	SetTextureRepeating(earthTex, true);
	SetTextureRepeating(earthBump, true);
	SetTextureRepeating(waterTex, true);
	SetTextureRepeating(snowTex, true);

	Vector3 heightmapSize = heightMap->GetHeightMapSize();
	camera = new Camera(-45.0f, 0.0f, heightmapSize * Vector3(0.5f, 5.0f, 0.5f));
	light = new Light(heightmapSize * Vector3(1.25f, 10.0f, 1.5f), Vector4(1, 1, 1, 1), 2 * heightmapSize.x);

	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);

	reflectShader = new Shader("reflectVertex.glsl", "reflectFragment.glsl");
	skyboxShader = new Shader("skyboxVertex.glsl", "skyboxFragment.glsl");
	lightShader = new Shader("Tutoria12Vertex.glsl", "Tutorial12Fragment.glsl");
	CubeRobotshader = new Shader("Tutoria6Vertex.glsl","Tutorial6Fragment.glsl");

	characterShader = new Shader("SkinningVertex.glsl", "Tutorial3Fragment.glsl");
	shadowShader = new Shader("shadowVertex.glsl", "shadowFragment.glsl");

	if (!reflectShader->LoadSuccess() || !skyboxShader->LoadSuccess() || 
		!lightShader->LoadSuccess()||!characterShader->LoadSuccess() ||
		!shadowShader->LoadSuccess()||!CubeRobotshader->LoadSuccess())
	{
		return;
	}
	cube = Mesh::LoadFromMeshFile("OffsetCubeY.msh");

	meshCharacter = Mesh::LoadFromMeshFile("Role_T.msh");
	characterAnim = new MeshAnimation("Role_T.anm");
	characterMaterial = new MeshMaterial("Role_T.mat");

	if (!cube)
	{
		std::cout << "mesh_Cube load failed" << std::endl;
		return;
	}

	for (int i = 0; i < meshCharacter->GetSubMeshCount(); ++i)
	{
		const MeshMaterialEntry* matEntry = characterMaterial->GetMaterialForLayer(i);
		const string* filename = nullptr;
		matEntry->GetEntry("Diffuse", &filename);
		string path = TEXTUREDIR + *filename;
		GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
		characterMatTextures.emplace_back(texID);
	}

	root = new SceneNode();

	root->SetTransform(Matrix4::Translation(Vector3(1800.0f, 150.0f, 1800.0f)) * Matrix4::Scale(Vector3(2.0f,2.0f,2.0f)));

	root->AddChild(new CubeRobot(cube));

	SnowRoot = new SceneNode();

	SnowRoot->SetTransform(Matrix4::Translation(Vector3(-2000.0f, 150.0f, -2000.0f)));
	//particles
	snow = new Snow(100, 100, 1000);

	SnowRoot->AddChild(snow);

	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,2048, 2048, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	currentFrame = 0;
	frameTime = 0.0f;
	sceneTime = 0.0f;
	waterRotate = 0.0f;
	waterCycle = 0.0f;
	lightRotateTime = 10.0f;
	init = true;
}

SecondRenderer ::~SecondRenderer(void)
{
	glDeleteTextures(1, &shadowTex);
	glDeleteFramebuffers(1, &shadowFBO);
	delete reflectShader;
	delete skyboxShader;
	delete lightShader;
	delete light;
	delete characterShader;

	delete meshCharacter;
	delete characterAnim;
	delete characterMaterial;

	delete heightMap;
	delete camera;
	delete quad;
	delete sceneShader;
	delete shadowShader;
	delete root;
	delete CubeRobotshader;
	delete cube;

	delete tree;
	for (auto tree : trees) {
		delete tree;
	}
}



void SecondRenderer::UpdateScene(float dt) {
	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();
	root->Update(dt);
	SnowRoot->Update(dt);
	waterRotate += dt * 2.0f; //2 degrees a second
	waterCycle += dt * 0.25f; // 10 units a second

	frameTime -= dt;
	while (frameTime < 0.0f) {
		 currentFrame = (currentFrame + 1) % characterAnim -> GetFrameCount();
		 frameTime += 1.0f / characterAnim-> GetFrameRate();
	}
}

void SecondRenderer::RenderScene()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	DrawSkybox();
	DrawShadowScene();
	//DrawTreeShadow(tree);
	DrawWater();
	DrawCubeRobot();
	DrawSnow();
	DrawHeightmap();
	DrawAnim();
	for (auto tree : trees) {
		DrawTree(tree);
	}

}

void SecondRenderer::CameraAutoMove()
{
	camera->autoMove = !camera->autoMove;
}

void SecondRenderer::CameraAutoRotate()
{
	camera->autoRotate =!camera->autoRotate;
}

void SecondRenderer::DrawShadowScene()
{
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 2048, 2048);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	BindShader(characterShader);
	SetShaderLight(*light);

	viewMatrix = Matrix4::BuildViewMatrix(light->GetPosition(), Vector3(0, 0, 0));
	std::cout << light->GetPosition() << std::endl;
	projMatrix = Matrix4::Perspective(1, 8000, 1, 45);
	shadowMatrix = projMatrix * viewMatrix; // used later

	glUniform3fv(glGetUniformLocation(characterShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	glUniform1i(glGetUniformLocation(characterShader->GetProgram(), "diffuseTex"), 0);

	modelMatrix = Matrix4::Translation(Vector3(2000.0f, 100.0f, 2000.0f)) * Matrix4::Scale(Vector3(100, 100, 100)) * Matrix4::Rotation(90, Vector3(0, 1, 0));
	UpdateShaderMatrices();
	vector < Matrix4 > frameMatrices;
	const Matrix4* invBindPose = meshCharacter->GetInverseBindPose();
	const Matrix4* frameData = characterAnim->GetJointData(currentFrame);
	for (unsigned int i = 0; i < meshCharacter->GetJointCount(); ++i) {
		frameMatrices.emplace_back(frameData[i] * invBindPose[i]);
	}
	int j = glGetUniformLocation(characterShader->GetProgram(), "joints");
	glUniformMatrix4fv(j, frameMatrices.size(), false, (float*)frameMatrices.data());

	for (int i = 0; i < meshCharacter->GetSubMeshCount(); ++i) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, characterMatTextures[i]);
		meshCharacter->DrawSubMesh(i);
	}

	for (auto tree : trees) {

		BindShader(tree->shader);
		SetShaderLight(*light);
		glUniform3fv(glGetUniformLocation(tree->shader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
		glUniform1i(glGetUniformLocation(tree->shader->GetProgram(), "diffuseTex"), 0);

		modelMatrix = Matrix4::Translation(tree->GetPosition()) * Matrix4::Scale(tree->GetScale()) * Matrix4::Rotation(tree->GetRotation(), Vector3(0, 1, 0));
		UpdateShaderMatrices();

		for (int i = 0; i < tree->mesh->GetSubMeshCount(); ++i) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tree->tex);
			tree->mesh->DrawSubMesh(i);
		}
	}


	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);
	viewMatrix = camera->BuildViewMatrix();
}


void SecondRenderer::DrawAnim()
{

	BindShader(characterShader);
	SetShaderLight(*light);
	glUniform3fv(glGetUniformLocation(characterShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	glUniform1i(glGetUniformLocation(characterShader->GetProgram(), "diffuseTex"), 0);

	modelMatrix = Matrix4::Translation(Vector3(2000.0f, 100.0f, 2000.0f)) * Matrix4::Scale(Vector3(100, 100, 100)) * Matrix4::Rotation(90, Vector3(0, 1, 0));
	UpdateShaderMatrices();
	vector < Matrix4 > frameMatrices;
	const Matrix4* invBindPose = meshCharacter->GetInverseBindPose();
	const Matrix4* frameData = characterAnim->GetJointData(currentFrame);
	for (unsigned int i = 0; i < meshCharacter->GetJointCount(); ++i) {
		frameMatrices.emplace_back(frameData[i] * invBindPose[i]);
	}
	int j = glGetUniformLocation(characterShader->GetProgram(), "joints");
	glUniformMatrix4fv(j, frameMatrices.size(), false, (float*)frameMatrices.data());

	for (int i = 0; i < meshCharacter->GetSubMeshCount(); ++i) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, characterMatTextures[i]);
		meshCharacter->DrawSubMesh(i);
	}



}



void SecondRenderer::SetTrees()
{
	for (int i = 0; i < 15; i++)
	{
		Tree* newTree = new Tree(Vector3(rand()%4000+50,150, rand()%4000 + 50),75,90);
		trees.push_back(newTree);
	}
}

void SecondRenderer::DrawTree(Tree* tree)
{
	BindShader(tree->shader);
	SetShaderLight(*light);
	glUniform3fv(glGetUniformLocation(tree->shader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	glUniform1i(glGetUniformLocation(tree->shader->GetProgram(), "diffuseTex"), 0);

	modelMatrix = Matrix4::Translation(tree->GetPosition()) * Matrix4::Scale(tree->GetScale()) * Matrix4::Rotation(tree->GetRotation(), Vector3(0, 1, 0));
	UpdateShaderMatrices();

	for (int i = 0; i <tree->mesh->GetSubMeshCount(); ++i) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tree->tex);
		tree->mesh->DrawSubMesh(i);
	}
}

void SecondRenderer::DrawHeightmap() {
	BindShader(lightShader);
	SetShaderLight(*light);
	glUniform3fv(glGetUniformLocation(lightShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

	glUniform1i(glGetUniformLocation(lightShader->GetProgram(), "diffuseTex"), 0);

	glUniform1i(glGetUniformLocation(lightShader->GetProgram(), "bumpTex"), 1);
	glUniform1i(glGetUniformLocation(lightShader->GetProgram(),"shadowTex"), 2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, earthTex);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, earthBump);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

	modelMatrix.ToIdentity(); // New !
	textureMatrix.ToIdentity(); // New !
	UpdateShaderMatrices();
	heightMap->Draw();
}

void SecondRenderer::DrawWater() {
	BindShader(reflectShader);
	SetShaderLight(*light); 
	glUniform3fv(glGetUniformLocation(reflectShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	glUniform1i(glGetUniformLocation(reflectShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(reflectShader->GetProgram(), "cubeTex"), 2);
	glUniform1i(glGetUniformLocation(reflectShader->GetProgram(), "shadowTex"), 3);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterTex);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

	Vector3 hSize = heightMap->GetHeightMapSize();
	modelMatrix = Matrix4::Translation(hSize * 0.5f) * Matrix4::Scale(hSize * 0.5f) * Matrix4::Rotation(-90, Vector3(1, 0, 0));
	textureMatrix = Matrix4::Translation(Vector3(waterCycle, 0.0f, waterCycle)) * Matrix4::Scale(Vector3(10, 10, 10)) * Matrix4::Rotation(waterRotate, Vector3(0, 0, 1));
	UpdateShaderMatrices();
	quad->Draw();
}

void SecondRenderer::DrawSkybox()
{
	glDepthMask(GL_FALSE);
	BindShader(skyboxShader);
	UpdateShaderMatrices();
	quad->Draw();
	glDepthMask(GL_TRUE);

}

void SecondRenderer::DrawCubeRobot()
{
	BindShader(CubeRobotshader);
	UpdateShaderMatrices();
	glUniform1i(glGetUniformLocation(CubeRobotshader->GetProgram(),"diffuseTex"), 1);
	DrawNode(root,0);
}

void SecondRenderer::DrawSnow()
{
	BindShader(CubeRobotshader);
	UpdateShaderMatrices();
	glUniform1i(glGetUniformLocation(CubeRobotshader->GetProgram(), "diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,snowTex);
	DrawNode(SnowRoot,1);
}

void  SecondRenderer::DrawNode(SceneNode* n,int useTexuture) {
	if (n->GetMesh()) {
		Matrix4 model = n->GetWorldTransform() * Matrix4::Scale(n->GetScale());
		glUniformMatrix4fv(glGetUniformLocation(CubeRobotshader->GetProgram(), "modelMatrix"), 1, false, model.values);
		glUniform4fv(glGetUniformLocation(CubeRobotshader->GetProgram(), "nodeColour"), 1, (float*)&n->GetColour());
		glUniform1i(glGetUniformLocation(CubeRobotshader->GetProgram(), "useTexture"), useTexuture); // Next tutorial ;)
		n->Draw(*this);
	}
	for (vector < SceneNode* >::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); ++i) {
		DrawNode(*i,useTexuture);
	}
}


