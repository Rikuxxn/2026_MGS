//===================================================
//
// 当たり判定のクラスのインターフェース [collision_system.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _COLLISION_SYSTEM_H_
#define _COLLISION_SYSTEM_H_

//***************************************************
// 当たり判定のクラスのインターフェースの定義
//***************************************************
class CCollisionSystem
{
public:
	CCollisionSystem() = default;
	virtual ~CCollisionSystem() = default;

	virtual void Update(void) = 0;
private:
};
#endif