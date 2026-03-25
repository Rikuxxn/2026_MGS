//=============================================================================
//
// 物理世界処理 [PhysicsWorld.h]
// Author : RIKU TANEKAWA
//
//=============================================================================
#ifndef _PHYSICSWORLD_H_// このマクロ定義がされていなかったら
#define _PHYSICSWORLD_H_// 2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "unordered_set"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class Collider;
class BoxCollider;
class CapsuleCollider;
class CylinderCollider;
class SphereCollider;
class RigidBody;

//=============================================================================
// Physics World
//=============================================================================
class PhysicsWorld
{
public:
    PhysicsWorld() : m_Gravity(0, DEFAULT_GRAVITY, 0) {}  // デフォルト重力

    // レイ構造体
    struct Ray
    {
        D3DXVECTOR3 origin;     // 原点
        D3DXVECTOR3 direction;  // 方向
    };

    // レイのヒット結果の構造体
    struct RaycastHit
    {
        bool        hit = false;
        float       distance = FLT_MAX;
        D3DXVECTOR3 point;
        D3DXVECTOR3 normal;
    };

    // レイキャスト処理
    bool Raycast(const Ray& ray, float maxDistance, RaycastHit& outHit);

    void AddRigidBody(std::shared_ptr<RigidBody> body) { m_Bodies.push_back(body); }
    void StepSimulation(float dt);
    void SetGravity(const D3DXVECTOR3& g) { m_Gravity = g; }
    void RemoveRigidBody(std::shared_ptr<RigidBody> body);
    void RemoveCollider(Collider* col);

    // コリジョンのプロセス処理
    void ProcessCollisionEvents(void);

    const D3DXVECTOR3& GetGravity(void) const { return m_Gravity; }
    const std::vector<std::shared_ptr<RigidBody>>& GetBodies(void) const { return m_Bodies; }

private:

    // 衝突ペアを管理する構造体
    struct CollisionPair
    {
        Collider* a;// コライダーA
        Collider* b;// コライダーB

        bool operator==(const CollisionPair& other) const
        {
            return (a == other.a && b == other.b) ||
                (a == other.b && b == other.a);
        }
    };

    struct CollisionPairHash
    {
        size_t operator()(const CollisionPair& p) const
        {
            // 順番を統一してからハッシュ
            auto a = p.a < p.b ? p.a : p.b;
            auto b = p.a < p.b ? p.b : p.a;

            // 2つのポインタを1つの値にする
            return std::hash<Collider*>()(a) ^ (std::hash<Collider*>()(b) << 1);
        }
    };

    D3DXVECTOR3 GetActualCollisionPoint(RigidBody* a, RigidBody* b, const D3DXVECTOR3& push);

    // 衝突判定と押し戻し
    bool CheckCollision(RigidBody* a, RigidBody* b, D3DXVECTOR3& outPush);

    // 各種判定関数
    bool BoxBoxCollision(BoxCollider* a, BoxCollider* b, D3DXVECTOR3& outPush);
    bool CapsuleBoxCollision(CapsuleCollider* cap, BoxCollider* box, D3DXVECTOR3& outPush);
    bool CapsuleCapsuleCollision(CapsuleCollider* a, CapsuleCollider* b, D3DXVECTOR3& outPush);
    bool CylinderBoxCollision(CylinderCollider* cyl, BoxCollider* box, D3DXVECTOR3& outPush);
    bool CylinderCapsuleCollision(CylinderCollider* cyl, CapsuleCollider* cap, D3DXVECTOR3& outPush);
    bool CylinderCylinderCollision(CylinderCollider* a, CylinderCollider* b, D3DXVECTOR3& outPush);
    bool SphereBoxCollision(SphereCollider* s, BoxCollider* b, D3DXVECTOR3& outPush);
    bool SphereCapsuleCollision(SphereCollider* s, CapsuleCollider* c, D3DXVECTOR3& outPush);
    bool SphereCylinderCollision(SphereCollider* s, CylinderCollider* c, D3DXVECTOR3& outPush);
    bool SphereSphereCollision(SphereCollider* s1, SphereCollider* s2, D3DXVECTOR3& outPush);

    // ペア生成時に順番固定
    CollisionPair MakePair(Collider* a, Collider* b);

    // ヘルパー関数
    // 線分p1-q1とp2-q2間の最短距離を求める（最近接点も返す）
    float DistanceSqSegmentSegment(
        const D3DXVECTOR3& p1, const D3DXVECTOR3& q1,
        const D3DXVECTOR3& p2, const D3DXVECTOR3& q2,
        D3DXVECTOR3* outClosest1, D3DXVECTOR3* outClosest2);

    D3DXVECTOR3 ClosestPointOnLineSegment(const D3DXVECTOR3& point, const D3DXVECTOR3& a, const D3DXVECTOR3& b);

    void ProjectOBB(const D3DXVECTOR3& axis, BoxCollider* obb, float& outMin, float& outMax);
    D3DXVECTOR3 ClosestPointOnOBB(const D3DXVECTOR3& point, BoxCollider* obb);

private:
    static constexpr int    AXIS             = 3;       // 各軸
    static constexpr int    ITERATIONS       = 1;       // 反復回数
    static constexpr float  DEFAULT_GRAVITY  = -300.0f; // デフォルトの重力

    std::vector<std::shared_ptr<RigidBody>>                 m_Bodies;               // リジッドボディ
    std::unordered_set<CollisionPair, CollisionPairHash>    m_prevCollisions;       // 前回のコリジョン
    std::unordered_set<CollisionPair, CollisionPairHash>    m_currentCollisions;    // 現在のコリジョン
    D3DXVECTOR3                                             m_Gravity;              // 重力
};

#endif

