//===================================================
//
// プレイヤーの処理 [player.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "player.h"
#include "character.h"
#include "camera.h"
#include "manager.h"
#include "input.h"
#include "input_system.h"
#include "utility_math.h"

//===================================================
// コンストラクタ
//===================================================
CPlayer::CPlayer() : 
	m_pCharacter(nullptr),
	m_rotDest(Const::VEC3_NULL),
	m_fSpeed(2.0f)
{
}

//===================================================
// デストラクタ
//===================================================
CPlayer::~CPlayer()
{
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Uninit();
		m_pCharacter.reset();
	}
}

//===================================================
// 生成処理
//===================================================
CPlayer* CPlayer::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pMotionFilePath)
{
	// 自分自身の生成
	CPlayer* pInstance = new CPlayer;

	// キャラクターの生成
	pInstance->m_pCharacter = CCharacter::Create(pos, rot, pMotionFilePath);

	// 初期化処理に失敗したら
	if (FAILED(pInstance->Init()))
	{
		// 終了処理
		pInstance->Uninit();
		pInstance = nullptr;
		return nullptr;
	}

	return pInstance;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CPlayer::Init(void)
{
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->SetParameter(0.25f, 0.1f);
	}
	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CPlayer::Uninit(void)
{
	// 自分自身の破棄
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CPlayer::Update(void)
{
	// キーボードの移動
	KeyboardMove();

	// パッドの移動
	JoyPadMove();

	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Update();
	}

	// 位置の取得
	D3DXVECTOR3 pos = m_pCharacter->GetPosition();

	// 地面に当たっていたら
	if (m_collisionMeshFieldResult.bHit)
	{
		// 高さの設定
		pos.y = m_collisionMeshFieldResult.fHeight;

		D3DXVECTOR3 move = m_pCharacter->GetMove();

		// 重力を消す
		move.y = 0.0f;

		m_pCharacter->SetMove(move);
	}

	// 位置の更新
	m_pCharacter->SetPosition(pos);

	// 向きの取得
	D3DXVECTOR3 rot = m_pCharacter->GetRotation();

	// 目的の向きまで最短で行くために正規化
	math::NormalizeDiffRot(m_rotDest.y - rot.y, &rot.y);

	// 目的の向きに近づける
	rot.y += (m_rotDest.y - rot.y) * 0.1f;

	// 向きの更新
	m_pCharacter->SetRotation(rot);
}

//===================================================
// 描画処理
//===================================================
void CPlayer::Draw(void)
{
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Draw();
	}
}

//===================================================
// 位置の取得処理
//===================================================
const D3DXVECTOR3& CPlayer::GetPosition(void) const
{
	return m_pCharacter->GetPosition();
}

//===================================================
// キーボードの移動
//===================================================
void CPlayer::KeyboardMove(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// パッドの取得
	CInputJoypad* pJoypad = pManager->GetInputJoypad();

	// カメラの取得
	CCamera* pCamera = pManager->GetCamera();

	// 取得できなかったら
	if (pCamera == nullptr)
	{
		return;
	}

	// パッドを動かしていたら
	if (pJoypad->GetJoyStickL())
	{
		return;
	}

	// カメラの向きの取得
	float fCameraAngleY = pCamera->GetRotation().y;

	// 移動量の取得
	D3DXVECTOR3 move = m_pCharacter->GetMove();

	// 左移動
	if (InputSystem::PlayerMoveLeft())
	{
		// 正面移動
		if (InputSystem::PlayerMoveForward())
		{
			move.x += sinf(fCameraAngleY - D3DX_PI * 0.25f) * m_fSpeed;
			move.z += cosf(fCameraAngleY - D3DX_PI * 0.25f) * m_fSpeed;

			m_rotDest.y = fCameraAngleY + D3DX_PI * 0.75f;
		}
		// 後ろ移動
		else if (InputSystem::PlayerMoveBack())
		{
			move.x += sinf(fCameraAngleY - D3DX_PI * 0.75f) * m_fSpeed;
			move.z += cosf(fCameraAngleY - D3DX_PI * 0.75f) * m_fSpeed;

			m_rotDest.y = fCameraAngleY + D3DX_PI * 0.25f;
		}
		// 左移動
		else
		{
			move.z += sinf(fCameraAngleY) * m_fSpeed;
			move.x -= cosf(fCameraAngleY) * m_fSpeed;

			m_rotDest.y = fCameraAngleY + D3DX_PI * 0.5f;
		}
	}
	// 右移動
	else if (InputSystem::PlayerMoveRight())
	{
		// 正面移動
		if (InputSystem::PlayerMoveForward())
		{
			move.x += sinf(fCameraAngleY + D3DX_PI * 0.25f) * m_fSpeed;
			move.z += cosf(fCameraAngleY + D3DX_PI * 0.25f) * m_fSpeed;

			m_rotDest.y = fCameraAngleY - D3DX_PI * 0.75f;
		}
		// 後ろ移動
		else if (InputSystem::PlayerMoveBack())
		{
			move.x += sinf(fCameraAngleY + D3DX_PI * 0.75f) * m_fSpeed;
			move.z += cosf(fCameraAngleY + D3DX_PI * 0.75f) * m_fSpeed;

			m_rotDest.y = fCameraAngleY - D3DX_PI * 0.25f;
		}
		// 右移動
		else
		{
			move.z -= sinf(fCameraAngleY) * m_fSpeed;
			move.x += cosf(fCameraAngleY) * m_fSpeed;

			m_rotDest.y = fCameraAngleY - D3DX_PI * 0.5f;
		}
	}
	// 正面移動
	else if (InputSystem::PlayerMoveForward())
	{
		move.x += sinf(fCameraAngleY) * m_fSpeed;
		move.z += cosf(fCameraAngleY) * m_fSpeed;

		m_rotDest.y = fCameraAngleY + D3DX_PI;
	}
	// 後ろ移動
	else if (InputSystem::PlayerMoveBack())
	{
		move.x -= sinf(fCameraAngleY) * m_fSpeed;
		move.z -= cosf(fCameraAngleY) * m_fSpeed;

		m_rotDest.y = fCameraAngleY;
	}

	// 移動量の設定
	m_pCharacter->SetMove(move);
}

//===================================================
// ジョイパッドの移動
//===================================================
void CPlayer::JoyPadMove(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// パッドの取得
	CInputJoypad* pJoypad = pManager->GetInputJoypad();

	XINPUT_STATE* pStick;

	pStick = pJoypad->GetJoyStickAngle();

	// カメラの取得
	CCamera* pCamera = pManager->GetCamera();

	// 取得できなかったら
	if (pCamera == nullptr)
	{
		return;
	}

	// カメラの向き
	D3DXVECTOR3 cameraRot = pCamera->GetRotation();

	// Lスティックの角度
	float LStickAngleY = pStick->Gamepad.sThumbLY;
	float LStickAngleX = pStick->Gamepad.sThumbLX;

	// デッドゾーン
	float deadzone = 32767.0f * 0.25f;

	// スティックの傾けた角度を求める
	float magnitude = sqrtf((LStickAngleX * LStickAngleX) + (LStickAngleY * LStickAngleY));

	// 動かせる
	if (magnitude > deadzone)
	{
		// アングルを正規化
		float normalizeX = (LStickAngleX / magnitude);
		float normalizeY = (LStickAngleY / magnitude);

		// プレイヤーの移動量
		float moveX = normalizeX * cosf(-cameraRot.y) - normalizeY * sinf(-cameraRot.y);
		float moveZ = normalizeX * sinf(-cameraRot.y) + normalizeY * cosf(-cameraRot.y);

		// 移動量をスティックの角度によって変更
		float speedWk = m_fSpeed * ((magnitude - deadzone) / (32767.0f - deadzone));

		// 移動量
		D3DXVECTOR3 moveWk = m_pCharacter->GetMove();

		// プレイヤーの移動
		moveWk.x += moveX * speedWk;
		moveWk.z += moveZ * speedWk;

		// 移動量の設定
		m_pCharacter->SetMove(moveWk);

		// プレイヤーの角度を移動方向にする
		float fDestAngle = atan2f(-moveX, -moveZ);

		m_rotDest.y = fDestAngle;
	}
}