//===========================================
//
// テクスチャマネージャー [texture_manager.h]
// Author:YUTO YOSHIDA
//
//===========================================

//*******************************************
// インクルードファイル
//*******************************************
#include "texture_manager.h"
#include "manager.h"
#include "renderer.h"
#include "stdio.h"
#include "fstream"
#include "json.hpp"

//*******************************************
// 定数宣言
//*******************************************
namespace  TEXTURE_INFO
{
	const char* JSON_FILE = "data/SYSTEM/texture_list.json";	// 読み込み用ファイル
	const char* KEY_INFO = "TEXTURE_INFO";						// キーConfig
	const char* KEY_FILE_PATH = "file_path";					// ファイルパスのキー
}

//*******************************************
// 静的メンバ変数宣言
//*******************************************
int CTextureManager::m_nNumAll = 0;	// テクスチャの総数

//===========================================
// コンストラクタ
//===========================================
CTextureManager::CTextureManager()
{
}

//===========================================
// デストラクタ
//===========================================
CTextureManager::~CTextureManager()
{
}

//===========================================
// テクスチャの番号の取得
//===========================================
int CTextureManager::Register(const char* pFilename)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// インデックス
	int nIdx = INVALID_ID;

	// テクスチャの数
	int nNumTexture = static_cast<int>(m_apTextureInfo.size());

	// nullだったら
	if (pFilename == nullptr)
	{
		return INVALID_ID;
	}

	// 省略用ファイルパス
	std::string filePath = pFilename;

	// ファイルパスが省略されていたら
	if (filePath.find(TEXTURE_ROOT_PATH) == std::string::npos)
	{
		// 文字列を連結
		filePath = TEXTURE_ROOT_PATH + filePath;
	}

	// テクスチャ分回す
	for (int nCnt = 0; nCnt < nNumTexture; nCnt++)
	{
		// 文字列が一致していたら
		if (m_apTextureInfo[nCnt].filepath == filePath)
		{
			return nCnt;
		}
	}

	// 指定されたテクスチャが登録されていなかったら
	if (nIdx == INVALID_ID)
	{
		// テクスチャの情報
		TextureInfo info = {};

		// テクスチャのパスを格納
		const char* pTexturePath = filePath.c_str();

		// テクスチャの読み込み
		if (FAILED(D3DXCreateTextureFromFile(pDevice,
			pTexturePath,
			&info.pTexture)))
		{
			return INVALID_ID;
		}

		// ファイルのパスの取得
		info.filepath = pTexturePath;

		// 要素の設定
		m_apTextureInfo.push_back(info);

		// インデックス番号を返す
		nIdx = m_nNumAll;

		// テクスチャの総数
		m_nNumAll++;
	}

	return nIdx;
}

//=============================================================================
// キューブマップ用テクスチャの指定処理
//=============================================================================
int CTextureManager::RegisterCube(
	const char* px,
	const char* nx,
	const char* py,
	const char* ny,
	const char* pz,
	const char* nz)
{
	// レンダラーの取得
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 device = pRenderer->GetDevice();

	// キューブマップテクスチャ
	LPDIRECT3DCUBETEXTURE9 cube = nullptr;

	// まず +X 画像からサイズ取得
	D3DXIMAGE_INFO info;
	if (FAILED(D3DXGetImageInfoFromFile(px, &info)))
	{
		return INVALID_ID;
	}

	// CubeTexture生成
	if (FAILED(D3DXCreateCubeTexture(
		device,
		info.Width,
		1,
		0,
		info.Format,
		D3DPOOL_MANAGED,
		&cube)))
	{
		return INVALID_ID;
	}

	const char* files[CUBEMAP_TEX_NUM] = { px, nx, py, ny, pz, nz };

	for (int nCnt = 0; nCnt < CUBEMAP_TEX_NUM; nCnt++)
	{
		LPDIRECT3DSURFACE9 face;
		cube->GetCubeMapSurface((D3DCUBEMAP_FACES)nCnt, 0, &face);

		if (FAILED(D3DXLoadSurfaceFromFile(
			face,
			nullptr,
			nullptr,
			files[nCnt],
			nullptr,
			D3DX_DEFAULT,
			0,
			nullptr)))
		{
			// 破棄
			face->Release();
			cube->Release();

			return INVALID_ID;
		}

		// 破棄
		face->Release();
	}

	// 登録
	CubeTextureInfo texInfo = {};
	texInfo.pCubeTexture = cube;

	// 名前は適当
	texInfo.name = px;

	m_apCubeTextureInfo.push_back(texInfo);

	return (int)m_apCubeTextureInfo.size() - 1;
}

//===========================================
// テクスチャの取得処理
//===========================================
LPDIRECT3DTEXTURE9 CTextureManager::GetAdress(const int nIdx)
{
	// テクスチャの数
	int nNumTexture = static_cast<int>(m_apTextureInfo.size());

	// 範囲外だったら
	if (nIdx < 0 || nIdx >= nNumTexture)
	{
		return nullptr;
	}

	return m_apTextureInfo[nIdx].pTexture;
}

//=============================================================================
// キューブマップ用テクスチャのアドレス取得
//=============================================================================
LPDIRECT3DCUBETEXTURE9 CTextureManager::GetCubeAddress(int nIdx)
{
	// テクスチャの数
	int nNumTexture = static_cast<int>(m_apCubeTextureInfo.size());

	// 範囲外はnullptr
	if (nIdx < 0 || nIdx >= nNumTexture)
	{
		return nullptr;
	}

	return m_apCubeTextureInfo[nIdx].pCubeTexture;
}

//===========================================
// すべてのテクスチャの読み込み処理
//===========================================
HRESULT CTextureManager::Load(void)
{
	using json = nlohmann::json; // jsonを使用

	// ファイルを開く
	std::ifstream file(TEXTURE_INFO::JSON_FILE);

	json config;

	if (file.is_open())
	{
		file >> config;
		file.clear();
		file.close();
	}
	else
	{
		MessageBox(NULL, "ファイルが開けません", TEXTURE_INFO::JSON_FILE, MB_OK | MB_ICONWARNING);
		return E_FAIL;
	}

	// テクスチャ分回す
	for (auto& info : config[TEXTURE_INFO::KEY_INFO])
	{
		std::string filePath = info[TEXTURE_INFO::KEY_FILE_PATH];

		// モデルの登録
		Register(filePath.c_str());
	}

	return S_OK;
}

//===========================================
// すべてのテクスチャの解放処理
//===========================================
void CTextureManager::UnLoad(void)
{
	// リストのセーブ
	SaveList();

	// すべてのテクスチャ分回す
	for (auto& itr : m_apTextureInfo)
	{
		// テクスチャの破棄
		if (itr.pTexture != nullptr)
		{
			itr.pTexture->Release();
			itr.pTexture = nullptr;
		}
	}

	// 要素のクリア
	m_apTextureInfo.clear();
}

//===========================================
// リストにセーブ
//===========================================
void CTextureManager::SaveList(void)
{
	using json = nlohmann::json; // jsonを使用

	json modelInfo;

	// 要素を調べる
	for (auto& list : m_apTextureInfo)
	{
		json config =
		{
			{TEXTURE_INFO::KEY_FILE_PATH,list.filepath}
		};

		modelInfo[TEXTURE_INFO::KEY_INFO].push_back(config);
	}

	// ファイルを開く
	std::ofstream file(TEXTURE_INFO::JSON_FILE);

	// ファイルが開けたら
	if (file.is_open())
	{
		file << modelInfo.dump(4);
		file.clear();
		file.close();
	}
	else
	{
		MessageBox(NULL, "ファイルが開けません", TEXTURE_INFO::JSON_FILE, MB_OK | MB_ICONWARNING);
	}
}
