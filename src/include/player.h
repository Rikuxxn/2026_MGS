//===================================================
//
// プレイヤーの処理 [player.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"
#include "collision_result.h"
#include "GameObject.h"
#include "State.h"

//***************************************************
// 前方宣言
//***************************************************
class CCharacter;	// キャラクタークラス
class CDebugProc3D;	// 3Dデバッグ表示クラス
class Collider;		// コライダークラス
class PhysicsWorld;	// 物理世界クラス
class RigidBody;	
class CPlankton;	// プランクトンクラス
class CWhale;		// クジラ

//***************************************************
// プレイヤーの処理のクラスの定義
//***************************************************
class CPlayer : public CObject, public IGameObject
{
public:
	CPlayer();
	~CPlayer();

	// 入力データ構造体
	struct InputData
	{
		D3DXVECTOR3 moveDir = Const::VEC3_NULL;     // 前後移動ベクトル
		bool		isJump	= false;				// ジャンプ入力
	};

	/// <summary>
	/// プレイヤーの生成
	/// </summary>
	/// <param name="位置"></param>
	/// <param name="向き"></param>
	/// <param name="モーションのファイルパス"></param>
	/// <returns>プレイヤーのポインタ</returns>
	static CPlayer* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pMotionFilePath);

	HRESULT Init			(void) override;
	void	Uninit			(void) override;
	void	Update			(void) override;
	void	Draw			(void) override;

	// Physics処理
	D3DXMATRIX GetWorldMatrix(void);
	void ReleasePhysics		(void);										// Physics削除処理
	void CreatePhysics		(D3DXVECTOR3 size, float mass);				// Phtsics生成処理
	void UpdateCollider		(D3DXVECTOR3 offset);						// コライダーの位置更新処理
	void SetPhysicsMove		(D3DXVECTOR3 move);							// 物理用移動量の設定処理
	void ApplyDeceleration	(void);										// 減速処理
	bool OnGround			(PhysicsWorld* world, float rayLength);		// 接地判定

	RigidBody*			GetRigidBody		(void) const { return m_pRigidBody.get(); }	// 剛体の取得
	Collider*			GetCollisionShape	(void) const;								// コリジョンの取得
	const D3DXVECTOR3&	GetPosition			(void) const;
	D3DXVECTOR3			GetMove				(void) const;
	D3DXVECTOR3			GetColliderPos		(void) const { return m_colliderPos; }
	bool				GetIsMoving			(void) const { return m_isMoving; }
	bool				GetOnGround			(void) { return m_bOnGround; }
	bool				GetIsJumping		(void) { return m_isJumping; }
	InputData			GatherInput			(void);
	inline void			OnHitWhale			(CWhale* pWhale) { m_pWhale = pWhale; }

	inline void SetMeshFieldCollisionResult(const CollisionResult::MeshField& result) { m_collisionMeshFieldResult = result; }

	/// <summary>
	/// プランクトンの追加
	/// </summary>
	/// <param name="pPlankton"></param>
	void RegisterPlankton			(CPlankton* pPlankton);
	void SetIsJumping(bool flag) { m_isJumping = flag; }

	// ステート用にフラグ更新
	void UpdateMovementFlags(const D3DXVECTOR3& moveDir)
	{
		m_isMoving = (moveDir.x != 0.0f || moveDir.z != 0.0f);
	}

public:
	static constexpr float SPEED				= 500.0f;	// 移動スピード
	static constexpr float MAX_JUMP_POWER		= 250.0f;	// ジャンプ初速
	static constexpr float DECELERATION_RATE	= 0.85f;	// 減速率
	static constexpr float ACCEL_RATE			= 0.15f;	// イージング率
private:
	static constexpr float	MASS				= 1.0f;		// 質量
	static constexpr float	MAX_GRAVITY			= -0.26f;	// 重力加速度
	static constexpr float	CAPSULE_RADIUS		= 16.5f;	// カプセルコライダーの半径
	static constexpr float	CAPSULE_HEIGHT		= 40.0f;	// カプセルコライダーの高さ
private:
	void UpdatePlankton(void);
	void UpdateWhale(void);
private:
	CollisionResult::MeshField m_collisionMeshFieldResult;	// メッシュフィールドの判定の結果

	std::list<CPlankton*>		m_pHasPlanktonList;			// 自分が持っているプランクトンのリスト

	std::unique_ptr<CCharacter> m_pCharacter;				// キャラクタークラスへのポインタ
	std::shared_ptr<RigidBody>  m_pRigidBody;				// 剛体へのポインタ
	std::shared_ptr<Collider>   m_pShape;					// 当たり判定の形へのポインタ

	CWhale*						m_pWhale;					// クジラクラスへのポインタ(参照用)				
	D3DXVECTOR3                 m_colliderPos;				// カプセルコライダーの位置
	D3DXVECTOR3					m_rotDest;					// 目的の向き
	float						m_fSpeed;					// 足の速さ
	bool						m_isMoving;					// 移動しているか
	bool						m_bOnGround;				// 接地フラグ
	bool						m_isJumping;				// ジャンプ中フラグ

	StateMachine<CPlayer>		m_stateMachine;				// ステートを管理するクラスのインスタンス
};
#endif