//===================================================
//
// クジラの処理をするクラス [whole.h]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _WHOLE_H_
#define _WHOLE_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"
#include "GameObject.h"

//***************************************************
// 前方宣言
//***************************************************
class CCharacter;	// キャラクタークラス
class Collider;		// コライダークラス
class RigidBody;	// リジッドボディクラス

//***************************************************
// クジラクラスの定義
//***************************************************
class CWhale : public CObject, public IGameObject
{
public:
	// モーションの種類
	enum MOTIONTYPE
	{
		MOTIONTYPE_NEUTRAL = 0,	// ニュートラル
		MOTIONTYPE_EAT,			// 食べる
		MOTIONTYPE_REACTION,	// プレイヤーが近づいたときのリアクション
		MOTIONTYPE_MAX
	};

	CWhale();
	~CWhale();

	/// <summary>
	/// クジラの生成
	/// </summary>
	/// <param name="位置"></param>
	/// <param name="向き"></param>
	/// <param name="モーションのファイルパス"></param>
	/// <returns>クジラのポインタ</returns>
	static CWhale* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pMotionFilePath);

	HRESULT Init	(void) override;
	void	Uninit	(void) override;
	void	Update	(void) override;
	void	Draw	(void) override;

	Collider* GetCollisionShape(void) const;						// コリジョンの取得

	// Physics処理
	void ReleasePhysics(void);										// Physics削除処理
	void CreatePhysics(D3DXVECTOR3 size, float mass);		// Phtsics生成処理
	void UpdateCollider(D3DXVECTOR3 offset);						// コライダーの位置更新処理

	void OnCollisionEnter(IGameObject* other) override;
	void SetMotionByPlayerDistance(const D3DXVECTOR3& playerPos);
	void SetScal(const D3DXVECTOR3& scale);
	void EatPlankton(void);

	inline void SetDestRotation(const D3DXVECTOR3& dest) { m_rotDest = dest; }

	const D3DXVECTOR3& GetPosition(void) const;
	const D3DXVECTOR3& GetEatPos(void);
private:
	std::unique_ptr<CCharacter> m_pCharacter;				// キャラクタークラスへのポインタ
	std::shared_ptr<RigidBody>  m_pRigidBody;				// 剛体へのポインタ
	std::shared_ptr<Collider>   m_pShape;					// 当たり判定の形へのポインタ
	D3DXVECTOR3					m_rotDest;					// 目的の向き
	float						m_fScalingTime;				// sinカーブ用のカウンター
	int							m_nNumPlankton;				// プランクトンの数
	int							m_nReactionMotionInterval;	// リアクションのモーションのインターバル
	bool						m_bScaling;					// スケーリングするなら
};
#endif