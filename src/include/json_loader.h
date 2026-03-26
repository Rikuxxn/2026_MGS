//===================================================
//
// プランクトンの処理 [plankton.h]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _JSON_LOADER_H_
#define _JSON_LOADER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "json.hpp"

//***************************************************
// jsonファイルのロードの補助
//***************************************************
namespace JsonLoader
{
	HRESULT Load(const char* pJsonFilePath, nlohmann::json& output);
	D3DXVECTOR3 GetVector3(const nlohmann::json& config);
	D3DXVECTOR2 GetVector2(const nlohmann::json& config);
	D3DXCOLOR GetColor(const nlohmann::json& config);
}
#endif