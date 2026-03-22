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
#include "State.h"
#include "RigidBody.h"
#include "Collider.h"

//***************************************************
// 前方宣言
//***************************************************
class CCharacter;	// キャラクタークラス
class CDebugProc3D;	// 3Dデバッグ表示クラス

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
		D3DXVECTOR3 moveDir;      // 前後移動ベクトル
	};

	/// <summary>
	/// プレイヤーの生成
	/// </summary>
	/// <param name="位置"></param>
	/// <param name="向き"></param>
	/// <param name="モーションのファイルパス"></param>
	/// <returns>プレイヤーのポインタ</returns>
	static CPlayer* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pMotionFilePath);

	HRESULT Init		(void) override;
	void	Uninit		(void) override;
	void	Update		(void) override;
	void	Draw		(void) override;

<<<<<<< HEAD
	// Physics処理
	void ReleasePhysics	(void);										// Physics削除処理
	void CreatePhysics	(float radius, float height, float mass);	// Phtsics生成処理
	void UpdateCollider	(D3DXVECTOR3 offset);						// コライダーの位置更新処理
	void SetPhysicsMove	(D3DXVECTOR3 move);							// 物理用移動量の設定処理
=======
	const D3DXVECTOR3& GetPosition(void) const;

	inline void SetMeshFieldCollisionResult(const CollisionResult::MeshField& result) { m_collisionMeshFieldResult = result; }
private:
	void	KeyboardMove(void);
	void	JoyPadMove	(void);
private:
	CollisionResult::MeshField m_collisionMeshFieldResult; // メッシュフィールドの判定の結果
>>>>>>> 6c982a355b5cb93e3e18d3d2d0fc6140cb6317d8

	// 減速処理
	void ApplyDeceleration(void);

	RigidBody*	GetRigidBody		(void) const { return m_pRigidBody.get(); }	// 剛体の取得
	Collider*	GetCollisionShape	(void) const { return m_pShape.get(); }		// コリジョンの取得
	const D3DXVECTOR3& GetPosition	(void) const;
	D3DXVECTOR3 GetMove(void) const;
	D3DXVECTOR3 GetColliderPos		(void) const { return m_colliderPos; }
	bool GetIsMoving(void) const { return m_isMoving; }

	inline void SetMeshFieldCollisionResult(const CollisionResult::MeshField& result) { m_collisionMeshFieldResult = result; }
	InputData GatherInput(void);

	// ステート用にフラグ更新
	void UpdateMovementFlags(const D3DXVECTOR3& moveDir)
	{
		m_isMoving = (moveDir.x != 0.0f || moveDir.z != 0.0f);
	}

public:
	static constexpr float SPEED				= 100.0f;	// 移動スピード
	static constexpr float DECELERATION_RATE	= 0.8f;		// 減速率
	static constexpr float ACCEL_RATE			= 0.15f;	// イージング率
private:
	static constexpr float	MASS			= 1.0f;		// 質量
	static constexpr float	MAX_GRAVITY		= -0.26f;	// 重力加速度
	static constexpr float	CAPSULE_RADIUS	= 16.5f;	// カプセルコライダーの半径
	static constexpr float	CAPSULE_HEIGHT	= 40.0f;	// カプセルコライダーの高さ
private:
	CollisionResult::MeshField m_collisionMeshFieldResult;	// メッシュフィールドの判定の結果

	std::unique_ptr<CCharacter> m_pCharacter;				// キャラクタークラスへのポインタ
	std::shared_ptr<RigidBody>  m_pRigidBody;				// 剛体へのポインタ
	std::shared_ptr<Collider>   m_pShape;					// 当たり判定の形へのポインタ
	CDebugProc3D*				m_pDebug3D;					// 3Dデバッグ表示へのポインタ
	D3DXVECTOR3                 m_colliderPos;				// カプセルコライダーの位置
	D3DXVECTOR3					m_rotDest;					// 目的の向き
	float						m_fSpeed;					// 足の速さ
	bool						m_isMoving;					// 移動しているか

	StateMachine<CPlayer>		m_stateMachine;				// ステートを管理するクラスのインスタンス
};
#endif