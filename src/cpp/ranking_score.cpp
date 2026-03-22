//===================================================
//
// ランキングのスコア [ranking_score.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "ranking_score.h"
#include "score.h"

//===================================================
// コンストラクタ
//===================================================
CRankingScore::CRankingScore() : 
	m_pScore(nullptr)
{
}

//===================================================
// デストラクタ
//===================================================
CRankingScore::~CRankingScore()
{
	m_pScore = nullptr;
}

//===================================================
// 生成処理
//===================================================
CRankingScore* CRankingScore::Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const char* pTexturePath, const int nScore)
{
	// 自分自身の生成
	CRankingScore* pInstance = new CRankingScore;

	// スコアの生成
	pInstance->m_pScore = CScore::Create(pos, size, nScore, pTexturePath);

	// 自分自身の生成
	if (FAILED(pInstance->Init()))
	{
		// 終了処理
		pInstance->Uninit();
		pInstance = nullptr;
		return nullptr;
	}
	return pInstance;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CRankingScore::Init(void)
{
	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CRankingScore::Uninit(void)
{
	// 自分自身の破棄
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CRankingScore::Update(void)
{
}

//===================================================
// 描画処理
//===================================================
void CRankingScore::Draw(void)
{

}