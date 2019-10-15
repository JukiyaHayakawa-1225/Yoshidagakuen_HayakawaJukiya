//=============================================================================
//
// ステータス数字の処理 [score.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "status_number.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "number.h"
#include "player.h"
#include "game.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
int CStatus_Number::m_nStatus_Number = 0;	//ステータス数字

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// ステータス数字のコンストラクタ
//=============================================================================
CStatus_Number::CStatus_Number() : CScene(6, OBJTYPE_NUMBER)
{
}
//=============================================================================
// ステータス数字のデストラクタ
//=============================================================================
CStatus_Number::~CStatus_Number()
{

}

//=============================================================================
//ステータス数字の生成
//=============================================================================
CStatus_Number *CStatus_Number::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CStatus_Number *pStatus_Number = NULL;
	if (pStatus_Number == NULL)
	{
		pStatus_Number = new CStatus_Number;		//シーンの動的確保
		pStatus_Number->m_pos = pos;				//位置の設置
		pStatus_Number->m_fWidth = fWidth;			//幅の設置
		pStatus_Number->m_fHeight = fHeight;		//高さの設置
		pStatus_Number->Init();						//初期化処理
	}
	return pStatus_Number;							//値を返す
}

//=============================================================================
// ステータス数字の初期化処理
//=============================================================================
HRESULT CStatus_Number::Init(void)
{
	int nLife = 0;	//体力
	int nSkill = 0;	//とくぎ
	int nLifeData = 0;	//体力格納
	int nSkillData = 0;	//とくぎ格納

	//プレイヤーの取得
	CPlayer *pPlayer = CGame::GetPlayer();
	nLife = pPlayer->GetLife();		//体力の取得
	nSkill = pPlayer->GetSkill();	//とくぎの取得

	for (int nCntStatus = 0; nCntStatus < MAX_STATUS; nCntStatus++)
	{//ステータスの数だけ繰り返し
		for (int nCntNumber = 0; nCntNumber < SCORE_DIGIT; nCntNumber++)
		{//桁の数だけ繰り返し
			m_apNumber[nCntStatus][nCntNumber] = NULL;
		}
	}

	for (int nCntStatus = 0; nCntStatus < MAX_STATUS; nCntStatus++)
	{//ステータスの数だけ繰り返し
		int nDigit = 1;
		for (int nCntNumber = 0; nCntNumber < MAX_STATUS_DIGIT; nCntNumber++)
		{//桁の数だけ繰り返し
			if (m_apNumber[nCntStatus][nCntNumber] == NULL)
			{
				//モードの取得
				CManager::MODE mode = CManager::GetMode();
				if (mode == CManager::MODE_GAME)
				{//ゲームの場合
					switch (nCntStatus)
					{
					case 0:
						//数字の生成
						m_apNumber[0][nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x - (30 * nCntNumber), m_pos.y, m_pos.z), m_fWidth, m_fHeight);
						nLifeData = nLife % (nDigit * 10) / nDigit;
						m_apNumber[0][nCntNumber]->SetNumber(nLifeData);
						nDigit *= 10;
						break;
					case 1:
						//数字の生成
						m_apNumber[1][nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x - (30 * nCntNumber), m_pos.y + 50.0f, m_pos.z), m_fWidth, m_fHeight);
						nSkillData = nSkill % (nDigit * 10) / nDigit;
						m_apNumber[1][nCntNumber]->SetNumber(nSkillData);
						nDigit *= 10;
						break;
					}
				}
				m_nStatus_Number = 0;
			}
		}
	}
	return S_OK;
}

//=============================================================================
// ステータス数字の終了処理
//=============================================================================
void CStatus_Number::Uninit(void)
{
	//ステータス数字の終了処理
	for (int nCntStatus = 0; nCntStatus < MAX_STATUS; nCntStatus++)
	{
		for (int nCntNumber = 0; nCntNumber < SCORE_DIGIT; nCntNumber++)
		{
			if (m_apNumber[nCntStatus][nCntNumber] != NULL)
			{
				//終了処理
				m_apNumber[nCntStatus][nCntNumber]->Uninit();
				//メモリの開放
				delete m_apNumber[nCntStatus][nCntNumber];
				//NULLにする
				m_apNumber[nCntStatus][nCntNumber] = NULL;
			}
		}
	}
	Release();
}

//=============================================================================
// ステータス数字の更新処理
//=============================================================================
void CStatus_Number::Update(void)
{
	int nLife = 0;	//体力
	int nSkill = 0;	//とくぎ
	int nLifeData = 0;	//体力格納
	int nSkillData = 0;	//とくぎ格納

	//プレイヤーの取得
	CPlayer *pPlayer = CGame::GetPlayer();
	nLife = pPlayer->GetLife();		//体力の取得
	nSkill = pPlayer->GetSkill();	//とくぎの取得


	for (int nCntStatus = 0; nCntStatus < MAX_STATUS; nCntStatus++)
	{
		int nDigit = 1;

		for (int nCntNumber = 0; nCntNumber < MAX_STATUS_DIGIT; nCntNumber++)
		{
				//種類別処理
				switch (nCntStatus)
				{
				case 0:
					nLifeData = nLife % (nDigit * 10) / nDigit;			//体力の計算
					m_apNumber[0][nCntNumber]->SetNumber(nLifeData);	//数字の設置処理
					nDigit *= 10;
					break;
				case 1:
					nSkillData = nSkill % (nDigit * 10) / nDigit;		//とくぎの計算
					m_apNumber[1][nCntNumber]->SetNumber(nSkillData);	//数字の設置処理
					nDigit *= 10;
					break;
				}
				m_nStatus_Number = 0;
		}
	}
}

//=============================================================================
// ステータス数字の描画処理
//=============================================================================
void CStatus_Number::Draw(void)
{
	for (int nCntStatus = 0; nCntStatus < MAX_STATUS; nCntStatus++)
	{
		for (int nCntStatus_Number = 0; nCntStatus_Number < SCORE_DIGIT; nCntStatus_Number++)
		{
			if (m_apNumber[nCntStatus][nCntStatus_Number] != NULL)
			{
				//数字の描画処理
				m_apNumber[nCntStatus][nCntStatus_Number]->Draw();
			}
		}
	}
}

//=============================================================================
// ステータス数字の設置処理
//=============================================================================
void CStatus_Number::SetStatus_Number(int nStatus_Number)
{
	//ステータス数字の加算
	m_nStatus_Number += nStatus_Number;
}

//=============================================================================
// ステータス数字の加算
//=============================================================================
void CStatus_Number::AddStatus_Number(int nValue,int nType)
{
	int nAnswer = 1;
	int nStatus_Number;

	m_nStatus_Number = nValue;	//

	for (int nCntNumber = 0; nCntNumber < MAX_STATUS_DIGIT; nCntNumber++)
	{
		nStatus_Number = m_nStatus_Number % (nAnswer * 10) / nAnswer;
		if (m_apNumber[nType][nCntNumber] != NULL)
		{
			//数字の設置
			m_apNumber[nType][nCntNumber]->SetNumber(nStatus_Number);
		}
		nAnswer *= 10;
	}
}


//=============================================================================
// ステータス数字の取得
//=============================================================================
int CStatus_Number::GetStatus_Number(void)
{
	//値を返す
	return m_nStatus_Number;
}

//=============================================================================
// ステータス数字の取得
//=============================================================================
void CStatus_Number::SetCol(D3DXCOLOR col)
{
	for (int nCntNumber = 0; nCntNumber < SCORE_DIGIT; nCntNumber++)
	{
		//m_apNumber[nCntNumber]->SetCol(col);
	}
}

//=============================================================================
//
// とくぎ量の数字処理
//
//=============================================================================
//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
int CSkill_Quantity_Number::m_nNumber = 0;	//数字

//=============================================================================
// ステータス数字のコンストラクタ
//=============================================================================
CSkill_Quantity_Number::CSkill_Quantity_Number() : CScene(6, OBJTYPE_NUMBER)
{
}
//=============================================================================
// ステータス数字のデストラクタ
//=============================================================================
CSkill_Quantity_Number::~CSkill_Quantity_Number()
{

}

//=============================================================================
//ステータス数字の生成
//=============================================================================
CSkill_Quantity_Number *CSkill_Quantity_Number::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CSkill_Quantity_Number *pSkill_Quantity_Number = NULL;
	if (pSkill_Quantity_Number == NULL)
	{
		pSkill_Quantity_Number = new CSkill_Quantity_Number;		//シーンの動的確保
		pSkill_Quantity_Number->m_pos = pos;						//位置の設置
		pSkill_Quantity_Number->m_fWidth = fWidth;					//幅の設置
		pSkill_Quantity_Number->m_fHeight = fHeight;				//高さの設置
		pSkill_Quantity_Number->Init();								//初期化処理
	}
	return pSkill_Quantity_Number;									//値を返す
}

//=============================================================================
// ステータス数字の初期化処理
//=============================================================================
HRESULT CSkill_Quantity_Number::Init(void)
{
	//プレイヤーの取得
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();

	for (int nCntSkill_Quantity = 0; nCntSkill_Quantity < MAX_SKILL; nCntSkill_Quantity++)
	{//とくぎの数だけ繰り返し
		for (int nCntNumber = 0; nCntNumber < MAX_QUANTITY_DIGIT; nCntNumber++)
		{//桁の数だけ繰り返し
			m_apNumber[nCntSkill_Quantity][nCntNumber] = NULL;
		}
	}

	//番号の生成
	m_apNumber[0][0] = CNumber::Create(D3DXVECTOR3(280.0f - 72.5f, SCREEN_HEIGHT / 2 - 50.0f , m_pos.z), m_fWidth, m_fHeight);
	m_apNumber[0][1] = CNumber::Create(D3DXVECTOR3(280.0f - 90.0f, SCREEN_HEIGHT / 2 - 50.0f, m_pos.z), m_fWidth, m_fHeight);
	m_apNumber[1][0] = CNumber::Create(D3DXVECTOR3(280.0f + 182.5f, SCREEN_HEIGHT / 2 - 125.0f, m_pos.z), m_fWidth, m_fHeight);
	m_apNumber[1][1] = CNumber::Create(D3DXVECTOR3(280.0f + 165.0f, SCREEN_HEIGHT / 2 - 125.0f, m_pos.z), m_fWidth, m_fHeight);
	m_apNumber[2][0] = CNumber::Create(D3DXVECTOR3(280.0f + 230.0f, SCREEN_HEIGHT / 2 + 27.5f, m_pos.z), m_fWidth, m_fHeight);
	m_apNumber[2][1] = CNumber::Create(D3DXVECTOR3(280.0f + 212.5f, SCREEN_HEIGHT / 2 + 27.5f, m_pos.z), m_fWidth, m_fHeight);

	for (int nCntSkill_Quantity = 0; nCntSkill_Quantity < MAX_SKILL; nCntSkill_Quantity++)
	{//とくぎの数だけ繰り返し
		int nDigit = 1;

		for (int nCntNumber = 0; nCntNumber < MAX_QUANTITY_DIGIT; nCntNumber++)
		{//桁の数だけ繰り返し
			if (m_apNumber[nCntSkill_Quantity][nCntNumber] != NULL)
			{
				//数字の生成
				int nSkillData = pPlayer->GetSkillLevel(nCntSkill_Quantity) % (nDigit * 10) / nDigit;
				m_apNumber[nCntSkill_Quantity][nCntNumber]->SetNumber(nSkillData);
				nDigit *= 10;
				m_nNumber = 0;
			}
		}
	}
	return S_OK;
}

//=============================================================================
// ステータス数字の終了処理
//=============================================================================
void CSkill_Quantity_Number::Uninit(void)
{
	//ステータス数字の終了処理
	for (int nCntSkill_Quantity = 0; nCntSkill_Quantity < MAX_SKILL; nCntSkill_Quantity++)
	{
		for (int nCntNumber = 0; nCntNumber < MAX_QUANTITY_DIGIT; nCntNumber++)
		{
			if (m_apNumber[nCntSkill_Quantity][nCntNumber] != NULL)
			{
				//終了処理
				m_apNumber[nCntSkill_Quantity][nCntNumber]->Uninit();
				//メモリの開放
				delete m_apNumber[nCntSkill_Quantity][nCntNumber];
				//NULLにする
				m_apNumber[nCntSkill_Quantity][nCntNumber] = NULL;
			}
		}
	}
	Release();
}

//=============================================================================
// ステータス数字の更新処理
//=============================================================================
void CSkill_Quantity_Number::Update(void)
{

}

//=============================================================================
// ステータス数字の描画処理
//=============================================================================
void CSkill_Quantity_Number::Draw(void)
{
	for (int nCntSkill_Quantity = 0; nCntSkill_Quantity < MAX_SKILL; nCntSkill_Quantity++)
	{
		for (int nCntSkill_Quantity_Number = 0; nCntSkill_Quantity_Number < MAX_QUANTITY_DIGIT; nCntSkill_Quantity_Number++)
		{
			if (m_apNumber[nCntSkill_Quantity][nCntSkill_Quantity_Number] != NULL)
			{
				//数字の描画処理
				m_apNumber[nCntSkill_Quantity][nCntSkill_Quantity_Number]->Draw();
			}
		}
	}
}

//=============================================================================
//
// どうぐ量の数字処理
//
//=============================================================================
//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
int CItem_Quantity_Number::m_nNumber = 0;		//数字

//=============================================================================
// ステータス数字のコンストラクタ
//=============================================================================
CItem_Quantity_Number::CItem_Quantity_Number() : CScene(6, OBJTYPE_NUMBER)
{
}
//=============================================================================
// ステータス数字のデストラクタ
//=============================================================================
CItem_Quantity_Number::~CItem_Quantity_Number()
{

}

//=============================================================================
//ステータス数字の生成
//=============================================================================
CItem_Quantity_Number *CItem_Quantity_Number::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CItem_Quantity_Number *pItem_Quantity_Number = NULL;
	if (pItem_Quantity_Number == NULL)
	{
		pItem_Quantity_Number = new CItem_Quantity_Number;		//シーンの動的確保
		pItem_Quantity_Number->m_pos = pos;						//位置の設置
		pItem_Quantity_Number->m_fWidth = fWidth;				//幅の設置
		pItem_Quantity_Number->m_fHeight = fHeight;				//高さの設置
		pItem_Quantity_Number->Init();							//初期化処理
	}
	return pItem_Quantity_Number;								//値を返す
}

//=============================================================================
// ステータス数字の初期化処理
//=============================================================================
HRESULT CItem_Quantity_Number::Init(void)
{
	//プレイヤーの取得
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();

	for (int nCntItem_Quantity = 0; nCntItem_Quantity < MAX_ITEM; nCntItem_Quantity++)
	{//どうぐの数だけ繰り返し
		for (int nCntNumber = 0; nCntNumber < MAX_QUANTITY_DIGIT; nCntNumber++)
		{//桁の数だけ繰り返し
			m_apNumber[nCntItem_Quantity][nCntNumber] = NULL;
		}
	}

	for (int nCntItem_Quantity = 0; nCntItem_Quantity < MAX_ITEM; nCntItem_Quantity++)
	{//どうぐの数だけ繰り返し
		int nDigit = 1;
		for (int nCntNumber = 0; nCntNumber < MAX_QUANTITY_DIGIT; nCntNumber++)
		{//桁の数だけ繰り返し
			if (m_apNumber[nCntItem_Quantity][nCntNumber] == NULL)
			{
				//数字の生成
				m_apNumber[nCntItem_Quantity][nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x - (25 * nCntNumber), m_pos.y + (62 * nCntItem_Quantity), m_pos.z), m_fWidth, m_fHeight);
				int nItemData = pPlayer->GetItemQuantity(nCntItem_Quantity) % (nDigit * 10) / nDigit;
				m_apNumber[nCntItem_Quantity][nCntNumber]->SetNumber(nItemData);
				nDigit *= 10;
				m_nNumber = 0;
			}
		}
	}
	return S_OK;
}

//=============================================================================
// ステータス数字の終了処理
//=============================================================================
void CItem_Quantity_Number::Uninit(void)
{
	//ステータス数字の終了処理
	for (int nCntItem_Quantity = 0; nCntItem_Quantity < MAX_ITEM; nCntItem_Quantity++)
	{
		for (int nCntNumber = 0; nCntNumber < MAX_QUANTITY_DIGIT; nCntNumber++)
		{
			if (m_apNumber[nCntItem_Quantity][nCntNumber] != NULL)
			{
				//終了処理
				m_apNumber[nCntItem_Quantity][nCntNumber]->Uninit();
				//メモリの開放
				delete m_apNumber[nCntItem_Quantity][nCntNumber];
				//NULLにする
				m_apNumber[nCntItem_Quantity][nCntNumber] = NULL;
			}
		}
	}
	Release();
}

//=============================================================================
// ステータス数字の更新処理
//=============================================================================
void CItem_Quantity_Number::Update(void)
{

}

//=============================================================================
// ステータス数字の描画処理
//=============================================================================
void CItem_Quantity_Number::Draw(void)
{
	for (int nCntItem_Quantity = 0; nCntItem_Quantity < MAX_ITEM; nCntItem_Quantity++)
	{
		for (int nCntItem_Quantity_Number = 0; nCntItem_Quantity_Number < MAX_QUANTITY_DIGIT; nCntItem_Quantity_Number++)
		{
			if (m_apNumber[nCntItem_Quantity][nCntItem_Quantity_Number] != NULL)
			{
				//数字の描画処理
				m_apNumber[nCntItem_Quantity][nCntItem_Quantity_Number]->Draw();
			}
		}
	}
}