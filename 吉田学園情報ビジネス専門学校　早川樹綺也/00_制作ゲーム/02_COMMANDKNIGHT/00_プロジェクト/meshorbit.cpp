//=============================================================================
//
// メッシュオービットの処理 [meshorbit.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "meshorbit.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "player.h"
#include "command.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME01 "data\\TEXTURE\\orbit.png"			//テクスチャ名

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// ポリゴンのコンストラクタ
//=============================================================================
CMeshOrbit::CMeshOrbit(int nPrioriry, OBJTYPE objtype) : CScene(nPrioriry, objtype)
{
	m_pTexture = NULL;							// テクスチャへのポインタ
	m_pVtxBuff = NULL;							// 頂点バッファへのポインタ
	m_pMtxParent = NULL;						// 親マトリックスへのポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置

}
//=============================================================================
// ポリゴンのコンストラクタ
//=============================================================================
CMeshOrbit::~CMeshOrbit()
{

}
//=============================================================================
// ポリゴンの生成
//=============================================================================
CMeshOrbit *CMeshOrbit::Create(void)
{
	CMeshOrbit *pMeshOrbit = NULL;		//オブジェクトのポインタ

	if (pMeshOrbit == NULL)
	{
		pMeshOrbit = new CMeshOrbit;	//動的確保
		if (pMeshOrbit != NULL)
		{
			pMeshOrbit->Init();		//初期化処理
		}
	}
	return pMeshOrbit;
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMeshOrbit::Init(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//頂点数を計算
	m_nNumVertex = ((MAX_X + 1) * (MAX_Z + 1));

	//ポリゴンの総数を計算
	m_nNumPolygon = ((MAX_X + 1) * (MAX_Z + 1)
		+ (2 * (MAX_Z - 1))
		+ (MAX_X + 1) * (MAX_Z - 1)) - 2;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME01,
		&m_pTexture);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,	// 確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,			// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	int nCntVtx = 0;
	for (int nCntX = 0; nCntX < MAX_X + 1; nCntX++)
	{
		// 頂点座標の設定
		pVtx[nCntX + nCntVtx].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntX + nCntVtx) + 1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//法線の設定
		pVtx[nCntX + nCntVtx].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[(nCntX + nCntVtx) + 1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//色の設定
		pVtx[nCntX + nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[(nCntX + nCntVtx) + 1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		//テクスチャの設定
		pVtx[nCntX + nCntVtx].tex = D3DXVECTOR2(0.0f,0.0f);
		pVtx[(nCntX + nCntVtx) + 1].tex = D3DXVECTOR2(0.0f, 1.0f);
		nCntVtx += MAX_Z;
	}
	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	for (int nCnt = 0; nCnt < MAX_X + 1; nCnt++)
	{
		m_aPosVertex[nCnt] = D3DXVECTOR3(0.0f, 0.0f,0.0f);
		//m_aColVertex[nCnt] = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		//m_aTexVertexX[nCnt] = D3DXVECTOR2(0.0f, 0.0f);
	}

	nCntVtx = 0;
	for (int nCntX = 0; nCntX < MAX_X + 1; nCntX++)
	{
		//色の設定
		m_aColVertex[nCntX + nCntVtx] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_aColVertex[(nCntX + nCntVtx) + 1] = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

		//テクスチャの設定
		m_aTexVertexX[nCntX + nCntVtx] = D3DXVECTOR2(1.0f, 1.0f);
		m_aTexVertexX[(nCntX + nCntVtx) + 1] = D3DXVECTOR2(1.0f, 0.0f);
		nCntVtx += MAX_Z;
	}
	m_aOffset[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_aOffset[1] = D3DXVECTOR3(0.0f, 25.0f, 0.0f);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMeshOrbit::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMeshOrbit::Update(void)
{
	VERTEX_3D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCntVtx = 0;
	for (int nCntX = 2; nCntX < MAX_X + 1; nCntX++)
	{//オービットの数だけ繰り返し
		//色の減算処理
		m_aColVertex[nCntX + nCntVtx].a -= 0.025f;
		m_aColVertex[nCntX + nCntVtx + 1].a -= 0.025f;

		m_aColVertex[nCntX + nCntVtx].a -= 0.015f;
		m_aColVertex[nCntX + nCntVtx + 1].a -= 0.015f;

		//テクスチャの減算処理
		m_aTexVertexX[nCntX + nCntVtx].x -= 0.02f;
		m_aTexVertexX[nCntX + nCntVtx + 1].x -= 0.02f;

		nCntVtx += MAX_Z;
	}
	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void CMeshOrbit::Draw(void)
{
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();

	if (pPlayer->GetMotion() != CPlayer::MOTION_ITEM &&
		pPlayer->GetMotion() != CPlayer::MOTION_JUMP &&
		pPlayer->GetMotion() != CPlayer::MOTION_NEUTRAL &&
		pPlayer->GetMotion() != CPlayer::MOTION_MOVE)
	{
		//レンダリングの取得
		CRenderer *pRenderer;
		pRenderer = CManager::GetRenderer();

		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice;
		pDevice = pRenderer->GetDevice();

		D3DXVECTOR3 pos;							//位置の情報
		D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
		D3DXMATRIX mtxParent;						//親子関係マトリックス

		// カリングをしない状態にする
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		// アルファテストを有効にする
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 200);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		 // αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		VERTEX_3D*pVtx;	//頂点情報へのポインタ

		//頂点バッファをロックし、頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntMeshOrbit = m_nNumVertex - 3; nCntMeshOrbit >= 0; nCntMeshOrbit--)
		{//頂点数分繰り返し
			//現在の情報を２つ先に入れ替え
			m_aPosVertex[nCntMeshOrbit + 2] = m_aPosVertex[nCntMeshOrbit];
			m_aColVertex[nCntMeshOrbit + 2] = m_aColVertex[nCntMeshOrbit];
			m_aTexVertexX[nCntMeshOrbit + 2] = m_aTexVertexX[nCntMeshOrbit];

			//２つ先の情報を頂点に代入
			pVtx[nCntMeshOrbit + 2].pos = m_aPosVertex[nCntMeshOrbit + 2];
			pVtx[nCntMeshOrbit + 2].col = m_aColVertex[nCntMeshOrbit + 2];
			pVtx[nCntMeshOrbit + 2].tex = m_aTexVertexX[nCntMeshOrbit + 2];
		}
		//頂点０番目の情報を代入
		//位置
		D3DXVec3TransformCoord(&m_aPosVertex[0], &m_aOffset[0], m_pMtxParent);
		pVtx[0].pos = m_aPosVertex[0];
		//色
		m_aColVertex[0].a = 1.0f;
		pVtx[0].col = m_aColVertex[0];
		//テクスチャ
		m_aTexVertexX[0] = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[0].tex = m_aTexVertexX[0];

		//頂点１番目の情報を代入
		//位置
		D3DXVec3TransformCoord(&m_aPosVertex[1], &m_aOffset[1], m_pMtxParent);
		pVtx[1].pos = m_aPosVertex[1];
		//色
		m_aColVertex[1].a = 1.0f;
		pVtx[1].col = m_aColVertex[1];
		//テクスチャ
		m_aTexVertexX[1] = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = m_aTexVertexX[1];

		// 頂点バッファをアンロック
		m_pVtxBuff->Unlock();

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		////親モデルのマトリックスを掛け合わせる
		//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, m_pMtxParent);


		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, m_pTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nNumPolygon);

		// αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// アルファテストを無効にする
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		// カリングを元に戻す
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	}
}

//=============================================================================
// マトリックスの設置処理
//=============================================================================
void CMeshOrbit::SetMtxParent(D3DXMATRIX *pMtx)
{
	//親子関係のマトリックスに代入
	m_pMtxParent = pMtx;
}