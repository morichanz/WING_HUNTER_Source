/**
* @file Texture.h
*/
#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED
#include "glad/glad.h"
#include <string>
#include <memory>

// ��s�錾
class Texture;
using TexturePtr = std::shared_ptr<Texture>;

/**
* �e�N�X�`��
*/
class Texture
{
public:
	// �e�N�X�`���̗��p�ړI
	enum class Usage
	{
		for2D, // 2D�`��p
		for3D, // 3D�`��p
	};

	//�e�N�X�`�����쐬
	static TexturePtr Create(const std::string& filename, GLenum filterMode = GL_NEAREST,Usage usage = Usage::for2D);
	
	static TexturePtr Create(
		const char* name, GLint width, GLint height,
		GLenum internalFormat = GL_RGBA8, GLenum filterMode = GL_LINEAR);

	//�R���X�g���N�^�A�f�X�g���N�^
	Texture(const std::string& filename, GLenum filterMode = GL_NEAREST,Usage usage = Usage::for2D);
	~Texture();
	
	Texture(const char* name, GLint width, GLint height,
		GLenum internalFormat = GL_RGBA8, GLenum filterMode = GL_LINEAR);

	//�R�s�[�Ƒ�����֎~
	Texture(const Texture&) = delete;
	Texture & operator=(const Texture&) = delete;
	
	//�Ǘ��ԍ����擾
	operator GLuint() const { return tex; }

	// ���ƍ������擾
	GLint GetWidth() const { return width; }
	GLint GetHeight() const { return height; }
	
	// ���b�v���[�h��ݒ�
	void SetWrapMode(GLenum wrapMode);

private:
	GLuint tex = 0;
	std::string filename;
	GLint width = 0;  // ��
	GLint height = 0; // ����
};
#endif // TEXTURE_H_INCLUDED