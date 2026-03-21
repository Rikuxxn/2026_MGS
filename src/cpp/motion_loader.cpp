//===================================================
//
// モーションのロードクラス [motion_loader.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "motion_loader.h"
#include "model.h"
#include "model_manager.h"

//===================================================
// コンストラクタ
//===================================================
CMotionLoader::CMotionLoader() :
    m_vData(),
    m_inputInfo(),
    m_nNumModel(0)
{
}

//===================================================
// デストラクタ
//===================================================
CMotionLoader::~CMotionLoader()
{
}

//===================================================
// 登録処理
//===================================================
int CMotionLoader::Register(const char* pFilePath, std::vector<std::unique_ptr<CModel>>& pModel)
{
    int nID = 0;

    // 省略用ファイルパス
    std::string filePath = pFilePath;

    // ファイルパスが省略されていたら
    if (filePath.find("data/MOTION/") == std::string::npos)
    {
        // 文字列を連結
        filePath = "data/MOTION/" + filePath;
    }

    // 要素分回す
    for (auto& data : m_vData)
    {
        if (data.filePath == filePath)
        {
            // モデルのサイズの取得(vKeyはモデル分サイズを確保しているためここから取得する)
            int nModelSize = static_cast<int>(m_vData[nID].info[0].vKeyInfo[0].vKey.size());

            // モデル数分回す
            for (int nCntModel = 0; nCntModel < nModelSize; nCntModel++)
            {
                auto pInstance = std::unique_ptr<CModel>(CModel::Create(m_vData[nID].model[nCntModel].GetModelPath()));

                // オフセットの設定処理
                pInstance->SetOffset(&m_vData[nID].model[nCntModel]);

                // 親のIDの取得
                int nParent = m_vData[nID].model[nCntModel].GetParentID();

                if (nParent != -1)
                {// 親が存在していたら
                    pInstance->SetParent(pModel[nParent].get());
                }
                else
                {// 親が存在していなかったら
                    pInstance->SetParent(nullptr);
                }

                pModel.push_back(std::move(pInstance));
            }

            return nID;
        }

        nID++;
    }

    // ロード処理
    if (FAILED(Load(pFilePath, pModel, nID)))
    {
        return -1;
    }

    return nID++;
}

//===================================================
// ロード処理
//===================================================
HRESULT CMotionLoader::Load(const char* pFilePath, std::vector<std::unique_ptr<CModel>>& pModel, const int nCurrentID)
{
    // 省略用ファイルパス
    std::string filePath = pFilePath;

    // ファイルパスが省略されていたら
    if (filePath.find("data/MOTION/") == std::string::npos)
    {
        // 文字列を連結
        filePath = "data/MOTION/" + filePath;
    }

    std::fstream file(filePath);
    std::string line;

    // キャラクター設定ができるかどうか
    bool bCharacterSet = true;

    // カウント用モーション
    int nCntMotion = 0;
    
    // 配列を増やす
    m_vData.resize(m_vData.size() + 1);

    // ファイルパスの取得
    m_vData[nCurrentID].filePath = filePath;

    // ファイルが開け無かったr
    if (!file.is_open())
    {
        MessageBox(NULL, "読み込み失敗", filePath.c_str(), MB_OK | MB_ICONWARNING);
        return E_FAIL;
    }

    // ファイルを一行ずつ読み取る
    while (std::getline(file, line))
    {
        // モデルの読み込み処理
        LoadModel(pModel,line, nCurrentID);

        // キャラクターの設定ができるなら
        if (bCharacterSet)
        {
            // キャラクター設定の読み込み処理
            bCharacterSet = LoadCharacterSet(pModel, line, nCurrentID);
        }
        else if (LoadMotionSet(line, nCntMotion, nCurrentID) == false)
        {
            // 読み込めなかったら処理を抜ける
            break;
        }
    }

    return S_OK;
}

//***************************************************
// モデルのロード処理
//***************************************************
void CMotionLoader::LoadModel(std::vector<std::unique_ptr<CModel>>& pModel, std::string& line, const int nCurrentID)
{
    std::string input;

    size_t equal_pos = line.find("="); // =の位置

    // [=] から先を求める
    input = line.substr(equal_pos + 1);

    // 読み取れたら
    if (line.find("NUM_MODEL") != std::string::npos)
    {
        int nNumModel = 0;

        // 値を取得
        std::istringstream value(input);

        value >> nNumModel;

        // モデル数の取得
        m_nNumModel = nNumModel;

        // サイズの確保
        pModel.reserve(nNumModel);
        m_vData[nCurrentID].model.resize(nNumModel);
    }

    if (line.find("MODEL_FILENAME") != std::string::npos)
    {
        // 数値を読み込む準備
        std::istringstream value(input);

        // モデルの名前格納用変数
        std::string modelName;

        // 数値を代入する
        value >> modelName;

        // モデルの生成
        auto pInstance = std::unique_ptr<CModel>(CModel::Create(modelName.c_str()));

        // モデルの取得
        pModel.push_back(std::move(pInstance));
    }
}
//***************************************************
// キャラクターセットのロード処理
//***************************************************
bool CMotionLoader::LoadCharacterSet(std::vector<std::unique_ptr<CModel>>& pModel, std::string& line, const int nCurrentID)
{
    std::string input;
    static int nIdx = 0;
    int nNumParts = 0;
    int nParent = 0;
    D3DXVECTOR3 offset = Const::VEC3_NULL;

    size_t equal_pos = line.find("="); // =の位置

    // [=] から先を求める
    input = line.substr(equal_pos + 1);

    if (line.find("NUM_PARTS") != std::string::npos)
    {
        // 数値を読み込む準備
        std::istringstream value(input);

        // 数値を代入する
        value >> nNumParts;
    }

    if (line.find("INDEX") != std::string::npos)
    {
        // 数値を読み込む準備
        std::istringstream value(input);

        // 数値を代入する
        value >> nIdx;

        std::string modelPath = pModel[nIdx]->GetModelPath();

        m_vData[nCurrentID].model[nIdx].SetModelPath(modelPath.c_str());
    }

    if (line.find("PARENT") != std::string::npos)
    {
        // 数値を読み込む準備
        std::istringstream value(input);

        // 数値を代入する
        value >> nParent;

        if (nParent != -1)
        {// 親が存在していたら
            pModel[nIdx]->SetParent(pModel[nParent].get());
        }
        else
        {// 親が存在していなかったら
            pModel[nIdx]->SetParent(nullptr);
        }

        pModel[nIdx]->SetParentID(nParent);

        m_vData[nCurrentID].model[nIdx].SetParentID(nParent);
    }

    if (line.find("MODEL_PART") != std::string::npos)
    {
        int nPart = -1;

        // 数値を読み込む準備
        std::istringstream value(input);

        // 数値を代入する
        value >> nPart;

        if (nPart != -1)
        {
            //pModel[nIdx]->SetParts(nPart == 1 ? true : false);
        }
    }

    if (line.find("POS") != std::string::npos)
    {
        // 数値を読み込む準備
        std::istringstream value(input);

        // 数値を代入する
        value >> offset.x;
        value >> offset.y;
        value >> offset.z;

        // オフセットの取得
        pModel[nIdx]->SetOffSetPosition(offset);
        m_vData[nCurrentID].model[nIdx].SetOffSetPosition(offset);
    }

    if (line.find("ROT") != std::string::npos)
    {
        // 数値を読み込む準備
        std::istringstream value(input);

        // 数値を代入する
        value >> offset.x;
        value >> offset.y;
        value >> offset.z;

        // オフセットの取得
        pModel[nIdx]->SetOffSetRotation(offset);
        m_vData[nCurrentID].model[nIdx].SetOffSetRotation(offset);
    }

    if (line.find("END_CHARACTERSET") != std::string::npos)
    {
        nIdx = 0;
        return false;
    }

    return true;
}
//***************************************************
// モーション設定のロード処理
//***************************************************
bool CMotionLoader::LoadMotionSet(std::string& currentLine, int& nCntMotion, const int nCurrentID)
{
    std::string input; // 値取得用文字列
    int loop = 0;      // ループ条件取得用変数
    static int nKey = 0;        // 現在のキー
    static int nCntModel = 0;   // モデルのカウント

    //// モーションのカウントが総数以上だったら読み込まない
    //if (nCntMotion >= motionComp.nNumMotion)
    //{
    //    nKey = 0;        // 現在のキーリセット
    //    nCntModel = 0;   // モデルのカウントリセット

    //    return false;
    //}

    size_t equal_pos = currentLine.find("="); // =の位置

    // [=] から先を求める
    input = currentLine.substr(equal_pos + 1);

    if (currentLine.find("LOOP") != std::string::npos)
    {
        // 数値を読み込む準備
        std::istringstream value(input);

        // 数値を代入する
        value >> loop;

        // ループするかどうか
        m_inputInfo.bLoop = (loop == 1) ? true : false;
    }

    if (currentLine.find("NUM_KEY") != std::string::npos)
    {
        // 数値を読み込む準備
        std::istringstream value(input);

        int nNumKey = 0;

        // 数値を代入する
        value >> nNumKey;
        m_inputInfo.nNumkey = nNumKey;

        // キーの総数分配列を確保する
        m_inputInfo.vKeyInfo.resize(nNumKey);

        // キーの総数分回す
        for (auto& keies : m_inputInfo.vKeyInfo)
        {
            // モデルのサイズ分配列の確保
            keies.vKey.resize(m_nNumModel);
        }
    }

    if (currentLine.find("NUM_FRAME") != std::string::npos)
    {
        // 数値を読み込む準備
        std::istringstream value(input);

        int nNumFrame = 0;

        // 数値を代入する
        value >> nNumFrame;
        m_inputInfo.nNumEventFrame = nNumFrame;

        // イベントフレームの総数分メモリの確保
        m_inputInfo.aStartFrame.reserve(nNumFrame);
        m_inputInfo.aEndFrame.reserve(nNumFrame);
    }

    if (currentLine.find("START_FRAME") != std::string::npos)
    {
        // 数値を読み込む準備
        std::istringstream value(input);

        int nFrame = 0;

        // 残りの値をすべて読み込む
        while (value >> nFrame)
        {
            m_inputInfo.aStartFrame.push_back(nFrame);
        }
    }
    if (currentLine.find("END_FRAME") != std::string::npos)
    {
        // 数値を読み込む準備
        std::istringstream value(input);

        int nFrame = 0;

        // 残りの値をすべて読み込む
        while (value >> nFrame)
        {
            m_inputInfo.aEndFrame.push_back(nFrame);
        }
    }

    if (currentLine.find("FRAME") != std::string::npos)
    {
        // 数値を読み込む準備
        std::istringstream value(input);

        // 数値を代入する
        value >> m_inputInfo.vKeyInfo[nKey].nFrame;
    }

    if (currentLine.find("POS") != std::string::npos)
    {
        // 数値を読み込む準備
        std::istringstream value(input);

        // 最大数をオーバーしていたら
        if (nCntModel >= m_nNumModel)
        {
            // エラー情報を設定
            std::ostringstream oss;

            oss << "現在のモーション = " << nCntMotion << "現在のキー = " << nKey << "現在のモデル = " << nCntModel << "\n"
                << "最大モーション数 = " << m_nNumModel << "最大モデル数 = " << m_nNumModel;

            // 情報を取得
            std::string message = oss.str();

            // エラー情報を表示
            MessageBox(NULL, message.c_str(), "MOTION_POS", MB_OK | MB_ICONWARNING);
            return false;
        }

        // 数値を代入する
        value >>m_inputInfo.vKeyInfo[nKey].vKey[nCntModel].fPosX;
        value >>m_inputInfo.vKeyInfo[nKey].vKey[nCntModel].fPosY;
        value >>m_inputInfo.vKeyInfo[nKey].vKey[nCntModel].fPosZ;
    }
    if (currentLine.find("ROT") != std::string::npos)
    {
        // 数値を読み込む準備
        std::istringstream value(input);

        // 数値を代入する
        value >> m_inputInfo.vKeyInfo[nKey].vKey[nCntModel].fRotX;
        value >> m_inputInfo.vKeyInfo[nKey].vKey[nCntModel].fRotY;
        value >> m_inputInfo.vKeyInfo[nKey].vKey[nCntModel].fRotZ;
    }

    if (currentLine.find("END_KEY") != std::string::npos)
    {
        nCntModel++;
    }

    if (currentLine.find("END_KEYSET") != std::string::npos)
    {
        nKey++;
        nCntModel = 0;
    }
    if (currentLine.find("END_MOTIONSET") != std::string::npos)
    {
        nKey = 0;

        m_vData[nCurrentID].info.push_back(m_inputInfo);
        m_inputInfo.vKeyInfo.clear();

        // 最大数じゃないなら
        nCntMotion++;
    }

    return true;
}