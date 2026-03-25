//=============================================================================
//
// プレイヤーの状態処理 [PlayerState.h]
// Author : RIKU TANEKAWA
//
//=============================================================================
#ifndef _PLAYERSTATE_H_// このマクロ定義がされていなかったら
#define _PLAYERSTATE_H_// 2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "State.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPlayer;
class CPlayerStandState;
class CPlayerMoveState;


//*****************************************************************************
// プレイヤーの待機状態
//*****************************************************************************
class CPlayerStandState :public StateBase<CPlayer>
{
public:

	void OnStart(CPlayer* /*pPlayer*/) override;
	void OnUpdate(CPlayer* pPlayer) override;
	void OnExit(CPlayer* /*pPlayer*/) override {}

private:

};

//*****************************************************************************
// プレイヤーの移動状態
//*****************************************************************************
class CPlayerMoveState :public StateBase<CPlayer>
{
public:

	void OnStart(CPlayer* /*pPlayer*/) override;
	void OnUpdate(CPlayer* pPlayer) override;
	void OnExit(CPlayer* /*pPlayer*/) override {}

private:
	static constexpr float	MOVE_SPEED_RATE = 1.0f;		// 移動時のスピードレート
	static constexpr float	DEC_SPEED_RATE	= 0.08f;	// スピードの減少率
	static constexpr float	MAX_DEC_RATE	= 0.5f;		// 減少する最大割合(%)

};

//*****************************************************************************
// プレイヤーのジャンプ状態
//*****************************************************************************
class CPlayerJumpState :public StateBase<CPlayer>
{
public:

	void OnStart(CPlayer* pPlayer) override;
	void OnUpdate(CPlayer* pPlayer) override;
	void OnExit(CPlayer* /*pPlayer*/) override {}

private:
	static constexpr int INTERVAL			= 20;	// 生成インターバル
	static constexpr int JUMP_RESET_TIME	= 15;	// ジャンプリセット時間

	int m_counter;
};

#endif