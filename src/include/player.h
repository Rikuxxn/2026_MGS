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

//***************************************************
// 前方宣言
//***************************************************
class CCharacter;	// キャラクタークラス

//***************************************************
// プレイヤーの処理のクラスの定義
//***************************************************
class CPlayer : public CObject
{
public:
	CPlayer();
	~CPlayer();

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

	const D3DXVECTOR3& GetPosition(void) const;
	inline void SetMeshFieldCollisionResult(const CollisionResult::MeshField& result) { m_collisionMeshFieldResult = result; }
private:
	void	KeyboardMove(void);
	void	JoyPadMove	(void);
private:
	CollisionResult::MeshField m_collisionMeshFieldResult; // メッシュフィールドの判定の結果

	std::unique_ptr<CCharacter> m_pCharacter;	// キャラクタークラスへのポインタ
	D3DXVECTOR3 m_rotDest;						// 目的の向き
	float		m_fSpeed;						// 足の速さ
};
#endif