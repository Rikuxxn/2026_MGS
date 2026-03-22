//===================================================
//
// モーションのロードクラス [motion_loader.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _MOTION_LOADER_H_
#define _MOTION_LOADER_H_

//***************************************************
// インクルードファイル
//***************************************************

//***************************************************
// 前方宣言
//***************************************************
class CModel;	// モデルクラス

//***************************************************
// モーションのロードクラスの定義
//***************************************************
class CMotionLoader
{
public:
	// キーの構造体
	struct Key
	{
		float fPosX; // 位置X
		float fPosY; // 位置Y
		float fPosZ; // 位置Z

		float fRotX; // 向きX
		float fRotY; // 向きY
		float fRotZ; // 向きZ
	};

	// キー情報の構造体
	struct KeyInfo
	{
		int nFrame;						// 再生フレーム
		std::vector <Key> vKey;				// 各キーパーツのキー要素
	};

	// モーション情報の構造体
	struct Info
	{
		bool bLoop;						// ループするかどうか
		int nNumkey;					// キーの総数
		std::vector<KeyInfo> vKeyInfo;	// キー情報
		std::vector<int> aStartFrame;	// スタートフレーム
		std::vector<int> aEndFrame;		// 終了フレーム
		int nNumEventFrame;				// イベントフレームの総数
	};

	// 読み込んだ情報
	struct LoadData
	{
		std::vector<CModel> model;		// モデルの情報
		std::vector<Info>	info;		// 読み込んだ情報
		std::string			filePath;	// ファイルパス
	};

	CMotionLoader();
	~CMotionLoader();

	int Register			(const char* pFilePath, std::vector<std::unique_ptr<CModel>>& pModel);
	HRESULT Load			(const char* pFilePath, std::vector<std::unique_ptr<CModel>>& pModel, const int nCurrentID);

	inline const std::vector<Info>& GetInfo(const int nID) const { return m_vData[nID].info; }

private:
	void LoadModel			(std::vector<std::unique_ptr<CModel>>& pModel, std::string& line, const int nCurrentID);
	bool LoadCharacterSet	(std::vector<std::unique_ptr<CModel>>& pModel, std::string& line, const int nCurrentID);
	bool LoadMotionSet		(std::string& currentLine, int& nCntMotion, const int nCurrentID);
private:
	std::vector<LoadData>	m_vData;		// 読み込んだデータ
	Info					m_inputInfo;	// 入力用データ
	int						m_nNumModel;	// モデル数
};
#endif