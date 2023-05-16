/**
* @file VecMath.cpp
*/
#include "VecMath.h"

namespace VecMath 
{
	/**
	* �s�񓯎m�̏�Z
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
	* ���s�ړ��s����쐬����
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
	* �g��k���s����쐬����
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
	* X����]�s����쐬����
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
	* Y����]�s����쐬����
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
	* Z����]�s����쐬����
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
	* �r���[�s����쐬����
	*
	* @param eye    ���_�̍��W
	* @param target �����_�̍��W
	* @param up     ���_�̏�������w���x�N�g��
	*
	* @return eye, target, up����쐬�����r���[�s��
	*/
	mat4 mat4::LookAt(const vec3& eye, const vec3& target, const vec3& up)
	{
		//���[���h���W�n�ɂ����鎋�_���W�n��XYZ���̌������v�Z
		const vec3 axisZ = normalize(eye - target);
		const vec3 axisX = normalize(cross(up, axisZ));
		const vec3 axisY = cross(axisZ, axisX);

		//���W���e���Ɏˉe����悤�ɍs��̒l��ݒ�
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

		//���s�ړ��̒l�����߂邽�߁A���_���W���e���Ɏˉe����
		m[3][0] = -dot(axisX, eye);
		m[3][1] = -dot(axisY, eye);
		m[3][2] = -dot(axisZ, eye);

		return m;
	}

	/**
	* �������e�s����쐬����
	*
	* @param left   �`��͈͂̍��[�܂ł̋���
	* @param right  �`��͈͂̉E�[�܂ł̋���
	* @param bottom �`��͈͂̉��[�܂ł̋���
	* @param top    �`��͈͂̏�[�܂ł̋���
	* @param zNear  �`��͈͂Ɋ܂܂��ŏ�Z���W
	* @param zFar   �`��͈͂Ɋ܂܂��ő�Z���W
	*
	* @return �������e�s��
	*/
	mat4 mat4::Orthogonal(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		//�g�嗦��ݒ�
		mat4 m(1);
		m[0][0] = 2 / (right - left);
		m[1][1] = 2 / (top - bottom);
		m[2][2] = -2 / (zFar - zNear);

		//���s�ړ���ݒ�
		m[3][0] = -(right + left) / (right - left);
		m[3][1] = -(top + bottom) / (top - bottom);
		m[3][2] = -(zFar + zNear) / (zFar - zNear);
		return m;
	}


} // namespace VecMath