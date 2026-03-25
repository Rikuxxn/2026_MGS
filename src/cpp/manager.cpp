//===================================================
//
// マネージャークラス [manager.cpp]
// Author:YOSHIDA YUUTO
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "manager.h"
#include "renderer.h"
#include "texture_manager.h"
#include "input.h"
#include "model_manager.h"
#include "fade.h"
#include "light.h"
#include "camera.h"
#include "scene.h"
#include "title.h"
#include "object.h"
#include "game.h"
#include "motion_loader.h"
#include "PhysicsWorld.h"
#include "BlockManager.h"
#include "shader_manager.h"
#include "texture_mrt_manager.h"
#include "particle_registry.h"

//***************************************************
// 定数宣言
//***************************************************
namespace ManagerConst
{
	const D3DXVECTOR3 LIGHT_DIR_000 = { 1.000f, -0.637f, -0.495f };	// ライトの方向0
	const D3DXVECTOR3 LIGHT_DIR_001 = { -0.403f, -0.47f, -0.401f };	// ライトの方向1
	const D3DXVECTOR3 LIGHT_DIR_002 = { 0.515f, -0.616f, 1.0f };	// ライトの方向2
	const float GRAVITY = -320.0f;									// 重力
}

//***************************************************
// 静的メンバ変数
//***************************************************
std::unique_ptr<CManager> CManager::m_pInstance = nullptr;			// 自分のインスタンス

//===================================================
// モードの変更
//===================================================
void CManager::ChangeMode(std::unique_ptr<CScene> pNewScene)
{
	// nullなら処理しない
	if (pNewScene == nullptr) return;

	// 現在のシーンの破棄
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		m_pScene = nullptr;
	}

	// 全てのオブジェクトの破棄
	CObject::ReleaseAll();

	// 所有権を移動
	m_pScene = std::move(pNewScene);

	// カメラの破棄
	m_pCamera->Uninit();

	// シーンの初期化処理
	if (FAILED(m_pScene->Init()))
	{
		MessageBox(NULL, "初期化失敗", "シーンの初期化失敗", MB_OK | MB_ICONWARNING);
	}
}

//===================================================
// フェードの開始
//===================================================
void CManager::BeginFade(std::unique_ptr<CScene> pNewScene, const D3DXCOLOR col)
{
	if (m_pFade != nullptr)
	{
		// フェードの開始
		m_pFade->SetFade(std::move(pNewScene), col);
	}
}

//===================================================
// コンストラクタ
//===================================================
CManager::CManager() :
	m_pRenderer(nullptr),
	m_pScene(nullptr),
	m_pTextureManager(nullptr),
	m_pModelManager(nullptr),
	m_pKeyboard(nullptr),
	m_pJoypad(nullptr),
	m_pMouse(nullptr),
	m_pFade(nullptr),
	m_pLight(nullptr),
	m_pCamera(nullptr),
	//m_pSound(nullptr),
	//m_pMotionManager(nullptr),
	m_pTextureMRTManager(nullptr),
	m_pShaderManager(nullptr),
	m_pParticleRegistry(nullptr),
	m_nFps(0)
{
}

//===================================================
// デストラクタ
//===================================================
CManager::~CManager()
{
}

//===================================================
// インスタンスの取得
//===================================================
CManager* CManager::GetInstance(void)
{
	if (m_pInstance == nullptr)
	{
		// 生成
		m_pInstance.reset(new CManager);
	}

	return m_pInstance.get();
}

//===================================================
// 初期化処理
//===================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWind)
{
	// 乱数の種の設定
	srand(static_cast<unsigned int>(time(nullptr)));

	//// 音の生成
	//m_pSound = std::make_unique<CSound>();

	// レンダラーの生成
	m_pRenderer = std::make_unique<CRenderer>();

	// キーボードの生成
	m_pKeyboard = std::make_unique<CInputKeyboard>();

	// ジョイパッドの生成
	m_pJoypad = std::make_unique<CInputJoypad>();

	// マウスの生成
	m_pMouse = std::make_unique<CInputMouse>();

	//// 音の初期化処理
	//if (FAILED(m_pSound->Init(hWnd))) return E_FAIL;

	// キーボードの初期化処理
	if (FAILED(m_pKeyboard->Init(hInstance, hWnd))) return E_FAIL;

	// ジョイパッドの初期化処理
	if (FAILED(m_pJoypad->Init())) return E_FAIL;

	// マウスの初期化処理
	if (FAILED(m_pMouse->Init(hWnd))) return E_FAIL;

	// レンダラー初期化処理 
	if (FAILED(m_pRenderer->Init(hWnd, bWind))) return E_FAIL;

	// 物理ワールドの生成
	m_pPhysicsWorld = std::make_unique <PhysicsWorld>();

	// 重力の設定
	m_pPhysicsWorld->SetGravity(D3DXVECTOR3(0.0f, ManagerConst::GRAVITY, 0.0f));

	// ブロックマネージャーの生成
	m_pBlockManager = std::make_unique<CBlockManager>();
	m_pBlockManager->Init();

	// カメラの生成
	m_pCamera = std::make_unique<CCamera>();
	m_pCamera->Init();

	// ライトの生成
	m_pLight = std::make_unique<CLight>();

	D3DXCOLOR col = { 1.0f,1.0f,1.0f,1.0f };

	m_pLight->Init();
	m_pLight->SetDirectional(col, ManagerConst::LIGHT_DIR_000, Const::VEC3_NULL);
	m_pLight->SetDirectional(col, ManagerConst::LIGHT_DIR_001, Const::VEC3_NULL);
	m_pLight->SetDirectional(col, ManagerConst::LIGHT_DIR_002, Const::VEC3_NULL);

	// テクスチャマネージャーの生成
	m_pTextureManager = std::make_unique<CTextureManager>();
	m_pTextureManager->Load();

	// モーションのロードクラスの生成
	m_pMotionLoader = std::make_unique<CMotionLoader>();

	// テクスチャMRTのマネージャーの取得
	m_pTextureMRTManager = std::make_unique<CTextureMRTManager>();
	m_pTextureMRTManager->Init();

	// モデルのマネージャーの生成
	m_pModelManager = std::make_unique<CModelManager>();
	m_pModelManager->Load();

	// シェーダー
	m_pShaderManager = CShaderManager::Create();

	// パーティクルの管理クラスの生成
	m_pParticleRegistry = CParticleRegistry::Create();

	// 初期モードの設定
	ChangeMode(std::make_unique<CTitle>());

	// フェードの生成
	m_pFade = CFade::Create();

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CManager::Uninit(void)
{
	// パーティクルの管理クラスの破棄
	m_pParticleRegistry.reset();

	// シェーダーのマネージャーの破棄
	if (m_pShaderManager != nullptr)
	{
		m_pShaderManager.reset();
	}

	// シーンの破棄
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		m_pScene = nullptr;
	}

	// カメラの破棄
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		m_pCamera.reset();
	}

	// ライトの破棄
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		m_pLight.reset();
	}

	// レンダラーの破棄
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		m_pRenderer.reset();
	}

	// テクスチャMRTマネージャの破棄
	if (m_pTextureMRTManager != nullptr)
	{
		m_pTextureMRTManager->Release();
		m_pTextureMRTManager.reset();
	}

	// テクスチャマネージャの破棄
	if (m_pTextureManager != nullptr)
	{
		m_pTextureManager->UnLoad();
		m_pTextureManager.reset();
	}

	// モデルマネージャの破棄
	if (m_pModelManager != nullptr)
	{
		m_pModelManager->UnLoad();
		m_pModelManager.reset();
	}

	// シーンの破棄
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		m_pScene.reset();
	}

	// フェードの破棄
	if (m_pFade != nullptr)
	{
		m_pFade->Uninit();
		m_pFade.reset();
	}

	// キーボードの破棄
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Uninit();
		m_pKeyboard.reset();
	}

	// パッドの破棄
	if (m_pJoypad != nullptr)
	{
		m_pJoypad->Uninit();
		m_pJoypad.reset();
	}

	// マウスの破棄
	if (m_pMouse != nullptr)
	{
		m_pMouse->Uninit();
		m_pMouse.reset();
	}

	// 音の破棄
	//if (m_pSound != nullptr)
	//{
	//	m_pSound->Uninit();
	//	m_pSound.reset();
	//}

	// 自分自身の破棄
	if (m_pInstance != nullptr)
	{
		// 破棄
		m_pInstance.reset();
	}
}

//===================================================
// 更新処理
//===================================================
void CManager::Update(void)
{
	// カメラの更新
	if (m_pCamera != nullptr)
	{
		m_pCamera->Update();
	}

	// ライトの更新
	if (m_pLight != nullptr)
	{
		m_pLight->Update();
	}

	// キーボードの更新処理
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Update();
	}

	// パッドの更新処理
	if (m_pJoypad != nullptr)
	{
		m_pJoypad->Update();
	}

	// マウスの更新処理
	if (m_pMouse != nullptr)
	{
		m_pMouse->Update();
	}

	// シーンの更新処理
	if (m_pScene != nullptr)
	{
		m_pScene->Update();
	}

	// レンダラーの更新処理
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Update();
	}

	// フェードの更新
	if (m_pFade != nullptr)
	{
		m_pFade->Update();
	}

	// 物理世界の更新
	if (m_pPhysicsWorld)
	{
		float dt = 1.0f / 60.0f;

		// 物理シミュレーション
		m_pPhysicsWorld->StepSimulation(dt);

		// 衝突イベント処理
		m_pPhysicsWorld->ProcessCollisionEvents();
	}

#ifdef _DEBUG

	// ワイヤーフレームの状態
	static bool bWireFrame = false;

	if (m_pKeyboard->GetTrigger(DIK_F6))
	{
		bWireFrame = bWireFrame ? false : true;
	}

	if (bWireFrame == true)
	{
		m_pRenderer->onWireFrame();
	}
	else
	{
		m_pRenderer->offWireFrame();
	}
#endif // _DEBUG
}

//===================================================
// 描画処理
//===================================================
void CManager::Draw(void)
{
	// レンダラーの描画処理
	if (m_pRenderer != nullptr)
	{
		// シーンの描画
		m_pRenderer->Draw(m_nFps, m_pScene.get(), m_pFade.get());
	}
}

