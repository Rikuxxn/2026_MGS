//===================================================
//
// シェーダーの初期化を行うs [shader_manager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

//***************************************************
// 前方宣言
//***************************************************
class CShader;	// シェーダーのクラスの取得

//***************************************************
// シェーダーのマネージャークラスの定義
//***************************************************
class CShaderManager
{
public:
	static constexpr int INVALID = -1;	// 無効なID

	// シェーダーの種類
	typedef enum
	{
		TYPE_OUTLINE = 0,			// アウトラインシェーダー
		TYPE_GAUS_BLUR,				// ガウシアンブラー
		TYPE_LUMINANCE,				// 輝度抽出
		TYPE_BLOOM,					// ブルーム
		TYPE_SHADOW_MAP,			// シャドウマップ
		TYPE_SHADOW_MAP_RECIEVE,	// 影を受けるオブジェクトシェーダー
		TYPE_SHADOW_MAP_RECIEVE_MT,	// 影を受けるマルチテクスチャオブジェクトシェーダー
		TYPE_MAX
	}TYPE;

	CShaderManager();
	~CShaderManager();

	static std::unique_ptr<CShaderManager> Create(void);
	
	HRESULT Init(void);
	inline CShader* GetAddrees(const TYPE type) { return m_apShader[type].get(); }
private:
	std::array<std::unique_ptr<CShader>, TYPE_MAX> m_apShader; // シェーダークラスへのポインタ
};
#endif