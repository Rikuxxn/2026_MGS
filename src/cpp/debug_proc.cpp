//===================================================
//
// デバッグフォント [debug_proc.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "debug_proc.h"
#include "manager.h"
#include"renderer.h"
#include <stdio.h>

//***************************************************
// 静的メンバ変数の宣言
//***************************************************
LPD3DXFONT CDebugProc::m_pFont = NULL;	// フォントへのポインタ
bool CDebugProc::m_bShow = true;		// 表示するかどうか
std::string CDebugProc::m_string = {};	// 文字列格納用バッファ

//===================================================
// コンストラクタ
//===================================================
CDebugProc::CDebugProc()
{
	m_pFont = NULL;
}

//===================================================
// デストラクタ
//===================================================
CDebugProc::~CDebugProc()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT CDebugProc::Init(void)
{
	CRenderer* pRenderer = nullptr;

	pRenderer = CManager::GetInstance()->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// デバッグ表示用のフォントを設定
	if (FAILED(D3DXCreateFont(pDevice, 18, 0, 0, 0,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal", &m_pFont)))
	{
		return E_FAIL;
	}

	// 要素をクリア
	m_string.clear();

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CDebugProc::Uninit(void)
{
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}

	m_string.clear();
}

//===================================================
// デバッグ表示処理
//===================================================
void CDebugProc::Print(const char* fmt ...)
{
#ifdef _DEBUG
	char buffer[Const::MAX_WORD] = {};

	// 可変引数を処理
	va_list args;

	// 可変引数の処理を始める
	va_start(args, fmt);

	// フォーマット済みの文字列をastringに保管
	vsnprintf(buffer, sizeof(buffer), fmt, args);

	// 文字列を追加
	m_string += buffer;

	// 可変引数の処理を終了
	va_end(args);
#else
	(void)fmt;
#endif // _DEBUG
}

//===================================================
// 描画処理
//===================================================
void CDebugProc::Draw(void)
{
#ifdef _DEBUG
	// デバッグ表示がONだったら
	if (m_bShow == true)
	{
		RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

		m_pFont->DrawText(NULL, &m_string[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255,255,255,255));
	}

	// 要素をクリア
	m_string.clear();
#endif // _DEBUG
}
