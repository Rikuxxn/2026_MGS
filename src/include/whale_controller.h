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

	// ドーナツ状にクジラ生成する処理
	void SpawnWhale(void);

	// プレイヤーと当たったとときの処理
	void OnHitPlayer(CWhale* pWhale);

	// クジラが満足した処理
	void OnWhaleSatisfied(CWhale* whale);

	void Erase(CWhale* whale);

	inline int GetSatisfied(void) const { return m_nSatisfiedWhaleNum; }
private:
	static constexpr int MAX_WHALE_NUM = 5;// クジラの最大生成数

	// 生成エフェクトの情報
	struct SpawnEffectInfo
	{

		int nTime;	// 終了時間
	};

	CPlayer* m_pPlayer;
	std::list<CWhale*> m_pList;	// クジラのリスト
	SpawnEffectInfo m_spawnEffectInfo;	// 生成のエフェクトの情報
	int m_nSatisfiedWhaleNum;	// 満足したクジラの数
};
#endif