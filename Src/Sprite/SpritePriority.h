/**
* @file SpritePriority.h
*/
#ifndef SPRITEPRIORITY_H_INCLUDED
#define SPRITEPRIORITY_H_INCLUDED

/**
* �\���D�揇��
*/
enum SpritePriority
{
	//�D�揇��(��)
	prioCloud,
	prioWeapon,
	prioWaaponBullet,
	prioLargeEnemyMachine,//��^�@
	prioBossBullet,
	prioLargeEnemyShadow, //��^�@�̉e
	prioPlayer,			  //���@
	prioEnemyBullet,	  //�G�̒e
	prioPlayerBullet,	  //���@�̒e
	prioExplosion,		  //�����G�t�F�N�g
	prioEnemy,			  //�G
	prioPExplosionShadow, //�v���C���[�̔����̉e
	prioEnemyShadow,	  //�G�̉e
	prioSmallEnemyMachine,//���^�@
	prioPlayerShadow,     //�v���C���[�̉e
	prioSmallEnemyShadow, //���^�@�̉e
};//�D�揇��(��)

#endif // SPRITEPRIORITY_H_INCLUDED