/**
* @file ScoreManager.h
*/
#ifndef COMPONENT_SCOREMANAGER_H_INCLUDE
#define COMPONENT_SCOREMANAGER_H_INCLUDE
#include"../Component.h"
#include"../Text.h"
#include<stdio.h>

/**
* 得点を管理するコンポーネント
*/
class ScoreManager : public Component
{
public:
	ScoreManager() = default;
	virtual ~ScoreManager() = default;
	virtual void Update(GameObject& gameObject, float deltaTime) override
	{
		if (textComponent)
		{
			char str[16];
			snprintf(str, std::size(str), "%010d", score);
			textComponent->SetText(str, txtSize);
		}
		if (txtSize > 2.0f)txtSize -= deltaTime * 2.0f;
		else txtSize = 2.0f;
	}

	int score = 0;
	float txtSize = 2.0f;
	std::shared_ptr<Text> textComponent;
};

using ScoreManagerPtr = std::shared_ptr<ScoreManager>;

#endif // !COMPONENT_SCOREMANAGER_H_INCLUDE