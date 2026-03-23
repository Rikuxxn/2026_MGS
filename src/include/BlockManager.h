//=============================================================================
//
// ブロックマネージャー処理 [bBockManager.h]
// Author : RIKU TANEKAWA
//
//=============================================================================
#ifndef _BLOCKMANAGER_H_
#define _BLOCKMANAGER_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "Block.h"
#include "cassert"

//*****************************************************************************
// ブロックマネージャークラス
//*****************************************************************************
class CBlockManager
{
public:
	CBlockManager();
	~CBlockManager();

    static std::unique_ptr<CBlockManager>Create(void);// ユニークポインタの生成
    static CBlock* CreateBlock(CBlock::TYPE type, D3DXVECTOR3 pos, bool isDynamic);
    void Init               (void);
    void Uninit             (void);                         // 終了処理
    void CleanupDeadBlocks  (void);                         // 削除予約があるブロックの削除
    void Update             (void);                         // 更新処理
    void SaveToJson         (const char* filename);         // ブロック情報の保存
    void LoadFromJson       (const char* filename);         // ブロック情報の読み込み
    void LoadConfig         (const std::string& filename);  // モデルリストの読み込み

    //*****************************************************************************
    // getter関数
    //*****************************************************************************
    static std::vector<CBlock*>& GetAllBlocks(void);

    // 特定の型をまとめて取得
    template<typename T>
    static std::vector<T*> GetBlocksOfType(void)
    {
        std::vector<T*> result;

        for (CBlock* block : m_blocks)
        {
            if (!block)
            {
                continue;
            }

            if (T* t = dynamic_cast<T*>(block))
            {
                result.push_back(t);
            }
        }

        return result;
    }

private:
    static const char* GetFilePathFromType(CBlock::TYPE type);
private:

    //*****************************************************************************
    // ブロック管理
    //*****************************************************************************
    static std::vector<CBlock*> m_blocks;               // ブロック情報

    //*****************************************************************************
    // ファイルパス管理
    //*****************************************************************************
    static std::unordered_map<CBlock::TYPE, std::string> s_FilePathMap;
};

#endif
