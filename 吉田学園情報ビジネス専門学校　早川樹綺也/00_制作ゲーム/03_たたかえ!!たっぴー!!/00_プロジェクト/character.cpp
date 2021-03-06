//=============================================================================
//
// キャラクターの処理 [character.cpp]
// Author : Jukiya Hayakawa & Hodaka Niwa
//
//=============================================================================
#include "character.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "model.h"
#include "motion.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// キャラクターのコンストラクタ
//=============================================================================
CCharacter::CCharacter(int nPrioriry, OBJTYPE objtype) : CObject3D(nPrioriry, objtype)
{
	m_apModel = NULL;		          // モデル情報へのポインタ
	m_pMotionManager = NULL;          // モーションクラスへのポインタ
	m_move = INITIALIZE_D3DXVECTOR3;  // 移動量
	m_bShoot = false;                 // 弾を打っているかどうか
	m_nCntBullet = 0;
}

//=============================================================================
// キャラクターのコンストラクタ
//=============================================================================
CCharacter::~CCharacter()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCharacter::Init(void)
{
	// オブジェクトの初期化処理
	CObject3D::Init();

	// 変数をクリアする
	ClearVariable();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCharacter::Uninit(void)
{
	// モデルの破棄
	ReleaseModel();

	// モーションの破棄
	ReleaseMotionManager();

	//オブジェクトの終了処理
	CObject3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CCharacter::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CCharacter::Draw(void)
{
	if (m_apModel != NULL)
	{
		ModelDraw();
	}
}

//=============================================================================
// 変数をクリアする処理
//=============================================================================
void CCharacter::ClearVariable(void)
{
	m_fAccel = 0.0f;                            // 移動できるスピード量
	m_fInertia = 0.0f;                          // 慣性量
	m_fRivisionRot = 0.0f;                      // 向きを補正する倍率
	m_posOld = INITIALIZE_D3DXVECTOR3;			// 過去の位置
	m_move = INITIALIZE_D3DXVECTOR3;			// 移動量
	m_bShoot = false;							// 撃っているかどうか
	m_nCntBullet = 0;							// 弾のカウンター
}

//=============================================================================
// モデルクラスを開放する処理
//=============================================================================
void CCharacter::ReleaseModel(void)
{
	if (m_apModel != NULL)
	{
		for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
		{
			if (m_apModel[nCntParts] != NULL)
			{
				m_apModel[nCntParts]->Uninit();
				delete m_apModel[nCntParts];
				m_apModel[nCntParts] = NULL;
			}
		}
		delete[] m_apModel;
		m_apModel = NULL;
	}
}

//=============================================================================
// モーション管轄クラスを開放する処理
//=============================================================================
void CCharacter::ReleaseMotionManager(void)
{
	if (m_pMotionManager != NULL)
	{
		m_pMotionManager->Uninit();
		delete m_pMotionManager;
		m_pMotionManager = NULL;
	}
}

//=============================================================================
// モデルを描画する処理
//=============================================================================
void CCharacter::ModelDraw(void)
{
	for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
	{
		if (m_apModel[nCntParts] != NULL)
		{
			m_apModel[nCntParts]->Draw();
		}
	}
}

//=============================================================================
// モデルのパーツ数を設定する
//=============================================================================
void CCharacter::SetNumPart(const int nNumParts)
{
	m_nNumParts = nNumParts;
}

//=============================================================================
// モデルクラスへのポインタを設定する
//=============================================================================
void CCharacter::SetModel(CModel **apModel)
{
	m_apModel = apModel;
}

//=============================================================================
// モーション管轄クラスへのポインタを設定する
//=============================================================================
void CCharacter::SetMotionManager(CMotionManager *pMotionManager)
{
	m_pMotionManager = pMotionManager;
}

//=============================================================================
// 移動できるスピード量を設定する
//=============================================================================
void CCharacter::SetAccel(const float fAccel)
{
	m_fAccel = fAccel;
}

//=============================================================================
// 慣性量を設定する
//=============================================================================
void CCharacter::SetInertia(const float fInertia)
{
	m_fInertia = fInertia;
}

//=============================================================================
// 向きを補正する倍率を設定する
//=============================================================================
void CCharacter::SetRivisionRot(const float fRivisionRot)
{
	m_fRivisionRot = fRivisionRot;
}

//=============================================================================
// 過去の位置の設置処理
//=============================================================================
void CCharacter::SetPosOld(D3DXVECTOR3 posOld)
{
	m_posOld = posOld;
}

//=============================================================================
// 移動量の設置処理
//=============================================================================
void CCharacter::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//=============================================================================
// 撃っているかどうかの設置処理
//=============================================================================
void CCharacter::SetShoot(bool bShoot)
{
	m_bShoot = bShoot;
}

//=============================================================================
// 現在向き情報設置処理
//=============================================================================
void CCharacter::SetNowRotInfo(CCharacter::NOW_ROT_INFO nowRotInfo)
{
	m_nowRotInfo = nowRotInfo;
}

//=============================================================================
// 弾のカウンター設置処理
//=============================================================================
void CCharacter::SetCntBullet(int nCntBullet)
{
	m_nCntBullet = nCntBullet;
}

//=============================================================================
// パーツ数取得処理
//=============================================================================
int CCharacter::GetNumPart(void)
{
	return m_nNumParts;
}

//=============================================================================
// モデルクラスへのポインタ取得処理
//=============================================================================
CModel **CCharacter::GetModel(void)
{
	return m_apModel;
}

//=============================================================================
// モーション管轄クラスへのポインタ取得処理
//=============================================================================
CMotionManager *CCharacter::GetMotionManager(void)
{
	return m_pMotionManager;
}

//=============================================================================
// 移動できるスピード量設定処理
//=============================================================================
float CCharacter::GetAccel(void)
{
	return m_fAccel;
}

//=============================================================================
// 慣性量取得処理
//=============================================================================
float CCharacter::GetInertia(void)
{
	return m_fInertia;
}

//=============================================================================
// 向きを補正する倍率取得処理
//=============================================================================
float CCharacter::GetRivisionRot(void)
{
	return m_fRivisionRot;
}

//=============================================================================
// 過去の位置の取得処理
//=============================================================================
D3DXVECTOR3 CCharacter::GetPosOld(void)
{
	return m_posOld;
}

//=============================================================================
// 移動量の取得処理
//=============================================================================
D3DXVECTOR3 CCharacter::GetMove(void)
{
	return m_move;
}

//=============================================================================
// 撃っているかどうかの取得処理
//=============================================================================
bool CCharacter::GetShoot(void)
{
	return m_bShoot;
}

//=============================================================================
// 現座の向き情報取得処理
//=============================================================================
CCharacter::NOW_ROT_INFO CCharacter::GetNowRotInfo(void)
{
	return m_nowRotInfo;
}

//=============================================================================
// 弾のカウンター取得処理
//=============================================================================
int CCharacter::GetCntBullet(void)
{
	return m_nCntBullet;
}