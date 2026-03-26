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

//===================================================
// vector3の取得
//===================================================
D3DXVECTOR3 JsonLoader::GetVector3(const nlohmann::json& config)
{
	D3DXVECTOR3 output;

	output.x = config[0];
	output.y = config[1];
	output.z = config[2];

	return output;
}

//===================================================
// vector2の取得
//===================================================
D3DXVECTOR2 JsonLoader::GetVector2(const nlohmann::json& config)
{
	D3DXVECTOR2 output;

	output.x = config[0];
	output.y = config[1];

	return output;
}

//===================================================
// 色の取得
//===================================================
D3DXCOLOR JsonLoader::GetColor(const nlohmann::json& config)
{
	D3DXCOLOR col;
	col.r = config[0];
	col.g = config[1];
	col.b = config[2];
	col.a = config[3];

	return col;
}
