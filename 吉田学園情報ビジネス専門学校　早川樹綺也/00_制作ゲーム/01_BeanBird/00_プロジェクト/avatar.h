//=============================================================================
//
// 分身の処理 [avatar.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _AVATAR_H_
#define _AVATAR_H_

#include "main.h"
#include "scene2D.h"

#define MAX_AVATAR (2)											//分身の数

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CAvatar : public CScene2D
{
public:
	//=========================================================================
	// 分身の状態
	//=========================================================================
	typedef enum
	{
		AVATAR_NONE = 0,		//使用していない状態
		AVATAR_USE,				//使用している状態
	} AVATAR_STATE;
	//メンバ関数
	CAvatar();
	~CAvatar();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void FollowPlayer(void);
	static AVATAR_STATE GetAvatar(void);
	static void SetAvatar(AVATAR_STATE);
	//静的メンバ関数
	static CAvatar *Create(D3DXVECTOR3 pos, float fWight, float fHeight);
	static HRESULT Load(void);
	static void Unload(void);

private:
	D3DXVECTOR3 m_move;						//移動量
	//静的メンバ変数
	static	AVATAR_STATE m_state;			//分身の状態
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャ
};
#endif