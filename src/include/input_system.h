//===================================================
//
// 入力処理をまとめる [input_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _INPUT_SYSTEM_H_
#define _INPUT_SYSTEM_H_

//***************************************************
// 入力処理をまとめた名前空間の定義
//***************************************************
namespace InputSystem
{
	bool PlayerMoveForward(void);
	bool PlayerMoveLeft(void);
	bool PlayerMoveBack(void);
	bool PlayerMoveRight(void);
	bool PlayerJump(void);
}
#endif