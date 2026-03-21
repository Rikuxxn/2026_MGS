//===================================================
//
// キャラクターのモデルのクラス [model.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "model.h"
#include "manager.h"
#include "model_manager.h"
#include "texture_manager.h"
#include "renderer.h"

//===================================================
// コンストラクタ
//===================================================
CModel::CModel() :
	m_size(Const::VEC3_NULL),
	m_offPos(Const::VEC3_NULL),
	m_offRot(Const::VEC3_NULL),
	m_pos(Const::VEC3_NULL),
	m_rot(Const::VEC3_NULL),
	m_mtxWorld(Const::MTX_IDENTITY),
	m_pParent(nullptr),
	m_vnTextureID(),
	m_nModelID(CModelManager::INVALID_ID)
{
}

//===================================================
// デストラクタ
//===================================================
CModel::~CModel()
{
	// 要素があるなら
	if (!m_vnTextureID.empty())
	{
		m_vnTextureID.clear();
	}
}

//===================================================
// 生成処理
//===================================================
CModel* CModel::Create(const char* pModelFilePath)
{
	// 自分自身の生成
	CModel* pInstance = new CModel;

	pInstance->m_modelPath = pModelFilePath;

	// 初期化処理に失敗したら
	if (FAILED(pInstance->Init(pModelFilePath)))
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
HRESULT CModel::Init(const char* pModelFilePath)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// モデルマネージャークラスの取得
	CModelManager* pModelManager = pManager->GetModelManager();

	// テクスチャクラスの取得
	CTextureManager* pTextureManager = pManager->GetTextureManager();

	// モデルの番号の登録
	m_nModelID = pModelManager->Register(pModelFilePath);

	if (m_nModelID == -1)
	{
		return E_FAIL;
	}

	// モデルの情報の取得
	CModelManager::ModelInfo modelInfo = pModelManager->GetModelInfo(m_nModelID);

	// マテリアルの取得
	DWORD dwNumMat = modelInfo.dwNumMat;
	LPD3DXBUFFER pBuffMat = modelInfo.pBuffMat;

	D3DXMATERIAL* pMat;//マテリアルへのポインタ

	//マテリアルのデータへのポインタを取得
	pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();

	for (int nCnt = 0; nCnt < (int)dwNumMat; nCnt++)
	{
		// テクスチャのIDの取得
		int nTextureID = pTextureManager->Register(pMat[nCnt].pTextureFilename);

		// テクスチャの登録
		m_vnTextureID.push_back(nTextureID);
	}

	// 大きさの取得
	m_size = modelInfo.size;

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CModel::Uninit(void)
{
}

//===================================================
// 更新処理
//===================================================
void CModel::Update(void)
{
}

//===================================================
// 描画処理
//===================================================
void CModel::Draw(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// レンダラーの取得
	CRenderer* pRenderer = pManager->GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// モデルマネージャークラスの取得
	CModelManager* pModelManager = pManager->GetModelManager();

	// テクスチャクラスの取得
	CTextureManager* pTextureManager = pManager->GetTextureManager();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal, mtxParent;

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXVECTOR3 rot = m_offRot + m_rot;
	D3DXVECTOR3 pos = m_offPos + m_pos;

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pParent != nullptr)
	{ // 親が存在している
		// 親モデルのマトリックスの取得
		mtxParent = m_pParent->m_mtxWorld;
	}
	else
	{
		// ワールドマトリックスの取得
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	// 親のワールドマトリックスと掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	if (m_nModelID == -1)
	{
		//保存していたマテリアルを元に戻す
		pDevice->SetMaterial(&matDef);

		return;
	}

	// モデルの情報の取得
	CModelManager::ModelInfo modelInfo = pModelManager->GetModelInfo(m_nModelID);

	// マテリアルのデータへのポインタを取得
	pMat = (D3DXMATERIAL*)modelInfo.pBuffMat->GetBufferPointer();

	// マテリアルの総数の取得
	DWORD dwNumMat = modelInfo.dwNumMat;

	// メッシュの取得
	LPD3DXMESH pMesh = modelInfo.pMesh;

	// マテリアル分回す
	for (int nCntMat = 0; nCntMat < (int)dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// モデルのテクスチャが無いなら
		if (m_vnTextureID.empty())
		{
			continue;
		}

		// テクスチャの設定
		pDevice->SetTexture(0, pTextureManager->GetAdress(m_vnTextureID[nCntMat]));

		// モデル(パーツ)の描画
		pMesh->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを元に戻す
	pDevice->SetMaterial(&matDef);
}

//===================================================
// オフセットの設定処理
//===================================================
void CModel::SetOffset(const CModel* pOther)
{
	m_offPos = pOther->m_offPos;
	m_offRot = pOther->m_offRot;
}