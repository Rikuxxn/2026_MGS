//===================================================
//
// 計算 [utility_math.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _UTILITY_MATH_H_
#define _UTILITY_MATH_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "random"

//***************************************************
// 計算用namespaceの定義
//***************************************************
namespace math
{
	// 範囲内に収める
	template <class T> T Clamp(const T value, const T min, const T max)
	{
		if (value < min) return min;

		if (value > max) return max;

		return value;
	};

	// 範囲内にをループする
	template <class T> T Wrap(const T value, const T min, const T max)
	{
		if (value < min) return max;

		if (value > max) return min;

		return value;
	};


	// 目的の値までの補間
	template <class T> T LerpDest(const T dest, const T value, const float fCoef)
	{
		T out = value + (dest - value) * fCoef;

		return out;
	};

	// 目的の値までの補間
	template <class T> T LerpDiff(const T offset, const T Diff, const float fRate)
	{
		T out = offset + (Diff * fRate);

		return out;
	};

	/// <summary>
	/// 範囲内かどうか
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="現在の値"></param>
	/// <param name="min"></param>
	/// <param name="max"></param>
	/// <returns>判定結果</returns>
	template <class T> bool IsInRange(const T value, const T min, const T max)
	{
		if (value >= min && value <= max)
		{
			return true;
		}

		return false;
	};

	/// <summary>
	/// ランダムな値を取得する関数
	/// </summary>
	/// <typeparam name="int型以外"></typeparam>
	/// <param name="min"></param>
	/// <param name="max"></param>
	/// <returns>範囲内の値</returns>
	template <class T> T Random(T min, T max)
	{
		if (min > max) std::swap(min, max);

		// int型は使えません
		static_assert(!std::is_same<T, int>::value, "********************<int> type is not allowrd use ***********************");

		std::random_device randomDevice;
		static std::mt19937 mt(randomDevice());

		// ランダムな範囲の指定
		std::uniform_real_distribution<T> dist(min, max);

		return dist(mt);
	}

	float GetDistance(const D3DXVECTOR3& diff);
	/// <summary>
	/// 単位ベクトルの取得
	/// </summary>
	/// <param name="目的の位置"></param>
	/// <param name="自分の位置"></param>
	/// <returns>正規化ベクトル</returns>
	D3DXVECTOR3 GetVector(const D3DXVECTOR3& dest, const D3DXVECTOR3& pos);
	float GetTargetAngle(const D3DXVECTOR3& dest, const D3DXVECTOR3& pos); // 目標までの角度の取得
	D3DXVECTOR3 GetPositionFromMatrix(const D3DXMATRIX& mtxWorld);		 // MATRIXの位置の情報をD3DXVECTOR3型に変換する
	D3DXQUATERNION CreateQuaternion(D3DXQUATERNION* pOut, D3DXVECTOR3& axis, float fAngle);
	D3DXMATRIX SetMatrixRotationQuaternion(D3DXMATRIX* pOut, D3DXQUATERNION& quaternion);
	D3DXVECTOR3 MatrixToEulerXYZ(const D3DXMATRIX& mtxWorld);			// 回転行列をオイラー角に変換
	D3DXVECTOR3 GetForward(const D3DXQUATERNION& quat);
	bool InTriangle(const D3DXVECTOR3& pos, const D3DXVECTOR3& vtx0, const D3DXVECTOR3& vtx1, const D3DXVECTOR3& vtx2);
	float Dot(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2);
	D3DXVECTOR3 Cross(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2);

	/// <summary>
	/// マウスのレイの取得
	/// </summary>
	/// <param name="pRayOrigin"></param>
	/// <param name="OutDir"></param>
	void GetMouseRay(D3DXVECTOR3* pRayOrigin, D3DXVECTOR3* pOutDir);

	int Random(const int nMin,const int nMax);
	/// <summary>
	/// スクリーン座標をワールド座標に変換
	/// </summary>
	/// <param name="screen"></param>
	/// <param name="fWorldY"></param>
	/// <param name="vp"></param>
	/// <param name="view"></param>
	/// <param name="proj"></param>
	/// <returns>変換座標</returns>
	D3DXVECTOR3 ScreenToWorld(const D3DXVECTOR2& screen, const float fWorldY, const D3DVIEWPORT9& vp, const D3DXMATRIX& view, const D3DXMATRIX& proj);

	void NormalizeRot(D3DXVECTOR3* pRot);
	void NormalizeRot(float* pRot);
	void NormalizeDiffRot(const float fDiff, float* pRot);

	D3DXMATRIX Inverse(const D3DXMATRIX& mtxWorld);
	D3DXVECTOR3 Bound(const D3DXVECTOR3& move, const D3DXVECTOR3& normal, const float fRestitution);
};
#endif