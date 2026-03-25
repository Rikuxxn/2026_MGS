//===================================================
//
// プランクトンを操作し、管理するクラス [plankton_controller.h]
// Author:YUTO YOSHIDA
// 
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _PLANKTON_CONTROLLER_H_
#define _PLANKTON_CONTROLLER_H_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// 前方宣言
//***************************************************
class CPlankton;	// プランクトンクラス
class CPlayer;		// プレイヤークラス

//***************************************************
// プランクトンのコントローラーのクラスの定義
//***************************************************
class CPlanktonController
{
public:
	CPlanktonController();
	~CPlanktonController();

	static std::unique_ptr<CPlanktonController> Create(CPlayer* pPlayer);
	HRESULT CreatePlankton(const D3DXVECTOR3& pos, const D3DXVECTOR2& size);

	// 更新処理
	void Update(void);
private:
	CPlayer* m_pPlayer;				// プレイヤーのクラス
	std::list<CPlankton*> m_pList;	// プランクトンのリスト
};
#endif