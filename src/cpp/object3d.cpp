//===================================================
//
// 3Dオブジェクトの基底クラス [object3d.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "object3d.h"
#include "manager.h"
#include "renderer.h"
#include "texture_manager.h"

//===================================================
// コンストラクタ
//===================================================
CObject3D::CObject3D(const PRIORITY priority) : 
	CObject(priority),
	m_pVtxBuffer(nullptr),
	m_mtxWorld(Const::MTX_IDENTITY),
	m_col(Const::WHITE),
	m_pos(Const::VEC3_NULL),
	m_rot(Const::VEC3_NULL),
	m_size(Const::VEC3_NULL),
	m_nTextureID(CTextureManager::INVALID_ID)
{
}

//===================================================
// デストラクタ
//===================================================
CObject3D::~CObject3D()
{
	// 頂点バッファの破棄
	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}
}

//===================================================
// 生成処理
//===================================================
CObject3D* CObject3D::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXVECTOR3& rot, const D3DCOLOR& col, const char* pTexturePath)
{
	// 自分自身の生成
	CObject3D* pInstance = new CObject3D;

	// テクスチャのIDの設定処理
	pInstance->SetTextureID(pTexturePath);

	pInstance->m_pos = pos;
	pInstance->m_size = size;
	pInstance->m_rot = rot;
	pInstance->m_col = col;


	// 初期化に失敗したら
	if (FAILED(pInstance->Init()))
	{
		// 終了処理
		pInstance->Uninit();
		pInstance = nullptr;
	}

	return pInstance;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CObject3D::Init(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// レンダラーの取得
	CRenderer* pRenderer = pManager->GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファの生成・頂点情報の設定
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	// 頂点の設定
	SetVtx();

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CObject3D::Uninit(void)
{
	// 自分自身の破棄
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CObject3D::Update(void)
{
	// 頂点バッファの設定
	SetVtx();
}

//===================================================
// 描画処理
//===================================================
void CObject3D::Draw(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// レンダラーの取得
	CRenderer* pRenderer = pManager->GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// テクスチャマネージャーの取得
	CTextureManager* pTextureManager = pManager->GetTextureManager();

	//	ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスを設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ設定
	pDevice->SetTexture(0, pTextureManager->GetAdress(m_nTextureID));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//===================================================
// テクスチャのIDの設定処理
//===================================================
void CObject3D::SetTextureID(const char* pTexturePath)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// テクスチャマネージャーの取得
	CTextureManager* pTextureManager = pManager->GetTextureManager();

	// テクスチャのIDの取得
	if (pTextureManager == nullptr)
	{
		return;
	}

	// テクスチャのIDの設定
	m_nTextureID = pTextureManager->Register(pTexturePath);
}

//===================================================
// 頂点の設定処理
//===================================================
void CObject3D::SetVtx(void)
{
	// 頂点情報のポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファのロック
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 大きさの取得
	D3DXVECTOR3 size = m_size;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-size.x, size.y, size.z);
	pVtx[1].pos = D3DXVECTOR3(size.x, size.y, size.z);
	pVtx[2].pos = D3DXVECTOR3(-size.x, -size.y, -size.z);
	pVtx[3].pos = D3DXVECTOR3(size.x, -size.y, -size.z);

	// 法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファのアンロック
	m_pVtxBuffer->Unlock();
}