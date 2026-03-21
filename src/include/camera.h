//===================================================
//
// カメラの処理 [camera.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _CAMERA_H_
#define _CAMERA_H_

//***************************************************
// インクルードファイル
//***************************************************
//#include "camera_system_base.h"

//***************************************************
// カメラクラスの定義
//***************************************************
class CCamera
{
public:
	CCamera();
	~CCamera();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);
	void SetCamera(const D3DXVECTOR3& posV, const D3DXVECTOR3& posR, const D3DXVECTOR3& rot);
	//void AddSystem(std::unique_ptr<CCameraSystemBase> pNewSystem);

	inline D3DXVECTOR3 GetRotation(void) const { return m_rot; }
	inline D3DXVECTOR3 GetPosV(void) const { return m_posV; }
	inline D3DXVECTOR3 GetPosR(void) const { return m_posR; }
	inline float GetDistance(void) const { return m_fDistance; }
private:
	void DebugCamera(void);
private:
	//std::vector<std::unique_ptr<CCameraSystemBase>> m_pSystem;	// 拡張機能
	D3DXMATRIX m_mtxView;										// ビューマトリックス
	D3DXMATRIX m_mtxProjection;									// プロジェクションマトリックス
	D3DXVECTOR3 m_posV;											// 視点の座標
	D3DXVECTOR3 m_posR;											// 注視点の座標
	D3DXVECTOR3 m_vecU;											// 上方向ベクトル
	D3DXVECTOR3 m_rot;											// 向き
	float m_fDistance;											// 距離
};
#endif
