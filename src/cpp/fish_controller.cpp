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
#include "plankton.h"

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
CFish* CFishController::Create(const D3DXVECTOR3& pos, const char* pModelFileName, const int nLife)
{
	CFish* pFish = CFish::Create(pos, pModelFileName, nLife);

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

	// 
	auto planktonList = m_pPlayer->GetPlankton();

	for (auto& list : m_pFishList)
	{		
		// プランクトンのリストが空だったら
		if (planktonList.empty())
		{
			continue;
		}

		// 死んでたら
		if (list->GetALive() == false)
		{
			Erase(list);
			return;
		}
		// プランクトンの位置
		D3DXVECTOR3 planktonPos = planktonList.back()->GetPosition();

		D3DXVECTOR3 fishPos = list->GetPos();

		// 方向
		D3DXVECTOR3 dir = planktonPos - fishPos;

		D3DXVECTOR3 moveDir = Const::VEC3_NULL;

		moveDir = dir * 2.0f;

		list->SetMove(moveDir);
	}
}

//===================================================
// 結合の解除
//===================================================
void CFishController::Erase(CFish* pFish)
{
	auto itr = std::find(m_pFishList.begin(), m_pFishList.end(), pFish);

	if (itr == m_pFishList.end())
	{
		return;
	}

	itr = m_pFishList.erase(itr);
}