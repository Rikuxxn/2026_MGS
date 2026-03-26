//===================================================
//
// 魚の操作クラス処理 [fish_controller.cpp]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "fish_controller.h"
#include "player.h"
#include "fish.h"

//===================================================
// コンストラクタ
//===================================================
CFishController::CFishController() : 
	m_pPlayer(nullptr),
	m_pFishList()
{
}

//===================================================
// デストラクタ
//===================================================
CFishController::~CFishController()
{
}

//===================================================
// 生成処理
//===================================================
std::unique_ptr<CFishController> CFishController::Create(CPlayer* pPlayer)
{
	// 自分自身の生成
	auto pInstance = std::make_unique<CFishController>();

	pInstance->m_pPlayer = pPlayer;
	return pInstance;
}

//===================================================
// 生成処理
//===================================================
CFish* CFishController::Create(const D3DXVECTOR3& pos, const char* pModelFileName)
{
	CFish* pFish = CFish::Create(pos, pModelFileName);

	m_pFishList.push_back(pFish);

	return pFish;
}

//===================================================
// 更新処理
//===================================================
void CFishController::Update(void)
{
	if (m_pPlayer == nullptr)
	{
		return;
	}

	float fDistance = 0.0f;

	for (auto& list : m_pFishList)
	{		
	}
}