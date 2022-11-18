#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/CubeRobot.h"
class Camera;
class Mesh;
class HeightMap;
class MeshAnimation;
class MeshMaterial;
class Tree;
class Snow;
struct Particle {
	Vector2 Position, Velocity;
	Vector4 Color;
	GLfloat Life;

	Particle()
		: Position(0.0f, 0.0f), Velocity(0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Life(0.0f) { }
};

class SecondRenderer : public OGLRenderer {
public:
	SecondRenderer(Window& parent);
	~SecondRenderer(void);

	void Init();
	void UpdateScene(float dt) override;
	void RenderScene();
	void CameraAutoMove();
	void CameraAutoRotate();
protected:

	void DrawShadowScene();
	void DrawMainScene();

	//Cubemapping 
	void DrawHeightmap();
	void DrawWater();
	void DrawSkybox();
	void DrawCubeRobot();
	void DrawSnow();
	void DrawNode(SceneNode* n, int useTexuture);
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
	GLuint snowTex;

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

	SceneNode* root;
	Mesh* cube;
	Shader* CubeRobotshader;

	SceneNode* SnowRoot;
	Snow* snow;

};
#pragma once
