/**
* @file Rect.h
*/
#ifndef RECT_H_INCLUED
#define RECT_H_INCLUED
#include<vector>

/**
* 2D�̒����`	
*/
struct Rect
{
	float min[2];
	float max[2];
};

//2D�̒����`�̔z��
using RectList = std::vector<Rect>;

/**
* �����`�̍��W���ړ�������
*/
inline Rect Add(const Rect& r, float x, float y)
{
	return { r.min[0] + x,r.min[1] + y,r.max[0] + x,r.max[1] + y };
}

/**
* 2�̒����`���d�Ȃ��Ă��邩���ׂ�
* @retval true �d�Ȃ��Ă���
* @retval false �d�Ȃ��Ă��Ȃ�
*/
inline bool Overlap(const Rect& a, const Rect& b)
{
	return a.min[0] <= b.max[0] && b.min[0] <= a.max[0] && a.min[1] <= b.max[1] && b.min[1] <= a.max[1];
}

/**
* 2�̒����`�z�񂪏d�Ȃ��Ă��邩���ׂ�
* @retval true �d�Ȃ��Ă���
* @retval false �d�Ȃ��Ă��Ȃ�
*/
inline bool Overlap(const RectList& ra, const RectList& rb)
{
	for (const auto& a : ra)
	{
		for (const auto& b : rb)
		{
			if (Overlap(a, b))
			{
				return true;
			}
		}
	}
	return false;
}

#endif // !RECT_H_INCLUED