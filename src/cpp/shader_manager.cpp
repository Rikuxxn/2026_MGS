//===================================================
//
// シェーダーの初期化を行うs [shader_manager.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "shader_manager.h"
#include "manager.h"
#include "renderer.h"
#include "shader.h"

//***************************************************
// 定数宣言
//***************************************************
namespace ShaderConst
{
    constexpr const char* SKY_CUBE_PATH                = "data/SHADER/skycube.hlsl";                   // スカイキューブのパス
    constexpr const char* GAUS_BLUR_PATH                = "data/SHADER/gausblur.hlsl";                  // ガウスブラーのパス
    constexpr const char* LUMINANCE_PATH                = "data/SHADER/luminance.hlsl";                 // 輝度抽出のパス
    constexpr const char* BLOOM_PATH                    = "data/SHADER/bloom.hlsl";                     // ブルームのパス
    constexpr const char* SHADOW_MAP_PATH               = "data/SHADER/shadow_map.hlsl";                // シャドウマップのパス
    constexpr const char* SHADOW_MAP_RECIEVE_PATH       = "data/SHADER/shadow_map_reciever.hlsl";       // シャドウマップを受けるオブジェクトのパス
    constexpr const char* SHADOW_MAP_RECIEVE_MT_PATH    = "data/SHADER/shadow_map_reciever_mt.hlsl";    // シャドウマップを受けるマルチテクスチャオブジェクトのパス
    constexpr const char* TECHNIQUE                     = "tech";                                       // テクニック宣言
}

//===================================================
// コンストラクタ
//===================================================
CShaderManager::CShaderManager() : 
    m_apShader()
{
}

//===================================================
// デストラクタ
//===================================================
CShaderManager::~CShaderManager()
{
    // シェーダー分回す
    for (auto& shader : m_apShader)
    {
        // メモリの破棄
        shader.reset();
    }
}

//===================================================
// 生成処理
//===================================================
std::unique_ptr<CShaderManager> CShaderManager::Create(void)
{
    // インスタンスの生成
    auto pInstance = std::make_unique<CShaderManager>();

    // 初期化処理
    if (FAILED(pInstance->Init()))
    {
        pInstance.reset();
        return nullptr;
    }

    return pInstance;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CShaderManager::Init(void)
{
    // シャドウマップのシェーダーの生成
    m_apShader[TYPE_SHADOW_MAP] = std::make_unique<CShader>();

    // 初期化処理
    if (FAILED(m_apShader[TYPE_SHADOW_MAP]->Init(ShaderConst::SHADOW_MAP_PATH, ShaderConst::TECHNIQUE)))
    {
        return E_FAIL;
    }

    // シャドウマップを受けるシェーダーの生成
    m_apShader[TYPE_SHADOW_MAP_RECIEVE] = std::make_unique<CShader>();

    // 初期化処理
    if (FAILED(m_apShader[TYPE_SHADOW_MAP_RECIEVE]->Init(ShaderConst::SHADOW_MAP_RECIEVE_PATH, ShaderConst::TECHNIQUE)))
    {
        return E_FAIL;
    }

    // シャドウマップを受けるシェーダーの生成
    m_apShader[TYPE_SKY_CUBE] = std::make_unique<CShader>();

    // 初期化処理
    if (FAILED(m_apShader[TYPE_SKY_CUBE]->Init(ShaderConst::SKY_CUBE_PATH, ShaderConst::TECHNIQUE)))
    {
        return E_FAIL;
    }

    return S_OK;
}
