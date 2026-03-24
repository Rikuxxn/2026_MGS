//===================================================
//
// カメラの処理のクラスの定義 [camera_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _CAMERA_SYSTEM_H_
#define _CAMERA_SYSTEM_H_

//***************************************************
// 前方宣言
//***************************************************
class CCamera;	// カメラのクラスの定義

//***************************************************
// カメラの処理のクラスの定義
//***************************************************
class CCameraSystem
{
public:
	CCameraSystem(CCamera* pCamera) : 
		m_pCamera(pCamera)
	{}

	virtual ~CCameraSystem() = default;
	
	virtual void Update(D3DXVECTOR3& posV, D3DXVECTOR3& posR, D3DXVECTOR3& rot) = 0;
protected:
	inline CCamera* GetCamera(void) { return m_pCamera; }
private:
	CCamera* m_pCamera;	// カメラのクラスへのポインタ
};
#endif