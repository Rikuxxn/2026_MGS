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
#include "plankton.h"
#include "particle_registry.h"
#include "color_constants.h"

#include "whale.h"

// コライダーパラメータ
namespace ColliderParam
{
	const D3DXVECTOR3	SPHERE_SIZE{ 120.0f,120.0f,120.0f };// サイズ
	const D3DXCOLOR		COLOR{ 0.0f, 1.0f, 0.3f, 1.0f };	// 色
}

//===================================================
// コンストラクタ
//===================================================
CPlayer::CPlayer() : 
	CObject(PRIORITY_CHARACTER),
	m_pCharacter(nullptr),
	m_rotDest(Const::VEC3_NULL),
	m_fSpeed(2.0f),
	m_pHasPlanktonList(),
	m_pWhale(nullptr),
	m_nPlanktonReleaseCnt(0)
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

	// スフィアコライダーの設定
	CreatePhysics(ColliderParam::SPHERE_SIZE, MASS);

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
	UpdateCollider(Const::VEC3_NULL);

	// プランクトンの更新処理
	UpdatePlankton();

	// クジラの更新処理
	UpdateWhale();

	// ワールドマトリックスの取得
	D3DXMATRIX mtxWorld = m_pCharacter->GetMtxWorld();

	D3DXVECTOR3 worldPos = math::GetPositionFromMatrix(mtxWorld);

	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// パーティクルのパラメータの管理クラスの取得
	CParticleRegistry* pParticleRegistry = pManager->GetParticleRegistry();

	CParticle::Info particleInfo;

	particleInfo.col = Color::AQUA;
	particleInfo.pos = worldPos;
	particleInfo.fAngleXMax = 120;
	particleInfo.fAngleXMin = -120;
	particleInfo.fAngleYMax = 120;
	particleInfo.fAngleYMin = -120;
	particleInfo.moveMax = { 5.0f,2.5f,5.0f };
	particleInfo.moveMin = { 2.5f,1.0f,2.5f };
	particleInfo.nNum = 1;
	particleInfo.size = { 10.0f,10.0f };
	particleInfo.texturePath = "effect000.jpg";
	particleInfo.nTime = 1;
	particleInfo.effectInfo.nLife = 20;

	particleInfo.effectInfo.unFlag =
		CEffect::FLAG_ALPHA_DECREASE |
		CEffect::FLAG_RADIUS_DECREASE |
		CEffect::FLAG_GRAVITY;

	CParticle::Create(particleInfo);

	// パーティクルの生成
	//pParticleRegistry->CreateParticle(
	//	"move",
	//	worldPos,
	//	Color::AQUA);

}

//===================================================
// 描画処理
//===================================================
void CPlayer::Draw(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// レンダラーの取得
	CRenderer* pRenderer = pManager->GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Draw();
	}

#ifdef _DEBUG
	// スフィアコライダーの描画
	if (auto cap = GetCollisionShape()->As<SphereCollider>())
	{
		// 3Dデバッグ情報の取得
		auto pDebug3D = pRenderer->GetDebugProc3D();

		pDebug3D->DrawSphereCollider(cap, ColliderParam::COLOR);
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
void CPlayer::CreatePhysics(D3DXVECTOR3 size, float mass)
{
	// コライダーを生成(スフィア)
	m_pShape = std::make_shared<SphereCollider>(size);

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
// ワールドマトリックスの取得
//=============================================================================
D3DXMATRIX CPlayer::GetWorldMatrix(void)
{
	D3DXMATRIX matScale, matRot, matTrans;

	// スケール行列
	D3DXVECTOR3 scale = Const::VEC3_NULL; // 拡大率
	D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z);

	// 回転行列
	D3DXVECTOR3 rot = m_pCharacter->GetRotation(); // ラジアン角
	D3DXMatrixRotationYawPitchRoll(&matRot, rot.y, rot.x, rot.z);

	// 平行移動行列
	D3DXVECTOR3 pos = GetPosition();
	D3DXMatrixTranslation(&matTrans, pos.x, pos.y, pos.z);

	// 合成：S * R * T
	D3DXMATRIX world = matScale * matRot * matTrans;

	return world;
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

//=============================================================================
// クジラに当たったら
//=============================================================================
void CPlayer::OnHitWhale(CWhale* pWhale)
{
	if (pWhale != nullptr)
	{
		// 満足していたら取得しない
		if (pWhale->CheckMaxPlankton())
		{
			return;
		}
	}
	if (m_pWhale == nullptr)
	{
		m_pWhale = pWhale;
		m_nPlanktonReleaseCnt = PLANKTON_RELEASE_TIME;
	}
}

//=============================================================================
// プランクトン
//=============================================================================
void CPlayer::RegisterPlankton(CPlankton* pPlankton)
{
	// 要素があるか調べる
	auto itr = std::find(m_pHasPlanktonList.begin(), m_pHasPlanktonList.end(), pPlankton);

	// 持っていないなら
	if (itr == m_pHasPlanktonList.end())
	{
		// 要素の追加
		m_pHasPlanktonList.push_back(pPlankton);
	}
}

//=============================================================================
// プランクトンの更新処理
//=============================================================================
void CPlayer::UpdatePlankton(void)
{
	// 持っているプランクトン分回す
	for (auto& plankton : m_pHasPlanktonList)
	{
		// 追従状態じゃないなら処理を飛ばす
		if (plankton->GetState() != CPlankton::State::Follow)
		{
			continue;
		}

		// 自分の位置の取得
		D3DXVECTOR3 pos = GetPosition();

		// プランクトンの位置の設定処理
		plankton->GetFollowPosition(pos);
	}
}

//=============================================================================
// クジラの更新処理
//=============================================================================
void CPlayer::UpdateWhale(void)
{
	// プランクトンを持っていないなら
	if (m_pHasPlanktonList.empty())
	{
		return;
	}

	// 当たったクジラがいないなら
	if (m_pWhale == nullptr)
	{
		return;
	}

	// クジラの位置の取得
	D3DXVECTOR3 whalePos = m_pWhale->GetPosition();
	
	// 位置の取得
	D3DXVECTOR3 pos = GetPosition();

	// クジラとプレイヤーの距離を求める
	float fDistance = math::GetDistance(whalePos - pos);

	if (m_nPlanktonReleaseCnt >= 0)
	{
		m_nPlanktonReleaseCnt--;

		return;
	}

	// 再設定
	m_nPlanktonReleaseCnt = PLANKTON_RELEASE_TIME;

	// 満足していたら(接続を外す)
	if (m_pWhale->CheckMaxPlankton())
	{
		m_pWhale = nullptr;
		return;
	}
	// プランクトン
	for (auto plankton = m_pHasPlanktonList.begin() ; plankton != m_pHasPlanktonList.end();plankton++)
	{
		// 食べられる状態にする
		(*plankton)->SetState(CPlankton::State::BeEaten);

		// 離れたら接続を解除
		if (fDistance >= PLANKTON_NO_EATEN_DISTANCE)
		{
			// 食べられる状態にする
			(*plankton)->SetState(CPlankton::State::Idel);

			m_pWhale = nullptr;
			return;
		}

		(*plankton)->ProceedToBeEaten(whalePos);

		// 要素から外す
		plankton = m_pHasPlanktonList.erase(plankton);

		m_pWhale->EatPlankton();

		break;
	}

	if (m_pHasPlanktonList.empty())
	{
		// 接続を消す
		m_pWhale = nullptr;
	}
}