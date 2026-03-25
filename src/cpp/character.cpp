//===================================================
//
// キャラクターの共通処理クラス [character.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "character.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "motion.h"
#include "..\include\character.h"

//===================================================
// コンストラクタ
//===================================================
CCharacter::CCharacter() : 
	m_vpModel(),
	m_pMotion(),
	m_pos(Const::VEC3_NULL),
	m_rot(Const::VEC3_NULL),
	m_move(Const::VEC3_NULL),
	m_mtxWorld(Const::MTX_IDENTITY),
	m_fInertia(0.0f),
	m_fGravity(0.0f),
	m_nNumModel(0),
	m_scale(Const::INIT_SCAL)
{
}

//===================================================
// デストラクタ
//===================================================
CCharacter::~CCharacter()
{
	// モデル分回す
	for (auto& model : m_vpModel)
	{
		// モデルの破棄
		model->Uninit();
		model.reset();
	}

	// モーションの破棄
	if (m_pMotion != nullptr)
	{
		m_pMotion->Uninit();
		m_pMotion.reset();
	}
}

//===================================================
// 生成処理
//===================================================
std::unique_ptr<CCharacter> CCharacter::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pMotionFilePath)
{
	// キャラクターの生成
	auto pInstance = std::make_unique<CCharacter>();

	pInstance->m_pos = pos;
	pInstance->m_rot = rot;

	// 初期化処理に失敗したら
	if (FAILED(pInstance->Init(pMotionFilePath)))
	{
		//終了処理
		pInstance->Uninit();
		pInstance = nullptr;
	}
	return pInstance;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CCharacter::Init(const char* pMotionFilePath)
{
	// モーションの読み込み
	m_pMotion = CMotion::Load(pMotionFilePath, m_vpModel);

	// 読み込めなかったら
	if (m_pMotion == nullptr)
	{
		return E_FAIL;
	}

	m_nNumModel = m_vpModel.size();

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CCharacter::Uninit(void)
{
}

//===================================================
// 更新処理
//===================================================
void CCharacter::Update(void)
{
	// 移動量の減衰
	m_move.x += (0.0f - m_move.x) * m_fInertia;
	m_move.z += (0.0f - m_move.z) * m_fInertia;

	// 重力を加算
	m_move.y -= m_fGravity;

	// 移動量の更新
	m_pos += m_move;

	if (m_pMotion != nullptr)
	{
		// モーションの更新処理
		m_pMotion->Update(m_vpModel, m_vpModel.size());
	}
}

//===================================================
// 描画処理
//===================================================
void CCharacter::Draw(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// レンダラーの取得
	CRenderer* pRenderer = pManager->GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScal, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScal);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// モデル分回す
	for (auto& model : m_vpModel)
	{
		// 描画処理
		model->Draw();
	}

	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//===================================================
// パラメータの設定
//===================================================
void CCharacter::SetParameter(const float fInertia, const float fGravity)
{
	m_fInertia = fInertia;
	m_fGravity = fGravity;
}

//===================================================
// スケールの設定
//===================================================
void CCharacter::SetScale(const D3DXVECTOR3& scale)
{
	m_scale = scale;
}
