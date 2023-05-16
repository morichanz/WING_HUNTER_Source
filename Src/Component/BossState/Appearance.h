/**
* @file Appearance.h
*/
#ifndef COMPONENT_APPEARANCE_H_INCLUDE
#define COMPONENT_APPEARANCE_H_INCLUDE
#include"../../Component/Component.h"
#include"../../GameObject/GameObject.h"

class Appearance : public Component
{
public:
	Appearance() = default;
	virtual ~Appearance() = default;

	void BossAppearance(GameObject& gameObject, float deltaTime, int stageCnt)
	{
		if (stageCnt == 6)
		{
			//ボスの出現(ステージ６の場合)
			if (!appearanceFlg)
			{
				//一旦画面外まで飛ぶ
				if (gameObject.y < 1000)
				{
					gameObject.y += deltaTime * 900;
					val = 0;
				}
				else appearanceFlg = true;
			}
			else
			{
				val += deltaTime * 0.5f;
			}
		}
		else
		{
			val += deltaTime * 0.5f;
		}

		//gameObject.x += deltaTime;
	}
	
	float val = 0;
private:
	int stageCnt = 0;
	bool appearanceFlg = false;
};
#endif // !COMPONENT_APPEARANCE_H_INCLUDE
