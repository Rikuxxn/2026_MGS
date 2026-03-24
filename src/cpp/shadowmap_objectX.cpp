//===================================================
//
// シャドウマップを適応するObjectXクラス [shadowmap_objectX.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "shadowmap_objectX.h"
#include "shader.h"
#include "shader_manager.h"
#include "manager.h"
#include "renderer.h"

//===================================================
// コンストラクタ
//===================================================
CShadowMapObjectX::CShadowMapObjectX()
{
}

//===================================================
// デストラクタ
//===================================================
CShadowMapObjectX::~CShadowMapObjectX()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT CShadowMapObjectX::Init(void)
{
	// 初期化処理
	if (FAILED(CObjectX::Init()))
	{
		return E_FAIL;
	}
	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CShadowMapObjectX::Uninit(void)
{
	// 終了処理
	CObjectX::Uninit();
}

//===================================================
// 更新処理
//===================================================
void CShadowMapObjectX::Update(void)
{
	// 更新処理
	CObjectX::Update();
}

//===================================================
// 描画処理
//===================================================
void CShadowMapObjectX::Draw(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// レンダラーの取得
	CRenderer* pRenderer = pManager->GetRenderer();

	// シェーダーのマネージャーの取得
	CShaderManager* pShaderManager = pManager->GetShaderManager();

	// シャドウマップの取得
	CShader* pShader = pShaderManager->GetAddrees(CShaderManager::TYPE_SHADOW_MAP);

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxView, mtxProj,mtxWorld;

	// ワールドマトリックスの取得
	mtxWorld = CObjectX::GetMatrixWorld();

	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);

	// マトリックスの設定
	pShader->SetMatrix("g_mtxWorld", mtxWorld);
	pShader->SetMatrix("g_mtxView", mtxView);
	pShader->SetMatrix("g_mtxProjection", mtxProj);

	// コミットチェンジ
	pShader->CommitChange();

	// 描画処理
	CObjectX::Draw();
}