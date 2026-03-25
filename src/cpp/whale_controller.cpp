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
}

//===================================================
// 生成処理
//===================================================
CWhale* CWhaleController::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pMotionFilePath)
{
	// 生成処理
	CWhale* pWhale = CWhale::Create(pos, rot, pMotionFilePath);

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