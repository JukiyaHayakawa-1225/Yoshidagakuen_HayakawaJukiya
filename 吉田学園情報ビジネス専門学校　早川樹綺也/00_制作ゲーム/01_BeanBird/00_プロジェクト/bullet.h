//=============================================================================
//
// 弾の処理 [bullet.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"
#include "scene2D.h"

#define TEXTURE_NAME001 "data\\TEXTURE\\bullet000.png"			//テクスチャ名
#define MAX_BULLET			(3)
#define BULLET_HIT			(30)								// 当たり判定

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CBullet : public CScene2D
{
public:
	//=========================================================================
	// 構造体の定義
	//=========================================================================
	typedef enum
	{
		BULLETTYPE_NONE = 0,
		BULLETTYPE_PLAYER_SALIVA,		//プレイヤーの唾
		BULLETTYPE_PLAYER_SHIT,			//プレイヤーの糞
		BULLETTYPE_ENEMY,				//敵
		BULLETTYPE_MAX,
	} BULLETTYPE;
	//メンバ関数
	CBullet();
	~CBullet();
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Collision(void);
	//静的メンバ関数
	static CBullet *Create(D3DXVECTOR3 pos,D3DXVECTOR3 move, float fWight, float fHeight,BULLETTYPE type);

private:
	//メンバ変数
	int m_nLife;						//体力
	D3DXVECTOR3 m_move;					//移動量
	BULLETTYPE m_bulletType;
	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_BULLET];
};
#endif