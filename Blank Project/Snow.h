#pragma once
#include "../nclgl/SceneNode.h"

class Snow : public SceneNode {
public:
	struct Particle {
		Vector3 speed;
		Vector3 position;
		bool isAlive;
	};
	Snow(float v,float b,float numParticle);
	void Update(float dt) override;
	GLuint texture;
protected:
	float variant;
	float base;
	float numParticles;
	vector<Particle> particle_list;
};