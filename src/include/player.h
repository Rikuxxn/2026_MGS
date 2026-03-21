//===================================================
//
// プレイヤーの処理 [player.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "object.h"

//***************************************************
// 前方宣言
//***************************************************
class CCharacter;	// キャラクタークラス

//***************************************************
// プレイヤーの処理のクラスの定義
//***************************************************
class CPlayer : public CObject
{
public:
	CPlayer();
	~CPlayer();

	/// <summary>
	/// プレイヤーの生成
	/// </summary>
	/// <param name="位置"></param>
	/// <param name="向き"></param>
	/// <param name="モーションのファイルパス"></param>
	/// <returns>プレイヤーのポインタ</returns>
	static CPlayer* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pMotionFilePath);

	HRESULT Init	(void) override;
	void	Uninit	(void) override;
	void	Update	(void) override;
	void	Draw	(void) override;
private:
	std::unique_ptr<CCharacter> m_pCharacter;	// キャラクタークラスへのポインタ
};
#endif