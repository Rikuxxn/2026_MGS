//===================================================
//
// プランクトンを操作し、管理するクラス [plankton_controller.cpp]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "plankton_controller.h"
#include "plankton.h"
#include "player.h"
#include "json_loader.h"
#include "input_system.h"
#include "MathConst.h"

//===================================================
// コンストラクタ
//===================================================
CPlanktonController::CPlanktonController() : 
	m_pPlayer(nullptr)
{
}

//===================================================
// デストラクタ
//===================================================
CPlanktonController::~CPlanktonController()
{
	m_pList.clear();
}

//===================================================
// 生成処理
//===================================================
std::unique_ptr<CPlanktonController> CPlanktonController::Create(CPlayer* pPlayer)
{
	// 自分自身の生成
	auto pInstance = std::make_unique<CPlanktonController>();

	// 要素の設定
	pInstance->m_pPlayer = pPlayer;

	// 初期化処理
	if (FAILED(pInstance->Init()))
	{
		pInstance = nullptr;
		return nullptr;
	}

	return pInstance;
}

//===================================================
// プランクトンの生成処理
//===================================================
HRESULT CPlanktonController::CreatePlankton(const D3DXVECTOR3& pos, const D3DXVECTOR2& size)
{
	// プランクトンの生成処理
	CPlankton *pPlankton = CPlankton::Create(pos, size);

	if (m_pPlayer == nullptr)
	{
		return E_FAIL;
	}

	m_pList.push_back(pPlankton);

	return S_OK;
}

//===================================================
// プランクトン塊生成処理
//===================================================
void CPlanktonController::CreateCluster(const D3DXVECTOR3& center, int count, float radiusMax)
{
	for (int nCnt = 0; nCnt < count; nCnt++)
	{
		// 0.0～1.0 の乱数
		float r = (rand() % CMathConstant::I_RAND_NUM) / CMathConstant::F_RAND_NUM;

		// 平方根で均一分布
		float radius = sqrtf(r) * radiusMax;

		float angle = ((rand() % CMathConstant::I_ANGLE_MAX) / CMathConstant::F_ANGLE_HALF) * D3DX_PI;

		D3DXVECTOR3 pos;
		pos.x = center.x + cosf(angle) * radius;
		pos.z = center.z + sinf(angle) * radius;
		pos.y = center.y;

		// プランクトンの生成
		CreatePlankton(pos, { 10.0f, 10.0f });
	}
}
//===================================================
// 更新処理
//===================================================
void CPlanktonController::Update(void)
{
#ifdef _DEBUG

	if (m_pPlayer != nullptr)
	{
		// プレイヤーの位置の取得
		D3DXVECTOR3 playerPos = m_pPlayer->GetPosition();

		if (InputSystem::EditRegisterPlanktonPos())
		{
			CEffect::Info info;

			info.unFlag = CEffect::FLAG_NO_LIFE;
			info.move = Const::VEC3_NULL;
			info.nLife = 999;
			
			CEffect::Create(info, playerPos, { 25.0f,25.0f }, Const::WHITE, "effect000.jpg");
			// 位置の保存
			m_edit.vPlanktonPos.push_back(playerPos);
		}
		if (InputSystem::EditSavePlanktonPos())
		{
			SavePlankton();
		}
	}
#endif // _DEBUG

	//if (m_pPlayer == nullptr)
	//{
	//	return;
	//}

	//// 位置の取得
	//D3DXVECTOR3 pos = m_pPlayer->GetPosition();

	//// 要素分回す
	//for (auto& list : m_pList)
	//{
	//	// 追従状態じゃないなら
	//	if (list->GetFollowState() == false)
	//	{
	//		continue;
	//	}

	//	// 位置の設定
	//	list->GetFollowPosition(pos);
	//}
}

//===================================================
// プレイヤーのプランクトンのリストの登録処理
//===================================================
void CPlanktonController::RegisterPlayerPlanktonList(CPlankton* pPlankton)
{
	if (m_pPlayer != nullptr)
	{
		// プランクトンの登録
		m_pPlayer->RegisterPlankton(pPlankton);
	}
}

//===================================================
// 初期化処理
//===================================================
HRESULT CPlanktonController::Init(void)
{
	nlohmann::json config;

	// 読み込み処理
	if (FAILED(JsonLoader::Load("plankton.json", config)))
	{
		return E_FAIL;
	}

	// 取得した分回す
	for (const auto& item : config["planktons"])
	{
		D3DXVECTOR3 pos;
		pos.x = item["pos"][0];
		pos.y = item["pos"][1];
		pos.z = item["pos"][2];

		// 生成処理
		CreatePlankton(pos, { 10.0f,10.0f });
	}

	// プランクトンの塊の中心位置
	m_clusterCenters = 
	{
		{0, 0, 0},
		//{100, 0, 50},
		//{-80, 0, 120},
		//{50, 0, -100}
	};

	// プランクトンの塊生成
	for (auto& center : m_clusterCenters)
	{
		CreateCluster(center, 20, 100.0f); // 20匹、半径50
	}

	return S_OK;
}

//===================================================
// プランクトンの位置のセーブ
//===================================================
HRESULT CPlanktonController::SavePlankton(void)
{
	nlohmann::json saveConfig;

	// 配列の確保
	saveConfig["planktons"] = nlohmann::json::array();

	// 要素分回す
	for (const auto& position : m_edit.vPlanktonPos)
	{
		nlohmann::json item;

		item["pos"] = { position.x,position.y,position.z };

		saveConfig["planktons"].push_back(item);
	}

	std::ofstream file("data/SYSTEM/plankton_save.json");

	if (!file.is_open())
	{
		MessageBox(NULL, "エラー", "data/SYSTEM/plankton_save.json", MB_OK);
		return E_FAIL;
	}

	file << saveConfig.dump(4);

	file.clear();
	file.close();

	return S_OK;
}
