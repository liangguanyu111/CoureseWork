#pragma once
#include "../NCLGL/OGLRenderer.h"

class Camera;
class Shader;
class HeightMap;

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	~Renderer(void);
	void RenderScene() override;
	void UpdateScene(float dt) override;
protected:
	void DrawHeightmap();
	void DrawWater();
	void DrawSkybox();

	void FillBuffers();
	void DrawPointLights();
	void CombineBuffers();

	void GenerateScreenTexture(GLuint& into, bool depth = false);

	Shader* sceneShader; 
	Shader * pointlightShader; 
	Shader * combineShader; 

	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;

	HeightMap* heightMap;
	Mesh* quad;
	Mesh* quad2;
	Light* light;
	Camera* camera;
	Light* pointLights; // Array of lighting data
	Mesh * sphere;	// Light volum

	GLuint cubeMap;
	GLuint waterTex;
	GLuint earthTex;
	GLuint earthBump;

	GLuint bufferFBO; // FBO for our G- Buffer pass
	GLuint bufferColourTex; // Albedo goes here
	GLuint bufferNormalTex; // Normals go here
	GLuint bufferDepthTex; // Depth goes here
	GLuint pointLightFBO; // FBO for our lighting pass
	GLuint lightDiffuseTex; // Store diffuse lighting
	GLuint lightSpecularTex; // Store specular lighting


	float waterRotate;
	float waterCycle;
};
