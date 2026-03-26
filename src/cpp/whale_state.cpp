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
#include "game.h"
#include "whale_controller.h"
#include "RigidBody.h"

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

	// 現在の向きの取得
	D3DXVECTOR3 rot = pCharacter->GetRotation();

	// クジラの向きを下方向にする
	rot.x = 0.0f;

	// 目的の向きを設定
	pWhale->SetDestRotation(rot);

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
	m_nBlowTime(0),
	m_nBeginDiveTime(DIVE_TIME),
	m_nReleaseTime(RELEASE_TIME)
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
	pMotion->Play(CWhale::MOTIONTYPE_REACTION, true, 10);
}

//===================================================
// 満足状態更新処理
//===================================================
void CWhaleSatisfaction::OnUpdate(CWhale* pWhale)
{
	// キャラクタークラスの取得
	CCharacter* pCharacter = pWhale->GetCharacter();

	if (pCharacter == nullptr) return;

	// モーションの取得
	CMotion* pMotion = pCharacter->GetMotion();

	if (pMotion == nullptr) return;

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

	m_nBeginDiveTime--;

	// モーションが終わったら
	if (m_nBeginDiveTime <= 0)
	{
		// 現在の向きの取得
		D3DXVECTOR3 rot = pCharacter->GetRotation();

		// 現在の位置の取得
		D3DXVECTOR3 pos = pCharacter->GetPosition();

		// クジラの向きを下方向にする
		rot.x = D3DX_PI * 0.5f;

		// 目的の向きを設定
		pWhale->SetDestRotation(rot);

		// 潜る
		pos.y -= 5.0f;

		// リジッドボディの取得
		RigidBody* pRigidBody = pWhale->GetRigidBody();

		if (pRigidBody)
		{
			// クォータニオンにして Rigidbody に渡す
			D3DXQUATERNION q;
			D3DXQuaternionRotationYawPitchRoll(&q, rot.y, 0, 0);
			pRigidBody->SetOrientation(q);

			pRigidBody->SetTransform(pos, q, pRigidBody->GetScale());
		}

		pCharacter->SetPosition(pos);

		if (m_nReleaseTime <= 0)
		{
			// クジラの操作クラス
			CWhaleController* pWhaleController = CGame::GetWhaleController();

			pWhaleController->Erase(pWhale);

			pWhale->Uninit();
		}

		m_nReleaseTime--;
	}
}

//===================================================
// スポーン状態コンストラクタ
//===================================================
CWhaleStateSpawn::CWhaleStateSpawn()
{
}

//===================================================
// スポーン状態デストラクタ
//===================================================
CWhaleStateSpawn::~CWhaleStateSpawn()
{
}

//===================================================
// スポーン状態開始
//===================================================
void CWhaleStateSpawn::OnStart(CWhale* pWhale)
{
	// キャラクタークラスの取得
	CCharacter* pCharacter = pWhale->GetCharacter();

	if (pCharacter == nullptr) return;

	// モーションの取得
	CMotion* pMotion = pCharacter->GetMotion();

	if (pMotion == nullptr) return;

	// 現在の向きの取得
	D3DXVECTOR3 rot = pCharacter->GetRotation();

	// 現在の位置の取得
	D3DXVECTOR3 pos = pCharacter->GetPosition();

	// クジラの向きを下方向にする
	rot.x = -D3DX_PI * 0.5f;

	// 目的の向きを設定
	pWhale->SetDestRotation(rot);

	// モーションの設定処理
	pMotion->Play(CWhale::MOTIONTYPE_REACTION, true, 10);
}

//===================================================
//スポーン状態更新
//===================================================
void CWhaleStateSpawn::OnUpdate(CWhale* pWhale)
{
	// キャラクタークラスの取得
	CCharacter* pCharacter = pWhale->GetCharacter();

	if (pCharacter == nullptr) return;

	// モーションの取得
	CMotion* pMotion = pCharacter->GetMotion();

	if (pMotion == nullptr) return;

	// 現在の向きの取得
	D3DXVECTOR3 rot = pCharacter->GetRotation();

	// 現在の位置の取得
	D3DXVECTOR3 pos = pCharacter->GetPosition();

	pWhale->SetPhysicsMove(D3DXVECTOR3(0.0f, 2.0f, 0.0f));

	// リジッドボディの取得
	RigidBody* pRigidBody = pWhale->GetRigidBody();

	if (pRigidBody)
	{
		// クォータニオンにして Rigidbody に渡す
		D3DXQUATERNION q;
		D3DXQuaternionRotationYawPitchRoll(&q, rot.y, 0, 0);
		pRigidBody->SetOrientation(q);

		pRigidBody->SetTransform(pos, q, pRigidBody->GetScale());
	}

	pWhale->SetPosition(pos);

	// 水面まで上がってきたら
	if (pos.y >= 0.0f)
	{
		pWhale->SetPhysicsMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		// 状態の変更
		m_pMachine->ChangeState<CWhaleStateNeutral>();
	}
}