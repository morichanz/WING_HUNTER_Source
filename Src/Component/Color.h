/**
* @file Color.h
*/

#ifndef COMPONENT_COLOR_H_INCLUDE
#define COMPONENT_COLOR_H_INCLUDE
#include"../GameObject/GameObject.h"
#include"../Component/Component.h"
#include"MeshRenderer.h"
#include"../VecMath/VecMath.h"

using namespace VecMath;
struct ObjectColor
{
	vec4 red = vec4(2, 0.25f, 0.25f, 1);
	vec4 baceColor = vec4(1);
	vec4 white = vec4(2, 2, 2, 1);
};


class Color : public Component
{
public:
	Color() = default;
	virtual ~Color() = default;

	void MaterialColor(MeshRendererPtr meshRenderer)
	{
		if (!initFlg)
		{
			//個別に色を変えられるように、マテリアルのコピーを作る
			for (int i = 0; i < meshRenderer->mesh->materials.size(); i++)
			{
				meshRenderer->materials.push_back(std::make_shared<Mesh::Material>(*meshRenderer->mesh->materials[i]));
			}
			initFlg = true;
		}
	}

	//色を変える
	void SetMaterialColor(MeshRendererPtr meshRenderer, vec4 vec)
	{
		for (int i = 0; i < meshRenderer->materials.size(); i++)
		{
			meshRenderer->materials[i]->baseColor = vec;
		}
	}

	void SetMaterialColorWeapon(MeshRendererPtr meshRenderer, vec4 vec)
	{
		for (int i = 0; i < meshRenderer->materials.size(); i++)
		{
			meshRenderer->materials[i]->baseColor = vec;
		}
	}


	//ダメージを受けたときの無敵時間中に色を変える
	void SetDamageMaterialColor(MeshRendererPtr meshRenderer, vec4 vec, float deltaTime)
	{
		if (colorInterval > 0)
		{
			if (!meshColor_on)
			{
				SetMaterialColor(meshRenderer, vec);
				meshColor_off = false;
				meshColor_on = true;
			}
			colorInterval -= deltaTime;
		}
		else
		{
			if (!meshColor_off)
			{
				SetMaterialColor(meshRenderer, objectColor.baceColor);
				meshColor_on = false;
				meshColor_off = true;
			}
			if (colorInterval1 > 0)	colorInterval1 -= deltaTime;
			else
			{
				colorInterval = 0.1f;
				colorInterval1 = 0.1f;
			}
		}
	}
		
	float color = 2;
	bool weaponFlg = false;
	bool initFlg = false;
	ObjectColor objectColor;
private:
	bool meshColor_on = false;
	bool meshColor_off = false;
	float colorInterval = 0.1f;
	float colorInterval1 = 0.1f;
};
#endif // !COMPONENT_COLOR_H_INCLUDE
