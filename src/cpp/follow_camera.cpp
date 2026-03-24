//===================================================
//
// カメラの追従処理 [follow_camera.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "follow_camera.h"
#include "manager.h"
#include "camera.h"
#include "input_system.h"
#include "player.h"

//***************************************************
// 定数宣言
//***************************************************
namespace FollowCameraConst
{
	constexpr float FOLLOW_ALPHA = 0.1f;		// 補間係数
}

//===================================================
// コンストラクタ
//===================================================
CFollowCamera::CFollowCamera(CCamera* pCamera, CPlayer* pPlayer) : 
	CCameraSystem(pCamera),
	m_pPlayer(pPlayer),
	m_posVDest(Const::VEC3_NULL),
	m_posRDest(Const::VEC3_NULL)
{
}

//===================================================
// デストラクタ
//===================================================
CFollowCamera::~CFollowCamera()
{
}

//===================================================
// 更新処理
//===================================================
void CFollowCamera::Update(D3DXVECTOR3& posV, D3DXVECTOR3& posR, D3DXVECTOR3& rot)
{
	// カメラの取得
	CCamera* pCamera = CCameraSystem::GetCamera();

	// 取得できなかったら処理しない
	if (pCamera == nullptr) return;

#ifdef _DEBUG

	static bool bFollow = true;

	// カメラ追従の切り替え
	if (InputSystem::DebugEnableFollowCamera())
	{
		bFollow = bFollow ? false : true;
	}

	if (bFollow == false)
	{
		return;
	}

#endif // _DEBUG

	// プレイヤーがnullだったら
	if (m_pPlayer == nullptr)
	{
		return;
	}

	// 距離の取得
	float fDistance = pCamera->GetDistance();

	// プレイヤーの位置
	D3DXVECTOR3 playerPos = m_pPlayer->GetPosition();

	// カメラの注視点初期座標を設定
	m_posRDest.x = playerPos.x;
	m_posRDest.y = playerPos.y;
	m_posRDest.z = playerPos.z;

	// カメラの視点の情報
	m_posVDest.x = playerPos.x - sinf(rot.x) * sinf(rot.y) * fDistance;
	m_posVDest.y = (playerPos.y + fDistance) - cosf(rot.x) * fDistance;
	m_posVDest.z = playerPos.z - sinf(rot.x) * cosf(rot.y) * fDistance;

	// 注視点の補間
	posR.x += ((m_posRDest.x - posR.x) * FollowCameraConst::FOLLOW_ALPHA);
	posR.y += ((m_posRDest.y - posR.y) * FollowCameraConst::FOLLOW_ALPHA);
	posR.z += ((m_posRDest.z - posR.z) * FollowCameraConst::FOLLOW_ALPHA);

	// 視点の補間
	posV.x += ((m_posVDest.x - posV.x) * FollowCameraConst::FOLLOW_ALPHA);
	posV.y += ((m_posVDest.y - posV.y) * FollowCameraConst::FOLLOW_ALPHA);
	posV.z += ((m_posVDest.z - posV.z) * FollowCameraConst::FOLLOW_ALPHA);
}