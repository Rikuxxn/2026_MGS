//===================================================
//
// textureアニメーションを適応するクラス [texture_animation.h]
// Author:YUTO YOSHIDA
// 
// 2D,3Dポリゴンがメンバ変数にもつ
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _TEXTURE_ANIMATION_H_
#define _TEXTURE_ANIMATION_H_

//***************************************************
// 前方宣言
//***************************************************
class CObject;	// オブジェクトクラスへのポインタ

//***************************************************
// テクスチャアニメーションをするクラスの定義
//***************************************************
class CTextureAnimation
{
public:
	// 種類
	enum TYPE
	{
		TYPE_2D = 0,	// 通常の2Dポリゴン
		TYPE_MULT2D,	// マルチテクスチャの2Dポリゴン
		TYPE_3D,		// 通常の3Dポリゴン
		TYPE_MULT3D,	// マルチテクスチャの3Dポリゴン
		TYPE_MAX
	};

	CTextureAnimation();
	~CTextureAnimation();

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="種類"></param>
	/// <param name="分割数"></param>
	/// <param name="速さ"></param>
	/// <param name="ループするかどうか(falseの場合最後まで行くと消えます)"></param>
	/// <returns>自分のポインタ</returns>
	static std::unique_ptr<CTextureAnimation> Create(const TYPE type, const VECTOR2INT& segment, const int nFrame, const bool bLoop);

	HRESULT Init	(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="頂点バッファ"></param>
	/// <param name="ループしないときに破棄するためにオブジェクトクラスのポインタを取得"></param>
	void	Update	(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, CObject* pOwner);
private:
	void	Apply2D	(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer);
	void	Apply3D	(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer);

	TYPE		m_type;		// 種類
	VECTOR2INT	m_segment;	// 分割数
	int			m_nFrame;	// 速さ
	int			m_nPattern;	// パターン数
	int			m_nCounter;	// 次のフレームに行くまでの計測カウンター
	bool		m_bLoop;	// ループするかどうか
};
#endif