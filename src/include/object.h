//===================================================
//
// オブジェクトの基底 [object.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _OBJECT_H_
#define _OBJECT_H_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// ゲームクラスの定義
//***************************************************
class CObject
{
public:

	// 優先順位
	enum PRIORITY
	{
		PRIORITY_NONE = 0,		// 無し
		PRIORITY_CHARACTER,		// キャラクター
		PRIORITY_3DPOLYGON,		// 3Dポリゴン
		PRIORITY_MODEL,			// モデル
		PRIORITY_MESH,			// メッシュ
		PRIORITY_BILLBOARD,		// ビルボード
		PRIORITY_UI_BACK,		// UI(奥)
		PRIORITY_UI_FRONT,		// UI(手前)
		PRIORITY_SHADOW_MAP,	// シャドウマップ(別のレンダーターゲット)
		PRIORITY_MAX
	};

	// 種類
	enum TYPE
	{
		TYPE_NONE = 0,
		TYPE_MAX
	};

	explicit CObject(const PRIORITY priority = PRIORITY_NONE);
	virtual ~CObject();

	static void		ReleaseAll		(void);
	static void		UpdateAll		(void);
	static void		DrawAll			(void);
	static void		DestroyAll		(void);
	static int		GetNumObject	(const int nPriority) { return m_anNumAll[nPriority]; }


	virtual HRESULT Init	(void)	= 0;
	virtual void	Uninit	(void)	= 0;
	virtual void	Update	(void)	= 0;
	virtual void	Draw	(void)	= 0;

	inline void		SetType		(const TYPE type)	{ m_type = type; }

	inline PRIORITY	GetPriority	(void) const		{ return m_priority; }
	inline TYPE		GetType		(void) const		{ return m_type; }
	inline bool		GetDeath	(void) const		{ return m_bDeath; }

protected:
	void Release(void);	
private:
	void Destroy(CObject* pObject);
	void DrawShadowMap(void);
private:
	static std::array<CObject*, PRIORITY_MAX>	m_apTop;	// 先頭オブジェクトへのポインタ
	static std::array<CObject*, PRIORITY_MAX>	m_apCur;	// 最後尾オブジェクトへのポインタ
	static std::array<int	  , PRIORITY_MAX>	m_anNumAll;	// オブジェクトの総数

	CObject*	m_pPrev;		// 前のオブジェクトへのポインタ
	CObject*	m_pNext;		// 次のオブジェクトへのポインタ
	TYPE		m_type;			// オブジェクトの種類
	PRIORITY	m_priority;		// 優先順位
	bool		m_bDeath;		// 死亡フラグ
};
#endif