//===================================================
//
// エフェクトのクラス [effect.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _EFFECT_H_
#define _EFFECT_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object_billboard.h"

//***************************************************
// ビルボードのクラスの定義
//***************************************************
class CEffect : public CObjectBillboard
{
public:
	// エフェクトのフラグ
	enum FLAG
	{
		FLAG_NONE				= 0,		// フラグ無し
		FLAG_GRAVITY			= 1 << 1,	// 重力
		FLAG_ALPHA_DECREASE		= 1 << 2,	// アルファ値を減らす
		FLAG_RADIUS_DECREASE	= 1 << 3,	// 半径を減らす
		FLAG_INERTIA			= 1 << 4,	// 慣性をつける
		FLAG_LERP				= 1 << 5,	// 補間する
		FLAG_MAX
	};

	// エフェクトの情報
	struct Info
	{
		D3DXVECTOR3		move;		// 移動量
		float			fGravity;	// 重力量
		float			fInertia;	// 慣性
		int				nLife;		// 寿命
		int				nNumLerp;	// 補間量
		unsigned int	unFlag;		// フラグ
	};

	CEffect();
	~CEffect();

	/// <summary>
	/// エフェクトの生成処理
	/// </summary>
	/// <param name="エフェクトの情報構造体"></param>
	/// <param name="位置"></param>
	/// <param name="大きさ"></param>
	/// <param name="色"></param>
	/// <param name="テクスチャのパス"></param>
	/// <returns>自分自身のポインタ</returns>
	static CEffect* Create(const Info& info, const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const D3DXCOLOR& col, const char* pTexturePath);

	HRESULT Init		(void) override;
	void	Uninit		(void) override;
	void	Update		(void) override;
	void	Draw		(void) override;
private:
	void	UpdateAlpha	(void);
	void	UpdateSize	(void);
	void	Lerp		(void);
private:
	Info		m_info;			// 情報
	D3DXVECTOR3 m_posOld;		// 前回の位置
	float		m_fDecRadius;	// 半径の減る量
	float		m_fDecAlv;		// アルファ値の減る量
};
#endif