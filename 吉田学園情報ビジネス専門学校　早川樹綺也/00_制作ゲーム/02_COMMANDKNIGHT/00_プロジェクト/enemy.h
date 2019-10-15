//=============================================================================
//
// 敵の処理 [enemy.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "scene.h"
#include "character.h"

#define MAX_ENEMY_MODEL (16)	//敵モデルの総数
#define MAX_ENEMY_PARTS (16)	//敵パーツの総数
#define MAX_ENEMY_MOTION (10)	//敵モーションの総数
#define MAX_KEY			 (10)	//キーの総数
#define MAX_ENEMY_LIFE	(10)	//敵の体力

//前方宣言
class CModel;
class CBillboardNumber;
class CBillboardLifeGauge;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEnemy : public CCharacter
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
	}KEY;

	typedef struct
	{
		int nFrame;
		KEY aKey[MAX_ENEMY_MODEL];
	}KEY_INFO;

	typedef struct
	{
		bool bLoop;
		int nNumKey;
		KEY_INFO aKeyInfo[MAX_KEY];
	}MOTION_INFO;

	typedef enum
	{
		MOTION_NEUTRAL = 0,		//ニュートラルモーション
		MOTION_MOVE,			//移動モーション
		MOTION_PANCH,			//パンチモーション
		MOTION_JUMP,			//ジャンプモーション
		MOTION_LANDING,			//着地モーション
		MOTION_MAX,
	}MOTION_TYPE;

	typedef enum
	{
		ENEMY_STATE_NOMAL = 0,
		ENEMY_STATE_ATTACK,
		EMEMY_STATE_DAMAGE,
		ENEMY_STATE_MAX,
	}ENEMY_STATE;

	typedef struct
	{
		int nNumFrame;
		int nAttackStartFrame;
		int nAttackEndFrame;
	}MOTION_ATTACK;

	//メンバ関数
	CEnemy(int nPriority = 3,OBJTYPE objtype = OBJTYPE_ENEMY);
	~CEnemy();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void UpdateMotion(void);
	void LoadMotion(char *textName);
	void SetMotion(MOTION_TYPE type);
	void SetAction(bool action);
	bool GetAction(void);
	void HitDamage(int damage);
	void SetDestAngle(float fDestAngle);
	ENEMY_STATE GetEnemyState(void);
	void SetEnemyState(ENEMY_STATE state);
	void Attack(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fColRange);

	//静的メンバ関数
	static CEnemy *Create(D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);
	static int GetNumEnemy(void);
	int GetNumBird(void);
	int GetNumFrog(void);

private:	//自分だけがアクセス可能
	//メンバ関数
	void Collision(void);

	//メンバ変数
	static int			m_nNumEnemy;							//敵の総数
	D3DXMATRIX	m_mtxWorld;										//ワールドマトリックス
	D3DXVECTOR3	m_VtxMin[MAX_ENEMY_MODEL];						//最小値
	D3DXVECTOR3	m_VtxMax[MAX_ENEMY_MODEL];						//最大値
	D3DXVECTOR3	m_offsetPos[MAX_ENEMY_MODEL];					//オフセット
	MOTION_ATTACK	m_aMotionAttack[MAX_ENEMY_MOTION];			//モーションの攻撃情報
	bool		m_bJump;										//ジャンプしているかどうか
	bool		m_bAction;										//行動しているかどうか
	int			m_nNumKey;										//キーの総数
	KEY_INFO	*m_pKeyInfo;									//キー情報のポインタ
	int			m_nKey;											//現在のキーNo.
	int			m_nCountFrame;									//フレームカウンター
	int			m_nNumParts;									//パーツの総数
	int			m_nIndexParent[MAX_ENEMY_PARTS];				//プレイヤーのパーツ番号
	KEY			m_aKeyOffset[MAX_ENEMY_PARTS];					//プレイヤーのオフセット
	MOTION_INFO	m_apMotionInfo[MAX_KEY];						//モーションの情報
	MOTION_TYPE	m_motionType;									//モーションの種類
	D3DXVECTOR3	 m_scale;										//大きさ
	int m_OldMotion;											//過去のモーション
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;						// 頂点バッファへのポインタ
	CModel						*m_apModel[MAX_ENEMY_MODEL];	//モデルのポインタ
	static char	m_aFilenameModel[MAX_ENEMY_MODEL][256];			//モデルファイル
	int			m_nNumModel;									//モデルの総数
	CBillboardNumber		*m_apNumber[2];						// ポインタ
	ENEMY_STATE				m_state;
	float					m_fDestAngle;
	int	m_nCntDamage;
	int m_nCounter;
	int	m_nCntNumFrame;
	int m_nCounterAttack;
	bool m_bAttack;
	//静的メンバ変数
	static LPD3DXMESH			m_pMesh[MAX_ENEMY_MODEL];		//メッシュ情報へのポインタ
	static LPD3DXBUFFER			m_pBuffMat[MAX_ENEMY_MODEL];	//マテリアル情報へのポインタ
	static DWORD				m_nNumMat[MAX_ENEMY_MODEL];	//マテリアル情報の数
	CBillboardLifeGauge			*m_pBillboardLifeGauge;
protected:	//自分と派生クラスのみ
	int m_nNumBird;
	int m_nNumFrog;
};


//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEnemyBird : public CEnemy
{
public:	//誰からもアクセス可能
	//メンバ関数
	CEnemyBird();
	~CEnemyBird();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ変数
	static CEnemyBird *Create(D3DXVECTOR3 pos);


private:	//自分だけがアクセス可能
			//メンバ変数

};

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEnemyFrog : public CEnemy
{
public:	//誰からもアクセス可能
		//メンバ関数
	CEnemyFrog();
	~CEnemyFrog();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ変数
	static CEnemyFrog *Create(D3DXVECTOR3 pos);


private:	//自分だけがアクセス可能
			//メンバ変数
	int m_nCntInterval;
};

#endif