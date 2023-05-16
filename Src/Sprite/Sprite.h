/**
 * @file Sprite.h
*/
#ifndef SPRITE_H_INCLUDED
 #define SPRITE_H_INCLUDED
 #include <vector>

//図形に表示する範囲を表す構造体
//数値はテクスチャ座標系で指定する
struct Texcoord
{
	float u, v;		//テクスチャ座標
	float sx, sy;	//画像の大きさ
};

//画像表示用データ(スプライト)を格納する構造体
struct Sprite
{
	float x, y, z, w;		//画像を表示する座標
	float sx, sy, sz, sw;	//拡大縮小
	float cx, cy, cz, cw;	//カラー変更
	Texcoord texcoord;		//テクスチャ座標と大きさ
};

//テクスチャ座標を作成するラムダ式(座標はピクセル数で指定)
inline Texcoord MakeTexcoord(float u, float v, float sx, float sy)
{
	return Texcoord{ u / 
		1024,v / 1024,sx / 1024,sy / 1024 };
};

//スプライト配列型
using SpriteList = std::vector<Sprite>;

#endif //SPRITE_H_INCLUDED