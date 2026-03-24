//=============================================================================
//
// スカイキューブ処理 [SkyCube.cpp]
// Author: RIKU TANEKAWA
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "SkyCube.h"
#include "Manager.h"
#include "renderer.h"
#include "texture_manager.h"
#include "shader.h"
#include "shader_manager.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CSkyCube::CSkyCube(const PRIORITY priority) : CObject(priority)
{
	// 値のクリア
	m_pVtxBuff		= nullptr;		// 頂点バッファへのポインタ
	m_nIdxTexture	= -1;			// テクスチャのインデックス
}
//=============================================================================
// デストラクタ
//=============================================================================
CSkyCube::~CSkyCube()
{
	// なし
}
//=============================================================================
// 生成処理
//=============================================================================
CSkyCube* CSkyCube::Create(void)
{
	CSkyCube* pSkyCube = new CSkyCube;

	// nullptrだったら
	if (pSkyCube == nullptr)
	{
		return nullptr;
	}

	// 初期化失敗時
	if (FAILED(pSkyCube->Init()))
	{
		return nullptr;
	}

	return pSkyCube;
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSkyCube::Init(void)
{
	// レンダラーの取得
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの取得
	CTextureManager* pTexture = CManager::GetInstance()->GetTextureManager();

	// キューブマップ用テクスチャのIDの設定
	m_nIdxTexture = pTexture->RegisterCube(
		"data/TEXTURE/SKYCUBE/posx.jpg",
		"data/TEXTURE/SKYCUBE/negx.jpg",
		"data/TEXTURE/SKYCUBE/posy.jpg",
		"data/TEXTURE/SKYCUBE/negy.jpg",
		"data/TEXTURE/SKYCUBE/posz.jpg",
		"data/TEXTURE/SKYCUBE/negz.jpg");

	// フルスクリーンクアッド生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-1, 1, 1);
	pVtx[1].pos = D3DXVECTOR3(1, 1, 1);
	pVtx[2].pos = D3DXVECTOR3(-1, -1, 1);
	pVtx[3].pos = D3DXVECTOR3(1, -1, 1);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CSkyCube::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// 自身の破棄
	this->Release();
}
//=============================================================================
// 更新処理
//=============================================================================
void CSkyCube::Update(void)
{


}
//=============================================================================
// 描画処理
//=============================================================================
void CSkyCube::Draw(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// レンダラーの取得
	CRenderer* pRenderer = pManager->GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの取得
	CTextureManager* pTexture = pManager->GetTextureManager();

	// シェーダーマネージャーの取得
	CShaderManager* pShaderManager = pManager->GetShaderManager();

	// 空のシェーダーの取得
	CShader* pShader = pShaderManager->GetAddrees(CShaderManager::TYPE_SKY_CUBE);

	// カメラの取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// Zテスト設定
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// サンプラーステートの設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);

	if (pShader != nullptr)
	{
		pShader->Begin();

		pShader->BeginPass();
	}

	// 行列
	D3DXMATRIX mtxView;
	D3DXMATRIX mtxProj;

	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);

	// Viewの平行移動除去
	mtxView._41 = mtxView._42 = mtxView._43 = 0.0f;

	D3DXMATRIX invVP;
	D3DXMATRIX vp = mtxView * mtxProj;
	D3DXMatrixInverse(&invVP, nullptr, &vp);

	// マトリックスの設定
	pShader->SetMatrix("g_InvViewProj", invVP);

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetCubeAddress(m_nIdxTexture));

	// ポリゴン描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	if (pShader != nullptr)
	{
		pShader->EndPass();

		pShader->End();
	}

	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}
