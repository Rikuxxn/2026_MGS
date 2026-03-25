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

//***************************************************
// 前方宣言
//***************************************************
class CCollisionSystem;		// 当たり判定の処理のクラス
class CPlanktonController;	// プランクトンを生成するクラス
class CWhaleController;		// クジラを生成クラス

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

	static inline CPlanktonController* GetPlanktonController(void) { return m_pPlanktonController.get(); }
	static inline CWhaleController* GetWhaleController		(void) { return m_pWhaleController.get(); }
private:
	std::vector<std::unique_ptr<CCollisionSystem>>	m_vpCollisionSystem;	// 当たり判定の処理
	static std::unique_ptr<CPlanktonController>		m_pPlanktonController;	// プランクトンを生成するクラス
	static std::unique_ptr<CWhaleController>		m_pWhaleController;		// クジラを生成するクラス
};
#endif