//===================================================
//
// リザルトシーン [result.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "result.h"

#include "input.h"
#include "manager.h"
#include "game.h"
#include "object2d.h"
#include "camera.h"
#include "mesh_field.h"
#include "skycube.h"
#include "objectx.h"
#include "score.h"

//===================================================
// コンストラクタ
//===================================================
CResult::CResult() :
	CScene(CScene::MODE_RESULT)
{
	std::ifstream file("data/score.txt");

	std::string line;
	std::string score;

	std::getline(file, line);
	std::istringstream iss(line);
	iss >> m_nScore;
	file.close();
}

//===================================================
// デストラクタ
//===================================================
CResult::~CResult()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT CResult::Init(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// カメラの取得
	CCamera* pCamera = pManager->GetCamera();

	pCamera->SetCamera(
		{ -420.0f,230.0f,130.0f },
		{ 0.0f,250.0f,120.0f },
		{ 1.5f, 1.60f, 0.0f });

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

	CScore::Create(
		{ 240.0f,650.0f,0.0f },
		{ 300.0f,350.0f },
		82,
		"number008.png");


	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CResult::Uninit(void)
{
}

//===================================================
// 更新処理
//===================================================
void CResult::Update(void)
{
}

//===================================================
// 描画処理
//===================================================
void CResult::Draw(void)
{

}