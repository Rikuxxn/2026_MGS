//===================================================
//
// タイトルシーン [title.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "title.h"

#include "input.h"
#include "manager.h"
#include "game.h"
#include "object2d.h"
#include "camera.h"
#include "mesh_field.h"
#include "skycube.h"
#include "objectx.h"

//===================================================
// コンストラクタ
//===================================================
CTitle::CTitle() : 
	CScene(CScene::MODE_TITLE)
{
}

//===================================================
// デストラクタ
//===================================================
CTitle::~CTitle()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT CTitle::Init(void)
{

	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// カメラの取得
	CCamera* pCamera = pManager->GetCamera();

	pCamera->SetCamera(
		{ -420.0f,230.0f,130.0f },
		{ 0.0f,250.0f,120.0f },
		{ 1.5f, 1.60f, 0.0f });

	// タイトル
	CObject2D::Create(
		Const::CENTER_POS_2D,
		{ 600.0f,150.0f },
		Const::WHITE,
		"title.png",
		0.0f);

	// 海底
	CObjectX::Create(
		"data/MODEL/seabed01.x",
		{ 0.0f,-500.0f,0.0f },
		Const::VEC3_NULL,
		{ 5.0f,5.0f,5.0f });

	// 空の生成
	CSkyCube::Create();

	CMeshField::Create(
		{ 0.0f,0.0f,0.0f },
		{ 3500.0f, 3500.0f },
		Const::WHITE,
		{ 120,120 },
		"sea.jpg");

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CTitle::Uninit(void)
{
}

//===================================================
// 更新処理
//===================================================
void CTitle::Update(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// キーボードの取得
	CInputKeyboard* pKeyboard = pManager->GetInputKeyboard();

	if (pKeyboard->GetTrigger(DIK_F9))
	{
		pManager->BeginFade(std::make_unique<CGame>());
	}

#ifdef _DEBUG
#endif // _DEBUG
}

//===================================================
// 描画処理
//===================================================
void CTitle::Draw(void)
{

}