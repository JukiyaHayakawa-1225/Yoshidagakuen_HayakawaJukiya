//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "player.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAMERA_MOVE (1.0f);	//カメラの移動量

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CCamera::CCamera()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// カメラの生成
//=============================================================================
CCamera *CCamera::Create()
{
	CCamera *pCamera = NULL;	//カメラのポインタ

	if (pCamera == NULL)
	{//NULLでない場合
		pCamera = new CCamera;	//カメラの動的確保
		pCamera->Init();		//初期化処理
	}
	return pCamera;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCamera::Init(void)
{
	//モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();

	//モードごとの初期化処理
	switch (mode)
	{
	case CManager::MODE_TITLE:	//タイトルの場合
		m_posV = D3DXVECTOR3(600.0f, 60.0f, -1200.0f);						//視点
		m_posR = D3DXVECTOR3(600.0f, 40.0f, -1000.0f);						//注視点

		break;
	case CManager::MODE_TUTORIAL:	//タイトルの場合
		m_posV = D3DXVECTOR3(600.0f, 60.0f, -1200.0f);						//視点
		m_posR = D3DXVECTOR3(600.0f, 40.0f, -1000.0f);						//注視点

		break;
	case CManager::MODE_GAME:	//ゲームの場合
		m_posV = D3DXVECTOR3(0.0f, 100.0f, -150.0f);						//視点
		m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//注視点

		break;
	case CManager::MODE_RESULT:	//リザルトの場合
		m_posV = D3DXVECTOR3(600.0f, 60.0f, -1200.0f);						//視点
		m_posR = D3DXVECTOR3(600.0f, 40.0f, -1000.0f);						//注視点

		break;
	case CManager::MODE_RANKING:	//ランキングの場合
		m_posV = D3DXVECTOR3(600.0f, 100.0f, -900.0f);						//視点
		m_posR = D3DXVECTOR3(600.0f, 60.0f, -700.0f);						//注視点

		break;
	}

	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);								//上方向ベクトル
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							//目的の視点
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							//目的の注視点
	m_rot.y =atan2f(m_posR.x - m_posV.x, m_posR.z - m_posV.z) ;			//向き
	m_rotDest = m_rot;													//目的の向き
	m_rotDiff = 0.0f;
	m_fLength = sqrtf((m_posV.x - m_posR.x) * (m_posV.x - m_posR.x) + (m_posV.z - m_posR.z) * (m_posV.z - m_posR.z));	//距離

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCamera::Uninit(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void CCamera::Update(void)
{
	//モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{//ゲームの場合
		//プレイヤーの取得
		CPlayer *pPlayer;
		pPlayer = CGame::GetPlayer();

		if (pPlayer != NULL)
		{//NULLでない場合
			//プレイヤーの位置の取得
			D3DXVECTOR3 playerpos = pPlayer->GetPos();
			//プレイヤーの向きの取得
			D3DXVECTOR3 playerrot = pPlayer->GetRot();

			//追尾処理
			m_posVDest.x = playerpos.x + sinf(m_rot.y) * -m_fLength;
			m_posVDest.y = (playerpos.y + 100.0f) /*+ m_eyepoint*/;
			m_posVDest.z = playerpos.z + cosf(m_rot.y) * -m_fLength;

			m_posRDest.x = playerpos.x - sinf(playerrot.y) * 1.0f;
			m_posRDest.y = (playerpos.y + 50.0f);
			m_posRDest.z = playerpos.z - cosf(playerrot.y) * 1.0f;

			m_posV.x += (m_posVDest.x - m_posV.x) * 0.4f;
			m_posV.y += (m_posVDest.y - m_posV.y) * 0.4f;
			m_posV.z += (m_posVDest.z - m_posV.z) * 0.4f;

			m_posR.x += (m_posRDest.x - m_posR.x) * 0.4f;
			m_posR.y += (m_posRDest.y - m_posR.y) * 0.4f;
			m_posR.z += (m_posRDest.z - m_posR.z) * 0.4f;

		}
		//キーボードの取得
		CInputKeyboard *pInputKeyboard;
		pInputKeyboard = CManager::GetKeyboard();

		//ジョイパッドの取得
		CInputJoypad *pInputJoypad;
		pInputJoypad = CManager::GetJoypad();

		if (pInputKeyboard->GetPress(DIK_Z) == true ||
			pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_RSTICK_LEFT) == true)
		{// キーボードの[Z]キーが押された
			m_rot.y += 0.025f;
			if (m_rot.y >= D3DX_PI)
			{
				m_rot.y = -D3DX_PI;
			}
			m_posV.x = m_posR.x - sinf(m_rot.y) * m_fLength;
			m_posV.z = m_posR.z - cosf(m_rot.y) *m_fLength;
		}
		if (pInputKeyboard->GetPress(DIK_C) == true||
			pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_RSTICK_RIGHT) == true)
		{// キーボードの[C]キーが押された
			m_rot.y -= 0.025f;
			if (m_rot.y <= -D3DX_PI)
			{
				m_rot.y = D3DX_PI;
			}
			m_posV.x = m_posR.x - sinf(m_rot.y) * m_fLength;
			m_posV.z = m_posR.z - cosf(m_rot.y) * m_fLength;
		}

	}
		////キーボードの取得
		//CInputKeyboard *pInputKeyboard;
		//pInputKeyboard = CManager::GetKeyboard();

		//if (pInputKeyboard->GetPress(DIK_RIGHT) == true)
		//{// キーボードの[→]キーが押された
		//	m_posR.x += 2.0f;
		//	m_posV.x += 2.0f;
		//}
		//if (pInputKeyboard->GetPress(DIK_LEFT) == true)
		//{// キーボードの[←]キーが押された
		//	m_posR.x -= 2.0f;
		//	m_posV.x -= 2.0f;
		//}
		//if (pInputKeyboard->GetPress(DIK_UP) == true)
		//{// キーボードの[↑]キーが押された
		//	m_posR.z += 2.0f;
		//	m_posV.z += 2.0f;
		//}
		//if (pInputKeyboard->GetPress(DIK_DOWN) == true)
		//{// キーボードの[↓]キーが押された
		//	m_posR.z -= 2.0f;
		//	m_posV.z -= 2.0f;
		//}
		////===================================================
		////　注視点処理
		////===================================================
		//if (pInputKeyboard->GetPress(DIK_T) == true)
		//{// キーボードの[T]キーが押された
		//	m_posR.y += 2.0f;
		//}
		//if (pInputKeyboard->GetPress(DIK_B) == true)
		//{// キーボードの[B]キーが押された
		//	m_posR.y -= 2.0f;
		//}
		//if (pInputKeyboard->GetPress(DIK_Q) == true)
		//{// キーボードの[Q]キーが押された（左回りに旋回）
		//	m_rot.y -= 0.025f;
		//	if (m_rot.y <= -D3DX_PI)
		//	{
		//		m_rot.y = D3DX_PI;
		//	}
		//	m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
		//	m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
		//}
		//if (pInputKeyboard->GetPress(DIK_E) == true)
		//{// キーボードの[E]キーが押された
		//	m_rot.y += 0.025f;
		//	if (m_rot.y >= D3DX_PI)
		//	{
		//		m_rot.y = -D3DX_PI;
		//	}
		//	m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
		//	m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
		//}

		////===================================================
		////　視点処理
		////===================================================
		//if (pInputKeyboard->GetPress(DIK_Y) == true)
		//{// キーボードの[Y]キーが押された
		//	m_posV.y += 2.0f;
		//}
		//if (pInputKeyboard->GetPress(DIK_N) == true)
		//{// キーボードの[N]キーが押された
		//	m_posV.y -= 2.0f;
		//}
		//if (pInputKeyboard->GetPress(DIK_Z) == true)
		//{// キーボードの[Z]キーが押された
		//	m_rot.y += 0.025f;
		//	if (m_rot.y >= D3DX_PI)
		//	{
		//		m_rot.y = -D3DX_PI;
		//	}
		//	m_posV.x = m_posR.x - sinf(m_rot.y) * m_fLength;
		//	m_posV.z = m_posR.z - cosf(m_rot.y) *m_fLength;
		//}
		//if (pInputKeyboard->GetPress(DIK_C) == true)
		//{// キーボードの[C]キーが押された
		//	m_rot.y -= 0.025f;
		//	if (m_rot.y <= -D3DX_PI)
		//	{
		//		m_rot.y = D3DX_PI;
		//	}
		//	m_posV.x = m_posR.x - sinf(m_rot.y) * m_fLength;
		//	m_posV.z = m_posR.z - cosf(m_rot.y) * m_fLength;
		//}


	//注視点の表示
	CDebugProc::Print(1, "視点の移動    : x[%.1f],y[%.1f],z[%.1f]\n", m_posV.x, m_posV.y, m_posV.z);
	//視点の表示
	CDebugProc::Print(1, "注視点の移動  : x[%.1f],y[%.1f],z[%.1f]\n", m_posR.x, m_posR.y, m_posR.z);
}

//=============================================================================
// 設定処理
//=============================================================================
void CCamera::SetCamera(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),						//画角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	//画面比率
		10.0f,										//手前
		1500.0f);									//奥行き	変更中元1000

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&m_mtxView,
				       &m_posV,
					   &m_posR,
					   &m_vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}


//=============================================================================
// 向き取得
//=============================================================================
D3DXVECTOR3 CCamera::GetRot(void)
{
	return m_rot;
}