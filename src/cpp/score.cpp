//===================================================
//
// スコアのクラス [score.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "score.h"
#include "number.h"
#include "utility_math.h"

//===================================================
// コンストラクタ
//===================================================
CScore::CScore() :
	CObject(PRIORITY_UI_FRONT),
	m_apNumber(),
	m_pos(Const::VEC3_NULL),
	m_size(Const::VEC2_NULL),
	m_texturePath()
{
}

//===================================================
// デストラクタ
//===================================================
CScore::~CScore()
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
CScore* CScore::Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const int nScore, const char* pTexturePath)
{
	// スコアの生成
	CScore* pInstance = new CScore;

	pInstance->m_pos = pos;
	pInstance->m_size = size;
	pInstance->m_nScore = nScore;
	pInstance->m_texturePath = pTexturePath;

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
HRESULT CScore::Init(void)
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
void CScore::Uninit(void)
{
	// 自分自身の破棄
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CScore::Update(void)
{
	// スコアの制限
	m_nScore = math::Clamp(m_nScore, MIN_SCORE, MAX_SCORE);

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
			int nNumber = m_nScore / nDivi % 10;

			m_apNumber[nCnt]->SetUV(nNumber);
		}
	}
}

//===================================================
// 描画処理
//===================================================
void CScore::Draw(void)
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