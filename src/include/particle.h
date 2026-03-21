//===================================================
//
// パーティクル [particle.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"
#include "effect.h"

//***************************************************
// パーティクルのクラスの定義
//***************************************************
class CParticle : public CObject
{
public:
	// パーティクルの情報
	struct Info
	{
		CEffect::Info	effectInfo;		// エフェクトの情報
		std::string		texturePath;	// テクスチャのパス
		D3DXCOLOR		col;			// 色
		D3DXVECTOR3		pos;			// 位置
		D3DXVECTOR2		size;			// 大きさ
		D3DXVECTOR3		moveMin;		// 最小移動量
		D3DXVECTOR3		moveMax;		// 最大移動量
		float			fAngleXMin;		// 最小の角度X
		float			fAngleXMax;		// 最大の角度X
		float			fAngleYMin;		// 最小の角度Y
		float			fAngleYMax;		// 最大の角度Y
		int				nTime;			// 発生させる時間
		int				nNum;			// 生成する粒子の数
	};

	CParticle();
	~CParticle();

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="パーティクルの情報"></param>
	/// <returns></returns>
	static CParticle* Create(const Info& info);

	HRESULT Init	(void) override;
	void	Uninit	(void) override;
	void	Update	(void) override;
	void	Draw	(void) override;
private:
	Info	m_info;			// パーティクルの情報
};
#endif