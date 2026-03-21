//==============================================
//
// モデルマネージャー [model_manager.h]
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// 多重インクルード防止
//**********************************************
#ifndef _MODELMANAGER_H_
#define _MODELMANAGER_H_

//**********************************************
// インクルードファイル
//**********************************************
#include "vector"

//**********************************************
// モデルのマネージャークラスの定義
//**********************************************
class CModelManager
{
public:

	static constexpr int INVALID_ID = -1;					// 無効なID
	static constexpr auto MODEL_ROOT_PATH = "data/MODEL/";	// 省略用パス

	// モデルの情報の定義
	struct ModelInfo
	{
		D3DXVECTOR3 size;				  // 大きさ
		LPD3DXMESH pMesh;				  // メッシュ(頂点情報)へのポインタ
		LPD3DXBUFFER pBuffMat;			  // マテリアルへのポインタ
		DWORD dwNumMat;					  // マテリアルの数
		std::vector<int> nTextureIdx;	  // テクスチャのインデックス
		std::string filepath;			  // ファイルパス
	};

	CModelManager();
	~CModelManager();

	int Register(const char* pFilename);
	ModelInfo GetModelInfo(const int nIdx);
	HRESULT Load(void);
	void UnLoad(void);
private:

	void SaveList(void);
	void SetSize(ModelInfo* pModelInfo);
	void SetMaterial(ModelInfo* pModelInfo);

	std::vector<ModelInfo> m_aModelInfo; // モデルの情報
	static int m_nNumAll;				 // テクスチャの番号
};

#endif
