//===================================================
//
// プランクトンを操作し、管理するクラス [plankton_controller.cpp]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "plankton_controller.h"
#include "plankton.h"
#include "player.h"

//===================================================
// コンストラクタ
//===================================================
CPlanktonController::CPlanktonController() : 
	m_pPlayer(nullptr)
{
}

//===================================================
// デストラクタ
//===================================================
CPlanktonController::~CPlanktonController()
{
	m_pList.clear();
}

//===================================================
// 生成処理
//===================================================
std::unique_ptr<CPlanktonController> CPlanktonController::Create(CPlayer* pPlayer)
{
	// 自分自身の生成
	auto pInstance = std::make_unique<CPlanktonController>();

	// 要素の設定
	pInstance->m_pPlayer = pPlayer;

	return pInstance;
}

//===================================================
// プランクトンの生成処理
//===================================================
HRESULT CPlanktonController::CreatePlankton(const D3DXVECTOR3& pos, const D3DXVECTOR2& size)
{
	// プランクトンの生成処理
	CPlankton *pPlankton = CPlankton::Create(pos, size);

	if (m_pPlayer == nullptr)
	{
		return E_FAIL;
	}

	m_pList.push_back(pPlankton);

	return S_OK;
}

//===================================================
// 更新処理
//===================================================
void CPlanktonController::Update(void)
{
	if (m_pPlayer == nullptr)
	{
		return;
	}

	// 位置の取得
	D3DXVECTOR3 pos = m_pPlayer->GetPosition();

	// 要素分回す
	for (auto& list : m_pList)
	{
		// 追従状態じゃないなら
		if (list->GetFollowState() == false)
		{
			continue;
		}

		// 位置の設定
		list->GetFollowPosition(pos);
	}
}