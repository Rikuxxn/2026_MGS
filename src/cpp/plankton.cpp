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

//***************************************************
// 定数宣言
//***************************************************
namespace PlanktonConst
{
	constexpr const char* TEXTURE_PATH = "effect000.jpg";	// テクスチャのパス
}

//===================================================
// コンストラクタ
//===================================================
CPlankton::CPlankton() : 
	m_pRigidBody(nullptr),
	m_pShape(nullptr),
	m_followPos(Const::VEC3_NULL),
	m_bFollow(true)
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

	// テクスチャのIDの設定
	CObjectBillboard::SetTextureID(PlanktonConst::TEXTURE_PATH);

	// 色の設定
	CObjectBillboard::SetColor(Color::KHAKI);

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

	CObjectBillboard::SetPosition(m_followPos);
}

//===================================================
// 描画処理
//===================================================
void CPlankton::Draw(void)
{
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
		m_bFollow = true;
	}
}
