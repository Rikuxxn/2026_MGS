//===================================================
//
// メッシュフィールドの当たり判定 [mesh_field_collision.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MESH_FIELD_COLLISION_H_
#define _MESH_FIELD_COLLISION_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "collision_result.h"

//***************************************************
// メッシュフィールドの当たり判定のクラスの定義
//***************************************************
class CMeshFieldCollision
{
public:

	CMeshFieldCollision();
	~CMeshFieldCollision();

	bool Collision(
		const D3DXVECTOR3& targetPos,
		const D3DXVECTOR3& pos,
		const VECTOR2INT& segment,
		const D3DXVECTOR2& size,
		LPDIRECT3DVERTEXBUFFER9& pVtxBuffer,
		LPDIRECT3DINDEXBUFFER9& pIdxBuffer,
		CollisionResult::MeshField& result);
private:
};
#endif
