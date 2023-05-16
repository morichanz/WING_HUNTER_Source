/**
* @file simple_2D.vert
*/
#version 450

// �V�F�[�_�ւ̓���
layout(location=0) in vec3 inPosition;
layout(location=1) in vec2 inTexcoord;

// �V�F�[�_����̏o��
layout(location=0) out vec3 outPosition;
layout(location=1) out vec2 outTexcoord;

// �v���O��������̓���
layout(location=0) uniform mat4 matModel;
layout(location=1) uniform mat4 matVewProjection;

/**
* �G���g���[�|�C���g
*/
void main()
{
   outTexcoord = inTexcoord;
   outPosition = vec3(matModel * vec4(inPosition, 1));
   gl_Position = matVewProjection * vec4(outPosition, 1); 
}