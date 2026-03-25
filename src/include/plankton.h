//===================================================
//
// プランクトンの処理 [plankton.h]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _PLANKTON_H_
#define _PLANKTON_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "effect.h"
#include "GameObject.h"

//***************************************************
// 前方宣言
//***************************************************
class Collider;		// コライダークラス
class PhysicsWorld;	// 物理世界クラス
class RigidBody;	// 剛体

//***************************************************
// プランクトンクラスの定義
//***************************************************
class CPlankton : public CObjectBillboard, public IGameObject
{
public:
	CPlankton();
	~CPlankton();

	static CPlankton* Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size);

	HRESULT Init	(void) override;
	void	Uninit	(void) override;
	void	Update	(void) override;
	void	Draw	(void) override;

	void	OnCollisionEnter(IGameObject* other) override;
	inline void	GetFollowPosition(const D3DXVECTOR3& followPos) { m_followPos = followPos; }
	inline bool GetFollowState(void) const { return m_bFollow; }
private:
	std::shared_ptr<RigidBody>  m_pRigidBody;	// 剛体へのポインタ
	std::shared_ptr<Collider>   m_pShape;		// 当たり判定の形へのポインタ
	D3DXVECTOR3					m_followPos;	// 追従位置
	bool						m_bFollow;		// 追従するかどうか
};
#endif