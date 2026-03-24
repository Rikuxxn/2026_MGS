//===================================================
//
// クジラの処理をするクラス [whole.cpp]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "whole.h"
#include "character.h"
#include "motion.h"

//===================================================
// コンストラクタ
//===================================================
CWhole::CWhole() : 
	CObject(PRIORITY_CHARACTER),
	m_pCharacter(nullptr)
{
}

//===================================================
// デストラクタ
//===================================================
CWhole::~CWhole()
{
	// キャラクタークラスの破棄
	if (m_pCharacter != nullptr)
	{
		m_pCharacter->Uninit();
		m_pCharacter = nullptr;
	}
}

//===================================================
// 生成処理
//===================================================
CWhole* CWhole::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pMotionFilePath)
{
	// 自分自身の生成
	CWhole* pInstance = new CWhole;

	// キャラクターの生成
	pInstance->m_pCharacter = CCharacter::Create(pos, rot, pMotionFilePath);

	// 初期化処理に失敗したら
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
HRESULT CWhole::Init(void)
{
	// モーションの取得
	CMotion *pMotion = m_pCharacter->GetMotion();

	pMotion->Play(MOTIONTYPE_EAT,true,10);

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CWhole::Uninit(void)
{
	// 自分自身の破棄
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CWhole::Update(void)
{
	if (m_pCharacter != nullptr)
	{
		// 更新処理
		m_pCharacter->Update();
	}
}

//===================================================
// 描画処理
//===================================================
void CWhole::Draw(void)
{
	if (m_pCharacter != nullptr)
	{
		// 描画処理
		m_pCharacter->Draw();
	}
}