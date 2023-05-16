/**
* @file ProgramPipeline.h
*/
#ifndef PROGRAMPIPELINE_H_INCLUDED
#define PROGRAMPIPELINE_H_INCLUDED
#include "glad/glad.h"
#include <memory>
#include <string>

//��s�錾
class ProgramPipeline;
using ProgramPipelinePtr = std::shared_ptr<ProgramPipeline>;

/**
* �v���O�����p�C�v���C�����Ǘ�����N���X
*/
class ProgramPipeline
{
public:
	//�v���O�����p�C�v���C�����쐬
	static ProgramPipelinePtr Create(const std::string & filenameVS, const std::string & filenameFS);
	//�R���X�g���N�^�A�f�X�g���N�^
	ProgramPipeline(const std::string & filenameVS, const std::string & filenameFS);
	~ProgramPipeline();
	
	//�R�s�[�Ƒ�����֎~
	ProgramPipeline(const ProgramPipeline&) = delete;
	ProgramPipeline & operator=(const ProgramPipeline&) = delete;
	
	//�Ǘ��ԍ����擾
	operator GLuint() const { return pp; }
	 GLuint GetVS() const { return vs; }
	 GLuint GetFS() const { return fs; }
	
private:
	GLuint vs = 0; //���_�V�F�[�_
	GLuint fs = 0; //�t���O�����g�V�F�[�_
	GLuint pp = 0; //�v���O�����p�C�v���C��
	std::string filenameVS; //���_�V�F�[�_�t�@�C����
	std::string filenameFS; //�t���O�����g�V�F�[�_�t�@�C����
};
#endif //PROGRAMPIPELINE_H_INCLUDED