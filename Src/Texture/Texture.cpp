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
* テクスチャを作成する
*
* @param filename   画像ファイル名
* @param filterMode テクスチャに設定する拡大縮小フィルタ
* @param usage      利用目的
* 
* @return 作成したテクスチャへのポインタ
*/
TexturePtr Texture::Create(const std::string& filename, GLenum filterMode,Usage usage)
{
	return std::make_shared<Texture>(filename, filterMode, usage);
}

/**
* テクスチャを作成する
*
* @param name            テクスチャ名(デバッグ用)
* @param width           テクスチャの幅
* @param height          テクスチャの高さ
* @param internalFormat  テクスチャのデータ形式
* @param filterMode      テクスチャに設定する拡大縮小フィルタ
*
* @return 作成したテクスチャへのポインタ
*/
TexturePtr Texture::Create(const char* name, GLint width, GLint height,	GLenum internalFormat, GLenum filterMode)
{
	return std::make_shared<Texture>(
	name, width, height, internalFormat, filterMode);
}

/**
* コンストラクタ
* @param filename	画像ファイル名
* @param filterMode テクスチャに設定する拡大縮小フィルタ
* @param usage      利用目的
* 
* @return 作成したテクスチャの管理番号
*/
Texture::Texture(const std::string& filename, GLenum filterMode,Usage usage)
	: filename(filename)
{
	//ファイルを開く
	std::ifstream file(filename, std::ios::binary);
	if (!file)
	{
		LOG_ERROR("%sを開けません", filename.c_str());
	}

	//ヘッダー情報を読み込む
	const size_t tgaHeaderSize = 18;	//ヘッダー情報のバイト数
	uint8_t header[tgaHeaderSize];
	file.read(reinterpret_cast<char*>(header), tgaHeaderSize);

	//ヘッダ情報から画像サイズを取り出す
	const GLsizei width = header[12] + header[13] * 0x100;
	const GLsizei height = header[14] + header[15] * 0x100;

	//画像情報を読み込む
	std::vector<char> img(std::filesystem::file_size(filename) - tgaHeaderSize);
	file.read(img.data(), img.size());

	// TGAヘッダ18バイト目の第5ビットは、画像データの格納方向を表す。
	//   0の場合: 下から上
	//   1の場合: 上から下
	// OpenGLでは画像データを「下から上」に格納するルールになっているので、
	// TGAが「上から下」に格納されている場合は画像を上下反転する。
	bool topToBottom = header[17] & 0x20;
	if (usage == Usage::for2D)
	{
		topToBottom = !topToBottom; // 2D画像用の場合は反転方向を逆にする
	}

	if (topToBottom)
	{
	    // 画像のバイト数と横1列分のバイト数を計算
		const int pixelDepth = header[16];
		const int imageSize = width * height * pixelDepth / 8;
		const size_t lineSize = width * pixelDepth / 8;
		
		// tmpに上下反転した画像データをコピー
		std::vector<char> tmp(imageSize);
		auto source = img.begin();
		auto destination = tmp.end();

		for (size_t i = 0; i < height; ++i)
		{
			destination -= lineSize;
			std::copy(source, source + lineSize, destination);
			source += lineSize;
		}
		// tmpとimgのデータを交換
		img.swap(tmp);
	}


	//テクスチャを作成
	glCreateTextures(GL_TEXTURE_2D, 1, &tex);
	glTextureStorage2D(tex, 1, GL_RGBA8, width, height);
	glTextureSubImage2D(tex, 0, 0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, img.data());

	//拡大縮小フィルタを設定
	glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, filterMode);
	glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, filterMode);

	// サイズを設定
	this->width = width;
	this->height = height;
}

/**
* コンストラクタ
*
* @param name            テクスチャ名(デバッグ用)
* @param width           テクスチャの幅
* @param height          テクスチャの高さ
* @param internalFormat  テクスチャのデータ形式
* @param filterMode      テクスチャに設定する拡大縮小フィルタ
*/
Texture::Texture(const char* name, GLint width, GLint height, GLenum internalFormat, GLenum filterMode)
{
	//テクスチャを作成
	glCreateTextures(GL_TEXTURE_2D, 1, &tex);
	glTextureStorage2D(tex, 1, internalFormat, width, height);
	
	//拡大縮小フィルタを設定
	glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, filterMode);
	glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, filterMode);

	// サイズを設定
	this->width = width;
	this->height = height;
}

/**
* デストラクタ
*/
Texture::~Texture()
{
	glDeleteTextures(1, &tex);
}

/**
* ラップモードを設定
*
* @param wrapMode 設定するラップモード
*/
void Texture::SetWrapMode(GLenum wrapMode)
{
	glTextureParameteri(tex, GL_TEXTURE_WRAP_S, wrapMode);
	glTextureParameteri(tex, GL_TEXTURE_WRAP_T, wrapMode);
}