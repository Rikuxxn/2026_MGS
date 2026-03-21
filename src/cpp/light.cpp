//===================================================
//
// ライトの処理 [light.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルード
//***************************************************
#include "light.h"
#include "renderer.h"
#include "manager.h"
#include "assert.h"
#include "input.h"
#include "debug_proc.h"

//===================================================
// コンストラクタ
//===================================================
CLight::CLight() : 
	m_Light()
{
}

//===================================================
// デストラクタ
//===================================================
CLight::~CLight()
{
	
}

//===================================================
// 初期化処理
//===================================================
void CLight::Init(void)
{
	// レンダラーの取得
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();

#ifdef _DEBUG
	assert(pRenderer != nullptr && "Light_GetRenderer");
#endif // _DEBUG

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// メモリのクリア
	ZeroMemory(&m_Light, sizeof(m_Light));

	int nIdx = 0;

	// ライトの要素分回す
	for (auto light : m_Light)
	{
		// オフにする
		pDevice->LightEnable(nIdx, FALSE);
		nIdx++;
	}
}

//===================================================
// 終了処理
//===================================================
void CLight::Uninit(void)
{

}

//===================================================
// 更新処理
//===================================================
void CLight::Update(void)
{
}

//===================================================
// ライトの設定処理
//===================================================
void CLight::SetDirectional(const D3DXCOLOR Diffuse, const D3DXVECTOR3 dir, const D3DXVECTOR3 pos)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	int nIdx = 0;

	for (auto& light : m_Light)
	{
		// 使われていないなら
		if (light.bUse == false)
		{
			light.Light.Type = D3DLIGHT_DIRECTIONAL;
			light.Light.Diffuse = Diffuse;

			// ライトの方向ベクトル
			D3DXVECTOR3 Direction = dir;

			// 正規化する
			D3DXVec3Normalize(&Direction, &Direction);

			// 方向ベクトルの代入
			light.Light.Direction = Direction;

			// 位置の設定
			light.Light.Position = pos;

			light.bUse = true;

			// ライトの設定
			pDevice->SetLight(nIdx, &light.Light);

			// ライトを有効化
			pDevice->LightEnable(nIdx, TRUE);

			break;
		}
		nIdx++;
	}
}