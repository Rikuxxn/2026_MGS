//===================================================
//
// スコアのクラス [score.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _SCORE_H_
#define _SCORE_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"

//***************************************************
// 前方宣言
//***************************************************
class CNumber;	// 数字クラス

//***************************************************
// スコアのクラスの定義
//***************************************************
class CScore : public CObject
{
public:
	static constexpr int MAX_DIGIT	= 2;		// 最大の桁数
	static constexpr int MAX_SCORE = 99;		// 最大の値
	static constexpr int MIN_SCORE = 0;			// 最小の値

	CScore();
	~CScore();

	static CScore* Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, const int nScore, const char* pTexturePath);

	HRESULT Init	(void) override;
	void	Uninit	(void) override;
	void	Update	(void) override;
	void	Draw	(void) override;

	inline void	AddScore(const int nScore) { m_nScore += nScore; }
	inline int GetScore(void) { return m_nScore; }
private:
	std::array<std::unique_ptr<CNumber>, MAX_DIGIT> m_apNumber;	// ナンバークラスへのポインタ
	D3DXVECTOR3 m_pos;											// 位置
	D3DXVECTOR2 m_size;											// 大きさ
	std::string m_texturePath;									// ファイルパス
	int m_nScore;												// 現在のスコア
};
#endif