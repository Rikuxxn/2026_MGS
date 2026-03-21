//===================================================
//
// タイマーの処理 [timer.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _TIMER_H_
#define _TIMER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"

//***************************************************
// 前方宣言
//***************************************************
class CNumber;	// 数字クラス

//***************************************************
// タイマーのクラスの定義
//***************************************************
class CTimer : public CObject
{
public:
	static constexpr int MAX_DIGIT = 3;	// 最大の桁数

	CTimer();
	~CTimer();

	static CTimer* Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const int nTime, const char* pTexturePath);

	HRESULT Init	(void) override;
	void	Uninit	(void) override;
	void	Update	(void) override;
	void	Draw	(void) override;
private:
	std::array<std::unique_ptr<CNumber>, MAX_DIGIT> m_apNumber;	// 数字クラスへのポインタ
	D3DXVECTOR3	m_pos;											// 位置
	D3DXVECTOR2	m_size;											// 大きさ
	std::string m_texturePath;									// テクスチャのパス
	int			m_nTime;										// 現在の時間
	int			m_nFrameCounter;								// 一秒計測用変数
};
#endif