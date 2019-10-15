//=============================================================================
//
// 氷柱処理 [icicle.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "icicle.h"
#include "sceneX.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPD3DXMESH CIcicle::m_pMesh = NULL;					//メッシュへのポインタ
LPD3DXBUFFER CIcicle::m_pBuffMat = NULL;			//マテリアル情報へのポインタ
DWORD CIcicle::m_nNumMat = NULL;					//マテリアルの数
LPDIRECT3DTEXTURE9 CIcicle::m_pTexture = NULL;		//テクスチャへのポインタ

//=============================================================================
// コンストラクタ
//=============================================================================
CIcicle::CIcicle() : CSceneX(3,OBJTYPE_ICICLE)
{

}
//=============================================================================
// デストラクタ
//=============================================================================
CIcicle::~CIcicle()
{

}

//=============================================================================
// UIベースゲージのテクスチャ読み込み
//=============================================================================
HRESULT CIcicle::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\icicle.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	return S_OK;
}

//=============================================================================
// UIベースゲージのテクスチャ破棄
//=============================================================================
void CIcicle::Unload(void)
{
	// メッシュの開放
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// マテリアルの開放
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

}

//=============================================================================
// 氷柱の生成
//=============================================================================
CIcicle *CIcicle::Create(D3DXVECTOR3 pos, CIcicle::ICICLE_TYPE Type)
{
	CIcicle *pIcicle = NULL;	//氷柱のポインタ

	if (pIcicle == NULL)
	{
		pIcicle = new CIcicle;	//氷柱の動的確保
		pIcicle->Init();		//初期化処理
		pIcicle->m_type = Type;	//種類の代入

		//種類分け
		switch (Type)
		{
		case ICICLE_RIGIHT:		//右の氷柱の場合
			pIcicle->SetPos(D3DXVECTOR3(pos.x - 100.0f,pos.y -80.0f, pos.z - 50.0f));	//位置の設置処理
			pIcicle->SetRot(D3DXVECTOR3(0.0f, 0.0f, -0.5f));							//向きの設置処理
			break;
		case ICICLE_LEFT:		//左の氷柱の場合
			pIcicle->SetPos(D3DXVECTOR3(pos.x + 100.0f, pos.y - 80.0f, pos.z + 50.0f));	//位置の設置処理
			pIcicle->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.5f));								//向きの設置処理
			break;
		}
		pIcicle->BindX(m_pMesh, m_pBuffMat, m_nNumMat);									//モデルの設置処理
	}
	return pIcicle;
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CIcicle::Init(void)
{
	CSceneX::Init();	//オブジェクトXの初期化処理
	m_nCounter = 0;		//カウンターの初期化
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CIcicle::Uninit(void)
{
	//オブジェクトXの終了処理
	CSceneX::Uninit();

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CIcicle::Update(void)
{
	m_nCounter++;	//カウンターの加算

	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	//位置の取得処理
	D3DXVECTOR3 pos;
	pos = GetPos();

	//種類分け
	switch (m_type)
	{
	case ICICLE_RIGIHT:	//右の氷柱の場合
		if (m_nCounter >= 0 && m_nCounter <= 25)
		{
		pos.x += 3;
		pos.y += 3;
		}
		break;
	case ICICLE_LEFT:	//左の氷柱の場合
		if (m_nCounter >= 0 && m_nCounter <= 25)
		{
			pos.x -= 3;
			pos.y += 3;
		}
		break;
	}

	if (m_nCounter >= 140)
	{//カウンターが１４０以上の場合
		//終了処理
		Uninit();
	}

	SetPos(pos);

}

//=============================================================================
// 描画処理
//=============================================================================
void CIcicle::Draw(void)
{
	//オブジェクトXの描画処理
	CSceneX::Draw();

}