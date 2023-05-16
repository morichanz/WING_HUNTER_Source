/**
* @file TileMap.h
*/
#ifndef TILEMAP_H_INCLUDED
#define TILEMAP_H_INCLUDED
#include <vector>

// �^�C���̔z�u�f�[�^
struct TileMap
{
	size_t sizeX = 0; // ���̃^�C����
	size_t sizeY = 0; // �c�̃^�C����
	int priority = 0;			//�\���D�揇��
	std::vector<int> data; // �z�u�f�[�^
};

#endif // TILEMAP_H_INCLUDED
