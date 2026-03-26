//===================================================
//
// 魚の処理 [fish.h]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _FISH_H_
#define _FISH_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "ObjectX.h"

//***************************************************
// 魚クラスの定義
//***************************************************
class CFish : public CObjectX
{
public:
	CFish();
	~CFish();

	static CFish* Create(const D3DXVECTOR3& pos, const char* pModelFileName);

	HRESULT Init	(void) override;
	void	Uninit	(void) override;
	void	Update	(void) override;
	void	Draw	(void) override;
private:

};
#endif