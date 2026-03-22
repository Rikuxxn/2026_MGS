//=============================================================================
//
// ブロック処理 [Block.h]
// Author : RIKU TANEKAWA
//
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "ObjectX.h"
#include "DebugProc3D.h"
#include "json.hpp"
#include "GameObject.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CBlock;
class Collider;
class RigidBody;
class PhysicsWorld;

//*****************************************************************************
// 型定義
//*****************************************************************************
using BlockCreateFunc = std::function<CBlock* ()>;

// JSONの使用
using json = nlohmann::json;

//*****************************************************************************
// ブロッククラス
//*****************************************************************************
class CBlock : public CObjectX, public IGameObject
{
public:
	CBlock(const PRIORITY priority = PRIORITY_MODEL);
	virtual ~CBlock() = default;

	//*****************************************************************************
	// ブロックの種類
	//*****************************************************************************
	enum TYPE
	{
		TYPE_BOX = 0,
		TYPE_CYLINDER,
		TYPE_SPHERE,
		TYPE_CAPSULE,
		TYPE_MAX
	};

	static CBlock* Create(const char* pFilepath, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, TYPE type, bool isDynamic);	// ブロックの生成
	static void InitFactory(void);
	virtual HRESULT Init(void);
	void Kill(void) { m_isDead = true; }												// ブロック削除
	void Uninit(void);
	void ReleasePhysics(void);
	void Update(void);
	void Draw(void);
	void DrawCollider(void);
	void CreatePhysicsFromScale(const D3DXVECTOR3& scale);								// ブロックスケールによるコライダーの生成
	void CreatePhysics(const D3DXVECTOR3& pos, const D3DXVECTOR3& size);				// コライダーの生成
	void RecreatePhysics(void);
	virtual std::shared_ptr<Collider> CreateCollisionShape(const D3DXVECTOR3& size);
	virtual void SaveToJson(json& b);
	virtual void LoadFromJson(const json& b);

	//*****************************************************************************
	// flagment関数
	//*****************************************************************************
	bool			IsDead			(void) const { return m_isDead; }					// 削除予約の取得
	virtual bool	IsDynamicBlock	(void) const { return m_isDynamic; }				// 動的ブロックの判別

	//*****************************************************************************
	// setter関数
	//*****************************************************************************
	void SetType		(TYPE type)			{ m_Type = type; }										// タイプの設定
	void SetIsDynamic	(bool isDynamic)	{ m_isDynamic = isDynamic; }

	//*****************************************************************************
	// getter関数
	//*****************************************************************************
	RigidBody*			GetRigidBody		(void)			{ return m_pRigidBody.get(); }
	TYPE				GetType				(void) const	{ return m_Type; }						// タイプの取得
	D3DXMATRIX			GetWorldMatrix		(void);
	virtual D3DXVECTOR3 GetLinearFactor		(void) const	{ return DEFAULT_LINEAR_FACTOR; }
	virtual D3DXVECTOR3 GetAngularFactor	(void) const	{ return DEFAULT_ANGLAR_FACTOR; }
	virtual int			GetCollisionFlags	(void) const	{ return 0; }							// デフォルトはフラグなし
	virtual float		GetMass				(void) const	{ return DEFAULT_MASS; }				// 質量の取得
	virtual float		GetRollingFriction	(void) const	{ return DEFAULT_ROLLING_FRICTION; }	// 回転摩擦の取得
	virtual float		GetFriction			(void) const	{ return DEFAULT_FRICTION; }			// 摩擦の取得

private:
	static constexpr float	DEFAULT_MASS				= 1.0f;							// デフォルトの質量
	static constexpr float	DEFAULT_ROLLING_FRICTION	= 1.7f;							// デフォルトの回転摩擦
	static constexpr float	DEFAULT_FRICTION			= 2.5f;							// デフォルトの摩擦
	const D3DXVECTOR3		DEFAULT_LINEAR_FACTOR		= { 1.0f, 1.0f, 1.0f };			// デフォルト線形係数
	const D3DXVECTOR3		DEFAULT_ANGLAR_FACTOR		= { 1.0f, 1.0f, 1.0f };			// デフォルト角度係数
	const D3DXCOLOR			SELECTED_COLOR				= { 1.0f, 0.0f, 0.0f, 0.6f };	// ブロック選択時の色
	const D3DXCOLOR			COLLIDER_COLOR				= { 0.0f, 1.0f, 0.3f, 1.0f };	// コライダーの色

	static std::unordered_map<TYPE, BlockCreateFunc>	m_BlockFactoryMap;				// ファクトリー
	std::shared_ptr<RigidBody>							m_pRigidBody;					// リジッドボディ
	std::shared_ptr<Collider>							m_pShape;						// コライダー
	CDebugProc3D*										m_pDebug3D;						// 3Dデバッグ表示へのポインタ
	D3DXCOLOR											m_col;							// アルファ値
	bool												m_isDead;						// 削除予約フラグ
	bool												m_isDynamic;					// 動的ブロックかどうか
	TYPE												m_Type;							// 種類

};

#endif