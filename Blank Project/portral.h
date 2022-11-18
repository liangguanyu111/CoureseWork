#pragma once
#include<string>
#include "../nclgl/Mesh.h"

class portral: public Mesh
{
public:
	portral(Vector3 pos);
	~portral();
	void SetPosition(const Vector3 p) { position = p; }
	Vector3 GetPosition() const { return position; };

protected:
	GLuint texture;
	Vector3 position;

};

