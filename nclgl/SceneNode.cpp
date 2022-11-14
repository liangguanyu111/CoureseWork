#include "SceneNode.h"

SceneNode::SceneNode(Mesh* mesh, Vector4 colour)
{
	this->mesh = mesh;
	this->colour = colour;

	parent = NULL;
	modelScale = Vector3(1, 1, 1);
}

SceneNode::~SceneNode(void)
{
	for (unsigned int i = 0; i < children.size(); i++)
	{
		delete children[i];
	}
}
void SceneNode::SetParent(SceneNode* s)
{
	this->parent = s;
}

void SceneNode::AddChild(SceneNode* s)
{
	children.push_back(s);
	s->SetParent(this);
}

void SceneNode::Draw(const OGLRenderer& r)
{
	if (mesh)
		mesh->Draw();
}

void SceneNode::Update(float dt)
{
	if (parent)
	{
		worldTransfrom = parent->worldTransfrom * transform;
	}
	else
	{
		worldTransfrom = transform;
	}

	for (vector<SceneNode*>::iterator i =children.begin();i!=children.end();++i)
	{
		(*i)->Update(dt);
	}
}