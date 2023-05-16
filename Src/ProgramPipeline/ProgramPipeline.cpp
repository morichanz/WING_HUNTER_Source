/**
* @file ProgramPipeline.cpp
*/
#include "ProgramPipeline.h"
#include "../Debug/Debug.h"
#include <fstream>
#include <filesystem>
#include <vector>

/**
* プログラムパイプラインを作成する
*
* @param filenameVS 頂点シェーダファイル名
* @param filenameFS フラグメントシェーダファイル名
*
* @return 作成したプログラムパイプラインへのポインタ
*/
ProgramPipelinePtr ProgramPipeline::Create(const std::string & filenameVS, const std::string & filenameFS)
{
	return std::make_shared<ProgramPipeline>(filenameVS, filenameFS);
}

/**
* コンストラクタ
*
* @param filenameVS 頂点シェーダファイル名
* @param filenameFS フラグメントシェーダファイル名
*/
ProgramPipeline::ProgramPipeline(const std::string & filenameVS, const std::string & filenameFS)
	: filenameVS(filenameVS), filenameFS(filenameFS)
{
	//頂点シェーダを読み込んでコンパイルする
	std::ifstream file;
	vs = glCreateShader(GL_VERTEX_SHADER);
	file.open(filenameVS, std::ios::binary);	//ファイルを開く
	if (file)
	{
		//メインメモリ領域を確保
		std::vector<char> s(std::filesystem::file_size(filenameVS));

		//ファイルを読み込む
		file.read(s.data(), s.size());
		s.push_back('\0');	//ヌル終端を追加

		//ソースを設定してコンパイル
		const char* a[] = { s.data() };
		glShaderSource(vs, 1, a, nullptr);
		glCompileShader(vs);
	}
	else 
	{
		LOG_ERROR("%sを開けません", filenameVS.c_str());
	}
	file.close();

	//フラグメントシェーダーを読み込んでコンパイルする
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	file.open(filenameFS, std::ios::binary);
	if (file)
	{
		std::vector<char> s(std::filesystem::file_size(filenameFS));
		file.read(s.data(), s.size());
		s.push_back('\0');
		const char* ss[] = { s.data() };
		glShaderSource(fs, 1, ss, nullptr);
		glCompileShader(fs);
	}
	else 
	{
		LOG_ERROR("%sを開けません", filenameFS.c_str());
	}
	file.close();

	//2つのシェーダーをリンクする
	if (vs && fs)
	{
		pp = glCreateProgram();
		glAttachShader(pp, vs);
		glAttachShader(pp, fs);
		glLinkProgram(pp);
		//リンクの成否を調べる
		GLint result;
		glGetProgramiv(pp, GL_LINK_STATUS, &result);
		if (result != GL_TRUE) 
		{
			LOG_ERROR("シェーダのリンクに失敗(vs=%s, fs=%s)",filenameVS.c_str(), filenameFS.c_str());
		}
	} //if(vs && vs)
}

/**
+* デストラクタ
+*/
ProgramPipeline::~ProgramPipeline()
{
	glDeleteProgram(pp);
	glDeleteShader(fs);
	glDeleteShader(vs);
}