//===================================================
//
// カメラの追従処理 [follow_camera.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _FOLLOW_CAMERA_H_
#define _FOLLOW_CAMERA_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "camera_system.h"

//***************************************************
// 前方宣言
//***************************************************
class CPlayer;	// プレイヤーのクラス

//***************************************************
// カメラの追従処理のクラスの定義
//***************************************************
class CFollowCamera : public CCameraSystem
{
public:
	CFollowCamera(CCamera* pCamera, CPlayer *pPlayer);
	~CFollowCamera();

	void Update(D3DXVECTOR3& posV, D3DXVECTOR3& posR, D3DXVECTOR3& rot) override;
private:
	D3DXVECTOR3 m_posVDest;	// 目的の視点
	D3DXVECTOR3 m_posRDest;	// 目的の注視点
	CPlayer* m_pPlayer;		// プレイヤークラスへのポインタ
};
#endif