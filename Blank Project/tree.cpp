#include"tree.h"

Tree::Tree(Vector3 pos,float scale,float rotation)
{
	mesh = Mesh::LoadFromMeshFile("tree.msh");
	shader = new Shader("treeVertex.glsl", "treeFragment.glsl");
	tex = SOIL_load_OGL_texture(TEXTUREDIR"tree_diffuse.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	this->position = pos;
	this->scale = scale;
	this->rotation = rotation;
}

Tree::~Tree()
{
	delete mesh;
	delete shader;
}
