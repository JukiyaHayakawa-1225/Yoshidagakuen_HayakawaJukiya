//=============================================================================
//
// 敵の防衛の処理 [defense.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "defense.h"
#include "enemy.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "bullet.h"
#include "input.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CDefense::m_pTexture = NULL;

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// 敵の防衛のコンストラクタ
//=============================================================================
CDefense::CDefense() : CScene2D(2)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//移動量の初期値
}
//=============================================================================
// 敵の防衛のデストラクタ
//=============================================================================
CDefense::~CDefense()
{

}

//=============================================================================
//敵の防衛の生成
//=============================================================================
CDefense *CDefense::Create(D3DXVECTOR3 pos, float fWight, float fHeight)
{
	CDefense *pDefense = NULL;
	if (pDefense == NULL)
	{
		pDefense = new CDefense;			//シーンの動的確保
		pDefense->SetPos(pos);				//位置の代入
		pDefense->SetWidth(fWight);			//幅の代入
		pDefense->SetHeight(fHeight);		//高さの代入
		pDefense->Init();					//初期化処理
		pDefense->BindTexture(m_pTexture);	//テクスチャの貼り付け
	}
	return pDefense;						//値を返す
}

//=============================================================================
// オブジェクトのテクスチャ読み込み
//=============================================================================
HRESULT CDefense::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\barrier.png",
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// オブジェクトのテクスチャ破棄
//=============================================================================
void CDefense::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 敵の防衛の初期化処理
//=============================================================================
HRESULT CDefense::Init(void)
{
	//移動量の初期化処理
	m_move = D3DXVECTOR3(5.0f, 5.0f, 5.0f);
	//カウンタ－の初期化処理
	m_nCounter = 0;
	//CScene2Dの初期化処理
	CScene2D::Init();
	return S_OK;
}

//=============================================================================
// 敵の防衛の終了処理
//=============================================================================
void CDefense::Uninit(void)
{
	//CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 敵の防衛の更新処理
//=============================================================================
void CDefense::Update(void)
{
	//プレイヤーの追尾処理
	FollowPlayer();
	//更新処理
	CScene2D::Update();
}

//=============================================================================
// 敵の防衛の描画処理
//=============================================================================
void CDefense::Draw(void)
{
	//CScene2Dの描画処理
	CScene2D::Draw();
}

//=============================================================================
// プレイヤーの追尾処理
//=============================================================================
void CDefense::FollowPlayer(void)
{
	D3DXVECTOR3 pos;	//位置
	float fAngle;		//角度
	m_nCounter++;
	pos = GetPos();		//位置の取得

	for (int nCntPrioty = 0; nCntPrioty < NUM_PRIORITY; nCntPrioty++)
	{//優先度の数だけ繰り返し
		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{//Sceneの数だけ繰り返し
			CScene2D *pScene;															//シーンのポインタ
			pScene = (CScene2D*)CScene::GetScene(nCntPrioty, nCntScene);				//シーンの取得
			if (pScene != NULL)
			{//SceneがNULLでない場合
				CScene::OBJTYPE objType;												//種類
				objType = pScene->GetObjType();											//種類の取得
				if (objType == OBJTYPE_BOSS)
				{//オブジェクトがプレイヤーの場合
					D3DXVECTOR3 BossPos;												//プレイヤーの位置
					CEnemyBoss *pBoss = (CEnemyBoss*)pScene;							//プレイヤーの動的確保
					BossPos = pBoss->GetPos();											//位置の取得
					fAngle = atan2f(BossPos.x - pos.x, BossPos.y - pos.y);				//向きの取得
					D3DXVECTOR3 SetPos(sinf(fAngle) * 2.5f, cosf(fAngle) * 2.5f, 0.0f);	//位置の情報
					m_move = SetPos;													//プレイヤーの追尾
					if (m_nCounter > 200)
					{
						m_move = D3DXVECTOR3(sinf(D3DX_PI*(m_nCounter - 200) / 160.0f)*3.0f, sinf(D3DX_PI*(m_nCounter - 120) / 160.0f)*3.0f, 0.0f);
					}
					pos += m_move;		//移動量の代入

					//位置の設置
					SetVtxPos(pos);
				}
			}
		}
	}
}