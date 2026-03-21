//===================================================
//
// メッシュサークル [mesh_cylinder.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESH_CIRCLE_H_
#define _MESH_CIRCLE_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"

//***************************************************
// メッシュサークルのクラスの定義
//***************************************************
class CMeshCircle : public CObject
{
public:

	// 情報構造体
	struct Info
	{
		D3DXCOLOR	col;			// 色
		float		fInRadius;		// 内側の半径
		float		fOutRadius;		// 外側の半径
		float		fInHeight;		// 内側の高さ
		float		fOutHeight;		// 外側の高さ
		float		fSpeed;			// 速さ
		int			nLife;			// 寿命
	};

	CMeshCircle();
	~CMeshCircle();

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="位置"></param>
	/// <param name="テクスチャのパス"></param>
	/// <param name="分割数、色、内側半径、外側半径、内側高さ、外側高さ、アルファ値の減少スピード、拡大スピード、寿命"></param>
	/// <returns>自分自身の生成</returns>
	static CMeshCircle* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pTexturePath, const int nSegment, const Info& info);

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
	D3DXCOLOR				m_inColor;		// 内側の色
	D3DXCOLOR				m_outColor;		// 外側の色
	D3DXVECTOR3				m_pos;			// 位置
	D3DXVECTOR3				m_rot;			// 向き
	Info					m_info;			// 情報
	float					m_fDecAlpha;	// アルファ値の減少スピード
	int						m_nSegmentU;	// 横の分割数
	int						m_nSegmentV;	// 縦の分割数
	int						m_nNumVertex;	// 頂点数
	int						m_nNumPolygon;	// ポリゴン数
	int						m_nNumIndex;	// インデックス数
	int						m_nTextureID;	// テクスチャのID
};
#endif
