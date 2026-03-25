//===================================================
//
// ゲームシーン [game.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "game.h"
#include "object2d.h"
#include "object3d.h"
#include "object_billboard.h"
#include "ranking.h"
#include "SkyCube.h"
#include "follow_camera.h"
#include "plankton.h"
#include "whole.h"
#include "plankton_controller.h"

#include "camera.h"
#include "manager.h"
#include "mesh_field.h"
#include "mesh_cylinder.h"
#include "mesh_circle.h"
#include "player.h"
#include "input.h"
#include "manager.h"
#include "score.h"
#include "timer.h"
#include "minute_second_timer.h"
#include "particle.h"
#include "collision_system.h"
#include "mesh_field_collision_system.h"
#include "BlockManager.h"
#include "texture_animation.h"
#include "Sound.h"

//===================================================
// コンストラクタ
//===================================================
CGame::CGame() :
	CScene(CScene::MODE_GAME),
	m_vpCollisionSystem(),
	m_pPlanktonController()
{
}

//===================================================
// デストラクタ
//===================================================
CGame::~CGame()
{
	// 要素分調べる
	for (auto& collisionSystem : m_vpCollisionSystem)
	{
		// 破棄
		collisionSystem = nullptr;
	}

	// プランクトンの操作クラスの破棄
	if (m_pPlanktonController != nullptr)
	{
		m_pPlanktonController.reset();
	}

	m_vpCollisionSystem.clear();
}

//===================================================
// 初期化処理
//===================================================
HRESULT CGame::Init(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// カメラの取得
	CCamera* pCamera = pManager->GetCamera();

	//// ブロックマネージャーの取得
	//CBlockManager* pBlockManager = pManager->GetBlockManager();

	//// ステージの読み込み
	//pBlockManager->LoadFromJson("data/STAGE/test.json");

	pCamera->SetCamera(
		{ 0.0f,0.0f,-300.0f },
		Const::VEC3_NULL,
		{ 1.68f, 0.0f, 0.0f });

	CPlayer* pPlayer = CPlayer::Create(
		{ 0.0f,100.0f,0.0f },
		{ 0.0f,0.0f,0.0f },
		"motion_dolphin.txt"
	);

	// プランクトンの操作クラスの生成
	m_pPlanktonController = CPlanktonController::Create(pPlayer);

	// カメラ追従処理の追加
	pCamera->AddSystem(std::make_unique<CFollowCamera>(pCamera, pPlayer));

	CMeshField* pMeshField = CMeshField::Create(
		{ 0.0f,0.0f,0.0f },
		{ 3500.0f, 3500.0f },
		Const::WHITE,
		{ 120,120 },
		"sea.jpg");

	// 空の生成
	CSkyCube::Create();

	// プランクトンの生成処理
	m_pPlanktonController->CreatePlankton(
		{ 150.0f,10.0f,0.0f },
		{ 50.0f,50.0f });

	//// プランクトンの生成
	//CPlankton::Create(
	//	{ 80.0f,10.0f,0.0f }, 
	//	{ 50.0f,50.0f });

	// クジラの生成
	CWhole::Create(
		{ -160.0f,10.0f,0.0f },
		{ 0.0f,0.0f,0.0f },
		"data/MOTION/motion_whole.txt");

#if 0
	CObject2D::Create(
		Const::CENTER_POS_2D,
		{ 80.0f,50.0f },
		Const::WHITE,
		"test.png",
		0.5f);

	CObject2D::Create(
		{ 800.0f,50.0f,0.0f},
		{ 80.0f,50.0f },
		Const::WHITE,
		"number001.png")
		->AttachAnimation(
			CTextureAnimation::TYPE_2D,
			{10,1},
			5,
			true);

	CObject3D::Create(
		{ 0.0f,100.0f,-200.0f },
		{ 50.0f,50.0f,0.0f },
		Const::VEC3_NULL,
		Const::WHITE,
		"number001.png")
		->AttachAnimation(
			CTextureAnimation::TYPE_3D,
			{ 10,1 },
			60,
			false);

	CObjectBillboard::Create(
		{ 0.0f,100.0f,0.0f },
		{ 50.0f,50.0f },
		Const::WHITE,
		"test.png");

	CMeshCylinder::Create(
		{ 0.0f,0.0f,0.0f },
		50.0f,
		50.0f,
		Const::WHITE,
		{ 8, 1 },
		"");

	CMeshCircle::Info info = {};

	info.col = { 1.0f,0.0f,0.0f,1.0f };
	info.fInHeight = 0.0f;
	info.fOutHeight = 0.0f;
	info.fInRadius = 0.0f;
	info.fOutRadius = 50.0f;
	info.fSpeed = 1.0f;
	info.nLife = 600;

	CMeshCircle::Create(
		{ 0.0f,2.0f,0.0f },
		Const::VEC3_NULL,
		"",
		32,
		info);

	CScore::Create(
		{ 640.0f,100.0f,0.0f },
		{ 120.0f,20.0f },
		4545,
		"number001.png");

	CTimer::Create(
		{ 640.0f,500.0f,0.0f },
		{ 120.0f,20.0f },
		120,
		"number001.png");

	CMinuteSecondTimer::Create(
		{ 240.0f,300.0f,0.0f },
		{ 120.0f,20.0f },
		120,
		"number001.png");
#endif
	// メッシュフィールドの判定処理
	auto pMeshFieldCollisionSystem = std::make_unique<CMeshFieldCollisionSystem>(pPlayer, pMeshField);

	// 当たり判定の追加
	m_vpCollisionSystem.push_back(std::move(pMeshFieldCollisionSystem));

	//// 音の取得
	//CSound* pSound = CManager::GetInstance()->GetSound();

	//// ゲームBGMの再生
	//if (pSound)
	//{
	//	pSound->Play(CSound::SOUND_LABEL_GAMEBGM);
	//}

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CGame::Uninit(void)
{
}

//===================================================
// 更新処理
//===================================================
void CGame::Update(void)
{
	// 要素分調べる
	for (auto& collisionSystem : m_vpCollisionSystem)
	{
		// 更新処理
		collisionSystem->Update();
	}

	// 更新処理
	if (m_pPlanktonController != nullptr)
	{
		m_pPlanktonController->Update();
	}
#ifdef _DEBUG
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_0))
	{
		CManager::GetInstance()->BeginFade(std::make_unique<CRanking>());
	}
#endif // _DEBUG


	CEffect::Info effectInfo;

	effectInfo.unFlag =
		CEffect::FLAG_GRAVITY |
		CEffect::FLAG_ALPHA_DECREASE |
		CEffect::FLAG_RADIUS_DECREASE |
		CEffect::FLAG_INERTIA;


	effectInfo.move = { 50.0f,1.0f,1.0f };
	effectInfo.nLife = 60;
	effectInfo.fGravity = 0.2f;
	effectInfo.fInertia = 0.9f;

	CEffect::Create(
		effectInfo,
		{ 0.0f,0.0f,0.0f },
		{ 10.0f,10.0f },
		Const::WHITE,
		"effect000.jpg");

	CParticle::Info particleInfo;

	// この地獄みてえな引数打てば操作できます
	particleInfo.pos = { 0.0f,0.0f,-200.0f };
	particleInfo.col = Const::WHITE;
	particleInfo.fAngleXMax = 90;
	particleInfo.fAngleXMin = -90;
	particleInfo.fAngleYMax = 60;
	particleInfo.fAngleYMin = -60;
	particleInfo.moveMax = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	particleInfo.moveMin = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	particleInfo.nNum = 5;
	particleInfo.nTime = 5;
	particleInfo.size = { 5.0f,5.0f };
	particleInfo.texturePath = "effect000.jpg";
	particleInfo.effectInfo.nLife = 20;
	
	CParticle::Create(particleInfo);
}

//===================================================
// 描画処理
//===================================================
void CGame::Draw(void)
{

}