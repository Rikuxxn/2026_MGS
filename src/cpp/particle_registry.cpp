//===================================================
//
// パーティクルの読み込んだ情報を格納するクラス [particle_registry.cpp]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "particle_registry.h"
#include "json_loader.h"

//***************************************************
// 定数宣言
//***************************************************
namespace RegistryConst
{
	constexpr const char* FILE_PATH = "particle.json";	// 読み込むファイル名
}

//===================================================
// コンストラクタ
//===================================================
CParticleRegistry::CParticleRegistry() : 
	m_umParticleInfo()
{
}

//===================================================
// デストラクタ
//===================================================
CParticleRegistry::~CParticleRegistry()
{
	m_umParticleInfo.clear();
}

//===================================================
// 生成処理
//===================================================
std::unique_ptr<CParticleRegistry> CParticleRegistry::Create(void)
{
	// 自分自身の生成
	auto pInstance = std::make_unique<CParticleRegistry>();

	// 読み込み処理
	if(FAILED(pInstance->Load(RegistryConst::FILE_PATH)))
	{
		pInstance = nullptr;
		return nullptr;
	}
	return pInstance;
}

//===================================================
// パーティクルの生成処理
//===================================================
CParticle* CParticleRegistry::CreateParticle(const std::string& key, const D3DXVECTOR3& pos, const int nFlag, const D3DXCOLOR& col)
{
	// 指定したキーのイテレータの取得
	auto itr = m_umParticleInfo.find(key);

	// 取得できなかったら
	if (itr == m_umParticleInfo.end())
	{
		return nullptr;
	}

	itr->second.pos = pos;
	itr->second.effectInfo.unFlag = nFlag;
	itr->second.col = col;

	CParticle* pParticle = CParticle::Create(itr->second);
	return pParticle;
}

//===================================================
// 読み込み処理
//===================================================
HRESULT CParticleRegistry::Load(const char* pFilename)
{
	nlohmann::json config;

	// ロード処理
	if (FAILED(JsonLoader::Load(pFilename, config)))
	{
		return E_FAIL;
	}

	// 読み込んだ情報分回す
	for (const auto& items : config["particles"])
	{
		// パーティクルの情報
		CParticle::Info info;

		std::string key				= items["key"];

		// パラメータの設定
		info.texturePath			= items.value("texturePath", "null");
		info.col					= JsonLoader::GetColor(items["col"]);
		info.fAngleXMax				= items.value("angleXMax",0.0f);
		info.fAngleXMin				= items.value("angleXMin",0.0f);
		info.fAngleYMax				= items.value("angleYMax",0.0f);
		info.fAngleYMin				= items.value("angleYMin",0.0f);
		info.moveMax				= JsonLoader::GetVector3(items["moveMax"]);
		info.moveMin				= JsonLoader::GetVector3(items["moveMin"]);
		info.nNum					= items.value("num",0);
		info.nTime					= items.value("time",0);
		info.size					= JsonLoader::GetVector2(items["size"]);
		info.effectInfo.nLife		= items.value("life",0);
		info.effectInfo.fGravity	= items.value("gravity",0.0f);
		info.effectInfo.fInertia	= items.value("inertia",0.0f);
		info.effectInfo.nNumLerp	= items.value("num_lerp",0);
		info.effectInfo.move		= JsonLoader::GetVector3(items["move"]);

		// 追加されていなければ追加
		m_umParticleInfo.emplace(key, info);
	}
	return S_OK;
}
