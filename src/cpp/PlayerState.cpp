//=============================================================================
//
// プレイヤーの状態処理 [PlayerState.cpp]
// Author : RIKU TANEKAWA
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "PlayerState.h"
#include "Player.h"
#include "RigidBody.h"
#include "particle.h"
#include "color_constants.h"

//=============================================================================
// 待機状態の開始処理
//=============================================================================
void CPlayerStandState::OnStart(CPlayer* /*pPlayer*/)
{

}
//=============================================================================
// 待機状態の更新処理
//=============================================================================
void CPlayerStandState::OnUpdate(CPlayer* pPlayer)
{
	// 入力を取得
	CPlayer::InputData input = pPlayer->GatherInput();

	// 移動フラグ更新
	pPlayer->UpdateMovementFlags(input.moveDir);

	// ジャンプ入力があればジャンプステートに移行
	if (input.isJump && pPlayer->GetOnGround() && !pPlayer->GetIsJumping())
	{
		m_pMachine->ChangeState<CPlayerJumpState>();
		return;
	}

	// 前方移動入力があれば通常移動ステートに移行
	if (pPlayer->GetIsMoving())
	{
		m_pMachine->ChangeState<CPlayerMoveState>();
	}

	// 減速処理
	pPlayer->ApplyDeceleration();

	// 移動量の取得
	D3DXVECTOR3 move = pPlayer->GetMove();

	// 移動量を設定
	pPlayer->SetPhysicsMove(move);
}

//=============================================================================
// 移動状態の開始処理
//=============================================================================
void CPlayerMoveState::OnStart(CPlayer* /*pPlayer*/)
{

}
//=============================================================================
// 移動状態の更新処理
//=============================================================================
void CPlayerMoveState::OnUpdate(CPlayer* pPlayer)
{
	// 入力取得
	CPlayer::InputData input = pPlayer->GatherInput();

	// 移動フラグ更新
	pPlayer->UpdateMovementFlags(input.moveDir);

	// プランクトンの取得
	auto plankton = pPlayer->GetPlankton();

	// スピードレート
	float speedRate = MOVE_SPEED_RATE;

	if (!plankton.empty())
	{
		// プランクトンの取得量
		int nPlanktonNum = plankton.size();

		// プランクトン取得量に応じたスピード
		speedRate = 1.0f - nPlanktonNum * DEC_SPEED_RATE;	// 8%ずつ低下
		speedRate = std::max(speedRate, MAX_DEC_RATE);		// 最大50%
	}

	// 目標速度計算
	float moveSpeed = CPlayer::SPEED * speedRate;
	D3DXVECTOR3 targetMove = input.moveDir;

	if (targetMove.x != 0.0f || targetMove.z != 0.0f)
	{
		D3DXVec3Normalize(&targetMove, &targetMove);
		targetMove *= moveSpeed;
	}
	else
	{
		targetMove = Const::VEC3_NULL;
	}

	// 現在速度との補間
	D3DXVECTOR3 currentMove = pPlayer->GetMove();

	currentMove.x += (targetMove.x - currentMove.x) * CPlayer::ACCEL_RATE;
	currentMove.z += (targetMove.z - currentMove.z) * CPlayer::ACCEL_RATE;

	// 補間後の速度をプレイヤーにセット
	pPlayer->SetPhysicsMove(currentMove);

	//CParticle::Info particleInfo;

	//// オフセット
	//D3DXVECTOR3 localOffset(0.0f, 0.0f, 100.0f);
	//D3DXVECTOR3 worldOffset;

	//// ブロックのワールドマトリックスを取得
	//D3DXMATRIX worldMtx = pPlayer->GetWorldMatrix();

	//D3DXVec3TransformCoord(&worldOffset, &localOffset, &worldMtx);

	//// パーティクル設定
	//particleInfo.pos = worldOffset;
	//particleInfo.col = Color::LIGHTBLUE;
	//particleInfo.fAngleXMax = 90;
	//particleInfo.fAngleXMin = -90;
	//particleInfo.fAngleYMax = 60;
	//particleInfo.fAngleYMin = -60;
	//particleInfo.moveMax = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	//particleInfo.moveMin = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	//particleInfo.nNum = 5;
	//particleInfo.nTime = 5;
	//particleInfo.size = { 50.0f,50.0f };
	//particleInfo.texturePath = "smoke.jpg";
	//particleInfo.effectInfo.nLife = 20;

	//CParticle::Create(particleInfo);

	// ジャンプ入力があればジャンプステートに切替
	if (input.isJump && pPlayer->GetOnGround() && !pPlayer->GetIsJumping())
	{
		// ジャンプ状態
		m_pMachine->ChangeState<CPlayerJumpState>();
	}

	// 移動していなければ待機ステートに戻す
	if (!pPlayer->GetIsMoving())
	{
		// 待機状態
		m_pMachine->ChangeState<CPlayerStandState>();
	}
}

//=============================================================================
// ジャンプ状態の開始処理
//=============================================================================
void CPlayerJumpState::OnStart(CPlayer* pPlayer)
{
	// ジャンプ中にする
	pPlayer->SetIsJumping(true);

	// 通常移動量の取得
	D3DXVECTOR3 normalMove = pPlayer->GetMove();

	// 移動量の設定
	pPlayer->SetPhysicsMove(normalMove);

	// リジッドボディの取得
	auto pRigidBody = pPlayer->GetRigidBody();

	if (pRigidBody)
	{
		D3DXVECTOR3 vel = pRigidBody->GetVelocity();
		vel.x = normalMove.x; // X方向速度
		vel.y = CPlayer::MAX_JUMP_POWER;
		vel.z = normalMove.z; // Z方向速度
		pRigidBody->SetVelocity(vel);  // RigidBody にセット
	}

	m_counter = 0;
}
//=============================================================================
// ジャンプ状態の更新処理
//=============================================================================
void CPlayerJumpState::OnUpdate(CPlayer* pPlayer)
{
	// 入力取得
	CPlayer::InputData input = pPlayer->GatherInput();

	// 移動フラグ更新
	pPlayer->UpdateMovementFlags(input.moveDir);

	// 目標速度計算
	D3DXVECTOR3 targetMove = input.moveDir;

	if (targetMove.x != 0.0f || targetMove.z != 0.0f)
	{
		D3DXVec3Normalize(&targetMove, &targetMove);
		targetMove *= CPlayer::SPEED;
	}
	else
	{
		targetMove = Const::VEC3_NULL;
	}

	// 現在速度との補間
	D3DXVECTOR3 currentMove = pPlayer->GetMove();

	currentMove.x += (targetMove.x - currentMove.x) * CPlayer::ACCEL_RATE;
	currentMove.z += (targetMove.z - currentMove.z) * CPlayer::ACCEL_RATE;

	// 補間後の速度をプレイヤーにセット
	pPlayer->SetPhysicsMove(currentMove);

	m_counter++;

	if (m_counter >= JUMP_RESET_TIME)
	{// 少し遅延してfalseにする
		pPlayer->SetIsJumping(false);
	}

	// 着地したら
	if (pPlayer->GetOnGround() && !pPlayer->GetIsJumping())
	{
		// 待機状態
		m_pMachine->ChangeState<CPlayerStandState>();
	}
}
