//===================================================
//
// プランクトンの処理 [plankton.cpp]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "json_loader.h"

//***************************************************
// 定数宣言
//***************************************************
namespace JsonLoaderConst
{
	constexpr const char *ROOT_FILE_PATH = "data/SYSTEM/";
}

//===================================================
// ロード処理
//===================================================
HRESULT JsonLoader::Load(const char* pJsonFilePath, nlohmann::json& output)
{
	// 省略用ファイルパス
	std::string filePath = pJsonFilePath;

	// ファイルパスが省略されていたら
	if (filePath.find(JsonLoaderConst::ROOT_FILE_PATH) == std::string::npos)
	{
		// 文字列を連結
		filePath = JsonLoaderConst::ROOT_FILE_PATH + filePath;
	}

	// ファイルを開く
	std::fstream file(filePath);

	// 開けなかったら処理をしない
	if (file.is_open() == false)
	{
		MessageBox(NULL, "ファイルが開けませんでした。", pJsonFilePath, MB_OK);
		return E_FAIL;
	}

	// 読み込む
	file >> output;

	// 要素をクリア
	file.close();
	file.clear();

	return S_OK;
}
