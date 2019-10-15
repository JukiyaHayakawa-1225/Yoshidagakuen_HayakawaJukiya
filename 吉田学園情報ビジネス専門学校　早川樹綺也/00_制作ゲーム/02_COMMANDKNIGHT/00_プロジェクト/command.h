//=============================================================================
//
// コマンド処理 [command.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "main.h"
#include "scene.h"

#define MAX_COMMAND_TYPE     (COMMAND_MAX - 1)		//コマンドの最大数
#define MAX_COMMAND_SELECT	 (COMMAND_MAX)			//コマンドの選択最大数
#define MAX_COMMAND_TEXTURE	 (3)					//コマンドのテクスチャ最大数
#define MAX_ENEMY_NAME		 (3)					//敵の名前を表示できる総数
#define MAX_COMMANDSKILL_TEXTURE (2)				//とくぎコマンドのテクスチャ最大数
class CScene2D;
class CSkill_Quantity_Number;
class CItem_Quantity_Number;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCommand : public CScene
{
public:
	//*****************************************************************************
	// 構造体の定義
	//*****************************************************************************
	typedef enum
	{
		COMMAND_NONE,			// コマンド実行していない状態
		COMMAND_PROCESS,		// コマンド実行中
		COMMAND_ATTACK_PROCESS, // コマンド攻撃中
		COMMAND_SKILL,			// とくぎコマンド実行中
		COMMAND_END,			// コマンド実行終了
		COMMAND_SELECT_MAX
	}COMMAND_STATE;
	typedef enum
	{
		COMMAND_NOCOUNT = 0,
		COMMAND_COUNT,
		COMMAND_COUNT_MAX,
	}COMMAND_COUNT_STATE;
	typedef enum
	{
		COMMAND_MAIN = 0,		//メイン
		COMMAND_ITEM,			//道具
		//COMMAND_SKILL,
		COMMAND_MAX,
	}COMMAND_TYPE;

	//メンバ関数
	CCommand();
	~CCommand();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetCommandState(COMMAND_STATE state);
	void SetCommandType(COMMAND_TYPE type);
	COMMAND_STATE GetCommandState(void);
	COMMAND_TYPE GetCommandType(void);
	bool GetUseSkill(void);
	//静的メンバ関数
	static CCommand *Create(D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);

private:
	//メンバ関数
	void SwitchCommnad(void);

	//メンバ変数
	static LPDIRECT3DTEXTURE9	m_apTexture[MAX_COMMAND_TEXTURE];				// テクスチャへのポインタ
	COMMAND_TYPE				m_commandType;									//
	CScene2D					*m_pCommandSelect;								// コマンドの画面種類と画面の数
	CScene2D					*m_apUI_CommandSkill[MAX_COMMANDSKILL_TEXTURE];	// とくぎコマンドのUI
	int							m_nCommandSelect;								//
	COMMAND_STATE				m_CommandState;									//
	int							m_nCntAttack;									//
	int							m_nCntInterval;									//
	bool						m_bUseSkill;										//スキル使用状態かどうか
	COMMAND_COUNT_STATE			m_CommandCountState;							//
	static CSkill_Quantity_Number		*m_pSkillQuantity;						//
	static CItem_Quantity_Number		*m_pItemQuantity;						//

};
#endif