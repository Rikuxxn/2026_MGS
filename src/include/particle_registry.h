//===================================================
//
// パーティクルの読み込んだ情報を格納するクラス [particle_registry.h]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _PARTICLE_REGISTRY_H_
#define _PARTICLE_REGISTRY_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "particle.h"

//***************************************************
// パーティクルの読み込んだ情報を格納するクラスの定義
//***************************************************
class CParticleRegistry
{
public:
	CParticleRegistry();
	~CParticleRegistry();

	static std::unique_ptr<CParticleRegistry> Create(void);
private:
	HRESULT Load(const char* pFilename);
private:
	std::unordered_map<std::string, CParticle::Info> m_umParticleInfo;	// パーティクルの読み込んだ情報
};
#endif