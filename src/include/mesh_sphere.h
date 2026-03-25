//===================================================
//
// メッシュの球体 [mesh_sphere.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESH_SPHERE_H_
#define _MESH_SPHERE_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"

//***************************************************
// メッシュの球体のクラスの定義
//***************************************************
class CMeshSphere : public CObject
{
public:
	CMeshSphere();
	~CMeshSphere();

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="位置"></param>
	/// <param name="色"></param>
	/// <param name="分割数"></param>
	/// <param name="半径"></param>
	/// <param name="テクスチャのパス"></param>
	/// <returns>自分自身のポインタ</returns>
	static CMeshSphere* Create(const D3DXVECTOR3& pos, const D3DXCOLOR& col, const VECTOR2INT& segment, const float fRadius, const char* pTexturePath);

	HRESULT Init	(void) override;
	void	Uninit	(void) override;
	void	Update	(void) override;
	void	Draw	(void) override;

	void	SetTextureID(const char* pTexturePath);
private:
	void	FanIndex(LPDIRECT3DINDEXBUFFER9* ppIndexBuffer, const int nSegmentU, int* pOffsetIndex);
	void	FanIndex(LPDIRECT3DINDEXBUFFER9* ppIndexBuffer, const int nSegmentU, int nOffsetVertex, int nOffsetIndex);
	int		GridIndex(LPDIRECT3DINDEXBUFFER9* ppIndexBuffer, const int nSegmentU, const int nSegmentV, const int nOffsetIndex, const int nOffsetVertex);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuffer;	// インデックスバッファへのポインタ

	D3DXCOLOR	m_col;						// 色
	D3DXMATRIX	m_mtxWorld;					// ワールドマトリックス
	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR3 m_rot;						// 向き
	VECTOR2INT	m_segment;					// 分割数
	float		m_fRadius;					// 半径
	int			m_nSideStartIndex;			// 側面の描画開始インデックス番号
	int			m_nBottomStartIndex;		// 下の蓋の描画開始インデックス番号
	int			m_nNumVertex;				// 頂点数
	int			m_nNumPolygon;				// ポリゴン数
	int			m_nNumIndex;				// インデックス数
	int			m_nTextureID;				// テクスチャのID
	bool		m_bReverse;					// 反転するかどうか
};
#endif