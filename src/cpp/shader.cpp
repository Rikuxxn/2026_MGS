//===================================================
//
// シェーダーのクラスの基底クラス [shader.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "shader.h"
#include "manager.h"
#include "renderer.h"
#include "texture_manager.h"
#include "assert.h"

//===================================================
// コンストラクタ
//===================================================
CShader::CShader() : 
    m_pEffect(nullptr),
    m_Param(),
    m_nNumPass(0)
{
}

//===================================================
// デストラクタ
//===================================================
CShader::~CShader()
{
    // シェーダーの破棄
    if (m_pEffect != nullptr)
    {
        m_pEffect->Release();
        m_pEffect = nullptr;
    }

    m_Param.clear();
}

//===================================================
// 初期化処理
//===================================================
HRESULT CShader::Init(const char* pShaderFile, const char* pTech)
{
    // マネージャーの取得
    CManager* pManager = CManager::GetInstance();

    // レンダラーの取得
    CRenderer* pRenderer = pManager->GetRenderer();

#ifdef _DEBUG
    assert(pRenderer != nullptr && "Shader");
#endif // _DEBUG

    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
    
    LPD3DXBUFFER pErros = nullptr; // エラー情報を格納する変数

    // シェーダーの生成
    HRESULT result = D3DXCreateEffectFromFile(
        pDevice,
        pShaderFile,
        nullptr,
        nullptr,
        0,
        nullptr,
        &m_pEffect,
        &pErros
    );

    // 生成に失敗したら
    if (FAILED(result) && pErros != nullptr)
    {
        OutputDebugStringA((char*)pErros->GetBufferPointer());
        pErros->Release();
        pErros = nullptr;
        return E_FAIL;
    }

    if (m_pEffect != nullptr)
    {
        // テクニックハンドルの設定
        if (FAILED(m_pEffect->SetTechnique(pTech)))
        {
            MessageBox(NULL, "テクニック宣言が見つかりません", pShaderFile, NULL);
            return E_FAIL;
        }

        D3DXTECHNIQUE_DESC techDesc;
        m_pEffect->GetTechniqueDesc(pTech, &techDesc);

        m_nNumPass = techDesc.Passes;   // パスの数の取得
    }

	return S_OK;
}

//===================================================
// シェーダーのパスの開始
//===================================================
void CShader::BeginPass(const int nPass)
{
    // シェーダーの描画パス開始
    if (m_pEffect != nullptr)
    {
        m_pEffect->BeginPass(nPass);
    }
}

//===================================================
// シェーダーのパスの終了
//===================================================
void CShader::EndPass(void)
{
    // シェーダーの描画パス終了
    if (m_pEffect != nullptr)
    {
        m_pEffect->EndPass();
    }
}

//===================================================
// 値の設定
//===================================================
void CShader::SetValue(const char* pKey, const void* pData, size_t size)
{
    // nullなら処理しない
    if (m_pEffect == nullptr) return;

    // 要素の設定
    m_pEffect->SetValue(pKey, pData, size);
}

//===================================================
// フロートの設定処理
//===================================================
void CShader::SetFloat(const char* pKey, const float fParam)
{   
    // nullなら処理しない
    if (m_pEffect == nullptr) return;

    // パラメータの設定
    m_pEffect->SetFloat(pKey, fParam);
}

//===================================================
// フロートの設定処理
//===================================================
void CShader::SetFloatArray(const D3DXHANDLE& hHandle, const FLOAT* pParam, const UINT count)
{
    // nullなら処理しない
    if (m_pEffect == nullptr) return;

    // パラメータの設定
    m_pEffect->SetFloatArray(hHandle, pParam, count);
}

//===================================================
// 配列のfloat4の値の設定
//===================================================
void CShader::SetVectorFloatArray(const D3DXHANDLE& hHandle, const D3DXVECTOR4* pVector4, const UINT count)
{
    // nullなら処理しない
    if (m_pEffect == nullptr) return;

    // パラメータの設定
    m_pEffect->SetVectorArray(hHandle, pVector4, count);
}

//===================================================
// 色の設定処理
//===================================================
void CShader::SetVector(const char* pKey, const D3DXVECTOR4& vector4)
{
    // nullなら処理しない
    if (m_pEffect == nullptr) return;

    // パラメータの設定
    m_pEffect->SetVector(pKey, &vector4);
}

//===================================================
// マトリックスの設定処理
//===================================================
void CShader::SetMatrix(const char* pKey, const D3DXMATRIX& matrix)
{
    // nullなら処理しない
    if (m_pEffect == nullptr) return;

    // パラメータの設定
    m_pEffect->SetMatrix(pKey, &matrix);
}

//===================================================
// テクスチャの設定
//===================================================
void CShader::SetTexture(const char* pKey, const int nTextureID)
{
    // テクスチャマネージャーの取得
    CTextureManager* pTextureManager = CManager::GetInstance()->GetTextureManager();

    // 取得できないなら処理しない
    if (pTextureManager == nullptr) return;

    // nullなら処理しない
    if (m_pEffect == nullptr) return;

    // テクスチャの取得
    LPDIRECT3DTEXTURE9 pTexture = pTextureManager->GetAdress(nTextureID);

    // テクスチャの設定
    m_pEffect->SetTexture(pKey, pTexture);
}

//===================================================
// テクスチャの設定処理
//===================================================
void CShader::SetTexture(const char* pKey, const LPDIRECT3DTEXTURE9& pTextureID)
{
    // nullなら処理しない
    if (m_pEffect == nullptr) return;

    // テクスチャの設定
    if (FAILED(m_pEffect->SetTexture(pKey, pTextureID)))
    {
        assert(false);
    }
}

//===================================================
// ハンドルの取得
//===================================================
D3DXHANDLE CShader::GetHandle(const char* pKey) const
{
    // nullなら処理しない
    if (m_pEffect == nullptr) return D3DXHANDLE();

    // ハンドルの取得
    D3DXHANDLE handle = m_pEffect->GetParameterByName(nullptr, pKey);

    return handle;
}

//===================================================
// ブールの設定
//===================================================
void CShader::SetBool(const char* pKey, const bool bEnable)
{
    // nullなら処理しない
    if (m_pEffect == nullptr) return;

    // テクスチャの設定
    if (FAILED(m_pEffect->SetBool(pKey, bEnable)))
    {
        assert(false);
    }
}

//===================================================
// コミットチェンジ
//===================================================
void CShader::CommitChange(void)
{
    if (m_pEffect != nullptr)
    {
        m_pEffect->CommitChanges();
    }
}

//===================================================
// シェーダーの開始
//===================================================
void CShader::Begin(void)
{
    // シェーダーの描画開始
    if (m_pEffect != nullptr)
    {
        m_pEffect->Begin(&m_nNumPass, 0);
        return;
    }

    assert(false && "shader_null");
}

//===================================================
// シェーダーの終了
//===================================================
void CShader::End(void)
{
    // シェーダーの描画終了
    if (m_pEffect != nullptr)
    {
        m_pEffect->End();
    }
}
