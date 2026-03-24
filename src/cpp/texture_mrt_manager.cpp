//===================================================
//
// マルチレンダーターゲットのテクスチャの管理 [texture_mt_manager.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "texture_mrt_manager.h"
#include "manager.h"
#include "renderer.h"
#include "color_constants.h"

//===================================================
// コンストラクタ
//===================================================
CTextureMRTManager::CTextureMRTManager() : 
	m_aInfo(),
	m_defInfo(),
	m_mtxViewDef(Const::MTX_IDENTITY),
	m_mtxProjDef(Const::MTX_IDENTITY)
{
}

//===================================================
// デストラクタ
//===================================================
CTextureMRTManager::~CTextureMRTManager()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT CTextureMRTManager::Init(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	LPDIRECT3DSURFACE9 pRenderDef, pZBufferDef;

	// 総数分回す
	for (auto& dec : CREATE_TABLE)
	{
		auto& info = m_aInfo[dec.type];

		// レンダラーターゲット用テクスチャの生成
		pDevice->CreateTexture(
			dec.nWidth,
			dec.nHeight,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A16B16G16R16F,
			D3DPOOL_DEFAULT,
			&info.pTextureMT,
			NULL);

		// テクスチャのレンダリングターゲット用インターフェースの生成
		info.pTextureMT->GetSurfaceLevel(0, &info.pRenderMT);

		// テクスチャレンダリング用Zバッファの生成
		pDevice->CreateDepthStencilSurface(
			dec.nWidth,
			dec.nHeight,
			D3DFMT_D24S8,
			D3DMULTISAMPLE_NONE,
			0,
			TRUE,
			&info.pZBuffMT,
			NULL);

		// 現在のレンダリングターゲットを取得(保存)
		pDevice->GetRenderTarget(0, &pRenderDef);

		// 現在のZバッファの取得(保存)
		pDevice->GetDepthStencilSurface(&pZBufferDef);

		// レンダリングターゲットを生成したテクスチャに設定
		pDevice->SetRenderTarget(0, info.pRenderMT);

		// Zバッファを生成したZバッファの設定
		pDevice->SetDepthStencilSurface(info.pZBuffMT);

		// レンダリングターゲット用テクスチャのクリア
		pDevice->Clear(0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
			Const::BACK_BUFFER_COLOR, 1.0f, 0);

		// レンダーターゲットをもとに戻す
		pDevice->SetRenderTarget(0, pRenderDef);

		// Zバッファをもとに戻す
		pDevice->SetDepthStencilSurface(pZBufferDef);

		// テクスチャレンダリング用ビューポートの生成
		info.viewport.X = 0;
		info.viewport.Y = 0;
		info.viewport.Width = dec.nWidth;
		info.viewport.Height = dec.nHeight;
		info.viewport.MinZ = 0.0f;
		info.viewport.MaxZ = 1.0f;
		info.nType = dec.type;
	}

    return S_OK;
}

//===================================================
// すべての破棄
//===================================================
void CTextureMRTManager::Release(void)
{
    // 総数分回す
    for (auto& info : m_aInfo)
    {
        // レンダーターゲット用のインターフェスの破棄
        if (info.pRenderMT != nullptr)
        {
            info.pRenderMT->Release();
            info.pRenderMT = nullptr;
        }

        // レンダーターゲット用のテクスチャの破棄
        if (info.pTextureMT != nullptr)
        {
            info.pTextureMT->Release();
            info.pTextureMT = nullptr;
        }

        // Zバッファの破棄
        if (info.pZBuffMT != nullptr)
        {
            info.pZBuffMT->Release();
            info.pZBuffMT = nullptr;
        }
	}
}

//===================================================
// 指定した情報の破棄
//===================================================
void CTextureMRTManager::Release(const int nIdx)
{
	// レンダーターゲット用のインターフェスの破棄
	if (m_aInfo[nIdx].pRenderMT != nullptr)
	{
		m_aInfo[nIdx].pRenderMT->Release();
		m_aInfo[nIdx].pRenderMT = nullptr;
	}

	// レンダーターゲット用のテクスチャの破棄
	if (m_aInfo[nIdx].pTextureMT != nullptr)
	{
		m_aInfo[nIdx].pTextureMT->Release();
		m_aInfo[nIdx].pTextureMT = nullptr;
	}

	// Zバッファの破棄
	if (m_aInfo[nIdx].pZBuffMT != nullptr)
	{
		m_aInfo[nIdx].pZBuffMT->Release();
		m_aInfo[nIdx].pZBuffMT = nullptr;
	}
}

//===================================================
// アドレスの取得
//===================================================
LPDIRECT3DTEXTURE9 CTextureMRTManager::GetAdress(const int nType)
{
	if (nType < 0)
	{
		return nullptr;
	}
	return m_aInfo[nType].pTextureMT;
}

//===================================================
// レンダーターゲットの変更処理
//===================================================
void CTextureMRTManager::ChangeRenderTarget(const int nIdx, const int nStage)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	if (m_defInfo.pRenderMT == nullptr)
	{
		auto table = CREATE_TABLE[nIdx];

		// 現在のレンダーターゲットの取得
		pDevice->GetRenderTarget(nStage, &m_defInfo.pRenderMT);

		// 現在のZバッファの取得
		pDevice->GetDepthStencilSurface(&m_defInfo.pZBuffMT);

		// 現在のビューポートの取得
		pDevice->GetViewport(&m_defInfo.viewport);

		// 現在のビューマトリックスの取得
		pDevice->GetTransform(D3DTS_VIEW, &m_mtxViewDef);

		// 現在のプロジェクションマトリックスの取得
		pDevice->GetTransform(D3DTS_PROJECTION, &m_mtxProjDef);

		D3DXMATRIX mtxView, mtxProjection; // ビューマトリックス、プロジェクションマトリックス
		float fAspect;

		// レンダリングターゲットを生成したテクスチャに設定
		pDevice->SetRenderTarget(0, m_aInfo[nIdx].pRenderMT);

		// 生成したZバッファに設定
		pDevice->SetDepthStencilSurface(m_aInfo[nIdx].pZBuffMT);

		// テクスチャレンダリング用ビューポートの設定
		pDevice->SetViewport(&m_aInfo[nIdx].viewport);

		// ビューマトリックスの初期化
		D3DXMatrixIdentity(&mtxView);

		// プロジェクションマトリックスの初期化
		D3DXMatrixIdentity(&mtxProjection);

		// ビューマトリックスの作成
		D3DXMatrixLookAtLH(
			&mtxView,
			&table.posV,
			&table.posR,
			&Const::VEC_U);

		// ビューマトリックスの設定
		pDevice->SetTransform(D3DTS_VIEW, &mtxView);

		// アスペクト比
		fAspect = (float)m_aInfo[nIdx].viewport.Width / (float)m_aInfo[nIdx].viewport.Height;

		// プロジェクションマトリックスの作成
		D3DXMatrixPerspectiveFovLH(&mtxProjection,
			D3DXToRadian(45.0f),
			fAspect,
			1.0f,
			5000.0f);

		// プロジェクションマトリックスの設定
		pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);

		// クリアするなら
		if (table.bClear)
		{
			// レンダリングターゲット用テクスチャのクリア
			pDevice->Clear(0,
				NULL,
				(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
				Color::RED, 1.0f, 0);
		}
	}
}

//===================================================
// レンダーターゲットのリセット処理
//===================================================
void CTextureMRTManager::ResetRenderTarget(const int nStage)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

	// 設定されていたら
	if (m_defInfo.pRenderMT != nullptr)
	{
		// レンダーターゲットをもとに戻す
		pDevice->SetRenderTarget(nStage, m_defInfo.pRenderMT);

		// Zバッファをもとに戻す
		pDevice->SetDepthStencilSurface(m_defInfo.pZBuffMT);

		// 使用が終わったので破棄
		if (m_defInfo.pRenderMT != nullptr)
		{
			m_defInfo.pRenderMT->Release();
			m_defInfo.pRenderMT = nullptr;
		}
		if (m_defInfo.pZBuffMT != nullptr)
		{
			m_defInfo.pZBuffMT->Release();
			m_defInfo.pZBuffMT = nullptr;
		}

		// ビューポートをもとに戻す
		pDevice->SetViewport(&m_defInfo.viewport);

		// ビューマトリックスをもとに戻す
		pDevice->SetTransform(D3DTS_VIEW, &m_mtxViewDef);

		// プロジェクションマトリックスをもとに戻す
		pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjDef);
	}
}