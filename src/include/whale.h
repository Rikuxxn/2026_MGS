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
		MOTIONTYPE_NEUTRAL = 0,
		MOTIONTYPE_EAT,
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

private:
	std::unique_ptr<CCharacter> m_pCharacter;	// キャラクタークラスへのポインタ
	std::shared_ptr<RigidBody>  m_pRigidBody;	// 剛体へのポインタ
	std::shared_ptr<Collider>   m_pShape;		// 当たり判定の形へのポインタ
};
#endif