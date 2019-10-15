//=============================================================================
//
// 背景の処理 [bg.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

#include "main.h"
#include "scene.h"
#include "result.h"

#define MAX_BG (3)											//背景の最大数

#define TEXTURE_NAME003 "data\\TEXTURE\\BG000.png"			//テクスチャ名
#define TEXTURE_NAME004 "data\\TEXTURE\\bg101.png"			//テクスチャ名
#define TEXTURE_NAME005 "data\\TEXTURE\\bg102.png"			//テクスチャ名

class CScene2D;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CBg : public CScene
{
public:
	//メンバ関数
	CBg(int nPriority = 1);
	~CBg();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 texture);
	void SetPos(D3DXVECTOR3 pos);
	void SetVtxPos(D3DXVECTOR3 pos);
	void SetTexMove(int move);
	void SetCol(D3DXCOLOR col);
	void SetWidth(float fWidth);
	void SetHeight(float fHeight);
	D3DXVECTOR3 GetPos(void);
private:
	//メンバ変数
	D3DXVECTOR3 m_pos;	//位置
	int m_move;			//移動量
	D3DXCOLOR	m_col;	//色
	float		m_fWidth;
	float		m_fHeight;
	LPDIRECT3DVERTEXBUFFER9				m_pVtxBuff;			// 頂点バッファへのポインタ

	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;
};

//*****************************************************************************
// タイトル背景
//*****************************************************************************
class CBgTitle : public CBg
{
public:
	//メンバ関数
	CBgTitle();
	~CBgTitle();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CBgTitle *Create(D3DXVECTOR3 pos, float fWight, float fHeight);
	static HRESULT Load(void);
	static void Unload(void);
private:
	//メンバ変数
	D3DXVECTOR3 m_move;	//移動量
	int m_nMoveSpeed;
	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;
};
//*****************************************************************************
// チュートリアル背景
//*****************************************************************************
class CBgTutorial : public CBg
{
public:
	typedef enum
	{
		BG_0 = 0,
		BG_1,
		BG_MAX,
	}BG_TYPE;
	typedef enum
	{
		STATE_NOMAL = 0,
		STATE_MOVE,
		STATE_REMOVE,
		STATE_MAX,
	}TUTORIAL_STATE;
	//メンバ関数
	CBgTutorial();
	~CBgTutorial();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static void SetTutorialState(TUTORIAL_STATE state);
	static TUTORIAL_STATE GetTutorialState(void);
	static CBgTutorial *Create(D3DXVECTOR3 pos, float fWight, float fHeight,BG_TYPE type);
	static HRESULT Load(void);
	static void Unload(void);
private:
	int						  m_moveTimer;
	CScene2D *m_apScene2D[2];
	//静的メンバ変数
	static TUTORIAL_STATE	  m_state;
	static LPDIRECT3DTEXTURE9 m_apTexture[2];
};

//*****************************************************************************
// ゲーム背景
//*****************************************************************************
class CBgGame : public CBg
{
public:
	//メンバ関数
	CBgGame();
	~CBgGame();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CBgGame *Create(D3DXVECTOR3 pos, float fWight, float fHeight);
	static HRESULT Load(void);
	static void Unload(void);
private:
	//メンバ変数
	D3DXVECTOR3 m_move;	//移動量
	int m_nMoveSpeed;
	CScene2D *m_pScene2D;
	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;
};
//*****************************************************************************
// リザルト背景
//*****************************************************************************
class CBgResult : public CBg
{
public:
	//メンバ関数
	CBgResult();
	~CBgResult();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CBgResult *Create(D3DXVECTOR3 pos, float fWight, float fHeight,CResult::RESULT result);
	static HRESULT Load(void);
	static void Unload(void);

private:
	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_RESULT];
};

//*****************************************************************************
// ランキング背景
//*****************************************************************************
class CBgRanking : public CBg
{
public:
	//メンバ関数
	CBgRanking();
	~CBgRanking();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CBgRanking *Create(D3DXVECTOR3 pos , float fWight, float fHeight);
	static HRESULT Load(void);
	static void Unload(void);
private:
	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;
};

//*****************************************************************************
// UI背景
//*****************************************************************************
class CBgUI : public CBg
{
public:
	//メンバ関数
	CBgUI();
	~CBgUI();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CBgUI *Create(D3DXVECTOR3 pos, float fWight, float fHeight);
	static HRESULT Load(void);
	static void Unload(void);
private:
	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;
};
#endif