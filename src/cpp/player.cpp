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
#include "PlayerState.h"
#include "PhysicsWorld.h"
#include "DebugProc3D.h"
#include "Collider.h"
#include "RigidBody.h"
#include "renderer.h"


#include "whole.h"

// コライダーパラメータ
namespace ColliderParam
{
	const D3DXVECTOR3	OFFSET{ 0.0f, 36.0f, 0.0f };		// オフセット
	const D3DXCOLOR		COLOR{ 0.0f, 1.0f, 0.3f, 1.0f };	// 色
}

//===================================================
// コンストラクタ
//===================================================
CPlayer::CPlayer() : 
	CObject(PRIORITY_CHARACTER),
	m_pCharacter(nullptr),
	m_rotDest(Const::VEC3_NULL),
	m_fSpeed(2.0f)
{
	// タグの設定
	SetTag("Player");

	// 値のクリア
	m_pRigidBody	= nullptr;				// 剛体へのポインタ
	m_pShape		= nullptr;				// 当たり判定の形へのポインタ
	m_colliderPos	= Const::VEC3_NULL;		// コライダーの位置
	m_isMoving		= false;				// 移動しているか
	m_bOnGround		= false;				// 接地フラグ
	m_isJumping		= false;				// ジャンプ中フラグ
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
	//if (m_pCharacter != nullptr)
	//{
	//	m_pCharacter->SetParameter(0.25f, 0.1f);
	//}

	// カプセルコライダーの設定
	CreatePhysics(CAPSULE_RADIUS, CAPSULE_HEIGHT, MASS);

	// インスタンスのポインタを渡す
	m_stateMachine.Start(this);

	// 初期状態のステートをセット
	m_stateMachine.ChangeState<CPlayerStandState>();

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CPlayer::Uninit(void)
{
	// Physicsの破棄
	ReleasePhysics();

	// 自分自身の破棄
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CPlayer::Update(void)
{
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Update();
	}

	//auto pPhysicsWorld = CManager::GetInstance()->GetPhysicsWorld();

	//if (!m_isJumping && pPhysicsWorld)
	//{
	//	// 接地判定
	//	m_bOnGround = OnGround(pPhysicsWorld, 5.0f);
	//}

	// ステートマシン更新
	m_stateMachine.Update();

	// 入力判定の取得
	InputData input = GatherInput();

	// 位置の取得
	D3DXVECTOR3 pos = m_pCharacter->GetPosition();

	// 向きの取得
	D3DXVECTOR3 rot = m_pCharacter->GetRotation();

	// 目的の向きまで最短で行くために正規化
	math::NormalizeDiffRot(m_rotDest.y - rot.y, &rot.y);

	// 目的の向きに近づける
	rot.y += (m_rotDest.y - rot.y) * 0.1f;

	// 向きの更新
	m_pCharacter->SetRotation(rot);

	// 移動入力があればプレイヤー向きを入力方向に
	if (input.moveDir.x != 0.0f || input.moveDir.z != 0.0f)
	{
		// Yを入力方向に向ける
		m_rotDest.y = atan2f(-input.moveDir.x, -input.moveDir.z);
	}

	// コライダーの更新
	UpdateCollider(ColliderParam::OFFSET);
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

#ifdef _DEBUG
	// カプセルコライダーの描画
	if (auto cap = GetCollisionShape()->As<CapsuleCollider>())
	{
		// レンダラーの取得
		CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

		// 3Dデバッグ情報の取得
		auto pDebug3D = pRenderer->GetDebugProc3D();

		pDebug3D->DrawCapsuleCollider(cap, ColliderParam::COLOR);
	}
#endif
}

//===================================================
// コライダーの取得処理
//===================================================
Collider* CPlayer::GetCollisionShape(void) const
{
	return m_pShape.get();
}

//===================================================
// 位置の取得処理
//===================================================
const D3DXVECTOR3& CPlayer::GetPosition(void) const
{
	return m_pCharacter->GetPosition();
}

//===================================================
// 移動量の取得処理
//===================================================
D3DXVECTOR3 CPlayer::GetMove(void) const
{
	return m_pCharacter->GetMove();
}

//=============================================================================
// Physicsの破棄
//=============================================================================
void CPlayer::ReleasePhysics(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	auto world = pManager->GetPhysicsWorld();

	// リジッドボディの破棄
	if (m_pRigidBody)
	{
		if (world)
		{
			world->RemoveRigidBody(m_pRigidBody);
		}

		m_pRigidBody = nullptr;
	}

	// シェイプの破棄
	if (m_pShape)
	{
		m_pShape = nullptr;
	}
}

//=============================================================================
// 当たり判定の生成処理
//=============================================================================
void CPlayer::CreatePhysics(float radius, float height, float mass)
{
	//*********************************************************************
	// カプセルコライダーの設定
	//*********************************************************************

	// コライダーを生成(カプセル)
	m_pShape = std::make_shared<CapsuleCollider>(radius, height);

	// オーナーの設定
	m_pShape->SetOwner(this);

	// コライダー中心
	m_colliderPos = GetPosition();

	// 質量を設定
	D3DXVECTOR3 inertia = Const::VEC3_NULL;  // 慣性

	m_pShape->calculateLocalInertia(mass, inertia);

	// リジッドボディの生成
	m_pRigidBody = std::make_shared<RigidBody>(m_pShape, mass);

	D3DXVECTOR3 euler = m_pCharacter->GetRotation(); // オイラー角（ラジアン）
	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, euler.y, euler.x, euler.z);

	// 位置の設定
	m_pRigidBody->SetTransform(m_colliderPos, q, Const::INIT_SCAL);

	m_pRigidBody->SetIsDynamic(true);			// ダイナミックブロックに設定

	//// パラメータは現在は無視
	//m_pRigidBody->SetLinearFactor(Const::INIT_SCAL);
	//m_pRigidBody->SetAngularFactor(D3DXVECTOR3(0, 0, 0));
	//m_pRigidBody->SetFriction(1.5f);// 摩擦
	//m_pRigidBody->SetRollingFriction(0.0f);// 転がり摩擦

	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// 物理ワールドに追加
	PhysicsWorld* pWorld = pManager->GetPhysicsWorld();

	if (pWorld != nullptr)
	{
		//// 重力を消す
		//pWorld->SetGravity(Const::VEC3_NULL);

		// リジッドボディを追加
		pWorld->AddRigidBody(m_pRigidBody);
	}
}

//=============================================================================
// 当たり判定の位置更新
//=============================================================================
void CPlayer::UpdateCollider(D3DXVECTOR3 offset)
{
	// 向きの取得
	D3DXVECTOR3 rot = m_pCharacter->GetRotation();

	// 位置の取得
	D3DXVECTOR3 pos = GetPosition();

	// クォータニオンにして Rigidbody に渡す
	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, rot.y, 0, 0);
	m_pRigidBody->SetOrientation(q);

	// Rigidbody から物理座標を取得（カプセル中心）
	D3DXVECTOR3 rigidPos = m_pRigidBody->GetPosition();
	
	// カプセルコライダーに反映
	m_colliderPos = rigidPos;

	// モデル描画やゲーム上の座標は足元基準に変換
	pos = rigidPos - offset;

	// 地面に当たっていたら
	if (m_collisionMeshFieldResult.bHit)
	{
		// マネージャーの取得
		CManager* pManager = CManager::GetInstance();

		// 物理ワールドに追加
		PhysicsWorld* pWorld = pManager->GetPhysicsWorld();

		// 高さの設定
		pos.y = m_collisionMeshFieldResult.fHeight;
		rigidPos.y = m_collisionMeshFieldResult.fHeight + offset.y;
		m_pRigidBody->SetTransform(rigidPos, q, m_pRigidBody->GetScale());

		// 重力を消す
		pWorld->SetGravity(Const::VEC3_NULL);
	}

	// プレイヤー位置を更新
	m_pCharacter->SetPosition(pos);
}

//=============================================================================
// 減速処理
//=============================================================================
void CPlayer::ApplyDeceleration(void)
{
	D3DXVECTOR3 move = GetMove();

	// 減速させる
	move *= DECELERATION_RATE;

	if (fabsf(move.x) < 0.01f) move.x = 0;
	if (fabsf(move.z) < 0.01f) move.z = 0;

	// 移動量の設定
	SetPhysicsMove(move);
}

//=============================================================================
// 物理用移動量設定処理
//=============================================================================
void CPlayer::SetPhysicsMove(D3DXVECTOR3 move)
{
	m_pCharacter->SetMove(move);

	if (m_pRigidBody)
	{
		D3DXVECTOR3 vel = m_pRigidBody->GetVelocity();
		vel.x = move.x; // X方向速度
		vel.z = move.z; // Z方向速度
		m_pRigidBody->SetVelocity(vel);  // RigidBody にセット
	}
}

//=============================================================================
// 接地判定処理
//=============================================================================
bool CPlayer::OnGround(PhysicsWorld* world, float rayLength)
{
	D3DXVECTOR3 start = GetPosition();
	start.y += 0.1f; // 少し上から（めり込み対策）

	PhysicsWorld::Ray ray;
	ray.origin = start;
	ray.direction = D3DXVECTOR3(0, -1, 0);

	float closestDist = rayLength;
	D3DXVECTOR3 hitNormal;

	for (auto& body : world->GetBodies())
	{
		// リジッドボディがnullptrだったら
		if (!body)
		{
			continue;
		}

		if (body.get() == m_pRigidBody.get())
		{
			continue; // 自分除外
		}

		// コライダーの取得
		auto collider = body->GetCollider();

		// コライダーがnullptrだったら
		if (!collider)
		{
			continue;
		}

		D3DXVECTOR3 normal;

		PhysicsWorld::RaycastHit hit;

		// レイキャスト
		if (collider->Raycast(ray, rayLength, hit))
		{
			if (hit.distance < closestDist)
			{
				closestDist = hit.distance;
				hitNormal = hit.normal;
			}
		}
	}

	// ヒットしてない
	if (closestDist >= rayLength)
	{
		return false;
	}

	return hitNormal.y > 0.5f;
}

//=============================================================================
// 入力判定取得関数
//=============================================================================
CPlayer::InputData CPlayer::GatherInput(void)
{
	InputData input{};
	input.moveDir = Const::VEC3_NULL;
	input.isJump = false;

	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	CInputJoypad* pJoypad = pManager->GetInputJoypad();			// ジョイパッドの取得
	XINPUT_STATE* pStick = pJoypad->GetJoyStickAngle();			// スティックの取得
	CCamera* pCamera = pManager->GetCamera();					// カメラの取得
	D3DXVECTOR3 CamRot = pCamera->GetRotation();				// カメラ角度の取得

	//---------------------------
	// ジャンプ入力
	//---------------------------
	if (InputSystem::PlayerJump())
	{
		input.isJump = true;
	}

	//---------------------------
	// ゲームパッド入力
	//---------------------------
	if (pJoypad->GetJoyStickL() && pStick)
	{
		float stickX = pStick->Gamepad.sThumbLX;
		float stickY = pStick->Gamepad.sThumbLY;
		float magnitude = sqrtf(stickX * stickX + stickY * stickY);

		if (magnitude >= CInputJoypad::DEAD_ZONE)
		{
			stickX /= magnitude;
			stickY /= magnitude;
			float normMag = std::min((magnitude - CInputJoypad::DEAD_ZONE) / (CInputJoypad::LSTICK_VALUE - CInputJoypad::DEAD_ZONE), 1.0f);
			stickX *= normMag;
			stickY *= normMag;

			float yaw = CamRot.y;

			D3DXVECTOR3 forward = D3DXVECTOR3(sinf(yaw), 0, cosf(yaw));
			D3DXVECTOR3 right = D3DXVECTOR3(cosf(yaw), 0, -sinf(yaw));

			D3DXVECTOR3 dir = forward * stickY + right * stickX;

			input.moveDir += dir;
		}
	}

	//---------------------------
	// キーボード入力
	//---------------------------
	if (InputSystem::PlayerMoveForward())
	{
		input.moveDir += D3DXVECTOR3(sinf(CamRot.y), 0, cosf(CamRot.y));
	}
	if (InputSystem::PlayerMoveBack())
	{
		input.moveDir += D3DXVECTOR3(-sinf(CamRot.y), 0, -cosf(CamRot.y));
	}
	if (InputSystem::PlayerMoveLeft())
	{
		input.moveDir += D3DXVECTOR3(-cosf(CamRot.y), 0, sinf(CamRot.y));
	}
	if (InputSystem::PlayerMoveRight())
	{
		input.moveDir += D3DXVECTOR3(cosf(CamRot.y), 0, -sinf(CamRot.y));
	}

	// 正規化
	if (input.moveDir.x != 0.0f || input.moveDir.z != 0.0f)
	{
		D3DXVec3Normalize(&input.moveDir, &input.moveDir);
	}

	return input;
}