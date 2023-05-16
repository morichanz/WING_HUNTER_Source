/**
* @file ProgramPipeline.cpp
*/
#include "ProgramPipeline.h"
#include "../Debug/Debug.h"
#include <fstream>
#include <filesystem>
#include <vector>

/**
* �v���O�����p�C�v���C�����쐬����
*
* @param filenameVS ���_�V�F�[�_�t�@�C����
* @param filenameFS �t���O�����g�V�F�[�_�t�@�C����
*
* @return �쐬�����v���O�����p�C�v���C���ւ̃|�C���^
*/
ProgramPipelinePtr ProgramPipeline::Create(const std::string & filenameVS, const std::string & filenameFS)
{
	return std::make_shared<ProgramPipeline>(filenameVS, filenameFS);
}

/**
* �R���X�g���N�^
*
* @param filenameVS ���_�V�F�[�_�t�@�C����
* @param filenameFS �t���O�����g�V�F�[�_�t�@�C����
*/
ProgramPipeline::ProgramPipeline(const std::string & filenameVS, const std::string & filenameFS)
	: filenameVS(filenameVS), filenameFS(filenameFS)
{
	//���_�V�F�[�_��ǂݍ���ŃR���p�C������
	std::ifstream file;
	vs = glCreateShader(GL_VERTEX_SHADER);
	file.open(filenameVS, std::ios::binary);	//�t�@�C�����J��
	if (file)
	{
		//���C���������̈���m��
		std::vector<char> s(std::filesystem::file_size(filenameVS));

		//�t�@�C����ǂݍ���
		file.read(s.data(), s.size());
		s.push_back('\0');	//�k���I�[��ǉ�

		//�\�[�X��ݒ肵�ăR���p�C��
		const char* a[] = { s.data() };
		glShaderSource(vs, 1, a, nullptr);
		glCompileShader(vs);
	}
	else 
	{
		LOG_ERROR("%s���J���܂���", filenameVS.c_str());
	}
	file.close();

	//�t���O�����g�V�F�[�_�[��ǂݍ���ŃR���p�C������
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
		LOG_ERROR("%s���J���܂���", filenameFS.c_str());
	}
	file.close();

	//2�̃V�F�[�_�[�������N����
	if (vs && fs)
	{
		pp = glCreateProgram();
		glAttachShader(pp, vs);
		glAttachShader(pp, fs);
		glLinkProgram(pp);
		//�����N�̐��ۂ𒲂ׂ�
		GLint result;
		glGetProgramiv(pp, GL_LINK_STATUS, &result);
		if (result != GL_TRUE) 
		{
			LOG_ERROR("�V�F�[�_�̃����N�Ɏ��s(vs=%s, fs=%s)",filenameVS.c_str(), filenameFS.c_str());
		}
	} //if(vs && vs)
}

/**
+* �f�X�g���N�^
+*/
ProgramPipeline::~ProgramPipeline()
{
	glDeleteProgram(pp);
	glDeleteShader(fs);
	glDeleteShader(vs);
}