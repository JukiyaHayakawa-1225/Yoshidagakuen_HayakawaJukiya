//=============================================================================
//
// ポーズ処理 [pause.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"
#include "scene.h"

#define MAX_PAUSE_SELECT     (SELECT_MAX)               // ポーズ選択できる項目の数
#define MAX_PAUSE_TEXTURE	 (4)						// ポーズのテクスチャ総数

class CScene2D;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPause : public CScene
{
public:
	//*************************************
	// 項目の状態
	//*************************************
	typedef enum
	{
		PAUSESELECT_NONE = 0,   // 選択されていない状態
		PAUSESELECT_SELECT,     // 選択されている状態
		PAUSESELECT_MAX
	}PAUSESELECT;

	typedef enum
	{
		SELECT_CONTINUE = 0,// コンティニュー
		SELECT_RETRY,		// ゲームリトライ
		SELECT_QUIT,		// タイトル遷移
		SELECT_MAX
	}SELECT;

	//メンバ関数
	CPause();
	~CPause();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);
	static void Unload(void);

	//静的メンバ関数
	static CPause *Create(void);

private:
	//メンバ変数
	CScene2D			*m_pScene2D;					//オブジェクト2Dのポインタ
	CScene2D			*m_apSelect[MAX_PAUSE_SELECT];	//選択数分のオブジェクト2Dのポインタ
	D3DXCOLOR			m_aCol[MAX_PAUSE_SELECT];		//選択数分の色情報
	float				m_fCol;							// 色の変化に使用
	PAUSESELECT			m_aSelect[MAX_PAUSE_SELECT];	//選択状態の情報
	SELECT				m_select;
	int					m_nSelect;
	//静的メンバ変数
	static LPDIRECT3DTEXTURE9	m_apTexture[MAX_PAUSE_TEXTURE];	//テクスチャのポインタ
protected:
};
#endif