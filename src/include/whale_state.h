//===================================================
//
// クジラの状態マシーン [whale_state.h]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _WHALE_STATE_H_
#define _WHALE_STATE_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "State.h"

//***************************************************
// 前方宣言
//***************************************************
class CWhale;	// クジラクラス

//***************************************************
// ニュートラル状態の処理
//***************************************************
class CWhaleStateNeutral : public StateBase<CWhale>
{
public:
	CWhaleStateNeutral();
	~CWhaleStateNeutral();

	void OnStart(CWhale* pWhale) override;
	void OnUpdate(CWhale* pWhale) override;
	void OnExit(CWhale* pWhale) override {}
private:
};

//***************************************************
// クジラ満足状態のクラスの定義
//***************************************************
class CWhaleSatisfaction : public StateBase<CWhale>
{
public:
	CWhaleSatisfaction();
	~CWhaleSatisfaction();

	void OnStart(CWhale* pWhale) override;
	void OnUpdate(CWhale* pWhale) override;
	void OnExit(CWhale* pWhale) override {}
private:
	static constexpr const char* PARTICLE_KEY	= "whale_blow";			// パーティクルのキー
	static constexpr int BLOW_STAY_TIME			= 600;					// 潮吹き待機時間
	static constexpr int DIVE_TIME				= 180;					// 潜るまでの時間
	static constexpr int RELEASE_TIME			= 180;					// 消えるまでの時間
	const D3DXVECTOR3 BLOW_OFFSET				= { 0.0f,40.0f,0.0f };	// 潮吹きのオフセット
private:
	int m_nBlowTime;		// 潮吹き時間
	int m_nBeginDiveTime;	// 海に潜るまでの時間
	int m_nReleaseTime;		// 消えるまでの時間
};
#endif