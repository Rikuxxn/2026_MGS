//===================================================
//
// モーション [motion.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MOTION_H_
#define _MOTION_H_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// 前方宣言
//***************************************************
class CModel;

//***************************************************
// モーションクラスの定義
//***************************************************
class CMotion
{
public:
	CMotion();
	~CMotion();

	static std::unique_ptr<CMotion> Load(const char* pLoadFileName, std::vector<std::unique_ptr<CModel>>& vpModel, int* pOutModel);

	void		Uninit				(void);
	void		Update				(std::vector<std::unique_ptr<CModel>>& vpModel, const int nNumModel);
	void		Play				(const int motiontype, bool bBlend, const int nBlendFrame);
	bool		IsEndMotion			(void);
	bool		IsEndLoopMotion		(void);
	bool		IsFinishEndBlend	(void);

	inline int	GetType				(void) const { return m_nType; }
	inline int	GetBlendType		(void) const { return m_nTypeBlend; }

	bool		IsEventFrame		(const int nStartFrame, const int nEndFrame, const int nType);
	bool		IsEventFrame		(const int nType);
	bool		IsEventFrame		(const int nType,const int nIdx);
	bool		FinishMotion		(void);
	void		Debug				(void);
private:
	void		UpdateCurrentMotion	(CModel& pModel, int nIdx);
	void		UpdateBlendMotion	(CModel& pModel, int nIdx);
	void		FinishFirstBlend	(void);
private:
	int m_nType;				// モーションの種類
	int m_nNumKey;				// キーの総数
	int m_nKey;					// 現在のキーNo.
	int m_nCount;				// モーションのカウンター
	int m_nextKey;				// 次のキー
	int m_nAllFrame;			// 全体のフレーム数
	int m_nAllCounter;			// 全体フレームのカウンター
	bool m_bLoopMotion;			// ループするかどうか

	bool m_bBlend;				// ブレンドがあるかどうか
	bool m_bLoopBlend;			// ループするかどうか
	bool m_bFinish;				// モーションが終わったかどうか
	bool m_bFirst;				// モーションが始まったフラグ

	int m_nTypeBlend;			// ブレンドの種類
	int m_nFrameBlend;			// ブレンドのフレーム数
	int m_nCounterBlend;		// ブレンドカウンター
	int m_nNumKeyBlend;			// ブレンドモーションの最大のキー
	int m_nKeyBlend;			// ブレンドモーションの現在のキー
	int m_nNextKeyBlend;		// ブレンドモーションの次のキー
	int m_nCounterMotionBlend;	// ブレンドのカウンター

	int m_nID;					// 自分のID
};
#endif