//===================================================
//
// textureアニメーションを適応するクラス [texture_animation.cpp]
// Author:YUTO YOSHIDA
// 
// 2D,3Dポリゴンがメンバ変数にもつ
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "texture_animation.h"
#include "object.h"

//===================================================
// コンストラクタ
//===================================================
CTextureAnimation::CTextureAnimation() : 
	m_type(TYPE_2D),
	m_segment(),
	m_nFrame(0),
	m_nPattern(0),
	m_nCounter(0),
	m_bLoop(true)
{
}

//===================================================
// デストラクタ
//===================================================
CTextureAnimation::~CTextureAnimation()
{

}

//===================================================
// 生成処理
//===================================================
std::unique_ptr<CTextureAnimation> CTextureAnimation::Create(const TYPE type, const VECTOR2INT& segment, const int nFrame, const bool bLoop)
{
	// 自分自身の生成
	auto pInstance = std::make_unique<CTextureAnimation>();

	pInstance->m_type = type;
	pInstance->m_segment = segment;
	pInstance->m_nFrame = nFrame;
	pInstance->m_bLoop = bLoop;

	return pInstance;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CTextureAnimation::Init(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer)
{
	// 種類ごとの処理
	switch (m_type)
	{
	case TYPE::TYPE_2D:
		Apply2D(pVtxBuffer);
		break;
	case TYPE::TYPE_3D:
		Apply3D(pVtxBuffer);
		break;
	default:
		break;
	}

	return S_OK;
}

//===================================================
// 更新処理
//===================================================
void CTextureAnimation::Update(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, CObject* pOwner)
{
	// カウンターを超えたら
	if (m_nCounter >= m_nFrame)
	{
		// パターンを増やす
		m_nPattern++;

		// アニメーションのカウンターを進める
		m_nCounter = 0;

		// 種類ごとの処理
		switch (m_type)
		{
		case TYPE::TYPE_2D:
			Apply2D(pVtxBuffer);
			break;
		case TYPE::TYPE_3D:
			Apply3D(pVtxBuffer);
			break;
		default:
			break;
		}

		// パターンが最大まで行ったか判定
		const bool bPatternMax = m_nPattern >= (m_segment.x * m_segment.y);

		// ループしないなら
		if (bPatternMax && m_bLoop == false && pOwner != nullptr)
		{
			// 終了処理
			pOwner->Uninit();
			return;
		}

		// 最大まで行ったら
		if (bPatternMax)
		{
			// パターンを初期化
			m_nPattern = 0;
		}
	}

	m_nCounter++;
}

//===================================================
// 適応処理(通常の2Dポリゴン)
//===================================================
void CTextureAnimation::Apply2D(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer)
{
	// 頂点情報へのポインタ
	VERTEX_2D* pVtx = nullptr;

	// 頂点バッファをロック
	if (FAILED(pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0)))
	{
		return;
	}

	float fPosTexU = 1.0f / m_segment.x;
	float fPosTexH = 1.0f / m_segment.y;

	// Y座標の割合
	int nRatePosY = m_nPattern / m_segment.x;

	// テクスチャ座標の計算
	float fNewTexPosU = fPosTexU * m_nPattern;
	float fNewTexPosV = nRatePosY * fPosTexH;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(fNewTexPosU, fNewTexPosV);
	pVtx[1].tex = D3DXVECTOR2(fNewTexPosU + fPosTexU, fNewTexPosV);
	pVtx[2].tex = D3DXVECTOR2(fNewTexPosU, fNewTexPosV + fPosTexH);
	pVtx[3].tex = D3DXVECTOR2(fNewTexPosU + fPosTexU, fNewTexPosV + fPosTexH);

	// 頂点バッファをアンロック
	pVtxBuffer->Unlock();
}


//===================================================
// 3Dの適応処理
//===================================================
void CTextureAnimation::Apply3D(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer)
{
	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	if (FAILED(pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0)))
	{
		return;
	}

	float fPosTexU = 1.0f / m_segment.x;
	float fPosTexH = 1.0f / m_segment.y;

	// Y座標の割合
	int nRatePosY = m_nPattern / m_segment.x;

	// テクスチャ座標の計算
	float fNewTexPosU = fPosTexU * m_nPattern;
	float fNewTexPosV = nRatePosY * fPosTexH;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(fNewTexPosU, fNewTexPosV);
	pVtx[1].tex = D3DXVECTOR2(fNewTexPosU + fPosTexU, fNewTexPosV);
	pVtx[2].tex = D3DXVECTOR2(fNewTexPosU, fNewTexPosV + fPosTexH);
	pVtx[3].tex = D3DXVECTOR2(fNewTexPosU + fPosTexU, fNewTexPosV + fPosTexH);

	// 頂点バッファをアンロック
	pVtxBuffer->Unlock();
}