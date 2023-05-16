/**
* @file Text.h
*/
#ifndef COMPONENT_TEXT_H_INCLUDE
#define COMPONENT_TEXT_H_INCLUDE
#include"../Component/Component.h"
#include"../GameObject/GameObject.h"
#include<string>

/**
* テキストコンポーネント
*/
class Text : public Component
{
public:
	Text() = default;
	virtual ~Text() = default;
	virtual void Start(GameObject& gameObject)override
	{
		fadeOut = true;
	}

	virtual void Update(GameObject& gameObject, float deltaTime) override
	{
		const float fontSizeX = 16 * scale;
		const float fontSizeY = 32 * scale;
		gameObject.spriteList.clear();
		for (size_t i = 0; i < std::size(text); ++i)
		{
			const char c = text[i];
			gameObject.AddSprite({
				static_cast<float>(c % 16) / 16,
				static_cast<float>(c / 16) / 8,
				1.0f / 16.0f,1.0f / 8.0f },
				fontSizeX * (0.5f + static_cast<float>(i)),
				fontSizeY * 0.5f, scale);
		}

		if (alphaFlg)
		{
			if (!push)
			{
				if (gameObject.alpha <= 0)
				{
					fadeIn = true;
					fadeOut = false;
				}
				else if (gameObject.alpha >= 1)
				{
					fadeIn = false;
					fadeOut = true;
				}
				if (fadeOut) gameObject.alpha -= deltaTime;
				if (fadeIn) gameObject.alpha += deltaTime;
			}
			else
			{
				if (timer > 0 && gameObject.alpha == 1)
				{
					timer -= deltaTime;
					if (timer <= 0)
					{
						gameObject.alpha = 0.0f;
						timer = 0.05f;
					}
				}
				if (timer > 0.0f && gameObject.alpha == 0.0f)
				{
					timer -= deltaTime;
					if (timer <= 0.0f)
					{
						gameObject.alpha = 1.0f;
						timer = 0.05f;
					}
				}
			}
		}
	}

	void SetText(const std::string& t, float s = 1)
	{
		text = t;
		scale = s;
	}

	std::string text;
	float scale = 1;
	bool fadeIn = false;
	bool fadeOut = false;
	bool alphaFlg = false;
	float timer = 0.05f;
	bool push = false;
};

#endif // !COMPONENT_TEXT_H_INCLUDE
