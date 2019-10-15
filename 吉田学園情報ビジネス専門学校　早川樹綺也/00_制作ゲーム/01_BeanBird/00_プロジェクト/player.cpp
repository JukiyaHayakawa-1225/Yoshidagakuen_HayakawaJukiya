//=============================================================================
//
// プレイヤーの処理 [player.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "player.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "bullet.h"
#include "barrier.h"
#include "explotion.h"
#include "stock.h"
#include "avatar.h"
#include "life.h"
#include "fade.h"
#include "skill.h"
#include "result.h"
#include "score.h"
#include "number.h"
#include "scene.h"
#include "logo.h"
#include "sound.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;
CPlayer::STATE CPlayer::m_state = STATE_NOMAL;	//通常状態にする
CPlayer::BULLET_TYPE CPlayer::m_bulletType = BULLET_NOMAL;
//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================
bool					g_bDelete;

//=============================================================================
// プレイヤーのコンストラクタ
//=============================================================================
CPlayer::CPlayer() : CScene2D(5)
{
	m_nCount = 0;								//カウントの初期値
	m_move = D3DXVECTOR3(0.0f,0.0f,0.0f);		//移動量の初期値
	m_speed = PLAYER_NOMAL_SPEED;				//スピードの初期値
}
//=============================================================================
// プレイヤーのデストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
//プレイヤーの生成
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, float fWight, float fHeight,STATE state)
{
	CPlayer *pPlayer = NULL;
	if (pPlayer == NULL)
	{
		pPlayer = new CPlayer;					//シーンの動的確保
		pPlayer->SetPos(pos);					//位置の代入
		pPlayer->SetWidth(fWight);				//幅の代入
		pPlayer->SetHeight(fHeight);			//高さの代入
		pPlayer->m_state = state;				//状態の代入
		pPlayer->Init();						//初期化処理
		pPlayer->m_speed = PLAYER_NOMAL_SPEED;
		g_bDelete = false;						//使用していない状態にする
		pPlayer->BindTexture(m_pTexture);		//テクスチャ
	}
	return pPlayer;					//値を返す
}

//=============================================================================
// プレイヤーのテクスチャ読み込み
//=============================================================================
HRESULT CPlayer::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME000,
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// プレイヤーのテクスチャ破棄
//=============================================================================
void CPlayer::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// プレイヤーの初期化処理
//=============================================================================
HRESULT CPlayer::Init(void)
{
	//移動量の初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//弾を初期化
	m_bulletType = BULLET_NOMAL;
	//種類の設定
	SetObjType(OBJTYPE_PLAYER);
	//CScene2Dの初期化処理
	CScene2D::Init();
	return S_OK;
}

//=============================================================================
// プレイヤーの終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	//CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// プレイヤーの更新処理
//=============================================================================
void CPlayer::Update(void)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	//ジョイパッドの取得
	CInputJoypad *pInputJoypad;
	pInputJoypad = CManager::GetJoypad();

	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	D3DXVECTOR3 pos;								//位置
	pos = GetPos();									//位置の取得

	//加算処理
	pos += m_move;

	//慣性処理
	m_move.x += (0.0f - m_move.x) * 0.15f;
	m_move.y += (0.0f - m_move.y) * 0.15f;

	//位置の設置
	SetVtxPos(pos);

	switch (m_state)
	{
	case STATE_NOMAL:
		//範囲外処理
		if (pos.x <= 330.0f)
		{
			pos.x = 330.0f;
		}
		else if (pos.x >= SCREEN_WIDTH - 30.0f)
		{
			pos.x = SCREEN_WIDTH - 30.0f;
		}

		if (pos.y <= 30.0f)
		{
			pos.y = 30.0f;
		}
		else if (pos.y >= SCREEN_HEIGHT - 30.0f)
		{
			pos.y = SCREEN_HEIGHT - 30.0f;
		}

		//位置の設置
		SetVtxPos(pos);
		if (pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_LSTICK_UP) == true ||
			pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_LSTICK_DOWN) == true ||
			pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_LSTICK_LEFT) == true ||
			pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_LSTICK_RIGHT) == true)
		{
			//移動処理
			m_move.x += sinf(pInputJoypad->GetLeftAxiz()) * m_speed;
			m_move.y += cosf(pInputJoypad->GetLeftAxiz()) * m_speed;
		}

		if (pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_UP) == true ||
			pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_DOWN) == true ||
			pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_LEFT) == true ||
			pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_RIGHT) == true)
		{
			//移動処理
			m_move.x += sinf(pInputJoypad->GetRadian()) * m_speed;
			m_move.y -= cosf(pInputJoypad->GetRadian()) * m_speed;
		}

		if (pInputKeyboard->GetPress(DIK_W) == true)
		{//Wキーを押した場合
			m_move.y -= m_speed;
		}
		if (pInputKeyboard->GetPress(DIK_S) == true)
		{//Sキーを押した場合
			m_move.y += m_speed;
		}
		if (pInputKeyboard->GetPress(DIK_A) == true)
		{//Aキーを押した場合
			m_move.x -= m_speed;
		}
		if (pInputKeyboard->GetPress(DIK_D) == true)
		{//Dキーを押した場合
			m_move.x += m_speed;
		}
		if (pInputKeyboard->GetTrigger(DIK_SPACE) == true || pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_A) == true)
		{//SPACEキーを押した場合
			pSound->PlaySound(CSound::SOUND_LABEL_SE_BULLET);
			if (m_bulletType == BULLET_NOMAL)
			{
				//弾の生成
				CBullet::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0, -10.0f, 0.0f), 10.0f, 10.0f, CBullet::BULLETTYPE_PLAYER_SALIVA);
			}
			else if (m_bulletType == BULLET_SKILL)
			{
				CBullet::Create(D3DXVECTOR3(pos.x - 10.0f, pos.y, pos.z), D3DXVECTOR3(0, -10.0f, 0.0f), 10.0f, 10.0f, CBullet::BULLETTYPE_PLAYER_SALIVA);
				CBullet::Create(D3DXVECTOR3(pos.x + 10.0f, pos.y, pos.z), D3DXVECTOR3(0, -10.0f, 0.0f), 10.0f, 10.0f, CBullet::BULLETTYPE_PLAYER_SALIVA);
			}
		}
		else if (pInputKeyboard->GetTrigger(DIK_K) == true || pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_B) == true)
		{//SPACEキーを押した場合
			pSound->PlaySound(CSound::SOUND_LABEL_SE_SHIT);
			//ターゲットの生成
			CTargetLogo::Create(D3DXVECTOR3(pos.x, pos.y - 200.0f, 0.0f), 30.0f, 30.0f);
			//弾の生成
			CBullet::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0, -10.0f, 0.0f), 10.0f, 10.0f, CBullet::BULLETTYPE_PLAYER_SHIT);
		}
		break;
	case STATE_FEVER:
		break;
	case STATE_REVIVAL:
		m_nCount++;					//カウント
		if (m_nCount >= 50)
		{//カウントが５０になったら
			m_move.y -= m_speed;

			if (pos.y <= SCREEN_HEIGHT - 100)
			{//指定の位置に到着したら
				m_state = STATE_INVINCIBLE;	//通常状態にする
				m_nCount = 0;
			}
		}
		break;
	case STATE_INVINCIBLE:
		//色の取得
		D3DXCOLOR col;
		col = GetCol();
		//カウンターの加算
		m_nCount++;

		if (col.a == 0.0f)
		{//色が透明の場合
			col.a = 1.0f;
		}
		else if (col.a == 1.0f)
		{//色が透明でない場合
			col.a = 0.0f;
		}
		//色の設置
		SetCol(col);

		if (m_nCount <= 300)
		{
			//範囲外処理
			if (pos.x <= 330.0f)
			{
				pos.x = 330.0f;
			}
			else if (pos.x >= SCREEN_WIDTH - 30.0f)
			{
				pos.x = SCREEN_WIDTH - 30.0f;
			}

			if (pos.y <= 30.0f)
			{
				pos.y = 30.0f;
			}
			else if (pos.y >= SCREEN_HEIGHT - 30.0f)
			{
				pos.y = SCREEN_HEIGHT - 30.0f;
			}

			//位置の設置
			SetVtxPos(pos);
			if (pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_LSTICK_UP) == true ||
				pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_LSTICK_DOWN) == true ||
				pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_LSTICK_LEFT) == true ||
				pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_LSTICK_RIGHT) == true)
			{
				//移動処理
				m_move.x += sinf(pInputJoypad->GetLeftAxiz()) * m_speed;
				m_move.y += cosf(pInputJoypad->GetLeftAxiz()) * m_speed;
			}

			if (pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_UP) == true ||
				pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_DOWN) == true ||
				pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_LEFT) == true ||
				pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_RIGHT) == true)
			{
				//移動処理
				m_move.x += sinf(pInputJoypad->GetRadian()) * m_speed;
				m_move.y -= cosf(pInputJoypad->GetRadian()) * m_speed;
			}

			if (pInputKeyboard->GetPress(DIK_W) == true)
			{//Wキーを押した場合
				m_move.y -= m_speed;
			}
			if (pInputKeyboard->GetPress(DIK_S) == true)
			{//Sキーを押した場合
				m_move.y += m_speed;
			}
			if (pInputKeyboard->GetPress(DIK_A) == true)
			{//Aキーを押した場合
				m_move.x -= m_speed;
			}
			if (pInputKeyboard->GetPress(DIK_D) == true)
			{//Dキーを押した場合
				m_move.x += m_speed;
			}
			if (pInputKeyboard->GetTrigger(DIK_SPACE) == true || pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_A) == true)
			{//SPACEキーを押した場合
				pSound->PlaySound(CSound::SOUND_LABEL_SE_BULLET);
				if (m_bulletType == BULLET_NOMAL)
				{
					//弾の生成
					CBullet::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0, -10.0f, 0.0f), 10.0f, 10.0f, CBullet::BULLETTYPE_PLAYER_SALIVA);
				}
				else if (m_bulletType == BULLET_SKILL)
				{
					CBullet::Create(D3DXVECTOR3(pos.x - 10.0f, pos.y, pos.z), D3DXVECTOR3(0, -10.0f, 0.0f), 10.0f, 10.0f, CBullet::BULLETTYPE_PLAYER_SALIVA);
					CBullet::Create(D3DXVECTOR3(pos.x + 10.0f, pos.y, pos.z), D3DXVECTOR3(0, -10.0f, 0.0f), 10.0f, 10.0f, CBullet::BULLETTYPE_PLAYER_SALIVA);
				}
			}
			else if (pInputKeyboard->GetTrigger(DIK_K) == true || pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_B) == true)
			{//SPACEキーを押した場合
			 //ターゲットの生成
				pSound->PlaySound(CSound::SOUND_LABEL_SE_SHIT);

				CTargetLogo::Create(D3DXVECTOR3(pos.x, pos.y - 200.0f, 0.0f), 30.0f, 30.0f);
				//弾の生成
				CBullet::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0, -10.0f, 0.0f), 10.0f, 10.0f, CBullet::BULLETTYPE_PLAYER_SHIT);
			}
		}
		else if (m_nCount >= 300)
		{
			col.a = 1.0f;				//透明度を戻す
			SetCol(col);				//色の設置
			m_state = STATE_NOMAL;		//通常状態にする
		}
		break;
	}
	CScene2D::Update();
}

//=============================================================================
// プレイヤーの描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	//CScene2Dの描画処理
	CScene2D::Draw();
}

//=============================================================================
// 向きの取得処理
//=============================================================================
float CPlayer::GetRot(void)
{
	return m_rot;	//値を返す
}

//=============================================================================
// 状態の取得処理
//=============================================================================
CPlayer::STATE CPlayer::GetState(void)
{
	return m_state;	//値を返す
}

//=============================================================================
// 状態の設置処理
//=============================================================================
void CPlayer::SetState(CPlayer::STATE state)
{
	m_state = state;
}
//=============================================================================
// 弾の取得処理
//=============================================================================
CPlayer::BULLET_TYPE CPlayer::GetBullet(void)
{
	return m_bulletType;
}
//=============================================================================
// 弾の設置処理
//=============================================================================
void CPlayer::SetBullet(CPlayer::BULLET_TYPE type)
{
	m_bulletType = type;
}
//=============================================================================
// スピードの取得処理
//=============================================================================
void CPlayer::SetSpeed(CPlayer::PLAYER_SPEED speed)
{
	switch (speed)
	{
	case CPlayer::SPEED_NOMAL:
		m_speed = PLAYER_NOMAL_SPEED;
		break;
	case CPlayer::SPEED_SKILL:
		m_speed = PLAYER_SKILL_SPEED;
		break;
	}
}

//=============================================================================
// 移動量の設置処理
//=============================================================================
void CPlayer::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//=============================================================================
// スピードの設置処理
//=============================================================================
float CPlayer::GetSpeed(void)
{
	return m_speed;
}

//=============================================================================
// ダメージ処理
//=============================================================================
void CPlayer::HitDamage(int nDamage)
{
	CLife *pLife = NULL;
	CStock *pStock = NULL;
	CSkill *pSkill = NULL;

	D3DXVECTOR3 pos;			//位置
	pos = GetPos();				//位置の取得

	if (pLife == NULL)
	{
		if (pStock == NULL)
		{
			if (m_state == STATE_NOMAL)
			{//通常状態の場合
				pLife->SetLife(-nDamage);	//ダメージ処理
				if (pLife->GetLife() <= 0)
				{
					//爆発の生成
					CExplotion::Create(pos, 50.0f, 50.0f);
					//終了処理
					Uninit();
					if (pStock->GetStock() >= 1)
					{//残機がまだある場合
						//プレイヤーの生成
						CPlayer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT + 50, 0.0f), PLAYER_SIZE, PLAYER_SIZE, STATE_REVIVAL);
						//残機減算
						pStock->SetStock(-1);
						//ライフを戻す
						pLife->SetLife(3);
						//スキルを戻す
						pSkill->SetSkill(pSkill->SKILL_2WAY, pSkill->SKILL_STATE_NONE);
						pSkill->SetSkill(pSkill->SKILL_SPEED, pSkill->SKILL_STATE_NONE);
						pSkill->SetSkill(pSkill->SKILL_AVATAR, pSkill->SKILL_STATE_NONE);
					}
					else
					{
						//終了する
						SetState(STATE_END);
					}
				}
			}
		}
	}
}

