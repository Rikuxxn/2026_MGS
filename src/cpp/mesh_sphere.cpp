//===================================================
//
// メッシュの球体 [mesh_sphere.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "mesh_sphere.h"
#include "manager.h"
#include "renderer.h"
#include "utility_math.h"
#include "texture_manager.h"


#include "effect.h"
#include "color_constants.h"
//===================================================
// コンストラクタ
//===================================================
CMeshSphere::CMeshSphere() : 
	CObject(PRIORITY_MESH),
	m_pVtxBuffer(nullptr),
	m_pIdxBuffer(nullptr),
	m_col(Const::WHITE),
	m_mtxWorld(Const::MTX_IDENTITY),
	m_pos(Const::VEC3_NULL),
	m_rot(Const::VEC3_NULL),
	m_segment(),
	m_fRadius(0.0f),
	m_nNumVertex(0),
	m_nNumPolygon(0),
	m_nNumIndex(0),
	m_nBottomStartIndex(0),
	m_nSideStartIndex(0),
	m_bReverse(false),
	m_nTextureID(-1)
{
}

//===================================================
// デストラクタ
//===================================================
CMeshSphere::~CMeshSphere()
{
	// 頂点バッファへのポインタ
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
CMeshSphere* CMeshSphere::Create(const D3DXVECTOR3& pos, const D3DXCOLOR& col, const VECTOR2INT& segment, const float fRadius, const char* pTexturePath)
{
	// 自分自身の生成
	CMeshSphere* pInstance = new CMeshSphere;

	// テクスチャのIDの取得
	pInstance->SetTextureID(pTexturePath);

	pInstance->m_pos = pos;
	pInstance->m_col = col;
	pInstance->m_segment = segment;
	pInstance->m_fRadius = fRadius;

	// 初期化処理
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
HRESULT CMeshSphere::Init(void)
{
	// 蓋の部分の頂点の計算
	int nFanVtx = (m_segment.x + 1) + 1;
	int nFanPolygon = m_segment.x;
	int nFanIndex = nFanPolygon + 2;

	// 側面の部分の頂点
	int nSideVtx = (m_segment.x + 1) * (m_segment.y + 1);
	int nSidePolygon = ((m_segment.x * m_segment.y) * 2) + (4 * (m_segment.y - 1));
	int nSideIndex = nSidePolygon + 2;

	int nNumFanVtx = nFanVtx * 2;
	int nNumFanPolygon = nFanPolygon * 2;
	int nNumFanIndex = nFanIndex * 2;

	// 頂点の総数とインデックスの総数の計算
	int nNumVertex = nNumFanVtx + nSideVtx;
	int nNumIndex = nNumFanIndex + nSideIndex;
	int nNumPolygon = nNumFanPolygon + nSidePolygon;

	// ポリゴン数の設定
	m_nNumPolygon = nNumPolygon;
	m_nNumVertex = nNumVertex;
	m_nNumIndex = nNumIndex;

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
	float fTexPosX = 0.5f / m_segment.x;
	float fTexPosY = 0.5f / m_segment.y;

	// 計算用の位置
	D3DXVECTOR3 posWk;

	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 反転するなら裏返す
	float fHeight = m_bReverse ? -m_fRadius : m_fRadius;

	pVtx[nCntVtx].pos = D3DXVECTOR3(0.0f, fHeight, 0.0f);
	pVtx[nCntVtx].nor = math::GetVector(pVtx[nCntVtx].pos, m_pos);
	pVtx[nCntVtx].col = m_col;
	pVtx[nCntVtx].tex = D3DXVECTOR2(fTexPosX, fTexPosY);

	nCntVtx++;

	// 反転するなら裏返す
	float fRadius = m_bReverse ? -m_fRadius : m_fRadius;

	// 横の分割数分回す
	for (int nCntU = 0; nCntU <= m_segment.x; nCntU++)
	{
		// 半周の割合(緯度)
		float fAngleX = D3DX_PI / m_segment.y;

		// 一周の割合を求める
		float fAngleY = (D3DX_PI * 2.0f) / m_segment.x * nCntU;

		posWk.x = sinf(fAngleX) * sinf(fAngleY) * fRadius;
		posWk.y = cosf(fAngleX) * fHeight;
		posWk.z = sinf(fAngleX) * cosf(fAngleY) * fRadius;

		pVtx[nCntVtx].pos = posWk;
		pVtx[nCntVtx].nor = math::GetVector(pVtx[nCntVtx].pos, m_pos);
		pVtx[nCntVtx].col = m_col;
		pVtx[nCntVtx].tex = D3DXVECTOR2(fTexPosX * nCntU, fTexPosY);

		nCntVtx++;
	}

	// テクスチャの座標の割合
	fTexPosX = 1.0f / m_segment.x;
	fTexPosY = 1.0f / m_segment.y;

	// 開始インデックス
	m_nSideStartIndex = nCntVtx;

	int nOffSetIndex = 0; // オフセットのインデックス

	// オフセットのインデックス
	FanIndex(&m_pIdxBuffer, m_segment.x, &nOffSetIndex);

	// 半周の割合(緯度)
	float fOffset = D3DX_PI / m_segment.y;

	// 縦の分割数
	for (int nCntV = 0; nCntV <= m_segment.y; nCntV++)
	{
		// 半周の割合(緯度)
		float fAngleX = ((D3DX_PI - fOffset * 2.0f) / m_segment.y * nCntV) + fOffset;

		// 横の分割数
		for (int nCntU = 0; nCntU <= m_segment.x; nCntU++)
		{
			// 一周の割合を求める
			float fAngleY = (-D3DX_PI * 2.0f) / m_segment.x * nCntU;

			posWk.x = sinf(fAngleX) * sinf(fAngleY) * fRadius;
			posWk.y = cosf(fAngleX) * fHeight;
			posWk.z = sinf(fAngleX) * cosf(fAngleY) * fRadius;

			pVtx[nCntVtx].pos = posWk;
			pVtx[nCntVtx].nor = math::GetVector(pVtx[nCntVtx].pos, m_pos);
			pVtx[nCntVtx].col = m_col;

			//テクスチャ座標の設定
			pVtx[nCntVtx].tex = D3DXVECTOR2((fTexPosX * nCntU), (fTexPosY * nCntV));

			nCntVtx++;
		}
	}

	int nBottomStartVertex = nCntVtx;

	// インデックスの設定
	nOffSetIndex = GridIndex(&m_pIdxBuffer, m_segment.x, m_segment.y, nOffSetIndex, m_nSideStartIndex);

	// 下の蓋の部分の開始頂点
	m_nBottomStartIndex = nOffSetIndex;

	// テクスチャの座標の割合
	fTexPosX = 0.5f / m_segment.x;
	fTexPosY = 0.5f / m_segment.y;

	// 横の分割数分回す
	for (int nCntU = 0; nCntU <= m_segment.x; nCntU++)
	{
		// 半周の割合(緯度)
		float fAngleX = D3DX_PI - (D3DX_PI / m_segment.y);

		// 一周の割合を求める
		float fAngleY = (D3DX_PI * 2.0f) / m_segment.x * nCntU;

		posWk.x = sinf(fAngleX) * sinf(fAngleY) * fRadius;
		posWk.y = cosf(fAngleX) * fHeight;
		posWk.z = sinf(fAngleX) * cosf(fAngleY) * fRadius;

		pVtx[nCntVtx].pos = posWk;
		pVtx[nCntVtx].nor = math::GetVector(pVtx[nCntVtx].pos, m_pos);
		pVtx[nCntVtx].col = m_col;
		pVtx[nCntVtx].tex = D3DXVECTOR2(fTexPosX * nCntU, fTexPosY);

		nCntVtx++;
	}

	pVtx[nCntVtx].pos = D3DXVECTOR3(0.0f, -fHeight, 0.0f);
	pVtx[nCntVtx].nor = math::GetVector(pVtx[nCntVtx].pos, m_pos);
	pVtx[nCntVtx].col = m_col;
	pVtx[nCntVtx].tex = D3DXVECTOR2(fTexPosX, fTexPosY);

	nCntVtx++;

	// インデックスの設定
	FanIndex(&m_pIdxBuffer, m_segment.x, nBottomStartVertex, nOffSetIndex);

	// 頂点バッファをアンロック
	m_pVtxBuffer->Unlock();

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CMeshSphere::Uninit(void)
{
	// 自分自身の破棄
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CMeshSphere::Update(void)
{

}

//===================================================
// 描画処理
//===================================================
void CMeshSphere::Draw(void)
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

	// ワールドマトリックスを設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuffer, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuffer);

	//テクスチャフォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, pTextureManager->GetAdress(m_nTextureID));

	// 蓋の部分の頂点の計算
	int nFanVtx = m_segment.x + 1 + 1;
	int nFanPolygon = m_segment.x;

	// 側面の部分の頂点
	int nSideVtx = (m_segment.x + 1) * (m_segment.y + 1);
	int nSidePolygon = ((m_segment.x * m_segment.y) * 2) + (4 * (m_segment.y - 1));

	// aブレンディング
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLEFAN,
		0,
		0,
		nFanVtx,
		0,
		nFanPolygon);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		nSideVtx,
		m_nSideStartIndex,
		nSidePolygon);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLEFAN,
		0,
		0,
		nFanVtx,
		m_nBottomStartIndex,
		nFanPolygon);

	// aブレンディングをもとに戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//===================================================
// テクスチャのIDの取得
//===================================================
void CMeshSphere::SetTextureID(const char* pTexturePath)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// テクスチャマネージャーの取得
	CTextureManager* pTextureManager = pManager->GetTextureManager();

	if (pTextureManager != nullptr)
	{
		// IDの登録
		m_nTextureID = pTextureManager->Register(pTexturePath);
	}
}

//===================================================
// 扇形のインデックス
//===================================================
void CMeshSphere::FanIndex(LPDIRECT3DINDEXBUFFER9* ppIndexBuffer, const int nSegmentU, int* pOffsetIndex)
{
	// インデックスバッファへのポインタ
	WORD* pIdx;

	// インデックスバッファのロック
	(*ppIndexBuffer)->Lock(0, 0, (void**)&pIdx, 0);

	int nIdx = 0;

	for (int nCnt = 0; nCnt <= nSegmentU; nCnt++)
	{
		pIdx[nIdx++] = static_cast<WORD>(nCnt);
	}

	pIdx[nIdx++] = 1;

	if (pOffsetIndex != nullptr)
	{
		// インデックスのオフセットを設定
		*pOffsetIndex = nIdx;
	}
	// インデックスバッファのアンロック
	(*ppIndexBuffer)->Unlock();
}

//===================================================
// グリッドのインデックス
//===================================================
int CMeshSphere::GridIndex(LPDIRECT3DINDEXBUFFER9* ppIndexBuffer, const int nSegmentU, const int nSegmentV, const int nOffsetIndex, const int nOffsetVertex)
{
	// インデックスバッファへのポインタ
	WORD* pIdx;

	// インデックスバッファのロック
	(*ppIndexBuffer)->Lock(0, 0, (void**)&pIdx, 0);

	int nIndxNum = nSegmentU + 1;//X

	int nIdxCnt = nOffsetIndex;//配列

	int nNum = 0;//

	// インデックスの設定
	for (int IndxCount1 = 0; IndxCount1 < nSegmentV; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= nSegmentU; IndxCount2++, nIndxNum++, nNum++)
		{
			// インデックスバッファの設定
			pIdx[nIdxCnt] = static_cast<WORD>(nIndxNum + nOffsetVertex);
			pIdx[nIdxCnt + 1] = static_cast<WORD>(nNum + nOffsetVertex);
			nIdxCnt += 2;
		}

		// NOTE:最後の行じゃなかったら
		if (IndxCount1 < nSegmentV - 1)
		{
			pIdx[nIdxCnt] = static_cast<WORD>(nNum - 1 + nOffsetVertex);
			pIdx[nIdxCnt + 1] = static_cast<WORD>(nIndxNum + nOffsetVertex);
			nIdxCnt += 2;
		}
	}

	// インデックスバッファのアンロック
	(*ppIndexBuffer)->Unlock();

	return nIdxCnt;
}
//===================================================
// 扇形のインデックス(オフセット指定)
//===================================================
void CMeshSphere::FanIndex(LPDIRECT3DINDEXBUFFER9* ppIndexBuffer, const int nSegmentU, int nOffsetVertex, int nOffsetIndex)
{
	// インデックスバッファへのポインタ
	WORD* pIdx;

	// インデックスバッファのロック
	(*ppIndexBuffer)->Lock(0, 0, (void**)&pIdx, 0);

	int nIdx = nOffsetIndex;

	for (int nCnt = nSegmentU + 1; nCnt >= 0; nCnt--)
	{
		pIdx[nIdx++] = static_cast<WORD>(nCnt + nOffsetVertex);
	}

	pIdx[nIdx++] = static_cast<WORD>(nOffsetVertex + nSegmentU);

	// インデックスバッファのアンロック
	(*ppIndexBuffer)->Unlock();
}