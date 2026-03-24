//===================================================
//
// プランクトンの処理 [plankton.h]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _PLANKTON_H_
#define _PLANKTON_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "effect.h"

//***************************************************
// プランクトンクラスの定義
//***************************************************
class CPlankton : public CObjectBillboard
{
public:
	CPlankton();
	~CPlankton();

	static CPlankton* Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size);

	HRESULT Init	(void) override;
	void	Uninit	(void) override;
	void	Update	(void) override;
	void	Draw	(void) override;
private:
};
#endif