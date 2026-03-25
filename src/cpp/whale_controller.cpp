//===================================================
//
// クジラを操作し、管理するクラス [whale_controller.cpp]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "whale_controller.h"
#include "whale.h"
#include "player.h"

//===================================================
// コンストラクタ
//===================================================
CWhaleController::CWhaleController() : 
	m_pPlayer(nullptr)
{
}

//===================================================
// デストラクタ
//===================================================
CWhaleController::~CWhaleController()
{
	m_pList.clear();
}

//===================================================
// 生成処理
//===================================================
CWhale* CWhaleController::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pMotionFilePath)
{
	// 生成処理
	CWhale* pWhale = CWhale::Create(pos, rot, pMotionFilePath);

	// クジラの追加
	m_pList.push_back(pWhale);

	// 生成処理
	return pWhale;
}

//===================================================
// 生成処理
//===================================================
std::unique_ptr<CWhaleController> CWhaleController::Create(CPlayer* pPlayer)
{
	// 自分自身の生成
	auto pInstance = std::make_unique<CWhaleController>();

	pInstance->m_pPlayer = pPlayer;

	return pInstance;
}

//===================================================
// 更新処理
//===================================================
void CWhaleController::Update(void)
{
	// プレイヤーの位置の取得
	D3DXVECTOR3 playerPos = m_pPlayer->GetPosition();

	// プランクトン
	for (auto& whale : m_pList)
	{
		// モーションを設定するかどうか判定する
		whale->SetMotionByPlayerDistance(playerPos);
	}
}

//===================================================
// プレイヤーとのヒット
//===================================================
void CWhaleController::OnHitPlayer(CWhale* pWhale)
{
	if (m_pPlayer != nullptr)
	{
		// クジラと当たったら
		m_pPlayer->OnHitWhale(pWhale);
	}
}