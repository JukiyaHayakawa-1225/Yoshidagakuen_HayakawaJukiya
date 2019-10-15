//=============================================================================
//
// モデル処理 [model.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
//=============================================================================
// コンストラクタ
//=============================================================================
CModel::CModel()
{
	// 位置・向きの初期設定
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置の初期設定
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向きの初期設定
	m_size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);		//大きさの初期設定
}
//=============================================================================
// デストラクタ
//=============================================================================
CModel::~CModel()
{

}
//=============================================================================
// オブジェクトXの生成
//=============================================================================
CModel *CModel::Create(D3DXVECTOR3 pos)
{
	CModel *pModel = NULL;

	if (pModel == NULL)
	{
		pModel = new CModel;		//動的確保
		if (pModel != NULL)
		{
			pModel->Init();			//初期化処理
			pModel->m_pos = pos;	//位置の代入8
		}
	}
	return pModel;
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModel::Init(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	BindX(m_pMesh, m_pBuffMat, m_pNumMat);
	m_size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CModel::Uninit(void)
{
	////バッファの開放
	//if (m_pBuffMat != NULL)
	//{
	//	m_pBuffMat->Release();
	//	m_pBuffMat = NULL;
	//}
	//// メッシュの開放
	//if (m_pMesh != NULL)
	//{
	//	m_pMesh->Release();
	//	m_pMesh = NULL;
	//}
}

//=============================================================================
// 更新処理
//=============================================================================
void CModel::Update(void)
{
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CModel::Draw(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;					//計算用マトリックス
	D3DMATERIAL9 matDef;							//現在のマテリアル保存用
	D3DXMATERIAL *pMat;								//マテリアルデータへのポインタ
	D3DXMATRIX mtxParent;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 大きさの反映
	D3DXMatrixScaling(&mtxScale,
		m_size.x, m_size.y, m_size.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pParent != NULL)
	{//親がいる場合
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{//親がいない場合
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}
	//親モデルのマトリックスを掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャ無効
		pDevice->SetTexture(0, NULL);

		// モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}


	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// 位置の設置処理
//=============================================================================
void CModel::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// 位置の設置処理
//=============================================================================
void CModel::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
//    モデルのメッシュ情報を設定する
//=============================================================================
void CModel::SetMesh(LPD3DXMESH pMesh)
{
	m_pMesh = pMesh;

	if (m_pMesh != NULL)
	{// モデルが読み込めた
	 // 最小値と最大値を初期化
		m_pVtxMin = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
		m_pVtxMax = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);

		int nNumVtx;     // 頂点数
		DWORD sizeFVF;   // 頂点フォーマットのサイズ
		BYTE *pVtxBuff;  // 頂点バッファへのポインタ

		 // 頂点数を獲得
		nNumVtx = m_pMesh->GetNumVertices();

		// 頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

		// 頂点バッファをロック
		m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{//
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;   // 頂点座標の代入

			if (vtx.x < m_pVtxMin.x)
			{
				m_pVtxMin.x = vtx.x;  // 最小値更新
			}
			if (vtx.y < m_pVtxMin.y)
			{
				m_pVtxMin.y = vtx.y;  // 最小値更新
			}
			if (vtx.z < m_pVtxMin.z)
			{
				m_pVtxMin.z = vtx.z;  // 最小値更新
			}
			if (vtx.x > m_pVtxMax.x)
			{
				m_pVtxMax.x = vtx.x;  // 最大値更新
			}
			if (vtx.y > m_pVtxMax.y)
			{
				m_pVtxMax.y = vtx.y;  // 最大値更新
			}
			if (vtx.z > m_pVtxMax.z)
			{
				m_pVtxMax.z = vtx.z;  // 最大値更新
			}

			pVtxBuff += sizeFVF;  // サイズ分ポインタを進める
		}
		// 頂点バッファをアンロック
		m_pMesh->UnlockVertexBuffer();
	}
}

//=============================================================================
//    モデルのマテリアル情報を設定する
//=============================================================================
void CModel::SetBuffMat(LPD3DXBUFFER buffMat)
{
	m_pBuffMat = buffMat;
}

//=============================================================================
//    モデルのマテリアルの数情報を設定する
//=============================================================================
void CModel::SetNumMat(DWORD nNumMat)
{
	m_pNumMat = nNumMat;
}

//=============================================================================
// 位置の取得処理
//=============================================================================
D3DXVECTOR3 CModel::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// 向きの取得処理
//=============================================================================
D3DXVECTOR3 CModel::GetRot(void)
{
	return m_rot;
}

//=============================================================================
// Xファイルの取得処理
//=============================================================================
void CModel::BindX(LPD3DXMESH mesh, LPD3DXBUFFER buffmat, DWORD nummat)
{
	SetMesh(mesh);
	SetBuffMat(buffmat);
	SetNumMat(nummat);
	/*m_pMesh = mesh;
	m_pBuffMat = buffmat;
	m_pNumMat = nummat;*/
}

//=============================================================================
// 親の設定
//=============================================================================
void CModel::SetParent(CModel *pModel)
{
	m_pParent = pModel;
}

//=============================================================================
// ワールドマトリックスの取得
//=============================================================================
D3DXMATRIX &CModel::GetMtxWorld(void)
{
	return m_mtxWorld;
}

//=============================================================================
// 大きさの設置取得
//=============================================================================
void CModel::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}