/**
* @file standard_2D.vert
*/

#version 450

//シェーダーへの入力
layout(location=0) in vec3 inPosition;	//頂点座標
layout(location=1) in vec2 inTexcoord;	//頂点座標

//シェーダーからの出力
layout(location=0) out vec4 outColor;		//頂点色
layout(location=1) out vec2 outTexcoord;	//頂点色

//プログラムからの入力
layout(location=0)uniform float timer;
layout(location=1)uniform vec3 position;

//画面座標系をクリップ座標系に変換するための係数
layout(location=2)uniform vec4 screenToClip;

//テクスチャ
layout(binding=0)uniform sampler2D texColor;	

//テクスチャ座標構造体
struct Texcoord
{
	vec2 uv;
	vec2 size;
};

//画像表示用データ(スプライト)を格納する構造体
struct Sprite
{
	vec4 position;
    vec4 scale;
    vec4 color;
	Texcoord texcoord;
};

//スプライト用のSSBO
layout(std430,binding=0) buffer spriteDataBlock
{
	Sprite spriteList[];
};

void main()
{
    outColor = vec4(spriteList[gl_InstanceID].color.x,spriteList[gl_InstanceID].color.y,spriteList[gl_InstanceID].color.z,spriteList[gl_InstanceID].position.w);

    //テクスチャ座標を計算
    Texcoord texcoord = spriteList[gl_InstanceID].texcoord;
    outTexcoord = texcoord.uv + (inTexcoord * texcoord.size);
    
    //頂点座標を計算
    //ユニフォーム変数position(ポジション)を頂点の座標に加算する
    gl_Position = vec4(inPosition,1);
    
    //拡大縮小
    gl_Position = vec4(spriteList[gl_InstanceID].scale.xyz * inPosition,1);

    //ローテーション
    float rad = spriteList[gl_InstanceID].position.z * 3.14 / 180;
    float bfrX = gl_Position.x;
    float bfrY = gl_Position.y;
    gl_Position.x = bfrX * cos(rad) - bfrY * sin(rad);
    gl_Position.y = bfrX * sin(rad) + bfrY * cos(rad);
    
    //図形の大きさを画像の大きさと等しくする
    gl_Position.xy *= abs(texcoord.size) * textureSize(texColor, 0);
    
    //図形をSSBOに移動
    gl_Position.xy += spriteList[gl_InstanceID].position.xy;

    //画面座標系からクリップ座標系へ変換
    gl_Position.xy =(gl_Position.xy * screenToClip.xy) + screenToClip.zw;
}