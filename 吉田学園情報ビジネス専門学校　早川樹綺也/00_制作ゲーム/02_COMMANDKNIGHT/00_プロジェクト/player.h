//=============================================================================
//
// プレイヤーの処理 [player.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "scene.h"
#include "character.h"

#define MAX_PLAYER_MODEL (17)
#define MAX_PLAYER_PARTS (17)
#define MAX_MOTION		 (11)
#define MAX_KEY			 (10)
#define MAX_QUANTITY	 (9)
#define MAX_SELECT		 (3)
#define PLAYER_MOVE		 (2.5f)
//前方宣言
class CModel;
class CMeshOrbit;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPlayer : public CCharacter
{
public:	//誰からもアクセス可能
	//*************************************************************************
	// 構造体定義
	//*************************************************************************
	typedef struct
	{
		float fPosX;
		float fPosY;
		float fPosZ;
		float fRotX;
		float fRotY;
		float fRotZ;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	}KEY;

	typedef struct
	{
		int nFrame;
		KEY aKey[MAX_PLAYER_MODEL];
	}KEY_INFO;

	typedef struct
	{
		bool bLoop;
		int nNumKey;
		KEY_INFO aKeyInfo[MAX_KEY];
	}MOTION_INFO;

	typedef struct
	{
		int nNumFrame;
		int nAttackStartFrame;
		int nAttackEndFrame;
	}MOTION_ATTACK;

	typedef enum
	{
		MOTION_NEUTRAL = 0,		//ニュートラルモーション
		MOTION_MOVE,			//移動モーション
		MOTION_COMBO0,			//パンチモーション
		MOTION_COMBO1,			//パンチモーション
		MOTION_JUMP,			//ジャンプモーション
		MOTION_ICRBRAKE,		//アイスブレイクモーション
		MOTION_REVOSLASH,		//レボスラッシュモーション
		MOTION_FULLSWING,		//フルスイングモーション
		MOTION_ITEM,			//アイテムモーション
		MOTION_DEATH,			//死亡モーション
		MOTION_MAX,
	}MOTION_TYPE;

	typedef enum
	{
		PLAYER_STATE_NOMAL = 0,
		PLAYER_STATE_DAMAGE,
		PLAYER_STATE_DEATH,
		PLAYER_STATE_MAX,
	}PLAYER_STATE;

	typedef enum
	{
		USE_ITEM0 = 0,
		USE_ITEM1,
		USE_ITEM2,
		USE_ITEM_MAX,
	}USE_ITEM;
	//メンバ関数
	CPlayer();
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void UpdateMotion(void);
	void LoadMotion(void);

	void SetMotion(MOTION_TYPE type);
	void SetAction(bool action);
	void SetLife(int nLife);
	void SetSkill(int nSkill);
	void SetMaxLife(int nMaxLife);
	void SetState(PLAYER_STATE state);
	void AddSkillLevel(int nSelect, int nValue);
	void AddItemQuantity(int nSelect, int nValue);

	bool GetAction(void);
	int	 GetLife(void);
	int  GetSkill(void);
	int GetMaxLife(void);
	int GetMaxSkill(void);
	int GetSkillLevel(int nSelect);
	int GetItemQuantity(int nSelect);
	MOTION_TYPE GetMotion(void);
	PLAYER_STATE GetState(void);
	void HitDamage(int nDamage);
	void SetUseItem(USE_ITEM item);
	void UseItem(void);
	int GetCutSkill(int nSkill);
	void AddDeathBird(int nValue);
	void AddDeathFrog(int nValue);
	int GetDeathBird(void);
	int GetDeathFrog(void);
	bool HitAttack(void);
	bool GetJump(void);

	//静的メンバ関数
	static CPlayer *Create(D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);

private:	//自分だけがアクセス可能
	//メンバ関数
	void Input(void);
	void Move(void);
	void Collision(void);
	void SetCountLevel(MOTION_TYPE type);
	void SwitchWeaponSize(void);
	void SwitchIceSize(void);
	void IceBreak(void);
	void RevoSlash(void);
	void FullSwing(void);
	void ReadCharacterSet(char *pStrCur, FILE *pFile,char *pLine);
	void ReadMotionSet(char *pStrCur, FILE *pFile, char *pLine,int nCntMotion);

	//メンバ変数
	D3DXMATRIX					m_mtxWorld;										//ワールドマトリックス
	D3DXVECTOR3					m_VtxMin[MAX_PLAYER_MODEL];						//最小値
	D3DXVECTOR3					m_VtxMax[MAX_PLAYER_MODEL];						//最大値
	D3DXVECTOR3					m_offsetPos[MAX_PLAYER_MODEL];					//オフセット
	bool						m_bAction;										//行動しているかどうか
	int							m_nNumKey;										//キーの総数
	KEY_INFO					*m_pKeyInfo;									//キー情報のポインタ
	int							m_nKey;											//現在のキーNo.
	int							m_nCountFrame;									//フレームカウンター
	int							m_nNumParts;									//パーツの総数
	int							m_nIndexParent[MAX_PLAYER_PARTS];				//プレイヤーのパーツ番号
	KEY							m_aKeyOffset[MAX_PLAYER_PARTS];					//プレイヤーのオフセット
	MOTION_INFO					m_apMotionInfo[MAX_MOTION];						//モーションの情報
	MOTION_TYPE					m_motionType;									//モーションの種類
	D3DXVECTOR3					m_scale;										//大きさ
	int							m_OldMotion;									//過去のモーション
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;										//頂点バッファへのポインタ
	CModel						*m_apModel[MAX_PLAYER_MODEL];					//モデルのポインタ
	int							m_nLife;										//体力
	int							m_nMaxLife;										//体力の最大値
	int							m_nSkill;										//とくぎ
	int							m_nMaxSkill;									//とくぎの最大値
	float						m_fSpin;										//回転
	int							m_aSkillLevel[MAX_SELECT];						//とくぎのレベル
	int							m_aCntLevel[MAX_SELECT];						//とくぎのレベルカウンター
	int							m_aItemQuantity[MAX_SELECT];					//道具の量
	MOTION_ATTACK				m_aMotionAttack[MAX_MOTION];
	int							m_nCntNumFrame;
	CMeshOrbit					*m_pMeshOrbit;									//メッシュオービットのポインタ
	PLAYER_STATE				m_state;
	int							m_nCounter;
	int							m_nCntDamage;
	USE_ITEM					m_useItem;
	int							m_aCutSkill[MAX_SELECT];
	int							m_nNumDeathBird;
	int							m_nNumDeathFrog;
	bool						m_bJump;
	//静的メンバ変数
	static LPD3DXMESH			m_pMesh[MAX_PLAYER_MODEL];						//メッシュ情報へのポインタ
	static LPD3DXBUFFER			m_pBuffMat[MAX_PLAYER_MODEL];					//マテリアル情報へのポインタ
	static DWORD				m_nNumMat[MAX_PLAYER_MODEL];					//マテリアル情報の数
};
#endif