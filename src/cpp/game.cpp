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
#include "effect.h"

//===================================================
// コンストラクタ
//===================================================
CGame::CGame() :
	CScene(CScene::MODE_GAME)
{
}

//===================================================
// デストラクタ
//===================================================
CGame::~CGame()
{
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

	pCamera->SetCamera(
		{ 0.0f,0.0f,-300.0f },
		Const::VEC3_NULL,
		{1.68f,0.0f,0.0f});

	CObject2D::Create(
		Const::CENTER_POS_2D,
		{ 80.0f,50.0f },
		Const::WHITE,
		"test.png",
		0.5f);

	CObject3D::Create(
		{ 0.0f,0.0f,0.0f },
		{ 50.0f,50.0f,0.0f },
		Const::VEC3_NULL,
		Const::WHITE,
		"test.png");

	CObjectBillboard::Create(
		{ 0.0f,100.0f,0.0f },
		{ 50.0f,50.0f },
		Const::WHITE,
		"test.png");

	CMeshField::Create(
		{ 0.0f,0.0f,0.0f },
		{ 300.0f,300.0f },
		Const::WHITE,
		{ 1,1 },
		"");

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

	CPlayer::Create(
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		"motion.txt"
	);

	CPlayer::Create(
		{ 0.0f,0.0f,-100.0f },
		{ 0.0f,0.0f,0.0f },
		"motion.txt"
	);

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
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_0))
	{
		CPlayer::Create(
			{ 0.0f,0.0f,-100.0f },
			{ 0.0f,0.0f,0.0f },
			"motion.txt"
		);
	}

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
}

//===================================================
// 描画処理
//===================================================
void CGame::Draw(void)
{

}