//===================================================
//
// ポーズメニュー毎の処理 [pause_menu_command.cpp]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "pause_menu_command.h"
#include "manager.h"
#include "game.h"
#include "title.h"
#include "pause_controller.h"

//===================================================
// コンストラクタ
//===================================================
CPauseMenuContinue::CPauseMenuContinue()
{
}

//===================================================
// デストラクタ
//===================================================
CPauseMenuContinue::~CPauseMenuContinue()
{
}

//===================================================
// 処理
//===================================================
void CPauseMenuContinue::Execute(void)
{
	CPauseController *pPauseController = CGame::GetPauseController();

	if(pPauseController != nullptr)
	{
		pPauseController->SetPause(false);
	}
}

//===================================================
// コンストラクタ
//===================================================
CPauseMenuRetry::CPauseMenuRetry()
{
}

//===================================================
// デストラクタ
//===================================================
CPauseMenuRetry::~CPauseMenuRetry()
{
}

//===================================================
// 処理
//===================================================
void CPauseMenuRetry::Execute(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// ゲームシーンへの移行
	pManager->BeginFade(std::make_unique<CGame>());
}

//===================================================
// コンストラクタ
//===================================================
CPauseMenuQuit::CPauseMenuQuit()
{
}

//===================================================
// デストラクタ
//===================================================
CPauseMenuQuit::~CPauseMenuQuit()
{
}

//===================================================
// 処理
//===================================================
void CPauseMenuQuit::Execute(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// タイトルシーンへの移行
	pManager->BeginFade(std::make_unique<CTitle>());
}