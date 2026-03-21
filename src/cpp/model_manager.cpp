//==============================================
//
// モデルマネージャー [model_manager.h]
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// インクルードファイル
//**********************************************
#include "model_manager.h"
#include "manager.h"
#include "renderer.h"
#include "stdio.h"
#include "cassert"
#include "texture_manager.h"
#include "json.hpp"
#include "fstream"

//**********************************************
// 定数宣言
//**********************************************
namespace MODEL_INFO
{
	const char* JSON_FILE		= "data/SYSTEM/model_list.json";	// 使用するjsonFIle
	const char* KEY_INFO		= "MODEL_INFO";						// モデルの情報
	const char* KEY_FILE_PATH	= "file_path";						// ファイルパス
};

//**********************************************
// 静的メンバ変数宣言
//**********************************************
int CModelManager::m_nNumAll = 0;	// モデルの総数

//==============================================
// コンストラクタ
//==============================================
CModelManager::CModelManager()
{
	
}

//==============================================
// デストラクタ
//==============================================
CModelManager::~CModelManager()
{
}

//==============================================
// インデックスの登録
//==============================================
int CModelManager::Register(const char* pFilename)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	int nIdx = INVALID_ID;

	// モデルの数
	int nNumModel = static_cast<int>(m_aModelInfo.size());

	// nullだったら
	if (pFilename == nullptr)
	{
		return INVALID_ID;
	}

	// 省略用ファイルパス
	std::string filePath = pFilename;

	// ファイルパスが省略されていたら
	if (filePath.find(MODEL_ROOT_PATH) == std::string::npos)
	{
		// 文字列を連結
		filePath = MODEL_ROOT_PATH + filePath;
	}

	// モデルの数分回す
	for (int nCnt = 0; nCnt < nNumModel; nCnt++)
	{
		if (m_aModelInfo[nCnt].filepath.empty())
		{
			continue;
		}

		// 文字列が一致したら
		if (m_aModelInfo[nCnt].filepath == filePath)
		{
			return nCnt;
		}
	}

	if (nIdx == INVALID_ID)
	{
		// モデルの情報
		ModelInfo info = {};

		// ファイルパス
		const char* pFilePath = filePath.c_str();

		////Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(pFilePath,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&info.pBuffMat,
			NULL,
			&info.dwNumMat,
			&info.pMesh)))
		{
			// メッセージボックスの表示
			MessageBox(NULL, pFilePath, "モデルが読み込めませんでした", MB_OK | MB_ICONWARNING);

			return INVALID_ID;
		}

		// ファイルパスの設定
		info.filepath = pFilePath;

		// 大きさの設定
		SetSize(&info);

		// マテリアルの設定
		SetMaterial(&info);

		// 要素の設定
		m_aModelInfo.push_back(info);

		// インデックスの番号を返す
		nIdx = m_nNumAll;

		m_nNumAll++;
	}

	return nIdx;
}

//==============================================
// モデルの情報の取得
//==============================================
CModelManager::ModelInfo CModelManager::GetModelInfo(const int nIdx)
{
	// モデルの数
	int nNumModel = static_cast<int>(m_aModelInfo.size());

	if (m_aModelInfo.empty())
	{
		MessageBox(NULL, "モデルが読み込まれていません", "モデルを登録してください",MB_OK);
		return ModelInfo();
	}

	if (nIdx < 0 || nIdx >= nNumModel)
	{
		assert(false && "インデックスオーバーModelInfo");
		return ModelInfo();
	}

	return m_aModelInfo[nIdx];
}

//==============================================
// リストの書き出し処理
//==============================================
void CModelManager::SaveList(void)
{
	using json = nlohmann::json; // jsonを使用

	json modelInfo;

	// 要素を調べる
	for (auto& list : m_aModelInfo)
	{
		json config =
		{
			{MODEL_INFO::KEY_FILE_PATH,list.filepath}
		};

		modelInfo[MODEL_INFO::KEY_INFO].push_back(config);
	}

	// ファイルを開く
	std::ofstream file(MODEL_INFO::JSON_FILE);

	if (file.is_open())
	{
		file << modelInfo.dump(4);
		file.clear();
		file.close();
	}
	else
	{
		MessageBox(NULL, "ファイルが開けません", MODEL_INFO::JSON_FILE, MB_OK | MB_ICONWARNING);
	}
}

//==============================================
// 大きさの取得
//==============================================
void CModelManager::SetSize(ModelInfo *pModelInfo)
{
	// 頂点座標の取得
	int nNumVtx;	// 頂点数
	DWORD sizeFVF;  // 頂点フォーマットのサイズ
	BYTE* pVtxBuff; // 頂点バッファへのポインタ

	// 頂点数の取得
	nNumVtx = pModelInfo->pMesh->GetNumVertices();

	// 頂点フォーマットのサイズ取得
	sizeFVF = D3DXGetFVFVertexSize(pModelInfo->pMesh->GetFVF());

	// 頂点バッファのロック
	pModelInfo->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// 最大の頂点、最小の頂点
	D3DXVECTOR3 vtxMin = Const::VEC3_NULL, vtxMax = Const::VEC3_NULL;

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		// 頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		// 頂点座標を比較してブロックの最小値,最大値を取得
		if (vtx.x < vtxMin.x)
		{
			vtxMin.x = vtx.x;
		}
		if (vtx.y < vtxMin.y)
		{
			vtxMin.y = vtx.y;
		}
		if (vtx.z < vtxMin.z)
		{
			vtxMin.z = vtx.z;
		}
		if (vtx.x > vtxMax.x)
		{
			vtxMax.x = vtx.x;
		}
		if (vtx.y > vtxMax.y)
		{
			vtxMax.y = vtx.y;
		}
		if (vtx.z > vtxMax.z)
		{
			vtxMax.z = vtx.z;
		}

		// 頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += sizeFVF;
	}

	// サイズに代入
	pModelInfo->size.x = vtxMax.x - vtxMin.x;
	pModelInfo->size.y = vtxMax.y - vtxMin.y;
	pModelInfo->size.z = vtxMax.z - vtxMin.z;

	// 頂点バッファのアンロック
	pModelInfo->pMesh->UnlockVertexBuffer();
}

//==============================================
// マテリアルの設定処理
//==============================================
void CModelManager::SetMaterial(ModelInfo* pModelInfo)
{
	// マテリアルの取得
	D3DXMATERIAL* pMat;//マテリアルへのポインタ

	//マテリアルのデータへのポインタを取得
	pMat = (D3DXMATERIAL*)pModelInfo->pBuffMat->GetBufferPointer();

	// テクスチャマネージャーの取得
	CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

	for (int nCnt = 0; nCnt < (int)pModelInfo->dwNumMat; nCnt++)
	{
		int nTextureIdx = -1;

		// ファイル名を使用してテクスチャを読み込む
		nTextureIdx = pTextureManager->Register(pMat[nCnt].pTextureFilename);

		pModelInfo->nTextureIdx.push_back(nTextureIdx);
	}
}

//==============================================
// すべてのxFileのロード処理
//==============================================
HRESULT CModelManager::Load(void)
{
	using json = nlohmann::json; // jsonを使用

	// ファイルを開く
	std::ifstream file(MODEL_INFO::JSON_FILE);

	json config;

	if (file.is_open())
	{
		file >> config;
		file.clear();
		file.close();
	}
	else
	{
		MessageBox(NULL, "ファイルが開けません", MODEL_INFO::JSON_FILE, MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}

	for (auto& info : config[MODEL_INFO::KEY_INFO])
	{
		std::string filePath = info[MODEL_INFO::KEY_FILE_PATH];

		// モデルの登録
		Register(filePath.c_str());
	}

	return S_OK;
}

//==============================================
// すべてのxFileの破棄処理
//==============================================
void CModelManager::UnLoad(void)
{
	// リストのセーブ
	SaveList();

	// すべてのモデルのクリア
	for (auto itr = m_aModelInfo.begin(); itr != m_aModelInfo.end(); itr++)
	{
		// メッシュの破棄
		if (itr->pMesh != nullptr)
		{
			itr->pMesh->Release();
			itr->pMesh = nullptr;
		}

		// マテリアルの破棄
		if (itr->pBuffMat != nullptr)
		{
			itr->pBuffMat->Release();
			itr->pBuffMat = nullptr;
		}


		itr->filepath.clear();
		itr->nTextureIdx.clear();
	}

	// 要素のクリア
	m_aModelInfo.clear();
}
