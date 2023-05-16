/**
* @file up_sampling.frag
*/
#version 450

// �V�F�[�_�ւ̓���
layout(location=1) in vec2 inTexcoord;

// �e�N�X�`��
layout(binding=0) uniform sampler2D texColor;

// �o�͂���F�f�[�^
out vec4 outColor;

/**
* �G���g���[�|�C���g
*/
void main()
{
  // �e�N�Z���T�C�Y���v�Z
  vec2 halfTexel = 0.5 / vec2(textureSize(texColor, 0));

  // 3x3�s�N�Z���̂ڂ����������s��
  outColor  = texture(texColor, inTexcoord + vec2(-halfTexel.x, halfTexel.y));
  outColor += texture(texColor, inTexcoord + vec2( halfTexel.x, halfTexel.y));
  outColor += texture(texColor, inTexcoord + vec2( halfTexel.x,-halfTexel.y));
  outColor += texture(texColor, inTexcoord + vec2(-halfTexel.x,-halfTexel.y));
  outColor *= 1.0 / 4.0; // ���ω�
}