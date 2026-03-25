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
CWhole::CWhole() : 
	CObject(PRIORITY_CHARACTER),
	m_pCharacter(nullptr)
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
CWhole::~CWhole()
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
CWhole* CWhole::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pMotionFilePath)
{
	// 自分自身の生成
	CWhole* pInstance = new CWhole;

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
HRESULT CWhole::Init(void)
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
void CWhole::Uninit(void)
{
	// Physicsの破棄
	ReleasePhysics();

	// 自分自身の破棄
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CWhole::Update(void)
{
	if (m_pCharacter != nullptr)
	{
		// 更新処理
		m_pCharacter->Update();
	}

	// 位置の取得
	D3DXVECTOR3 pos = m_pCharacter->GetPosition();

	// コライダーの更新
	UpdateCollider(pos);
}

//===================================================
// 描画処理
//===================================================
void CWhole::Draw(void)
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
Collider* CWhole::GetCollisionShape(void) const
{
	return m_pShape.get();
}

//=============================================================================
// Physicsの破棄
//=============================================================================
void CWhole::ReleasePhysics(void)
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
void CWhole::CreatePhysics(D3DXVECTOR3 size, float mass)
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
void CWhole::UpdateCollider(D3DXVECTOR3 offset)
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
void CWhole::OnCollisionEnter(IGameObject* other)
{
	// プレイヤー
	if (other->CompareTag("Player"))
	{

	}
}
