//================================================
//
// 入力処理 [input.h]
// Author:YUTO YOSHIDA
//
//=================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _INPUT_H_
#define _INPUT_H_

//*************************************************
// インクルードファイル
//*************************************************
#include"main.h"

//*************************************************
// 入力クラスの定義
//*************************************************
class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInstance);
	virtual void Uninit(void);
	virtual void Update(void) = 0;
protected:
	static LPDIRECTINPUT8 m_pInput; // DIrectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8 m_pDevice;	// 入力デバイスへのポインタ
};

//*************************************************
// キーボードクラスの定義
//*************************************************
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(const int nKey);
	bool GetTrigger(const int nkey);
	bool GetRelease(const int nKey);
	bool GetRepeat(const int nKey, const int nMaxCnt);
	bool GetPressAnyKey(void);
private:
	static constexpr int MAX_KEYBOARD = 256; // キーの数

	BYTE m_aKeyState[MAX_KEYBOARD];		// キーボードの情報
	BYTE m_OldKeyState[MAX_KEYBOARD];	// 前のキーボードの情報
	int m_nRepeatCount;					// キーの長押しカウンター
	bool m_bFirstPress;					// 最初に押した判定用変数
};

//*************************************************
// パッドの定義
//*************************************************
class CInputJoypad : public CInput
{
public:
	CInputJoypad();
	~CInputJoypad();

	// ボタンの種類
	typedef enum
	{
		JOYKEY_UP = 0,					// 十字キー上
		JOYKEY_DOWN,					// 十字キー下
		JOYKEY_LEFT,					// 十字キー左
		JOYKEY_RIGHT,					// 十字キー右
		JOYKEY_START,					// START
		JOYKEY_BAKC,					// BACK
		JOYKEY_LEFT_THUMB,
		JOYKEY_RIGHT_THUMB,
		JOYKEY_LEFT_SHOULDER,			// 左トリガー
		JOYKEY_RIGHT_SHOULDER,			// 右トリガー
		JOYKEY_L2,
		JOYKEY_R2,
		JOYKEY_A,						// Aボタン
		JOYKEY_B,						// Bボタン
		JOYKEY_X,						// Xボタン
		JOYKEY_Y,						// Yボタン
		JOYKEY_MAX
	}JOYKEY;

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	bool GetPress(const int nKey);
	bool GetTrigger(const int nKey);
	bool GetRelease(const int nKey);
	bool GetRepeat(const int nKey, const int nMaxCnt);
	bool GetJoyStickL(void);
	bool GetJoyStickR(void);
	XINPUT_STATE* GetJoyStickAngle(void);
	bool GetPressAnyButton(void);
	bool GetTriggerTrigger(const int nTrigger);
	bool GetTriggerPress(const int nTrigger);
private:

	static constexpr int DEAD_ZONE = 10000; // デッドゾーン

	XINPUT_STATE m_joyKeyState;				// ジョイパッドのプレス情報
	XINPUT_STATE m_OldKeyState;				// ジョイパッドのプレス情報(判定)
	XINPUT_STATE m_joyKeyStateTrigger;		// ジョイパッドのトリガー情報
	XINPUT_STATE m_joykeyStateRelease;		// ジョイパッドのリリース情報
	int m_nRepeatCount;						// キーの長押しカウンター
	bool m_bFirstPress;						// 最初に押した判定用変数
};

//*************************************************
// マウスクラスの定義
//*************************************************
class CInputMouse : public CInput
{
public:
	static constexpr int LEFT = 0;   // 左クリック
	static constexpr int RIGHT = 1;  // 右クリック
	static constexpr int WHEEL = 2;  // ホイールクリック

	CInputMouse();
	~CInputMouse();

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool OnMousePress(int button_type);
	bool OnMouseTriggerDown(int button_type);
	bool OnMouseTriggerUp(int button_type);
	bool OnMouseRepet(int button_type, const int nMaxCnt);
	D3DXVECTOR2 GetVelocity(void);
	D3DXVECTOR2 GetOldVelocity(void);
	int GetMouseWeel(void);
private:
	DIMOUSESTATE m_PrevState;		// マウスの過去の情報
	DIMOUSESTATE m_CurrentState;	// マウスの現在の情報
	int m_nRepeatCount;				// キーの長押しカウンター
	bool m_bFirstPress;				// 最初に押した判定用変数
};

#endif // !_INPUT_H_