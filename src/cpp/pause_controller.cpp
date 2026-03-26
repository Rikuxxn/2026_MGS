//===================================================
//
// ポーズの処理 [pause_controller.cpp]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "pause_controller.h"
#include "input_system.h"
#include "menu_command_base.h"
#include "utility_math.h"
#include "object2d.h"

//***************************************************
// 定数宣言
//***************************************************
namespace PauseConst
{
	const char* PAUSE_TEXTURE[CPauseController::MENU_MAX] =
	{
		"pause.png",
		"pause.png",
		"pause.png",
	};
}
//===================================================
// コンストラクタ
//===================================================
CPauseController::CPauseController() : 
	m_bPause(false),
	m_apMenuCommand(),
	m_apPolygon(),
	m_currentMenu(MENU_CONTINUE)
{
}

//===================================================
// デストラクタ
//===================================================
CPauseController::~CPauseController()
{
	// メニューのコマンドの破棄
	for (auto& command : m_apMenuCommand)
	{
		command.reset();
	}
}

//===================================================
// 生成処理
//===================================================
std::unique_ptr<CPauseController> CPauseController::Create(void)
{
	auto pInstance = std::make_unique<CPauseController>();

	// 初期化に失敗したら
	if (FAILED(pInstance->Init()))
	{
		pInstance = nullptr;
	}
	return pInstance;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CPauseController::Init(void)
{
	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		m_apPolygon[nCnt] = CObject2D::Create(
			{ 640.0f,360.0f + nCnt * 100.0f,0.0f },
			{ 50.0f,50.0f },
			Const::WHITE,
			PauseConst::PAUSE_TEXTURE[nCnt]);

		// 種類の設定
		m_apPolygon[nCnt]->SetType(CObject::TYPE_PAUSE);
	}
	return S_OK;
}

//===================================================
// 更新処理
//===================================================
void CPauseController::Update(void)
{
	// ポーズの切り替えキーが押されたら
	if (InputSystem::EnablePause())
	{
		m_bPause = m_bPause ? false : true;
	}

	// ポーズ状態じゃないなら処理しない
	if(m_bPause == false)
	{
		return;
	}

	// 決定ボタンが押されたら
	if (m_apMenuCommand[m_currentMenu] != nullptr && InputSystem::PauseMenuDecision())
	{
		// 処理の実行
		m_apMenuCommand[m_currentMenu]->Execute();
	}

	// メニューの選択上
	if (InputSystem::PauseMenuSelectUp())
	{
		m_currentMenu = static_cast<MENU>((m_currentMenu + 1) % MENU_MAX);
	}
	// メニューの選択下
	else if (InputSystem::PauseMenuSelectDown())
	{
		m_currentMenu = static_cast<MENU>(m_currentMenu - 1);

		m_currentMenu = math::Wrap(m_currentMenu, MENU_CONTINUE, MENU_MAX);
	}
}