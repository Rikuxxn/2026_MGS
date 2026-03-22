//=============================================================================
//
// ブロック処理 [Block.cpp]
// Author : RIKU TANEKAWA
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "Block.h"
#include "Manager.h"
#include "algorithm"
#include "BlockList.h"
#include "Collider.h"
#include "RigidBody.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
std::unordered_map<CBlock::TYPE, BlockCreateFunc> CBlock::m_BlockFactoryMap = {};


//=============================================================================
// コンストラクタ
//=============================================================================
CBlock::CBlock(const PRIORITY priority) : CObjectX(priority)
{
	// 値のクリア
	m_col			 = Const::WHITE;			// 色
	m_isDead		 = false;					// 削除予約フラグ
	m_isDynamic		 = false;					// ダイナミックかどうか
	m_pDebug3D		 = nullptr;					// 3Dデバッグへのポインタ
}
//=============================================================================
// 生成処理
//=============================================================================
CBlock* CBlock::Create(const char* pFilepath, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, TYPE type, bool isDynamic)
{
	if (m_BlockFactoryMap.empty())
	{
		// ファクトリー
		InitFactory();
	}

	CBlock* pBlock = nullptr;

	auto it = m_BlockFactoryMap.find(type);
	if (it != m_BlockFactoryMap.end())
	{
		pBlock = it->second();
	}
	else
	{
		pBlock = new CBlock(); // デフォルト基底クラス
	}

	// nullptrだったら
	if (!pBlock)
	{
		return nullptr;
	}

	pBlock->SetPos(pos);
	pBlock->SetRot(rot);
	pBlock->SetSize(size);
	pBlock->SetType(type);
	pBlock->SetPath(pFilepath);
	pBlock->SetIsDynamic(isDynamic);

	// 初期化失敗時
	if (FAILED(pBlock->Init()))
	{
		return nullptr;
	}

	// 大きさからコライダー等を生成
	pBlock->CreatePhysicsFromScale(size);

	return pBlock;
}
//=============================================================================
// ファクトリー
//=============================================================================
void CBlock::InitFactory(void)
{
	// リストを空にする
	m_BlockFactoryMap.clear();

	m_BlockFactoryMap[CBlock::TYPE_BOX]			= []() -> CBlock* { return new CBoxBlock(); };
	m_BlockFactoryMap[CBlock::TYPE_CYLINDER]	= []() -> CBlock* { return new CCylinderBlock(); };
	m_BlockFactoryMap[CBlock::TYPE_SPHERE]		= []() -> CBlock* { return new CSphereBlock(); };
	m_BlockFactoryMap[CBlock::TYPE_CAPSULE]		= []() -> CBlock* { return new CCapsuleBlock(); };
}
//=============================================================================
// 当たり判定の生成処理
//=============================================================================
void CBlock::CreatePhysics(const D3DXVECTOR3& pos, const D3DXVECTOR3& size)
{
	// BoxCollider を作成
	m_pShape = CreateCollisionShape(size);

	// オーナーの設定
	m_pShape->SetOwner(this);

	float mass = (/*m_isEditMode ? 0.0f :*/ (IsDynamicBlock() ? GetMass() : 0.0f));

	D3DXVECTOR3 inertia = Const::VEC3_NULL;

	if (mass != 0.0f)
	{
		m_pShape->calculateLocalInertia(mass, inertia);
	}

	// リジッドボディの生成
	m_pRigidBody = std::make_shared<RigidBody>(m_pShape, mass);

	D3DXVECTOR3 euler = GetRot(); // オイラー角（ラジアン）
	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, euler.y, euler.x, euler.z);

	// 初期位置の設定
	m_pRigidBody->SetTransform(pos, q, GetSize());

	m_pRigidBody->SetIsDynamic(IsDynamicBlock());			// ダイナミックブロックかどうか

	m_pRigidBody->SetLinearFactor(GetLinearFactor());		// 移動方向
	m_pRigidBody->SetAngularFactor(GetAngularFactor());		// 回転方向
	m_pRigidBody->SetRollingFriction(GetRollingFriction());	// 転がり摩擦
	m_pRigidBody->SetFriction(GetFriction());				// 摩擦

	// PhysicsWorldを取得
	auto pPhysicsWorld = CManager::GetInstance()->GetPhysicsWorld();

	// PhysicsWorld に追加
	if (pPhysicsWorld)
	{
		pPhysicsWorld->AddRigidBody(m_pRigidBody);
	}
}
//=============================================================================
// スケールによるコライダーの生成処理
//=============================================================================
void CBlock::CreatePhysicsFromScale(const D3DXVECTOR3& scale)
{
	// モデルの元サイズの取得
	D3DXVECTOR3 modelSize = GetModelSize();

	D3DXVECTOR3 newColliderSize =
	{
		modelSize.x * scale.x,
		modelSize.y * scale.y,
		modelSize.z * scale.z
	};

	CreatePhysics(GetPos(), newColliderSize);// 再生成
}
//=============================================================================
// コライダーの再生成処理
//=============================================================================
void CBlock::RecreatePhysics(void)
{
	if (!m_pRigidBody)
	{
		return;
	}

	// 位置の取得
	D3DXVECTOR3 Pos = GetPos();

	// 削除して再生成
	ReleasePhysics();
	CreatePhysics(Pos, GetModelSize());
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBlock::Init(void)
{
	// オブジェクトXの初期化処理
	CObjectX::Init();

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CBlock::Uninit(void)
{
	ReleasePhysics();

	// オブジェクトXの終了処理
	CObjectX::Uninit();
}
//=============================================================================
// Physicsの破棄
//=============================================================================
void CBlock::ReleasePhysics(void)
{
	auto world = CManager::GetInstance()->GetPhysicsWorld();

	//// 剛体やシェイプを消す前に派生クラス用の特殊処理破棄を呼ぶ
	//OnPhysicsReleased();

	if (m_pShape && world)
	{
		world->RemoveCollider(m_pShape.get());
	}

	// リジッドボディの破棄
	if (m_pRigidBody)
	{
		if (world)
		{
			world->RemoveRigidBody(m_pRigidBody);
		}

		m_pRigidBody = nullptr;
	}

	// シェイプの破棄
	if (m_pShape)
	{
		m_pShape = nullptr;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void CBlock::Update(void)
{
	if (!IsDynamicBlock()/* || IsEditMode()*/)
    {
        // static ブロック
        D3DXVECTOR3 pos = GetPos();
        D3DXVECTOR3 rot = GetRot();
        D3DXVECTOR3 scale = GetSize();

        if (m_pRigidBody)
        {
			// オイラー角 → クォータニオン変換
			D3DXQUATERNION q;
			D3DXQuaternionRotationYawPitchRoll(&q, rot.y, rot.x, rot.z);

			m_pRigidBody->SetTransform(pos, q, scale);

            // 静的なので角速度はリセット
            m_pRigidBody->SetVelocity(Const::VEC3_NULL);
            m_pRigidBody->SetAngularVelocity(Const::VEC3_NULL);
        }
    }
    else
    {
        // dynamic ブロック
		if (!m_pRigidBody)
		{
			return;
		}

        // Rigidbody 側の位置・回転を取得
        D3DXVECTOR3 pos = m_pRigidBody->GetPosition();
        D3DXQUATERNION q = m_pRigidBody->GetOrientation();

        // クォータニオン → マトリックス → オイラー角
        D3DXMATRIX matRot;
        D3DXMatrixRotationQuaternion(&matRot, &q);

        D3DXVECTOR3 euler;
        float sy = -matRot._32;
        sy = std::clamp(sy, -1.0f, 1.0f);
        euler.x = asinf(sy);

        if (fabsf(cosf(euler.x)) > 1e-4f)
        {
            euler.y = atan2f(matRot._31, matRot._33);
            euler.z = atan2f(matRot._12, matRot._22);
        }
        else
        {
            euler.y = 0.0f;
            euler.z = atan2f(-matRot._21, matRot._11);
        }

        static D3DXVECTOR3 prevEuler(0,0,0);
        auto FixAngleJump = [](float prev, float current) -> float
        {
			if (_isnan(current))
			{
				return prev;
			}

            float diff = current - prev;
			if (diff > D3DX_PI)
			{
				current -= CMathConstant::I_DOUBLE * D3DX_PI;
			}
			else if (diff < -D3DX_PI)
			{
				current += CMathConstant::I_DOUBLE * D3DX_PI;
			}

            return current;
        };

        euler.x = FixAngleJump(prevEuler.x, euler.x);
        euler.y = FixAngleJump(prevEuler.y, euler.y);
        euler.z = FixAngleJump(prevEuler.z, euler.z);
        prevEuler = euler;

        SetPos(pos);
        SetRot(euler);
    }
}
//=============================================================================
// 描画処理
//=============================================================================
void CBlock::Draw(void)
{
	// オブジェクトXの描画処理
	CObjectX::Draw();
}
//=============================================================================
// 当たり判定描画処理
//=============================================================================
void CBlock::DrawCollider(void)
{
	// コライダーの描画
	if (auto box = m_pShape->As<BoxCollider>())
	{
		// ボックス
		m_pDebug3D->DrawCollider(box, COLLIDER_COLOR);
	}
	else if (auto cylinder = m_pShape->As<CylinderCollider>())
	{
		// シリンダー
		m_pDebug3D->DrawCollider(cylinder, COLLIDER_COLOR);
	}
	else if (auto sphere = m_pShape->As<SphereCollider>())
	{
		// スフィア
		m_pDebug3D->DrawCollider(sphere, COLLIDER_COLOR);
	}
	else if (auto capsule = m_pShape->As<CapsuleCollider>())
	{
		// カプセル
		m_pDebug3D->DrawCollider(capsule, COLLIDER_COLOR);
	}
}
//=============================================================================
// ワールドマトリックスの取得
//=============================================================================
D3DXMATRIX CBlock::GetWorldMatrix(void)
{
	D3DXMATRIX matScale, matRot, matTrans;

	// スケール行列
	D3DXVECTOR3 scale = GetSize(); // 拡大率
	D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z);

	// 回転行列
	D3DXVECTOR3 rot = GetRot(); // ラジアン角
	D3DXMatrixRotationYawPitchRoll(&matRot, rot.y, rot.x, rot.z);

	// 平行移動行列
	D3DXVECTOR3 pos = GetPos();
	D3DXMatrixTranslation(&matTrans, pos.x, pos.y, pos.z);

	// 合成：S * R * T
	D3DXMATRIX world = matScale * matRot * matTrans;

	return world;
}
//=============================================================================
// ブロック情報保存処理
//=============================================================================
void CBlock::SaveToJson(json& b)
{
	D3DXVECTOR3 degRot = D3DXToDegree(GetRot());
	b["type"]		= m_Type;
	b["pos"]		= { GetPos().x, GetPos().y, GetPos().z };
	b["rot"]		= { degRot.x, degRot.y, degRot.z };
	b["size"]		= { GetSize().x, GetSize().y, GetSize().z };
	b["is_dynamic"] = IsDynamicBlock();
}
//=============================================================================
// ブロック情報読み込み処理
//=============================================================================
void CBlock::LoadFromJson(const json& b)
{
	D3DXVECTOR3 pos(b["pos"][0], b["pos"][1], b["pos"][2]);
	D3DXVECTOR3 degRot(b["rot"][0], b["rot"][1], b["rot"][2]);
	D3DXVECTOR3 size(b["size"][0], b["size"][1], b["size"][2]);

	SetPos(pos);
	SetRot(D3DXToRadian(degRot));
	SetSize(size);
	SetIsDynamic(b["is_dynamic"]);
}
//=============================================================================
// コリジョン生成処理
//=============================================================================
std::shared_ptr<Collider> CBlock::CreateCollisionShape(const D3DXVECTOR3& size)
{
	return std::make_shared <BoxCollider>(size);	// デフォルトはボックス 派生クラスでオーバーライドしてShapeを設定
}
