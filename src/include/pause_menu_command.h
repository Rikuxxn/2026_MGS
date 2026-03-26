//===================================================
//
// ポーズメニュー毎の処理 [pause_menu_command.h]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _PAUSE_MESH_COMMAND_H_
#define _PAUSE_MESH_COMMAND_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "menu_command_base.h"

//***************************************************
// ポーズのゲームを続けるコマンドのクラスの定義
//***************************************************
class CPauseMenuContinue : public CMenuCommandBase
{
public:
	CPauseMenuContinue();
	~CPauseMenuContinue();

	void Execute(void) override;
private:
};

//***************************************************
// ポーズのゲームをリトライするコマンドのクラスの定義
//***************************************************
class CPauseMenuRetry : public CMenuCommandBase
{
public:
	CPauseMenuRetry();
	~CPauseMenuRetry();

	void Execute(void) override;
private:
};

//***************************************************
// ポーズのゲームをやめるコマンドのクラスの定義
//***************************************************
class CPauseMenuQuit : public CMenuCommandBase
{
public:
	CPauseMenuQuit();
	~CPauseMenuQuit();

	void Execute(void) override;
private:
};

#endif