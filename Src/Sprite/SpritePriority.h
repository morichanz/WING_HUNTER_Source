/**
* @file SpritePriority.h
*/
#ifndef SPRITEPRIORITY_H_INCLUDED
#define SPRITEPRIORITY_H_INCLUDED

/**
* 表示優先順位
*/
enum SpritePriority
{
	//優先順位(高)
	prioCloud,
	prioWeapon,
	prioWaaponBullet,
	prioLargeEnemyMachine,//大型機
	prioBossBullet,
	prioLargeEnemyShadow, //大型機の影
	prioPlayer,			  //自機
	prioEnemyBullet,	  //敵の弾
	prioPlayerBullet,	  //自機の弾
	prioExplosion,		  //爆発エフェクト
	prioEnemy,			  //敵
	prioPExplosionShadow, //プレイヤーの爆発の影
	prioEnemyShadow,	  //敵の影
	prioSmallEnemyMachine,//小型機
	prioPlayerShadow,     //プレイヤーの影
	prioSmallEnemyShadow, //小型機の影
};//優先順位(低)

#endif // SPRITEPRIORITY_H_INCLUDED