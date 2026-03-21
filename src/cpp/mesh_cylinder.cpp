//===================================================
//
// メッシュシリンダー [mesh_cylinder.cp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "mesh_cylinder.h"
#include "manager.h"
#include "renderer.h"
#include "texture_manager.h"

//===================================================
// コンストラクタ
//===================================================
CMeshCylinder::CMeshCylinder() : 
	CObject(PRIORITY_MESH),
	m_pVtxBuffer(nullptr),
	m_pIdxBuffer(nullptr),
	m_mtxWorld(Const::MTX_IDENTITY),
	m_col(Const::WHITE),
	m_pos(Const::VEC3_NULL),
	m_rot(Const::VEC3_NULL),
	m_fRadius(0.0f),
	m_fHeight(0.0f),
	m_segment(),
	m_nNumVertex(0),
	m_nNumPolygon(0),
	m_nNumIndex(0),
	m_nTextureID(CTextureManager::INVALID_ID)
{
}

//===================================================
// デストラクタ
//===================================================
CMeshCylinder::~CMeshCylinder()
{
	// 頂点バッファの破棄
	if (m_pVtxBuffer != nullptr)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = nullptr;
	}
	// インデックスバッファの破棄
	if (m_pIdxBuffer != nullptr)
	{
		m_pIdxBuffer->Release();
		m_pIdxBuffer = nullptr;
	}
}

//===================================================
// 生成処理
//===================================================
CMeshCylinder* CMeshCylinder::Create(const D3DXVECTOR3& pos, const float fRadius, const float fHeight, const D3DXCOLOR& col, const VECTOR2INT& segment, const char* pTexturePath)
{
	// 自分自身の生成
	CMeshCylinder* pInstance = new CMeshCylinder;

	// テクスチャのIDの設定
	pInstance->SetTextureID(pTexturePath);

	pInstance->m_pos = pos;
	pInstance->m_fHeight = fHeight;
	pInstance->m_fRadius = fRadius;
	pInstance->m_col = col;
	pInstance->m_segment = segment;

	// 初期化処理に失敗したら
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
HRESULT CMeshCylinder::Init(void)
{
	// 頂点数の設定
	m_nNumVertex = (m_segment.x + 1) * (m_segment.y + 1);

	// ポリゴン数の設定
	m_nNumPolygon = ((m_segment.x * m_segment.y) * 2) + (4 * (m_segment.y - 1));

	// インデックス数の設定
	m_nNumIndex = m_nNumPolygon + 2;

	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// レンダラーの取得
	CRenderer* pRenderer = pManager->GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	//インデックスバッファの生成
	if (FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuffer,
		NULL)))
	{
		return E_FAIL;
	}

	// 頂点のカウント
	int nCntVtx = 0;

	float fTexPosX = 1.0f / m_segment.x;
	float fTexPosY = 1.0f / m_segment.y;

	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 縦の分割数分回す
	for (int nCntV = 0; nCntV <= m_segment.y; nCntV++)
	{
		for (int nCntH = 0; nCntH <= m_segment.x; nCntH++)
		{
			// 計算用の位置
			D3DXVECTOR3 posWk = Const::VEC3_NULL;

			// 横の分割数
			float fAngle = (D3DX_PI * 2.0f) / m_segment.x * nCntH;

			// 位置の設定
			posWk.x = sinf(fAngle) * m_fRadius;
			posWk.y = m_fHeight - (m_fHeight / m_segment.y * nCntV);
			posWk.z = cosf(fAngle) * m_fRadius;

			// 位置の設定
			pVtx[nCntVtx].pos = posWk;

			// 頂点までの方向を求める
			D3DXVECTOR3 nor = pVtx[nCntVtx].pos - m_pos;

			// 方向ベクトルにする
			D3DXVec3Normalize(&nor, &nor);

			// 法線の設定
			pVtx[nCntVtx].nor = nor;

			// 色の設定
			pVtx[nCntVtx].col = m_col;

			// テクスチャ座標の設定
			pVtx[nCntVtx].tex = D3DXVECTOR2((fTexPosX * nCntH), (fTexPosY * nCntV));

			nCntVtx++;
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuffer->Unlock();

	int IndxNum = m_segment.x + 1; // インデックスの数値1

	int IdxCnt = 0; // 配列

	int Num = 0; // インデックスの数値2

	// インデックスバッファへのポインタ
	WORD* pIdx;

	// インデックスバッファのロック
	m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	//インデックスの設定
	for (int IndxCount1 = 0; IndxCount1 < m_segment.y; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= m_segment.x; IndxCount2++, IndxNum++, Num++)
		{
			// インデックスバッファの設定
			pIdx[IdxCnt] = static_cast<WORD>(IndxNum);
			pIdx[IdxCnt + 1] = static_cast<WORD>(Num);
			IdxCnt += 2;
		}

		// NOTE:最後の行じゃなかったら
		if (IndxCount1 < m_segment.y - 1)
		{
			pIdx[IdxCnt] = static_cast<WORD>(Num - 1);
			pIdx[IdxCnt + 1] = static_cast<WORD>(IndxNum);
			IdxCnt += 2;
		}
	}

	//インデックスバッファのアンロック
	m_pIdxBuffer->Unlock();

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CMeshCylinder::Uninit(void)
{
	// 終了処理
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CMeshCylinder::Update(void)
{
}

//===================================================
// 描画処理
//===================================================
void CMeshCylinder::Draw(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// レンダラーの取得
	CRenderer* pRenderer = pManager->GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャマネージャーの取得
	CTextureManager* pTextureManager = pManager->GetTextureManager();

	// 計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuffer);

	// テクスチャフォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTextureManager->GetAdress(m_nTextureID));

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);
}

//===================================================
// テクスチャのIDの設定
//===================================================
void CMeshCylinder::SetTextureID(const char* pTexturePath)
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