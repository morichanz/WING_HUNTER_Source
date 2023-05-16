/**
* @file standard_3D.frag
*/
#version 450

//シェーダへの入力
layout(location=0) in vec3 inPosition;
layout(location=1) in vec2 inTexcoord;
layout(location=2) in vec3 inNormal;
layout(location=3) in vec3 inShadowPosition;

//テクスチャ
layout(binding=0) uniform sampler2D texColor;
layout(binding=1) uniform sampler2DShadow texDepth;

//出力する色データ
out vec4 outColor;

layout(location=4) uniform vec3 cameraPosition; //カメラ座標

//平行光源パラメータ
struct DirectionalLight
{
   vec3 color;     //ライトの明るさ
   vec3 direction; //ライトの向き
};
layout(location=100) uniform DirectionalLight directionalLight;

//マテリアル
struct Material
{
   vec4 baseColor; //基本色+アルファ

   //x: 鏡面反射指数
   //y: 正規化係数
   vec2 specularFactor;
};
layout(location=102) uniform Material material;

//環境光パラメータ
layout(location=104) uniform vec3 ambientLight;

void main()
{
   outColor = texture(texColor, inTexcoord);

   //透明と判断されるフラグメントは描画しない
   if (outColor.a < 0.5) 
   {
     discard;
   }

   float invPi = 1 / acos(-1); //πの逆数

   //線形補間によって長さが1ではなくなっているので、正規化して長さを1に復元する
   vec3 normal = normalize(inNormal);

   //ランベルトの余弦則を使って明るさを計算
   float theta = max(dot(-directionalLight.direction, normal), 0);
 
   //拡散光の明るさを計算
   vec3 diffuse = directionalLight.color * theta * invPi;

    //鏡面反射パラメータを取得
   float specularPower = material.specularFactor.x;
   float normalizeFactor = material.specularFactor.y;

   //ライトとカメラの中間の向きを求める
   vec3 cameraVector = normalize(cameraPosition - inPosition);
   vec3 halfVector = normalize(-directionalLight.direction + cameraVector);

   //正規化Blinn-Phong法によって鏡面反射の明るさを計算
   float dotNH = max(dot(normal, halfVector), 0);
   vec3 specular = directionalLight.color;
   specular *= normalizeFactor * pow(dotNH, specularPower) * theta;

   //環境光を計算
   vec3 ambient = outColor.rgb * ambientLight * 2;

   //光が届いている比率を計算
   float shadow = texture(texDepth, inShadowPosition) / 3;

   //拡散光と鏡面反射を合成する
   float specularRatio = 0.04; // 鏡面反射の比率
   diffuse *= outColor.rgb * (1 - specularRatio);
   specular *= specularRatio;
   outColor.rgb = (diffuse + specular) * shadow + ambient;
   outColor *= material.baseColor;
}