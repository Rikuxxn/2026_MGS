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
// 前方宣言
//***************************************************
class CPlankton;	// プランクトン

//***************************************************
// 魚クラスの定義
//***************************************************
class CFish : public CObjectX
{
public:
	CFish();
	~CFish();

	static CFish* Create(const D3DXVECTOR3& pos, const char* pModelFileName, const int nLife);

	HRESULT Init	(void) override;
	void	Uninit	(void) override;
	void	Update	(void) override;
	void	Draw	(void) override;

	inline const D3DXVECTOR3& GetMove(void) const { return m_move; }
	inline void SetMove(const D3DXVECTOR3& move) { m_move = move; }

	bool GetALive(void) const;

	bool SetPlankton(CPlankton* pPlankton);
	inline bool HasPlankton(void) { return m_pHasPlankton != nullptr; }
private:
	CPlankton* m_pHasPlankton;	// プランクトンへのポインタ
	D3DXVECTOR3 m_move;	// 移動量
	int m_nLife;		// 寿命
	int m_nDeleteTime;	// 破棄までの時間
};
#endif