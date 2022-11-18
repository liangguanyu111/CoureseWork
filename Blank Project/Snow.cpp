#include"Snow.h"

Snow::Snow(float v, float b, float numParticle)
{
	this->variant = v;
	this->base = b;
	this->numParticles = numParticle;

	mesh = Mesh::LoadFromMeshFile("Sphere.msh");
	texture = SOIL_load_OGL_texture(TEXTUREDIR"Snow.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	for (int i = 0; i < numParticles; i++) {
		Particle particle;
		particle.isAlive = true;
		particle.position = Vector3(rand() % 16384, 000, rand() % 16384);
		float speed = (rand() % 15) + 8;
		particle.speed = Vector3(0, -speed, -3);
		particle_list.push_back(particle);	
	
		SceneNode* snow = new SceneNode();

		snow->SetMesh(this->mesh);
		snow->SetTransform(Matrix4::Translation(particle.position));
		snow->SetScale(Vector3(10, 10, 10));
		AddChild(snow);
	}
}

void Snow::Update(float dt)
{
	for (int i = 0; i < numParticles; i++) {
		if (children[i]->GetTransfrom().GetPositionVector().y <= 0) {
			children[i]->SetTransform(Matrix4::Translation(Vector3(rand() % 16384, 8000, rand() % 16384)));
			float speed = (rand() % 15) + 8;
			particle_list[i].speed = Vector3(0, -speed, 0);
		}
		children[i]->SetTransform(children[i]->GetTransfrom() * Matrix4::Translation(particle_list[i].speed));
	}
	SceneNode::Update(dt);
}
