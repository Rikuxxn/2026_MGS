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
CPlankton::CPlankton()
{
}

//===================================================
// デストラクタ
//===================================================
CPlankton::~CPlankton()
{
}

//===================================================
// 生成処理
//===================================================
CPlankton* CPlankton::Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size)
{
	// 自分自身の生成
	CPlankton* pInstance = new CPlankton;

	// 大きさ
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
}

//===================================================
// 描画処理
//===================================================
void CPlankton::Draw(void)
{
	// 描画処理
	CObjectBillboard::Draw();
}