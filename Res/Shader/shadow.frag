/**
* @file shadow.frag
*/
#version 450

//�V�F�[�_�ւ̓���
layout(location=1) in vec2 inTexcoord;

//�e�N�X�`��
layout(binding=0) uniform sampler2D texColor;

//�}�e���A��
struct Material
{
  vec4 baseColor; //��{�F+�A���t�@

  //x: ���ʔ��ˎw��
  //y: ���K���W��
  vec2 specularFactor;
};
layout(location=102) uniform Material material;

void main()
{
  float alpha = texture(texColor, inTexcoord).a * material.baseColor.a;
  if (alpha < 0.5)
  {
    discard;
  }
}