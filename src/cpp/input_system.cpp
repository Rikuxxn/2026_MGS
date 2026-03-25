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
