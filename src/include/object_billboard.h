//===================================================
//
// ビルボードオブジェクトの基底クラス [object_billboard.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _OBJECT_BILLBOARD_H_
#define _OBJECT_BILLBOARD_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"

//***************************************************
// ビルボードのクラスの定義
//***************************************************
class CObjectBillboard : public CObject
{
public:
	explicit CObjectBillboard(const PRIORITY priority = PRIORITY_BILLBOARD);
	virtual ~CObjectBillboard();

	static CObjectBillboard* Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size,const D3DXCOLOR& col, const char* pTexturePath);

	virtual HRESULT Init	(void) override;
	virtual void	Uninit	(void) override;
	virtual void	Update	(void) override;
	virtual void	Draw	(void) override;

	/// <summary>
	/// テクスチャのIDの設定処理
	/// </summary>
	/// <param name="テクスチャのパス"></param>
	void			SetTextureID	(const char* pTexturePath);
	inline void		SetPosition		(const D3DXVECTOR3& pos)	{ m_pos = pos; }
	inline void		SetSize			(const D3DXVECTOR2& size)	{ m_size = size; }
	inline void		SetColor		(const D3DXCOLOR& col)		{ m_col = col; }

	inline const D3DXVECTOR3&	GetPosition	(void) const { return m_pos; }
	inline const D3DXVECTOR2&	GetSize		(void) const { return m_size; }
	inline const D3DXCOLOR&		GetColor	(void) const { return m_col; }

private:
	void SetVtx(void);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// 頂点へのポインタ
	D3DXMATRIX				m_mtxWorld;		// ワールドマトリックス
	D3DXCOLOR				m_col;			// 色
	D3DXVECTOR3				m_pos;			// 位置のクラスのポインタ
	D3DXVECTOR2				m_size;			// 大きさ
	int						m_nTextureID;	// テクスチャのインデックス
};
#endif
