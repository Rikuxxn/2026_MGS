//===================================================
//
// タイマーの処理 [timer.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "timer.h"
#include "number.h"
#include "utility_math.h"

//===================================================
// コンストラクタ
//===================================================
CTimer::CTimer() : 
	m_apNumber(),
	m_pos(Const::VEC3_NULL),
	m_size(Const::VEC2_NULL),
	m_nTime(0),
	m_nFrameCounter(0)
{
}

//===================================================
// デストラクタ
//===================================================
CTimer::~CTimer()
{
	// 要素分調べる
	for (auto& number : m_apNumber)
	{
		// 終了処理
		if (number != nullptr)
		{
			number->Uninit();
			number = nullptr;
		}
	}
}

//===================================================
// 生成処理
//===================================================
CTimer* CTimer::Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const int nTime, const char* pTexturePath)
{
	// 自分自身の生成
	CTimer* pInstance = new CTimer;
	
	pInstance->m_pos = pos;
	pInstance->m_size = size;
	pInstance->m_nTime = nTime;
	pInstance->m_texturePath = pTexturePath;

	// 初期化処理に失敗したら
	if (FAILED(pInstance->Init()))
	{
		// 終了処理
		pInstance->Uninit();
		pInstance = nullptr;
		return pInstance;
	}

	return pInstance;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CTimer::Init(void)
{
	// 横幅の割合を求める
	float fWidth = m_size.x / MAX_DIGIT;

	// オフセット
	float offPosX = fWidth * 2.0f;

	for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		// ナンバーのメモリの確保
		if (m_apNumber[nCnt] == nullptr)
		{
			// スコアの生成
			CNumber* pNumber = CNumber::Create(D3DXVECTOR3(m_pos.x - (offPosX * nCnt), m_pos.y, 0.0f), D3DXVECTOR2(fWidth, m_size.y), Const::WHITE, "");
			m_apNumber[nCnt].reset(pNumber);

			// テクスチャのIDの設定
			m_apNumber[nCnt]->SetTextureID(m_texturePath.c_str());
		}
	}

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CTimer::Uninit(void)
{
	// 自分自身の破棄
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CTimer::Update(void)
{
	// 1フレーム
	if (m_nFrameCounter >= Const::FRAME)
	{
		m_nFrameCounter = 0;

		// 秒が0じゃなかったら
		if (m_nTime > 0)
		{
			// 秒を減らす
			m_nTime--;
		}
	}

	// 範囲の制限
	m_nTime = math::Clamp(m_nTime, 0, 999);

	m_nFrameCounter++;

	for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		// ナンバーオブジェクトの更新
		if (m_apNumber[nCnt] != nullptr)
		{
			m_apNumber[nCnt]->Update();

			// スコアを割る値
			int nData = 10;

			// 桁に応じた値を求める
			int nDivi = (int)pow((double)nData, (double)nCnt);

			// テクスチャ座標を求める
			int nNumber = m_nTime / nDivi % 10;

			m_apNumber[nCnt]->SetUV(nNumber);
		}
	}
}

//===================================================
// 描画処理
//===================================================
void CTimer::Draw(void)
{
	// 要素分調べる
	for (auto& number : m_apNumber)
	{
		// 描画処理
		if (number != nullptr)
		{
			number->Draw();
		}
	}
}