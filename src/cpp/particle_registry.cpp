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

		std::string key = items["key"];

		// 追加されていなければ追加
		m_umParticleInfo.emplace("", info);
	}
	return S_OK;
}
