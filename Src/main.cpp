/**
* @file main.cpp
*/
#include "Sprite/Sprite.h"
#include "Engine/Engine.h"
#include<Windows.h>


/**
* OpenGLからのメッセージを処理するコールバック関数
* 
* @param source		メッセージの発信者
* @param type		メッセージの種類(エラー、警告など)
* @param id			メッセージを一位に識別する値
* @param severity	メッセージの重要度(高、中、低、最低)
* @param lenght		メッセージの文字数. 負数ならメッセージは0終端されている
* @param message	メッセージ本体
* @param userParam	コールバック設定時に指定したポインタ
*/

/**
* エントリーポイント
*/
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,	//GLFWライブラリの初期化
	_In_opt_ HINSTANCE hPrevInstance,	//描画ウィンドウの作成
	_In_ LPSTR IpCmdLine,	//OpenGLコンテキストの作成
	_In_ int nShowCmd)	//GLFWライブラリの終了
{
	WNDCLASS  winc;            //ウインドウクラス構造体
	//winc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));         //アイコン
	Engine engine;
	return engine.Run();
}