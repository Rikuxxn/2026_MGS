//===================================================
//
// メッシュフィールドと各オブジェクトの当たり判定 [mesh_field_collision_system.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "mesh_field_collision_system.h"
#include "player.h"
#include "mesh_field.h"

//===================================================
// コンストラクタ
//===================================================
CMeshFieldCollisionSystem::CMeshFieldCollisionSystem(CPlayer* pPlayer, CMeshField* pMeshField) : 
	m_pPlayer(pPlayer),
	m_pMeshField(pMeshField)
{
}

//===================================================
// デストラクタ
//===================================================
CMeshFieldCollisionSystem::~CMeshFieldCollisionSystem()
{
}

//===================================================
// 更新処理
//===================================================
void CMeshFieldCollisionSystem::Update(void)
{
	if (m_pMeshField == nullptr)
	{
		return;
	}

	if (m_pPlayer == nullptr)
	{
		return;
	}

	// プレイヤーの位置の取得
	D3DXVECTOR3 playerPos = m_pPlayer->GetPosition();

	// 結果の取得
	CollisionResult::MeshField result;

	// 判定結果の取得
	m_pMeshField->Collision(playerPos, result);
	
	// 判定結果を渡す
	m_pPlayer->SetMeshFieldCollisionResult(result);
	
}