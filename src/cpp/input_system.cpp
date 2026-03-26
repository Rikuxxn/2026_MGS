//===================================================
//
// 入力処理をまとめる [input_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "input_system.h"
#include "manager.h"
#include "input.h"
#include "..\include\input_system.h"

//===================================================
// プレイヤーの移動正面
//===================================================
bool InputSystem::PlayerMoveForward(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// キーボードの取得
 	CInputKeyboard *pKeyboard = pManager->GetInputKeyboard();

	return pKeyboard->GetPress(DIK_W);
}

//===================================================
// プレイヤーの移動左
//===================================================
bool InputSystem::PlayerMoveLeft(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// キーボードの取得
	CInputKeyboard* pKeyboard = pManager->GetInputKeyboard();

	return pKeyboard->GetPress(DIK_A);
}

//===================================================
// プレイヤーの移動後ろ
//===================================================
bool InputSystem::PlayerMoveBack(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// キーボードの取得
	CInputKeyboard* pKeyboard = pManager->GetInputKeyboard();

	return pKeyboard->GetPress(DIK_S);
}

//===================================================
// プレイヤーの移動右
//===================================================
bool InputSystem::PlayerMoveRight(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// キーボードの取得
	CInputKeyboard* pKeyboard = pManager->GetInputKeyboard();

	return pKeyboard->GetPress(DIK_D);
}

//===================================================
// プレイヤーのジャンプ
//===================================================
bool InputSystem::PlayerJump(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// キーボードの取得
	CInputKeyboard* pKeyboard = pManager->GetInputKeyboard();

	// ジョイパッドの取得
	CInputJoypad* pJoypad = pManager->GetInputJoypad();

	return (pKeyboard->GetPress(DIK_SPACE) || pJoypad->GetTrigger(pJoypad->JOYKEY_A));
}

//===================================================
// ポーズメニューの決定
//===================================================
bool InputSystem::PauseMenuDecision(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// キーボードの取得
	CInputKeyboard* pKeyboard = pManager->GetInputKeyboard();

	// ジョイパッドの取得
	CInputJoypad* pJoypad = pManager->GetInputJoypad();

	return (pKeyboard->GetTrigger(DIK_RETURN) || pJoypad->GetTrigger(pJoypad->JOYKEY_A));
}

//===================================================
// ポーズメニューの選択上
//===================================================
bool InputSystem::PauseMenuSelectUp(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// キーボードの取得
	CInputKeyboard* pKeyboard = pManager->GetInputKeyboard();

	// ジョイパッドの取得
	CInputJoypad* pJoypad = pManager->GetInputJoypad();
	
	const bool bKeyboard = pKeyboard->GetTrigger(DIK_UP) || pKeyboard->GetTrigger(DIK_W);
	const bool bJoyPad = pJoypad->GetTrigger(pJoypad->JOYKEY_UP);

	return bKeyboard || bJoyPad;
}

//===================================================
// ポーズメニュー選択下
//===================================================
bool InputSystem::PauseMenuSelectDown(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// キーボードの取得
	CInputKeyboard* pKeyboard = pManager->GetInputKeyboard();

	// ジョイパッドの取得
	CInputJoypad* pJoypad = pManager->GetInputJoypad();

	const bool bKeyboard = pKeyboard->GetTrigger(DIK_DOWN) || pKeyboard->GetTrigger(DIK_S);
	const bool bJoyPad = pJoypad->GetTrigger(pJoypad->JOYKEY_DOWN);

	return bKeyboard || bJoyPad;
}

//===================================================
// ポーズの状態の切り替え処理
//===================================================
bool InputSystem::EnablePause(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// キーボードの取得
	CInputKeyboard* pKeyboard = pManager->GetInputKeyboard();

	// ジョイパッドの取得
	CInputJoypad* pJoypad = pManager->GetInputJoypad();

	return (pKeyboard->GetTrigger(DIK_P) || pJoypad->GetTrigger(pJoypad->JOYKEY_START));
}

//===================================================
// カメラ追従の切り替えキー
//===================================================
bool InputSystem::DebugEnableFollowCamera(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// キーボードの取得
	CInputKeyboard* pKeyboard = pManager->GetInputKeyboard();

	return (pKeyboard->GetTrigger(DIK_F8));
}

//===================================================
// 編集モードのプランクトンの位置の決定
//===================================================
bool InputSystem::EditRegisterPlanktonPos(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// キーボードの取得
	CInputKeyboard* pKeyboard = pManager->GetInputKeyboard();

	return (pKeyboard->GetTrigger(DIK_RETURN));
}

//===================================================
// 編集モードのセーブ
//===================================================
bool InputSystem::EditSavePlanktonPos(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// キーボードの取得
	CInputKeyboard* pKeyboard = pManager->GetInputKeyboard();

	return (pKeyboard->GetTrigger(DIK_F4));
}
