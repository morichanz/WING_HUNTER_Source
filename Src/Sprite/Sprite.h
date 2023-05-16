/**
 * @file Sprite.h
*/
#ifndef SPRITE_H_INCLUDED
 #define SPRITE_H_INCLUDED
 #include <vector>

//�}�`�ɕ\������͈͂�\���\����
//���l�̓e�N�X�`�����W�n�Ŏw�肷��
struct Texcoord
{
	float u, v;		//�e�N�X�`�����W
	float sx, sy;	//�摜�̑傫��
};

//�摜�\���p�f�[�^(�X�v���C�g)���i�[����\����
struct Sprite
{
	float x, y, z, w;		//�摜��\��������W
	float sx, sy, sz, sw;	//�g��k��
	float cx, cy, cz, cw;	//�J���[�ύX
	Texcoord texcoord;		//�e�N�X�`�����W�Ƒ傫��
};

//�e�N�X�`�����W���쐬���郉���_��(���W�̓s�N�Z�����Ŏw��)
inline Texcoord MakeTexcoord(float u, float v, float sx, float sy)
{
	return Texcoord{ u / 
		1024,v / 1024,sx / 1024,sy / 1024 };
};

//�X�v���C�g�z��^
using SpriteList = std::vector<Sprite>;

#endif //SPRITE_H_INCLUDED