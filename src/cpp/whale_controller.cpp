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
#include "MathConst.h"
#include "score.h"
#include "game.h"

//===================================================
// コンストラクタ
//===================================================
CWhaleController::CWhaleController() : 
	m_pPlayer(nullptr),
	m_nSatisfiedWhaleNum(0)
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

	// 初期に数体クジラをスポーンしておく
	for (int nCnt = 0; nCnt < MAX_WHALE_NUM; nCnt++)
	{
		// クジラの生成
		pInstance->SpawnWhale();
	}

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
// ドーナツ状にクジラを生成する処理
//===================================================
void CWhaleController::SpawnWhale(void)
{
	// 半径（内側と外側）
	const float minRadius = 400.0f;
	const float maxRadius = 700.0f;

	// 0.0～1.0
	float r = (rand() % 1000) / 1000.0f;

	// ドーナツ分布
	float radius = sqrtf(r) * (maxRadius - minRadius) + minRadius;

	// 角度
	float angle = (rand() % CMathConstant::I_ANGLE_MAX) * (D3DX_PI / CMathConstant::F_ANGLE_HALF);

	// 中心位置
	D3DXVECTOR3 centerPos = D3DXVECTOR3(0.0f, -10.0f, 200.0f);

	// 最終位置
	D3DXVECTOR3 pos;
	pos.x = centerPos.x + cosf(angle) * radius;
	pos.y = centerPos.y;
	pos.z = centerPos.z + sinf(angle) * radius;

	// 生成
	Create(pos, { 0,0,0 }, "data/MOTION/motion_whole.txt");
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

//===================================================
// クジラを満足させたか
//===================================================
void CWhaleController::OnWhaleSatisfied(CWhale* whale)
{
	 // 満足したクジラの数
	m_nSatisfiedWhaleNum++;

	// スコアの取得
	CScore* pScore = CGame::GetScore();

	if (pScore != nullptr)
	{
		pScore->SetScore(m_nSatisfiedWhaleNum);
	}

	// 新しく補充
	SpawnWhale();
}

//===================================================
// 要素の結合解除
//===================================================
void CWhaleController::Erase(CWhale* whale)
{
	// イテレータの取得
	auto itr = std::find(m_pList.begin(), m_pList.end(), whale);

	if (itr == m_pList.end())
	{
		return;
	}

	// 結合の解除
	itr = m_pList.erase(itr);
}