//===================================================
//
// ゲームオブジェクトのクラス [game_object.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"
#include "manager.h"
#include "camera.h"

//***************************************************
// 静的メンバ変数の宣言
//***************************************************
std::array<CObject*, CObject::PRIORITY_MAX> CObject::m_apTop	= {}; // 先頭オブジェクトへのポインタ
std::array<CObject*, CObject::PRIORITY_MAX>	CObject::m_apCur	= {}; // 最後尾オブジェクトへのポインタ
std::array<int	   , CObject::PRIORITY_MAX>	CObject::m_anNumAll	= {}; // オブジェクトの総数

//===================================================
// コンストラクタ(オーバーロード)
//===================================================
CObject::CObject(const PRIORITY priority)
{
	m_type = TYPE_NONE;
	m_priority = priority;
	m_bDeath = false;

	m_pPrev = nullptr;
	m_pNext = nullptr;

	// 先頭がnullだったら
	if (m_apTop[priority] == nullptr)
	{
		// 先頭を設定
		m_apTop[priority] = this;
		m_apCur[priority] = this;
	}
	else
	{
		// 既存リストの最後尾に追加
		m_pPrev = m_apCur[priority];
		m_apCur[priority]->m_pNext = this;
		m_apCur[priority] = this;
	}

	// 総数をカウント
	m_anNumAll[priority]++;
}

//===================================================
// デストラクタ
//===================================================
CObject::~CObject()
{
}

//===================================================
// すべてのオブジェクトの破棄
//===================================================
void CObject::ReleaseAll(void)
{
	// 優先順位分回す
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// 先頭オブジェクトを代入
		CObject* pObject = m_apTop[nCntPriority];

		// nullじゃないなら
		while (pObject != nullptr)
		{
			// 次のオブジェクトのポインタを代入
			CObject* pObjectNext = pObject->m_pNext;

			// 更新処理
			pObject->Uninit();

			// 次のオブジェクトを代入
			pObject = pObjectNext;
		}
	}

	// 優先順位分回す
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// 先頭オブジェクトを代入
		CObject* pObject = m_apTop[nCntPriority];

		// nullじゃないなら
		while (pObject != nullptr)
		{
			// 次のオブジェクトのポインタを代入
			CObject* pObjectNext = pObject->m_pNext;

			// 死亡フラグがたっていたら
			if (pObject->m_bDeath == true)
			{
				// オブジェクトの破棄
				pObject->Destroy(pObject);
			}
			// 次のオブジェクトを代入
			pObject = pObjectNext;
		}
	}

}
//===================================================
// すべてのオブジェクトの更新処理
//===================================================
void CObject::UpdateAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// 先頭オブジェクトを代入
		CObject* pObject = m_apTop[nCntPriority];

		// nullじゃないなら
		while (pObject != nullptr)
		{
			// 次のオブジェクトのポインタを代入
			CObject* pObjectNext = pObject->m_pNext;

			//// ポーズ状態の取得
			//bool bPause = CPauseManager::GetPause();

			//// ポーズ中だったらポーズ以外のオブジェクトの処理をしない
			//if (pObject->GetType() != TYPE_PAUSE && bPause == true)
			//{
			//	// 次のオブジェクトを代入
			//	pObject = pObjectNext;

			//	continue;
			//}

			// 更新処理
			pObject->Update();

			// 次のオブジェクトを代入
			pObject = pObjectNext;
		}
	}

	// 優先順位分回す
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// 先頭オブジェクトを代入
		CObject* pObject = m_apTop[nCntPriority];

		// nullじゃないなら
		while (pObject != nullptr)
		{
			// 次のオブジェクトのポインタを代入
			CObject* pObjectNext = pObject->m_pNext;

			// 死亡フラグがたっていたら
			if (pObject->m_bDeath == true)
			{
				// オブジェクトの破棄
				pObject->Destroy(pObject);
			}
			// 次のオブジェクトを代入
			pObject = pObjectNext;
		}
	}
}
//===================================================
// すべてのオブジェクトの描画処理
//===================================================
void CObject::DrawAll(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	if (pManager == nullptr)
	{
		return;
	}

	// カメラの取得
	CCamera* pCamera = pManager->GetCamera();

	if (pCamera == nullptr)
	{
		return;
	}

	// カメラの設定処理
	pCamera->SetCamera();

	// 優先順位分回す
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CObject* pObject = m_apTop[nCntPriority]; // 先頭オブジェクトを代入

		// nullじゃないなら
		while (pObject != nullptr)
		{
			CObject* pObjectNext = pObject->m_pNext; // 次のオブジェクトのポインタを代入

			//// ポーズ状態の取得
			//bool bPause = CPauseManager::GetPause();

			//// オブジェクトがポーズでポーズ中じゃないならポーズを描画しない
			//if (pObject->GetType() == TYPE_PAUSE && bPause == false)
			//{
			//	pObject = pObjectNext; // 次のオブジェクトを代入

			//	// 処理を飛ばす
			//	continue;
			//}

			// 更新処理
			pObject->Draw();

			pObject = pObjectNext; // 次のオブジェクトを代入
		}
	}
}

//===================================================
// 死亡フラグがたっているすべてのオブジェクトの破棄
//===================================================
void CObject::DestroyAll(void)
{
	// 優先順位分回す
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// 先頭オブジェクトを代入
		CObject* pObject = m_apTop[nCntPriority];

		// nullじゃないなら
		while (pObject != nullptr)
		{
			// 次のオブジェクトのポインタを代入
			CObject* pObjectNext = pObject->m_pNext;

			// 死亡フラグがたっていたら
			if (pObject->m_bDeath == true)
			{
				// オブジェクトの破棄
				pObject->Destroy(pObject);
			}
			// 次のオブジェクトを代入
			pObject = pObjectNext;
		}
	}
}

//===================================================
//死亡フラグの設定
//===================================================
void CObject::Release(void)
{
	// 死亡フラグをオンにする
	m_bDeath = true;
}

//===================================================
// オブジェクトの破棄
//===================================================
void CObject::Destroy(CObject* pObject)
{
	// 自分の優先順位
	int nPriority = m_priority;

	// 先頭だったら
	if (pObject == m_apTop[nPriority] && m_pNext != nullptr)
	{
		// 先頭を次のオブジェクトにする
		m_apTop[nPriority] = m_pNext;
		m_pNext->m_pPrev = nullptr;
	}
	// 最後尾だったら
	if (pObject == m_apCur[nPriority] && m_pPrev != nullptr)
	{
		// 最後尾をひとつ前のオブジェクトにする
		m_apCur[nPriority] = m_pPrev;
		m_pPrev->m_pNext = nullptr;
	}
	// 最後尾でも先頭でも無かったら
	if (m_pPrev != nullptr && m_pNext != nullptr)
	{
		// 間をつなげる
		m_pPrev->m_pNext = m_pNext;
		m_pNext->m_pPrev = m_pPrev;
	}

	// 残りが一つしか無かったら
	if (m_apTop[nPriority] == pObject)
	{
		m_apTop[nPriority] = nullptr;
	}

	// 残りが一つしか無かったら
	if (m_apCur[nPriority] == pObject)
	{
		m_apCur[nPriority] = nullptr;
	}

	m_pNext = nullptr;
	m_pPrev = nullptr;

	// オブジェクトの数を減らす
	m_anNumAll[nPriority]--;

	delete pObject;

	pObject = nullptr;
}
