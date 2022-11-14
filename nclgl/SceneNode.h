#pragma once
#include "Matrix4.h"
#include "Mesh.h"
#include "Vector3.h"
#include "Vector4.h"


class SceneNode
{
public:
	SceneNode(Mesh* m = NULL,Vector4 cOLOUR = Vector4(1,1,1,1));
	~SceneNode(void);

	void SetMesh(Mesh* m) {
		mesh = m;
	}
	const Mesh* GetMesh() const { return mesh; }


	void SetTransform(const Matrix4& matrix) {
		transform = matrix;
	}
	const Matrix4& GetTransfrom() const { return transform; }
	Matrix4 GetWorldTransform() { return worldTransfrom; }

	void SetColour(const Vector4& colour) {
		this->colour = colour;
	}
	Vector4 GetColour() const { return colour; }

	void SetScale(const Vector3& s) {
		modelScale = s;
	}
	Vector3 GetScale() const { return modelScale; }

	void AddChild(SceneNode* s);
	void SetParent(SceneNode* s);

	virtual void Update(float dt);
	virtual void Draw(const OGLRenderer& r);

	std::vector<SceneNode*>::const_iterator GetChildIteratorStart()
	{
		return children.begin();
	}

	std::vector<SceneNode*>::const_iterator GetChildIteratorEnd()
	{
		return children.end();
	}

protected:
	SceneNode* parent;
	Mesh* mesh;
	Matrix4 worldTransfrom;
	Matrix4 transform;

	Vector3 modelScale;
	Vector4 colour;
	std::vector<SceneNode*> children;
};

