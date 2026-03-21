//===================================================
//
// デバッグ情報の描画 [debug_proc.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "string"

//***************************************************
// マネージャークラスの定義
//***************************************************
class CDebugProc
{
public:
	CDebugProc();
	~CDebugProc();

	HRESULT Init(void);
	void Uninit(void);
	static void Print(const char* fmt...);
	static void Draw(void);
	static void EnableShow(const bool bShow) { m_bShow = bShow; }
private:
	static LPD3DXFONT m_pFont;		// フォントへのポインタ
	static std::string m_string;	// 文字列を格納するバッファ
	static bool m_bShow;			// 表示するかどうか
};
#endif