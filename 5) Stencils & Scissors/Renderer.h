#pragma once

#include "../nclgl/OGLRenderer.h";
#include "../nclgl/Camera.h";

class Renderer : public OGLRenderer
{
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	void RenderScene() override;

	virtual void UpdateScene(float dt);

	void ToggleScissor();
	void ToggleStencil();

	void ToggleObject();
	void ToggleDepth();
	void ToggleAlphaBlend();
	void ToggleBlendMode();
	void MoveObject(float by);

	void UpdateTextureMatrix(float rotation);
	void ToggleRepeating();
	void ToggleFiltering();

	void SwitchToPerspective();
	void SwitchToOrthographic();


private:

protected:
	Mesh* meshes[2];
	Shader* matrixShader;
	Camera* camera;

	GLuint textures[2];
	Vector3 position[2];

	bool usingScissor;
	bool usingStencil;

	bool filtering;
	bool repeating;
	//Transfrom attribute
	bool modifyObject;
	bool usingDepth;
	bool usingAlpha;
	int blendMode;
};


