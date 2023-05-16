/**
* @file Texture.h
*/
#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED
#include "glad/glad.h"
#include <string>
#include <memory>

// 先行宣言
class Texture;
using TexturePtr = std::shared_ptr<Texture>;

/**
* テクスチャ
*/
class Texture
{
public:
	// テクスチャの利用目的
	enum class Usage
	{
		for2D, // 2D描画用
		for3D, // 3D描画用
	};

	//テクスチャを作成
	static TexturePtr Create(const std::string& filename, GLenum filterMode = GL_NEAREST,Usage usage = Usage::for2D);
	
	static TexturePtr Create(
		const char* name, GLint width, GLint height,
		GLenum internalFormat = GL_RGBA8, GLenum filterMode = GL_LINEAR);

	//コンストラクタ、デストラクタ
	Texture(const std::string& filename, GLenum filterMode = GL_NEAREST,Usage usage = Usage::for2D);
	~Texture();
	
	Texture(const char* name, GLint width, GLint height,
		GLenum internalFormat = GL_RGBA8, GLenum filterMode = GL_LINEAR);

	//コピーと代入を禁止
	Texture(const Texture&) = delete;
	Texture & operator=(const Texture&) = delete;
	
	//管理番号を取得
	operator GLuint() const { return tex; }

	// 幅と高さを取得
	GLint GetWidth() const { return width; }
	GLint GetHeight() const { return height; }
	
	// ラップモードを設定
	void SetWrapMode(GLenum wrapMode);

private:
	GLuint tex = 0;
	std::string filename;
	GLint width = 0;  // 幅
	GLint height = 0; // 高さ
};
#endif // TEXTURE_H_INCLUDED