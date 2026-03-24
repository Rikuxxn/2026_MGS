//===================================================
//
// シャドウマップを適応するObjectXクラス [shadowmap_objectX.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _RANKING_SCORE_H_
#define _RANKING_SCORE_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "ObjectX.h"

//***************************************************
// シャドウマップを適応するオブジェクトXクラスの定義
//***************************************************
class CShadowMapObjectX : public CObjectX
{
public:
	CShadowMapObjectX();
	~CShadowMapObjectX();

	HRESULT Init	(void) override;
	void	Uninit	(void) override;
	void	Update	(void) override;
	void	Draw	(void) override;
private:
};
#endif