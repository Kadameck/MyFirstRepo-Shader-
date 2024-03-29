#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <DirectXMath.h>
using namespace DirectX;

class Camera
{
public:
	Camera();
	~Camera();
	void Init();
	void Release();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();
	
	void Render();
	void GetViewMatrix(XMMATRIX&);

private:
	float positionX, positionY, positionZ;
	float rotationX, rotationY, rotationZ;
	XMMATRIX viewMatrix;
};
#endif
