//===================================================
//
// プランクトンの処理 [plankton.cpp]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "plankton.h"
#include "manager.h"
#include "renderer.h"
#include "color_constants.h"
#include "Collider.h"
#include "RigidBody.h"
#include "player.h"
#include "DebugProc3D.h"
#include "game.h"
#include "plankton_controller.h"

//***************************************************
// 定数宣言
//***************************************************
namespace PlanktonConst
{
	constexpr const char* TEXTURE_PATH = "effect000.jpg";	// テクスチャのパス

	constexpr float FOLLOW_LENGTH = 50.0f;	// 位置の補間の係数
	constexpr float FOLLOW_SPEED = 0.04f;
	constexpr float EATEN_LERP_TIME = 60.0f;	// 食べられに行く演出の補完係数
}

//===================================================
// コンストラクタ
//===================================================
CPlankton::CPlankton() : 
	m_pRigidBody(nullptr),
	m_pShape(nullptr),
	m_followPos(Const::VEC3_NULL),
	m_state(State::Idel),
	m_eatenData(),
	m_fTime(0.0f)
{
	// プランクトンのタグを設定
	SetTag("planckton");
}

//===================================================
// デストラクタ
//===================================================
CPlankton::~CPlankton()
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	auto world = pManager->GetPhysicsWorld();

	if (m_pShape != nullptr && world != nullptr)
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

//===================================================
// 生成処理
//===================================================
CPlankton* CPlankton::Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size)
{
	// 自分自身の生成
	CPlankton* pInstance = new CPlankton;

	// パラメータ設定
	pInstance->SetPosition(pos);
	pInstance->SetSize(size);

	// 初期化処理
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
HRESULT CPlankton::Init(void)
{
	// 初期化処理
	if (FAILED(CObjectBillboard::Init()))
	{
		return E_FAIL;
	}

	m_eatenData.nFrameCounter = 0;

	// テクスチャのIDの設定
	CObjectBillboard::SetTextureID(PlanktonConst::TEXTURE_PATH);

	// 色の設定
	CObjectBillboard::SetColor(Color::GOLD);

	// コライダーを生成(カプセル)
	m_pShape = std::make_shared<SphereCollider>(D3DXVECTOR3(50.0f,50.0f,50.0f));

	// オーナーの設定
	m_pShape->SetOwner(this);

	//// コライダー中心
	//m_colliderPos = GetPosition();

	// 質量を設定
	D3DXVECTOR3 inertia = Const::VEC3_NULL;  // 慣性

	m_pShape->calculateLocalInertia(5.0f, inertia);

	// リジッドボディの生成
	m_pRigidBody = std::make_shared<RigidBody>(m_pShape, 5.0f);

	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, 0.0f, 0.0f, 0.0f);

	// 位置の取得
	D3DXVECTOR3 colliderPos = CObjectBillboard::GetPosition();

	// 位置の設定
	m_pRigidBody->SetTransform(colliderPos, q, Const::INIT_SCAL);

	m_pRigidBody->SetIsDynamic(false);			// ダイナミックブロックに設定
	m_pRigidBody->SetCollisionFlag(true);		// 物理的な衝突判定をしない

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
		pWorld->AddRigidBody(m_pRigidBody);
	}

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CPlankton::Uninit(void)
{
	// 終了処理
	CObjectBillboard::Uninit();
}

//===================================================
// 更新処理
//===================================================
void CPlankton::Update(void)
{
	// 更新処理
	CObjectBillboard::Update();

	if (m_state == State::Follow)
	{
		// 現在の位置の取得
		D3DXVECTOR3 currentPos = CObjectBillboard::GetPosition();

		m_fTime += PlanktonConst::FOLLOW_SPEED;
		if (m_fTime >= D3DX_PI * 2.0f)
		{
			m_fTime = 0.0f;
		}

		// 現在の位置と補間する
		currentPos.x = m_followPos.x + sinf(m_fTime) * PlanktonConst::FOLLOW_LENGTH;
		currentPos.z = m_followPos.z + cosf(m_fTime) * PlanktonConst::FOLLOW_LENGTH;

		// コライダー位置の設定
		m_pRigidBody->SetTransform(currentPos, Const::QUATERNION_IDENTITY, Const::INIT_SCAL);

		// プランクトンの位置を更新(プレイヤー追従)
		CObjectBillboard::SetPosition(currentPos);
	}
}

//===================================================
// 描画処理
//===================================================
void CPlankton::Draw(void)
{
#ifdef _DEBUG
	// スフィアコライダーの描画
	if (auto cap = GetCollisionShape()->As<SphereCollider>())
	{
		// レンダラーの取得
		CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

		// 3Dデバッグ情報の取得
		auto pDebug3D = pRenderer->GetDebugProc3D();

		pDebug3D->DrawSphereCollider(cap, Color::GREENYELLOW);
	}
#endif

	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// レンダラーの取得
	CRenderer* pRenderer = pManager->GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ゼットテスト
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//// アルファテストを有効
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, NULL);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// aブレンディング
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 描画処理
	CObjectBillboard::Draw();

	// ゼットテスト
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// アルファテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//// aブレンディングをもとに戻す
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//===================================================
// 当たり判定
//===================================================
void CPlankton::OnCollisionEnter(IGameObject* other)
{
	// プレイヤーとの当たり判定
	if (other->CompareTag("Player"))
	{
		// プランクトンの操作クラスの定義
		CPlanktonController *pPlanktonController = CGame::GetPlanktonController();

		// プライヤーとの結合
		pPlanktonController->RegisterPlayerPlanktonList(this);

		m_state = State::Follow;
	}
}

//===================================================
// コライダーの取得処理
//===================================================
Collider* CPlankton::GetCollisionShape(void) const
{
	return m_pShape.get();
}

//===================================================
// 状態の取得
//===================================================
CPlankton::State CPlankton::GetState(void) const
{
	return m_state;
}

//===================================================
// 食べられに行く処理
//===================================================
bool CPlankton::ProceedToBeEaten(const D3DXVECTOR3& destPos)
{
	// 食べられる状態じゃないなら
	if (m_state != CPlankton::State::BeEaten)
	{
		return false;
	}

	// 位置の取得
	D3DXVECTOR3 pos = CObjectBillboard::GetPosition();

	// フレームの割合を求める
	float fRate = m_eatenData.nFrameCounter / PlanktonConst::EATEN_LERP_TIME;

	// フレームを加算
	m_eatenData.nFrameCounter++;

	// 目的の位置に近づける
	pos += (destPos - pos) * fRate;

	CObjectBillboard::SetPosition(pos);

	// コライダー位置の設定
	m_pRigidBody->SetTransform(pos, Const::QUATERNION_IDENTITY, Const::INIT_SCAL);

	if (fRate >= 1.0f)
	{
		return true;
	}

	return false;
}