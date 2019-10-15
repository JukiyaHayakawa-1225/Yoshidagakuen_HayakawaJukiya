//=============================================================================
//
// オブジェクト処理 [scene.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_PRIORITY (10)		//優先順位の総数

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CScene
{
public:	//誰からもアクセス可能
	//=========================================================================
	// 構造体の定義
	//=========================================================================
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_SCENE2D,		// オブジェクト2D
		OBJTYPE_BLOSSOMS,       // 花びら
		OBJTYPE_SCENE3D,		// オブジェクト3D
		OBJTYPE_SCENEX,			// オブジェクトX
		OBJTYPE_NUMBER,         // 数字
		OBJTYPE_FADE,			// フェード
		OBJTYPE_MESHFIELD,		// メッシュフィールド
		OBJTYPE_MESHCYLINDER,   // メッシュシリンダー
		OBJTYPE_GOALCYLINDER,   // ゴール用円筒
		OBJTYPE_ITEMCYLINDER,   // アイテム用円筒
		OBJTYPE_CHARACYLINDER,  // キャラセレクト用円筒
		OBJTYPE_BILLBOARD,		// ビルボード
		OBJTYPE_OBJECT,			// オブジェクト
		OBJTYPE_3D,             // 3Dオブジェクト
		OBJTYPE_BLOCK,          // ブロック
		OBJTYPE_CHARACTER,      // キャラクター
		OBJTYPE_PLAYER,			// プレイヤー
		OBJTYPE_BULLET,         // 弾
		OBJTYPE_BULLETEFFECT,   // 弾のエフェクト
		OBJTYPE_ENEMY,          // 敵
		OBJTYPE_RIVER,          // 川
		OBJTYPE_ICEFIELD,       // 氷の地面
		OBJTYPE_SPLASH,         // 飛沫
		OBJTYPE_OBJBILLBOARD,   // ビルボードオブジェクト
		OBJTYPE_HEADQUARTERS,   // 司令部
		OBJTYPE_MESHSPHERE,     // メッシュスフィア
		OBJTYPE_MESHRING,       // メッシュリング
		OBJTYPE_ITEM,           // アイテム
		OBJTYPE_EMITTER,        // エミッター
		OBJTYPE_PAREMITTER,     // パーティクルエミッター
		OBJTYPE_RINGEMITTER,    // リングエフェクトエミッター
		OBJTYPE_PARTICLE,       // パーティクル
		OBJTYPE_EFFECTMANAGER,  // エフェクトマネージャー
		OBJTYPE_HINAARARE,      // ひなあられ
		OBJTYPE_MIST,           // もや
		OBJTYPE_POWERMAP,       // パワーマップ
		OBJTYPE_MAX,
	} OBJTYPE;

	//メンバ関数
	CScene(int nPriority, OBJTYPE objtype);
	void SetObjType(OBJTYPE objType);
	OBJTYPE GetObjType(void);
	CScene *GetNext(void);
	CScene *GetPrev(void);
	int GetPriority(void);

	//純粋仮想関数
	virtual ~CScene();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	//静的メンバ関数
	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static void DeathCheck(void);
	static void DeathCheck(int nPriority);
	static CScene *GetTop(int nPriority);

private:	//自分だけがアクセス可能
	//メンバ関数
	void Death(void);

	//メンバ変数
	CScene			*m_pPrev;		//前のオブジェクトへのポインタ
	CScene			*m_pNext;		//次のオブジェクトへのポインタ
	bool			m_bDeath;		//死亡フラグ
	int				m_nPriority;	//優先順位の番号
	OBJTYPE			m_objType;		//オブジェクトの種類

	//静的メンバ変数
	static CScene	*m_apTop[NUM_PRIORITY];			//先頭のオブジェクトへのポインタ
	static CScene	*m_apCur[NUM_PRIORITY];			//現在（最後尾）のオブジェクトへのポインタ
	static int		m_nNumAll;						//オブジェクトの総数
	static int		m_nNumPriority[NUM_PRIORITY];	//優先順位の情報
protected:	//自分と派生クラスだけがアクセス可能
	void Release(void);								//データ開放
};
#endif