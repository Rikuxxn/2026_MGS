//===================================================
//
// 2Dオブジェクトの基底クラス [object2d.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "object2d.h"
#include "texture_manager.h"
#include "manager.h"
#include "renderer.h"

//===================================================
// コンストラクタ
//===================================================
CObject2D::CObject2D(const PRIORITY priority) : 
	CObject(priority),
	m_pVtxBuffer(nullptr),
	m_pos(Const::VEC3_NULL),
	m_size(Const::VEC2_NULL),
	m_fRot(0.0f),
	m_fAngle(0.0f),
	m_fLength(0.0f),
	m_nTextureID(CTextureManager::INVALID_ID)
{
}

//===================================================
// デストラクタ
//===================================================
CObject2D::~CObject2D()
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
CObject2D* CObject2D::Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const D3DCOLOR& col, const char* pTexturePath, const float fRot)
{
	// 自分自身の生成
	CObject2D* pInstance = new CObject2D;

	// パラメータの設定
	pInstance->m_pos	= pos;
	pInstance->m_size	= size;
	pInstance->m_fRot	= fRot;
	pInstance->m_col	= col;

	// 初期化処理に失敗したら
	if (FAILED(pInstance->Init()))
	{
		// 終了処理
		pInstance->Uninit();
		pInstance = nullptr;
		return nullptr;
	}

	// テクスチャのIDの設定
	pInstance->SetTextureID(pTexturePath);

	return pInstance;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CObject2D::Init(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// レンダラーの取得
	CRenderer* pRenderer = pManager->GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファの生成・頂点情報の設定
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
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
void CObject2D::Uninit(void)
{
	// 自分自身の破棄
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CObject2D::Update(void)
{
	// 頂点の設定
	SetVtx();
}

//===================================================
// 描画処理
//===================================================
void CObject2D::Draw(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// レンダラーの取得
	CRenderer* pRenderer = pManager->GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャマネージャーの取得
	CTextureManager* pTextureManager = pManager->GetTextureManager();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャを設定
	pDevice->SetTexture(0, pTextureManager->GetAdress(m_nTextureID));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2); // プリミティブの種類	
}

//===================================================
// テクスチャのIDの設定
//===================================================
void CObject2D::SetTextureID(const char* pTexturePath)
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
// 頂点の設定
//===================================================
void CObject2D::SetVtx(void)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx = nullptr;

	// 頂点バッファのロック
	if (FAILED(m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0)))
	{
		return;
	}

	m_fLength = sqrtf((m_size.x * m_size.x) + (m_size.y * m_size.y));
	m_fAngle = atan2f(m_size.x, m_size.y);

	// 頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(m_fRot - (D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_fRot - (D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + sinf(m_fRot + (D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_fRot + (D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_fRot - m_fAngle) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_fRot - m_fAngle) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_fRot + m_fAngle) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_fRot + m_fAngle) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 色の設定
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