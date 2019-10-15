//=============================================================================
//
// 豆の処理 [beans.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _BEAMS_H_
#define _BEAMS_H_

#include "main.h"
#include "scene2D.h"

#define TEXTURE_NAME007 "data\\TEXTURE\\beans_score.png"			//テクスチャ名
#define MAX_BEANS_TEXTURE (6)										//豆のテクスチャの最大数
#define BEANS_SIZE	(25)
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CBeans : public CScene2D
{
public:
	//=========================================================================
	// 構造体の定義
	//=========================================================================
	typedef enum
	{
		BEANS_TYPE_NONE = 0,
		BEANS_TYPE_SCORE,		//スコア豆
		BEANS_TYPE_BULLET,		//弾豆
		BEANS_TYPE_SPEED,		//スピード豆
		BEANS_TYPE_AVATAR,		//分身豆
		BEANS_TYPE_BARRIER,		//バリア豆
		BEANS_TYPE_MAX,
	} BEANS_TYPE;
	typedef enum
	{
		BEANS_FLOATING = 0,		//浮遊状態
		BEANS_FALL,				//落下状態
		BEANS_MAX,
	} BEANS_STATE;

	//メンバ関数
	CBeans();
	~CBeans();
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Collision(void);
	float GetRot(void);
	void HitBeans(D3DXVECTOR3 pos);
	void SetMove(D3DXVECTOR3 move);
	//静的メンバ関数
	static CBeans *Create(D3DXVECTOR3 pos, float fWight, float fHeight);

private:
	int				m_nCountBeans;
	BEANS_TYPE		m_beansType;
	BEANS_STATE		m_beansState;
	D3DXVECTOR3		m_move;
	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_BEANS_TEXTURE];
};
#endif