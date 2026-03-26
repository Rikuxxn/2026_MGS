//===================================================
//
// ポーズの処理 [pause.h]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _PAUSE_CONTROLLER_H_
#define _PAUSE_CONTROLLER_H_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// 前方宣言
//***************************************************
class CMenuCommandBase;	// メニューのコマンド
class CObject2D;		// 2Dオブジェクトへのポインタ

//***************************************************
// ポーズの操作クラスの定義
//***************************************************
class CPauseController
{
public:

	// 種類
	enum MENU
	{
		MENU_CONTINUE = 0,
		MENU_RETRY,
		MENU_QUIT,
		MENU_MAX
	};

	CPauseController();
	~CPauseController();

	static std::unique_ptr<CPauseController> Create(void);

	HRESULT Init(void);
	void Update(void);
	void SetPause(const bool bPause) { m_bPause = bPause; }
	inline bool GetPause(void) const { return m_bPause; }
private:
	void UpdatePolygon(void);
private:
	std::array<std::unique_ptr<CMenuCommandBase>, MENU_MAX> m_apMenuCommand;	// メニューのコマンドクラスへのポインタ

	std::array<CObject2D*, MENU_MAX> m_apPolygon;	// ポーズのポリゴン
	MENU							 m_currentMenu;	// 現在選択中のメニュー
	float							 m_fSinCurve;	// サインカーブのカウンター
	bool							 m_bPause;		// ポーズが有効かどうか
};
#endif