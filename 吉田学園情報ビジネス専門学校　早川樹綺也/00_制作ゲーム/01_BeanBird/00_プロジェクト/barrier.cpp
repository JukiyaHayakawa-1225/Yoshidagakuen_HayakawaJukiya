//=============================================================================
//
// バリアの処理 [barrier.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "barrier.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "skill.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CBarrier::m_pTexture = NULL;			//テクスチャのポインタ
CBarrier::BARRIER_STATE CBarrier::m_state = STATE_NONE;	//バリアの状態

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// オブジェクトのコンストラクタ
//=============================================================================
CBarrier::CBarrier() : CScene2D(6)
{
	m_timer = 0;		//タイマーの初期値
}
//=============================================================================
// オブジェクトのデストラクタ
//=============================================================================
CBarrier::~CBarrier()
{
}

//=============================================================================
//オブジェクトの生成
//=============================================================================
CBarrier *CBarrier::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWight, float fHeight)
{
	CBarrier *pBarrier = NULL;										//バリアのポインタ
	if (pBarrier == NULL)
	{//NULLの場合
		pBarrier = new CBarrier;									//バリアの動的確保
		if (pBarrier != NULL)
		{//NULLでない場合
			pBarrier->SetPos(pos);									//位置の設置
			pBarrier->SetWidth(fWight);								//幅の設置
			pBarrier->SetHeight(fHeight);							//高さの設置
			pBarrier->Init();										//初期化処理
			pBarrier->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));	//色の設置
			pBarrier->BindTexture(m_pTexture);						//テクスチャ
		}
	}
	return pBarrier;						//値を返す
}

//=============================================================================
// オブジェクトのテクスチャ読み込み
//=============================================================================
HRESULT CBarrier::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME015,
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// オブジェクトのテクスチャ破棄
//=============================================================================
void CBarrier::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
//=============================================================================
// オブジェクトの初期化処理
//=============================================================================
HRESULT CBarrier::Init(void)
{
	if (m_state == STATE_NONE)
	{//バリアが使われてない場合
		//CScene2Dの初期化処理
		CScene2D::Init();
		//種類の設定
		SetObjType(OBJTYPE_BARRIER);
		//使用している状態にする
		m_state = STATE_USE;
		//タイマーの初期化
		m_timer = 0;
	}
	return S_OK;
}

//=============================================================================
// オブジェクトの終了処理
//=============================================================================
void CBarrier::Uninit(void)
{
	//バリアを使用していない状態にする
	m_state = STATE_NONE;
	//CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// オブジェクトの更新処理
//=============================================================================
void CBarrier::Update(void)
{
	//プレイヤーの追尾処理
	FollowPlayer();

	//CScene2Dの更新処理
	CScene2D::Update();
}

//=============================================================================
// オブジェクトの描画処理
//=============================================================================
void CBarrier::Draw(void)
{
	//CScene2Dの描画処理
	CScene2D::Draw();
}

//=============================================================================
// プレイヤーの追尾処理
//=============================================================================
void CBarrier::FollowPlayer(void)
{
	if (m_state == STATE_USE)
	{//バリアを使用している場合
	 //タイマーの加算
		m_timer++;
		for (int nCntPrioty = 0; nCntPrioty < NUM_PRIORITY; nCntPrioty++)
		{//優先度の数だけ繰り返し
			for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
			{//Sceneの4番だけ繰り返し
				CScene2D *pScene;												//シーンのポインタ
				pScene = (CScene2D*)CScene::GetScene(nCntPrioty, nCntScene);	//シーンの取得
				if (pScene != NULL)
				{//SceneがNULLでない場合
					CScene::OBJTYPE objType;		//種類
					objType = pScene->GetObjType();	//種類の取得
					if (objType == OBJTYPE_PLAYER)
					{//プレイヤーの場合
						CSkill *pSkill = NULL;					//スキルのポインタ
						D3DXVECTOR3 PlayerPos;					//プレイヤーの位置
						CPlayer *pPlayer = (CPlayer*)pScene;	//プレイヤーの動的確保
						PlayerPos = pPlayer->GetPos();			//位置の取得
						if (pPlayer != NULL)
						{//プレイヤーがNULLでない場合
						 //位置の設置
							SetVtxPos(PlayerPos);
							if (m_timer == 1500)
							{//タイマーが1500に達した場合
								m_state = STATE_NONE;			//バリアを使用していない状態にする
								Uninit();						//終了処理
								//スキルを使用していない状態にする
								pSkill->SetSkill(pSkill->SKILL_BARRIER, pSkill->SKILL_STATE_NONE);
							}
						}
					}
				}
			}
		}
	}
}

//=============================================================================
// オブジェクトの描画処理
//=============================================================================
CBarrier::BARRIER_STATE CBarrier::GetState(void)
{
	return m_state;
}

//=============================================================================
// 状態の設置処理
//=============================================================================
void CBarrier::SetState(CBarrier::BARRIER_STATE state)
{
	m_state = state;
}