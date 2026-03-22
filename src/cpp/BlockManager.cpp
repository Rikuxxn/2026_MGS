//=============================================================================
//
// ブロックマネージャー処理 [BlockManager.cpp]
// Author : RIKU TANEKAWA
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "BlockManager.h"
#include "json.hpp"
#include "Manager.h"
#include "RigidBody.h"
#include "MathConst.h"
#include "renderer.h"

// JSONの使用
using json = nlohmann::json;

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
std::vector<CBlock*> CBlockManager::m_blocks = {};	// ブロックの情報
std::unordered_map<CBlock::TYPE, std::string> CBlockManager::s_FilePathMap; 


//=============================================================================
// コンストラクタ
//=============================================================================
CBlockManager::CBlockManager()
{
	// 値のクリア
}
//=============================================================================
// デストラクタ
//=============================================================================
CBlockManager::~CBlockManager()
{
	// なし
}
//=============================================================================
// ユニークポインタの生成
//=============================================================================
std::unique_ptr<CBlockManager> CBlockManager::Create(void)
{ 
	return std::make_unique<CBlockManager>();
}
//=============================================================================
// 生成処理
//=============================================================================
CBlock* CBlockManager::CreateBlock(CBlock::TYPE type, D3DXVECTOR3 pos, bool isDynamic)
{
	const char* path = CBlockManager::GetFilePathFromType(type);

	CBlock* newBlock = CBlock::Create(path, pos, Const::VEC3_NULL, Const::INIT_SCAL, type, isDynamic);

	if (newBlock)
	{
		m_blocks.push_back(newBlock);
	}

	return newBlock;
}
//=============================================================================
// 初期化処理
//=============================================================================
void CBlockManager::Init(void)
{
	// モデルリストの読み込み
	LoadConfig("data/ModelList.json");

	// 動的配列を空にする (サイズを0にする)
	m_blocks.clear();
}
//=============================================================================
// 終了処理
//=============================================================================
void CBlockManager::Uninit(void)
{
	// 動的配列を空にする (サイズを0にする)
	m_blocks.clear();
}
//=============================================================================
// 削除予約があるブロックの削除処理
//=============================================================================
void CBlockManager::CleanupDeadBlocks(void)
{
	for (int nCnt = (int)m_blocks.size() - 1; nCnt >= 0; nCnt--)
	{
		if (m_blocks[nCnt]->IsDead())
		{
			// ブロックの終了処理
			m_blocks[nCnt]->Uninit();
			m_blocks.erase(m_blocks.begin() + nCnt);
		}
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void CBlockManager::Update(void)
{

}
//=============================================================================
// タイプからXファイルパスを取得
//=============================================================================
const char* CBlockManager::GetFilePathFromType(CBlock::TYPE type)
{
	auto it = s_FilePathMap.find(type);
	return (it != s_FilePathMap.end()) ? it->second.c_str() : "";
}
//=============================================================================
// ブロック情報の保存処理
//=============================================================================
void CBlockManager::SaveToJson(const char* filename)
{
	// JSONオブジェクト
	json j;

	// 1つづつJSON化
	for (const auto& block : m_blocks)
	{
		json b;
		block->SaveToJson(b);

		// 追加
		j.push_back(b);
	}

	// 出力ファイルストリーム
	std::ofstream file(filename);

	if (file.is_open())
	{
		file << std::setw(4) << j;

		// ファイルを閉じる
		file.close();
	}
}
//=============================================================================
// ブロック情報の読み込み処理
//=============================================================================
void CBlockManager::LoadFromJson(const char* filename)
{
	std::ifstream file(filename);

	if (!file.is_open())
	{// 開けなかった
		return;
	}

	json j;
	file >> j;

	// ファイルを閉じる
	file.close();

	// 既存のブロックを消す
	for (auto block : m_blocks)
	{
		if (block != nullptr)
		{
			// ブロックの終了処理
			block->Uninit();
		}
	}

	// 動的配列を空にする (サイズを0にする)
	m_blocks.clear();
	
	// 新たに生成
	for (const auto& b : j)
	{
		CBlock::TYPE type = b["type"];
		D3DXVECTOR3 pos(b["pos"][0], b["pos"][1], b["pos"][2]);
		bool isDynamic = b["is_dynamic"];

		// ブロックの生成
		CBlock* block = CreateBlock(type, pos, isDynamic);

		if (!block)
		{
			continue;
		}

		block->LoadFromJson(b);
	}
}
//=============================================================================
// モデルリストの読み込み
//=============================================================================
void CBlockManager::LoadConfig(const std::string& filename)
{
	std::ifstream ifs(filename);

	if (!ifs)
	{// 開けなかった
		MessageBox(nullptr, "モデルリストのオープンに失敗 ", "警告！", MB_ICONWARNING);

		return;
	}

	json j;
	ifs >> j;

	// j は配列になってるのでループする
	for (auto& block : j)
	{
		int typeInt = block["type"];
		std::string filepath = block["modelpath"];

		s_FilePathMap[(CBlock::TYPE)typeInt] = filepath;
	}
}
//=============================================================================
// 全ブロックの取得
//=============================================================================
std::vector<CBlock*>& CBlockManager::GetAllBlocks(void)
{
	return m_blocks;
}