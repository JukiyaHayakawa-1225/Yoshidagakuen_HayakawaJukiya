//=============================================================================
//
// 豆の処理 [player.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "beans.h"
#include "game.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "bullet.h"
#include "player.h"
#include "barrier.h"
#include "avatar.h"
#include "skill.h"
#include "score.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CBeans::m_apTexture[MAX_BEANS_TEXTURE] = {};		//テクスチャのポインタ

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// 豆のコンストラクタ
//=============================================================================
CBeans::CBeans() : CScene2D(3)
{
	m_nCountBeans = 0;						//豆の当たった回数を初期値
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//移動量の初期値
	m_beansType = BEANS_TYPE_SCORE;			//豆の種類の初期
	m_beansState = BEANS_FLOATING;			//豆の状態の初期
}
//=============================================================================
// 豆のデストラクタ
//=============================================================================
CBeans::~CBeans()
{

}

//=============================================================================
//豆の生成
//=============================================================================
CBeans *CBeans::Create(D3DXVECTOR3 pos, float fWight, float fHeight)
{
	CBeans *pBeans = NULL;									//豆のポインタ
	if (pBeans == NULL)
	{//NULLの場合
		pBeans = new CBeans;								//豆の動的確保
		pBeans->SetPos(pos);								//位置の代入
		pBeans->SetWidth(fWight);							//幅の代入
		pBeans->SetHeight(fHeight);							//高さの代入
		pBeans->Init();										//初期化処理
		pBeans->BindTexture(m_apTexture[5]);				//テクスチャ
		pBeans->m_nCountBeans = 0;							//豆の初期化
		pBeans->m_move = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//移動量の初期化
	}
	return pBeans;											//値を返す
}

//=============================================================================
// オブジェクトのテクスチャ読み込み
//=============================================================================
HRESULT CBeans::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME007,
		&m_apTexture[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\beans_bullet.png",
		&m_apTexture[1]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\beans_avatar.png",
		&m_apTexture[2]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\beans_barrier.png",
		&m_apTexture[3]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\beans_speed.png",
		&m_apTexture[4]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\beans_floating.png",
		&m_apTexture[5]);
	return S_OK;
}

//=============================================================================
// オブジェクトのテクスチャ破棄
//=============================================================================
void CBeans::Unload(void)
{
	for (int nCntEnemyTex = 0; nCntEnemyTex < MAX_BEANS_TEXTURE; nCntEnemyTex++)
	{//テクスチャの数だけ繰り返し
		// テクスチャの破棄
		if (m_apTexture[nCntEnemyTex] != NULL)
		{
			m_apTexture[nCntEnemyTex]->Release();
			m_apTexture[nCntEnemyTex] = NULL;
		}
	}
}

//=============================================================================
// 豆の初期化処理
//=============================================================================
HRESULT CBeans::Init(void)
{
	//CScene2Dの初期化処理
	CScene2D::Init();
	//種類の設定
	SetObjType(OBJTYPE_BEANS);
	return S_OK;
}

//=============================================================================
// 豆の終了処理
//=============================================================================
void CBeans::Uninit(void)
{
	//CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 豆の更新処理
//=============================================================================
void CBeans::Update(void)
{
	//豆の当たり判定処理
	Collision();
	//CScene2Dの更新処理
	CScene2D::Update();

}

//=============================================================================
// 豆の描画処理
//=============================================================================
void CBeans::Draw(void)
{
	//CScene2Dの描画処理
	CScene2D::Draw();
}

//=============================================================================
// 豆の当たり判定処理
//=============================================================================
void CBeans::Collision(void)
{
	D3DXVECTOR3 pos;	//位置
	pos = GetPos();		//位置の取得

	m_rot -= 0.05f;	//向きの回転処理

	if (m_rot > D3DX_PI)
	{
		m_rot -= D3DX_PI * 2.0f;
	}
	else if (m_rot < -D3DX_PI)
	{
		m_rot += D3DX_PI * 2.0f;
	}

	if (m_beansState == BEANS_FLOATING)
	{//豆が浮遊状態の場合
	 //豆の移動
		D3DXVECTOR3 SetVtxPos(0.0f, -2.0f, 0.0f);
		m_move = SetVtxPos;
		//豆を落下状態にする
		m_beansState = BEANS_FALL;
	}
	else if (m_beansState == BEANS_FALL)
	{//豆が落ちる状態の場合
		m_move.y += 0.03f;	//移動量の加算
	}
	pos += m_move;			//移動量の加算

	//位置の設置
	CScene2D::SetVtxRotPos(pos);

	for (int nCntPrioty = 0; nCntPrioty < 6; nCntPrioty++)
	{//優先度の5番まで繰り返し
		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{//Sceneの数だけ繰り返し
			CScene2D *pScene;												//シーンのポインタ
			pScene = (CScene2D*)CScene::GetScene(nCntPrioty, nCntScene);	//シーンの取得
			if (pScene != NULL)
			{//SceneがNULLでない場合
				CScene::OBJTYPE objType;					//種類
				objType = pScene->GetObjType();				//種類の取得

				if (objType == OBJTYPE_PLAYER)
				{//プレイヤーの場合
					D3DXVECTOR3 PlayerPos;					//敵の位置
					CPlayer *pPlayer = (CPlayer*)pScene;	//豆の動的確保
					PlayerPos = pPlayer->GetPos();			//位置の取得
					CSkill *pSkill = NULL;					//スキルのポインタ
					CScore *pScore = CGame::GetScore();		//スコアの取得

					if (pPlayer != NULL)
					{//プレイヤーがNULLでない場合
						if (pos.x > PlayerPos.x - PLAYER_SIZE
							&& pos.x < PlayerPos.x + PLAYER_SIZE
							&& pos.y <  PlayerPos.y + PLAYER_SIZE
							&& pos.y > PlayerPos.y - PLAYER_SIZE)
						{//プレイヤーが豆の範囲内の場合
							switch (m_beansType)
							{
							case BEANS_TYPE_SCORE:		//スコア豆
								pScore->AddScore(1000);	//スコアの加算
								break;
							case BEANS_TYPE_BULLET:		//弾豆
														//弾を２Wayにする
								pPlayer->SetBullet(pPlayer->BULLET_SKILL);
								//弾を使用している状態にする
								pSkill->SetSkill(pSkill->SKILL_2WAY, pSkill->SKILL_STATE_USE);
								break;
							case BEANS_TYPE_SPEED:		//スピード豆
														//プレイヤーのスピードを上げる
								pPlayer->SetSpeed(CPlayer::SPEED_SKILL);
								//スピードを使用している状態にする
								pSkill->SetSkill(pSkill->SKILL_SPEED, pSkill->SKILL_STATE_USE);
								break;
							case BEANS_TYPE_AVATAR:		//分身豆
								CAvatar::AVATAR_STATE pAvatarState;				//分身状態
								pAvatarState = CAvatar::GetAvatar();			//分身の状態取得
								if (pAvatarState == CAvatar::AVATAR_NONE)
								{//分身が使用していない状態にする
									CAvatar::Create(PlayerPos, 40.0f, 40.0f);	//分身の生成
																				//分身を使用している状態にする
									pSkill->SetSkill(pSkill->SKILL_AVATAR, pSkill->SKILL_STATE_USE);
								}
								break;
							case BEANS_TYPE_BARRIER:	//バリア豆
								CBarrier::BARRIER_STATE pBarrierState;	//バリアの状態
								pBarrierState = CBarrier::GetState();	//バリアの取得
								if (pBarrierState == CBarrier::STATE_NONE)
								{//バリアが使用されていない状態
								 //バリアの生成
									CBarrier::Create(D3DXVECTOR3(PlayerPos.x, PlayerPos.y, PlayerPos.z), D3DXVECTOR3(0, 0.0, 0.0f), BARRIER_SIZE, BARRIER_SIZE);
									//バリアを使用している状態にする
									pSkill->SetSkill(pSkill->SKILL_BARRIER, pSkill->SKILL_STATE_USE);
								}
								break;
							}
							//豆の終了処理
							CBeans::Uninit();
						}
					}
				}
			}
		}
	}
	if (m_beansState == BEANS_FLOATING)
	{//豆が浮遊状態の場合
		BindTexture(m_apTexture[5]);
	}
	else if (m_beansState == BEANS_FALL)
	{//豆が落下状態の場合
		switch (m_beansType)
		{
		case BEANS_TYPE_SCORE:			//スコア豆
			BindTexture(m_apTexture[0]);
			break;
		case BEANS_TYPE_BULLET:			//弾豆
			BindTexture(m_apTexture[1]);
			break;
		case BEANS_TYPE_AVATAR:			//分身豆
			BindTexture(m_apTexture[2]);
			break;
		case BEANS_TYPE_BARRIER:		//バリア豆
			BindTexture(m_apTexture[3]);
			break;
		case BEANS_TYPE_SPEED:			//スピード豆
			BindTexture(m_apTexture[4]);
			break;
		}
	}
	if (pos.y >= SCREEN_HEIGHT + 50 || pos.x <= 250.0f || pos.x >= SCREEN_WIDTH)
	{//画面外にいった場合
		//豆の終了処理
		CBeans::Uninit();
	}
}

//=============================================================================
// 向きの取得処理
//=============================================================================
float CBeans::GetRot(void)
{
	return m_rot;		//値を返す
}

//=============================================================================
// 豆のダメージ処理
//=============================================================================
void CBeans::HitBeans(D3DXVECTOR3 pos)
{
	int nType;					//種類
	if (m_beansState == BEANS_FALL)
	{//豆が落下状態の場合
		m_nCountBeans++;		//豆のカウント加算
		D3DXVECTOR3 BeansPos;	//位置
		BeansPos = GetPos();	//位置の取得
		if (m_nCountBeans == 3)
		{//豆に３回あった場合
			//ランダムで切り替わる
			nType = rand() % 4 + 2;
			switch (nType)
			{
			case BEANS_TYPE_BULLET:
				//弾豆にする
				m_beansType = BEANS_TYPE_BULLET;
				break;
			case BEANS_TYPE_SPEED:
				//スピード豆にする
				m_beansType = BEANS_TYPE_SPEED;
				break;
			case BEANS_TYPE_AVATAR:
				//分身豆にする
				m_beansType = BEANS_TYPE_AVATAR;
				break;
			case BEANS_TYPE_BARRIER:
				//バリア豆にする
				m_beansType = BEANS_TYPE_BARRIER;
				break;
			}
		}
		else if (m_nCountBeans == 4)
		{//4回目の場合
			m_beansType = BEANS_TYPE_SCORE;	//スコア豆にする
			m_nCountBeans = 0;				//豆のカウント初期化
		}
		////角度の設定
		//float fAngle = atan2f(pos.x - BeansPos.x, pos.y - BeansPos.y);
		////位置設置
		//D3DXVECTOR3 SetVtxPos(sinf(fAngle) * -0.5f, cosf(fAngle) * -3.0f, 0.0f);
		if (pos.x >= 790)
		{
			D3DXVECTOR3 leftPos = D3DXVECTOR3(-0.5f, -2.0f, 0.0f);
			m_move = leftPos;
		}
		else if (pos.x <= 790)
		{
			D3DXVECTOR3 rightPos = D3DXVECTOR3(0.5f, -2.0f, 0.0f);
			m_move = rightPos;
		}
	}
}

//=============================================================================
// 移動量の設置処理
//=============================================================================
void CBeans::SetMove(D3DXVECTOR3 move)
{
	D3DXVECTOR3 pos;	//位置

	pos = GetPos();		//位置の取得
	m_move = move;		//移動量代入
	m_move.y += 0.5;	//
	pos += m_move;		//

}