//===================================================
//
// メッシュフィールドと各オブジェクトの当たり判定 [mesh_field_collision_system.h]
// Author:YUTO YOSHIDA
// 
// 判定結果を取得するための関数
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESH_FIELD_COLLISION_SYSTEM_H_
#define _MESH_FIELD_COLLISION_SYSTEM_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "collision_system.h"

//***************************************************
// 前方宣言
//***************************************************
class CPlayer;		// プレイヤークラス
class CMeshField;	// メッシュフィールドのクラス

//***************************************************
// メッシュフィールドと各オブジェクトの当たり判定
//***************************************************
class CMeshFieldCollisionSystem : public CCollisionSystem
{
public:
	CMeshFieldCollisionSystem(CPlayer* pPlayer, CMeshField* pMeshField);
	~CMeshFieldCollisionSystem();

	void Update(void) override;
private:
	CPlayer*	m_pPlayer;		// プレイヤーのクラス
	CMeshField* m_pMeshField;	// メッシュフィールドのクラス
};
#endif