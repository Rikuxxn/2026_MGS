//===================================================
//
// マネージャークラス [manager.h]
// Author:YOSHIDA YUUTO
//
//===================================================

//***************************************************
// 二重インクルード防止
//***************************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// 前方宣言
//***************************************************
class CRenderer;			// レンダラー
class CScene;				// シーン
class CTextureManager;		// テクスチャマネージャー
class CModelManager;		// モデルマネージャー
class CInputKeyboard;		// キーボード
class CInputJoypad;			// ジョイパッド
class CInputMouse;			// マウス
class CFade;				// フェード
class CLight;				// ライト
class CCamera;				// カメラの取得
class CMotionLoader;		// モーションのロードクラス
class PhysicsWorld;			// 物理世界クラス
class CBlockManager;		// ブロックマネージャークラス
class CShaderManager;		// シェーダーマネージャーのクラス

//***************************************************
// マネージャークラスの定義
//***************************************************
class CManager final
{
public:
	~CManager();

	// マネージャーの取得
	static CManager* GetInstance(void);

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWind);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ChangeMode(std::unique_ptr<CScene> pNewScene);
	void BeginFade(std::unique_ptr<CScene> pNewScene, const D3DXCOLOR col = { 0.0f,0.0f,0.0f,0.0f });
	
	inline CRenderer*		GetRenderer			(void)				{ return m_pRenderer.get (); }
	inline CTextureManager* GetTextureManager	(void)				{ return m_pTextureManager.get (); }
	inline CInputKeyboard*	GetInputKeyboard	(void)				{ return m_pKeyboard.get(); }
	inline CInputJoypad*	GetInputJoypad		(void)				{ return m_pJoypad.get(); }
	inline CInputMouse*		GetInputMouse		(void)				{ return m_pMouse.get(); }
	inline CModelManager*	GetModelManager		(void)				{ return m_pModelManager.get(); }
	inline CScene*			GetScene			(void)				{ return m_pScene.get(); }
	inline CLight*			GetLight			(void)				{ return m_pLight.get(); }
	inline CCamera*			GetCamera			(void)				{ return m_pCamera.get(); }
	inline CMotionLoader*	GetMotionLoader		(void)				{ return m_pMotionLoader.get(); }
	inline PhysicsWorld*	GetPhysicsWorld		(void)				{ return m_pPhysicsWorld.get(); }
	inline CBlockManager*	GetBlockManager		(void)				{ return m_pBlockManager.get(); }
	inline CShaderManager*	GetShaderManager	(void)				{ return m_pShaderManager.get(); }
	inline void				SetFps				(const int nFps)	{ m_nFps = nFps; }
private:
	CManager();

	static std::unique_ptr<CManager>		m_pInstance;			// 自分のインスタンス
	std::unique_ptr<CRenderer>				m_pRenderer;			// 描画処理クラスへのポインタ
	std::unique_ptr<CScene>					m_pScene;				// シーンクラスへのポインタ
	std::unique_ptr<CTextureManager>		m_pTextureManager;		// テクスチャマネージャークラスへのポインタ
	std::unique_ptr<CModelManager>			m_pModelManager;		// モデルマネージャークラスへのポインタ
	std::unique_ptr<CInputKeyboard>			m_pKeyboard;			// キーボードクラスへのポインタ
	std::unique_ptr<CInputJoypad>			m_pJoypad;				// パッドクラスへのポインタ
	std::unique_ptr<CInputMouse>			m_pMouse;				// マウスクラスへのポインタ
	std::unique_ptr<CFade>					m_pFade;				// フェードクラスへのポインタ
	std::unique_ptr<CLight>					m_pLight;				// ライトクラスへのポインタ
	std::unique_ptr<CCamera>				m_pCamera;				// カメラクラスへのポインタ
	std::unique_ptr<CMotionLoader>			m_pMotionLoader;		// モーションのロードクラスへのポインタ
	std::unique_ptr<PhysicsWorld>			m_pPhysicsWorld;		// 物理世界へのポインタ
	std::unique_ptr<CBlockManager>			m_pBlockManager;		// ブロックマネージャーへのポインタ
	std::unique_ptr<CShaderManager>			m_pShaderManager;		// シェーダーマネージャーへのポインタ

	int m_nFps;														// フレームレート
};

#endif
