//=============================================================================
//
// 入力処理 [input.h]
// Author :　Jukiya Hayakawa
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_KEY_MAX			(256)	// キーの最大数
#define DI_JOY_I_INPUT		(1000)

//*****************************************************************************
// クラス定義(入力処理)
//*****************************************************************************
class CInput
{
public:		//誰からもアクセス可能
	//メンバ関数
	CInput();
	//仮想関数
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	//純粋仮想関数
	virtual void Update(void) = 0;

private:	//自分だけがアクセス可能

protected:	//自分と派生クラスだけがアクセス可能
	//メンバ変数
	static LPDIRECTINPUT8 m_pInput;
	LPDIRECTINPUTDEVICE8 m_pDevice;
};

//*****************************************************************************
// クラス定義(キーボードの入力処理)
//*****************************************************************************
class CInputKeyboard : public CInput
{
public:		//誰からもアクセス可能
	//メンバ関数
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);

private:	//自分だけがアクセス可能
	//メンバ変数
	BYTE m_aKeyState[NUM_KEY_MAX];					// キーボードの入力情報（プレス情報）
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];			// キーボードの入力情報（トリガー情報）

protected:	//自分と派生クラスだけがアクセス可能
};
//*****************************************************************************
// クラス定義(ジョイパッドの入力処理)
//*****************************************************************************
class CInputJoypad : public CInput
{
public:		//誰からもアクセス可能
	typedef enum
	{
		DIJS_BUTTON_X = 0,
		DIJS_BUTTON_A,
		DIJS_BUTTON_B,
		DIJS_BUTTON_Y,
		DIJS_BUTTON_LB,
		DIJS_BUTTON_RB,
		DIJS_BUTTON_LT,
		DIJS_BUTTON_RT,
		DIJS_BUTTON_BACK,
		DIJS_BUTTON_START,
		DIJS_BUTTON_LSTICK_PUSH,
		DIJS_BUTTON_RSTICK_PUSH,
		DIJS_BUTTON_13,
		DIJS_BUTTON_14,
		DIJS_BUTTON_15,
		DIJS_BUTTON_16,
		DIJS_BUTTON_17,
		DIJS_BUTTON_18,
		DIJS_BUTTON_19,
		DIJS_BUTTON_UP,				//十字キー上
		DIJS_BUTTON_DOWN,			//十字キー下
		DIJS_BUTTON_LEFT,			//十字キー左
		DIJS_BUTTON_RIGHT,			//十字キー右
		DIJS_BUTTON_LSTICK_UP,		//左スティック上
		DIJS_BUTTON_LSTICK_DOWN,	//左スティック下
		DIJS_BUTTON_LSTICK_LEFT,	//左スティック左
		DIJS_BUTTON_LSTICK_RIGHT,	//左スティック右
		DIJS_BUTTON_RSTICK_UP,		//右スティック上
		DIJS_BUTTON_RSTICK_DOWN,	//右スティック下
		DIJS_BUTTON_RSTICK_LEFT,	//右スティック左
		DIJS_BUTTON_RSTICK_RIGHT,	//右スティック右
		DIJS_BUTTON_MAX,
	} DIJS_BUTTON;

	//メンバ関数
	CInputJoypad();
	~CInputJoypad();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(DIJS_BUTTON button);
	bool GetTrigger(DIJS_BUTTON button);
	float GetLeftAxiz(void);
	float GetRightAxiz(void);
	float GetRadian(void);

private:	//自分だけがアクセス可能
	//メンバ変数
	BYTE m_aButtonState[32];					// キーボードの入力情報（プレス情報）
	BYTE m_aButtonStateTrigger[32];			// キーボードの入力情報（トリガー情報）
	float m_LeftAxizX;
	float m_LeftAxizY;
	float m_RightAxizX;
	float m_RightAxizY;
	float m_Radian;

protected:	//自分と派生クラスだけがアクセス可能
};
#endif