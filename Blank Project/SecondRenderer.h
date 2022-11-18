#pragma once
#include "../nclgl/OGLRenderer.h"
class Camera;
class Mesh;
class HeightMap;
class MeshAnimation;
class MeshMaterial;
class Tree;

class SecondRenderer : public OGLRenderer {
public:
	SecondRenderer(Window& parent);
	~SecondRenderer(void);

	void Init();
	void UpdateScene(float dt) override;
	void RenderScene();

protected:

	void DrawShadowScene();
	void DrawMainScene();

	//Cubemapping 
	void DrawHeightmap();
	void DrawWater();
	void DrawSkybox();
	void DrawAnim();
	void DrawTree(Tree* tree);
	void SetTrees();
	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;


	Light* light;
	HeightMap* heightMap; // Terrain !
	Mesh* quad; // To draw a full - screen quad
	Camera* camera; // Our usual camera

	GLuint earthTex;
	GLuint earthBump;
	GLuint cubeMap;
	GLuint waterTex;
	GLuint grassTex;

	float waterRotate;
	float waterCycle;

	GLuint shadowTex;
	GLuint shadowFBO;

	GLuint sceneDiffuse;
	GLuint sceneBump;
	float sceneTime;

	Shader * sceneShader;
	Shader * shadowShader;
	vector < Mesh* > sceneMeshes;
	vector < Matrix4 > sceneTransforms;

	Mesh* meshCharacter;
	Shader* characterShader;
	MeshAnimation * characterAnim;
	MeshMaterial * characterMaterial;
	vector <GLuint> characterMatTextures;
	int currentFrame;
	float frameTime;
	float lightRotateTime; 


	vector<Tree*> trees;
	Tree* tree;
};
#pragma once
