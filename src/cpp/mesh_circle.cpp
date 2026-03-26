//===================================================
//
// メッシュサークル [mesh_cylinder.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "mesh_circle.h"
#include "manager.h"
#include "renderer.h"
#include "texture_manager.h"

//===================================================
// コンストラクタ
//===================================================
CMeshCircle::CMeshCircle() : 
	CObject(PRIORITY_MESH),
	m_pVtxBuffer(nullptr),
	m_pIdxBuffer(nullptr),
	m_mtxWorld(Const::MTX_IDENTITY),
	m_inColor(Const::WHITE),
	m_outColor(Const::WHITE),
	m_pos(Const::VEC3_NULL),
	m_rot(Const::VEC3_NULL),
	m_info(),
	m_fDecAlpha(0.0f),
	m_nSegmentU(0),
	m_nSegmentV(0),
	m_nNumVertex(0),
	m_nNumPolygon(0),
	m_nNumIndex(0),
	m_nTextureID(CTextureManager::INVALID_ID)
{
}

//===================================================
// デストラクタ
//===================================================
CMeshCircle::~CMeshCircle()
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
CMeshCircle* CMeshCircle::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pTexturePath, const int nSegment, const Info& info)
{
	// 自分の生成
	CMeshCircle* pInstance = new CMeshCircle;

	// テクスチャのIDの設定
	pInstance->SetTextureID(pTexturePath);

	pInstance->m_pos = pos;
	pInstance->m_rot = rot;
	pInstance->m_nSegmentU = nSegment;
	pInstance->m_nSegmentV = 1;
	pInstance->m_info = info;
	pInstance->m_outColor = info.col;
	pInstance->m_fDecAlpha = info.col.a / info.nLife;

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
HRESULT CMeshCircle::Init(void)
{
	// 頂点数の設定
	m_nNumVertex = (m_nSegmentU + 1) * (m_nSegmentV + 1);

	// ポリゴン数の設定
	m_nNumPolygon = ((m_nSegmentU * m_nSegmentV) * 2) + (4 * (m_nSegmentV - 1));

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
	float fTexPosX = 1.0f / m_nSegmentU;

	// 計算用の位置
	D3DXVECTOR3 posWk;

	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 内側の色の設定
	D3DXCOLOR InColor = m_info.col;
	InColor.a *= 0.5f;

	// 内側の色の設定
	m_inColor = InColor;

	// 横の分割数分回す
	for (int nCntH = 0; nCntH <= m_nSegmentU; nCntH++)
	{
		// 一周の割合をもとめる
		float fAngle = (D3DX_PI * 2.0f) / m_nSegmentU * nCntH;

		// 位置の設定
		posWk.x = sinf(fAngle) * m_info.fOutRadius;
		posWk.y = m_info.fOutHeight;
		posWk.z = cosf(fAngle) * m_info.fOutRadius;

		// 位置の設定
		pVtx[nCntVtx].pos = posWk;

		// 法線の設定
		pVtx[nCntVtx].nor = Const::VEC_U;

		// 内側の色の設定
		pVtx[nCntVtx].col = m_outColor;

		// テクスチャ座標の設定
		pVtx[nCntVtx].tex = D3DXVECTOR2(fTexPosX * nCntH, 1.0f);

		nCntVtx++;
	}

	// 横の分割数分回す
	for (int nCntH = 0; nCntH <= m_nSegmentU; nCntH++)
	{
		// 一周の割合をもとめる
		float fAngle = (D3DX_PI * 2.0f) / m_nSegmentU * nCntH;

		// 位置の設定
		posWk.x = sinf(fAngle) * m_info.fInRadius;
		posWk.y = m_info.fInHeight;
		posWk.z = cosf(fAngle) * m_info.fInRadius;

		// 位置の設定
		pVtx[nCntVtx].pos = posWk;

		// 法線の設定
		pVtx[nCntVtx].nor = Const::VEC_U;

		// 内側の色の設定
		pVtx[nCntVtx].col = InColor;

		// テクスチャ座標の設定
		pVtx[nCntVtx].tex = D3DXVECTOR2(fTexPosX * nCntH, 1.0f);

		nCntVtx++;
	}

	// 頂点バッファをアンロック
	m_pVtxBuffer->Unlock();

	// インデックスバッファへのポインタ
	WORD* pIdx;

	// インデックスバッファのロック
	m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	int IndxNum = m_nSegmentU + 1;//X

	int IdxCnt = 0;//配列

	int Num = 0;//

	//インデックスの設定
	for (int IndxCount1 = 0; IndxCount1 < m_nSegmentV; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= m_nSegmentU; IndxCount2++, IndxNum++, Num++)
		{
			// インデックスバッファの設定
			pIdx[IdxCnt] = static_cast<WORD>(IndxNum);
			pIdx[IdxCnt + 1] = static_cast<WORD>(Num);
			IdxCnt += 2;
		}

		// NOTE:最後の行じゃなかったら
		if (IndxCount1 < m_nSegmentV - 1)
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
void CMeshCircle::Uninit(void)
{
	// 終了処理
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CMeshCircle::Update(void)
{
	VERTEX_3D* pVtx = nullptr;

	// 頂点のカウンター
	int nCntVtx = 0;

	// 頂点バッファをロック
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// 計算用の位置
	D3DXVECTOR3 posWk = Const::VEC3_NULL;

	// 横の分割数分回す
	for (int nCntH = 0; nCntH <= m_nSegmentU; nCntH++)
	{
		// 一周の割合をもとめる
		float fAngle = (D3DX_PI * 2.0f) / m_nSegmentU * nCntH;

		// 位置の設定
		posWk.x = sinf(fAngle) * m_info.fOutRadius;
		posWk.y = m_info.fOutHeight;
		posWk.z = cosf(fAngle) * m_info.fOutRadius;

		// 位置の設定
		pVtx[nCntVtx].pos = posWk;

		// 内側の色の設定
		pVtx[nCntVtx].col = m_outColor;

		nCntVtx++;
	}

	// 横の分割数分回す
	for (int nCntH = 0; nCntH <= m_nSegmentU; nCntH++)
	{
		// 一周の割合をもとめる
		float fAngle = (D3DX_PI * 2.0f) / m_nSegmentU * nCntH;

		// 位置の設定
		posWk.x = sinf(fAngle) * m_info.fInRadius;
		posWk.y = m_info.fInHeight;
		posWk.z = cosf(fAngle) * m_info.fInRadius;

		// 位置の設定
		pVtx[nCntVtx].pos = posWk;

		// 内側の色の設定
		pVtx[nCntVtx].col = m_inColor;

		nCntVtx++;
	}

	// 頂点バッファをアンロック
	m_pVtxBuffer->Unlock();

	// 半径を拡大
	m_info.fInRadius += m_info.fSpeed;
	m_info.fOutRadius += m_info.fSpeed;

	// アルファ値を減少させる
	m_inColor.a -= m_fDecAlpha;
	m_outColor.a -= m_fDecAlpha;

	// 寿命が尽きたら
	if (m_info.nLife <= 0)
	{
		Uninit();
		return;
	}

	// 寿命を減らす
	m_info.nLife--;
}

//===================================================
// 描画処理
//===================================================
void CMeshCircle::Draw(void)
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

	// ゼットテスト
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// aブレンディング
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// テクスチャの設定
	pDevice->SetTexture(0, pTextureManager->GetAdress(m_nTextureID));

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);

	// ゼットテスト
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// aブレンディングをもとに戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//===================================================
// テクスチャのIDの設定処理
//===================================================
void CMeshCircle::SetTextureID(const char* pTexturePath)
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