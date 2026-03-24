//=============================================================================
//
// コライダー処理 [Collider.cpp]
// Author : RIKU TANEKAWA
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "Collider.h"
#include "Block.h"

//=============================================================================
// 衝突した瞬間の処理
//=============================================================================
void Collider::OnCollisionEnter(Collider* other)
{
    // nullptrだったら飛ばす
    if (!m_owner || !other->m_owner)
    {
        return;
    }

    m_owner->OnCollisionEnter(other->m_owner);
}
//=============================================================================
// 衝突中の処理
//=============================================================================
void Collider::OnCollisionStay(Collider* other)
{
    // nullptrだったら飛ばす
    if (!m_owner || !other->m_owner)
    {
        return;
    }

    m_owner->OnCollisionStay(other->m_owner);
}
//=============================================================================
// 衝突後の処理
//=============================================================================
void Collider::OnCollisionExit(Collider* other)
{
    // nullptrだったら飛ばす
    if (!m_owner || !other->m_owner)
    {
        return;
    }

    m_owner->OnCollisionExit(other->m_owner);
}

//=============================================================================
// ボックスコライダー(OBB)のトランスフォーム処理
//=============================================================================
void BoxCollider::UpdateTransform(const D3DXVECTOR3& pos, const D3DXQUATERNION& rot, const D3DXVECTOR3& scale)
{
    // 実サイズ = 元サイズ × スケール
    m_ScaledSize.x = m_Size.x * scale.x;
    m_ScaledSize.y = m_Size.y * scale.y;
    m_ScaledSize.z = m_Size.z * scale.z;

    // ワールド位置
    m_Position = pos;
    m_RotationQuat = rot;

    // 回転行列
    D3DXMatrixRotationQuaternion(&m_Rotation, &rot);
}
//=============================================================================
// ボックスコライダー(OBB)の慣性計算処理
//=============================================================================
void BoxCollider::calculateLocalInertia(float mass, D3DXVECTOR3& inertia) const
{
    if (mass <= 0.0f)
    {
        inertia = Const::VEC3_NULL;
        return;
    }

    D3DXVECTOR3 s = m_ScaledSize;
    inertia.x = (1.0f / 12.0f) * mass * (s.y * s.y + s.z * s.z);
    inertia.y = (1.0f / 12.0f) * mass * (s.x * s.x + s.z * s.z);
    inertia.z = (1.0f / 12.0f) * mass * (s.x * s.x + s.y * s.y);
}

//=============================================================================
// レイキャスト処理
//=============================================================================
bool BoxCollider::Raycast(const PhysicsWorld::Ray& ray, float maxDistance, PhysicsWorld::RaycastHit& hit)
{
    return RayVsOBB(ray, this, maxDistance, hit);
}
//=============================================================================
// ボックスコライダー(OBB)とレイのヒット処理
//=============================================================================
bool BoxCollider::RayVsOBB(
    const PhysicsWorld::Ray& ray,
    const BoxCollider* box,
    float maxDistance,
    PhysicsWorld::RaycastHit& hit)
{
    const float EPSILON = 1e-6f;

    // OBB情報
    D3DXVECTOR3 p = box->GetPosition();// 中心
    D3DXVECTOR3 o = ray.origin;

    D3DXVECTOR3 d = ray.direction;
    D3DXVec3Normalize(&d, &d);

    D3DXVECTOR3 delta = p - o;

    float tMin = 0.0f;
    float tMax = maxDistance;

    D3DXVECTOR3 hitNormal(0, 0, 0);

    D3DXMATRIX R = box->GetRotation();

    D3DXVECTOR3 axes[3] =
    {
        D3DXVECTOR3(R._11, R._12, R._13),
        D3DXVECTOR3(R._21, R._22, R._23),
        D3DXVECTOR3(R._31, R._32, R._33)
    };

    // 正規化
    for (int nCnt = 0; nCnt < 3; nCnt++)
    {
        D3DXVec3Normalize(&axes[nCnt], &axes[nCnt]);
    }

    // ハーフサイズ
    D3DXVECTOR3 half = box->GetScaledSize() * 0.5f;

    // 各軸ごとに判定
    for (int nCnt = 0; nCnt < 3; nCnt++)
    {
        D3DXVECTOR3 axis = axes[nCnt];     // right, up, forward
        float e = D3DXVec3Dot(&axis, &delta);
        float f = D3DXVec3Dot(&d, &axis);

        if (fabs(f) > EPSILON)
        {
            float t1 = (e + half[nCnt]) / f;
            float t2 = (e - half[nCnt]) / f;

            if (t1 > t2) std::swap(t1, t2);

            if (t1 > tMin)
            {
                tMin = t1;

                // 法線更新
                hitNormal = axis * (f > 0 ? -1.0f : 1.0f);
            }

            tMax = std::min(tMax, t2);

            if (tMin > tMax)
            {
                return false;
            }
        }
        else
        {
            // レイが軸と平行
            if (-e - half[nCnt] > 0.0f ||
                -e + half[nCnt] < 0.0f)
            {
                return false;
            }
        }
    }

    if (tMin < 0.0f)
    {
        return false;
    }

    // ヒット結果
    hit.hit = true;
    hit.distance = tMin;
    hit.point = ray.origin + ray.direction * tMin;
    hit.normal = hitNormal;

    return true;
}

//=============================================================================
// カプセルコライダーの上点取得処理
//=============================================================================
D3DXVECTOR3 CapsuleCollider::GetTop(void) const
{
    // 位置の取得
    D3DXVECTOR3 pos = GetPosition();

    return pos + D3DXVECTOR3(0, m_Height * CMathConstant::HALF * m_Scale.y, 0);
}
//=============================================================================
// カプセルコライダーの下点取得処理
//=============================================================================
D3DXVECTOR3 CapsuleCollider::GetBottom(void) const
{
    // 位置の取得
    D3DXVECTOR3 pos = GetPosition();

    return pos - D3DXVECTOR3(0, m_Height * CMathConstant::HALF * m_Scale.y, 0);
}
//=============================================================================
// カプセルコライダーのトランスフォーム処理
//=============================================================================
void CapsuleCollider::UpdateTransform(const D3DXVECTOR3& pos, const D3DXQUATERNION& rot, const D3DXVECTOR3& scale)
{
    m_Position = pos;
    m_Scale = scale;  // 半径・高さにスケールをかけるときに利用
    m_Rotation = rot;
}
//=============================================================================
// カプセルコライダーの慣性計算処理
//=============================================================================
void CapsuleCollider::calculateLocalInertia(float mass, D3DXVECTOR3& inertia) const
{
    float r = m_Radius;
    float h = m_Height;
    inertia.y = CMathConstant::HALF * mass * r * r;
    inertia.x = inertia.z = (1.0f / 12.0f) * mass * (3.0f * r * r + h * h);
}


//=============================================================================
// シリンダーコライダーのトランスフォーム処理
//=============================================================================
void CylinderCollider::UpdateTransform(const D3DXVECTOR3& pos, const D3DXQUATERNION& rot, const D3DXVECTOR3& scale)
{
    m_Position = pos;

    m_RadiusScaled = m_Radius * (scale.x + scale.z) * CMathConstant::HALF;
    m_HeightScaled = m_Height * scale.y;

    m_RotationQuat = rot;
    D3DXMatrixRotationQuaternion(&m_Rotation, &rot);
}
//=============================================================================
// シリンダーコライダーの慣性計算処理
//=============================================================================
void CylinderCollider::calculateLocalInertia(float mass, D3DXVECTOR3& inertia) const
{
    float r = m_RadiusScaled;
    float h = m_HeightScaled;

    inertia.y = 0.5f * mass * r * r;          // 軸方向
    inertia.x = inertia.z = (1.0f / 12.0f) * mass * (3.0f * r * r + h * h); // 横
}


//=============================================================================
// スフィアコライダーのトランスフォーム処理
//=============================================================================
void SphereCollider::UpdateTransform(const D3DXVECTOR3& pos, const D3DXQUATERNION& rot, const D3DXVECTOR3& scale)
{
    m_Position = pos;

    // スケール反映
    float s = std::max({ scale.x, scale.y, scale.z });
    m_ScaledRadius = m_Radius * s;

    m_RotationQuat = rot;
    D3DXMatrixRotationQuaternion(&m_Rotation, &rot);
}
//=============================================================================
// スフィアコライダーの慣性計算処理
//=============================================================================
void SphereCollider::calculateLocalInertia(float mass, D3DXVECTOR3& inertia) const
{
    if (mass <= 0.0f)
    {
        inertia = Const::VEC3_NULL;
        return;
    }

    // 球の慣性モーメント I = 2/5 * m * r^2
    float I = (2.0f / 5.0f) * mass * m_ScaledRadius * m_ScaledRadius;
    inertia = D3DXVECTOR3(I, I, I);
}
