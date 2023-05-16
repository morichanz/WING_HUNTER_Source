/**
* @file standard_3D.frag
*/
#version 450

//�V�F�[�_�ւ̓���
layout(location=0) in vec3 inPosition;
layout(location=1) in vec2 inTexcoord;
layout(location=2) in vec3 inNormal;
layout(location=3) in vec3 inShadowPosition;

//�e�N�X�`��
layout(binding=0) uniform sampler2D texColor;
layout(binding=1) uniform sampler2DShadow texDepth;

//�o�͂���F�f�[�^
out vec4 outColor;

layout(location=4) uniform vec3 cameraPosition; //�J�������W

//���s�����p�����[�^
struct DirectionalLight
{
   vec3 color;     //���C�g�̖��邳
   vec3 direction; //���C�g�̌���
};
layout(location=100) uniform DirectionalLight directionalLight;

//�}�e���A��
struct Material
{
   vec4 baseColor; //��{�F+�A���t�@

   //x: ���ʔ��ˎw��
   //y: ���K���W��
   vec2 specularFactor;
};
layout(location=102) uniform Material material;

//�����p�����[�^
layout(location=104) uniform vec3 ambientLight;

void main()
{
   outColor = texture(texColor, inTexcoord);

   //�����Ɣ��f�����t���O�����g�͕`�悵�Ȃ�
   if (outColor.a < 0.5) 
   {
     discard;
   }

   float invPi = 1 / acos(-1); //�΂̋t��

   //���`��Ԃɂ���Ē�����1�ł͂Ȃ��Ȃ��Ă���̂ŁA���K�����Ē�����1�ɕ�������
   vec3 normal = normalize(inNormal);

   //�����x���g�̗]�������g���Ė��邳���v�Z
   float theta = max(dot(-directionalLight.direction, normal), 0);
 
   //�g�U���̖��邳���v�Z
   vec3 diffuse = directionalLight.color * theta * invPi;

    //���ʔ��˃p�����[�^���擾
   float specularPower = material.specularFactor.x;
   float normalizeFactor = material.specularFactor.y;

   //���C�g�ƃJ�����̒��Ԃ̌��������߂�
   vec3 cameraVector = normalize(cameraPosition - inPosition);
   vec3 halfVector = normalize(-directionalLight.direction + cameraVector);

   //���K��Blinn-Phong�@�ɂ���ċ��ʔ��˂̖��邳���v�Z
   float dotNH = max(dot(normal, halfVector), 0);
   vec3 specular = directionalLight.color;
   specular *= normalizeFactor * pow(dotNH, specularPower) * theta;

   //�������v�Z
   vec3 ambient = outColor.rgb * ambientLight * 2;

   //�����͂��Ă���䗦���v�Z
   float shadow = texture(texDepth, inShadowPosition) / 3;

   //�g�U���Ƌ��ʔ��˂���������
   float specularRatio = 0.04; // ���ʔ��˂̔䗦
   diffuse *= outColor.rgb * (1 - specularRatio);
   specular *= specularRatio;
   outColor.rgb = (diffuse + specular) * shadow + ambient;
   outColor *= material.baseColor;
}