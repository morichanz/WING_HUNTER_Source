/**
* @file Camera.h
*/
#ifndef COMPONENT_CAMERA_H_INCLUDE
#define COMPONENT_CAMERA_H_INCLUDE
#include"../Component/Component.h"
#include<math.h>

/**
* ビューポート
*/
struct Viewport
{
	int x;      // 左下X座標
	int y;      // 左下Y座標
	int width;  // 幅
	int height; // 高さ
};

/**
* カメラコンポーネント
*/
class Camera : public Component
{
public:
	Camera() = default;
	virtual ~Camera() = default;

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		//遠近法パラメータA,Bを計算
		A = -2.0f * far * near / (far - near);
		B = (far + near) / (far - near);

		//視野角による拡大率を計算
		const float rad = fovY * acos(-1.0f) / 180.0f;
		fovScaleY = 1.0f / tan(rad / 2.0f);
		fovScaleX = aspect * fovScaleY;
	}

	float near = 1.0f; //近平面
	float far = 1000.0f;	//遠平面
	float fovY = 60.0f;	//視野角
	float aspect = 9.0f / 16.0f; //アスペクト比
	Viewport viewport = { 0, 0, 1280, 720 };

	//ユニフォーム変数に設定するパラメータ
	float fovScaleX = 1.0f;	//視野角による拡大率
	float fovScaleY = 1.0f;	//視野角による拡大率
	float A = 0.01f;		//遠近法パラメータA
	float B = 0.99f;		//遠近法パラメータB
};
#endif // !COMPONENT_CAMERA_H_INCLUDE
