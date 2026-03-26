//===================================================
//
// メニューのコマンドのインターフェース [MenuCommandBase.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MENU_COMMAND_BASE_H_
#define _MENU_COMMAND_BASE_H_

//***************************************************
// メニューのコマンドのインターフェースの定義
//***************************************************
class CMenuCommandBase
{
public:
	CMenuCommandBase() = default;
	virtual ~CMenuCommandBase() = default;

	// 実行される処理
 	virtual void Execute(void) = 0;
};
#endif