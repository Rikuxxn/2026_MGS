//===================================================
//
// 分と秒のタイマーの処理 [minute_second_timer.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "minute_second_timer.h"
#include "number.h"

//===================================================
// コンストラクタ
//===================================================
CMinuteSecondTimer::CMinuteSecondTimer() : 
	CObject(PRIORITY_UI_FRONT),
	m_apNumberMinute(),
	m_apNumberSecond(),
	m_pos(Const::VEC3_NULL),
	m_size(Const::VEC2_NULL),
	m_texturePath(),
	m_nMinute(0),
	m_nSecond(0),
	m_nFrameCounter(0),
	m_nTime(0)
{
}

//===================================================
// デストラクタ
//===================================================
CMinuteSecondTimer::~CMinuteSecondTimer()
{
	// 要素分調べる
	for (auto& number : m_apNumberMinute)
	{
		// 終了処理
		if (number != nullptr)
		{
			number->Uninit();
			number = nullptr;
		}
	}

	// 要素分調べる
	for (auto& number : m_apNumberSecond)
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
CMinuteSecondTimer* CMinuteSecondTimer::Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const int nTime, const char* pTexturePath)
{
	// 自分自身の生成
	CMinuteSecondTimer* pInstance = new CMinuteSecondTimer;

	pInstance->m_pos = pos;
	pInstance->m_size = size;
	pInstance->m_nTime = nTime;
	pInstance->m_texturePath = pTexturePath;

	pInstance->SetMinute(nTime);
	pInstance->SetSecond(nTime);

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
HRESULT CMinuteSecondTimer::Init(void)
{
	// 横幅の割合
	float fRateWidth = m_size.x / MAX_DIGIT;

	for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		// 分のメモリの確保
		if (m_apNumberMinute[nCnt] == nullptr)
		{
			// 数字の生成
			CNumber *pNumber = CNumber::Create(
				D3DXVECTOR3(m_pos.x - (fRateWidth * 2.0f * nCnt), m_pos.y, 0.0f),
				D3DXVECTOR2(fRateWidth, m_size.y),
				Const::WHITE,
				m_texturePath.c_str());

			m_apNumberMinute[nCnt].reset(pNumber);
		}
	}

	// オフセットの設定
	float offPosX = fRateWidth * MAX_DIGIT;

	//// コロンの作成
	//auto pCoron = CObject2D::Create(fRateWidth * 0.4f, m_fHeight * 0.75f, D3DXVECTOR3(offPosX + m_pos.x, m_pos.y, 0.0f));
	//pCoron->SetTextureID(CORON_TEXTURE);

	offPosX = fRateWidth * (3.0f * MAX_DIGIT);

	for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		// 秒のメモリの確保
		if (m_apNumberSecond[nCnt] == nullptr)
		{
			// 数字の生成
			CNumber* pNumber = CNumber::Create(
				D3DXVECTOR3(offPosX + m_pos.x - (fRateWidth * 2.0f * nCnt), m_pos.y, 0.0f),
				D3DXVECTOR2(fRateWidth, m_size.y),
				Const::WHITE,
				m_texturePath.c_str());

			m_apNumberSecond[nCnt].reset(pNumber);
		}
	}

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CMinuteSecondTimer::Uninit(void)
{
	// 自分自身の破棄
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CMinuteSecondTimer::Update(void)
{
	// 1フレーム
	if (m_nFrameCounter >= Const::FRAME)
	{
		m_nFrameCounter = 0;

		// 秒が0じゃなかったら
		if (m_nSecond > 0)
		{
			// 秒を減らす
			m_nSecond--;
		}
		// 分が0じゃなかったら
		else if (m_nSecond <= 0 && m_nMinute != 0)
		{
			// もう一周する
			m_nSecond = Const::FRAME - 1;

			// 分を減らす
			m_nMinute--;
		}
	}

	m_nFrameCounter++;

	// 60を超えたら
	if (m_nSecond >= Const::FRAME)
	{
		m_nMinute++; // 分を1増やす

		// 余りが0じゃなかったら(60より上だったら)
		if (m_nSecond % Const::FRAME != 0)
		{
			int Time = m_nSecond % Const::FRAME; // 余りを求める

			m_nSecond = 0; // 0にする
			m_nSecond += Time; // 余りを加算する
		}
		else
		{
			m_nSecond = 0; // 0にする
		}
	}

	// 桁数分回す
	for (int nCnt = 0; nCnt < MAX_DIGIT; nCnt++)
	{
		if (m_apNumberMinute[nCnt] != nullptr)
		{
			// UV座標の設定
			SetUV(m_apNumberMinute[nCnt].get(), m_nMinute, nCnt);
		}
		if (m_apNumberSecond[nCnt] != nullptr)
		{
			// UV座標の設定
			SetUV(m_apNumberSecond[nCnt].get(), m_nSecond, nCnt);
		}
	}
}

//===================================================
// 描画処理
//===================================================
void CMinuteSecondTimer::Draw(void)
{
	// 要素分調べる
	for (auto& number : m_apNumberMinute)
	{
		// 描画処理
		if (number != nullptr)
		{
			number->Draw();
		}
	}

	// 要素分調べる
	for (auto& number : m_apNumberSecond)
	{
		// 描画処理
		if (number != nullptr)
		{
			number->Draw();
		}
	}
}

//===================================================
// UVの設定処理
//===================================================
void CMinuteSecondTimer::SetUV(CNumber* pNumber, const int nValue, const int nCnt)
{
	// ナンバーオブジェクトの更新
	if (pNumber != nullptr)
	{
		pNumber->Update();

		// スコアを割る値
		int nData = 10;

		// 桁に応じた値を求める
		int nDivi = (int)pow((double)nData, (double)nCnt);

		// テクスチャ座標を求める
		int nNumber = nValue / nDivi % 10;

		pNumber->SetUV(nNumber);
	}
}