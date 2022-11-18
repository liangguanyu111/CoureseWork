#include"Camera.h"
#include"../nclgl/Window.h"
#include<algorithm>
#include<cmath>

Camera::Camera(void)
{
	yaw = 0.0f;
	pitch = 0.0f;
}

Camera::Camera(float pitch, float yaw, Vector3 position)
{
	this->SetPitch(pitch);
	this->SetYaw(yaw);
	this->SetPosition(position);
}

void Camera::UpdateCamera(float dt)
{
	this->pitch -= (Window::GetMouse()->GetRelativePosition().y);
	this->yaw -= (Window::GetMouse()->GetRelativePosition().x);

	pitch = std::min(pitch, 90.0f);
	pitch = std::max(pitch, -90.0f);

	yaw = yaw < 0 ? yaw + 360.0f : yaw;
	yaw = yaw > 360.0f ? yaw - 360.0f : yaw;

	Matrix4 rotation = Matrix4::Rotation(yaw, Vector3(0, 1, 0));

	Vector3 forward = rotation * Vector3(0, 0, -1);
	Vector3 right = rotation * Vector3(1, 0, 0);

	float speed = 3000.0f * dt;

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_W))
	{
		position += forward * speed;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_S))
	{
		position -= forward * speed;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_A))
	{
		position -= right * speed;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_D))
	{
		position += right * speed;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_Q))
	{
		position.y += speed;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_E))
	{
		position.y -= speed;
	}

	if (autoMove)
	{
		position += forward * speed *0.1f;
	}
	
	if (autoRotate)
	{
		yaw += 0.5f;
	}

}




Matrix4 Camera::BuildViewMatrix()
{
	return Matrix4::Rotation(-pitch, Vector3(1, 0, 0)) * Matrix4::Rotation(-yaw, Vector3(0, 1, 0)) * Matrix4::Translation(-position);
}

