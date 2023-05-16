/**
* @file Texture.cpp
*/
#include "Texture.h"
#include "../Debug/Debug.h"
#include <filesystem>
#include <fstream>
#include <vector>
#include <algorithm>

/**
* �e�N�X�`�����쐬����
*
* @param filename   �摜�t�@�C����
* @param filterMode �e�N�X�`���ɐݒ肷��g��k���t�B���^
* @param usage      ���p�ړI
* 
* @return �쐬�����e�N�X�`���ւ̃|�C���^
*/
TexturePtr Texture::Create(const std::string& filename, GLenum filterMode,Usage usage)
{
	return std::make_shared<Texture>(filename, filterMode, usage);
}

/**
* �e�N�X�`�����쐬����
*
* @param name            �e�N�X�`����(�f�o�b�O�p)
* @param width           �e�N�X�`���̕�
* @param height          �e�N�X�`���̍���
* @param internalFormat  �e�N�X�`���̃f�[�^�`��
* @param filterMode      �e�N�X�`���ɐݒ肷��g��k���t�B���^
*
* @return �쐬�����e�N�X�`���ւ̃|�C���^
*/
TexturePtr Texture::Create(const char* name, GLint width, GLint height,	GLenum internalFormat, GLenum filterMode)
{
	return std::make_shared<Texture>(
	name, width, height, internalFormat, filterMode);
}

/**
* �R���X�g���N�^
* @param filename	�摜�t�@�C����
* @param filterMode �e�N�X�`���ɐݒ肷��g��k���t�B���^
* @param usage      ���p�ړI
* 
* @return �쐬�����e�N�X�`���̊Ǘ��ԍ�
*/
Texture::Texture(const std::string& filename, GLenum filterMode,Usage usage)
	: filename(filename)
{
	//�t�@�C�����J��
	std::ifstream file(filename, std::ios::binary);
	if (!file)
	{
		LOG_ERROR("%s���J���܂���", filename.c_str());
	}

	//�w�b�_�[����ǂݍ���
	const size_t tgaHeaderSize = 18;	//�w�b�_�[���̃o�C�g��
	uint8_t header[tgaHeaderSize];
	file.read(reinterpret_cast<char*>(header), tgaHeaderSize);

	//�w�b�_��񂩂�摜�T�C�Y�����o��
	const GLsizei width = header[12] + header[13] * 0x100;
	const GLsizei height = header[14] + header[15] * 0x100;

	//�摜����ǂݍ���
	std::vector<char> img(std::filesystem::file_size(filename) - tgaHeaderSize);
	file.read(img.data(), img.size());

	// TGA�w�b�_18�o�C�g�ڂ̑�5�r�b�g�́A�摜�f�[�^�̊i�[������\���B
	//   0�̏ꍇ: �������
	//   1�̏ꍇ: �ォ�牺
	// OpenGL�ł͉摜�f�[�^���u�������v�Ɋi�[���郋�[���ɂȂ��Ă���̂ŁA
	// TGA���u�ォ�牺�v�Ɋi�[����Ă���ꍇ�͉摜���㉺���]����B
	bool topToBottom = header[17] & 0x20;
	if (usage == Usage::for2D)
	{
		topToBottom = !topToBottom; // 2D�摜�p�̏ꍇ�͔��]�������t�ɂ���
	}

	if (topToBottom)
	{
	    // �摜�̃o�C�g���Ɖ�1�񕪂̃o�C�g�����v�Z
		const int pixelDepth = header[16];
		const int imageSize = width * height * pixelDepth / 8;
		const size_t lineSize = width * pixelDepth / 8;
		
		// tmp�ɏ㉺���]�����摜�f�[�^���R�s�[
		std::vector<char> tmp(imageSize);
		auto source = img.begin();
		auto destination = tmp.end();

		for (size_t i = 0; i < height; ++i)
		{
			destination -= lineSize;
			std::copy(source, source + lineSize, destination);
			source += lineSize;
		}
		// tmp��img�̃f�[�^������
		img.swap(tmp);
	}


	//�e�N�X�`�����쐬
	glCreateTextures(GL_TEXTURE_2D, 1, &tex);
	glTextureStorage2D(tex, 1, GL_RGBA8, width, height);
	glTextureSubImage2D(tex, 0, 0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, img.data());

	//�g��k���t�B���^��ݒ�
	glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, filterMode);
	glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, filterMode);

	// �T�C�Y��ݒ�
	this->width = width;
	this->height = height;
}

/**
* �R���X�g���N�^
*
* @param name            �e�N�X�`����(�f�o�b�O�p)
* @param width           �e�N�X�`���̕�
* @param height          �e�N�X�`���̍���
* @param internalFormat  �e�N�X�`���̃f�[�^�`��
* @param filterMode      �e�N�X�`���ɐݒ肷��g��k���t�B���^
*/
Texture::Texture(const char* name, GLint width, GLint height, GLenum internalFormat, GLenum filterMode)
{
	//�e�N�X�`�����쐬
	glCreateTextures(GL_TEXTURE_2D, 1, &tex);
	glTextureStorage2D(tex, 1, internalFormat, width, height);
	
	//�g��k���t�B���^��ݒ�
	glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, filterMode);
	glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, filterMode);

	// �T�C�Y��ݒ�
	this->width = width;
	this->height = height;
}

/**
* �f�X�g���N�^
*/
Texture::~Texture()
{
	glDeleteTextures(1, &tex);
}

/**
* ���b�v���[�h��ݒ�
*
* @param wrapMode �ݒ肷�郉�b�v���[�h
*/
void Texture::SetWrapMode(GLenum wrapMode)
{
	glTextureParameteri(tex, GL_TEXTURE_WRAP_S, wrapMode);
	glTextureParameteri(tex, GL_TEXTURE_WRAP_T, wrapMode);
}