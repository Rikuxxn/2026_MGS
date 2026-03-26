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
class CWhaleController;		// クジラを生成クラス
class CScore;				// クジラの数を出すUIクラス
class CPauseController;		// ポーズの操作クラス

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
	static inline CWhaleController*		GetWhaleController		(void) { return m_pWhaleController.get(); }
	static inline CPauseController*		GetPauseController		(void) { return m_pPauseController.get(); }
	static inline CScore*				GetScore				(void) { return m_pScore; }
private:
	std::vector<std::unique_ptr<CCollisionSystem>>	m_vpCollisionSystem;	// 当たり判定の処理

	static std::unique_ptr<CPlanktonController>		m_pPlanktonController;	// プランクトンを生成するクラス
	static std::unique_ptr<CWhaleController>		m_pWhaleController;		// クジラを生成するクラス
	static std::unique_ptr<CBlockManager>			m_pBlockManager;		// ブロックマネージャーへのポインタ
	static std::unique_ptr<CPauseController>		m_pPauseController;		// ポーズのコントローラーへのポインタ
	static CScore*									m_pScore;				// スコアへのポインタ
};
#endif