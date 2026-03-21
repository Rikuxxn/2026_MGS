//===================================================
//
// フェード処理 [fade.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _FADE_H_
#define _FADE_H_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// 前方宣言
//***************************************************
class CScene;

//***************************************************
// フェードクラスの定義
//***************************************************
class CFade
{
public:
	enum class State : int
	{
		None = 0,	// フェードなしの状態
		In,			// フェードインの状態
		Out,		// フェードアウトの状態
		Max
	};

	CFade();
	~CFade();

	static std::unique_ptr<CFade> Create(void);
	void SetFade(std::unique_ptr<CScene> pNewScene,const D3DXCOLOR col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	State GetState(void) const { return m_Fade; }
private:
	std::unique_ptr<CScene> m_pScene;		// シーンのポインタ
	State m_Fade;							// フェード
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;   // 頂点バッファへのポインタ
	D3DXCOLOR m_col;						// フェードの色
};

#endif
