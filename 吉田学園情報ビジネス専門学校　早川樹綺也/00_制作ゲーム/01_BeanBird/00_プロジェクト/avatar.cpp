//=============================================================================
//
// 分身の処理 [avatar.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "avatar.h"
#include "player.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "bullet.h"
#include "input.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CAvatar::m_pTexture = NULL;			//テクスチャのポインタ
CAvatar::AVATAR_STATE CAvatar::m_state = AVATAR_NONE;	//分身の情報

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// 分身のコンストラクタ
//=============================================================================
CAvatar::CAvatar() : CScene2D(3)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//移動量の初期値
}
//=============================================================================
// 分身のデストラクタ
//=============================================================================
CAvatar::~CAvatar()
{

}

//=============================================================================
//分身の生成
//=============================================================================
CAvatar *CAvatar::Create(D3DXVECTOR3 pos, float fWight, float fHeight)
{
	CAvatar *pAvatar = NULL;
	if (pAvatar == NULL)
	{
		pAvatar = new CAvatar;				//シーンの動的確保
		pAvatar->SetPos(pos);				//位置の代入
		pAvatar->SetWidth(fWight);			//幅の代入
		pAvatar->SetHeight(fHeight);		//高さの代入
		pAvatar->Init();					//初期化処理
		pAvatar->m_state = AVATAR_USE;		//使用している状態にする
		pAvatar->BindTexture(m_pTexture);	//テクスチャの貼り付け
	}
	return pAvatar;						//値を返す
}

//=============================================================================
// オブジェクトのテクスチャ読み込み
//=============================================================================
HRESULT CAvatar::Load(void)
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
// オブジェクトのテクスチャ破棄
//=============================================================================
void CAvatar::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 分身の初期化処理
//=============================================================================
HRESULT CAvatar::Init(void)
{
	//移動量の初期化処理
	m_move = D3DXVECTOR3(5.0f, 5.0f, 5.0f);

	//CScene2Dの初期化処理
	CScene2D::Init();
	return S_OK;
}

//=============================================================================
// 分身の終了処理
//=============================================================================
void CAvatar::Uninit(void)
{
	m_state = AVATAR_NONE;

	//CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 分身の更新処理
//=============================================================================
void CAvatar::Update(void)
{
	//プレイヤーの追尾処理
	FollowPlayer();
	//更新処理
	CScene2D::Update();
}

//=============================================================================
// 分身の描画処理
//=============================================================================
void CAvatar::Draw(void)
{
	//CScene2Dの描画処理
	CScene2D::Draw();
}

//=============================================================================
// プレイヤーの追尾処理
//=============================================================================
void CAvatar::FollowPlayer(void)
{
	D3DXVECTOR3 pos;	//位置
	float fAngle;		//角度

	pos = GetPos();		//位置の取得

	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	//ジョイパッドの取得
	CInputJoypad *pInputJoypad;
	pInputJoypad = CManager::GetJoypad();

	if (m_state == AVATAR_USE)
	{//分身を使用している場合
		for (int nCntPrioty = 0; nCntPrioty < NUM_PRIORITY; nCntPrioty++)
		{//優先度の数だけ繰り返し
			for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
			{//Sceneの数だけ繰り返し
				CScene2D *pScene;												//シーンのポインタ
				pScene = (CScene2D*)CScene::GetScene(nCntPrioty, nCntScene);	//シーンの取得
				if (pScene != NULL)
				{//SceneがNULLでない場合
					CScene::OBJTYPE objType;					//種類
					objType = pScene->GetObjType();				//種類の取得
					if (objType == OBJTYPE_PLAYER)
					{//オブジェクトがプレイヤーの場合
						D3DXVECTOR3 PlayerPos;					//プレイヤーの位置
						float speed;							//スピード
						CPlayer *pPlayer = (CPlayer*)pScene;	//プレイヤーの動的確保
						speed = pPlayer->GetSpeed();			//スピードの取得
						PlayerPos = pPlayer->GetPos();			//位置の取得
						fAngle = atan2f(PlayerPos.x - pos.x, PlayerPos.y - pos.y);								//向きの取得
						D3DXVECTOR3 SetPos(sinf(fAngle) * (2.5f + speed), cosf(fAngle) * (2.5f + speed), 0.0f);	//位置の情報
						if (pPlayer != NULL)
						{
							m_move = SetPos;	//プレイヤーの追尾
							pos += m_move;		//移動量の代入
						}
						if (pInputKeyboard->GetTrigger(DIK_SPACE) == true || pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_A) == true)
						{//SPACEキーを押した場合
							if (pPlayer->GetBullet() == pPlayer->BULLET_NOMAL)
							{//プレイヤーの弾が通常状態の場合
							 //弾の生成
								CBullet::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0, -10.0f, 0.0f), 10.0f, 10.0f, CBullet::BULLETTYPE_PLAYER_SALIVA);
							}
							else if (pPlayer->GetBullet() == pPlayer->BULLET_SKILL)
							{//プレイヤーの弾がスキル状態の場合
							 //弾の生成
								CBullet::Create(D3DXVECTOR3(pos.x - 10.0f, pos.y, pos.z), D3DXVECTOR3(0, -10.0f, 0.0f), 10.0f, 10.0f, CBullet::BULLETTYPE_PLAYER_SALIVA);
								CBullet::Create(D3DXVECTOR3(pos.x + 10.0f, pos.y, pos.z), D3DXVECTOR3(0, -10.0f, 0.0f), 10.0f, 10.0f, CBullet::BULLETTYPE_PLAYER_SALIVA);
							}
						}

						//位置の設置
						SetVtxPos(pos);

						if (pPlayer->GetState() == pPlayer->STATE_REVIVAL)
						{//プレイヤーが死んだ場合
						 //分身を使用していない状態にする
							m_state = AVATAR_NONE;
							//分身の終了処理
							CAvatar::Uninit();
						}
					}
				}
			}
		}
	}
}
//=============================================================================
// 分身の取得処理
//=============================================================================
CAvatar::AVATAR_STATE CAvatar::GetAvatar(void)
{
	return m_state;
}

//=============================================================================
// 分身の設置処理
//=============================================================================
void CAvatar::SetAvatar(AVATAR_STATE state)
{
	m_state = state;
}