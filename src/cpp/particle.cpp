//===================================================
//
// パーティクル [particle.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "particle.h"
#include "utility_math.h"

//===================================================
// コンストラクタ
//===================================================
CParticle::CParticle() : 
	m_info()
{
}

//===================================================
// デストラクタ
//===================================================
CParticle::~CParticle()
{
}

//===================================================
// 生成処理
//===================================================
CParticle* CParticle::Create(const Info& info)
{
	// 生成処理
	CParticle* pInstance = new CParticle;

	pInstance->m_info = info;

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
HRESULT CParticle::Init(void)
{
	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CParticle::Uninit(void)
{
	// 自分自身の破棄
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CParticle::Update(void)
{
	// 発生させる粒子の数分回す
	for (int nCnt = 0; nCnt < m_info.nNum; nCnt++)
	{
		// 角度の選出
		float fAngleX = (math::Random(m_info.fAngleXMin, m_info.fAngleXMax) * 0.01f);
		float fAngleY = (math::Random(m_info.fAngleYMin, m_info.fAngleYMax) * 0.01f);

		D3DXVECTOR3 move;

		// 移動用の選出
		move.x = math::Random(m_info.moveMin.x, m_info.moveMax.x);
		move.y = math::Random(m_info.moveMin.y, m_info.moveMax.y);
		move.z = math::Random(m_info.moveMin.z, m_info.moveMax.z);

		D3DXVECTOR3 moveWk;

		// 移動量の設定
		moveWk.x = sinf(fAngleX) * sinf(fAngleY) * move.x;
		moveWk.y = cosf(fAngleX) * move.y;
		moveWk.z = sinf(fAngleX) * cosf(fAngleY) * move.z;

		// 移動量の設定
		m_info.effectInfo.move = moveWk;

		// 半径
		float fRadius = math::Random(m_info.size.x * 0.5f, m_info.size.x);

		// 寿命の設定
		int nLife = math::Random(m_info.effectInfo.nLife / 2, m_info.effectInfo.nLife);

		m_info.effectInfo.nLife = nLife;

		// エフェクトの生成処理
		CEffect::Create(
			m_info.effectInfo,
			m_info.pos,
			{ fRadius,fRadius },
			m_info.col, 
			m_info.texturePath.c_str());
	}

	if (m_info.nTime <= 0)
	{
		Uninit();
	}

	// 発生時間を減らす
	m_info.nTime--;
}

//===================================================
// 描画処理
//===================================================
void CParticle::Draw(void)
{
}