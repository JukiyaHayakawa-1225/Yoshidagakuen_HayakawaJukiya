//=============================================================================
//
// ポーズ処理 [pause.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "pause.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "sound.h"
//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CPause::m_apTexture[MAX_PAUSE_TEXTURE] = {};

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// ポーズのコンストラクタ
//=============================================================================
CPause::CPause() :CScene(6)
{
}
//=============================================================================
// ポーズのデストラクタ
//=============================================================================
CPause::~CPause()
{
}
//=============================================================================
// ポーズのテクスチャ読み込み
//=============================================================================
HRESULT CPause::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\pause.jpg",
		&m_apTexture[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\pause000.png",
		&m_apTexture[1]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\pause001.png",
		&m_apTexture[2]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\pause002.png",
		&m_apTexture[3]);
	return S_OK;
}

//=============================================================================
// ポーズのテクスチャ破棄
//=============================================================================
void CPause::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_PAUSE_SELECT; nCnt++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCnt] != NULL)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
//ポーズの生成
//=============================================================================
CPause *CPause::Create(void)
{
	CPause *pPause = NULL;
	if (pPause == NULL)
	{
		pPause = new CPause;		//シーンの動的確保
		if (pPause != NULL)
		{
			pPause->Init();					//初期化処理
		}
	}
	return pPause;					//値を返す
}

//=============================================================================
// ポーズの初期化処理
//=============================================================================
HRESULT CPause::Init(void)
{
	//背景の初期化
	m_pScene2D = new CScene2D(6);
	m_pScene2D->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	m_pScene2D->SetWidth(250.0f);
	m_pScene2D->SetHeight(250.0f);
	m_pScene2D->Init();
	m_pScene2D->BindTexture(m_apTexture[0]);

	//CONTINUEの初期化
	float Pos = 0;
	for (int nCnt = 0; nCnt < MAX_PAUSE_SELECT; nCnt++)
	{
		m_apSelect[nCnt] = new CScene2D(7);
		m_apSelect[nCnt]->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, 250.0f + Pos, 0.0f));
		m_apSelect[nCnt]->SetWidth(150);
		m_apSelect[nCnt]->SetHeight(50);
		m_apSelect[nCnt]->Init();
		m_apSelect[nCnt]->BindTexture(m_apTexture[nCnt + 1]);
		Pos += 130;
	}
	m_aCol[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);						// 色を初期化
	m_aSelect[0] = PAUSESELECT_SELECT;									// スタートはCONTINUEから
	for (int nCnt = 1; nCnt < MAX_PAUSE_SELECT; nCnt++)
	{
		m_aCol[nCnt] = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);				//色を初期化
		m_aSelect[nCnt] = PAUSESELECT_NONE;								//スタートはCONTINUEから
	}
	m_fCol = 0.01f;														//色の変化を初期化
	m_nSelect = 0;														// 選択されている番号はCONTINUEに
	m_select = SELECT_CONTINUE;											//ゲーム再開状態にしておく

	for (int nCnt = 0; nCnt < MAX_PAUSE_SELECT; nCnt++)
	{
		m_apSelect[nCnt]->SetCol(m_aCol[nCnt]);
	}
	return S_OK;
}

//=============================================================================
// ポーズの終了処理
//=============================================================================
void CPause::Uninit(void)
{
	if (m_pScene2D != NULL)
	{
		m_pScene2D->Uninit();
		m_pScene2D = NULL;
	}
	for (int nCntPause = 0; nCntPause < MAX_PAUSE_SELECT; nCntPause++)
	{
		if (m_apSelect[nCntPause] != NULL)
		{
			m_apSelect[nCntPause]->Uninit();
			m_apSelect[nCntPause] = NULL;
		}
	}
	Release();
}

//=============================================================================
// ポーズの更新処理
//=============================================================================
void CPause::Update(void)
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

	for (int nCntSelect = 0; nCntSelect < MAX_PAUSE_SELECT; nCntSelect++)
	{
		if (m_apSelect[nCntSelect] != NULL)
		{
			m_apSelect[nCntSelect]->Update();
		}
	}

	// 選択項目移動処理
	if (pInputKeyboard->GetTrigger(DIK_S) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_DOWN) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_LSTICK_DOWN) == true)
	{// Sキーまたは十字下または左スティック下が押された
		m_aSelect[m_nSelect] = PAUSESELECT_NONE;
		m_aSelect[(m_nSelect + 1) % MAX_PAUSE_SELECT] = PAUSESELECT_SELECT;
		m_nSelect = (m_nSelect + 1) % MAX_PAUSE_SELECT;
	}
	if (pInputKeyboard->GetTrigger(DIK_W) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_UP) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_LSTICK_UP) == true)
	{// Wキーまたは十字上または左スティック上が押された
		m_aSelect[m_nSelect] = PAUSESELECT_NONE;
		m_aSelect[(m_nSelect + (MAX_PAUSE_SELECT - 1)) % MAX_PAUSE_SELECT] = PAUSESELECT_SELECT;
		m_nSelect = (m_nSelect + (MAX_PAUSE_SELECT - 1)) % MAX_PAUSE_SELECT;
	}

	for (int nCntPauseSelect = 0; nCntPauseSelect < MAX_PAUSE_SELECT; nCntPauseSelect++)
	{// 項目の数だけ繰り返し
		if (m_aSelect[nCntPauseSelect] == PAUSESELECT_SELECT)
		{// 選択されていたら
			m_aCol[nCntPauseSelect].r += m_fCol;
			m_aCol[nCntPauseSelect].g += m_fCol;
			m_aCol[nCntPauseSelect].b += m_fCol;

			if (m_aCol[nCntPauseSelect].r < 0.2f || m_aCol[nCntPauseSelect].r >= 1.0f)
			{// 色が既定の値に達した
				m_fCol *= -1;
			}
		}
		else if (m_aSelect[nCntPauseSelect] == PAUSESELECT_NONE)
		{// 選択されていなかったら
			m_aCol[nCntPauseSelect].r = 0.2f;
			m_aCol[nCntPauseSelect].g = 0.2f;
			m_aCol[nCntPauseSelect].b = 0.2f;
		}
		// 頂点カラーの更新
		m_apSelect[nCntPauseSelect]->SetCol(m_aCol[nCntPauseSelect]);
	}
	// 画面遷移
	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_B) == true)
	{// ENTERキーが押された
		pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSESELECT);
		CFade::FADE fade;
		CFade *pFade = NULL;
		fade = pFade->GetFade();

		//if (fade == CFade::FADE_NONE)
		//{// フェード状態でない

			if (m_nSelect == SELECT_CONTINUE)
			{
				m_select = SELECT_CONTINUE; // ゲーム再開状態に
			}
			else if (m_nSelect == SELECT_RETRY)
			{
				m_select = SELECT_RETRY;    // ゲームやり直し状態に
			}
			else if (m_nSelect == SELECT_QUIT)
			{
				m_select = SELECT_QUIT;     // タイトル遷移状態に
			}

			switch (m_select)
			{
			case CPause::SELECT_CONTINUE:  // ゲーム再開
				break;
			case CPause::SELECT_RETRY:     // ゲームやり直し
				CFade::Create(CManager::MODE_GAME, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
				//return; // これ以上ゲームはUpdateしない
				break;
			case CPause::SELECT_QUIT:      // タイトル遷移
				CFade::Create(CManager::MODE_TITLE, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
				//return; // これ以上ゲームはUpdateしない
				break;
			}
			CManager::GetRenderer()->RelesePause();
	}

	if (pInputKeyboard->GetTrigger(DIK_P) == true||
		pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_START) == true)
	{// ポーズが解除された
		m_aCol[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色を初期化
		m_aSelect[0] = PAUSESELECT_SELECT;					// スタートはCONTINUEから
		for (int nCntPauseSelect = 1; nCntPauseSelect < MAX_PAUSE_SELECT; nCntPauseSelect++)
		{// 項目の数だけ繰り返し
			m_aCol[nCntPauseSelect] = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);		// 色を初期化
			m_aSelect[nCntPauseSelect] = PAUSESELECT_NONE;						// 選択されていない状態にする
		}
		m_nSelect = 0;      // 選択されている番号はCONTINUEに

		for (int nCntPauseSelect = 1; nCntPauseSelect < MAX_PAUSE_SELECT; nCntPauseSelect++)
		{
			m_apSelect[nCntPauseSelect]->SetCol(m_aCol[nCntPauseSelect]);
		}
	}
}

//=============================================================================
// ポーズの描画処理
//=============================================================================
void CPause::Draw(void)
{
	for (int nCntSelect = 0; nCntSelect < MAX_PAUSE_SELECT; nCntSelect++)
	{
		if (m_apSelect != NULL)
		{
			m_apSelect[nCntSelect]->Draw();
		}
	}
}