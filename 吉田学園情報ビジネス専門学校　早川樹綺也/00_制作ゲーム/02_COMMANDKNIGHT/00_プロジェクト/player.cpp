//=============================================================================
//
// プレイヤーの処理 [player.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "player.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "debugproc.h"
#include "scene3D.h"
#include "meshfield.h"
#include "model.h"
#include "game.h"
#include "object.h"
#include "command.h"
#include "enemy.h"
#include "loadfilefunction.h"
#include "status_number.h"
#include "ui.h"
#include "particle.h"
#include "meshorbit.h"
#include "command.h"
#include "icecircle.h"
#include "icicle.h"

#define MOTION_TXT_NAME "data\\TEXT\\motion.txt"

//=============================================================================
// グローバル変数
//=============================================================================
int g_nNumModel = 2;			//モデルの総数
char g_aFilenameModel[MAX_PLAYER_MODEL][256];	//

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPD3DXMESH			CPlayer::m_pMesh[MAX_PLAYER_MODEL] = {};
LPD3DXBUFFER		CPlayer::m_pBuffMat[MAX_PLAYER_MODEL] = {};
DWORD				CPlayer::m_nNumMat[MAX_PLAYER_MODEL] = {};

//=============================================================================
// プレイヤーのコンストラクタ
//=============================================================================
CPlayer::CPlayer() : CCharacter(3, OBJTYPE_PLAYER)
{
	m_bJump = false;								//ジャンプしていない状態
	m_bAction = false;								//行動していない状態
	m_motionType = MOTION_NEUTRAL;					//モーションを初期化
	m_nCountFrame = 0;								//カウントの初期値
	m_nLife = 100;									//体力初期値
	m_nSkill = 40;									//とくぎ初期値
	m_nMaxLife = 100;								//体力最大値初期化
	m_nMaxSkill = 40;								//とくぎの最大値初期化
	m_state = PLAYER_STATE_NOMAL;
	m_useItem = USE_ITEM0;
	m_nNumDeathBird = 0;
	m_nNumDeathFrog = 0;
	m_pMeshOrbit = NULL;
	for (int nCntSelect = 0; nCntSelect < MAX_SELECT; nCntSelect++)
	{
		m_aSkillLevel[nCntSelect] = 1;
		m_aCntLevel[nCntSelect] = 0;
		m_aItemQuantity[nCntSelect] = 1;
		m_aCutSkill[nCntSelect] = 1;
	}
	for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
	{//モデルの数だけ繰り返し
		m_apModel[nCntModel] = NULL;
	}
	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{//モデルの数だけ繰り返し
		m_aMotionAttack[nCntMotion].nAttackEndFrame = 0;
		m_aMotionAttack[nCntMotion].nAttackStartFrame = 0;
		m_aMotionAttack[nCntMotion].nNumFrame = 0;
	}
}
//=============================================================================
// プレイヤーのデストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// オブジェクトのテクスチャ読み込み
//=============================================================================
HRESULT CPlayer::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	return S_OK;
}

//=============================================================================
// オブジェクトのテクスチャ破棄
//=============================================================================
void CPlayer::Unload(void)
{
	for (int nCntMesh = 0; nCntMesh < MAX_PLAYER_MODEL; nCntMesh++)
	{//モデルの数だけ繰り返し
	 // メッシュの開放
		if (m_pMesh[nCntMesh] != NULL)
		{
			m_pMesh[nCntMesh]->Release();
			m_pMesh[nCntMesh] = NULL;
		}
	}

	for (int nCntMat = 0; nCntMat < MAX_PLAYER_MODEL; nCntMat++)
	{//モデルの数だけ繰り返し
	 // マテリアルの開放
		if (m_pBuffMat[nCntMat] != NULL)
		{
			m_pBuffMat[nCntMat]->Release();
			m_pBuffMat[nCntMat] = NULL;
		}
	}
}

//=============================================================================
//プレイヤーの生成
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos)
{
	CPlayer *pPlayer = NULL;					//プレイヤーのポインタ
	if (pPlayer == NULL)
	{
		pPlayer = new CPlayer;					//シーンの動的確保
		pPlayer->Init();						//初期化処理
		pPlayer->SetPos(pos);					//位置の設置処理
	}
	return pPlayer;					//値を返す
}

//=============================================================================
// プレイヤーの初期化処理
//=============================================================================
HRESULT CPlayer::Init(void)
{
	//キャラクターの初期化処理
	CCharacter::Init();

	//モーション情報を読み込み
	LoadMotion();
	m_bJump = false;							//ジャンプしていない状態
	m_nKey = 0;									//現在のキーを初期化
	m_nNumKey = 2;								//キーの総数を初期化
	m_nCountFrame = 0;							//カウントフレームを初期化
												//m_pMeshOrbit = CMeshOrbit::Create();
	m_state = PLAYER_STATE_NOMAL;				//通常状態にする
	m_nCntDamage = 0;							//ダメージカウンター
	m_nCounter = 0;								//カウンター
	SetColRange(15.0f);							//当たり判定の大きさ設置処理
	for (int nCntSelect = 0; nCntSelect < MAX_SELECT; nCntSelect++)
	{
		m_aSkillLevel[nCntSelect] = 1;	//とくぎレベルの初期化
		m_aCntLevel[nCntSelect] = 0;	//カウントレベルの初期化
		m_aItemQuantity[nCntSelect] = 3;	//アイテム数の初期化
	}
	//敵死亡数を初期化
	m_nNumDeathBird = 0;
	m_nNumDeathFrog = 0;

	return S_OK;
}

//=============================================================================
// プレイヤーの終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
	{//モデルの数だけ繰り返し
		if (m_apModel[nCntModel] != NULL)
		{
			//終了処理
			m_apModel[nCntModel]->Uninit();
			delete m_apModel[nCntModel];
			m_apModel[nCntModel] = NULL;
		}
	}

	Release();
}

//=============================================================================
// プレイヤーの更新処理
//=============================================================================
void CPlayer::Update(void)
{
	//過去の位置設置処理
	SetPosOld(GetPos());

	//モーションの更新処理
	UpdateMotion();

	//現在のモーションを過去のモーション情報に代入
	m_OldMotion = m_motionType;

	switch (m_state)
	{
	case PLAYER_STATE_NOMAL:
		break;
	case PLAYER_STATE_DAMAGE:
		m_nCntDamage++;
		if (m_nCntDamage >= 100)
		{
			m_nCntDamage = 0;
			m_state = PLAYER_STATE_NOMAL;
		}
		break;
	}

	//入力処理
	Input();

	//移動処理
	Move();

	////当たり判定処理
	Collision();

#ifdef _DEBUG
	CDebugProc::Print(1, "プレイヤーの位置  : x[%.1f],y[%.1f],z[%.1f]\n", GetPos().x, GetPos().y, GetPos().z);
	CDebugProc::Print(1, "プレイヤーの体力  : %d]\n", m_nLife);
	CDebugProc::Print(1, "プレイヤーのとくぎ : %d]\n", m_nSkill);
	CDebugProc::Print(1, "モーションOLD %d\n", m_OldMotion);

#endif
}

//=============================================================================
// プレイヤーの描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	D3DXMATRIX mtxRot, mtxTrans;					//計算用マトリックス

	if (m_pMeshOrbit != NULL)
	{
		//オービットの設置処理
		m_pMeshOrbit->SetMtxParent(&m_apModel[15]->GetMtxWorld());
	}
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		rot.y, rot.x, rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	D3DXMATRIX	mtxShadow;		//シャドウマトリックス
	D3DXPLANE	planeField;		//平面情報
	D3DXVECTOR4 vecLight;		//ライト情報
	D3DXVECTOR3	posNomal, normal;	//位置、法線

									//減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP,
		D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND,
		D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,
		D3DBLEND_ONE);

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//シャドウマトリックスを初期化
	D3DXMatrixIdentity(&mtxShadow);

	//ライトの設定
	vecLight = D3DXVECTOR4(0.25f, 0.87f, -0.44f, 0.0f);

	//位置の設定
	posNomal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//法線の設定
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//位置と法線から平面を作成する
	D3DXPlaneFromPointNormal(&planeField, &posNomal, &normal);

	//平面の射影
	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);

	//ワールドマトリックスと掛け合わせる
	D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	if (m_motionType == MOTION_ICRBRAKE)
	{
		for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
		{//モデルの数だけ繰り返し
		 //テクスチャ無効
		 //pDevice->SetTexture(0, NULL);
		 // モデル(パーツ)の描画
		 //m_pMesh[nCntModel]->DrawSubset(0);
		 //描画処理
			if (m_apModel[nCntModel] != NULL)
			{
				m_apModel[nCntModel]->Draw();
			}
		}
	}
	else
	{
		for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL - 1; nCntModel++)
		{//モデルの数だけ繰り返し
		 //描画処理
			if (m_apModel[nCntModel] != NULL)
			{
				m_apModel[nCntModel]->Draw();
			}
		}
	}

	//元の設定に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP,
		D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND,
		D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,
		D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);


	if (m_motionType == MOTION_ICRBRAKE)
	{
		for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
		{
			if (m_state == PLAYER_STATE_DAMAGE)
			{// 押されていない状態の場合
				if (m_nCntDamage % 5 == 0)
				{// カウンターが一定の値になったら
				 //モデルの描画
					if (m_apModel[nCntModel] != NULL)
					{
						m_apModel[nCntModel]->Draw();
					}
				}
			}
			else
			{
				//モデルの描画
				m_apModel[nCntModel]->Draw();
			}
		}
	}
	else
	{
		for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL - 1; nCntModel++)
		{
			if (m_state == PLAYER_STATE_DAMAGE)
			{// 押されていない状態の場合
				if (m_nCntDamage % 5 == 0)
				{// カウンターが一定の値になったら
				 //モデルの描画
					if (m_apModel[nCntModel] != NULL)
					{
						m_apModel[nCntModel]->Draw();
					}
				}
			}
			else
			{
				//モデルの描画
				if (m_apModel[nCntModel] != NULL)
				{
					m_apModel[nCntModel]->Draw();
				}
			}
		}
	}
}

//=============================================================================
// プレイヤーのモーション更新処理
//=============================================================================
void CPlayer::UpdateMotion(void)
{
	KEY *pKey, *pKeyNext;		// キーの情報
	float fRateMotion;			// 相対値
	float fDiffMotion;			// 差分
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き

	if (m_OldMotion != m_motionType)
	{//前回のモーションを違う場合
		m_nKey = 0;			//現在のキーを初期化
		m_nCountFrame = 0;	//カウントフレームを初期化
		m_nCntNumFrame = 0;
	}
	m_nCntNumFrame++;
	if (m_nCntNumFrame >= m_aMotionAttack[m_motionType].nAttackStartFrame &&
		m_nCntNumFrame <= m_aMotionAttack[m_motionType].nAttackEndFrame)
	{
		switch (m_motionType)
		{
		case MOTION_COMBO0:
			m_bAction = true;
			break;
		case MOTION_COMBO1:
			m_bAction = true;
			break;
		case MOTION_ICRBRAKE:
			if (m_bAction == false)
			{
				m_bAction = true;
				SwitchIceSize();
			}
			break;
		case MOTION_REVOSLASH:
			RevoSlash();
			break;
		case MOTION_FULLSWING:
			SwitchWeaponSize();
			break;
		case MOTION_ITEM:
			UseItem();
			break;
		}
	}
	else
	{
		m_bAction = false;
		m_apModel[15]->SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	}

	if (m_state != PLAYER_STATE_DEATH)
	{

		for (int nCntModel = 0; nCntModel < g_nNumModel; nCntModel++)
		{//モデルの数だけ繰り返し
			if (m_apModel[nCntModel] != NULL)
			{//モデルがある場合
				pKey = &m_apMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[nCntModel];
				pKeyNext = &m_apMotionInfo[m_motionType].aKeyInfo[(m_nKey + 1) % m_apMotionInfo[m_motionType].nNumKey].aKey[nCntModel];

#if 1
				//位置座標X
				fRateMotion = (float)(m_nCountFrame) / (float)m_apMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame;	// 相対値の取得
				fDiffMotion = pKeyNext->pos.x - pKey->pos.x;														// 差分の取得
				pos.x = pKey->pos.x + (fDiffMotion * fRateMotion);

				//位置座標Y
				fDiffMotion = pKeyNext->pos.y - pKey->pos.y;								// 差分の取得
				pos.y = pKey->pos.y + (fDiffMotion * fRateMotion);

				//位置座標Z
				fDiffMotion = pKeyNext->pos.z - pKey->pos.z;								// 差分の取得
				pos.z = pKey->pos.z + (fDiffMotion * fRateMotion);

				//絶対値処理
				D3DXVECTOR3 rotPlus, rotMinus;
				rotPlus.x = pKeyNext->rot.x + pKey->rot.x;
				rotPlus.y = pKeyNext->rot.y + pKey->rot.y;
				rotPlus.z = pKeyNext->rot.z + pKey->rot.z;
				rotMinus.x = pKeyNext->rot.x - pKey->rot.x;
				rotMinus.y = pKeyNext->rot.y - pKey->rot.y;
				rotMinus.z = pKeyNext->rot.z - pKey->rot.z;
				rotPlus.x = abs(rotPlus.x);
				rotPlus.y = abs(rotPlus.y);
				rotPlus.z = abs(rotPlus.z);
				rotMinus.x = abs(rotMinus.x);
				rotMinus.y = abs(rotMinus.y);
				rotMinus.z = abs(rotMinus.z);

				if (nCntModel == 0)
				{
					if (rotPlus.x < rotMinus.x)
					{
						//向き座標X
						fDiffMotion = pKeyNext->rot.x + pKey->rot.x;								// 差分の取得
						rot.x = pKey->rot.x + (fDiffMotion * fRateMotion);
					}
					else
					{
						//向き座標X
						fDiffMotion = pKeyNext->rot.x - pKey->rot.x;								// 差分の取得
						rot.x = pKey->rot.x + (fDiffMotion * fRateMotion);
					}

					if (rotPlus.y < rotMinus.y)
					{
						//向き座標Y
						fDiffMotion = pKeyNext->rot.y + pKey->rot.y;								// 差分の取得
						rot.y = pKey->rot.y + (fDiffMotion * fRateMotion);

					}
					else
					{
						//向き座標Y
						fDiffMotion = pKeyNext->rot.y - pKey->rot.y;								// 差分の取得
						rot.y = pKey->rot.y + (fDiffMotion * fRateMotion);
					}

					if (rotPlus.z < rotMinus.z)
					{
						//向き座標Z
						fDiffMotion = pKeyNext->rot.z + pKey->rot.z;								// 差分の取得
						rot.z = pKey->rot.z + (fDiffMotion * fRateMotion);
					}
					else
					{
						//向き座標Z
						fDiffMotion = pKeyNext->rot.z - pKey->rot.z;								// 差分の取得
						rot.z = pKey->rot.z + (fDiffMotion * fRateMotion);
					}
				}
				else
				{
					//向き座標X
					fDiffMotion = pKeyNext->rot.x - pKey->rot.x;								// 差分の取得
					rot.x = pKey->rot.x + (fDiffMotion * fRateMotion);

					//向き座標Y
					fDiffMotion = pKeyNext->rot.y - pKey->rot.y;								// 差分の取得
					rot.y = pKey->rot.y + (fDiffMotion * fRateMotion);

					//向き座標Z
					fDiffMotion = pKeyNext->rot.z - pKey->rot.z;								// 差分の取得
					rot.z = pKey->rot.z + (fDiffMotion * fRateMotion);

				}
#endif
				//向きの設置処理
				m_apModel[nCntModel]->SetRot(rot);
				m_apModel[nCntModel]->SetPos(D3DXVECTOR3(m_offsetPos[nCntModel].x + pos.x, m_offsetPos[nCntModel].y + pos.y, m_offsetPos[nCntModel].z + pos.z));
				//位置の設置処理
				//m_apModel[nCntModel]->SetPos(pos);

			}
		}

		if (m_nKey <= m_apMotionInfo[m_motionType].nNumKey - 1)
		{//現在のキーが対象のキーの総数より小さい場合カウントフレームを加算
			m_nCountFrame++;
		}
		if (m_nCountFrame >= m_apMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame)
		{//現在のフレーム数が対象のキーフレーム数以上の場合
		 //m_nKey = (m_nKey + (m_apMotionInfo[m_motionType].nNumKey - 1)) % m_apMotionInfo[m_motionType].nNumKey;
		 //現在のキーの切り替え
			m_nKey = (m_nKey + 1) % m_apMotionInfo[m_motionType].nNumKey;
			if (m_motionType == MOTION_ICRBRAKE && m_nKey == 5)
			{
				IceBreak();
			}
			else if (m_motionType == MOTION_REVOSLASH)
			{
				RevoSlash();
			}
			else if (m_motionType == MOTION_FULLSWING)
			{
				if (m_nKey >= 5 && m_nKey <= 6)
				{
					FullSwing();
				}
			}
			if (m_nKey == m_apMotionInfo[m_motionType].nNumKey - 1 && m_apMotionInfo[m_motionType].bLoop == false)
			{//現在のキーが対象のキー総数に達しかつループしない場合
				if (m_motionType == MOTION_JUMP && m_bJump == true)
				{//
					m_nKey = m_apMotionInfo[m_motionType].nNumKey - 1;
				}
				//if (m_motionType == MOTION_PANCH || m_motionType == MOTION_LANDING)
				else if (m_motionType != MOTION_MOVE || m_motionType != MOTION_NEUTRAL)
				{

					if (m_motionType == MOTION_DEATH)
					{
						m_nKey = m_apMotionInfo[m_motionType].nNumKey - 1;
						SetState(PLAYER_STATE_DEATH);
					}
					else
					{

						//現在のキーを初期化
						m_nKey = 0;
						//コマンドの取得
						CCommand *pCommand;
						pCommand = CGame::GetCommad();
						//ニュートラルに切り替え
						//m_motionType = MOTION_NEUTRAL;
						if (m_bJump == false)
						{
							SetCountLevel(m_motionType);
							//if (m_motionType == MOTION_COMBO1)
							//{
							//	pCommand->SetCommandState(pCommand->COMMAND_SKILL);
							//}
							//else
							{
								//コマンドを実行していない状態にする
								pCommand->SetCommandState(pCommand->COMMAND_END);
							}
						}
						m_bAction = false;
					}
				}
			}
			//カウントフレームを初期化
			m_nCountFrame = 0;
		}
	}

#ifdef _DEBUG
	CDebugProc::Print(1, "プレイヤー移動[W,A,S,D]\n");
	CDebugProc::Print(1, "Key[%d / %d]\n", m_nKey, m_apMotionInfo[m_motionType].nNumKey);
	CDebugProc::Print(1, "Frame[%d / %d]\n", m_nCountFrame, m_apMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame);
	CDebugProc::Print(1, "モーション[%d]\n", m_motionType);
	CDebugProc::Print(1, "カウンター総数[%d]\n", m_aMotionAttack[m_motionType].nNumFrame);

	switch (m_apMotionInfo[m_motionType].bLoop)
	{
	case false:
		CDebugProc::Print(1, "ループしない\n");
		break;
	case true:
		CDebugProc::Print(1, "ループする\n");
		break;
	}
	switch (m_bJump)
	{
	case false:
		CDebugProc::Print(1, "ジャンプしていない\n");
		break;
	case true:
		CDebugProc::Print(1, "ジャンプしている\n");
		break;
	}
	switch (m_bAction)
	{
	case false:
		CDebugProc::Print(1, "当たり判定OFF\n");

		break;
	case true:
		CDebugProc::Print(1, "当たり判定ON\n");
		break;
	}
#endif
}
//=============================================================================
// プレイヤーの行動状態を取得
//=============================================================================
bool CPlayer::GetAction(void)
{
	return m_bAction;
}

//=============================================================================
// プレイヤーの体力取得処理
//=============================================================================
int CPlayer::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
// プレイヤーのとくぎ取得処理
//=============================================================================
int CPlayer::GetSkill(void)
{
	return m_nSkill;
}

//=============================================================================
// プレイヤーの体力最大値取得処理
//=============================================================================
int CPlayer::GetMaxLife(void)
{
	return m_nMaxLife;
}

//=============================================================================
// プレイヤーのとくぎ最大値取得処理
//=============================================================================
int CPlayer::GetMaxSkill(void)
{
	return m_nMaxSkill;
}

//=============================================================================
// プレイヤーの行動状態の設置処理
//=============================================================================
void CPlayer::SetAction(bool action)
{
	m_bAction = action;
}

//=============================================================================
// プレイヤーの体力設置処理
//=============================================================================
void CPlayer::SetLife(int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
// プレイヤーのとくぎ設置処理
//=============================================================================
void CPlayer::SetSkill(int nSkill)
{
	m_nSkill = nSkill;
}

//=============================================================================
// プレイヤーの体力最大値設置処理
//=============================================================================
void CPlayer::SetMaxLife(int nMaxLife)
{
	m_nMaxLife = nMaxLife;
}

//=============================================================================
// プレイヤーの状態設置処理
//=============================================================================
void CPlayer::SetState(PLAYER_STATE state)
{
	m_state = state;
}

//=============================================================================
// プレイヤーのとくぎ量加算処理
//=============================================================================
void CPlayer::AddSkillLevel(int nSelect, int nValue)
{
	m_aSkillLevel[nSelect] += nValue;
}

//=============================================================================
// プレイヤーのどうぐ量加算処理
//=============================================================================
void CPlayer::AddItemQuantity(int nSelect, int nValue)
{
	m_aItemQuantity[nSelect] += nValue;
}

//=============================================================================
// プレイヤーのとくぎ量取得処理
//=============================================================================
int CPlayer::GetSkillLevel(int nSelect)
{
	return m_aSkillLevel[nSelect];
}

//=============================================================================
// プレイヤーのどうぐ量取得処理
//=============================================================================
int CPlayer::GetItemQuantity(int nSelect)
{
	return m_aItemQuantity[nSelect];
}

//=============================================================================
// モーションの設置処理
//=============================================================================
void CPlayer::SetMotion(MOTION_TYPE type)
{
	m_motionType = type;
}

//=============================================================================
//　ジャンプをしているかを取得
//=============================================================================
bool CPlayer::GetJump(void)
{
	return m_bJump;
}

//=============================================================================
//　モーション種類の取得
//=============================================================================
CPlayer::MOTION_TYPE CPlayer::GetMotion(void)
{
	return m_motionType;
}

CPlayer::PLAYER_STATE CPlayer::GetState(void)
{
	return m_state;
}

//=============================================================================
//　入力処理
//=============================================================================
void CPlayer::Input(void)
{
	float fDiffAngle;

	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();
	//ジョイパッドの取得
	CInputJoypad *pInputJoypad;
	pInputJoypad = CManager::GetJoypad();

	//カメラの取得
	CCamera *pCamera;
	pCamera = CManager::GetCamera();
	//カメラの向き取得
	D3DXVECTOR3 cameraRot = pCamera->GetRot();

	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	//コマンドの取得
	CCommand *pCommand;
	pCommand = CGame::GetCommad();

	//ステータス数字の取得
	CStatus_Number *pStatus_Number;
	pStatus_Number = CGame::GetStatusNumber();

	CUI_LifeGauge *pUI_Life;
	pUI_Life = CGame::GetUI_LifeGauge();

	CUI_SkillGauge *pUI_Skill;
	pUI_Skill = CGame::GetUI_SkillGauge();

	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();

	if (m_state != PLAYER_STATE_DEATH)
	{//死んでいない場合
		if (pCommand != NULL)
		{//コマンドがNULLではない場合
			if (pCommand->GetCommandState() == pCommand->COMMAND_NONE || pCommand->GetCommandState() == pCommand->COMMAND_END /*|| pCommand->GetCommandState() == pCommand->COMMAND_SKILL*/)
			{//コマンドが実行されていない場合

			 //パッドでの操作
				if (pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_LSTICK_UP) == true ||
					pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_LSTICK_DOWN) == true ||
					pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_LSTICK_LEFT) == true ||
					pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_LSTICK_RIGHT) == true)
				{
					if (m_bJump == false)
					{//ジャンプをしていない場合
					 //移動モーションに切り替え
						m_motionType = MOTION_MOVE;
					}
					//移動処理
					move.x -= sinf(cameraRot.y - pInputJoypad->GetLeftAxiz()) * 2.0f;
					move.z -= cosf(cameraRot.y - pInputJoypad->GetLeftAxiz()) * 2.0f;
					float fDest = (cameraRot.y - pInputJoypad->GetLeftAxiz());

					fDiffAngle = fDest - rot.y;
					SetDiffAngle(fDiffAngle);
				}
				else
				{
					m_motionType = MOTION_NEUTRAL;
				}

				//キーボードでの操作
				if (pInputKeyboard->GetPress(DIK_A) == true)
				{// キーボードの[A]キーが押された（左移動）
				 //移動モーションにする
					if (m_bJump == false)
					{//ジャンプをしていない場合
					 //移動モーションに切り替え
						m_motionType = MOTION_MOVE;
					}
					if (pInputKeyboard->GetPress(DIK_W) == true)
					{// キーボードの[W]キーが押された（左上移動）
						move.x += -sinf(cameraRot.y + D3DX_PI * 0.75f) * PLAYER_MOVE;
						move.z += -cosf(cameraRot.y + D3DX_PI * 0.75f) * PLAYER_MOVE;
						//rot.y = cameraRot.y + D3DX_PI * 0.75f;
						fDiffAngle = (cameraRot.y + D3DX_PI * 0.75f) - rot.y;
						SetDiffAngle(fDiffAngle);

					}
					else if (pInputKeyboard->GetPress(DIK_S) == true)
					{// キーボードの[S]キーが押された（左下移動）
						move.x += -sinf(cameraRot.y + D3DX_PI * 0.25f) * PLAYER_MOVE;
						move.z += -cosf(cameraRot.y + D3DX_PI * 0.25f) * PLAYER_MOVE;
						fDiffAngle = (cameraRot.y + D3DX_PI * 0.25f) - rot.y;
						SetDiffAngle(fDiffAngle);

					}
					else
					{// キーボードの[A]キーが押された（左移動）
						move.x += -sinf(cameraRot.y + D3DX_PI / 2) * PLAYER_MOVE;
						move.z += -cosf(cameraRot.y + D3DX_PI / 2) * PLAYER_MOVE;
						fDiffAngle = (cameraRot.y + D3DX_PI / 2) - rot.y;
						SetDiffAngle(fDiffAngle);

					}
				}
				else if (pInputKeyboard->GetPress(DIK_D) == true)
				{// キーボードの[D]キーが押された（右移動）
					if (m_bJump == false)
					{//ジャンプをしていない場合
					 //移動モーションに切り替え
						m_motionType = MOTION_MOVE;
					}

					if (pInputKeyboard->GetPress(DIK_W) == true)
					{// キーボードの[W]キーが押された（右上移動）
						move.x += -sinf(cameraRot.y + D3DX_PI * -0.75f) * PLAYER_MOVE;
						move.z += -cosf(cameraRot.y + D3DX_PI * -0.75f) * PLAYER_MOVE;
						//rot.y = cameraRot.y + D3DX_PI * -0.75f;
						fDiffAngle = (cameraRot.y + D3DX_PI * -0.75f) - rot.y;
						SetDiffAngle(fDiffAngle);

					}
					else if (pInputKeyboard->GetPress(DIK_S) == true)
					{// キーボードの[S]キーが押された（右下移動）
						move.x += -sinf(cameraRot.y + D3DX_PI * -0.25f) * PLAYER_MOVE;
						move.z += -cosf(cameraRot.y + D3DX_PI * -0.25f) * PLAYER_MOVE;
						//rot.y = cameraRot.y + D3DX_PI * -0.25f;
						fDiffAngle = (cameraRot.y + D3DX_PI * -0.25f) - rot.y;
						SetDiffAngle(fDiffAngle);

					}
					else
					{// キーボードの[D]キーが押された（右移動）
						move.x += -sinf(cameraRot.y + D3DX_PI / -2) * PLAYER_MOVE;
						move.z += -cosf(cameraRot.y + D3DX_PI / -2) * PLAYER_MOVE;
						//rot.y = cameraRot.y + D3DX_PI / -2;
						fDiffAngle = (cameraRot.y + D3DX_PI / -2) - rot.y;
						SetDiffAngle(fDiffAngle);

					}
				}
				else if (pInputKeyboard->GetPress(DIK_W) == true)
				{//キーボードの[W]キーが押された（前移動）
					if (m_bJump == false)
					{//ジャンプをしていない場合
					 //移動モーションに切り替え
						m_motionType = MOTION_MOVE;
					}
					move.x -= sinf(cameraRot.y + D3DX_PI * 1)*PLAYER_MOVE;
					move.z -= cosf(cameraRot.y + D3DX_PI * 1)*PLAYER_MOVE;
					//	rot.y = cameraRot.y + D3DX_PI * 1;
					fDiffAngle = (cameraRot.y + D3DX_PI) - rot.y;
					SetDiffAngle(fDiffAngle);

				}
				else if (pInputKeyboard->GetPress(DIK_S) == true)
				{//キーボードの[S]キーが押された（後移動）
					if (m_bJump == false)
					{//ジャンプをしていない場合
					 //移動モーションに切り替え
						m_motionType = MOTION_MOVE;
					}
					move.x += -sinf(cameraRot.y + D3DX_PI * 0)*PLAYER_MOVE;
					move.z += -cosf(cameraRot.y + D3DX_PI * 0)*PLAYER_MOVE;
					fDiffAngle = (cameraRot.y + D3DX_PI * 0) - rot.y;
					SetDiffAngle(fDiffAngle);

				}

			}
		}
	}
	SetMove(move);
}

//=============================================================================
// プレイヤーの移動処理
//=============================================================================
void CPlayer::Move(void)
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	pos = GetPos();
	move = GetMove();

	//重力
	move.y -= cosf(D3DX_PI * 0) * 0.5f;

	//移動加算処理
	//pos.y += move.y;

	pos += move;

	//慣性処理
	move.x += (0.0f - move.x) * 0.8f;
	move.z += (0.0f - move.z) * 0.8f;
	if (m_bJump == false)
	{//ジャンプしていない場合
	 //地面の高さ取得位置代入
		pos.y = CGame::GetMeshField()->GetHeight(pos);
		//移動量初期化
		move.y = 0.0f;
	}
	else
	{//ジャンプしている場合
		if (pos.y <= CGame::GetMeshField()->GetHeight(pos))
		{//地面についている場合
		 //ジャンプしていない状態にする
			m_bJump = false;
			//ニュートラルモーションにする
			m_motionType = MOTION_NEUTRAL;
			//移動量を初期化
			move.y = 0.0f;
		}
	}
	SetPos(pos);
	SetMove(move);
	CDebugProc::Print(1, "移動量ｙ %f\n", move.y);
}

//=============================================================================
// プレイヤーの当たり判定処理
//=============================================================================
void CPlayer::Collision(void)
{
	int nattack = 0;
	float fRange = 30.0f;
	float fWeaponRange = 15.0f;
	float fLength = 0.0f;
	float fHit = 0.0f;
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 posOld = GetPosOld();
	D3DXVECTOR3 move = GetMove();

	// 配置物と地面へのポインタ取得する
	CScene *pScene = NULL;               // オブジェクトのポインタ
	CScene *pSceneNext = NULL;           // 次のオブジェクトのポインタ
	CObject *pObject = NULL;             // 配置物のポインタ
	D3DXVECTOR3 Offset = D3DXVECTOR3(0.0f, 25.0f, 0.0f);
	D3DXVec3TransformCoord(&Offset, &Offset, &m_apModel[15]->GetMtxWorld());
	fHit = sqrtf((fRange + fWeaponRange) * (fRange + fWeaponRange));

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULLになるまで繰り返す
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_OBJECT)
			{// 配置物だったら
				pObject = (CObject*)pScene;
				if (pObject != NULL)
				{// 配置物が取得できた
					if (pObject->Collision(&pos, &posOld, &move) == true)
					{// 配置物の上に乗っている
						m_bJump = false;
					}
				}
			}
			if (pScene->GetObjType() == OBJTYPE_ENEMY ||
				pScene->GetObjType() == OBJTYPE_BIRD ||
				pScene->GetObjType() == OBJTYPE_FROG)
			{
				CEnemy *pEnemy = (CEnemy*)pScene;
				if (pEnemy != NULL)
				{// 配置物が取得できた
					if (m_bAction == true)
					{

						D3DXVECTOR3 EnemeyPos, EnemyMove;
						EnemeyPos = pEnemy->GetPos();
						EnemyMove = pEnemy->GetMove();
						fLength = sqrtf((Offset.x - EnemeyPos.x) * (Offset.x - EnemeyPos.x) + (Offset.y - EnemeyPos.y) * (Offset.y - EnemeyPos.y) + (Offset.z - EnemeyPos.z) * (Offset.z - EnemeyPos.z));

						if (fHit >= fLength)
						{
							int nDamage = rand() % 1 + 3;
							pEnemy->HitDamage(nDamage);
							float fAttackAngle = atan2f(EnemeyPos.x - pos.x, EnemeyPos.z - pos.z);// 当たった角度を計算
							EnemyMove.x = sinf((D3DX_PI *  fAttackAngle)) * -0.5f;
							EnemyMove.z = cosf((D3DX_PI *  fAttackAngle)) * 0.5f;
							if (pEnemy->GetObjType() == OBJTYPE_BIRD)
							{
								pEnemy->SetMove(EnemyMove);
							}
							nattack = 1;
							m_bAction = false;
						}

					}
					else
					{
						/*if (pEnemy->Collision(&pos, &posOld, &move, GetColRange()) == true)
						{

						}*/
					}
				}
			}
			// 次のオブジェクトへのポインタを取得
			pScene = pSceneNext;
		}
	}
	SetPos(pos);
	CDebugProc::Print(1, "当たっているかどうか%d\n", nattack);
}

//=============================================================================
// プレイヤーのモーションカウンター設置取得
//=============================================================================
void CPlayer::SetCountLevel(MOTION_TYPE type)
{
	//とくぎのレベルカウンター加算処理
	switch (type)
	{
	case MOTION_ICRBRAKE:
		m_aCntLevel[0]++;
		break;
	case MOTION_REVOSLASH:
		m_aCntLevel[1]++;
		break;
	case MOTION_FULLSWING:
		m_aCntLevel[2]++;
		break;
	}

	//カウンターをもとにレベルを変更処理
	for (int nCntLevel = 0; nCntLevel < MAX_SELECT; nCntLevel++)
	{
		switch (m_aCntLevel[nCntLevel])
		{
		case 1:
			m_aSkillLevel[nCntLevel] = 2;
			m_aCutSkill[nCntLevel] = 2;
			break;
		case 2:
			m_aSkillLevel[nCntLevel] = 3;
			m_aCutSkill[nCntLevel] = 3;
			break;
		case 4:
			m_aSkillLevel[nCntLevel] = 4;
			m_aCutSkill[nCntLevel] = 3;
			break;
		case 6:
			m_aSkillLevel[nCntLevel] = 5;
			m_aCutSkill[nCntLevel] = 4;
			break;
		case 8:
			m_aSkillLevel[nCntLevel] = 6;
			m_aCutSkill[nCntLevel] = 4;
			break;
		case 10:
			m_aSkillLevel[nCntLevel] = 7;
			m_aCutSkill[nCntLevel] = 5;
			break;
		case 12:
			m_aSkillLevel[nCntLevel] = 8;
			m_aCutSkill[nCntLevel] = 5;
			break;
		case 14:
			m_aSkillLevel[nCntLevel] = 9;
			m_aCutSkill[nCntLevel] = 6;

			break;
		case 16:
			m_aSkillLevel[nCntLevel] = 10;
			m_aCutSkill[nCntLevel] = 6;
			break;
		}
	}
}
//=============================================================================
// プレイヤーの武器大きさ切り替え処理
//=============================================================================
void CPlayer::SwitchWeaponSize(void)
{
	float fData = (float)m_aSkillLevel[2] / 5.0f;
	m_apModel[15]->SetSize(D3DXVECTOR3(1.0f + fData, 1.0f + fData, 1.0f + fData));

	for (int nCntParticle = 0; nCntParticle < 5 * fData; nCntParticle++)
	{
		m_fSpin += 50.0f;
		float fAngle = (rand() % 628 * 0.01f) - D3DX_PI;
		D3DXVECTOR3 movePar(0.0f, 0.0f, 0.0f);
		movePar.x = sinf(fAngle) * (rand() % 10 * 0.75f);
		movePar.y = (rand() % 10 * 0.75f);
		movePar.z = cosf(fAngle) * (rand() % 10 * 0.75f);
		CDamageParticle::Create(D3DXVECTOR3(m_apModel[15]->GetMtxWorld()._41, m_apModel[15]->GetMtxWorld()._42, m_apModel[15]->GetMtxWorld()._43), movePar, D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f), rand() % 10 + 10.0f, 25);
	}
}

//=============================================================================
// プレイヤーの武器大きさ切り替え処理
//=============================================================================
void CPlayer::SwitchIceSize(void)
{
	D3DXVECTOR3 pos = GetPos();
	float fData = (float)m_aSkillLevel[0] / 5.0f;
	if (m_aSkillLevel[0] >= 0 && m_aSkillLevel[0] <= 4)
	{
		CIceCircle::Create(D3DXVECTOR3(pos.x, 2.0f, pos.z));
		float fAngle = D3DX_PI;	//円周率
		float fRadius = 40.0f;	//半径
		D3DXVECTOR3 ParPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//パーティクルの位置
		if (fAngle > D3DX_PI)
		{
			fAngle -= -D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{
			fAngle += D3DX_PI * 2.0f;
		}

		for (int nCntRad = 0; nCntRad < 3; nCntRad++)
		{
			for (int nCntPar = 0; nCntPar < 10 * fData; nCntPar++)
			{
				ParPos = D3DXVECTOR3(sinf(fAngle) * fRadius, 0.0f, cosf(fAngle) * fRadius);

				CSkill2Particle::Create(D3DXVECTOR3(ParPos.x + pos.x, 0.0f, ParPos.z + pos.z),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					rand() % 10 + 5.0f,
					50);

				fAngle += 15.0f;
			}
			fRadius += 30.0f;
		}
	}
	else if (m_aSkillLevel[0] >= 5 && m_aSkillLevel[1] <= 10)
	{
		CIcicle::Create(pos, CIcicle::ICICLE_RIGIHT);
		CIcicle::Create(pos, CIcicle::ICICLE_LEFT);

		CIceCircle::Create(D3DXVECTOR3(pos.x, 2.0f, pos.z));
		float fAngle = D3DX_PI;	//円周率
		float fRadius = 40.0f;	//半径
		D3DXVECTOR3 ParPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//パーティクルの位置
		if (fAngle > D3DX_PI)
		{
			fAngle -= -D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{
			fAngle += D3DX_PI * 2.0f;
		}

		for (int nCntRad = 0; nCntRad < 3; nCntRad++)
		{
			for (int nCntPar = 0; nCntPar < 5 * fData; nCntPar++)
			{
				ParPos = D3DXVECTOR3(sinf(fAngle) * fRadius, 0.0f, cosf(fAngle) * fRadius);

				CSnowParticle::Create(D3DXVECTOR3(ParPos.x + pos.x, 150.0f, ParPos.z + pos.z),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					rand() % 10 + 5.0f,
					150);

				fAngle += 10.0f;
			}
			fRadius += 30.0f;
		}
	}
}
//=============================================================================
// プレイヤーのダメージ処理
//=============================================================================
void CPlayer::HitDamage(int nDamage)
{
	D3DXVECTOR3 pos = GetPos();

	CCommand::COMMAND_STATE state;
	state = CGame::GetCommad()->GetCommandState();
	if (m_state != PLAYER_STATE_DAMAGE &&state != CCommand::COMMAND_PROCESS)
	{
		//ステータス数字の取得
		CStatus_Number *pStatus_Number;
		pStatus_Number = CGame::GetStatusNumber();

		CUI_LifeGauge *pUI_Life;
		pUI_Life = CGame::GetUI_LifeGauge();

		CUI_SkillGauge *pUI_Skill;
		pUI_Skill = CGame::GetUI_SkillGauge();
		m_nLife -= nDamage;
		pUI_Life->CutLifeGauge(-1);
		pStatus_Number->AddStatus_Number(m_nLife, 0);

		if (m_nLife <= 0)
		{
			m_nLife = 0;
			SetMotion(MOTION_DEATH);
		}
		else
		{
			m_state = PLAYER_STATE_DAMAGE;
		}
		//サウンドの取得
		CSound *pSound;
		pSound = CManager::GetSound();
		pSound->PlaySound(CSound::SOUND_LABEL_SE_DAMAGE);
		for (int nCntParticle = 0; nCntParticle < 15; nCntParticle++)
		{
			m_fSpin += 50.0f;
			float fAngle = (rand() % 628 * 0.01f) - D3DX_PI;
			D3DXVECTOR3 movePar(0.0f, 0.0f, 0.0f);
			movePar.x = sinf(fAngle) * (rand() % 10 * 0.75f);
			movePar.y = (rand() % 10 * 0.75f);
			movePar.z = cosf(fAngle) * (rand() % 10 * 0.75f);

			CDamageParticle::Create(pos, movePar, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), rand() % 10 + 10.0f, 25);
		}
	}

}

//=============================================================================
// プレイヤーのどうぐ設置処理
//=============================================================================
void CPlayer::SetUseItem(USE_ITEM item)
{
	m_useItem = item;
}

//=============================================================================
// プレイヤーのどうぐ使用処理
//=============================================================================
void CPlayer::UseItem(void)
{
	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_SE_CURE);

	for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
	{
		m_fSpin += 50.0f;
		float fAngle = (rand() % 628 * 0.01f) - D3DX_PI;
		D3DXVECTOR3 movePar(0.0f, 0.0f, 0.0f);
		movePar.x = sinf(fAngle) * (rand() % 10 * 0.75f);
		movePar.y = (rand() % 10 * 0.75f);
		movePar.z = cosf(fAngle) * (rand() % 10 * 0.75f);
		CDamageParticle::Create(D3DXVECTOR3(m_apModel[8]->GetMtxWorld()._41, m_apModel[8]->GetMtxWorld()._42, m_apModel[8]->GetMtxWorld()._43), movePar, D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f), rand() % 3 + 5.0f, 25);
	}

	switch (m_useItem)
	{
	case USE_ITEM0:
		m_nLife += 50;
		if (m_nLife >= 100)
		{
			m_nLife = 100;
		}
		break;
	case USE_ITEM1:
		m_nSkill += 20;
		if (m_nSkill >= 40)
		{
			m_nSkill = 40;
		}
		break;
	case USE_ITEM2:
		m_nLife += 50;
		m_nSkill += 20;

		if (m_nLife >= 100)
		{
			m_nLife = 100;
		}
		if (m_nSkill >= 40)
		{
			m_nSkill = 40;
		}
		break;
	}
}
//=============================================================================
// プレイヤーのスキル減産量取得処理
//=============================================================================
int CPlayer::GetCutSkill(int nSkill)
{
	return m_aCutSkill[nSkill];
}

//=============================================================================
// プレイヤーのダメージ処理
//=============================================================================
void CPlayer::IceBreak(void)
{
	// 配置物と地面へのポインタ取得する
	CScene *pScene = NULL;               // オブジェクトのポインタ
	CScene *pSceneNext = NULL;           // 次のオブジェクトのポインタ
	CObject *pObject = NULL;             // 配置物のポインタ
	D3DXVECTOR3 pos = GetPos();

	//CIceCircle::Create(D3DXVECTOR3(pos.x, 2.0f, pos.z));
	float fAngle = D3DX_PI;	//円周率
	float fRadius = 10.0f;	//半径
	D3DXVECTOR3 ParPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//パーティクルの位置

	for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
	{
		m_fSpin += 50.0f;
		float fAngle = (rand() % 628 * 0.01f) - D3DX_PI;
		D3DXVECTOR3 movePar(0.0f, 0.0f, 0.0f);
		movePar.x = sinf(fAngle) * (rand() % 10 * 1.0f);
		movePar.y = (rand() % 30 * 0.75f);
		movePar.z = cosf(fAngle) * (rand() % 10 * 1.0f);
		CFountainParticle::Create(pos, movePar, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), rand() % 10 + 10.0f, 25);
	}
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULLになるまで繰り返す
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_ENEMY ||
				pScene->GetObjType() == OBJTYPE_BIRD ||
				pScene->GetObjType() == OBJTYPE_FROG)
			{
				CEnemy *pEnemy = (CEnemy*)pScene;

				if (pEnemy != NULL)
				{// 配置物が取得できた
					D3DXVECTOR3 EnemeyPos, EnemyMove;
					EnemeyPos = pEnemy->GetPos();
					EnemyMove = pEnemy->GetMove();

					float fLength = powf((EnemeyPos.x - pos.x)*(EnemeyPos.x - pos.x) + (EnemeyPos.z - pos.z)*(EnemeyPos.z - pos.z), 0.5f);	//プレイヤーとの距離
					float fHit = powf(12.0f, 2.0f);	//半径

					if (fHit > fLength)
					{
						int nData = (int)m_aSkillLevel[0];
						nData = rand() % 2 + nData;
						pEnemy->HitDamage(nData);
						float fAttackAngle = atan2f(EnemeyPos.x - pos.x, EnemeyPos.z - pos.z);// 当たった角度を計算
						EnemyMove.x = sinf((D3DX_PI *  fAttackAngle)) * 25.0f;
						EnemyMove.z = cosf((D3DX_PI *  fAttackAngle)) * 25.0f;
						if (pEnemy->GetObjType() == OBJTYPE_BIRD)
						{
							pEnemy->SetMove(EnemyMove);
						}
					}

				}
			}
			// 次のオブジェクトへのポインタを取得
			pScene = pSceneNext;

		}
	}
}

//=============================================================================
// プレイヤーのダメージ処理
//=============================================================================
void CPlayer::RevoSlash(void)
{
	// 配置物と地面へのポインタ取得する
	CScene *pScene = NULL;               // オブジェクトのポインタ
	CScene *pSceneNext = NULL;           // 次のオブジェクトのポインタ
	CObject *pObject = NULL;             // 配置物のポインタ
	D3DXVECTOR3 pos = GetPos();

	float fData = (float)m_aSkillLevel[1] / 5.0f;

	for (int nCntParticle = 0; nCntParticle < 5 * fData; nCntParticle++)
	{
		m_fSpin += 50.0f;
		float fAngle = (rand() % 628 * 0.01f) - D3DX_PI;
		D3DXVECTOR3 movePar(0.0f, 0.0f, 0.0f);
		movePar.x = sinf(fAngle) * (rand() % 10 * 0.75f);
		movePar.y = (rand() % 10 * 0.0f);
		movePar.z = cosf(fAngle) * (rand() % 10 * 0.75f);
		CDamageParticle::Create(D3DXVECTOR3(m_apModel[15]->GetMtxWorld()._41, m_apModel[15]->GetMtxWorld()._42, m_apModel[15]->GetMtxWorld()._43), movePar, D3DXCOLOR(rand() % 10 * 0.5f, rand() % 10 * 0.5f, 1.0f, 1.0f), rand() % 3 + 15.0f, 25);
	}

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULLになるまで繰り返す
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_ENEMY ||
				pScene->GetObjType() == OBJTYPE_BIRD ||
				pScene->GetObjType() == OBJTYPE_FROG)
			{
				CEnemy *pEnemy = (CEnemy*)pScene;

				if (pEnemy != NULL)
				{// 配置物が取得できた
					D3DXVECTOR3 EnemeyPos, EnemyMove;
					EnemeyPos = pEnemy->GetPos();
					EnemyMove = pEnemy->GetMove();
					float fLength = powf((EnemeyPos.x - pos.x)*(EnemeyPos.x - pos.x) + (EnemeyPos.z - pos.z)*(EnemeyPos.z - pos.z), 0.5f);	//プレイヤーとの距離
					float fHit = powf(8.0f, 2.0f);	//半径

					if (fHit > fLength)
					{
						int nData = (int)m_aSkillLevel[1];
						nData = rand() % 3 + nData;
						pEnemy->HitDamage(nData);
						float fAttackAngle = atan2f(EnemeyPos.x - pos.x, EnemeyPos.z - pos.z);// 当たった角度を計算
						EnemyMove.x = sinf((D3DX_PI *  fAttackAngle)) * 25.0f;
						EnemyMove.z = cosf((D3DX_PI *  fAttackAngle)) * 25.0f;
						if (pEnemy->GetObjType() == OBJTYPE_BIRD)
						{
							pEnemy->SetMove(EnemyMove);
						}
					}

				}
			}
			// 次のオブジェクトへのポインタを取得
			pScene = pSceneNext;

		}
	}
}

//=============================================================================
// プレイヤーのダメージ処理
//=============================================================================
void CPlayer::FullSwing(void)
{
	// 配置物と地面へのポインタ取得する
	CScene *pScene = NULL;               // オブジェクトのポインタ
	CScene *pSceneNext = NULL;           // 次のオブジェクトのポインタ
	CObject *pObject = NULL;             // 配置物のポインタ
	D3DXVECTOR3 pos = GetPos();

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULLになるまで繰り返す
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_ENEMY ||
				pScene->GetObjType() == OBJTYPE_BIRD ||
				pScene->GetObjType() == OBJTYPE_FROG)
			{
				CEnemy *pEnemy = (CEnemy*)pScene;

				if (pEnemy != NULL)
				{// 配置物が取得できた
					D3DXVECTOR3 EnemeyPos, EnemyMove;
					EnemeyPos = pEnemy->GetPos();
					EnemyMove = pEnemy->GetMove();
					float fLength = powf((EnemeyPos.x - pos.x)*(EnemeyPos.x - pos.x) + (EnemeyPos.z - pos.z)*(EnemeyPos.z - pos.z), 0.75f);	//プレイヤーとの距離
					float fHit = powf(6.0f, 4.0f);	//半径

					if (fHit > fLength)
					{
						int nData = (int)m_aSkillLevel[2];
						nData = 7;
						pEnemy->HitDamage(nData);
						float fAttackAngle = atan2f(EnemeyPos.x - pos.x, EnemeyPos.z - pos.z);// 当たった角度を計算
						EnemyMove.x = sinf((D3DX_PI *  fAttackAngle)) * 25.0f;
						EnemyMove.z = cosf((D3DX_PI *  fAttackAngle)) * 25.0f;
						if (pEnemy->GetObjType() == OBJTYPE_BIRD)
						{
							pEnemy->SetMove(EnemyMove);
						}

					}

				}
			}
			// 次のオブジェクトへのポインタを取得
			pScene = pSceneNext;

		}
	}
}
//=============================================================================
// 鳥敵の死亡数加算処理
//=============================================================================
void CPlayer::AddDeathBird(int nValue)
{
	m_nNumDeathBird += nValue;
}

//=============================================================================
// かえる敵の死亡数加算処理
//=============================================================================
void CPlayer::AddDeathFrog(int nValue)
{
	m_nNumDeathFrog += nValue;
}

//=============================================================================
// 鳥敵の死亡数加算処理
//=============================================================================
int CPlayer::GetDeathBird(void)
{
	return m_nNumDeathBird;
}

//=============================================================================
// かえる敵の死亡数取得処理
//=============================================================================
int CPlayer::GetDeathFrog(void)
{
	return m_nNumDeathFrog;
}

//=============================================================================
// 攻撃の当たり判定処理
//=============================================================================
bool CPlayer::HitAttack(void)
{
	int nattack = 0;
	float fRange = 30.0f;
	float fWeaponRange = 15.0f;
	float fLength = 0.0f;
	float fHit = 0.0f;
	bool bHit = false;
	D3DXVECTOR3 pos = GetPos();

	// 配置物と地面へのポインタ取得する
	CScene *pScene = NULL;               // オブジェクトのポインタ
	CScene *pSceneNext = NULL;           // 次のオブジェクトのポインタ
	CObject *pObject = NULL;             // 配置物のポインタ
	D3DXVECTOR3 Offset = D3DXVECTOR3(0.0f, 25.0f, 0.0f);
	D3DXVec3TransformCoord(&Offset, &Offset, &m_apModel[15]->GetMtxWorld());
	fHit = sqrtf((fRange + fWeaponRange) * (fRange + fWeaponRange));

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULLになるまで繰り返す
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_ENEMY ||
				pScene->GetObjType() == OBJTYPE_BIRD ||
				pScene->GetObjType() == OBJTYPE_FROG)
			{
				CEnemy *pEnemy = (CEnemy*)pScene;
				if (pEnemy != NULL)
				{// 配置物が取得できた
					if (m_bAction == true)
					{

						D3DXVECTOR3 EnemeyPos, EnemyMove;
						EnemeyPos = pEnemy->GetPos();
						EnemyMove = pEnemy->GetMove();
						fLength = sqrtf((Offset.x - EnemeyPos.x) * (Offset.x - EnemeyPos.x) + (Offset.y - EnemeyPos.y) * (Offset.y - EnemeyPos.y) + (Offset.z - EnemeyPos.z) * (Offset.z - EnemeyPos.z));

						if (fHit >= fLength)
						{
							int nDamage = rand() % 1 + 3;
							pEnemy->HitDamage(nDamage);
							float fAttackAngle = atan2f(EnemeyPos.x - pos.x, EnemeyPos.z - pos.z);// 当たった角度を計算
							EnemyMove.x = sinf((D3DX_PI *  fAttackAngle)) * -25.0f;
							EnemyMove.z = cosf((D3DX_PI *  fAttackAngle)) * 25.0f;
							if (pEnemy->GetObjType() == OBJTYPE_BIRD)
							{
								pEnemy->SetMove(EnemyMove);
								bHit = true;
							}
							m_nSkill += 1;
							if (m_nSkill >= 40)
							{
								m_nSkill = 40;
							}
							nattack = 1;
							m_bAction = false;
						}

					}
				}
			}
			// 次のオブジェクトへのポインタを取得
			pScene = pSceneNext;
		}
	}

	return bHit;
}

//=============================================================================
// プレイヤーのモーション読み込み
//=============================================================================
void CPlayer::LoadMotion(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXVECTOR3 pos = GetPos();

	int nCntAttack = 0;
	FILE *pFile;	//ファイルのポインタ

	pFile = fopen(MOTION_TXT_NAME, "r");	//テキスト読み込み
	if (pFile != NULL)
	{//ファイルが開けた場合
		char *pStrCur;						//文字列の先頭へのポインタ
		int nCntModel = 0;					//モデルカウンター
		int nCntMotion = 0;
		char *pStr = NULL;
		char *pLine = NULL;

		pStr = new char[256];
		pLine = new char[256];

		//************************************************************************
		// モデルの総数&ファイルの読み込み
		//************************************************************************
		while (1)
		{//モデルファイルを読み込むまで繰り返し
			pStrCur = CLoadFileFunction::ReadLine(pFile, pLine);
			strcpy(pStr, pStrCur);				//文字列を取り出す

			if (CLoadFileFunction::Memcmp(pStrCur, TEXT_NUM_MODEL) == 0)
			{//テキストがモデルの総数を示す行の場合
				strcpy(pStr, pStrCur);
				g_nNumModel = CLoadFileFunction::ReadInt(pStrCur, TEXT_NUM_MODEL);	//モデルの総数に代入
			}
			else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_FILENAME_MODEL) == 0)
			{//モデルファイル名を示す行の場合
				if (nCntModel < g_nNumModel)
				{
					pStr = CLoadFileFunction::ReadString(pStrCur, pStr, TEXT_FILENAME_MODEL);
					strcpy(&g_aFilenameModel[nCntModel][0], pStr);	//モデルの名前を取得

																	// Xファイルの読み込み
					D3DXLoadMeshFromX(&g_aFilenameModel[nCntModel][0],
						D3DXMESH_SYSTEMMEM,
						pDevice,
						NULL,
						&m_pBuffMat[nCntModel],
						NULL,
						&m_nNumMat[nCntModel],
						&m_pMesh[nCntModel]);
					nCntModel++;
				}
			}
			else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_CHARASET) == 0)
			{//キャラクター設置情報を示す行の場合
				strcpy(pStr, pStrCur);								//文字列を取り出す
				ReadCharacterSet(pStrCur, pFile, pLine);
			}
			else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_MOTIONSET) == 0)
			{//モーション情報設置を示す行の場合
				if (nCntMotion < MOTION_MAX)
				{
					ReadMotionSet(pStrCur, pFile, pLine, nCntMotion);
					nCntMotion++;
				}
			}
			else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_END_SCRIPT) == 0)
			{//スクリプト終了を示す行の場合
				break;
			}
		}
		if (pStr != NULL)
		{
			delete[] pStr;
			pStr = NULL;
		}
		if (pLine != NULL)
		{
			delete[] pLine;
			pLine = NULL;
		}
	}
	fclose(pFile);	//ファイルを閉じる
}

//=============================================================================
// キャラクター設置情報の読み込み処理
//=============================================================================
void CPlayer::ReadCharacterSet(char * pStrCur, FILE * pFile, char * pLine)
{
	int nCntParts = 0;
	int nIdx = 0;

	while (1)
	{
		pStrCur = CLoadFileFunction::ReadLine(pFile, pLine);
		if (CLoadFileFunction::Memcmp(pStrCur, TEXT_NUM_PARTS) == 0)
		{//パーツの総数を示す行の場合
			m_nNumParts = CLoadFileFunction::ReadInt(pStrCur, TEXT_NUM_PARTS);
		}
		else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_PARTSSET) == 0)
		{
			if (nCntParts < m_nNumParts)
			{
				while (1)
				{
					pStrCur = CLoadFileFunction::ReadLine(pFile, pLine);
					if (CLoadFileFunction::Memcmp(pStrCur, TEXT_INDEX) == 0)
					{//テキストがインデックスを示す行の場合
						nIdx = CLoadFileFunction::ReadInt(pStrCur, TEXT_INDEX);
					}
					else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_PARENT) == 0)
					{//テキストが親子関係を示す行の場合
						m_nIndexParent[nIdx] = CLoadFileFunction::ReadInt(pStrCur, TEXT_PARENT);
					}
					else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_POS) == 0)
					{//テキストが位置を示す行の場合
						m_aKeyOffset[nCntParts].pos = CLoadFileFunction::ReadVector3(pStrCur, TEXT_POS);
					}
					else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_ROT) == 0)
					{//テキストが位置を示す行の場合
						m_aKeyOffset[nCntParts].rot = CLoadFileFunction::ReadVector3(pStrCur, TEXT_ROT);

					}
					else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_END_PARTSSET) == 0)
					{//テキストがパーツの設置終了を示す行の場合
						if (m_nIndexParent[nCntParts] == -1)
						{
							//m_apModel[nIdx] = CModel::Create(D3DXVECTOR3(pos.x + m_aKeyOffset[nIdx].pos.x, pos.y + m_aKeyOffset[nIdx].pos.y, pos.z + m_aKeyOffset[nIdx].pos.z));
							m_apModel[nIdx] = CModel::Create(D3DXVECTOR3(m_aKeyOffset[nIdx].pos));
							m_apModel[nIdx]->BindX(m_pMesh[nIdx], m_pBuffMat[nIdx], m_nNumMat[nIdx]);
							m_apModel[nIdx]->SetParent(NULL);
							//m_offsetPos[nIdx] = m_apModel[nIdx]->GetPos();
						}
						else if (m_nIndexParent[nCntParts] > -1)
						{
							//m_apModel[nIdx] = CModel::Create(D3DXVECTOR3(pos.x + m_aKeyOffset[nIdx].pos.x, pos.y + m_aKeyOffset[nIdx].pos.y, pos.z + m_aKeyOffset[nIdx].pos.z));
							m_apModel[nIdx] = CModel::Create(D3DXVECTOR3(m_aKeyOffset[nIdx].pos));
							m_apModel[nIdx]->BindX(m_pMesh[nIdx], m_pBuffMat[nIdx], m_nNumMat[nIdx]);
							m_apModel[nIdx]->SetParent(m_apModel[m_nIndexParent[nIdx]]);
						}
						m_offsetPos[nIdx] = m_apModel[nIdx]->GetPos();
						break;
					}
				}
				nCntParts++;	//パーツカウンター加算
			}
		}
		else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_END_CHARASET) == 0)
		{//キャラクター設置情報終了を示す行の場合
			break;
		}
	}
}

//=============================================================================
// モーション設置情報の読み込み処理
//=============================================================================
void CPlayer::ReadMotionSet(char * pStrCur, FILE * pFile, char * pLine, int nCntMotion)
{
	int nCntKeySet = 0;
	int nCntKey = 0;
	while (1)
	{
		pStrCur = CLoadFileFunction::ReadLine(pFile, pStrCur);
		if (CLoadFileFunction::Memcmp(pStrCur, TEXT_NUM_KEY) == 0)
		{//キー数を示す行の場合
			m_apMotionInfo[nCntMotion].nNumKey = CLoadFileFunction::ReadInt(pStrCur, TEXT_NUM_KEY);	//キーの総数を代入
		}
		if (CLoadFileFunction::Memcmp(pStrCur, TEXT_LOOP) == 0)
		{//ループを示す行の場合
			m_apMotionInfo[nCntMotion].bLoop = CLoadFileFunction::ReadBool(pStrCur, TEXT_LOOP);
		}
		else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_COLLISION) == 0)
		{//当たり判定を示す行の場合
			pStrCur += strlen(TEXT_COLLISION);								//頭出し
			pStrCur = CLoadFileFunction::GetLineTop(pStrCur);				//空白消去
			int nWord = 0;

			m_aMotionAttack[nCntMotion].nAttackStartFrame = atoi(pStrCur);	//X座標の代入

			nWord = CLoadFileFunction::PopString(pStrCur, pStrCur);		//文字数カウント

			pStrCur += nWord;												//カウントされた分頭出し
			m_aMotionAttack[nCntMotion].nAttackEndFrame = atoi(pStrCur);	//X座標の代入
		}
		else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_KEYSET) == 0)
		{
			if (nCntKeySet < m_apMotionInfo[nCntMotion].nNumKey)
			{//キー設置カウンターがキー数より小さい場合
				int nCntKey = 0;

				while (1)
				{
					pStrCur = CLoadFileFunction::ReadLine(pFile, pStrCur);

					if (CLoadFileFunction::Memcmp(pStrCur, TEXT_FRAME) == 0)
					{//フレーム数を示す行の場合
						m_apMotionInfo[nCntMotion].aKeyInfo[nCntKeySet].nFrame = CLoadFileFunction::ReadInt(pStrCur, TEXT_FRAME);	//フレームの代入
						m_aMotionAttack[nCntMotion].nNumFrame += m_apMotionInfo[nCntMotion].aKeyInfo[nCntKeySet].nFrame;
					}
					else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_KEY) == 0)
					{//キーの開始を示す行の場合
						if (nCntKey < m_nNumParts)
						{
							while (1)
							{
								pStrCur = CLoadFileFunction::ReadLine(pFile, pLine);	//行の読み込み

								if (CLoadFileFunction::Memcmp(pStrCur, TEXT_POS) == 0)
								{//位置の情報を示す行の場合
									m_apMotionInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].pos = CLoadFileFunction::ReadVector3(pStrCur, TEXT_POS);

								}
								else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_ROT) == 0)
								{//向きの情報を示す行の場合
									m_apMotionInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].rot = CLoadFileFunction::ReadVector3(pStrCur, TEXT_ROT);
								}
								else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_END_KEY) == 0)
								{//キーの終了を示す行の場合
									nCntKey++;
									break;
								}
							}
						}
					}
					else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_END_KEYSET) == 0)
					{//キー設置情報終了を示す行の場合
						nCntKeySet++;
						break;
					}
				}
			}
		}
		else if (CLoadFileFunction::Memcmp(pStrCur, TEXT_END_MOTIONSET) == 0)
		{//モーション設置情報終了を示す行の場合
			break;
		}
	}
}