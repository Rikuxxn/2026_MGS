//===================================================
//
// DirectXの描画処理 [renderer.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _RENDERER_H_
#define _RENDERER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "memory"

//***************************************************
// 前方宣言
//***************************************************
class CDebugProc;		// デバッグ情報
class CScreenPolygon;	// 画面のポリゴン
class CScene;			// シーンのクラス
class CFade;			// フェードクラス

//***************************************************
// レンダラークラスの定義
//***************************************************
class CRenderer
{
public:

	// スクリーンポリゴンの種類
	enum SCREEN_POLYGON
	{
		SCREEN_POLYGON_SCENE = 0,		// シーン
		SCREEN_POLYGON_LUMINANCE,		// 輝度抽出
		SCREEN_POLYGON_GAUSBLUR_X,		// ガウスブラー横
		SCREEN_POLYGON_GAUSBLUR_Y,		// ガウスブラー縦
		SCREEN_POLYGON_BLURBLEND,		// ボケ画像加算合成
		SCREEN_POLYGON_MAX
	};

	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(const int fps, CScene* pScene, CFade* pFade);

	LPDIRECT3DDEVICE9 GetDevice(void);

	void onWireFrame();
	void offWireFrame();
private:
	std::unique_ptr<CDebugProc>		m_pDebugProc;		// デバッグ情報のクラス
	LPDIRECT3D9						m_pD3D;				// Directxデバイスへのポインタ
	LPDIRECT3DDEVICE9				m_pD3DDevice;		// Directxデバイスへのポインタ
};

#endif