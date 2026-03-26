//===================================================
//
// 魚の処理 [fish.cpp]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "fish.h"
#include "plankton.h"
#include "game.h"
#include "fish_controller.h"
#include "utility_math.h"
#include "plankton_controller.h"

//===================================================
// コンストラクタ
//===================================================
CFish::CFish() : 
	m_move(Const::VEC3_NULL),
	m_pHasPlankton(nullptr),
	m_nDeleteTime(120)
{
}

//===================================================
// デストラクタ
//===================================================
CFish::~CFish()
{
}

//===================================================
// 生成処理
//===================================================
CFish* CFish::Create(const D3DXVECTOR3& pos, const char* pModelFileName, const int nLife)
{
	CFish* pInstacne = new CFish;

	pInstacne->SetPos(pos);
	pInstacne->SetPath(pModelFileName);
	pInstacne->m_nLife = nLife;

	// 初期化処理
	if (FAILED(pInstacne->Init()))
	{
		pInstacne->Uninit();
		pInstacne = nullptr;
		return nullptr;
	}
	return pInstacne;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CFish::Init(void)
{
	// 初期化処理
	if (FAILED(CObjectX::Init()))
	{
		return E_FAIL;
	}

	float fScal = math::Random(1.3f, 4.0f);
	// 大きさの生成
	CObjectX::SetSize(D3DXVECTOR3(fScal, fScal, fScal));

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CFish::Uninit(void)
{
	CObjectX::Uninit();
}

//===================================================
// 更新処理
//===================================================
void CFish::Update(void)
{
	// 位置の取得
	D3DXVECTOR3 pos = CObjectX::GetPos();

	m_move.x += (0.0f - m_move.x) * 0.1f;
	m_move.y += (0.0f - m_move.x) * 0.1f;
	m_move.z += (0.0f - m_move.z) * 0.1f;

	// 位置の更新処理
	pos += m_move;

	m_nLife--;

	// 位置の設定
	CObjectX::SetPos(pos);

	CObjectX::Update();

	D3DXVECTOR3 rot = CObjectX::GetRot();

	rot.y = atan2f(m_move.x, m_move.z);

	CFishController* pFishController = CGame::GetFishController();

	// クジラの操作クラス
	CPlanktonController* pPlanktonController = CGame::GetPlanktonController();

	if (m_pHasPlankton != nullptr)
	{
		m_pHasPlankton->SetState(CPlankton::State::FishFollow);
		m_pHasPlankton->GetFollowPosition({ pos.x,pos.y + 20.0f,pos.z });

		m_nDeleteTime--;

		// 原点の方向
		D3DXVECTOR3 moveDir = math::GetVector(Const::VEC3_NULL, pos);

		m_move = moveDir * 2.0f;


		if (pFishController != nullptr && m_nDeleteTime <= 0)
		{
			m_pHasPlankton->Uninit();
			pPlanktonController->Erase(m_pHasPlankton);

			Uninit();
			pFishController->Erase(this);
			return;
		}
	}

	if (m_nLife <= 0)
	{
		if (m_pHasPlankton != nullptr)
		{
			m_pHasPlankton->Uninit();
			pPlanktonController->Erase(m_pHasPlankton);
		}
		Uninit();
		pFishController->Erase(this);
		return;
	}

	CObjectX::SetRot(rot);
}

//===================================================
// 描画処理
//===================================================
void CFish::Draw(void)
{
	CObjectX::Draw();
}

//===================================================
// 生きてるかどうか
//===================================================
bool CFish::GetALive(void) const
{
	return (m_nLife >= 0);
}

//===================================================
// プランクトンの取得
//===================================================
bool CFish::SetPlankton(CPlankton* pPlankton)
{
	if (m_pHasPlankton == nullptr)
	{
		m_pHasPlankton = pPlankton;
		return true;
	}

	return false;
}
