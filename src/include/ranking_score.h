//===================================================
//
// ランキングのスコア [ranking_score.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _RANKING_SCORE_H_
#define _RANKING_SCORE_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"

//***************************************************
// 前方宣言
//***************************************************
class CScore;	// スコアクラス

//***************************************************
// ランキングのスコアのクラス
//***************************************************
class CRankingScore : public CObject
{
public:
	CRankingScore();
	~CRankingScore();

	/// <summary>
	/// ランキングのスコアの生成
	/// </summary>
	/// <param name="位置"></param>
	/// <param name="大きさ"></param>
	/// <param name="色"></param>
	/// <param name="テクスチャのパス"></param>
	/// <param name="スコア"></param>
	/// <returns>自分自身のポインタ</returns>
	static CRankingScore* Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const char* pTexturePath, const int nScore);

	HRESULT Init	(void) override;
	void	Uninit	(void) override;
	void	Update	(void) override;
	void	Draw	(void) override;
private:
	CScore* m_pScore;	// スコアクラスへのポインタ
};
#endif