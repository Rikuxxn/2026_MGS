//===================================================
//
// イージングの処理 [easing.cpp]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "easing.h"

//===================================================
// イージングバウンス
//===================================================
float CEasing::EaseOutBounce(float time)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (time < 1.0f / d1)
	{
		return n1 * time * time;
	}
	else if (time < 2.0f / d1)
	{
		time -= 1.5f / d1;
		return n1 * time * time + 0.75f;
	}
	else if (time < 2.5f / d1)
	{
		time -= 2.25f / d1;
		return n1 * time * time + 0.9375f;
	}
	else
	{
		time -= 2.625f / d1;
		return n1 * time * time + 0.984375f;;
	}
}
