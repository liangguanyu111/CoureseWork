#pragma once
#include "../nclgl/Mesh.h"
#include "../nclgl/Matrix4.h"
#include "../nclgl/Mesh.h"
#include "../nclgl/Vector3.h"
#include "../nclgl/Vector4.h"

class  Tree 
{
public:
	 Tree(Vector3 pos, float Scale, float Rotation);
	~ Tree();
	void Draw();
	void SetPosition(Vector3 p) {	position = p;}
	Vector3 GetPosition() { return position;}

	void SetRotation(float r) { rotation = r; }
	float GetRotation() { return rotation; }

	void SetScale(float r) { scale = r; }
	Vector3 GetScale() { return Vector3(scale,scale,scale); }

	Mesh* mesh;
	Shader* shader;
	GLuint tex;
protected:



	float scale;
	Vector3 position;
	float rotation;
};
