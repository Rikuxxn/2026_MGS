//===================================================
//
// クジラの処理をするクラス [whole.cpp]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "whale.h"
#include "character.h"
#include "motion.h"
#include "manager.h"
#include "PhysicsWorld.h"
#include "Collider.h"
#include "RigidBody.h"
#include "renderer.h"
#include "DebugProc3D.h"
#include "whale_controller.h"
#include "game.h"
#include "utility_math.h"
#include "effect.h"
#include "easing.h"
#include "particle.h"
#include "color_constants.h"

//***************************************************
// 定数宣言
//***************************************************
namespace WhaleConst
{
	constexpr float REACTION_MOTION_DISTANCE	= 250.0f;	// リアクションモーションをする範囲
	constexpr float ROT_LERP_ALPHA				= 0.04f;	// 目的の向きに補完する係数
	constexpr int REACTION_INTERVAL				= 600;		// 1回リアクションした後の待機時間
	constexpr int MAX_PLANKTON					= 1;		// 最大成長
	constexpr int BLOW_STAY_TIME				= 600;		// 潮吹きのインターバル
}

// コライダーパラメータ
namespace ColliderParam
{
	const D3DXVECTOR3	SPHERE_SIZE{ 120.0f,120.0f,120.0f };// サイズ
	const D3DXCOLOR		COLOR{ 0.0f, 1.0f, 0.3f, 1.0f };	// 色
	constexpr float MASS = 1.0f;// 質量
}

//===================================================
// コンストラクタ
//===================================================
CWhale::CWhale() : 
	CObject(PRIORITY_CHARACTER),
	m_pCharacter(nullptr),
	m_rotDest(Const::VEC3_NULL),
	m_blowInfo(),
	m_nReactionMotionInterval(0),
	m_nNumPlankton(0),
	m_fScalingTime(0.0f)
{
	// タグの設定
	SetTag("Whale");

	// 値のクリア
	m_pRigidBody	= nullptr;				// 剛体へのポインタ
	m_pShape		= nullptr;				// 当たり判定の形へのポインタ
}

//===================================================
// デストラクタ
//===================================================
CWhale::~CWhale()
{
	// キャラクタークラスの破棄
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Uninit();
		m_pCharacter = nullptr;
	}
}

//===================================================
// 生成処理
//===================================================
CWhale* CWhale::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pMotionFilePath)
{
	// 自分自身の生成
	CWhale* pInstance = new CWhale;

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
HRESULT CWhale::Init(void)
{
	// モーションの取得
	CMotion *pMotion = m_pCharacter->GetMotion();

	pMotion->Play(MOTIONTYPE_EAT,true,10);

	// スフィアコライダーの設定
	CreatePhysics(ColliderParam::SPHERE_SIZE, ColliderParam::MASS);

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CWhale::Uninit(void)
{
	// Physicsの破棄
	ReleasePhysics();

	// 自分自身の破棄
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CWhale::Update(void)
{
	if (m_pCharacter != nullptr)
	{
		// 更新処理
		m_pCharacter->Update();
	}

	// 位置の取得
	D3DXVECTOR3 pos = m_pCharacter->GetPosition();

	// 向きの取得
	D3DXVECTOR3 rot = m_pCharacter->GetRotation();

	// 目的の向きまで最短で行くために正規化
	math::NormalizeDiffRot(m_rotDest.y - rot.y, &rot.y);

	// 目的の向きに近づける
	rot.y += (m_rotDest.y - rot.y) * WhaleConst::ROT_LERP_ALPHA;

	// 向きの更新
	m_pCharacter->SetRotation(rot);

	// コライダーの更新
	UpdateCollider(pos);

	// 最大まで成長出来たら
	if (m_nNumPlankton >= WhaleConst::MAX_PLANKTON)
	{
		// 潮吹きできる
		m_blowInfo.bBlow = true;

		m_blowInfo.nBlowTime--;

		// 潮吹き時間になったら
		if (m_blowInfo.nBlowTime <= 0)
		{
			CParticle::Info particleInfo;

			// この地獄みてえな引数打てば操作できます
			particleInfo.pos.x = pos.x;
			particleInfo.pos.y = pos.y + 40.0f;
			particleInfo.pos.z = pos.z;
			particleInfo.col = Color::AQUA;
			particleInfo.fAngleXMax = 120;
			particleInfo.fAngleXMin = -120;
			particleInfo.fAngleYMax = 250;
			particleInfo.fAngleYMin = -250;

			particleInfo.moveMax = D3DXVECTOR3(13.0f, 25.0f, 13.0f);
			particleInfo.moveMin = D3DXVECTOR3(5.5f, 12.5f, 5.5f);
			particleInfo.nNum = 10;
			particleInfo.nTime = 60;
			particleInfo.size = { 10.0f,10.0f };
			particleInfo.texturePath = "effect000.jpg";
			particleInfo.effectInfo.nLife = 60;
			particleInfo.effectInfo.fGravity = 0.4f;
			particleInfo.effectInfo.unFlag =
				CEffect::FLAG_ALPHA_DECREASE |
				CEffect::FLAG_RADIUS_DECREASE |
				CEffect::FLAG_GRAVITY;

			CParticle::Create(particleInfo);

			m_blowInfo.nBlowTime = WhaleConst::BLOW_STAY_TIME;
		}
	}
	if (m_bScaling)
	{
		m_fScalingTime += 1.0f;

		float fRate = m_fScalingTime / 60.0f;

		// バウンドの割合の取得
		fRate = CEasing::EaseOutBounce(fRate);

		D3DXVECTOR3 destScale(3.0f, 3.0f, 3.0f);

		// 現在のスケール
		D3DXVECTOR3 scale = Const::INIT_SCAL + (destScale - Const::INIT_SCAL) * fRate;

		// スケールを設定
		m_pCharacter->SetScale(scale);

		if (fRate >= 1.0f)
		{
			m_bScaling = false;
		}
	}
}

//===================================================
// 描画処理
//===================================================
void CWhale::Draw(void)
{
	if (m_pCharacter != nullptr)
	{
		// 描画処理
		m_pCharacter->Draw();
	}

#ifdef _DEBUG
	// スフィアコライダーの描画
	if (auto sphere = GetCollisionShape()->As<SphereCollider>())
	{
		// レンダラーの取得
		CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

		// 3Dデバッグ情報の取得
		auto pDebug3D = pRenderer->GetDebugProc3D();

		pDebug3D->DrawSphereCollider(sphere, ColliderParam::COLOR);
	}
#endif
}

//===================================================
// コライダーの取得処理
//===================================================
Collider* CWhale::GetCollisionShape(void) const
{
	return m_pShape.get();
}

//=============================================================================
// Physicsの破棄
//=============================================================================
void CWhale::ReleasePhysics(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	auto world = pManager->GetPhysicsWorld();

	if (m_pShape && world)
	{
		world->RemoveCollider(m_pShape.get());
	}

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
void CWhale::CreatePhysics(D3DXVECTOR3 size, float mass)
{
	// コライダーを生成(スフィア)
	m_pShape = std::make_shared<SphereCollider>(size);

	// オーナーの設定
	m_pShape->SetOwner(this);

	//// コライダー中心
	//m_colliderPos = GetPosition();

	// 質量を設定
	D3DXVECTOR3 inertia = Const::VEC3_NULL;  // 慣性

	m_pShape->calculateLocalInertia(mass, inertia);

	// リジッドボディの生成
	m_pRigidBody = std::make_shared<RigidBody>(m_pShape, mass);

	D3DXVECTOR3 euler = m_pCharacter->GetRotation(); // オイラー角（ラジアン）
	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, euler.y, euler.x, euler.z);

	// 位置の取得
	D3DXVECTOR3 colliderPos = m_pCharacter->GetPosition();

	// 位置の設定
	m_pRigidBody->SetTransform(colliderPos, q, Const::INIT_SCAL);

	m_pRigidBody->SetIsDynamic(false);			// ダイナミックブロックに設定

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
void CWhale::UpdateCollider(D3DXVECTOR3 offset)
{
	// 向きの取得
	D3DXVECTOR3 rot = m_pCharacter->GetRotation();

	// 位置の取得
	D3DXVECTOR3 pos = m_pCharacter->GetPosition();

	// クォータニオンにして Rigidbody に渡す
	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, rot.y, 0, 0);
	m_pRigidBody->SetOrientation(q);

	// Rigidbody から物理座標を取得（カプセル中心）
	D3DXVECTOR3 rigidPos = m_pRigidBody->GetPosition();

	//// カプセルコライダーに反映
	//m_colliderPos = rigidPos;

	// モデル描画やゲーム上の座標は足元基準に変換
	pos = rigidPos - offset;

	//// 地面に当たっていたら
	//if (m_collisionMeshFieldResult.bHit)
	//{
	//	// マネージャーの取得
	//	CManager* pManager = CManager::GetInstance();

	//	// 物理ワールドに追加
	//	PhysicsWorld* pWorld = pManager->GetPhysicsWorld();

	//	// 高さの設定
	//	pos.y = m_collisionMeshFieldResult.fHeight;
	//	rigidPos.y = m_collisionMeshFieldResult.fHeight + offset.y;
	//	m_pRigidBody->SetTransform(rigidPos, q, m_pRigidBody->GetScale());

	//	// 重力を消す
	//	pWorld->SetGravity(Const::VEC3_NULL);
	//}

	//// 位置を更新
	//m_pCharacter->SetPosition(pos);
}

//===================================================
// 衝突瞬間判定
//===================================================
void CWhale::OnCollisionEnter(IGameObject* other)
{
	// 最大まで成長していたら処理をしない
	if (CheckMaxPlankton())
	{
		return;
	}

	// プレイヤー
	if (other->CompareTag("Player"))
	{
		// クジラの操作クラスの取得
		CWhaleController* pWhaleController = CGame::GetWhaleController();

		// プレイヤーと当たった
		pWhaleController->OnHitPlayer(this);
	}
}

//===================================================
// プレイヤーとの距離を判定してモーションを設定する関数
//===================================================
void CWhale::SetMotionByPlayerDistance(const D3DXVECTOR3& playerPos)
{
	// 生成できてないなら
	if (m_pCharacter == nullptr) return;

	D3DXVECTOR3 pos = m_pCharacter->GetPosition();

	// プレイヤーとの距離を求める
	float fDistance = math::GetDistance(pos - playerPos);

	// 距離を求める
	if (fDistance <= WhaleConst::REACTION_MOTION_DISTANCE)
	{
		// モーションクラスの取得
		CMotion* pMotion = m_pCharacter->GetMotion();

		if (pMotion == nullptr)
		{
			return;
		}

		if (m_nReactionMotionInterval <= 0)
		{
			// モーションの再生
			pMotion->Play(MOTIONTYPE_REACTION, true, 10);
		}

		m_rotDest.y = math::GetTargetAngle(pos, playerPos);

		m_nReactionMotionInterval = WhaleConst::REACTION_INTERVAL;
	}
	else // 範囲外に出たら
	{
		m_nReactionMotionInterval = 0;
	}
	m_nReactionMotionInterval--;
}

//===================================================
// スケールの設定
//===================================================
void CWhale::SetScal(const D3DXVECTOR3& scale)
{
	m_pCharacter->SetScale(scale);
}

//===================================================
// プランクトンを食べる処理
//===================================================
void CWhale::EatPlankton(void)
{
	// 食べた数を加算
	m_nNumPlankton++;

	m_bScaling = true;

	m_fScalingTime = 0.0f;

	if (m_nNumPlankton >= WhaleConst::MAX_PLANKTON)
	{
		CGame::GetWhaleController()->OnWhaleSatisfied(this);
	}
}

//===================================================
// プランクトンが最大まで行ったかどうか判定
//===================================================
inline bool CWhale::CheckMaxPlankton(void) const
{
	// 最大まで成長していたら
	if (m_nNumPlankton >= WhaleConst::MAX_PLANKTON)
	{
		return true;
	}
	return false;
}

//===================================================
// 位置の取得
//===================================================
const D3DXVECTOR3& CWhale::GetPosition(void) const
{
	return m_pCharacter->GetPosition();
}

//===================================================
// 食べる位置の取得
//===================================================
const D3DXVECTOR3& CWhale::GetEatPos(void)
{
	// プレイヤーと
	const D3DXVECTOR3& pos = m_pCharacter->GetPosition();

#ifdef _DEBUG
	CEffect::Info effectInfo;

	effectInfo.unFlag = CEffect::FLAG_NONE;
	effectInfo.move = Const::VEC3_NULL;
	effectInfo.nLife = 5;
	
	D3DXVECTOR3 effectPos = pos;

	CEffect::Create(effectInfo, effectPos, { 10.0f,10.0f},Const::WHITE,"effect000.jpg");
#endif // _DEBUG
	return pos;
}
