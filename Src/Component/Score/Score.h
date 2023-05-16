/**
* @file Score.h
*/
#ifndef COMPONENT_SCORE_H_INCLUDE
#define COMPONENT_SCORE_H_INCLUDE
#include"../Component.h"
#include"ScoreManager.h"

/**
* 得点コンポーネント
*/
class Score : public Component
{
public:
	Score() = default;
	virtual ~Score() = default;
	virtual void OnDestroy(GameObject& gameObject)override
	{
		if (scoreManager && gameObject.scoreFlg)
		{
			scoreManager->score += score;
			scoreManager->txtSize = 2.3f;
		}
	}

	ScoreManagerPtr scoreManager;

	int score = 100;
};

#endif // !COMPONENT_SCORE_H_INCLUDE

