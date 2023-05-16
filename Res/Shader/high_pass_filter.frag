/**
* @file high_pass_filter.frag
*/
#version 450

// �V�F�[�_�ւ̓���
layout(location=1) in vec2 inTexcoord;

// �e�N�X�`��
layout(binding=0) uniform sampler2D texColor;

// �v���O��������̓���
// x: ���P�x�Ƃ݂Ȃ����邳(�������l)
// y: �u���[���̋���
layout(location=100) uniform vec2 bloomParam;

// �o�͂���F�f�[�^
out vec4 outColor;

/**
* ���P�x�s�N�Z���̐F�������擾����
*/
vec3 GetBrightnessPart(vec3 color)
{
  // RGB�̂����A�����Ƃ����邢�v�f�̒l���s�N�Z���̋P�x�Ƃ���
  float brightness = max(color.r, max(color.g, color.b));
  
  // ���P�x�����̔䗦���v�Z
  float threshold = bloomParam.x;
  float ratio = max(brightness - threshold, 0) / max(brightness, 0.00001);
  
  // ���P�x�������v�Z
  return color * ratio;
}


/**
* �G���g���[�|�C���g
*/
void main()
{
  // �e�N�Z���T�C�Y���v�Z
  vec2 oneTexel = 1 / vec2(textureSize(texColor, 0));

  // ���邢�������v�Z���A4x4�s�N�Z���̏k���ڂ����������s��
  outColor.rgb = GetBrightnessPart(texture(texColor, inTexcoord).rgb) * 4;
  outColor.rgb += GetBrightnessPart(
    texture(texColor, inTexcoord + vec2(-oneTexel.x, oneTexel.y)).rgb);
  outColor.rgb += GetBrightnessPart(
    texture(texColor, inTexcoord + vec2( oneTexel.x, oneTexel.y)).rgb);
  outColor.rgb += GetBrightnessPart(
    texture(texColor, inTexcoord + vec2(-oneTexel.x,-oneTexel.y)).rgb);
  outColor.rgb += GetBrightnessPart(
    texture(texColor, inTexcoord + vec2( oneTexel.x,-oneTexel.y)).rgb);

  // �u���[���̋�������Z
  float strength = bloomParam.y;
  outColor.rgb *= (1.0 / 8.0) * strength;
  outColor.a = 1;
}