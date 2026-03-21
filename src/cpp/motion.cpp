//===================================================
//
// モーション [motion.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "motion.h"
#include "stdio.h"
#include "object.h"
#include "math.h"
#include "manager.h"
#include "model.h"
#include "utility_math.h"
#include "motion_loader.h"

//===================================================
// コンストラクタ
//===================================================
CMotion::CMotion() : 
	m_nID(-1)
{
	//m_aInfo = nullptr;
	m_bLoopMotion = false;
	m_nCount = NULL;
	m_nextKey = NULL;
	m_nKey = NULL;
	m_nNumKey = NULL;
	m_nType = NULL;
	m_nTypeBlend = NULL;
	m_bFinish = false;
	m_bFirst = false;
	m_bBlend = false;
	m_bLoopBlend = false;
	m_nCounterBlend = NULL;
	m_nCounterMotionBlend = NULL;
	m_nNextKeyBlend = NULL;
	m_nFrameBlend = NULL;
	m_nKeyBlend = NULL;
	m_nNumKeyBlend = NULL;
	m_nAllCounter = NULL;
	m_nAllFrame = NULL;
}

//===================================================
// デストラクタ
//===================================================
CMotion::~CMotion()
{
}

//===================================================
// モーションのロード処理
//===================================================
std::unique_ptr<CMotion> CMotion::Load(const char* pLoadFileName, std::vector<std::unique_ptr<CModel>>& vpModel)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// モーションのロードクラスの取得
	CMotionLoader* pLoader = pManager->GetMotionLoader();

	// モーションを生成
	auto pInstance = std::make_unique<CMotion>();

	pInstance->m_nID = pLoader->Register(pLoadFileName, vpModel);

	return pInstance;
}

////===================================================
//// モーションの情報の取得
////===================================================
//void CMotion::GetInfo(CMotion* pMotion)
//{
//	pMotion->m_aInfo = m_aInfo;
//
//	pMotion->m_pLoader = make_unique<CMotionLoader>();
//
//	m_pLoader->GetInfo(pMotion->m_pLoader.get());
//}

//===================================================
// イベントフレームの判定
//===================================================
bool CMotion::IsEventFrame(const int nStartFrame, const int nEndFrame,const int nType)
{
	if (m_nAllCounter >= nStartFrame &&
		m_nAllCounter <= nEndFrame &&
		m_nTypeBlend == nType &&
		m_bFinish == false)
	{
		return true;
	}

	return false;
}

//===================================================
// イベントフレームの判定
//===================================================
bool CMotion::IsEventFrame(const int nType)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// モーションのロードクラスの取得
	CMotionLoader* pLoader = pManager->GetMotionLoader();

	// モーションのデータの取得
	const auto& info = pLoader->GetInfo(m_nID);

	// 要素が無いなら処理しない
	if (info.empty())
	{
		return false;
	}

	// イベントフレームの総数分回す
	for (int nCnt = 0; nCnt < info[nType].nNumEventFrame; nCnt++)
	{
		int Start = info[nType].aStartFrame[nCnt];
		int End = info[nType].aEndFrame[nCnt];

		if (m_nAllCounter >= Start &&
			m_nAllCounter <= End &&
			m_nTypeBlend == nType &&
			m_bFinish == false)
		{
			return true;
		}
	}

	return false;
}

//===================================================
// イベントフレームの判定
//===================================================
bool CMotion::IsEventFrame(const int nType,const int nIdx)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// モーションのロードクラスの取得
	CMotionLoader* pLoader = pManager->GetMotionLoader();

	// モーションのデータの取得
	const auto& info = pLoader->GetInfo(m_nID);

	// 要素が無いなら処理しない
	if (info.empty())
	{
		return false;
	}

	// イベントフレーム
	int Start = info[nType].aStartFrame[nIdx];
	int End = info[nType].aEndFrame[nIdx];

	if (m_nAllCounter >= Start &&
		m_nAllCounter <= End &&
		m_nTypeBlend == nType &&
		m_bFinish == false)
	{
		return true;
	}

	return false;
}

//===================================================
// ループモーションの終了判定
//===================================================
bool CMotion::FinishMotion(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// モーションのロードクラスの取得
	CMotionLoader* pLoader = pManager->GetMotionLoader();

	// モーションのデータの取得
	const auto& info = pLoader->GetInfo(m_nID);

	// 要素が無いなら処理しない
	if (info.empty())
	{
		return false;
	}

	if (info[m_nType].nNumkey - 1 <= m_nKey && m_bFirst == false)
	{
		return true;
	}
	return false;
}

//===================================================
// デバッグ
//===================================================
void CMotion::Debug(void)
{
	//CDebugProc::Print(" = %d / %d");
}

//===================================================
// 終了処理
//===================================================
void CMotion::Uninit(void)
{
}

//===================================================
// 最初のモーションのブレンドが終わったら
//===================================================
void CMotion::FinishFirstBlend(void)
{
	// モーションの出だしのブレンドが終了した
	if (m_bFirst == true && m_nCounterBlend >= m_nFrameBlend && m_bFinish == false)
	{
		// もとに戻す
		m_bFirst = false;

		m_nKey = m_nKeyBlend;

		m_nKeyBlend = 0;
		
		// モーションをブレンドしたモーションにする
		m_nType = m_nTypeBlend;

		m_nCount = m_nCounterMotionBlend;
		m_nAllCounter = m_nCount;

		m_nCounterBlend = 0;
	}
}

//===================================================
// 現在のモーションの更新処理
//===================================================
void CMotion::UpdateCurrentMotion(CModel& pModel, int nIdx)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// モーションのロードクラスの取得
	CMotionLoader* pLoader = pManager->GetMotionLoader();

	// モーションのデータの取得
	const auto& info = pLoader->GetInfo(m_nID);

	// 要素が無いなら処理しない
	if (info.empty())
	{
		return;
	}

	// 次のキーのアドレスの取得
	const CMotionLoader::Key* pKeyNext = &info[m_nType].vKeyInfo[m_nextKey].vKey[nIdx];

	// 現在のキーのアドレスの取得
	const CMotionLoader::Key* pKey = &info[m_nType].vKeyInfo[m_nKey].vKey[nIdx];

	// キー情報のアドレスの取得
	const CMotionLoader::KeyInfo* pKeyInfo = &info[m_nType].vKeyInfo[m_nKey];

	// 次のキーまでの距離を求める
	float fDiffPosX = pKeyNext->fPosX - pKey->fPosX;
	float fDiffPosY = pKeyNext->fPosY - pKey->fPosY;
	float fDiffPosZ = pKeyNext->fPosZ - pKey->fPosZ;

	float fDiffRotX = pKeyNext->fRotX - pKey->fRotX;
	float fDiffRotY = pKeyNext->fRotY - pKey->fRotY;
	float fDiffRotZ = pKeyNext->fRotZ - pKey->fRotZ;

	//// 角度の正規化
	//math::NormalizeRot(&fDiffRotX);
	//math::NormalizeRot(&fDiffRotY);
	//math::NormalizeRot(&fDiffRotZ);

	float fFrame = static_cast<float>(pKeyInfo->nFrame);

	// フレームと最大フレームの割合を求める
	float fRateFrame = m_nCount / fFrame;

	// 次のキーの位置までフレームに応じた位置を設定する
	float fPosX = math::LerpDiff(pKey->fPosX, fDiffPosX, fRateFrame);
	float fPosY = math::LerpDiff(pKey->fPosY, fDiffPosY, fRateFrame);
	float fPosZ = math::LerpDiff(pKey->fPosZ, fDiffPosZ, fRateFrame);

	// 次のキーの向きまでフレームに応じた向きを設定する
	float fRotX = math::LerpDiff(pKey->fRotX, fDiffRotX, fRateFrame);
	float fRotY = math::LerpDiff(pKey->fRotY, fDiffRotY, fRateFrame);
	float fRotZ = math::LerpDiff(pKey->fRotZ, fDiffRotZ, fRateFrame);

	// 位置の設定
	pModel.SetPosition(D3DXVECTOR3(fPosX, fPosY, fPosZ));

	// 向きの設定
	pModel.SetRotation(D3DXVECTOR3(fRotX, fRotY, fRotZ));
}

//===================================================
// ブレンドのモーションの更新処理
//===================================================
void CMotion::UpdateBlendMotion(CModel& pModel, int nIdx)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// モーションのロードクラスの取得
	CMotionLoader* pLoader = pManager->GetMotionLoader();

	// モーションのデータの取得
	const auto& info = pLoader->GetInfo(m_nID);

	// 要素が無いなら処理しない
	if (info.empty())
	{
		return;
	}

	// 現在のキーのアドレスを取得
	const CMotionLoader::Key* pCurrentKey = &info[m_nType].vKeyInfo[m_nKey].vKey[nIdx];

	// 次のキーのアドレスを取得
	const CMotionLoader::Key* pNextKey = &info[m_nType].vKeyInfo[m_nextKey].vKey[nIdx];

	// キー情報のアドレスの取得
	const CMotionLoader::KeyInfo* pKeyInfo = &info[m_nType].vKeyInfo[m_nKey];

	// 現在のフレームを計算
	float fCurrentFrame = static_cast<float>(pKeyInfo->nFrame);

	// ブレンドフレームを計算
	float fBlendFrame = static_cast<float>(info[m_nTypeBlend].vKeyInfo[m_nKeyBlend].nFrame);

	float fRateMotion = m_nCount / fCurrentFrame; // 相対値

	float fRateMotionBlend = m_nCounterMotionBlend / fBlendFrame;
	float fRateBlend = m_nCounterBlend / static_cast<float>(m_nFrameBlend);

	// 現在のモーションの角度の差分
	float fDiffMotionRX = pNextKey->fRotX - pCurrentKey->fRotX;
	float fDiffMotionRY = pNextKey->fRotY - pCurrentKey->fRotY;
	float fDiffMotionRZ = pNextKey->fRotZ - pCurrentKey->fRotZ;

	//// 角度の正規化
	//NormalizeRot(&fDiffMotionRX);
	//NormalizeRot(&fDiffMotionRY);
	//NormalizeRot(&fDiffMotionRZ);

	// 現在のモーションのフレームに応じた角度を設定
	float fRotXCurrent = math::LerpDiff(pCurrentKey->fRotX, fDiffMotionRX, fRateMotion);
	float fRotYCurrent = math::LerpDiff(pCurrentKey->fRotY, fDiffMotionRY, fRateMotion);
	float fRotZCurrent = math::LerpDiff(pCurrentKey->fRotZ, fDiffMotionRZ, fRateMotion);

	// ブレンドのキーのアドレスを取得
	const CMotionLoader::Key* pCurrentBlendKey = &info[m_nTypeBlend].vKeyInfo[m_nKeyBlend].vKey[nIdx];

	// ブレンドの次のキーのアドレスを取得
	const CMotionLoader::Key* pNextBlendKey = &info[m_nTypeBlend].vKeyInfo[m_nNextKeyBlend].vKey[nIdx];

	//ブレンドモーションの角度の差分を求める
	float fDiffMotionBlendRX = pNextBlendKey->fRotX - pCurrentBlendKey->fRotX;
	float fDiffMotionBlendRY = pNextBlendKey->fRotY - pCurrentBlendKey->fRotY;
	float fDiffMotionBlendRZ = pNextBlendKey->fRotZ - pCurrentBlendKey->fRotZ;

	//// 角度の正規化
	//NormalizeRot(&fDiffMotionBlendRX);
	//NormalizeRot(&fDiffMotionBlendRY);
	//NormalizeRot(&fDiffMotionBlendRZ);

	// ブレンドモーションのフレームに応じた角度の設定
	float fRotXBlend = math::LerpDiff(pCurrentBlendKey->fRotX, fDiffMotionBlendRX, fRateMotionBlend);
	float fRotYBlend = math::LerpDiff(pCurrentBlendKey->fRotY, fDiffMotionBlendRY, fRateMotionBlend);
	float fRotZBlend = math::LerpDiff(pCurrentBlendKey->fRotZ, fDiffMotionBlendRZ, fRateMotionBlend);

	//現在のモーションの位置の差分を求める
	float fDiffMotionPX = pNextKey->fPosX - pCurrentKey->fPosX;
	float fDiffMotionPY = pNextKey->fPosY - pCurrentKey->fPosY;
	float fDiffMotionPZ = pNextKey->fPosZ - pCurrentKey->fPosZ;

	// 現在のモーションのフレームに応じた位置の設定
	float fPosXCurrent = math::LerpDiff(pCurrentKey->fPosX, fDiffMotionPX, fRateMotion);
	float fPosYCurrent = math::LerpDiff(pCurrentKey->fPosY, fDiffMotionPY, fRateMotion);
	float fPosZCurrent = math::LerpDiff(pCurrentKey->fPosZ, fDiffMotionPZ, fRateMotion);

	// ブレンドモーションの位置の差分を求める
	float fDiffMotionBlendPX = pNextBlendKey->fPosX - pCurrentBlendKey->fPosX;
	float fDiffMotionBlendPY = pNextBlendKey->fPosY - pCurrentBlendKey->fPosY;
	float fDiffMotionBlendPZ = pNextBlendKey->fPosZ - pCurrentBlendKey->fPosZ;

	// ブレンドモーションのフレームに応じた位置の設定
	float fPosXBlend = math::LerpDiff(pCurrentBlendKey->fPosX, fDiffMotionBlendPX, fRateMotionBlend);
	float fPosYBlend = math::LerpDiff(pCurrentBlendKey->fPosY, fDiffMotionBlendPY, fRateMotionBlend);
	float fPosZBlend = math::LerpDiff(pCurrentBlendKey->fPosZ, fDiffMotionBlendPZ, fRateMotionBlend);

	// 現在の角度とブレンドする角度の差分を求める
	float fDiffBlendRX = fRotXBlend - fRotXCurrent;
	float fDiffBlendRY = fRotYBlend - fRotYCurrent;
	float fDiffBlendRZ = fRotZBlend - fRotZCurrent;

	// 角度の正規化
	math::NormalizeRot(&fDiffBlendRX);
	math::NormalizeRot(&fDiffBlendRY);
	math::NormalizeRot(&fDiffBlendRZ);

	// ブレンドのフレームに応じた向きの設定
	float fRotX = math::LerpDiff(fRotXCurrent, fDiffBlendRX, fRateBlend);
	float fRotY = math::LerpDiff(fRotYCurrent, fDiffBlendRY, fRateBlend);
	float fRotZ = math::LerpDiff(fRotZCurrent, fDiffBlendRZ, fRateBlend);

	// 現在の位置とブレンドする位置の差分を求める
	float fDiffBlendPX = fPosXBlend - fPosXCurrent;
	float fDiffBlendPY = fPosYBlend - fPosYCurrent;
	float fDiffBlendPZ = fPosZBlend - fPosZCurrent;

	// ブレンドのフレームに応じた位置の設定
	float fPosX = math::LerpDiff(fPosXCurrent, fDiffBlendPX, fRateBlend);
	float fPosY = math::LerpDiff(fPosYCurrent, fDiffBlendPY, fRateBlend);
	float fPosZ = math::LerpDiff(fPosZCurrent, fDiffBlendPZ, fRateBlend);

	// 位置を設定
	pModel.SetPosition(D3DXVECTOR3(fPosX, fPosY, fPosZ));

	// 向きを設定
	pModel.SetRotation(D3DXVECTOR3(fRotX, fRotY, fRotZ));
}

//===================================================
// 更新処理
//===================================================
void CMotion::Update(std::vector<std::unique_ptr<CModel>>& vpModel,const int nNumModel)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// モーションのロードクラスの取得
	CMotionLoader* pLoader = pManager->GetMotionLoader();

	// モーションのデータの取得
	const auto& info = pLoader->GetInfo(m_nID);

	// 要素が無いなら処理しない
	if (info.empty())
	{
		return;
	}

	// モデル数分回す
	for (int nCntModel = 0; nCntModel < nNumModel; nCntModel++)
	{
		// キーの総数
		m_nNumKey = info[m_nType].nNumkey;
		m_nNumKeyBlend = info[m_nTypeBlend].nNumkey;

		// ループするかどうか
		m_bLoopMotion = info[m_nType].bLoop;
		
		// 次のキーを増やす
		m_nextKey = (m_nKey + 1) % info[m_nType].nNumkey;

		// 次のブレンドキーを増やす
		m_nNextKeyBlend = (m_nKeyBlend + 1) % m_nNumKeyBlend;

		if (m_bFinish == false && m_bFirst == false)
		{
			// 現在のモーションの更新処理
			UpdateCurrentMotion(*vpModel[nCntModel].get(), nCntModel);
		}
		if ((m_bFinish == true || m_bFirst == true) && m_bBlend == true)
		{
			// ブレンドのモーションの更新処理
			UpdateBlendMotion(*vpModel[nCntModel].get(), nCntModel);
		}
	}	
	
	// モーションが終了したら
	if (IsEndMotion())
	{
		// ブレンドのフレームを計算
		int nBlendFrame = info[m_nType].vKeyInfo[m_nNumKey - 1].nFrame;

		m_nCounterMotionBlend = 0;		// ブレンドモーションのカウンターをリセット
		m_nKeyBlend = 0;				// ブレンドキーをリセット
		m_nCounterBlend = 0;			// ブレンドフレームをリセット
		m_bFinish = true;				// モーションが終わった
		m_nFrameBlend = nBlendFrame;	// ブレンドフレームを設定
		m_nTypeBlend = 0;	// ニュートラルにブレンドする
	}

	// モーションの出だしのブレンドが終了した
	FinishFirstBlend();

	// キーが最大かつブレンドのカウントが最大になった
	if (IsFinishEndBlend() == true)
	{
		m_bFinish = false;				// もとに戻す
		m_bBlend = false;				// もとに戻す
		m_nCount = m_nCounterMotionBlend;	    // フレームをブレンドした先のフレームに合わせる
		m_nAllCounter = m_nCounterMotionBlend;
		m_nTypeBlend = 0;				// ニュートラルにブレンドする
		m_nCounterBlend = 0;
		m_nKey = m_nKeyBlend;
	}

	// フレームを計算
	int nFrame = info[m_nType].vKeyInfo[m_nKey].nFrame;

	// フレームが最大フレームを超えたら
	if (m_nCount >= nFrame)
	{
		// キーを増やす
		m_nKey++;

		// 範囲内にラップする
		m_nKey = math::Wrap(m_nKey, 0, info[m_nType].nNumkey - 1);

		// カウンターをリセット
		m_nCount = 0;
	}

	// ブレンドフレームを計算
	nFrame = info[m_nTypeBlend].vKeyInfo[m_nKeyBlend].nFrame;

	// ブレンドキーを進める
	if (m_nCounterMotionBlend >= nFrame && (m_bFinish == true || m_bFirst == true))
	{
		// キーを増やす
		m_nKeyBlend++;

		// 範囲内にラップする
		m_nKeyBlend = math::Wrap(m_nKeyBlend, 0, info[m_nTypeBlend].nNumkey - 1);

		m_nCounterMotionBlend = 0;
	}

	// 最初のブレンドじゃないなら
	if (m_bFirst == false)
	{
		m_nAllCounter++;
		m_nCount++;
	}

	// ブレンドが始まったら
	if (m_bFinish == true || m_bFirst == true)
	{
		m_nCounterBlend++;
		m_nCounterMotionBlend++;
	}

	// 計算用ALLフレーム
	int nAllFrame = 0;

	for (int nCnt = 0; nCnt < info[m_nType].nNumkey; nCnt++)
	{
		nAllFrame += info[m_nType].vKeyInfo[nCnt].nFrame;
	}

	m_nAllFrame = nAllFrame;

	// ブレンドフレームを計算
	nFrame = m_nAllFrame;

	// 最大を超えたら
	if (m_nAllCounter >= nFrame)
	{
		m_nAllCounter = 0;
	}
}

//===================================================
// モーションの設定処理
//===================================================
void CMotion::Play(const int motiontype, bool bBlend, const int nBlendFrame)
{
	// 同じ種類のモーションだったら
	if (m_nTypeBlend == motiontype || m_nType == motiontype) return;
	
	// ブレンドがあるなら
	if (bBlend == true)
	{
		m_nKeyBlend = 0;				// ブレンドキーをリセット

		if (m_bFirst == false)
		{
			m_nCounterBlend = 0;		 // ブレンドカウンターをリセット
			m_nFrameBlend = nBlendFrame;		 // ブレンドフレームを設定する
			m_bFirst = true;			 // 最初のブレンド開始フラグをtrueにする
		}

		m_bFinish = false;			 // モーションが終わっていない
		m_nTypeBlend = motiontype;   // ブレンド先の種類を設定
	}
	else
	{
		m_nKey = 0;						// 最初から始める
		m_nCount = 0;					// 最初から始める
		m_nType = motiontype;			// ブレンドするモーションのタイプを代入
		m_nTypeBlend = motiontype;		// ブレンドするモーションのタイプを代入
		m_bFinish = false;
		m_bFirst = false;
	}

	m_bBlend = bBlend; // ブレンドするかどうか判定
	m_nCounterMotionBlend = 0;
	m_nAllCounter = 0; // 全体のフレームのカウンターをリセットする
}

//===================================================
// モーションが終わったかどうか
//===================================================
bool CMotion::IsEndMotion(void)
{
	// モーションが終了したら
	if (m_bFinish == false &&
		m_nKey >= m_nNumKey - 1 &&
		m_bLoopMotion == false &&
		m_bFirst == false)
	{
		m_bBlend = true;
		return true;
	}
	return false;
}

//===================================================
// ループモーションが終わったら
//===================================================
bool CMotion::IsEndLoopMotion(void)
{
	// マネージャーの取得
	CManager* pManager = CManager::GetInstance();

	// モーションのロードクラスの取得
	CMotionLoader* pLoader = pManager->GetMotionLoader();

	// モーションのデータの取得
	const auto& info = pLoader->GetInfo(m_nID);

	// 要素が無いなら処理しない
	if (info.empty())
	{
		return false;
	}

	if (m_nKey >= info[m_nType].nNumkey - 1 && info[m_nType].bLoop)
	{
		return true;
	}
	return false;
}

//===================================================
// 終わりのブレンドが完了したか判定
//===================================================
bool CMotion::IsFinishEndBlend(void)
{
	if (m_bFinish == true && m_nFrameBlend <= m_nCounterBlend - 1 && m_bFirst == false)
	{
		return true;
	}
	return false;
}