//===================================================
//
// メッシュシリンダー [mesh_cylinder.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESH_CYLINDER_H_
#define _MESH_CYLINDER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"

//***************************************************
// メッシュシリンダーのクラスの定義
//***************************************************
class CMeshCylinder : public CObject
{
public:
	CMeshCylinder();
	~CMeshCylinder();

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="位置"></param>
	/// <param name="半径"></param>
	/// <param name="高さ"></param>
	/// <param name="色"></param>
	/// <param name="分割数"></param>
	/// <param name="テクスチャのパス"></param>
	/// <returns>自分自身のポインタ</returns>
	static CMeshCylinder* Create(const D3DXVECTOR3& pos, const float fRadius,const float fHeight, const D3DXCOLOR& col, const VECTOR2INT& segment, const char* pTexturePath);

	HRESULT Init	(void) override;
	void	Uninit	(void) override;
	void	Update	(void) override;
	void	Draw	(void) override;

	/// <summary>
	/// テクスチャのIDの設定
	/// </summary>
	/// <param name="テクスチャのパス"></param>
	void SetTextureID(const char* pTexturePath);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuffer;	// インデックスバッファへのポインタ
	D3DXMATRIX				m_mtxWorld;		// ワールドマトリックス
	D3DXCOLOR				m_col;			// 色
	D3DXVECTOR3				m_pos;			// 位置
	D3DXVECTOR3				m_rot;			// 向き
	VECTOR2INT				m_segment;		// 分割数
	float					m_fRadius;		// 半径
	float					m_fHeight;		// 高さ
	int						m_nNumVertex;	// 頂点数
	int						m_nNumPolygon;	// ポリゴン数
	int						m_nNumIndex;	// インデックス数
	int						m_nTextureID;	// テクスチャのID
};
#endif
