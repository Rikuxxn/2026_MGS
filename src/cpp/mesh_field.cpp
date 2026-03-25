//===================================================
//
// メッシュフィールド [mesh_field.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "mesh_field.h"
#include "texture_manager.h"
#include "manager.h"
#include "renderer.h"
#include "mesh_field_collision.h"
#include "utility_math.h"
#include "shader_manager.h"
#include "shader.h"

//===================================================
// コンストラクタ
//===================================================
CMeshField::CMeshField() :
	CObject(PRIORITY_MESH),
	m_pCollision(nullptr),
	m_pVtxBuffer(nullptr),
	m_pIdxBuffer(nullptr),
	m_mtxWorld(Const::MTX_IDENTITY),
	m_col(Const::WHITE),
	m_pos(Const::VEC3_NULL),
	m_rot(Const::VEC3_NULL),
	m_size(Const::VEC2_NULL),
	m_segment(),
	m_nNumVertex(0),
	m_nNumPolygon(0),
	m_nNumIndex(0),
	m_nNoiseTextureID(CTextureManager::INVALID_ID),
	m_nSeaTextureID(CTextureManager::INVALID_ID),
	m_fTime(1.0f)
{
}

//===================================================
// デストラクタ
//===================================================
CMeshField::~CMeshField()
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

	// 破棄
	m_pCollision.reset();
}

//===================================================
// 生成処理
//===================================================
CMeshField* CMeshField::Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const D3DXCOLOR& col, const VECTOR2INT& segment, const char* pTexturePath)
{
	// 自分自身の生成
	CMeshField* pInstance = new CMeshField;

	// テクスチャのIDの設定
	pInstance->SetTextureID("data/TEXTURE/SEA/waterNoise.png","data/TEXTURE/SEA/water.png");

	pInstance->m_pos = pos;
	pInstance->m_size = size;
	pInstance->m_col = col;
	pInstance->m_segment = segment;
	
	// 初期化処理に失敗したら
	if (FAILED(pInstance->Init()))
	{
		pInstance->Uninit();
		pInstance = nullptr;
	}

	return pInstance;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CMeshField::Init(void)
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

	// テクスチャの座標の割合
	float fTexPosX = 1.0f / m_segment.x;
	float fTexPosY = 1.0f / m_segment.y;

	// 計算用の位置
	D3DXVECTOR3 posWk;

	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 縦の分割数
	for (int nCntV = 0; nCntV <= m_segment.y; nCntV++)
	{
		for (int nCntU = 0; nCntU <= m_segment.x; nCntU++)
		{
			// 位置の設定
			posWk.x = ((m_size.x / m_segment.x) * nCntU) - (m_size.x * 0.5f);
			posWk.y = m_pos.y;
			posWk.z = m_size.y - ((m_size.y / m_segment.y) * nCntV) - (m_size.y * 0.5f);

			pVtx[nCntVtx].pos = posWk;

			//法線ベクトルの設定
			pVtx[nCntVtx].nor = Const::VEC_U;

			//頂点カラーの設定
			pVtx[nCntVtx].col = m_col;

			//テクスチャ座標の設定
			pVtx[nCntVtx].tex = D3DXVECTOR2((fTexPosX * nCntU), (fTexPosY * nCntV));

			nCntVtx++;
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuffer->Unlock();

	// インデックスバッファへのポインタ
	WORD* pIdx;

	// インデックスバッファのロック
	m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	int IndxNum = m_segment.x + 1;	// X

	int IdxCnt = 0;					// 配列

	int nNum = 0;					//

	//インデックスの設定
	for (int IndxCount1 = 0; IndxCount1 < m_segment.y; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= m_segment.x; IndxCount2++, IndxNum++, nNum++)
		{
			// インデックスバッファの設定
			pIdx[IdxCnt] = static_cast<WORD>(IndxNum);
			pIdx[IdxCnt + 1] = static_cast<WORD>(nNum);
			IdxCnt += 2;
		}

		// NOTE:最後の行じゃなかったら
		if (IndxCount1 < m_segment.y - 1)
		{
			pIdx[IdxCnt] = static_cast<WORD>(nNum - 1);
			pIdx[IdxCnt + 1] = static_cast<WORD>(IndxNum);
			IdxCnt += 2;
		}
	}

	// インデックスバッファのアンロック
	m_pIdxBuffer->Unlock();

	// メッシュフィールドの当たり判定の生成
	m_pCollision = std::make_unique<CMeshFieldCollision>();

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CMeshField::Uninit(void)
{
	// 自分自身の破棄
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CMeshField::Update(void)
{
}

//===================================================
// 描画処理
//===================================================
void CMeshField::Draw(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// レンダラーの取得
	CRenderer* pRenderer = pManager->GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャマネージャーの取得
	CTextureManager* pTextureManager = pManager->GetTextureManager();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// シェーダーマネージャーの取得
	CShaderManager* pShaderManager = pManager->GetShaderManager();

	// 空のシェーダーの取得
	CShader* pShader = pShaderManager->GetAddrees(CShaderManager::TYPE_SEA);

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

	// マトリックスの設定
	pShader->SetFloat("g_fTime", m_fTime);
	pShader->SetMatrix("g_mtxWld", m_mtxWorld);
	pShader->SetMatrix("g_mtxView", mtxView);
	pShader->SetMatrix("g_mtxProj", mtxProj);

	// テクスチャの設定
	pShader->SetTexture("g_NoiseMap", pTextureManager->GetAdress(m_nNoiseTextureID));
	pShader->SetTexture("g_WaterMap", pTextureManager->GetAdress(m_nSeaTextureID));

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);

	if (pShader != nullptr)
	{
		pShader->EndPass();

		pShader->End();
	}
}

//===================================================
// 当たり判定の処理
//===================================================
bool CMeshField::Collision(const D3DXVECTOR3& targetPos, CollisionResult::MeshField& outResult)
{
	// 判定結果
	CollisionResult::MeshField result;

	// 判定結果
	bool bResult = false;

	if (m_pCollision != nullptr)
	{
		// 判定処理
		bResult = m_pCollision->Collision(
			targetPos,
			m_pos,
			m_segment,
			m_size,
			m_pVtxBuffer,
			m_pIdxBuffer,
			result);

		// 高さの取得
		outResult = result;
	}

	return bResult;
}

//===================================================
// テクスチャのIDの設定処理
//===================================================
void CMeshField::SetTextureID(const char* pNoiseTexturePath, const char* pSeaTexturePath)
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
	m_nNoiseTextureID = pTextureManager->Register(pNoiseTexturePath);
	m_nSeaTextureID = pTextureManager->Register(pSeaTexturePath);
}