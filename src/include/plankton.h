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
	// 状態の種類
	enum class State : uint8_t
	{
		Idel = 0,	// 何もしない
		Follow,		// 追従
		BeEaten,	// 食われる
		Max
	};

	CPlankton();
	~CPlankton();

	static CPlankton* Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size);

	HRESULT Init	(void) override;
	void	Uninit	(void) override;
	void	Update	(void) override;
	void	Draw	(void) override;

	void	OnCollisionEnter(IGameObject* other) override;

	void	ReleasePhysics(void);

	Collider* GetCollisionShape(void) const;								// コリジョンの取得

	inline void	GetFollowPosition(const D3DXVECTOR3& followPos) { m_followPos = followPos; }
	State GetState(void) const;
	void SetState(State state) { m_state = state; }

	/// <summary>
	/// 食べられに行く処理
	/// </summary>
	/// <param name="destPos"></param>
	bool ProceedToBeEaten(const D3DXVECTOR3& destPos);

	//inline void SetFollowState(const bool b) 
private:
	// 食べられる演出のパラメータ
	struct EatenData
	{
		D3DXVECTOR3	eatenPos;			// 食べられる位置
		D3DXVECTOR3	startPos;			// 開始位置
		int			nFrameCounter;		// カウンター
	};
private:
	std::shared_ptr<RigidBody>  m_pRigidBody;	// 剛体へのポインタ
	std::shared_ptr<Collider>   m_pShape;		// 当たり判定の形へのポインタ
	D3DXVECTOR3					m_followPos;	// 追従位置
	State						m_state;		// 状態
	EatenData					m_eatenData;	// 食べられる演出のデータ
	float						m_fTime;		// ぐるぐる回る
};
#endif