//===================================================
//
// エフェクトのクラス [effect.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "effect.h"
#include "manager.h"
#include "renderer.h"

//===================================================
// コンストラクタ
//===================================================
CEffect::CEffect() : 
	m_info(),
	m_posOld(Const::VEC3_NULL),
	m_fDecRadius(0.0f),
	m_fDecAlv(0.0f)
{
}

//===================================================
// デストラクタ
//===================================================
CEffect::~CEffect()
{
}

//===================================================
// 生成処理
//===================================================
CEffect* CEffect::Create(const Info& info, const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const D3DXCOLOR& col, const char* pTexturePath)
{
	// 自分自身の生成
	CEffect* pInstance = new CEffect;

	// テクスチャのIDの設定
	pInstance->SetTextureID(pTexturePath);
	pInstance->SetPosition(pos);
	pInstance->SetSize(size);
	pInstance->SetColor(col);
	pInstance->m_info = info;

	// 初期化処理に失敗したら
	if (FAILED(pInstance->Init()))
	{
		pInstance->Uninit();
		pInstance = nullptr;
		return nullptr;
	}

	return pInstance;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CEffect::Init(void)
{
	// 初期化処理
	if (FAILED(CObjectBillboard::Init()))
	{
		return E_FAIL;
	}

	// 色の取得
	D3DXCOLOR col = CObjectBillboard::GetColor();

	// 大きさの取得
	D3DXVECTOR2 size = CObjectBillboard::GetSize();

	// 1フレームに減る量を求める
	m_fDecAlv		= col.a  / m_info.nLife;
	m_fDecRadius	= size.x / m_info.nLife;

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CEffect::Uninit(void)
{
	// 終了処理
	CObjectBillboard::Uninit();
}

//===================================================
// 更新処理
//===================================================
void CEffect::Update(void)
{
	D3DXVECTOR3 pos = CObjectBillboard::GetPosition();

	// 重力をつけるなら
	if (m_info.unFlag & FLAG_GRAVITY)
	{
		// 重力を設定
		m_info.move.y -= m_info.fGravity;
	}

	// 慣性をつけるなら
	if (m_info.unFlag & FLAG_INERTIA)
	{
		// 移動量の減衰
		m_info.move.x *= m_info.fInertia;
		m_info.move.z *= m_info.fInertia;
	}

	// 前回の位置の取得
	m_posOld = pos;

	// 移動量を加算
	pos += m_info.move;

	// 寿命が尽きたら
	if (m_info.nLife <= 0)
	{
		Uninit();
	}

	// アルファ値の更新処理
	UpdateAlpha();

	// 大きさの更新処理
	UpdateSize();

	// 寿命を減らす
	m_info.nLife--;

	CObjectBillboard::SetPosition(pos);

	// 更新処理
	CObjectBillboard::Update();
}

//===================================================
// 描画処理
//===================================================
void CEffect::Draw(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// レンダラーの取得
	CRenderer* pRenderer = pManager->GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ゼットテスト
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, NULL);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// aブレンディング
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 描画処理
	CObjectBillboard::Draw();

	// 補間処理
	Lerp();

	// ゼットテスト
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// アルファテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// aブレンディングをもとに戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//===================================================
// アルファ値の更新処理
//===================================================
void CEffect::UpdateAlpha(void)
{
	// アルファ値を減らすかどうか判定
	const bool bAlphaFlag = m_info.unFlag & FLAG_ALPHA_DECREASE;

	// アルファ値を減らさないなら処理しない
	if (bAlphaFlag == false)
	{
		return;
	}

	// 色の取得
	D3DXCOLOR col = CObjectBillboard::GetColor();

	// アルファ値を減らす
	col.a -= m_fDecAlv;

	// 色を更新
	CObjectBillboard::SetColor(col);
}

//===================================================
// 大きさの更新処理
//===================================================
void CEffect::UpdateSize(void)
{
	// 大きさを減らすかどうか判定
	const bool bRadiusFlag = m_info.unFlag & FLAG_RADIUS_DECREASE;

	// 大きさを減らさないなら処理しない
	if (bRadiusFlag == false)
	{
		return;
	}

	// 大きさの取得
	D3DXVECTOR2 size = CObjectBillboard::GetSize();

	// 大きさを減らす
	size.x -= m_fDecRadius;
	size.y -= m_fDecRadius;

	// 大きさを更新
	CObjectBillboard::SetSize(size);
}

//===================================================
// 補間処理
//===================================================
void CEffect::Lerp(void)
{
	// 間を補間するかどうか判定
	const bool bLerpFlag = m_info.unFlag & FLAG_LERP;

	// 間を補間しないなら処理しない
	if (bLerpFlag == false)
	{
		return;
	}

	D3DXVECTOR3 pos = CObjectBillboard::GetPosition();
	D3DXVECTOR2 size = CObjectBillboard::GetSize();
	D3DXCOLOR col = CObjectBillboard::GetColor();

	// 補完する分回す
	for (int nCnt = 0; nCnt < m_info.nNumLerp; nCnt++)
	{
		// 割合を求める
		float fRate = nCnt / static_cast<float>(m_info.nNumLerp);

		// 今の位置と前回の位置を求める
		D3DXVECTOR3 diff = pos - m_posOld;

		// 設定する位置
		D3DXVECTOR3 setPos = m_posOld + diff * fRate;

		// 補間用エフェクト用の位置を設定
		CObjectBillboard::SetPosition(setPos);

		// 描画処理
		CObjectBillboard::Draw();
	}
}