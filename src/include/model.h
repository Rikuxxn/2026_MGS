//===================================================
//
// キャラクターのモデルのクラス [model.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MODEL_H_
#define _MODEL_H_

//***************************************************
// キャラクターのモデルのクラスの定義
//***************************************************
class CModel
{
public:
	CModel();
	~CModel();

	static CModel* Create(const char* pModelFilePath);

	HRESULT Init(const char* pModelFilePath);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	inline void SetPosition			(const D3DXVECTOR3& pos)	{ m_pos = pos; }
	inline void SetRotation			(const D3DXVECTOR3& rot)	{ m_rot = rot; }

	inline void SetOffSetPosition	(const D3DXVECTOR3& pos)	{ m_offPos = pos; }
	inline void SetOffSetRotation	(const D3DXVECTOR3& rot)	{ m_offRot = rot; }

	inline void SetParent			(CModel* pModel)			{ m_pParent = pModel; }
	inline void SetParentID			(const int nParentID)		{ m_nParentID = nParentID; }
	inline void SetModelPath		(const char* pPath)			{ m_modelPath = pPath; }

	inline int	GetParentID			(void) const { return m_nParentID; }
	inline const char* GetModelPath	(void) const { return m_modelPath.c_str(); }

	void		SetOffset			(const CModel* pOther);
private:
	D3DXVECTOR3			m_size;			// 大きさ
	D3DXVECTOR3			m_offPos;		// 基準の位置
	D3DXVECTOR3			m_offRot;		// 基準の向き
	D3DXVECTOR3			m_pos;			// 位置
	D3DXVECTOR3			m_rot;			// 向き
	D3DXMATRIX			m_mtxWorld;		// ワールドマトリックス
	CModel*				m_pParent;		// 親モデルへのポインタ
	std::vector<int>	m_vnTextureID;	// テクスチャのID
	std::string			m_modelPath;	// モデルのファイルパス
	int					m_nParentID;	// 親のインデックス
	int					m_nModelID;		// モデルのインデックス
};
#endif