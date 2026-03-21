//===================================================
//
// メッシュフィールドの当たり判定 [mesh_field_collision.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "mesh_field_collision.h"

//===================================================
// コンストラクタ
//===================================================
CMeshFieldCollision::CMeshFieldCollision()
{
}

//===================================================
// デストラクタ
//===================================================
CMeshFieldCollision::~CMeshFieldCollision()
{
}

//===================================================
// 当たり判定の処理
//===================================================
bool CMeshFieldCollision::Collision(
	const D3DXVECTOR3& targetPos,
	const D3DXVECTOR3& pos,
	const VECTOR2INT& segment,
	const D3DXVECTOR2& size,
	LPDIRECT3DVERTEXBUFFER9& pVtxBuffer,
	LPDIRECT3DINDEXBUFFER9& pIdxBuffer,
	CollisionResult::MeshField& result)
{
	bool bResult = false;

	// 1マスのサイズ
	float fGridSizeU = size.x / static_cast<float>(segment.x);
	float fGridSizeV = size.y / static_cast<float>(segment.y);

	// どこにいるか調べる
	float fCurrentPosU = targetPos.x + (size.x * 0.5f);
	float fCurrentPosV = (size.y * 0.5f) - targetPos.z;

	// 何番目のポリゴンか
	int nPolygonU = static_cast<int>(fCurrentPosU / fGridSizeU);
	int nPolygonV = static_cast<int>(fCurrentPosV / fGridSizeV);

	// 現在のポリゴンのインデックス番号
	int polyIndex = ((nPolygonV * (segment.x - 1) + nPolygonU) * 2) + (nPolygonV * 6);

	// ポリゴン数の設定
	int nNumPolygon = ((segment.x * segment.y) * 2) + (4 * (segment.y - 1));

	// インデックス数の設定
	int nNumIndex = nNumPolygon + 2;

	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	// インデックスバッファへのポインタ
	WORD* pIdx;

	// インデックスバッファのロック
	pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	const int NUM_POLYGON = 2;

	for (int nCnt = 0; nCnt < NUM_POLYGON; nCnt++)
	{
		// 頂点のインデックス
		int nCntVertex = (polyIndex + nCnt);

		// マイナスだったら
		if (nCntVertex < 0 || nCntVertex > nNumIndex - 2) continue;

		// インデックスを取得
		int nIdx0 = pIdx[nCntVertex];
		int nIdx1 = pIdx[nCntVertex + 1];
		int nIdx2 = pIdx[nCntVertex + 2];

		//// オーバーしていたら
		//if (nIdx0 >= nCntVertex || nIdx1 >= nCntVertex || nIdx2 >= nCntVertex) continue;

		// 頂点を取得
		D3DXVECTOR3 vtx0 = pVtx[nIdx0].pos;
		D3DXVECTOR3 vtx1 = pVtx[nIdx1].pos;
		D3DXVECTOR3 vtx2 = pVtx[nIdx2].pos;

		D3DXVECTOR3 edge0 = vtx1 - vtx0; // 辺ベクトル0
		D3DXVECTOR3 edge1 = vtx2 - vtx1; // 辺ベクトル1
		D3DXVECTOR3 edge2 = vtx0 - vtx2; // 辺ベクトル2

		D3DXVECTOR3 normal = {};

		if (nCnt % 2 == 0)
		{
			// 偶数番目の三角形
			D3DXVec3Cross(&normal, &edge0, &edge1);
		}
		else
		{
			// 奇数番目の三角形（順序が逆になっている）
			D3DXVec3Cross(&normal, &edge1, &edge0);
		}

		D3DXVec3Normalize(&normal, &normal);

		D3DXVECTOR3 targetVec0 = targetPos - vtx0;
		D3DXVECTOR3 targetVec1 = targetPos - vtx1;
		D3DXVECTOR3 targetVec2 = targetPos - vtx2;

		D3DXVECTOR3 cross0 = {};
		D3DXVECTOR3 cross1 = {};
		D3DXVECTOR3 cross2 = {};

		if (nCnt % 2 == 0)
		{
			// 三角形の頂点から外積
			D3DXVec3Cross(&cross0, &edge0, &targetVec0);
			D3DXVec3Normalize(&cross0, &cross0);

			D3DXVec3Cross(&cross1, &edge1, &targetVec1);
			D3DXVec3Normalize(&cross1, &cross1);

			D3DXVec3Cross(&cross2, &edge2, &targetVec2);
			D3DXVec3Normalize(&cross2, &cross2);
		}
		else
		{
			// 三角形の頂点から外積
			D3DXVec3Cross(&cross0, &targetVec0, &edge0);
			D3DXVec3Normalize(&cross0, &cross0);

			D3DXVec3Cross(&cross1, &targetVec1, &edge1);
			D3DXVec3Normalize(&cross1, &cross1);

			D3DXVec3Cross(&cross2, &targetVec2, &edge2);
			D3DXVec3Normalize(&cross2, &cross2);
		}

		if (cross0.y >= 0.0f && cross1.y >= 0.0f && cross2.y >= 0.0f)
		{
			// 平面の方程式のDを計算
			float D = -(normal.x * vtx0.x + normal.y * vtx0.y + normal.z * vtx0.z);

			// プレイヤーの位置に基づいて、プレイヤーのY位置を計算
			float PosY = (normal.x * targetPos.x + normal.z * targetPos.z + D) / -normal.y;

			D3DXVECTOR3 vec = vtx0 - targetPos;
			D3DXVec3Normalize(&vec, &vec);

			// プレイヤーがポリゴンの裏側かどうかの判定
			float dot = D3DXVec3Dot(&normal, &vec); // 法線とプレイヤー方向との内積

			if (dot >= 0.0f)
			{
				result.normal = normal;

				// 位置の設定
				result.fHeight = pos.y + PosY;

				bResult = true;
				break;
			}
		}
	}

	//インデックスバッファのアンロック
	pIdxBuffer->Unlock();

	// 頂点バッファをアンロック
	pVtxBuffer->Unlock();

	result.bHit = bResult;

	return bResult;
}
