#pragma once

#include "../nclgl/OGLRenderer.h";
#include "../nclgl/Camera.h";

class Renderer : public OGLRenderer
{
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();

	virtual void UpdateScene(float dt);

	void UpdateTextureMatrix(float rotation);
	void ToggleRepeating();
	void ToggleFiltering();

	void SwitchToPerspective();
	void SwitchToOrthographic();

	inline void SetScale(float s) {
		scale = s;
	}

	inline void SetRotation(float r) {
		rotation = r;
	}

	inline void SetPosition(Vector3 p) {
		position = p;
	}
private:

protected:
	Mesh* triangle;
	Shader* matrixShader;
	Camera* camera;

	GLuint texture;
	bool filtering;
	bool repeating;
	//Transfrom attribute
	float scale;
	float rotation;
	Vector3 position;
};

#pragma once
