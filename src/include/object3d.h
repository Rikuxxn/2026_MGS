//===================================================
//
// 3Dオブジェクトの基底クラス [object3d.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"

//***************************************************
// 前方宣言
//***************************************************
class CTextureAnimation;	// テクスチャアニメーションクラス

//***************************************************
// 3Dオブジェクトの基底クラスの定義
//***************************************************
class CObject3D : public CObject
{
public:
	explicit CObject3D(const PRIORITY priority = PRIORITY_3DPOLYGON);
	virtual ~CObject3D();

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="位置(中心位置)"></param>
	/// <param name="大きさ"></param>
	/// <param name="色"></param>
	/// <param name="テクスチャのパス"></param>
	/// <param name=""></param>
	/// <returns>自分自身のポインタ</returns>
	static CObject3D* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size,const D3DXVECTOR3& rot, const D3DCOLOR& col, const char* pTexturePath);

	virtual HRESULT Init	(void) override;
	virtual void	Uninit	(void) override;
	virtual void	Update	(void) override;
	virtual void	Draw	(void) override;

	/// <summary>
	/// テクスチャのIDの設定
	/// </summary>
	/// <param name="テクスチャのパス"></param>
	void SetTextureID		(const char* pTexturePath);

	/// <summary>
	/// アニメーションを適応する関数
	/// </summary>
	/// <param name="種類(アニメーションクラスの列挙型を使用)"></param>
	/// <param name="分割数"></param>
	/// <param name="速さ"></param>
	/// <param name="ループするかどうか(falseにすると最大まで行ったら消えます)"></param>
	void AttachAnimation	(const int nType, const VECTOR2INT& segment, const int nFrame, const bool bLoop);
private:
	std::unique_ptr<CTextureAnimation> m_pTextureAnimation;	// テクスチャアニメーションクラスへのポインタ

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// 頂点バッファへのポインター
	D3DXMATRIX				m_mtxWorld;		// ワールドマトリックス
	D3DXCOLOR				m_col;			// 色
	D3DXVECTOR3				m_pos;			// 位置
	D3DXVECTOR3				m_rot;			// 向き
	D3DXVECTOR3				m_size;			// 大きさ
	int						m_nTextureID;	// テクスチャのID
};
#endif