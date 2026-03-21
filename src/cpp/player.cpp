//===================================================
//
// プレイヤーの処理 [player.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "player.h"
#include "character.h"

//===================================================
// コンストラクタ
//===================================================
CPlayer::CPlayer() : 
	m_pCharacter(nullptr)
{
}

//===================================================
// デストラクタ
//===================================================
CPlayer::~CPlayer()
{
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Uninit();
		m_pCharacter.reset();
	}
}

//===================================================
// 生成処理
//===================================================
CPlayer* CPlayer::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pMotionFilePath)
{
	// 自分自身の生成
	CPlayer* pInstance = new CPlayer;

	// キャラクターの生成
	pInstance->m_pCharacter = CCharacter::Create(pos, rot, pMotionFilePath);

	// 初期化処理に失敗したら
	if (FAILED(pInstance->Init()))
	{
		pInstance->Uninit();
		pInstance = nullptr;
		return nullptr;
	}

	return pInstance;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CPlayer::Init(void)
{
	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CPlayer::Uninit(void)
{
	// 自分自身の破棄
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CPlayer::Update(void)
{
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Update();
	}
}

//===================================================
// 描画処理
//===================================================
void CPlayer::Draw(void)
{
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Draw();
	}
}