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
	void CreateCluster(const D3DXVECTOR3& center, int count, float radiusMax);

	// 更新処理
	void Update(void);

	void RegisterPlayerPlanktonList(CPlankton* pPlankton);
private:
	HRESULT Init(void);
	HRESULT SavePlankton(void);
private:
	CPlayer* m_pPlayer;							// プレイヤーのクラス
	std::list<CPlankton*> m_pList;				// プランクトンのリスト
	std::vector<D3DXVECTOR3> m_clusterCenters;	// 塊の中心位置

	struct Edit
	{
		std::vector<D3DXVECTOR3> vPlanktonPos;	// プランクトンの位置
	};

	Edit m_edit;	// 編集モード
};
#endif