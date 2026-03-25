//===================================================
//
// メッシュフィールド [mesh_field.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESH_FIELD_H_
#define _MESH_FIELD_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"
#include "collision_result.h"

//***************************************************
// 前方宣言
//***************************************************
class CMeshFieldCollision;	// メッシュフィールドの当たり判定クラス

//***************************************************
// メッシュフィールドのクラスの定義
//***************************************************
class CMeshField : public CObject
{
public:
	CMeshField();
	~CMeshField();

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="位置"></param>
	/// <param name="大きさ"></param>
	/// <param name="色"></param>
	/// <param name="分割数"></param>
	/// <param name="テクスチャのパス"></param>
	/// <returns>自分自身のポインタ</returns>
	static CMeshField* Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const D3DXCOLOR& col, const VECTOR2INT& segment, const char* pTexturePath);

	HRESULT Init		(void) override;
	void	Uninit		(void) override;
	void	Update		(void) override;
	void	Draw		(void) override;

	bool	Collision(const D3DXVECTOR3& targetPos, CollisionResult::MeshField& outResult);
	/// <summary>
	/// テクスチャのIDの設定
	/// </summary>
	/// <param name="テクスチャのパス"></param>
	void	SetTextureID(const char* pNoiseTexturePath, const char* pSeaTexturePath);
private:
	std::unique_ptr<CMeshFieldCollision> m_pCollision;	// 当たり判定

	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffer;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuffer;	// インデックスバッファへのポインタ
	D3DXMATRIX					m_mtxWorld;		// ワールドマトリックス
	D3DXCOLOR					m_col;			// 色
	D3DXVECTOR3					m_pos;			// 位置
	D3DXVECTOR3					m_rot;			// 向き
	D3DXVECTOR2					m_size;			// 大きさ
	VECTOR2INT					m_segment;		// 分割数

	int	m_nNumVertex;			// 頂点数
	int	m_nNumPolygon;			// ポリゴン数
	int	m_nNumIndex;			// インデックス数
	int	m_nNoiseTextureID;		// テクスチャのID
	int	m_nSeaTextureID;		// テクスチャのID
	float m_fTime;				// 波の動きを出す
};
#endif
