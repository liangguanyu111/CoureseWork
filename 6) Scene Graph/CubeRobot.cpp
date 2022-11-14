#include "CubeRobot.h"

CubeRobot::CubeRobot(Mesh* cube)
{
	SetMesh(cube);

	SceneNode* Body = new SceneNode(cube, Vector4(1, 0, 0, 1));
	Body->SetScale(Vector3(10, 15, 5));
	Body -> SetTransform(Matrix4::Translation(Vector3(0, 35, 0)));
	AddChild(Body);
	
	head = new SceneNode(cube, Vector4(0, 1, 0, 1)); // Green !
	head -> SetScale(Vector3(5, 5, 5));
	head -> SetTransform(Matrix4::Translation(Vector3(0, 30, 0)));
	Body -> AddChild(head);
	
	leftArm = new SceneNode(cube, Vector4(0, 0, 1, 1)); // Blue !
	leftArm -> SetScale(Vector3(3, -18, 3));
	leftArm -> SetTransform(Matrix4::Translation(Vector3(-12, 30, -1)));
	Body -> AddChild(leftArm);
	
	rightArm = new SceneNode(cube, Vector4(0, 0, 1, 1)); // Blue !
	rightArm -> SetScale(Vector3(3, -18, 3));
	rightArm -> SetTransform(Matrix4::Translation(Vector3(12, 30, -1)));
	Body -> AddChild(rightArm);
	
	SceneNode * leftLeg = new SceneNode(cube, Vector4(0, 0, 1, 1)); // Blue !
	leftLeg -> SetScale(Vector3(3, -17.5, 3));
	leftLeg -> SetTransform(Matrix4::Translation(Vector3(-8, 0, 0)));
	Body -> AddChild(leftLeg);
	
	SceneNode * rightLeg = new SceneNode(cube, Vector4(0, 0, 1, 1)); // Blue !
	rightLeg -> SetScale(Vector3(3, -17.5, 3));
	rightLeg -> SetTransform(Matrix4::Translation(Vector3(8, 0, 0)));
	Body -> AddChild(rightLeg);
}

void CubeRobot::Update(float dt)
{
	transform = transform * Matrix4::Rotation(30.0f * dt, Vector3(0, 1, 0));
	head -> SetTransform(head ->GetTransfrom() * Matrix4::Rotation(-30.0f * dt, Vector3(0, 1, 0)));

	leftArm -> SetTransform(leftArm -> GetTransfrom() * Matrix4::Rotation(-30.0f * dt, Vector3(1, 0, 0)));
	
	rightArm -> SetTransform(rightArm -> GetTransfrom() *Matrix4::Rotation(30.0f * dt, Vector3(1, 0, 0)));
	SceneNode::Update(dt);

}