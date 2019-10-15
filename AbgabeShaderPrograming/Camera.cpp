#include "Camera.h"

Camera::Camera()
{
	positionX = 0.0f;
	positionY = 0.0f;
	positionZ = 0.0f;
				 
	rotationX = 0.0f;
	rotationY = 0.0f;
	rotationZ = 0.0f;
}

Camera::~Camera()
{
}

void Camera::Init()
{
}

void Camera::Release()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	positionX = x;
	positionY = y;
	positionZ = z;
}

void Camera::SetRotation(float x, float y, float z)
{
	rotationX = x;
	rotationY = y;
	rotationZ = z;
}

XMFLOAT3 Camera::GetPosition()
{
	return XMFLOAT3(positionX,
					positionY,
					positionZ);
}

XMFLOAT3 Camera::GetRotation()
{
	return XMFLOAT3(rotationX,
					rotationY,
					rotationZ);
}

void Camera::Render()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	upVector = XMLoadFloat3(&up);

	position.x = positionX;
	position.y = positionY;
	position.z = positionZ;

	positionVector = XMLoadFloat3(&position);

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	lookAtVector = XMLoadFloat3(&lookAt);

	float rad = 0.0174532925f;

	pitch = rotationX * rad;
	yaw = rotationY * rad;
	roll = rotationZ * rad;

	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, 
		upVector);
}

void Camera::GetViewMatrix(XMMATRIX& _viewMatrix)
{
	_viewMatrix = viewMatrix;
}
