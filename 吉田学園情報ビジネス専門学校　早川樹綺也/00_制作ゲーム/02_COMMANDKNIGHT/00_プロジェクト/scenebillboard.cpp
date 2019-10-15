//=============================================================================
//
// ビルボードの処理 [billboard.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "scenebillboard.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CSceneBillboard::CSceneBillboard(int nPriority, OBJTYPE objtype) : CScene(nPriority, objtype)
{
	m_pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);		//位置の初期値
	m_fWidth = 10.0f;							//幅の初期値
	m_fHeight = 10.0f;							//高さの初期値
	m_fLength = 10.0f;							//長さの初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量を初期化
}

//=============================================================================
// デストラクタ
//=============================================================================
CSceneBillboard::~CSceneBillboard()
{

}

//=============================================================================
// ビルボードの生成
//=============================================================================
CSceneBillboard *CSceneBillboard::Create(void)
{
	CSceneBillboard *pBillboard = NULL;	//ポインタ

	if (pBillboard == NULL)
	{//NULLの場合
		pBillboard = new CSceneBillboard;	//動的確保
		if (pBillboard != NULL)
		{//NULLでない場合
			pBillboard->Init();			//初期化処理
		}
	}
	return pBillboard;
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSceneBillboard::Init(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,	// 確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,			// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, -m_fHeight, 0.0f);

	//法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// 頂点カラー
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSceneBillboard::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	//データの開放
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSceneBillboard::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CSceneBillboard::Draw(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxView;	//計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW,
		&mtxView);

	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x, m_pos.y, m_pos.z);

	//マトリックスを掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//レンダーステートの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			//
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);						//基準値
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//レンダーステートを元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//=============================================================================
// テクスチャの取得処理
//=============================================================================
void CSceneBillboard::BindTexture(LPDIRECT3DTEXTURE9 texture)
{
	m_pTexture = texture;
}

//=============================================================================
// 位置の設置処理
//=============================================================================
void CSceneBillboard::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// 幅の設置処理
//=============================================================================
void CSceneBillboard::SetWidth(float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
// 高さの設置処理
//=============================================================================
void CSceneBillboard::SetHeight(float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
// 向きの取得処理
//=============================================================================
void CSceneBillboard::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
// 色の設置処理
//=============================================================================
void CSceneBillboard::SetCol(D3DXCOLOR col)
{
	m_col = col;

	VERTEX_3D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 長さの設置処理
//=============================================================================
void CSceneBillboard::SetLength(float fLength)
{
	m_fLength = fLength;
}

//=============================================================================
// 移動量設置処理
//=============================================================================
void CSceneBillboard::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//=============================================================================
// 長さの設置処理
//=============================================================================
void CSceneBillboard::SetLengthPos(void)
{
	VERTEX_3D*pVtx;	//頂点情報へのポインタ

					//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fLength, m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fLength, m_fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fLength, -m_fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fLength, -m_fLength, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}

//=============================================================================
// 位置の取得処理
//=============================================================================
D3DXVECTOR3 CSceneBillboard::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// 向きの取得処理
//=============================================================================
D3DXVECTOR3 CSceneBillboard::GetRot(void)
{
	return m_rot;
}

//=============================================================================
// 長さの取得処理
//=============================================================================
float CSceneBillboard::GetLength(void)
{
	return m_fLength;
}

//=============================================================================
// 色の取得処理
//=============================================================================
D3DXCOLOR CSceneBillboard::GetCol(void)
{
	return m_col;
}

//=============================================================================
// 幅の取得処理
//=============================================================================
float CSceneBillboard::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
// 高さの取得処理
//=============================================================================
float CSceneBillboard::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
// サイド選択型減算処理
//=============================================================================
void CSceneBillboard::CutSide(int nType)
{
	VERTEX_3D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (nType == 0)
	{//左から減算
	 // 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_fWidth, -m_fHeight, 0.0f);
	}
	else if (nType == 1)
	{//右から減算
	 // 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);

	}
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 移動量取得処理
//=============================================================================
D3DXVECTOR3 CSceneBillboard::GetMove(void)
{
	return m_move;
}