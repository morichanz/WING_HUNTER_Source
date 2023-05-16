/**
* @file VecMath.cpp
*/
#include "VecMath.h"

namespace VecMath 
{
	/**
	* 行列同士の乗算
	*/
	mat4 & mat4::operator*=(const mat4 & other)
	{
		const mat4 m = *this;
		data[0] = m * other[0];
		data[1] = m * other[1];
		data[2] = m * other[2];
		data[3] = m * other[3];
		return *this;
	}

	/**
	* 平行移動行列を作成する
	*/
	mat4 mat4::Translate(const vec3 & v)
	{
		mat4 m(1);
		m[3][0] = v.x;
		m[3][1] = v.y;
		m[3][2] = v.z;
		return m;
	}

	/**
	* 拡大縮小行列を作成する
	*/
	mat4 mat4::Scale(const vec3 & v)
	{
		mat4 m(1);
		m[0][0] = v.x;
		m[1][1] = v.y;
		m[2][2] = v.z;
		return m;
	}

	/**
	* X軸回転行列を作成する
	*/
	mat4 mat4::RotateX(float angle)
	{
		const float c = cos(angle);
		const float s = sin(angle);
		mat4 m(1);
		//y' = y * c - z * s
		//z' = y * s + z * c
		m[1][1] = c;
		m[2][1] = -s;
		m[1][2] = s;
		m[2][2] = c;
		return m;
	}

	/**
	* Y軸回転行列を作成する
	*/
	mat4 mat4::RotateY(float angle)
	{
		const float c = cos(angle);
		const float s = sin(angle);
		mat4 m(1);
		//x' = x * c + z * s
		//z' = x * -s + z * c
		m[0][0] = c;
		m[2][0] = s;
		m[0][2] = -s;
		m[2][2] = c;
		return m;
	}


	/**
	* Z軸回転行列を作成する
	*/
	mat4 mat4::RotateZ(float angle)
	{
		const float c = cos(angle);
		const float s = sin(angle);
		mat4 m(1);
		//x' = x * c + z * s
		//z' = x * s + z * -c
		m[0][0] = c;
		m[1][0] = -s;
		m[0][1] = s;
		m[1][1] = c;
		return m;
	}

	/**
	* ビュー行列を作成する
	*
	* @param eye    視点の座標
	* @param target 注視点の座標
	* @param up     視点の上方向を指すベクトル
	*
	* @return eye, target, upから作成したビュー行列
	*/
	mat4 mat4::LookAt(const vec3& eye, const vec3& target, const vec3& up)
	{
		//ワールド座標系における視点座標系のXYZ軸の向きを計算
		const vec3 axisZ = normalize(eye - target);
		const vec3 axisX = normalize(cross(up, axisZ));
		const vec3 axisY = cross(axisZ, axisX);

		//座標を各軸に射影するように行列の値を設定
		mat4 m(1);
		m[0][0] = axisX.x;
		m[1][0] = axisX.y;
		m[2][0] = axisX.z;

		m[0][1] = axisY.x;
		m[1][1] = axisY.y;
		m[2][1] = axisY.z;

		m[0][2] = axisZ.x;
		m[1][2] = axisZ.y;
		m[2][2] = axisZ.z;

		//平行移動の値を求めるため、視点座標を各軸に射影する
		m[3][0] = -dot(axisX, eye);
		m[3][1] = -dot(axisY, eye);
		m[3][2] = -dot(axisZ, eye);

		return m;
	}

	/**
	* 垂直投影行列を作成する
	*
	* @param left   描画範囲の左端までの距離
	* @param right  描画範囲の右端までの距離
	* @param bottom 描画範囲の下端までの距離
	* @param top    描画範囲の上端までの距離
	* @param zNear  描画範囲に含まれる最小Z座標
	* @param zFar   描画範囲に含まれる最大Z座標
	*
	* @return 垂直投影行列
	*/
	mat4 mat4::Orthogonal(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		//拡大率を設定
		mat4 m(1);
		m[0][0] = 2 / (right - left);
		m[1][1] = 2 / (top - bottom);
		m[2][2] = -2 / (zFar - zNear);

		//平行移動を設定
		m[3][0] = -(right + left) / (right - left);
		m[3][1] = -(top + bottom) / (top - bottom);
		m[3][2] = -(zFar + zNear) / (zFar - zNear);
		return m;
	}


} // namespace VecMath