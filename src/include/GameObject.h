//=============================================================================
//
// ゲームオブジェクト処理 [GameObject.h]
// Author : RIKU TANEKAWA
//
//=============================================================================
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_


//*****************************************************************************
// 共通インターフェースクラス
//*****************************************************************************
class IGameObject
{
public:
    virtual ~IGameObject() = default;

    // タグの設定処理
    void SetTag(const std::string& tag) { m_tag = tag; }

    // タグの比較(一致するか)処理
    bool CompareTag(const std::string& tag) const
    {
        return m_tag == tag;
    }

    virtual void OnCollisionEnter(IGameObject* /*other*/) {}
    virtual void OnCollisionStay(IGameObject* /*other*/) {}
    virtual void OnCollisionExit(IGameObject* /*other*/) {}

private:
    std::string m_tag;  // タグ
};

#endif