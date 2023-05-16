/**
* @file VertexArray.h
*/
#ifndef VERTEXARRAY_H_INCLUDED
#define VERTEXARRAY_H_INCLUDED
#include "glad/glad.h"
#include <memory>

//��s�錾
class VertexArray;
using VertexArrayPtr = std::shared_ptr<VertexArray>;

/**
* VAO���Ǘ�����N���X
*/
class VertexArray
{
public:
	//VAO���쐬
	static VertexArrayPtr Create() { return std::make_shared<VertexArray>(); }
	
	//�R���X�g���N�^�A�f�X�g���N�^
	VertexArray() { glCreateVertexArrays(1, &vao); }
	~VertexArray() { glDeleteVertexArrays(1, &vao); }
	
	//�R�s�[�Ƒ�����֎~
	VertexArray(const VertexArray&) = delete;
	VertexArray & operator=(const VertexArray&) = delete;
	
	//�Ǘ��ԍ����擾
	operator GLuint() const { return vao; }
	
private:
	GLuint vao = 0;
};
#endif // VERTEXARRAY_H_INCLUDED