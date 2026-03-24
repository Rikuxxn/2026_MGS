//===================================================
//
// マルチレンダーターゲットのテクスチャの管理 [texture_mt_manager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _TEXTURE_MT_MANAGER_H_
#define _TEXTURE_MT_MANAGER_H_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// マルチレンダーターゲットのテクスチャのマネージャーの定義
//***************************************************
class CTextureMRTManager
{
public:

	static constexpr int INVALID = -1;	// 無効なID
	enum TYPE
	{
		TYPE_TUTORIAL = 0,	// チュートリアル
		TYPE_SCENE,			// シーン
		TYPE_SHADOW_MAP,	// シャドウマップ
		TYPE_MAX
	};

	struct MRTDec
	{
		TYPE type;	// レンダーターゲットの種類
		D3DXVECTOR3 posV;	// 視点
		D3DXVECTOR3 posR;	// 注視点
		int nWidth;			// 横幅
		int nHeight;		// 縦幅
		bool bClear;		// クリアするかどうか
	};

	// 初期設定
	const MRTDec CREATE_TABLE[TYPE_MAX] =
	{
		{ TYPE_TUTORIAL,	D3DXVECTOR3(-83.0f, 210.0f, -220.0f),	D3DXVECTOR3(-79.0f, 210.0f, 10.0f)	,SCREEN_WIDTH,SCREEN_HEIGHT,true },
		{ TYPE_SCENE,		D3DXVECTOR3(0.0f,0.0f,0.0f)			,	D3DXVECTOR3(0.0f,0.0f,0.0f)			,SCREEN_WIDTH,SCREEN_HEIGHT,true },
		{ TYPE_SHADOW_MAP,	D3DXVECTOR3(0.0f, 1200.0f, 500.0f)	,	D3DXVECTOR3(1.0f, 1.0f, 1.0f)		,SCREEN_WIDTH,SCREEN_HEIGHT,true } ,
	};

	// テクスチャの情報
	struct Info
	{
		LPDIRECT3DTEXTURE9 pTextureMT;	// レンダリングターゲット用テクスチャ
		LPDIRECT3DSURFACE9 pRenderMT;	// レンダリングターゲット用インターフェース
		LPDIRECT3DSURFACE9 pZBuffMT;	// レンダリングターゲット用Zバッファ
		D3DVIEWPORT9 viewport;			// テクスチャレンダリング用ビューポート
		int nType;						// 自分の種類の番号
	};

	CTextureMRTManager();
	~CTextureMRTManager();

	LPDIRECT3DTEXTURE9 GetAdress(const int nType);

	HRESULT Init(void);
	void Release(void);
	void Release(const int nIdx);

	void ChangeRenderTarget(const int nIdx, const int nStage = 0);
	void ResetRenderTarget(const int nStage = 0);
private:
	std::array<Info, TYPE_MAX> m_aInfo;	// マルチレンダーターゲットの情報
	Info m_defInfo;						// デフォルトの描画情報の保存用変数
	D3DXMATRIX m_mtxViewDef;			// ビューマトリックスの保存用変数
	D3DXMATRIX m_mtxProjDef;			// プロジェクションマトリックスの保存用変数
};
#endif