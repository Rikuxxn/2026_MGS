//===================================================
//
// シーンの基底クラス [scene.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _SCENE_H_
#define _SCENE_H_

//***************************************************
// シーンのクラスの定義
//***************************************************
class CScene
{
public:
	// モード
	enum MODE
	{
		MODE_BASE = 0,	// 種類
		MODE_TITLE,		// タイトル
		MODE_TUTORIAL,	// チュートリアル
		MODE_GAME,		// ゲーム
		MODE_RESULT,	// リザルト
		MODE_RANKING,	// ランキング
		MODE_MAX
	};

	explicit CScene(const MODE type);
	virtual ~CScene();

	virtual HRESULT Init	(void)	= 0;
	virtual void	Uninit	(void)	= 0;
	virtual void	Update	(void)	= 0;
	virtual void	Draw	(void)	= 0;

	// 現在のモードの取得
	inline MODE GetScene(void) const { return m_mode; }
private:
	const MODE m_mode;	// 現在のモード
};
#endif