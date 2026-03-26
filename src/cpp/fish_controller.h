//===================================================
//
// 魚の操作クラス処理 [fish_controller.h]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _FISH_CONTROLLER_H_
#define _FISH_CONTROLLER_H_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// 前方宣言
//***************************************************
class CPlayer;
class CFish;

//***************************************************
// 魚の操作クラスの定義
//***************************************************
class CFishController
{
public:
	CFishController();
	~CFishController();

	static std::unique_ptr<CFishController> Create(CPlayer* pPlayer);
	// 魚の生成
	CFish* Create(const D3DXVECTOR3& pos, const char* pModelFileName, const int nLife);

	void Update(void);

	void Erase(CFish* pFish);
private:
	CPlayer* m_pPlayer;
	std::list<CFish*> m_pFishList;
};
#endif