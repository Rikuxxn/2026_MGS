//=============================================================================
//
// ブロックリスト処理 [BlockList.h]
// Author : RIKU TANEKAWA
//
//=============================================================================
#ifndef _BLOCKLIST_H_
#define _BLOCKLIST_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "Block.h"


//*****************************************************************************
// 前方宣言
//*****************************************************************************
class Collider;
class BoxCollider;
class CapsuleCollider;
class CylinderCollider;
class SphereCollider;

//*****************************************************************************
// ボックスブロッククラス
//*****************************************************************************
class CBoxBlock : public CBlock
{
public:
	CBoxBlock();
	~CBoxBlock();

	// コライダー
	std::shared_ptr<Collider> CreateCollisionShape(const D3DXVECTOR3& size) override;

	float GetMass(void) const override { return MASS; }  // 質量の取得
	void OnCollisionEnter(IGameObject* other);
	void OnCollisionStay(IGameObject* other);
	void OnCollisionExit(IGameObject* other);

private:
	static constexpr float MASS = 4.0f;// 質量
};

//*****************************************************************************
// シリンダーブロッククラス
//*****************************************************************************
class CCylinderBlock : public CBlock
{
public:
	CCylinderBlock();
	~CCylinderBlock();

	// コライダー
	std::shared_ptr<Collider> CreateCollisionShape(const D3DXVECTOR3& size) override;

	float GetMass(void) const override { return MASS; }  // 質量の取得

private:
	static constexpr float MASS = 4.0f;// 質量

};

//*****************************************************************************
// スフィアブロッククラス
//*****************************************************************************
class CSphereBlock : public CBlock
{
public:
	CSphereBlock();
	~CSphereBlock();

	// コライダー
	std::shared_ptr<Collider> CreateCollisionShape(const D3DXVECTOR3& size) override;

	float GetMass(void) const override { return MASS; }  // 質量の取得

private:
	static constexpr float MASS = 5.0f;// 質量

};

//*****************************************************************************
// カプセルブロッククラス
//*****************************************************************************
class CCapsuleBlock : public CBlock
{
public:
	CCapsuleBlock();
	~CCapsuleBlock();

	// コライダー
	std::shared_ptr<Collider> CreateCollisionShape(const D3DXVECTOR3& /*size*/) override;

	D3DXVECTOR3 GetAngularFactor(void) const { return ANGULAR_FACTOR; }
	float		GetMass(void) const override { return MASS; }  // 質量の取得

private:
	const D3DXVECTOR3 ANGULAR_FACTOR	= { 0.0f, 0.0f, 0.0f };// 角度係数
	static constexpr float MASS			= 4.0f;// 質量

};

//*****************************************************************************
// 岩ブロッククラス
//*****************************************************************************
class CRockBlock : public CBlock
{
public:
	CRockBlock();
	~CRockBlock();

	// コライダー
	std::shared_ptr<Collider> CreateCollisionShape(const D3DXVECTOR3& size) override;

	float GetMass(void) const override { return MASS; }  // 質量の取得
	//void OnCollisionEnter(IGameObject* other);
	//void OnCollisionStay(IGameObject* other);
	//void OnCollisionExit(IGameObject* other);

private:
	static constexpr float MASS = 4.0f;// 質量
};

#endif

