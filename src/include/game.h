//===================================================
//
// ゲームシーン [game.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _GAME_H_
#define _GAME_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "scene.h"
#include "BlockManager.h"

//***************************************************
// 前方宣言
//***************************************************
class CCollisionSystem;		// 当たり判定の処理のクラス
class CPlanktonController;	// プランクトンを生成するクラス

//***************************************************
// ゲームシーンのクラスの定義
//***************************************************
class CGame : public CScene
{
public:
	CGame();
	~CGame();

	HRESULT Init	(void) override;
	void	Uninit	(void) override;
	void	Update	(void) override;
	void	Draw	(void) override;

	static inline CPlanktonController*	GetPlanktonController	(void) { return m_pPlanktonController.get(); }
	static inline CBlockManager*		GetBlockManager			(void) { return m_pBlockManager.get(); }
private:
	std::vector<std::unique_ptr<CCollisionSystem>>	m_vpCollisionSystem;	// 当たり判定の処理
	static std::unique_ptr<CPlanktonController>		m_pPlanktonController;	// プランクトンの生成するクラス
	static std::unique_ptr<CBlockManager>			m_pBlockManager;		// ブロックマネージャーへのポインタ
};
#endif