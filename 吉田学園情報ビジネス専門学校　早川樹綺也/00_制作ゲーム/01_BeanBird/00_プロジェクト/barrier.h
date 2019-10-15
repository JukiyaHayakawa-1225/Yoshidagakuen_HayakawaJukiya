//=============================================================================
//
// バリアの処理 [barrier.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _BARRIER_H_
#define _BARRIER_H_

#include "main.h"
#include "scene2D.h"

#define TEXTURE_NAME015 "data\\TEXTURE\\barrier.png"			//テクスチャ名
#define BARRIER_SIZE (60)
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CBarrier : public CScene2D
{
public:
	//=========================================================================
	// バリアの状態
	//=========================================================================
	typedef enum
	{
		STATE_NONE = 0,	//バリアを使用している
		STATE_USE,		//バリアを使用していない
		STATE_MAX,

	} BARRIER_STATE;

	//メンバ関数
	CBarrier();
	~CBarrier();
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void FollowPlayer(void);
	static BARRIER_STATE GetState(void);
	static void SetState(BARRIER_STATE state);

	//静的メンバ関数
	static CBarrier *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWight, float fHeight);

private:
	//メンバ変数
	static BARRIER_STATE m_state;
	int m_timer;
	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;
};
#endif