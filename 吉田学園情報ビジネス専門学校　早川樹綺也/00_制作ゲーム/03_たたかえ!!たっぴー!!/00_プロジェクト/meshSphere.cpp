//*****************************************************************************
//
//     球の処理[meshSphere.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "meshSphere.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************


//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CMeshSphere::CMeshSphere(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_pVtxBuff = NULL;                          // 頂点バッファへのポインタ
	m_pVtxBuffCover[0] = NULL;                  // 頂点バッファへのポインタ(蓋用)
	m_pVtxBuffCover[1] = NULL;                  // 頂点バッファへのポインタ(蓋用)
	m_pIdxBuff = NULL;                          // インデックスバッファへのポインタ
	m_pTexture = NULL;	                        // テクスチャへのポインタ
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // メッシュスフィアの座標
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // メッシュスフィアの向き
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // メッシュスフィアの色
	m_fRadius = 0.0f;                           // メッシュスフィアの半径
	m_nXBlock = 0;                              // 横の分割数
	m_nYBlock = 0;                              // 奥行の分割数
	m_nNumVertex = 0;                           // 頂点数
	m_nNumIndex = 0;                            // インデックス数
	m_nNumPolygon = 0;                          // ポリゴン数
	D3DXMatrixIdentity(&m_MtxWorld);            // ワールドマトリックス
}

//=============================================================================
//    デストラクタ
//=============================================================================
CMeshSphere::~CMeshSphere()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CMeshSphere *CMeshSphere::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, int nPriority)
{
	CMeshSphere *pMeshSphere = NULL;       // メッシュスフィアクラス型のポインタ
	if (pMeshSphere == NULL)
	{// メモリが空になっている
		pMeshSphere = new CMeshSphere(nPriority);
		if (pMeshSphere != NULL)
		{// インスタンスを生成できた
		    // 各種値の代入
			pMeshSphere->SetPos(pos);         // メッシュスフィアの座標
			pMeshSphere->SetRot(rot);         // メッシュスフィアの向き
			pMeshSphere->SetCol(col);         // メッシュスフィアの色
			pMeshSphere->SetRadius(fRadius);  // メッシュスフィアの半径
			pMeshSphere->SetXBlock(nXBlock);  // 横の分割数
			pMeshSphere->SetYBlock(nYBlock);  // 縦の分割数

			if (FAILED(pMeshSphere->Init()))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}

	return pMeshSphere;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CMeshSphere::Init(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // 作成に必要な頂点数,インデックス数,ポリゴン数を計算
			m_nNumVertex = (m_nXBlock + 1) * (m_nYBlock + 1);                                                               // 頂点数を計算
			m_nNumIndex = ((m_nXBlock + 1) * (m_nYBlock + 1)) + (2 * (m_nYBlock - 1)) + (m_nXBlock + 1) * (m_nYBlock - 1);  // インデックス数を設定
			m_nNumPolygon = ((m_nXBlock * m_nYBlock) * 2) + ((m_nYBlock - 1) * 4);                                          // 描画するポリゴン数を設定

			// 頂点バッファの生成
			MakeVertex(pDevice);

			// インデックスバッファの生成
			MakeIndex(pDevice);
		}
		else
		{// デバイスを取得できなかった
			return E_FAIL;
		}
	}
	else
	{// レンダリングクラスが生成されていない
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CMeshSphere::Uninit(void)
{
	// 頂点バッファの破棄
	DIRECT_RELEASE(m_pVtxBuff);
	DIRECT_RELEASE(m_pVtxBuffCover[0]);
	DIRECT_RELEASE(m_pVtxBuffCover[1]);

	// インデックスバッファの破棄
	DIRECT_RELEASE(m_pIdxBuff);

	// リリース処理
	CScene::Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CMeshSphere::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CMeshSphere::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	 // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // ワールドマトリックスの設定処理
			SetMtxWorld(pDevice);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

			// インデックスバッファをデバイスのデータストリームにバインド
			pDevice->SetIndices(m_pIdxBuff);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, m_pTexture);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);

			// 上側の蓋の描画
			pDevice->SetStreamSource(0, m_pVtxBuffCover[0], 0, sizeof(VERTEX_3D));
			pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, m_nXBlock);

			// 下側の蓋の描画
			pDevice->SetStreamSource(0, m_pVtxBuffCover[1], 0, sizeof(VERTEX_3D));
			pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, m_nXBlock);
		}
	}
}

//=============================================================================
//    ワールドマトリックス計算処理
//=============================================================================
void CMeshSphere::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// 回転行列を作成(D3DXMatrixRotationYawPitchRoll参照)
	float fSinPitch = sinf(m_Rot.x);
	float fCosPitch = cosf(m_Rot.x);
	float fSinYaw = sinf(m_Rot.y);
	float fCosYaw = cosf(m_Rot.y);
	float fSinRoll = sinf(m_Rot.z);
	float fCosRoll = cosf(m_Rot.z);
	mtxRot._11 = fSinRoll * fSinPitch * fSinYaw + fCosRoll * fCosYaw;
	mtxRot._12 = fSinRoll * fCosPitch;
	mtxRot._13 = fSinRoll * fSinPitch * fCosYaw - fCosRoll * fSinYaw;
	mtxRot._21 = fCosRoll * fSinPitch * fSinYaw - fSinRoll * fCosYaw;
	mtxRot._22 = fCosRoll * fCosPitch;
	mtxRot._23 = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fSinYaw;
	mtxRot._31 = fCosPitch * fSinYaw;
	mtxRot._32 = -fSinPitch;
	mtxRot._33 = fCosPitch * fCosYaw;

	// 回転を反映する
	m_MtxWorld._11 = mtxRot._11;
	m_MtxWorld._12 = mtxRot._12;
	m_MtxWorld._13 = mtxRot._13;
	m_MtxWorld._21 = mtxRot._21;
	m_MtxWorld._22 = mtxRot._22;
	m_MtxWorld._23 = mtxRot._23;
	m_MtxWorld._31 = mtxRot._31;
	m_MtxWorld._32 = mtxRot._32;
	m_MtxWorld._33 = mtxRot._33;

	// オフセット位置を反映
	m_MtxWorld._41 = m_Pos.x;
	m_MtxWorld._42 = m_Pos.y;
	m_MtxWorld._43 = m_Pos.z;

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CMeshSphere::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (m_nXBlock + 2),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffCover[0],
		NULL);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (m_nXBlock + 2),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffCover[1],
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fAngle = 0;     // 角度
	float fRadius = 0;    // 半径
	float fSphere = 0.0f; // スフィア計算用(半径を縮める倍率)
	D3DXVECTOR3 pos;      // 座標
	float Utex;           // テクスチャU座標
	float Vtex;           // テクスチャV座標

	fAngle = 0.0f;                         // 角度は0に
	fSphere = -D3DX_PI * 0.5f;             // 倍率を設定
	fRadius = cosf(fSphere) * m_fRadius;   // 半径を設定
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 座標を設定
	Utex = 0.0f;                           // テクスチャU座標を設定
	Vtex = -1.0f;                          // テクスチャV座標を設定

	for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
	{// 垂直方向の分割数だけ繰り返し
		for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
		{// 水平方向の分割数だけ繰り返し
			pos.x = sinf(fAngle) * fRadius;
			pos.y = sinf(fSphere) * m_fRadius;
			pos.z = cosf(fAngle) * fRadius;

			// 頂点座標
			pVtx[nCntH].pos = pos;

			// 法線ベクトル
			pVtx[nCntH].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);

			// 頂点カラー
			pVtx[nCntH].col = m_Col;

			// テクスチャ座標
			pVtx[nCntH].tex = D3DXVECTOR2(Utex, Vtex);

			fAngle -= D3DX_PI / (m_nXBlock / 2);  // 角度を進める
			Utex += 1.0f / (m_nXBlock / 2);       // テクスチャU座標を進める

			if (fAngle <= -D3DX_PI)
			{// 倍率が円周率を超えた
				fAngle += D3DX_PI * 2.0f;
			}
		}
		fSphere += D3DX_PI / (m_nYBlock + 1);    // 半径を縮める倍率を上げる
		fRadius = cosf(fSphere) * m_fRadius;     // 半径をコサインカーブで縮めていく
		fAngle = 0.0f;                           // 角度は0に
		Utex = 0.0f;                             // テクスチャU座標を設定
		Vtex -= 1.0f / (m_nYBlock);              // テクスチャV座標を進める
		pVtx += m_nXBlock + 1;                   // 頂点の先頭の番号を進める
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// 上側の蓋用の頂点情報を作成
	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuffCover[0]->Lock(0, 0, (void**)&pVtx, 0);

	fSphere = D3DX_PI * 0.5f;   // 半径を縮める倍率を上げる
	// 頂点座標
	pos = D3DXVECTOR3(0.0f, sinf(fSphere) * m_fRadius, 0.0f);
	pVtx[0].pos = pos;

	// 法線ベクトル
	pVtx[0].nor = D3DXVECTOR3(-0.22f, 0.87f, -2.2f);

	// 頂点カラー
	pVtx[0].col = m_Col;

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, -0.99f);
	pVtx++;

	fSphere -= D3DX_PI / (m_nYBlock + 1);  // 半径を縮める倍率を戻しておく
	fRadius = cosf(fSphere) * m_fRadius;   // 半径計算を戻しておく
	for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
	{// 水平方向の分割数だけ繰り返し
		pos.x = sinf(fAngle) * fRadius;
		pos.y = sinf(fSphere) * m_fRadius;
		pos.z = cosf(fAngle) * fRadius;

		// 頂点座標
		pVtx[nCntH].pos = pos;

		// 法線ベクトル
		pVtx[nCntH].nor = D3DXVECTOR3(-0.22f, 0.87f, -2.2f);

		// 頂点カラー
		pVtx[nCntH].col = m_Col;

		// テクスチャ座標
		pVtx[nCntH].tex = D3DXVECTOR2(0.0f, -0.98f);

		fAngle += -D3DX_PI / (m_nXBlock / 2);  // 角度を進める
		Utex -= 1.0f / (m_nXBlock / 2);        // テクスチャU座標を進める

		if (fAngle >= -D3DX_PI)
		{// 倍率が円周率を超えた
			fAngle -= D3DX_PI * 2.0f;
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuffCover[0]->Unlock();


	// 下側側の蓋用の頂点情報を作成
	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuffCover[1]->Lock(0, 0, (void**)&pVtx, 0);
	fSphere = -D3DX_PI * 0.5f;   // 半径を縮める倍率を上げる
	// 頂点座標
	pos = D3DXVECTOR3(0.0f, sinf(fSphere) * m_fRadius, 0.0f);
	pVtx[0].pos = pos;

	// 法線ベクトル
	pVtx[0].nor = D3DXVECTOR3(-0.22f, 0.87f, -2.2f);

	// 頂点カラー
	pVtx[0].col = m_Col;

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.99f);
	pVtx++;

	fSphere += D3DX_PI / (m_nYBlock + 1);  // 半径を縮める倍率を戻しておく
	fRadius = cosf(fSphere) * m_fRadius;   // 半径計算を戻しておく
	for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
	{// 水平方向の分割数だけ繰り返し
		pos.x = sinf(fAngle) * fRadius;
		pos.y = sinf(fSphere) * m_fRadius;
		pos.z = cosf(fAngle) * fRadius;

		// 頂点座標
		pVtx[nCntH].pos = pos;

		// 法線ベクトル
		pVtx[nCntH].nor = D3DXVECTOR3(-0.22f, 0.87f, -2.2f);

		// 頂点カラー
		pVtx[nCntH].col = m_Col;

		// テクスチャ座標
		pVtx[nCntH].tex = D3DXVECTOR2(0.0f, 0.98f);

		fAngle += -D3DX_PI / (m_nXBlock / 2);  // 角度を進める
		Utex -= 1.0f / (m_nXBlock / 2);        // テクスチャU座標を進める

		if (fAngle <= -D3DX_PI)
		{// 倍率が円周率を超えた
			fAngle += D3DX_PI * 2.0f;
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuffCover[1]->Unlock();

	// 法線計算処理
	MakeNormal();
}

//=============================================================================
//    インデックスバッファ生成処理
//=============================================================================
void CMeshSphere::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	WORD *pIdx;       // インデックスデータへのポインタ
	int nCntIdx = 0;  // インデックス番号

	// インデックスバッファをロックし,インデックスデータへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdxY = 0; nCntIdxY < m_nYBlock; nCntIdxY++)
	{// 縦の分割数分繰り返し
		for (int nCntIdxX = 0; nCntIdxX < m_nXBlock + 1; nCntIdxX++, nCntIdx++)
		{// 横の分割数+１繰り返し
		    // 上下の二つ分インデックス番号を設定
			pIdx[0] = nCntIdx + m_nXBlock + 1; // 下側
			pIdx[1] = nCntIdx;                 // 上側

			pIdx += 2;  // 2つ分進める
			if (nCntIdxY < m_nYBlock - 1 && nCntIdxX == m_nXBlock)
			{// 1 , 下側が縦の分割数の最下層ではない
			 // 2 , 横の分割数分設定が終わった
				pIdx[0] = nCntIdx;                       // 上側
				pIdx[1] = nCntIdx + (m_nXBlock + 1) + 1; // 次の下側

				pIdx += 2; // 2つ分進める
			}
		}
	}

	// インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();
}

//=============================================================================
//    法線計算処理
//=============================================================================
void CMeshSphere::MakeNormal(void)
{
	VERTEX_3D *pVtx;	                              // 頂点データへのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();  // 頂点バッファ

	D3DXVECTOR3 VecVer[3];	    // 法線計算用ベクトル
	D3DXVECTOR3 *pNor = NULL;   // 頂点の法線

	// 必要な数だけ動的に確保する
	pNor = new D3DXVECTOR3[(m_nXBlock * m_nYBlock) * 2];

	// 外積用にベクトルを作る
	if (pNor != NULL)
	{	// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		int nCntVec1 = 0;                // 番号計算用(左上の頂点番号)
		int nCntVec2 = (m_nXBlock + 1);  // 番号計算用(左下の頂点番号)
		int nCntVec3 = (m_nXBlock + 2);  // 番号計算用(右下の頂点番号)
		int nCntVec4 = 1;                // 番号計算用(右上の頂点番号)
		int nCntNor = 0;                 // 番号計算用(法線の番号)
		for (int nCntV = 0; nCntV < m_nYBlock; nCntV++)
		{// 垂直方向の分割数だけ繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock; nCntH++)
			{// 水平方向の分割数だけ繰り返し
				VecVer[0] = pVtx[nCntVec2].pos - pVtx[nCntVec1].pos;
				VecVer[1] = pVtx[nCntVec3].pos - pVtx[nCntVec1].pos;
				VecVer[2] = pVtx[nCntVec4].pos - pVtx[nCntVec1].pos;

				// 外積を使ってベクトルにする
				D3DXVec3Cross(&pNor[nCntNor], &VecVer[1], &VecVer[0]);
				D3DXVec3Cross(&pNor[nCntNor + 1], &VecVer[2], &VecVer[1]);

				// 出てきたベクトルを正規化して法線にする
				D3DXVec3Normalize(&pNor[nCntNor], &pNor[nCntNor]);
				D3DXVec3Normalize(&pNor[nCntNor + 1], &pNor[nCntNor + 1]);

				// 番号をずらす
				nCntNor += 2;
				nCntVec1++;
				nCntVec2++;
				nCntVec3++;
				nCntVec4++;
			}
			// 右端はもう終わっているので左端になるように番号をそろえる
			nCntVec1++;
			nCntVec2++;
			nCntVec3++;
			nCntVec4++;
		}

		// 法線を代入
		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// 垂直方向の分割数 + 1だけ繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// 水平方向の分割数 + 1だけ繰り返し
				if (nCntH == 0)
				{// 右端だった場合
					if (nCntV == 0)
					{// 一番左上の頂点番号の場合
						pVtx[nCntH].nor = (pNor[0] + pNor[1]) / 2;  // OK
					}
					else if (nCntV == m_nYBlock)
					{// 一番左下の頂点番号の場合
						pVtx[nCntH].nor = pNor[((m_nXBlock * 2) * (m_nYBlock - 1))];  // OK
					}
					else
					{// それ以外の頂点番号の場合
						pVtx[nCntH].nor = (pNor[((nCntV - 1) * (m_nXBlock * 2))] + pNor[nCntV * (m_nXBlock * 2)] + pNor[nCntV * (m_nXBlock * 2) + 1]) / 3;   // OK
					}
				}
				else if (nCntH == m_nXBlock)
				{// 右端だった場合
					if (nCntV == 0)
					{// 一番右上の頂点番号の場合
						pVtx[nCntH].nor = pNor[(m_nXBlock * 2) - 1];  // OK
					}
					else if (nCntV == m_nYBlock)
					{// 一番右下の頂点番号の場合
						pVtx[nCntH].nor = (pNor[((m_nXBlock * 2) * m_nYBlock) - 1] + pNor[((m_nXBlock * 2) * m_nYBlock) - 2]) / 2;  // OK
					}
					else
					{// それ以外の頂点番号の場合
						pVtx[nCntH].nor = (pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((m_nXBlock * 2) - 2)] + pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((m_nXBlock * 2) - 2) + 1] + pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((m_nXBlock * 2) - 2) + 2]) / 3;  // OK
					}
				}
				else
				{// 真ん中の場合
					if (nCntV == 0)
					{// 一番上の頂点番号の場合
						pVtx[nCntH].nor = (pNor[(nCntH * 2) - 1] + pNor[(nCntH * 2)] + pNor[(nCntH * 2) + 1]) / 3;   // OK
					}
					else if (nCntV == m_nYBlock)
					{// 一番下の頂点番号の場合
						pVtx[nCntH].nor = (pNor[((nCntH - 1) * 2) + ((m_nXBlock * 2) * (m_nYBlock - 1))] + pNor[((nCntH - 1) * 2) + ((m_nXBlock * 2) * (m_nYBlock - 1)) + 1] + pNor[((nCntH - 1) * 2) + ((m_nXBlock * 2) * (m_nYBlock - 1)) + 2]) / 3; // OK
					}
					else
					{// それ以外の頂点番号の場合
						pVtx[nCntH].nor = (pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((nCntH - 1) * 2)] + pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((nCntH - 1) * 2) + 1] + pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((nCntH - 1) * 2) + 2] + pNor[(nCntV * (m_nXBlock * 2) + 1) + ((nCntH - 1) * 2)] + pNor[(nCntV * (m_nXBlock * 2) + 1) + ((nCntH - 1) * 2) + 1] + pNor[(nCntV * (m_nXBlock * 2) + 1) + ((nCntH - 1) * 2) + 2]) / 6;  // OK
					}
				}
			}
			// ポインタを進める
			pVtx += m_nXBlock + 1;
		}

		if (pNor != NULL)
		{// メモリが確保されている
			delete[] pNor;
			pNor = NULL;
		}

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}

	// 頂点バッファを設定する
	SetVtxBuff(pVtxBuff);
}

//=============================================================================
//    テクスチャ設定処理
//=============================================================================
void CMeshSphere::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    頂点バッファ設定処理
//=============================================================================
void CMeshSphere::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    インデックスバッファ設定処理
//=============================================================================
void CMeshSphere::SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff)
{
	m_pIdxBuff = pIdxBuff;
}

//=============================================================================
//    メッシュスフィアの座標設定処理
//=============================================================================
void CMeshSphere::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    メッシュスフィアの向き設定処理
//=============================================================================
void CMeshSphere::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    メッシュスフィアの色設定処理
//=============================================================================
void CMeshSphere::SetCol(const D3DXCOLOR col)
{
	// 色を設定
	m_Col = col;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	    // 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// 垂直方向の分割数 + 1だけ繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// 水平方向の分割数 + 1だけ繰り返し
			    // 頂点カラー
				pVtx[nCntH].col = m_Col;
			}
			// ポインタを進める
			pVtx += m_nXBlock + 1;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    メッシュスフィアの半径設定処理
//=============================================================================
void CMeshSphere::SetRadius(const float fRadius)
{
	m_fRadius = fRadius;
}

//=============================================================================
//    横の分割数設定処理
//=============================================================================
void CMeshSphere::SetXBlock(const int nXBlock)
{
	m_nXBlock = nXBlock;
}

//=============================================================================
//    奥行の分割数設定処理
//=============================================================================
void CMeshSphere::SetYBlock(const int nYBlock)
{
	m_nYBlock = nYBlock;
}

//=============================================================================
//    ワールドマトリックス設定処理
//=============================================================================
void CMeshSphere::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    頂点バッファ取得処理
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CMeshSphere::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    インデックスバッファ取得処理
//=============================================================================
LPDIRECT3DINDEXBUFFER9 CMeshSphere::GetIdxBuff(void)
{
	return m_pIdxBuff;
}

//=============================================================================
//    テクスチャ取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 CMeshSphere::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    メッシュスフィアの座標取得処理
//=============================================================================
D3DXVECTOR3 CMeshSphere::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    メッシュスフィアの向き取得処理
//=============================================================================
D3DXVECTOR3 CMeshSphere::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    メッシュスフィアの色取得処理
//=============================================================================
D3DXCOLOR CMeshSphere::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    メッシュスフィアの半径取得処理
//=============================================================================
float CMeshSphere::GetRadius(void)
{
	return m_fRadius;
}

//=============================================================================
//    横の分割数取得処理
//=============================================================================
int CMeshSphere::GetXBlock(void)
{
	return m_nXBlock;
}

//=============================================================================
//    奥行の分割数取得処理
//=============================================================================
int CMeshSphere::GetYBlock(void)
{
	return m_nYBlock;
}

//=============================================================================
//    頂点数取得処理
//=============================================================================
int CMeshSphere::GetNumVertex(void)
{
	return m_nNumVertex;
}

//=============================================================================
//    インデックス数取得処理
//=============================================================================
int CMeshSphere::GetNumIndex(void)
{
	return m_nNumIndex;
}

//=============================================================================
//    ポリゴン数取得処理
//=============================================================================
int CMeshSphere::GetNumPolygon(void)
{
	return m_nNumPolygon;
}

//=============================================================================
//    ワールドマトリックス取得処理
//=============================================================================
D3DXMATRIX CMeshSphere::GetMtxWorld(void)
{
	return m_MtxWorld;
}