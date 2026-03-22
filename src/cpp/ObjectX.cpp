//=============================================================================
//
// Xファイル処理 [ObjectX.cpp]
// Author : RIKU TANEKAWA
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "ObjectX.h"
#include "Renderer.h"
#include "Manager.h"
#include "texture_manager.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CObjectX::CObjectX(const PRIORITY priority) : CObject(priority)
{
	// 値のクリア
	memset(m_szPath, 0, sizeof(m_szPath));				// ファイルパス
	m_nIdxTexture	= 0;								// テクスチャインデックス
	m_pos			= Const::VEC3_NULL;					// 位置
	m_rot			= Const::VEC3_NULL;					// 向き
	m_move			= Const::VEC3_NULL;					// 移動量
	m_size			= Const::INIT_SCAL;					// サイズ
	m_pMesh			= nullptr;							// メッシュへのポインタ
	m_pBuffMat		= nullptr;							// マテリアルへのポインタ
	m_dwNumMat		= NULL;								// マテリアル数
	m_mtxWorld		= {};								// ワールドマトリックス
	m_modelSize		= Const::VEC3_NULL;					// モデルの元サイズ（全体の幅・高さ・奥行き）
}
//=============================================================================
// デストラクタ
//=============================================================================
CObjectX::~CObjectX()
{
	// なし
}
//=============================================================================
// 生成処理
//=============================================================================
CObjectX* CObjectX::Create(const char* pFilepath, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{
	// オブジェクト生成
	CObjectX* pObjectX = new CObjectX;

	// nullptrだったら
	if (pObjectX == nullptr)
	{
		return nullptr;
	}

	pObjectX->m_pos = pos;
	pObjectX->m_rot = D3DXToRadian(rot);
	pObjectX->m_size = size;
	pObjectX->SetPath(pFilepath);	// パス保存

	// 初期化失敗時
	if (FAILED(pObjectX->Init()))
	{
		return nullptr;
	}

	return pObjectX;
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CObjectX::Init(void)
{
	// テクスチャパスのクリア
	m_texPaths.clear();

	// テクスチャの取得
	CTextureManager* pTexture = CManager::GetInstance()->GetTextureManager();

	// レンダラーの取得
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX(m_szPath,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh);

	int nNumVtx;		// 頂点数
	DWORD sizeFVF;		// 頂点フォーマットのサイズ
	BYTE* pVtxBuff;		// 頂点バッファへのポインタ

	// メッシュが読み込まれているか確認
	if (m_pMesh == nullptr)
	{
		MessageBox(nullptr, "Xファイルの読み込みに失敗しました（メッシュがNULLです）", "エラー", MB_OK | MB_ICONERROR);
		return 0;
	}

	// スムーズ化
	{
		ID3DXMesh* pTempMesh = nullptr;

		// 隣接情報を作成
		DWORD* pAdjacency = new DWORD[m_pMesh->GetNumFaces() * 3];
		m_pMesh->GenerateAdjacency(1e-6f, pAdjacency);

		// 法線生成（スムーズ化）
		HRESULT hr = D3DXComputeNormals(m_pMesh, pAdjacency);

		if (FAILED(hr))
		{
			// モデルに法線が無い場合、いったんクローンして法線を付与
			D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE];
			m_pMesh->GetDeclaration(decl);
			m_pMesh->CloneMesh(D3DXMESH_SYSTEMMEM, decl, pDevice, &pTempMesh);

			if (pTempMesh)
			{
				pTempMesh->GenerateAdjacency(1e-6f, pAdjacency);
				D3DXComputeNormals(pTempMesh, pAdjacency);
				m_pMesh->Release();
				m_pMesh = pTempMesh;
			}
		}

		delete[] pAdjacency;
	}

	// 頂点数の取得
	nNumVtx = m_pMesh->GetNumVertices();

	// 頂点数が0なら、モデルが空とみなす
	if (nNumVtx == 0)
	{
		MessageBox(nullptr, "Xファイルの読み込みに失敗しました（頂点数が0です）", "エラー", MB_OK | MB_ICONERROR);
		return 0;
	}
	else
	{// モデルあり

		// 頂点フォーマットの取得
		sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

		// 頂点バッファのロック
		m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		// AABB計算用の最小・最大値初期化
		D3DXVECTOR3 vMin(FLT_MAX, FLT_MAX, FLT_MAX);
		D3DXVECTOR3 vMax(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		// 最大値・最小値を求める
		for (int nCnt = 0; nCnt < nNumVtx; nCnt++)
		{
			D3DXVECTOR3* p = (D3DXVECTOR3*)(pVtxBuff + sizeFVF * nCnt);

			vMin.x = std::min(vMin.x, p->x);
			vMin.y = std::min(vMin.y, p->y);
			vMin.z = std::min(vMin.z, p->z);

			vMax.x = std::max(vMax.x, p->x);
			vMax.y = std::max(vMax.y, p->y);
			vMax.z = std::max(vMax.z, p->z);
		}

		// 頂点バッファのアンロック
		m_pMesh->UnlockVertexBuffer();

		// 元サイズ = 最大 - 最小
		m_modelSize = vMax - vMin;

		D3DXMATERIAL* pMat;// マテリアルへのポインタ

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		m_nIdxTexture = new int[m_dwNumMat];

		for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != nullptr)
			{// テクスチャファイルが存在する
				// テクスチャの登録
				m_nIdxTexture[nCntMat] = pTexture->Register(pMat[nCntMat].pTextureFilename);
			}
			else
			{// テクスチャが存在しない
				m_nIdxTexture[nCntMat] = -1;
			}
		}
	}

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CObjectX::Uninit(void)
{
	m_texPaths.clear();

	// テクスチャインデックスの破棄
	if (m_nIdxTexture != nullptr)
	{
		delete[] m_nIdxTexture;
		m_nIdxTexture = nullptr;
	}

	// メッシュの破棄
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}

	// マテリアルの破棄
	if (m_pBuffMat != nullptr)
	{
		m_pBuffMat->Release();
		m_pBuffMat = nullptr;
	}

	// オブジェクトの破棄(自分自身)
	this->Release();
}
//=============================================================================
// 更新処理
//=============================================================================
void CObjectX::Update(void)
{


}
//=============================================================================
// 描画処理
//=============================================================================
void CObjectX::Draw(void)
{
	if (!m_pBuffMat || !m_pMesh || m_dwNumMat == 0)
	{
		return;
	}

	// レンダラーの取得
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxSize;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// サイズを反映
	D3DXMatrixScaling(&mtxSize, m_size.x, m_size.y, m_size.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxSize);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスを設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 通常の描画
	DrawNormal(pDevice);
}
//=============================================================================
// 通常の描画処理
//=============================================================================
void CObjectX::DrawNormal(LPDIRECT3DDEVICE9 pDevice)
{
	// テクスチャの取得
	CTextureManager* pTexture = CManager::GetInstance()->GetTextureManager();

	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);// 法線正規化を有効にする

	D3DMATERIAL9 matDef;	// 現在のマテリアル保存用

	D3DXMATERIAL* pMat;		// マテリアルデータへのポインタ

	// ワールドマトリックスを設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルの取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		if (m_nIdxTexture[nCntMat] == -1)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, nullptr);
		}
		else
		{
			// テクスチャの設定
			pDevice->SetTexture(0, pTexture->GetAdress(m_nIdxTexture[nCntMat]));
		}

		// モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}

	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);// 法線正規化を無効にする

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}
//=============================================================================
// マテリアルの取得
//=============================================================================
D3DXCOLOR CObjectX::GetMaterialColor(void) const
{
	if (m_pBuffMat && m_dwNumMat > 0)
	{
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
		return pMat[0].MatD3D.Diffuse;  // 0番目のマテリアルの拡散光色を返す
	}

	return Const::WHITE; // デフォルト白
}