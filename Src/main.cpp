/**
* @file main.cpp
*/
#include "Sprite/Sprite.h"
#include "Engine/Engine.h"
#include<Windows.h>


/**
* OpenGL����̃��b�Z�[�W����������R�[���o�b�N�֐�
* 
* @param source		���b�Z�[�W�̔��M��
* @param type		���b�Z�[�W�̎��(�G���[�A�x���Ȃ�)
* @param id			���b�Z�[�W����ʂɎ��ʂ���l
* @param severity	���b�Z�[�W�̏d�v�x(���A���A��A�Œ�)
* @param lenght		���b�Z�[�W�̕�����. �����Ȃ烁�b�Z�[�W��0�I�[����Ă���
* @param message	���b�Z�[�W�{��
* @param userParam	�R�[���o�b�N�ݒ莞�Ɏw�肵���|�C���^
*/

/**
* �G���g���[�|�C���g
*/
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,	//GLFW���C�u�����̏�����
	_In_opt_ HINSTANCE hPrevInstance,	//�`��E�B���h�E�̍쐬
	_In_ LPSTR IpCmdLine,	//OpenGL�R���e�L�X�g�̍쐬
	_In_ int nShowCmd)	//GLFW���C�u�����̏I��
{
	WNDCLASS  winc;            //�E�C���h�E�N���X�\����
	//winc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));         //�A�C�R��
	Engine engine;
	return engine.Run();
}