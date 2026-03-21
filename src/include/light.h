//===================================================
//
// ライトの処理 [light.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _LIGHT_H_
#define _LIGHT_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "array"

//***************************************************
// ライトのクラスの定義
//***************************************************
class CLight
{
public:
	static constexpr int NUM_LIGHT = 8; // ライトの最大数

	struct Info
	{
		D3DLIGHT9 Light;	// ライトの情報
		bool bUse;			// 使用状態かどうか
	};

	CLight();
	~CLight();

	void Init(void);
	void Uninit(void);
	void Update(void);

	void SetDirectional(const D3DXCOLOR Diffuse, const D3DXVECTOR3 dir, const D3DXVECTOR3 pos);
	std::array<Info, NUM_LIGHT> GetLight(void) const { return m_Light; }
private:

	std::array<Info, NUM_LIGHT> m_Light; // ライトの情報
};
#endif