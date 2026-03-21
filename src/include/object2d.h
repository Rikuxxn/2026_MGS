//===================================================
//
// 2Dオブジェクトの基底クラス [object2d.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"

//***************************************************
// 2Dオブジェクトの基底クラスの定義
//***************************************************
class CObject2D : public CObject
{
public:
	explicit CObject2D(const PRIORITY priority = PRIORITY_UI_FRONT);
	virtual ~CObject2D();

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="位置(ポリゴンの中心x.y)"></param>
	/// <param name="大きさ"></param>
	/// <param name="色"></param>
	/// <param name="テクスチャのパス"></param>
	/// <param name="向き"></param>
	/// <returns>自分自身のポインタ</returns>
	static CObject2D* Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const D3DCOLOR& col, const char* pTexturePath, const float fRot = 0.0f);

	virtual HRESULT Init	(void) override;
	virtual void	Uninit	(void) override;
	virtual void	Update	(void) override;
	virtual void	Draw	(void) override;

	/// <summary>
	/// テクスチャのIDの設定
	/// </summary>
	/// <param name="テクスチャのパス"></param>
	void SetTextureID		(const char* pTexturePath);
private:
	void SetVtx				(void);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// 頂点バッファへのポインター
	D3DXCOLOR				m_col;			// 色
	D3DXVECTOR3				m_pos;			// 位置
	D3DXVECTOR2				m_size;			// 大きさ
	float					m_fRot;			// 向き
	float					m_fAngle;		// ポリゴンの向き
	float					m_fLength;		// 長さ
	int						m_nTextureID;	// テクスチャのID
};
#endif