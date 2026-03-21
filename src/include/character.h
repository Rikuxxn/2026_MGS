//===================================================
//
// キャラクターの共通処理クラス [character.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// 前方宣言
//***************************************************
class CModel;	// モデルのクラス
class CMotion;	// モーションクラス

//***************************************************
// キャラクターの共通処理クラスの定義
//***************************************************
class CCharacter
{
public:
	CCharacter();
	~CCharacter();

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="位置"></param>
	/// <param name="向き"></param>
	/// <param name="モーションのファイルパス"></param>
	/// <returns>キャラクタークラスのポインタ(ユニーク)</returns>
	static std::unique_ptr<CCharacter> Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pMotionFilePath);

	HRESULT Init	(const char* pMotionFilePath);
	void	Uninit	(void);
	void	Update	(void);
	void	Draw	(void);

	inline void SetMove		(const D3DXVECTOR3& move)	{ m_move = move; }
	inline void SetPosition	(const D3DXVECTOR3& pos)	{ m_pos = pos; }
	inline void SetRotation	(const D3DXVECTOR3& rot)	{ m_rot = rot; }

	inline const D3DXVECTOR3& GetMove		(void) const { return m_move; }
	inline const D3DXVECTOR3& GetPosition	(void) const { return m_pos; }
	inline const D3DXVECTOR3& GetRotation	(void) const { return m_rot; }

	// パラメータの設定処理
	void SetParameter(const float fInertia, const float fGravity);
private:
	std::vector<std::unique_ptr<CModel>> m_vpModel;		// モデルのクラスへのポインタ
	std::unique_ptr<CMotion>			 m_pMotion;		// モーションクラスへのポインタ
	D3DXVECTOR3							 m_pos;			// 位置
	D3DXVECTOR3							 m_rot;			// 向き
	D3DXVECTOR3							 m_move;		// 移動量
	D3DXMATRIX							 m_mtxWorld;	// ワールドマトリックス
	float								 m_fInertia;	// 慣性
	float								 m_fGravity;	// 重力量
	int									 m_nNumModel;	// モデルの総数
};
#endif