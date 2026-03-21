//==============================================
//
// テクスチャマネージャー [texture_manager.h]
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// 多重インクルード防止
//**********************************************
#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

//**********************************************
// インクルードファイル
//**********************************************
#include "vector"

//**********************************************
// テクスチャのマネージャークラスの定義
//**********************************************
class CTextureManager
{
public:

	static constexpr int INVALID_ID = -1;						// 使えないID
	static constexpr auto TEXTURE_ROOT_PATH = "data/TEXTURE/";	// 省略用テクスチャパス

	CTextureManager();
	~CTextureManager();

	int Register(const char* pFilename);
	LPDIRECT3DTEXTURE9 GetAdress(const int nIdx);
	HRESULT Load(void);
	void UnLoad(void);

private:
	void SaveList(void);

	// テクスチャの情報の定義
	struct TextureInfo
	{
		LPDIRECT3DTEXTURE9 pTexture;	// テクスチャへのポインタ
		std::string filepath;			// ファイルパス
	}; 

	std::vector<TextureInfo> m_apTextureInfo;	 // テクスチャの情報
	static int m_nNumAll;						 // テクスチャの番号
};

#endif