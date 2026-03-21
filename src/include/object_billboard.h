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
	void SetTextureID	(const char* pTexturePath);
private:
	void SetVtx			(void);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// 頂点へのポインタ
	D3DXMATRIX				m_mtxWorld;		// ワールドマトリックス
	D3DXCOLOR				m_col;			// 色
	D3DXVECTOR3				m_pos;			// 位置のクラスのポインタ
	D3DXVECTOR2				m_size;			// 大きさ
	int						m_nTextureID;	// テクスチャのインデックス
};
#endif
