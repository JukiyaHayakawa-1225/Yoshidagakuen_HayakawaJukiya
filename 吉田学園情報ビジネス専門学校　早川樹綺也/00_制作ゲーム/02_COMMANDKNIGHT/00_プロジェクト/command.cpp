//=============================================================================
//
// コマンド処理 [command.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "command.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "scene2D.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "debugproc.h"
#include "loadfilefunction.h"
#include "status_number.h"
#include "sound.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CCommand::m_apTexture[MAX_COMMAND_TEXTURE] = {};					//テクスチャのポインタ
CSkill_Quantity_Number *CCommand::m_pSkillQuantity = NULL;							//とくぎの項目ポインタ
CItem_Quantity_Number	*CCommand::m_pItemQuantity = NULL;							//アイテムの項目ポインタ

//=============================================================================
// コマンドのテクスチャ読み込み
//=============================================================================
HRESULT CCommand::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Command\\cursor.png",
		&m_apTexture[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Command\\command_skill_cicle.png",
		&m_apTexture[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Command\\command_skill_icon.png",
		&m_apTexture[2]);
	return S_OK;
}

//=============================================================================
// コマンドのテクスチャ破棄
//=============================================================================
void CCommand::Unload(void)
{
	// テクスチャの破棄
	for (int nCntTexture = 0 ; nCntTexture < MAX_COMMAND_TEXTURE; nCntTexture++)
	{
		if (m_apTexture[nCntTexture] != NULL)
		{
			m_apTexture[nCntTexture]->Release();
			m_apTexture[nCntTexture] = NULL;
		}
	}
}

//=============================================================================
// コマンドのコンストラクタ
//=============================================================================
CCommand::CCommand() : CScene(4,OBJTYPE_SCENE2D)
{
	m_commandType = COMMAND_MAIN;		//コマンド状態を初期化

	m_pCommandSelect = NULL;			//選択コマンドをNULLにする

	for (int nCntTexture = 0; nCntTexture < MAX_COMMANDSKILL_TEXTURE; nCntTexture++)
	{
		m_apUI_CommandSkill[nCntTexture] = NULL;			//とくぎコマンドのUIをNULLにする
	}

	m_CommandState = COMMAND_NONE;		//実行していない状態にする

	m_nCommandSelect = 0;				//選択番号を初期化

	m_nCntAttack = 0;					//攻撃のカウンターを初期化

	m_nCntInterval = 0;					//間隔カウンターを初期化

	m_CommandCountState = COMMAND_NOCOUNT;	//コマンド

}
//=============================================================================
// コマンドのデストラクタ
//=============================================================================
CCommand::~CCommand()
{

}

//=============================================================================
//コマンドの生成
//=============================================================================
CCommand *CCommand::Create(D3DXVECTOR3 pos)
{
	CCommand *pCommand = NULL;	//CCommandのポインタ

	if (pCommand == NULL)
	{//NULLの場合
		pCommand = new CCommand;	//シーンの動的確保
		if (pCommand != NULL)
		{
			pCommand->Init();		//初期化処理
		}
	}
	return pCommand;				//値を返す
}

//=============================================================================
// コマンドの初期化処理
//=============================================================================
HRESULT CCommand::Init(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//ポインタの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	m_commandType = COMMAND_MAIN;	//コマンド状態を初期化

	//コマンド選択の初期化
	if (m_pCommandSelect == NULL)
	{
		m_pCommandSelect = new CScene2D(7);										//動的確保
		if (m_pCommandSelect != NULL)
		{
			m_pCommandSelect->SetPos(D3DXVECTOR3(15.0f, 595.0f, 0.0f));			//位置の設置処理
			m_pCommandSelect->SetWidth(10.0f);									//幅の設置処理
			m_pCommandSelect->SetHeight(10.0f);									//高さの設置処理
			m_pCommandSelect->Init();											//初期化処理
			m_pCommandSelect->BindTexture(m_apTexture[0]);						//テクスチャの読み込み
		}
	}

	//とくぎコマンドのUIの初期化
	for (int nCntSkill = 0; nCntSkill < MAX_COMMANDSKILL_TEXTURE; nCntSkill++)
	{
		if (m_apUI_CommandSkill[nCntSkill] == NULL)
		{
			m_apUI_CommandSkill[nCntSkill] = new CScene2D(6);										//動的確保
			if (m_apUI_CommandSkill[nCntSkill] != NULL)
			{
				m_apUI_CommandSkill[nCntSkill]->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2 , SCREEN_HEIGHT / 2, 0.0f));			//位置の設置処理
				m_apUI_CommandSkill[nCntSkill]->Init();																		//初期化処理
				m_apUI_CommandSkill[nCntSkill]->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
				if (nCntSkill == 0)
				{
					m_apUI_CommandSkill[nCntSkill]->SetLength(150.0f);												//長さの設置処理
					m_apUI_CommandSkill[nCntSkill]->SetVtxLenghtPos(D3DXVECTOR3(280.0f, SCREEN_HEIGHT / 2, 0.0f));	//長さの設置処理
				}
				else
				{
					m_apUI_CommandSkill[nCntSkill]->SetLength(250.0f);																//長さの設置処理
					m_apUI_CommandSkill[nCntSkill]->SetVtxLenghtPos(D3DXVECTOR3(280.0f - 5.0f, SCREEN_HEIGHT / 2 - 25.0f, 0.0f));	//長さの設置処理
				}
				m_apUI_CommandSkill[nCntSkill]->BindTexture(m_apTexture[1 + nCntSkill]);														//テクスチャの読み込み
			}
		}
	}
	m_nCommandSelect = 0;	//コマンド選択初期化
	m_bUseSkill = false;
	return S_OK;
}

//=============================================================================
// コマンドの終了処理
//=============================================================================
void CCommand::Uninit(void)
{
	//コマンドの破棄
	Release();
}

//=============================================================================
// コマンドの更新処理
//=============================================================================
void CCommand::Update(void)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	//ジョイパッドの取得
	CInputJoypad *pInputJoypad;
	pInputJoypad = CManager::GetJoypad();

	//プレイヤーの取得
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();

	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	if (m_bUseSkill == false)
	{
		for (int nCntSkill = 0; nCntSkill < MAX_COMMANDSKILL_TEXTURE; nCntSkill++)
		{
			m_apUI_CommandSkill[nCntSkill]->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		}

	}
	else
	{
		{
			for (int nCntSkill = 0; nCntSkill < MAX_COMMANDSKILL_TEXTURE; nCntSkill++)
			{
				m_apUI_CommandSkill[nCntSkill]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}

	if (m_bUseSkill == false)
	{
		if (m_CommandState == COMMAND_NONE /*&& m_commandType != COMMAND_SKILL*/)
		{//コマンド実行をしていない場合
			m_pCommandSelect->SetHeight(10.0f);
			// 選択項目移動処理
			if (pInputKeyboard->GetTrigger(DIK_DOWN) == true ||
				pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_DOWN) == true)
			{// 下キーまたは十字下または左スティック下が押された
				m_nCommandSelect = (m_nCommandSelect + 1) % MAX_COMMAND_SELECT;								//コマンド選択番号を減算
			}
			if (pInputKeyboard->GetTrigger(DIK_UP) == true ||
				pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_UP) == true)
			{// 上キーまたは十字上または左スティック上が押された
				m_nCommandSelect = (m_nCommandSelect + (MAX_COMMAND_SELECT - 1)) % MAX_COMMAND_SELECT;		//コマンド選択番号を加算
			}

			// 画面遷移
			if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true ||
				pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_B) == true)
			{// ENTERキーまたはBボタンが押された
				SwitchCommnad();	//コマンドの切り替え処理
			}
			if (pInputKeyboard->GetTrigger(DIK_LEFT) == true ||
				pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_A) == true)
			{//左キーまたはAボタンが押された
				m_nCommandSelect = 0;				//コマンド選択番号を初期化
				m_commandType = COMMAND_MAIN;		//メインコマンド画面にする

				//とくぎ項目の終了処理
				if (m_pSkillQuantity != NULL)
				{
					m_pSkillQuantity->Uninit();
					m_pSkillQuantity = NULL;
				}
				//どうぐ項目の終了処理
				if (m_pItemQuantity != NULL)
				{
					m_pItemQuantity->Uninit();
					m_pItemQuantity = NULL;
				}
			}
		}
		else if (m_CommandState == COMMAND_PROCESS)
		{//実行状態の場合

			m_nCommandSelect = 0;					//コマンド選択番号を初期化
			m_commandType = COMMAND_MAIN;			//メインコマンド画面にする
			m_pCommandSelect->SetHeight(0.0f);		//コマンドの高さを０にする
		}
	}
	else
	{
		m_nCommandSelect = 0;					//コマンド選択番号を初期化
		m_pCommandSelect->SetHeight(0.0f);		//コマンドの高さを０にする
		 if (pInputKeyboard->GetTrigger(DIK_LEFT) == true ||
			pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_LEFT) == true)
		{// 上キーまたは十字上または左スティック上が押された
			if (m_nCommandSelect == 0 && pPlayer->GetSkill() >= pPlayer->GetCutSkill(0))
			{//コマンド選択が０かつMPが使用する量以上ある場合
				pPlayer->SetMotion(pPlayer->MOTION_ICRBRAKE);							//とくぎ０モーションにする
				pPlayer->SetSkill(pPlayer->GetSkill() - pPlayer->GetCutSkill(0));	//MPの設置処理
				m_CommandState = COMMAND_PROCESS;									//コマンドを実行している状態にする
				pSound->PlaySound(CSound::SOUND_LABEL_SE_SKILL0);
				//とくぎ項目の終了処理
				if (m_pSkillQuantity != NULL)
				{
					m_pSkillQuantity->Uninit();
					m_pSkillQuantity = NULL;
				}
				m_bUseSkill = false;
			}
		}
		else if (pInputKeyboard->GetTrigger(DIK_UP) == true ||
			pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_UP) == true)
		{// 上キーまたは十字上または左スティック上が押された
			if (m_nCommandSelect == 0 && pPlayer->GetSkill() >= pPlayer->GetCutSkill(1))
			{//コマンド選択が０かつMPが使用する量以上ある場合
				pPlayer->SetMotion(pPlayer->MOTION_REVOSLASH);							//とくぎ１モーションにする
				pPlayer->SetSkill(pPlayer->GetSkill() - pPlayer->GetCutSkill(1));	//MPの設置処理
				m_CommandState = COMMAND_PROCESS;									//コマンドを実行している状態にする
				pSound->PlaySound(CSound::SOUND_LABEL_SE_SKILL1);
				//とくぎ項目の終了処理
				if (m_pSkillQuantity != NULL)
				{
					m_pSkillQuantity->Uninit();
					m_pSkillQuantity = NULL;
				}
				m_bUseSkill = false;
			}
		}
		else if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true ||
			pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_RIGHT) == true)
		{// 上キーまたは十字上または左スティック上が押された
			if (m_nCommandSelect == 0 && pPlayer->GetSkill() >= pPlayer->GetCutSkill(2))
			{//コマンド選択が０かつMPが使用する量以上ある場合
				pPlayer->SetMotion(pPlayer->MOTION_FULLSWING);							//とくぎ２モーションにする
				pPlayer->SetSkill(pPlayer->GetSkill() - pPlayer->GetCutSkill(2));	//MPの設置処理
				m_CommandState = COMMAND_PROCESS;									//コマンドを実行している状態にする
				pSound->PlaySound(CSound::SOUND_LABEL_SE_SKILL2);
																					//とくぎ項目の終了処理
				if (m_pSkillQuantity != NULL)
				{
					m_pSkillQuantity->Uninit();
					m_pSkillQuantity = NULL;
				}
				m_bUseSkill = false;
			}
		}


	}


	//コマンドカーソル別処理
	switch (m_nCommandSelect)
	{
	case 0:
		m_pCommandSelect->SetPos(D3DXVECTOR3(95.0f, 575.0f, 0.0f));			//位置の設置処理
		break;
	case 1:
		m_pCommandSelect->SetPos(D3DXVECTOR3(95.0f, 650.0f, 0.0f));			//位置の設置処理
		break;
	}

	//コマンド状態別処理
	switch (m_CommandCountState)
	{
	case COMMAND_NOCOUNT:	//カウントしていない場合
		m_nCntInterval = 0;	//間隔カウンターを初期化
		m_nCntAttack = 0;	//攻撃カウンターの初期化
		break;

	case COMMAND_COUNT:			//カウントしている場合
		m_nCntInterval++;		//間隔カウンターを初期化

		if (m_nCntInterval >= 120)
		{//間隔カウンターが１２０以上の場合
			m_CommandCountState = COMMAND_NOCOUNT;	//カウントしていない状態にする
		}
		break;
	}
		m_pCommandSelect->SetVtxPos(m_pCommandSelect->GetPos());	//位置の設置処理

#if _DEBUG
		CDebugProc::Print(1, "コマンド選択 [I : 上移動][K : 下移動][L : コマンド実行][J : コマンドを戻す]\n");
		CDebugProc::Print(1, "コマンド実行状態[0 : していない][1 : している][2 : 終了]  %d\n", m_CommandState);
		CDebugProc::Print(1, "選択番号[%d/%d]\n", m_nCommandSelect, MAX_COMMAND_SELECT);
		CDebugProc::Print(1, "カウンター%d\n", m_nCntInterval);
		CDebugProc::Print(1, "アタック%d\n", m_nCntAttack);

#endif
}

//=============================================================================
// コマンドの描画処理
//=============================================================================
void CCommand::Draw(void)
{
}

//=============================================================================
// コマンドの状態設置処理
//=============================================================================
void CCommand::SetCommandState(COMMAND_STATE state)
{
	m_CommandState = state;
}

//=============================================================================
// コマンドの種類設置処理
//=============================================================================
void CCommand::SetCommandType(COMMAND_TYPE type)
{
	m_commandType = type;
}

//=============================================================================
// コマンドの状態取得処理
//=============================================================================
CCommand::COMMAND_STATE CCommand::GetCommandState(void)
{
	return m_CommandState;
}

//=============================================================================
// コマンドの種類取得処理
//=============================================================================
CCommand::COMMAND_TYPE CCommand::GetCommandType(void)
{
	return m_commandType;
}

//=============================================================================
// コマンドのとくぎを使用しているか取得
//=============================================================================
bool CCommand::GetUseSkill(void)
{
	return m_bUseSkill;
}

//=============================================================================
// コマンドの切り替え処理
//=============================================================================
void CCommand::SwitchCommnad(void)
{
	//プレイヤーの取得
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();

	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	switch (m_commandType)
	{
	case COMMAND_MAIN:	//メイン画面の場合
		if (m_nCommandSelect == 0)
		{

				if (m_nCntAttack == 0)
				{

					pPlayer->SetMotion(pPlayer->MOTION_COMBO0);			//攻撃モーションにする
					pSound->PlaySound(CSound::SOUND_LABEL_SE_SLASH);
					pPlayer->SetAction(true);							//行動している状態にする
					m_CommandCountState = COMMAND_COUNT;				//カウントをしている状態にする
					m_CommandState = COMMAND_ATTACK_PROCESS;			//コマンドを実行している状態にする

					if (m_nCntInterval <= 120)
					{//間隔カウンターが１２０以上の場合
						m_nCntAttack++;		//攻撃カウンター加算

					}

				}
				else if (m_nCntAttack == 1)
				{//攻撃カウンターが１の場合
					pPlayer->SetMotion(pPlayer->MOTION_COMBO1);		//攻撃モーションにする
					pSound->PlaySound(CSound::SOUND_LABEL_SE_SLASH);

					pPlayer->SetAction(true);						//行動している状態にする
					m_CommandCountState = COMMAND_NOCOUNT;			//カウントしていない状態にする
					m_CommandState = COMMAND_ATTACK_PROCESS;			//コマンドを実行している状態にする
					m_pSkillQuantity = CSkill_Quantity_Number::Create(D3DXVECTOR3(375.0f, SCREEN_HEIGHT - 165.0f, 0.0f), 10.0f, 13.0f);
					m_bUseSkill = true;
				}

		}
		else if (m_nCommandSelect == 1)
		{//コマンド選択が２の場合
			//どうぐ項目の生成
			m_pItemQuantity = CItem_Quantity_Number::Create(D3DXVECTOR3(405.0f, SCREEN_HEIGHT - 132.5f, 0.0f), 13.0f, 17.5f);

			m_commandType = COMMAND_ITEM;						//どうぐのコマンド画面にする
			m_nCommandSelect = 0;								//コマンド選択番号を初期化
			m_nCntAttack = 0;									//攻撃カウンターの初期化

		}
		break;
	case COMMAND_ITEM:	//どうぐ画面の場合
			if (m_nCommandSelect == 0 && pPlayer->GetItemQuantity(0) > 0)
			{//どうぐ選択が０かつどうぐが０より大きい場合
				pPlayer->SetMotion(pPlayer->MOTION_ITEM);	//どうぐ使用モーションにする
				pPlayer->AddItemQuantity(0, -1);			//どうぐ０の残数を減らす
				pPlayer->SetUseItem(CPlayer::USE_ITEM0);	//どうぐの設置処理
				m_CommandState = COMMAND_PROCESS;			//コマンドを実行している状態にする
				//アイテム項目の終了処理
				if (m_pItemQuantity != NULL)
				{
					m_pItemQuantity->Uninit();
					m_pItemQuantity = NULL;
				}
			}
			else if (m_nCommandSelect == 1 && pPlayer->GetItemQuantity(1) > 0)
			{//どうぐ選択が１かつどうぐが０より大きい場合
				pPlayer->SetMotion(pPlayer->MOTION_ITEM);	//どうぐ使用モーションにする
				pPlayer->AddItemQuantity(1, -1);			//どうぐ１の残数を減らす
				pPlayer->SetUseItem(CPlayer::USE_ITEM1);	//どうぐの設置処理
				m_CommandState = COMMAND_PROCESS;			//コマンドを実行している状態にする
				//アイテム項目の終了処理
				if (m_pItemQuantity != NULL)
				{
					m_pItemQuantity->Uninit();
					m_pItemQuantity = NULL;
				}
			}
			else if (m_nCommandSelect == 2 && pPlayer->GetItemQuantity(2) > 0)
			{//どうぐ選択が１かつどうぐが０より大きい場合
				pPlayer->SetMotion(pPlayer->MOTION_ITEM);	//どうぐ使用モーションにする
				pPlayer->SetUseItem(CPlayer::USE_ITEM2);	//どうぐ１の残数を減らす
				pPlayer->AddItemQuantity(2, -1);			//どうぐの設置処理
				m_CommandState = COMMAND_PROCESS;			//コマンドを実行している状態にする
				//アイテム項目の終了処理
				if (m_pItemQuantity != NULL)
				{
					m_pItemQuantity->Uninit();
					m_pItemQuantity = NULL;
				}
			}
		break;
	}
}