/**
* @file AudioSetting.h
*/
#ifndef SUDIOSETTING_H_INCLUDE
#define SUDIOSETTING_H_INCLUDE

/**
* �����Đ��v���C���[
*/
namespace AudioPlayer
{
	inline constexpr int bgm = 0;	//BGM�̍Đ��Ɏg���v���C���[�ԍ�
	inline constexpr int se = 1;	//SE�̍Đ��Ɏg���v���C���[�ԍ�
}//namespace Player

/**
* BGM�ݒ�
*/
namespace BGM
{
	inline constexpr char title[] = "Res/Audio/title.mp3";	//�^�C�g����ʂ�BGM
	inline constexpr char gameOver[] = "Res/Audio/gmaeOver.mp3";	//�Q�[���I�[�o�[��ʂ�BGM
	inline constexpr char gameClear[] = "Res/Audio/gameClear.mp3";	//�Q�[���N���A��ʂ�BGM
	inline constexpr char stage01[] = "Res/Audio/play.mp3";	//�X�e�[�W1��BGM
	inline constexpr char stage02[] = "Res/Audio/stage02.mp3";	//�X�e�[�W2��BGM
	inline constexpr char stage03[] = "Res/Audio/stage03.mp3";	//�X�e�[�W3��BGM
	inline constexpr char stage04[] = "Res/Audio/stage04.mp3";	//�X�e�[�W4��BGM
	inline constexpr char stage05[] = "Res/Audio/stage05.mp3";	//�X�e�[�W5��BGM
	inline constexpr char stage06[] = "Res/Audio/stage06.mp3";	//�X�e�[�W6��BGM
	inline constexpr char Boss[] = "Res/Audio/BossBattle.mp3";	//�{�X�X�e�[�W1��BGM
	inline constexpr char Boss2[] = "Res/Audio/BossBattle2.mp3";	//�{�X�X�e�[�W2��BGM
	inline constexpr char Boss3[] = "Res/Audio/BossBattle3.mp3";	//�{�X�X�e�[�W3��BGM
	inline constexpr char Boss4[] = "Res/Audio/BossBattle4.mp3";	//�{�X�X�e�[�W4��BGM
	inline constexpr char Boss5[] = "Res/Audio/BossBattle5.mp3";	//�{�X�X�e�[�W5��BGM
	inline constexpr char Boss6[] = "Res/Audio/BossBattle6.mp3";	//�{�X�X�e�[�W6��BGM
}//namespace BGM

/**
* ���ʉ��ݒ�
*/
namespace SE
{
	inline constexpr char playerShot[] = "Res/Audio/shoot.mp3";	//�e�̔��ˉ�
	inline constexpr char decision[] = "Res/Audio/decision.mp3";	//���艹
	inline constexpr char enemyShot[] = "Res/Audio/enemyShot.mp3";	//���ˉ�
	inline constexpr char bossShot[] = "Res/Audio/bossShot1.mp3";	//���e��
	inline constexpr char bossDamage[] = "Res/Audio/break.mp3";	//���e��
	inline constexpr char weaponDamage[] = "Res/Audio/weaponDamage.mp3";	//����_���[�W��
	inline constexpr char Beam[] = "Res/Audio/beam.mp3";	//�{�X�̃��[�U�[��
	inline constexpr char BeamC[] = "Res/Audio/beamC.mp3";	//�{�X�̃��[�U�[��
	inline constexpr char BeamS[] = "Res/Audio/beamS.mp3";	//�{�X�̃��[�U�[��
	inline constexpr char playerExplosion[] = "Res/Audio/Explosion.mp3"; // ����(���@)
	inline constexpr char enemyExplosionS[] = "Res/Audio/Explosion.mp3"; // ����(��)
	inline constexpr char enemyExplosionM[] = "Res/Audio/Explosion.mp3"; // ����(��)
	inline constexpr char explosion[] = "Res/Audio/Explosion.mp3"; // ����(��)
	inline constexpr char warning[] = "Res/Audio/Warning.mp3"; //����(��)
}//namespace SE

#endif // !SUDIOSETTING_H_INCLUDE
