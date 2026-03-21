//===================================================
//
// 数字のポリゴン用クラス(メンバ変数に持つ) [number.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _NUMBER_H_
#define _NUMBER_H_

//***************************************************
// 数字のポリゴン用クラスの定義
//***************************************************
class CNumber
{
public:
	static constexpr int SEGMENT_U = 9;	// 横の分割数
	static constexpr int SEGMENT_V = 1;	// 縦の分割数

	CNumber();
	~CNumber();

	static CNumber* Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const D3DCOLOR& col, const char* pTexturePath, const float fRot = 0.0f);

	HRESULT Init	(void);
	void	Uninit	(void);
	void	Update	(void);
	void	Draw	(void);

	/// <summary>
	/// テクスチャのIDの設定
	/// </summary>
	/// <param name="テクスチャのパス"></param>
	void SetTextureID	(const char* pTexturePath);

	void SetUV		(const int nNumber);
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