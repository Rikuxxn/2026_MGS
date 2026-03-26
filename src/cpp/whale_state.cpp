//===================================================
//
// クジラの状態マシーン [whale_state.cpp]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "whale_state.h"
#include "whale.h"
#include "character.h"
#include "motion.h"
#include "particle_registry.h"
#include "manager.h"
#include "color_constants.h"

//===================================================
// ニュートラル状態コンストラクタ
//===================================================
CWhaleStateNeutral::CWhaleStateNeutral()
{
}

//===================================================
// ニュートラル状態デストラクタ
//===================================================
CWhaleStateNeutral::~CWhaleStateNeutral()
{
}

//===================================================
// ニュートラル状態開始処理
//===================================================
void CWhaleStateNeutral::OnStart(CWhale* pWhale)
{
	// キャラクタークラスの取得
	CCharacter* pCharacter = pWhale->GetCharacter();

	if (pCharacter == nullptr) return;

	// モーションの取得
	CMotion* pMotion = pCharacter->GetMotion();

	if (pMotion == nullptr) return;

	// モーションの設定処理
	pMotion->Play(CWhale::MOTIONTYPE_NEUTRAL, true, 10);
}

//===================================================
// ニュートラル状態更新処理
//===================================================
void CWhaleStateNeutral::OnUpdate(CWhale* pWhale)
{
	(void)pWhale;
}

//===================================================
// 満足状態コンストラクタ
//===================================================
CWhaleSatisfaction::CWhaleSatisfaction() : 
	m_nBlowTime(0)
{
}

//===================================================
// 満足状態デストラクタ
//===================================================
CWhaleSatisfaction::~CWhaleSatisfaction()
{
}

//===================================================
// 満足状態開始処理
//===================================================
void CWhaleSatisfaction::OnStart(CWhale* pWhale)
{
	// キャラクタークラスの取得
	CCharacter* pCharacter = pWhale->GetCharacter();

	if (pCharacter == nullptr) return;

	// モーションの取得
	CMotion* pMotion = pCharacter->GetMotion();

	if (pMotion == nullptr) return;

	// モーションの設定処理
	pMotion->Play(CWhale::MOTIONTYPE_NEUTRAL, true, 10);
}

//===================================================
// 満足状態更新処理
//===================================================
void CWhaleSatisfaction::OnUpdate(CWhale* pWhale)
{
	// 最大まで成長出来たら
	if (pWhale->CheckMaxPlankton())
	{
		m_nBlowTime--;

		// 潮吹き時間になったら
		if (m_nBlowTime <= 0)
		{
			// マネージャーの取得
			CManager* pManager = CManager::GetInstance();

			// パーティクルのパラメータの管理クラスの取得
			CParticleRegistry* pParticleRegistry = pManager->GetParticleRegistry();

			// 位置の取得
			D3DXVECTOR3 pos = pWhale->GetPosition();

			// パーティクルの生成
			pParticleRegistry->CreateParticle(
				PARTICLE_KEY,
				pos + BLOW_OFFSET,
				CEffect::FLAG_ALPHA_DECREASE |
				CEffect::FLAG_RADIUS_DECREASE |
				CEffect::FLAG_GRAVITY,
				Color::AQUA);

			m_nBlowTime = BLOW_STAY_TIME;
		}
	}
}