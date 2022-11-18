#pragma once

#include "../nclgl/Matrix4.h"
#include "../nclgl/Vector3.h"

class  Camera
{
public:
	bool autoMove = false;
	bool autoRotate = false;
	Camera(void);
	Camera(float pitch, float yaw, Vector3 position);
	~Camera(void) {};

	void UpdateCamera(float dt = 1.0f);

	Matrix4 BuildViewMatrix();

	Vector3	GetPosition() const { return position; }
	void SetPosition(const Vector3 val) { position = val; }

	float GetYaw() const { return yaw; }
	void SetYaw(const float y) { yaw = y; }

	float GetPitch() const { return pitch; }
	void SetPitch(const float p) { pitch = p; }



private:
	float yaw;
	float pitch;
	Vector3 position;
};

