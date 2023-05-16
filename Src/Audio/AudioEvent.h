/**
* @file AudioEvent.h
*/
#ifndef COMPONENT_AUDIOEVENT_H_INCLUDED
#define COMPONENT_AUDIOEVENT_H_INCLUDED
#include "../Component/Component.h"
#include "../Audio/EasyAudio.h"
#include <string>

/**
* 音声再生コンポーネント
*/
class AudioEvent : public Component
{
public:
	AudioEvent() = default;
	virtual ~AudioEvent() = default;
	
		virtual void OnCollision(GameObject&, GameObject&) override
		{
			if (!files.onCollision.empty()) {
				Audio::PlayOneShot(files.onCollision.c_str());
			}
		}
	
		virtual void OnTakeDamage(GameObject & gameObject, GameObject & other, const Damage & damage) override 
		{
			if (!files.onTakeDamage.empty()) {
				Audio::PlayOneShot(files.onTakeDamage.c_str());
			}
		}

		virtual void OnDestroy(GameObject & gameObject) override 
		{
			if (!files.onDestroy.empty() && gameObject.scoreFlg) {
				Audio::PlayOneShot(files.onDestroy.c_str());
			}
		}

		struct 
		{
			std::string onCollision; // 衝突時の音声ファイル名
			std::string onTakeDamage;// ダメージを受けたときの音声ファイル名
			std::string onDestroy;   // 破壊時の音声ファイル名
		} files;
};
#endif // COMPONENT_AUDIOEVENT_H_INCLUDED