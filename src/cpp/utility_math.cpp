//===================================================
//
// 計算 [utility_math.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "utility_math.h"
#include "manager.h"
#include "renderer.h"

//===================================================
// 距離の取得
//===================================================
float math::GetDistance(const D3DXVECTOR3& diff)
{
	// 距離を二乗する
	float fSqX = diff.x * diff.x;
	float fSqY = diff.y * diff.y;
	float fSqZ = diff.z * diff.z;
	
	// 距離を求める
	float fOut = sqrtf(fSqX + fSqY + fSqZ);
	
	return fOut;
}

//===================================================
// 目的の方向ベクトルの取得
//===================================================
D3DXVECTOR3 math::GetVector(const D3DXVECTOR3& dest, const D3DXVECTOR3& pos)
{
	// 方向を求める
	D3DXVECTOR3 diff = dest - pos;

	// 正規化する
	D3DXVec3Normalize(&diff, &diff);

	return diff;
}

//===================================================
// 目的までの角度の取得
//===================================================
float math::GetTargetAngle(const D3DXVECTOR3& dest, const D3DXVECTOR3& pos)
{
	D3DXVECTOR3 diff = pos - dest;

	float fAngle = atan2f(diff.x, diff.z);

	return fAngle;
}

//===================================================
// 位置の行列をD3DXVECTOR3型に変換
//===================================================
D3DXVECTOR3 math::GetPositionFromMatrix(const D3DXMATRIX& mtxWorld)
{
	return D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
}

//===================================================
// クウォータ二オンの軸の作成処理
//===================================================
D3DXQUATERNION math::CreateQuaternion(D3DXQUATERNION* pOut, D3DXVECTOR3& axis, float fAngle)
{
	// クウォータ二オン
	D3DXQUATERNION quaternion;

	// 角度を半分にする(クウォータ二オンは半分の角度をもとに計算するため)
	float angle = fAngle / 2.0f;

	D3DXVECTOR3 NormalizeAxis;

	// 軸を正規化する
	D3DXVec3Normalize(&NormalizeAxis, &axis);

	// クウォータ二オンの計算
	quaternion.w = cosf(angle);
	quaternion.x = sinf(angle) * NormalizeAxis.x;
	quaternion.y = sinf(angle) * NormalizeAxis.y;
	quaternion.z = sinf(angle) * NormalizeAxis.z;

	// 計算結果を入れる
	*pOut = quaternion;

	return quaternion;
}

//===================================================
// クウォータ二オンを回転行列に変換する
//===================================================
D3DXMATRIX math::SetMatrixRotationQuaternion(D3DXMATRIX* pOut, D3DXQUATERNION& quaternion)
{
	// 計算用ワールドマトリックス
	D3DXMATRIX outMatrix;

	// 行列を初期化さ
	D3DXMatrixIdentity(&outMatrix);

	// 角度の2乗
	float fSqX = quaternion.x * quaternion.x;
	float fSqY = quaternion.y * quaternion.y;
	float fSqZ = quaternion.z * quaternion.z;
	//float sqW = quaternion.w * quaternion.w;

	// 行列_11 _21 _31の計算
	outMatrix._11 = 1 - (2 * (fSqY + fSqZ));
	outMatrix._21 = (2 * ((quaternion.x * quaternion.y) + (quaternion.w * quaternion.z)));
	outMatrix._31 = (2 * ((quaternion.x * quaternion.z) + (quaternion.w * quaternion.y)));

	// 行列_12 _22 _32の計算
	outMatrix._12 = 2 * ((quaternion.x * quaternion.y) - (quaternion.w * quaternion.z));
	outMatrix._22 = 1 - (2 * (fSqX + fSqZ));
	outMatrix._32 = (2 * ((quaternion.y * quaternion.z) + (quaternion.w * quaternion.x)));

	// 行列_13 _23 _33の計算
	outMatrix._13 = 2 * ((quaternion.x * quaternion.z) - (quaternion.w * quaternion.y));
	outMatrix._23 = 2 * ((quaternion.y * quaternion.z) - (quaternion.w * quaternion.x));
	outMatrix._33 = 1 - (2 * (fSqX + fSqY));

	outMatrix._14 = 0.0f;
	outMatrix._24 = 0.0f;
	outMatrix._34 = 0.0f;

	outMatrix._41 = 0.0f;
	outMatrix._42 = 0.0f;
	outMatrix._43 = 0.0f;
	outMatrix._44 = 1.0f;

	// 計算結果を代入
	*pOut = outMatrix;

	// 計算したマトリックスを返す
	return outMatrix;
}

//===================================================
// 回転行列をオイラー角に変換
//===================================================
D3DXVECTOR3 math::MatrixToEulerXYZ(const D3DXMATRIX& mtxWorld)
{
	D3DXVECTOR3 rot;

	if (fabsf(mtxWorld._32) < 1.0f)
	{
		rot.x = asinf(-mtxWorld._32);
		rot.y = atan2f(mtxWorld._31,mtxWorld._33);
		rot.z = atan2f(mtxWorld._12, mtxWorld._22);
	}
	else
	{
		rot.x = (mtxWorld._32 <= -1.0f) ? D3DX_PI * 0.5f : -D3DX_PI * 0.5f;
		rot.y = atan2f(-mtxWorld._13, mtxWorld._11);
		rot.z = 0.0f;
	}

	return rot;
}

//===================================================
// 向いている方向の取得
//===================================================
D3DXVECTOR3 math::GetForward(const D3DXQUATERNION& quat)
{
	D3DXVECTOR3 forward;

	// 向いている方向を求める
	forward.x = 2.0f * (quat.x * quat.z + quat.w * quat.y);
	forward.y = 2.0f * (quat.y * quat.z - quat.w * quat.x);
	forward.z = 1.0f - 2.0f * (quat.x * quat.x + quat.y * quat.y);

	return forward;
}

//===================================================
// 三角形の内部にいるかどうか(バリセントリック法で三角形の内部にいるか求める)
//===================================================
bool math::InTriangle(const D3DXVECTOR3& pos, const D3DXVECTOR3& vtx0, const D3DXVECTOR3& vtx1, const D3DXVECTOR3& vtx2)
{
	

	// ベクトルを引く
	D3DXVECTOR3 v0 = vtx1 - vtx0;
	D3DXVECTOR3 v1 = vtx2 - vtx0;
	D3DXVECTOR3 v2 = pos - vtx0;

	float d00 = D3DXVec3Dot(&v0, &v0);
	float d01 = D3DXVec3Dot(&v0, &v1);
	float d11 = D3DXVec3Dot(&v1, &v1);
	float d20 = D3DXVec3Dot(&v2, &v0);
	float d21 = D3DXVec3Dot(&v2, &v1);

	float denom = (d00 * d11) - (d01 * d01);

	// ゼロ割り対策
	if (fabsf(denom) < 1e-6f)
	{
		return false;
	}

	float v = ((d11 * d20) - (d01 * d21)) / denom;

	float w = ((d00 * d21) - (d01 * d20)) / denom;

	float u = 1.0f - v - w;

	return (u >= 0 && v >= 0 && w >= 0);
}

//===================================================
// 内積の取得
//===================================================
float math::Dot(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2)
{
	// 内積を求める
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

//===================================================
// 外積の取得
//===================================================
D3DXVECTOR3 math::Cross(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2)
{
	D3DXVECTOR3 cross;

	// 外積を求める
	cross.x = (v1.y * v2.z) - (v1.z * v2.y);
	cross.y = (v1.z * v2.x) - (v1.x * v2.z);
	cross.z = (v1.x * v2.y) - (v1.y * v2.x);

	return cross;
}

//===================================================
// マウスのレイの取得
//===================================================
void math::GetMouseRay(D3DXVECTOR3 *pRayOrigin,D3DXVECTOR3 *pOutDir)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(FindWindowA(CLASS_NAME, WINDOW_NAME), &mousePos); // ウィンドウ内座標に変換

	// プロジェクションマトリックスとビューマトリックス
	D3DXMATRIX matView, matproj;

	// プロジェクションマトリックスとビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &matView);
	pDevice->GetTransform(D3DTS_PROJECTION, &matproj);

	RECT rect;
	GetClientRect(FindWindowA(CLASS_NAME, nullptr), &rect);

	int screenWidth = rect.right - rect.left;
	int screenHeight = rect.bottom - rect.top;

	// NDC (-1～1)に変換
	float pX = ((2.0f * mousePos.x) / screenWidth) - 1.0f;
	float pY = ((2.0f * mousePos.y) / screenHeight) - 1.0f;
	pY *= -1.0f;

	// ビュー空間のNear/Far
	D3DXVECTOR3 vNear = D3DXVECTOR3(pX, pY, 0.0f);
	D3DXVECTOR3 vFar = D3DXVECTOR3(pX, pY, 1.0f);

	// 合成行列の逆行列
	D3DXMATRIX matInvVP;
	D3DXMATRIX matVP = matView * matproj;
	D3DXMatrixInverse(&matInvVP, nullptr, &matVP);

	// ワールド座標に変換
	D3DXVec3TransformCoord(&vNear, &vNear, &matInvVP);
	D3DXVec3TransformCoord(&vFar, &vFar, &matInvVP);

	// レイの原点
	*pRayOrigin = vNear;
	D3DXVECTOR3 ray = vFar - vNear;
	D3DXVec3Normalize(pOutDir, &ray);
}

//===================================================
// ランダムな値の選出(int型)
//===================================================
int math::Random(const int nMin, const int nMax)
{
	std::random_device randomDevice;
	static std::mt19937 mt(randomDevice());

	// ランダムな範囲の指定
	std::uniform_int_distribution<int> dist(nMin, nMax);

	return dist(mt);
}

//===================================================
// スクリーン座標をワールド座標に変換
//===================================================
D3DXVECTOR3 math::ScreenToWorld(const D3DXVECTOR2& screen, const float fWorldY, const D3DVIEWPORT9& vp, const D3DXMATRIX& view, const D3DXMATRIX& proj)
{
	// 手前の位置
	D3DXVECTOR3 nearPos(screen.x, screen.y, 0.0f);

	// 奥の位置
	D3DXVECTOR3 farPos(screen.x, screen.y, 1.0f);

	D3DXVECTOR3 nearWorld, farWorld;

	D3DXVec3Unproject(&nearWorld, &nearPos, &vp, &proj, &view, nullptr);
	D3DXVec3Unproject(&farWorld, &farPos, &vp, &proj, &view, nullptr);

	D3DXVECTOR3 dir = farWorld - nearWorld;
	float t = (fWorldY - nearWorld.y) / dir.y;

	return nearWorld + dir * t;
}

//===================================================
// 角度の正規化
//===================================================
void math::NormalizeRot(D3DXVECTOR3* pRot)
{
	if (pRot->x < -D3DX_PI)
	{
		pRot->x += D3DX_PI * 2.0f;
	}
	else if (pRot->x > D3DX_PI)
	{
		pRot->x += -D3DX_PI * 2.0f;
	}

	if (pRot->y < -D3DX_PI)
	{
		pRot->y += D3DX_PI * 2.0f;
	}
	else if (pRot->y > D3DX_PI)
	{
		pRot->y += -D3DX_PI * 2.0f;
	}

	if (pRot->z < -D3DX_PI)
	{
		pRot->z += D3DX_PI * 2.0f;
	}
	else if (pRot->z > D3DX_PI)
	{
		pRot->z += -D3DX_PI * 2.0f;
	}

}

//===================================================
// 角度の正規化
//===================================================
void math::NormalizeRot(float* pRot)
{
	if ((*pRot) < -D3DX_PI)
	{
		(*pRot) += D3DX_PI * 2.0f;
	}
	else if ((*pRot) > D3DX_PI)
	{
		(*pRot) += -D3DX_PI * 2.0f;
	}
}

//===================================================
// 差分の正規化
//===================================================
void math::NormalizeDiffRot(const float fDiff, float* pRot)
{
	if (fDiff <= -D3DX_PI)
	{
		(*pRot) += -D3DX_PI * 2.0f;
	}
	else if (fDiff >= D3DX_PI)
	{
		(*pRot) += D3DX_PI * 2.0f;
	}
}

//===================================================
// ワールドマトリックスの逆行列を返す関数
//===================================================
D3DXMATRIX math::Inverse(const D3DXMATRIX& mtxWorld)
{
	// 逆行列
	D3DXMATRIX Inverse;

	// 逆行列を求める
	D3DXMatrixInverse(&Inverse, nullptr, &mtxWorld);

	return Inverse;
}

//===================================================
// バウンド処理
//===================================================
D3DXVECTOR3 math::Bound(const D3DXVECTOR3& move, const D3DXVECTOR3& normal, const float fRestitution)
{
	// 反射ベクトルを求める
	float dot = D3DXVec3Dot(&move, &normal);

	// 反射の移動量の作成
	D3DXVECTOR3 newMove = move - (dot * 2.0f) * normal;

	// 移動量の設定
	D3DXVECTOR3 outMove = newMove * fRestitution;

	return outMove;
}
