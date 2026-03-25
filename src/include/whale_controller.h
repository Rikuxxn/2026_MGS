//===================================================
//
// クジラを操作し、管理するクラス [whale_controller.h]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _WHALE_CONTROLLER_H_
#define _WHALE_CONTROLLER_H_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// 前方宣言
//***************************************************
class CPlayer;	// プレイヤークラス
class CWhale;	// クジラクラス

//***************************************************
// クジラの操作クラスの定義
//***************************************************
class CWhaleController
{
public:
	CWhaleController();
	~CWhaleController();

	CWhale* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pMotionFilePath);
	static std::unique_ptr<CWhaleController> Create(CPlayer* pPlayer);

	// 更新処理
	void Update(void);

	// プレイヤーと当たったとときの処理
	void OnHitPlayer(CWhale* pWhale);
private:
	CPlayer* m_pPlayer;
	std::list<CWhale*> m_pList;	// クジラのリスト
};
#endif