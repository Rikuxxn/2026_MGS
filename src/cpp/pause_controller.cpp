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
#include "pause_menu_command.h"

//***************************************************
// 定数宣言
//***************************************************
namespace PauseConst
{
	const char* PAUSE_TEXTURE[CPauseController::MENU_MAX] =
	{
		"pauseContinue.png",
		"pauseRetry.png",
		"pauseTitle.png",
	};

	constexpr float SIN_TIME = 0.1f;	// 点滅の時間間隔
	constexpr float ALPHA_OFFSET = 0.5f;	// アルファ値のオフセット
	const D3DXVECTOR3 POS = { 640.0f,260.0f,0.0f };	// 生成位置
	const D3DXVECTOR2 SIZE = { 100.0f,80.0f };	// 生成位置
}
//===================================================
// コンストラクタ
//===================================================
CPauseController::CPauseController() : 
	m_bPause(false),
	m_apMenuCommand(),
	m_apPolygon(),
	m_fSinCurve(0.0f),
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
			{ PauseConst::POS.x,PauseConst::POS.y + nCnt * (PauseConst::SIZE.y * 2.0f),0.0f },
			PauseConst::SIZE,
			Const::WHITE,
			PauseConst::PAUSE_TEXTURE[nCnt]);

		// 種類の設定
		m_apPolygon[nCnt]->SetType(CObject::TYPE_PAUSE);
	}

	// メニュー生成
	m_apMenuCommand[MENU_CONTINUE]	= std::make_unique<CPauseMenuContinue>();
	m_apMenuCommand[MENU_RETRY]		= std::make_unique<CPauseMenuRetry>();
	m_apMenuCommand[MENU_QUIT]		= std::make_unique<CPauseMenuQuit>();

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
		m_currentMenu = static_cast<MENU>(m_currentMenu - 1);

		m_currentMenu = math::Wrap(m_currentMenu, MENU_CONTINUE, MENU_QUIT);
	}
	// メニューの選択下
	else if (InputSystem::PauseMenuSelectDown())
	{
		m_currentMenu = static_cast<MENU>((m_currentMenu + 1) % MENU_MAX);
	}

	// ポリゴンの更新処理
	UpdatePolygon();
}

//===================================================
// ポリゴンの更新処理
//===================================================
void CPauseController::UpdatePolygon(void)
{
	// メニュー分回す
	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		if (m_currentMenu == nCnt)
		{
			m_fSinCurve += PauseConst::SIN_TIME;

			// アルファ値の取得
			D3DXCOLOR col = m_apPolygon[nCnt]->GetColor();

			// 透明度
			col.a = (1.0f - fabsf(sinf(m_fSinCurve)) * PauseConst::ALPHA_OFFSET);

			m_apPolygon[nCnt]->SetColor(col);
		}
		else
		{
			m_apPolygon[nCnt]->SetColor(Const::WHITE);
		}
	}
}