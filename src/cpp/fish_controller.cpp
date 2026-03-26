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
#include "utility_math.h"
#include "MathConst.h"
#include "game.h"
#include "timer.h"

//***************************************************
// 定数宣言
//***************************************************
namespace FishControllerConst
{
	constexpr int SPAWN_TIME = 300;
	constexpr int MAX_SPAWN = 6;
}
//===================================================
// コンストラクタ
//===================================================
CFishController::CFishController() : 
	m_pPlayer(nullptr),
	m_pFishList(),
	m_nSpawnCounter(FishControllerConst::SPAWN_TIME)
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
	auto& planktonList = m_pPlayer->GetPlankton();

	for (auto& list : m_pFishList)
	{		
		// プランクトンのリストが空だったら
		if (planktonList.empty())
		{
			list->SetMove(Const::VEC3_NULL);
			continue;
		}

		// 死んでたら
		if (list->GetALive() == false)
		{
			Erase(list);
			continue;
		}

		// プランクトンを持っていたら処理を飛ばす
		if (list->HasPlankton())
		{
			continue;
		}

		// プランクトンの位置
		D3DXVECTOR3 planktonPos = planktonList.back()->GetPosition();

		D3DXVECTOR3 fishPos = math::GetPositionFromMatrix(list->GetMatrixWorld());

		// 方向
		D3DXVECTOR3 dir = math::GetVector(planktonPos, fishPos);

		D3DXVECTOR3 moveDir = Const::VEC3_NULL;

		// プランクトンと魚の距離を求める
		float fDistance = math::GetDistance(planktonPos - fishPos);

		moveDir = dir * 3.0f;

		// 移動量の設定
		list->SetMove(moveDir);

		if (fDistance <= 70.0f)
		{
			// プランクトンを奪う
			if (list->SetPlankton(planktonList.back()))
			{
				planktonList.pop_back();
			}
		}
	}

	CTimer *pTimer = CGame::GetTimer();

	if (pTimer->IsTimerStop())
	{
		return;
	}

	m_nSpawnCounter--;

	if (m_nSpawnCounter <= 0)
	{
		Spawn();
		Spawn();
		Spawn();
		Spawn();

		m_nSpawnCounter = FishControllerConst::SPAWN_TIME;
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

//===================================================
// スポーン処理
//===================================================
void CFishController::Spawn(void)
{
	// 半径（内側と外側）
	const float minRadius = 400.0f;
	const float maxRadius = 700.0f;

	// 0.0～1.0
	float r = math::Random(0.0f, 1.0f);

	// ドーナツ分布
	float radius = sqrtf(r) * (maxRadius - minRadius) + minRadius;

	// 角度
	float angle = (rand() % CMathConstant::I_ANGLE_MAX) * (D3DX_PI / CMathConstant::F_ANGLE_HALF);

	// 中心位置
	D3DXVECTOR3 centerPos = D3DXVECTOR3(0.0f, -100.0f, 200.0f);

	// 最終位置
	D3DXVECTOR3 pos;
	pos.x = centerPos.x + cosf(angle) * radius;
	pos.y = centerPos.y;
	pos.z = centerPos.z + sinf(angle) * radius;

	// 生成処理
	Create(centerPos, "data/MODEL/fish.x", 1800);
}