//=============================================================================
//
// 敵の処理 [player.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "enemy.h"
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
#include "player.h"
#include "loadfilefunction.h"
#include "particle.h"
#include "score.h"
#include "billboardgauge.h"
#include "bullet.h"
#include "command.h"

#define MOTION_TXT_NAME "data\\TEXT\\motionEnemy.txt"

//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPD3DXMESH			CEnemy::m_pMesh[MAX_ENEMY_MODEL] = {};		//メッシュのポインタ
LPD3DXBUFFER		CEnemy::m_pBuffMat[MAX_ENEMY_MODEL] = {};	//マテリアルの情報
DWORD				CEnemy::m_nNumMat[MAX_ENEMY_MODEL] = {};	//マテリアルの数
int					CEnemy::m_nNumEnemy = 0;					//敵の総数
char				CEnemy::m_aFilenameModel[MAX_ENEMY_MODEL][256] = {};		//モデルファイル

//CBillboardLifeGauge *CEnemy::m_pBillboardLifeGauge = NULL;

//=============================================================================
// 敵のコンストラクタ
//=============================================================================
CEnemy::CEnemy(int nPriority ,OBJTYPE objtype) : CCharacter(nPriority, objtype)
{
	m_nNumEnemy ++;
	m_nNumModel = 0;
	m_bJump = false;								//ジャンプしていない状態
	m_bAction = false;								//行動していない状態
	m_motionType = MOTION_NEUTRAL;					//モーションを初期化
	m_nCountFrame = 0;								//フレームの初期値
	m_nCounter = 0;
	m_fDestAngle = 0.0f;
	for (int nCntModel = 0; nCntModel < MAX_ENEMY_MODEL; nCntModel++)
	{//モデルの数だけ繰り返し
		m_apModel[nCntModel] = NULL;
	}
	for (int nCntMotion = 0; nCntMotion < MAX_ENEMY_MOTION; nCntMotion++)
	{//モデルの数だけ繰り返し
		m_aMotionAttack[nCntMotion].nAttackEndFrame = 0;
		m_aMotionAttack[nCntMotion].nAttackStartFrame = 0;
		m_aMotionAttack[nCntMotion].nNumFrame = 0;
	}
}
//=============================================================================
// 敵のデストラクタ
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// オブジェクトのテクスチャ読み込み
//=============================================================================
HRESULT CEnemy::Load(void)
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
void CEnemy::Unload(void)
{
	for (int nCntMesh = 0; nCntMesh < MAX_ENEMY_MODEL; nCntMesh++)
	{//モデルの数だけ繰り返し
	 // メッシュの開放
		if (m_pMesh[nCntMesh] != NULL)
		{
			m_pMesh[nCntMesh]->Release();
			m_pMesh[nCntMesh] = NULL;
		}
	}

	for (int nCntMat = 0; nCntMat < MAX_ENEMY_MODEL; nCntMat++)
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
//敵の生成
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos)
{
	CEnemy *pEnemy = NULL;					//敵のポインタ
	if (pEnemy == NULL)
	{
		pEnemy = new CEnemy;				//シーンの動的確保
		pEnemy->Init();						//初期化処理
		pEnemy->SetPos(pos);				//位置の代入
	}
	return pEnemy;							//値を返す
}

//=============================================================================
// 敵の初期化処理
//=============================================================================
HRESULT CEnemy::Init(void)
{
	SetLife(10);								//体力の初期化
	SetMaxLife(MAX_ENEMY_LIFE);
	SetRot(INITIALIZE_D3DXVECTOR3);				//向きの初期化
	SetMove(INITIALIZE_D3DXVECTOR3);			//移動量初期値
	SetColRange(5.0f);
	m_bJump = false;							//ジャンプしていない状態
	m_nKey = 0;									//現在のキーを初期化
	m_nNumKey = 2;								//キーの総数を初期化
	m_nCountFrame = 0;							//カウントフレームを初期化
	m_nCounter = 0;								//カウンターの初期化
	m_state = ENEMY_STATE_NOMAL;				//通常状態にする
	m_nCntDamage = 0;							//ダメージカウンターの初期化
	m_nCounterAttack = 0;						//攻撃カウンターの初期化
	m_bAttack = false;							//攻撃していない
	m_pBillboardLifeGauge = CBillboardLifeGauge::Create(D3DXVECTOR3(GetPos().x, GetPos().y + 60.0f, GetPos().z),GetLife(),GetMaxLife());	//ビルボード体力ゲージの生成

	return S_OK;
}

//=============================================================================
// 敵の終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
	m_nNumEnemy--;		//敵の総数減算

	for (int nCntModel = 0; nCntModel < MAX_ENEMY_MODEL; nCntModel++)
	{//モデルの数だけ繰り返し
		if (m_apModel[nCntModel] != NULL)
		{
			//終了処理
			m_apModel[nCntModel]->Uninit();
			delete m_apModel[nCntModel];
			m_apModel[nCntModel] = NULL;
		}
	}

	//ビルボード体力ゲージの終了処理
	if (m_pBillboardLifeGauge != NULL)
	{
		m_pBillboardLifeGauge->Uninit();
		m_pBillboardLifeGauge = NULL;
	}
	//データ開放
	Release();
}

//=============================================================================
// 敵の更新処理
//=============================================================================
void CEnemy::Update(void)
{
	//現在の位置情報を過去の位置に代入
	SetPosOld(GetPos());

	//モーションの更新処理
	UpdateMotion();

	//現在のモーションを過去のモーション情報に代入
	m_OldMotion = m_motionType;

	m_nCounter++;	//カウンターの加算

	if (m_nCounter >= 100)
	{//カウンターが100以上の場合
		m_nCounter = 0;	//カウンターの初期化
	}

	if (m_pBillboardLifeGauge != NULL)
	{//ビルボード体力ゲージがNULLでない場合
		m_pBillboardLifeGauge->SetPos(D3DXVECTOR3(GetPos().x, GetPos().y + 60.0f, GetPos().z));	//位置の設置処理
	}

	Collision();

	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();
	//慣性処理
	move.x += (0.0f - move.x) * 0.5f;
	move.z += (0.0f - move.z) * 0.5f;

	pos += move;	//位置の加算処理

	//目的の角度まで
	float fAngle = m_fDestAngle - rot.y;

	//角度調整
	if (fAngle > D3DX_PI)
	{
		fAngle -= D3DX_PI * 2;
	}
	if (fAngle < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2;
	}
	//敵の角度調整
	if (rot.y > D3DX_PI)
	{
		rot.y -= D3DX_PI * 2;
	}

	if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2;
	}
	//角度情報
	rot.y += fAngle * 0.1f;

	//種類分け
	switch (m_state)
	{
	case ENEMY_STATE_NOMAL:	//通常状態の場合
		break;
	case EMEMY_STATE_DAMAGE:	//ダメージ状態の場合
		m_nCntDamage++;			//ダメージカウンターの加算
		if (m_nCntDamage >= 50)
		{//ダメージカウンターが５０以上の場合
			m_nCntDamage = 0;	//ダメージカウンターの初期化
			m_state = ENEMY_STATE_NOMAL;	//通常状態にする
		}
		break;
	}

	if (m_bAttack == true)
	{
		m_nCounterAttack++;		//攻撃カウンターの加算
		if (m_nCounterAttack >= 50)
		{//攻撃カウンターが５０以上の場合
			m_nCounterAttack = 0;	//
			m_bAttack = false;		//攻撃していないようにする
		}
	}
	SetPos(pos);
	SetRot(rot);
	SetMove(move);
#ifdef _DEBUG
	//CDebugProc::Print(1, "敵の位置  : x[%.1f],y[%.1f],z[%.1f]\n", m_pos.x, m_pos.y, m_pos.z);
#endif
}

//=============================================================================
// 敵の描画処理
//=============================================================================
void CEnemy::Draw(void)
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
	D3DXVECTOR3	posNormal, normal;	//位置、法線

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
	posNormal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//法線の設定
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//位置と法線から平面を作成する
	D3DXPlaneFromPointNormal(&planeField, &posNormal, &normal);

	//平面の射影
	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);

	//ワールドマトリックスと掛け合わせる
	D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	for (int nCntModel = 0; nCntModel < MAX_ENEMY_MODEL; nCntModel++)
	{//モデルの数だけ繰り返し
	 //テクスチャ無効
	 //pDevice->SetTexture(0, NULL);
	 // モデル(パーツ)の描画
	 //m_pMesh[nCntModel]->DrawSubset(0);
		if (m_apModel[nCntModel] != NULL)
		{
			//描画処理
			m_apModel[nCntModel]->Draw();
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

	for (int nCntModel = 0; nCntModel < MAX_ENEMY_MODEL; nCntModel++)
	{//モデルの数だけ繰り返し
		if (m_apModel[nCntModel] != NULL)
		{
			if (m_state == EMEMY_STATE_DAMAGE)
			{// 押されていない状態の場合
				if (m_nCounter % 5 == 0)
				{// カウンターが一定の値になったら
				//モデルの描画
					m_apModel[nCntModel]->Draw();
				}
			}
			else
			{
				//モデルの描画
				m_apModel[nCntModel]->Draw();
			}
		}
	}
}

//=============================================================================
// 敵のモーション更新処理
//=============================================================================
void CEnemy::UpdateMotion(void)
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
		m_bAction = true;
	}
	else
	{
		m_bAction = false;

	}
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{//モデルの数だけ繰り返し
		if (m_apModel[nCntModel] != NULL)
		{//モデルがある場合
			pKey = &m_apMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[nCntModel];
			pKeyNext = &m_apMotionInfo[m_motionType].aKeyInfo[(m_nKey + 1) % /*2*/m_apMotionInfo[m_motionType].nNumKey].aKey[nCntModel];

			//位置座標X
			fRateMotion = (float)(m_nCountFrame) / (float)m_apMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame;	// 相対値の取得
			fDiffMotion = pKeyNext->fPosX - pKey->fPosX;														// 差分の取得
			pos.x = pKey->fPosX + (fDiffMotion * fRateMotion);

			//位置座標Y
			fDiffMotion = pKeyNext->fPosY - pKey->fPosY;								// 差分の取得
			pos.y = pKey->fPosY + (fDiffMotion * fRateMotion);

			//位置座標Z
			fDiffMotion = pKeyNext->fPosZ - pKey->fPosZ;								// 差分の取得
			pos.z = pKey->fPosZ + (fDiffMotion * fRateMotion);

			//絶対値処理
			D3DXVECTOR3 rotPlus, rotMinus;
			rotPlus.x = pKeyNext->fRotX + pKey->fRotX;
			rotPlus.y = pKeyNext->fRotY + pKey->fRotY;
			rotPlus.z = pKeyNext->fRotZ + pKey->fRotZ;
			rotMinus.x = pKeyNext->fRotX - pKey->fRotX;
			rotMinus.y = pKeyNext->fRotY - pKey->fRotY;
			rotMinus.z = pKeyNext->fRotZ - pKey->fRotZ;
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
					fDiffMotion = pKeyNext->fRotX + pKey->fRotX;								// 差分の取得
					rot.x = pKey->fRotX + (fDiffMotion * fRateMotion);
				}
				else
				{
					//向き座標X
					fDiffMotion = pKeyNext->fRotX - pKey->fRotX;								// 差分の取得
					rot.x = pKey->fRotX + (fDiffMotion * fRateMotion);
				}

				if (rotPlus.y < rotMinus.y)
				{
					//向き座標Y
					fDiffMotion = pKeyNext->fRotY + pKey->fRotY;								// 差分の取得
					rot.y = pKey->fRotY + (fDiffMotion * fRateMotion);

				}
				else
				{
					//向き座標Y
					fDiffMotion = pKeyNext->fRotY - pKey->fRotY;								// 差分の取得
					rot.y = pKey->fRotY + (fDiffMotion * fRateMotion);
				}

				if (rotPlus.z < rotMinus.z)
				{
					//向き座標Z
					fDiffMotion = pKeyNext->fRotZ + pKey->fRotZ;								// 差分の取得
					rot.z = pKey->fRotZ + (fDiffMotion * fRateMotion);
				}
				else
				{
					//向き座標Z
					fDiffMotion = pKeyNext->fRotZ - pKey->fRotZ;								// 差分の取得
					rot.z = pKey->fRotZ + (fDiffMotion * fRateMotion);
				}
			}
			else
			{
				//向き座標X
				fDiffMotion = pKeyNext->fRotX - pKey->fRotX;								// 差分の取得
				rot.x = pKey->fRotX + (fDiffMotion * fRateMotion);

				//向き座標Y
				fDiffMotion = pKeyNext->fRotY - pKey->fRotY;								// 差分の取得
				rot.y = pKey->fRotY + (fDiffMotion * fRateMotion);

				//向き座標Z
				fDiffMotion = pKeyNext->fRotZ - pKey->fRotZ;								// 差分の取得
				rot.z = pKey->fRotZ + (fDiffMotion * fRateMotion);

			}
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
		if (m_nKey == m_apMotionInfo[m_motionType].nNumKey - 1 && m_apMotionInfo[m_motionType].bLoop == false)
		{//現在のキーが対象のキー総数に達しかつループしない場合
			if (m_motionType == MOTION_JUMP && m_bJump == true)
			{//
				m_nKey = m_apMotionInfo[m_motionType].nNumKey - 1;
			}
			//if (m_motionType == MOTION_PANCH || m_motionType == MOTION_LANDING)
			else if (m_motionType != MOTION_MOVE || m_motionType != MOTION_NEUTRAL)
			{
				//現在のキーを初期化
				m_nKey = 0;
				//ニュートラルモーションにする
				m_motionType = MOTION_NEUTRAL;
				//行動していない状態にする
				m_bAction = false;
			}
		}
		//カウントフレームを初期化
		m_nCountFrame = 0;
	}
}
//=============================================================================
// 敵の行動状態を取得
//=============================================================================
bool CEnemy::GetAction(void)
{
	return m_bAction;
}

//=============================================================================
// 敵の行動状態の設置処理
//=============================================================================
void CEnemy::SetAction(bool action)
{
	m_bAction = action;
}

//=============================================================================
// 敵の向き設置処理
//=============================================================================
void CEnemy::SetDestAngle(float fDestAngle)
{
	m_fDestAngle = fDestAngle;
}
//=============================================================================
// 敵の状態取得処理
//=============================================================================
CEnemy::ENEMY_STATE CEnemy::GetEnemyState(void)
{
	return m_state;
}
//=============================================================================
// 敵の状態取得処理
//=============================================================================
void CEnemy::SetEnemyState(ENEMY_STATE state)
{
	m_state = state;
}

//=============================================================================
// 敵の状態取得処理
//=============================================================================
void CEnemy::Attack(void)
{
	//コマンドの取得
	CCommand *pCommand = NULL;
	pCommand = CGame::GetCommad();

	//プレイヤーの取得
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();

	D3DXVECTOR3 PlayerPos,PlayerMove;
	PlayerPos = pPlayer->GetPos();
	PlayerMove = pPlayer->GetMove();

	D3DXVECTOR3 pos = GetPos();

	float fLength = powf((PlayerPos.x - pos.x)*(PlayerPos.x - pos.x) + (PlayerPos.z - pos.z)*(PlayerPos.z - pos.z), 0.5f);	//プレイヤーとの距離

	float fFind = powf(17.0f, 2.0f);	//半径
	float fAttackFind = powf(7.5f, 2.0f);	//半径

	if (fLength < fAttackFind &&
		pPlayer->GetState() != CPlayer::PLAYER_STATE_DAMAGE&&
		pCommand->GetCommandState() != CCommand::COMMAND_PROCESS)
	{//対象の頂点が範囲内だったら
		float fAttackAngle = atan2f(PlayerPos.x - pos.x,PlayerPos.z - pos.z);// 当たった角度を計算
		PlayerMove.x = sinf((D3DX_PI *  fAttackAngle)) * 25.0f;
		PlayerMove.z = cosf((D3DX_PI *  fAttackAngle)) * 25.0f;
		pPlayer->HitDamage(rand() % 5 + 2);
		CGame::GetPlayer()->SetMove(PlayerMove);
	}

}

//=============================================================================
// 当たり判定の処理
//=============================================================================
bool CEnemy::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fColRange)
{
	bool bHit = false;
	// 必要な値を取得
	D3DXVECTOR3 pos = GetPos();
	float fRange = GetColRange();
	float fDistance = 0.0f;
	float fHitRange = sqrtf((fColRange + fRange) * (fColRange + fRange));

	// 距離を計算
	fDistance = sqrtf((pPos->x - pos.x) * (pPos->x - pos.x) + (pPos->z - pos.z) * (pPos->z - pos.z));

	if (fDistance < fHitRange)
	{// 半径内に入っている
	 // ベクトルで押し返す
		D3DXVECTOR3 VecA = pos - *pPos;
		D3DXVec3Normalize(&VecA, &VecA);
		pPos->x -= VecA.x * (fHitRange - fDistance);
		pPos->z -= VecA.z * (fHitRange - fDistance);

		bHit = true;
	}

	return bHit;
}
//=============================================================================
// 敵のダメージ処理
//=============================================================================
void CEnemy::HitDamage(int nDamage)
{
	//プレイヤーの取得
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();

	int nLife = GetLife();
	D3DXVECTOR3 pos = GetPos();

	if (m_state != EMEMY_STATE_DAMAGE)
	{//ダメージ状態
		nLife -= nDamage;//体力減算処理
		SetLife(nLife);
		m_pBillboardLifeGauge->CutLifeGauge(nDamage);	//ビルボード体力ゲージ減算処理
		if (nLife <= 0)
		{//体力が０以下の場合
			OBJTYPE type = GetObjType();	//オブジェクトの種類を取得
			switch (type)
			{
			case OBJTYPE_BIRD:	//鳥敵の場合
				pPlayer->AddDeathBird(1);	//鳥の死亡加算処理
				//m_nNumBird++;
				break;

			case OBJTYPE_FROG: //かえるの場合
				pPlayer->AddDeathFrog(1);	//かえるの死亡加算処理

				//m_nNumFrog++;
				break;
			}
			Uninit();	//終了処理

			//スコアの取得処理
			CScore *pScore;
			pScore = CGame::GetScore();
			pScore->AddScore(100);	//スコア加算
		}
		else
		{
			m_state = EMEMY_STATE_DAMAGE;	//ダメージ状態にする
		}
		//サウンドの取得
		CSound *pSound;
		pSound = CManager::GetSound();
		pSound->PlaySound(CSound::SOUND_LABEL_SE_DAMAGE);

		//パーティクルの生成
		for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
		{
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
// 敵の総数取得処理
//=============================================================================
int CEnemy::GetNumEnemy(void)
{
	return m_nNumEnemy;
}

//=============================================================================
// 鳥敵の総数取得処理
//=============================================================================
int CEnemy::GetNumBird(void)
{
	return m_nNumBird;
}

//=============================================================================
// かえる敵の総数取得処理
//=============================================================================
int CEnemy::GetNumFrog(void)
{
	return m_nNumFrog;
}

//=============================================================================
//	当たり判定処理
//=============================================================================
void CEnemy::Collision(void)
{
	// 配置物と地面へのポインタ取得する
	CScene *pScene = NULL;               // オブジェクトのポインタ
	CScene *pSceneNext = NULL;           // 次のオブジェクトのポインタ

	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 posOld = GetPosOld();
	D3DXVECTOR3 move = GetMove();
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULLになるまで繰り返す
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_BIRD)
			{
				CEnemy *pEnemy = NULL;
				pEnemy = (CEnemy*)pScene;

				if (pEnemy->Collision(&pos, &posOld, &move, GetColRange()) == true)
				{

				}
			}
			// 次のオブジェクトへのポインタを取得
			pScene = pSceneNext;
		}
	}
	SetPos(pos);
}

//=============================================================================
// 敵のモーション読み込み
//=============================================================================
void CEnemy::LoadMotion(char *textName)
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

	pFile = fopen(textName, "r");	//テキスト読み込み
	if (pFile != NULL)
	{//ファイルが開けた場合
		char *pStrCur;						//文字列の先頭へのポインタ
		char aLine[256];					//文字列の読み込み用
		char aStr[256];						//文字列抜き出し用
		int nIdx;

		//************************************************************************
		// モデルの総数&ファイルの読み込み
		//************************************************************************
		while (1)
		{//モデルファイルを読み込むまで繰り返し
			pStrCur = CLoadFileFunction::ReadLine(pFile, &aLine[0]);
			strcpy(&aStr[0], pStrCur);				//文字列を取り出す

			if (memcmp(&aStr[0], TEXT_NUM_MODEL, strlen(TEXT_NUM_MODEL)) == 0)
			{//テキストがモデルの総数を示す行の場合
				pStrCur += strlen(TEXT_NUM_MODEL);	//頭出し
				strcpy(&aStr[0], pStrCur);

				m_nNumModel = atoi(&aStr[0]);	//モデルの総数に代入

				for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
				{
					while (1)
					{
						pStrCur = CLoadFileFunction::ReadLine(pFile, &aLine[0]);
						strcpy(&aStr[0], pStrCur);				//文字列を取り出す
						if (memcmp(&aStr[0], TEXT_FILENAME_MODEL, strlen(TEXT_FILENAME_MODEL)) == 0)
						{//テキストがモデルの名前を示す行の場合
							pStrCur += strlen(TEXT_FILENAME_MODEL);					//頭出し
							strcpy(&aStr[0], pStrCur);
							int nNum = CLoadFileFunction::PopString(pStrCur, &aStr[0]);
							strcpy(&aStr[0], pStrCur);
							aStr[nNum - 1] = '\0';

							strcpy(&m_aFilenameModel[nCntModel][0], aStr);	//モデルの名前を取得

																			// Xファイルの読み込み
							D3DXLoadMeshFromX(&m_aFilenameModel[nCntModel][0],
								D3DXMESH_SYSTEMMEM,
								pDevice,
								NULL,
								&m_pBuffMat[nCntModel],
								NULL,
								&m_nNumMat[nCntModel],
								&m_pMesh[nCntModel]);

							break;
						}
					}
				}
				break;		//抜け出す
			}
		}

		while (1)
		{//モデルの情報を読み取るまで繰り返し
			pStrCur = CLoadFileFunction::ReadLine(pFile, &aLine[0]);
			strcpy(&aStr[0], pStrCur);				//文字列を取り出す

			//************************************************************************
			// モデルの情報読み込み
			//************************************************************************
			if (memcmp(&aStr[0], TEXT_CHARASET, strlen(TEXT_CHARASET)) == 0)
			{//テキストがモデル設置を示す行の場合
				while (1)
				{
					pStrCur = CLoadFileFunction::ReadLine(pFile, pStrCur);
					strcpy(&aStr[0], pStrCur);				//文字列を取り出す
					if (memcmp(&aStr[0], TEXT_NUM_PARTS, strlen(TEXT_NUM_PARTS)) == 0)
					{
						pStrCur += strlen(TEXT_NUM_PARTS);
						pStrCur = CLoadFileFunction::GetLineTop(pStrCur);
						strcpy(&aStr[0], pStrCur);			//文字列を取り出す
						m_nNumParts = atoi(&aStr[0]);

						for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
						{
							pStrCur = CLoadFileFunction::ReadLine(pFile, &aLine[0]);
							strcpy(&aStr[0], pStrCur);				//文字列を取り出す

							while (1)
							{
								if (memcmp(&aStr[0], TEXT_PARTSSET, strlen(TEXT_PARTSSET)) == 0)
								{//パーツ情報の設置を示す行の場合
									while (1)
									{
										pStrCur = CLoadFileFunction::ReadLine(pFile, &aLine[0]);
										strcpy(&aStr[0], pStrCur);				//文字列を取り出す

										if (memcmp(&aStr[0], TEXT_INDEX, strlen(TEXT_INDEX)) == 0)
										{//テキストがインデックスを示す行の場合
											pStrCur += strlen(TEXT_INDEX);
											pStrCur = CLoadFileFunction::GetLineTop(pStrCur);
											strcpy(&aStr[0], pStrCur);			//文字列を取り出す
											nIdx = atoi(&aStr[0]);
											m_nIndexParent[nIdx];
										}
										else if (memcmp(&aStr[0], TEXT_PARENT, strlen(TEXT_PARENT)) == 0)
										{//テキストが親子関係を示す行の場合
											pStrCur += strlen(TEXT_PARENT);
											pStrCur = CLoadFileFunction::GetLineTop(pStrCur);
											strcpy(&aStr[0], pStrCur);			//文字列を取り出す
											m_nIndexParent[nIdx] = atoi(&aStr[0]);
										}
										else if (memcmp(&aStr[0], TEXT_POS, strlen(TEXT_POS)) == 0)
										{//テキストが位置を示す行の場合
											int nWord = 0;
											pStrCur += strlen(TEXT_POS);
											//nWord = PopString(pStrCur, &aStr[0]);
											//pStrCur += nWord;
											strcpy(&aStr[0], pStrCur);			//文字列を取り出す
											m_aKeyOffset[nCntParts].fPosX = (float)atof(&aStr[0]);

											nWord = CLoadFileFunction::PopString(pStrCur, &aStr[0]);
											pStrCur += nWord;
											strcpy(&aStr[0], pStrCur);			//文字列を取り出す
											m_aKeyOffset[nCntParts].fPosY = (float)atof(&aStr[0]);

											nWord = CLoadFileFunction::PopString(pStrCur, &aStr[0]);
											pStrCur += nWord;
											strcpy(&aStr[0], pStrCur);			//文字列を取り出す
											m_aKeyOffset[nCntParts].fPosZ = (float)atof(&aStr[0]);
										}
										else if (memcmp(&aStr[0], TEXT_ROT, strlen(TEXT_ROT)) == 0)
										{//テキストが位置を示す行の場合
											int nWord = 0;
											pStrCur += strlen(TEXT_ROT);
											//nWord = PopString(pStrCur, &aStr[0]);
											//pStrCur += nWord;
											strcpy(&aStr[0], pStrCur);			//文字列を取り出す
											m_aKeyOffset[nCntParts].fRotX = (float)atof(&aStr[0]);

											nWord = CLoadFileFunction::PopString(pStrCur, &aStr[0]);
											pStrCur += nWord;
											strcpy(&aStr[0], pStrCur);			//文字列を取り出す
											m_aKeyOffset[nCntParts].fRotY = (float)atof(&aStr[0]);

											nWord = CLoadFileFunction::PopString(pStrCur, &aStr[0]);
											pStrCur += nWord;
											strcpy(&aStr[0], pStrCur);			//文字列を取り出す
											m_aKeyOffset[nCntParts].fRotZ = (float)atof(&aStr[0]);
										}
										else if (memcmp(&aStr[0], TEXT_END_PARTSSET, strlen(TEXT_END_PARTSSET)) == 0)
										{//テキストがパーツの設置終了を示す行の場合
											if (m_nIndexParent[nCntParts] == -1)
											{
												m_apModel[nIdx] = CModel::Create(D3DXVECTOR3(pos.x + m_aKeyOffset[nIdx].fPosX, pos.y + m_aKeyOffset[nIdx].fPosY, pos.z + m_aKeyOffset[nIdx].fPosZ));
												m_apModel[nIdx]->BindX(m_pMesh[nIdx], m_pBuffMat[nIdx], m_nNumMat[nIdx]);
												m_apModel[nIdx]->SetParent(NULL);
												//m_offsetPos[nIdx] = m_apModel[nIdx]->GetPos();
											}
											else if (m_nIndexParent[nCntParts] > -1)
											{
												m_apModel[nIdx] = CModel::Create(D3DXVECTOR3(pos.x + m_aKeyOffset[nIdx].fPosX, pos.y + m_aKeyOffset[nIdx].fPosY, pos.z + m_aKeyOffset[nIdx].fPosZ));
												m_apModel[nIdx]->BindX(m_pMesh[nIdx], m_pBuffMat[nIdx], m_nNumMat[nIdx]);
												m_apModel[nIdx]->SetParent(m_apModel[m_nIndexParent[nIdx]]);
												//m_offsetPos[nIdx] = m_apModel[nIdx]->GetPos();
											}
											m_offsetPos[nIdx] = m_apModel[nIdx]->GetPos();
											break;
										}
									}
									break;
								}
							}
						}
					}
					else if (memcmp(&aStr[0], TEXT_END_CHARASET, strlen(TEXT_END_CHARASET)) == 0)
					{//テキストがモデルの設置終了を示す行の場合
						break;
					}
				}
				break;
			}
		}

		//************************************************************************
		// モーションの読み込み
		//************************************************************************
		for (int nCntMotion = 0; nCntMotion < MAX_ENEMY_MOTION; nCntMotion++)
		{
			pStrCur = CLoadFileFunction::ReadLine(pFile, &aLine[0]);
			strcpy(&aStr[0], pStrCur);				//文字列を取り出す
			if (memcmp(&aStr[0], TEXT_MOTIONSET, strlen(TEXT_MOTIONSET)) == 0)
			{//モーションの設置を示す行の場合
				while (1)
				{
					pStrCur = CLoadFileFunction::ReadLine(pFile, pStrCur);				//行の読み込み
					strcpy(&aStr[0], pStrCur);						//文字列を取り出す
					if (memcmp(&aStr[0], TEXT_LOOP, strlen(TEXT_LOOP)) == 0)
					{//ループを示す行の場合
						pStrCur += strlen(TEXT_LOOP);				//頭出し
						pStrCur = CLoadFileFunction::GetLineTop(pStrCur);				//空白消去
						int nNum = CLoadFileFunction::PopString(pStrCur, &aStr[0]);	//文字数のカウント
						strcpy(&aStr[0], pStrCur);
						aStr[nNum - 1] = '\0';

						if (atoi(&aStr[0]) == 0)
						{//０が記入されていた場合ループしない
							m_apMotionInfo[nCntMotion].bLoop = false;
						}
						else if (atoi(&aStr[0]) == 1)
						{//１が記入されていた場合ループする
							m_apMotionInfo[nCntMotion].bLoop = true;
						}
					}
					else if (memcmp(&aStr[0], TEXT_COLLISION, strlen(TEXT_COLLISION)) == 0)
					{
						pStrCur += strlen(TEXT_COLLISION);								//頭出し
						pStrCur = CLoadFileFunction::GetLineTop(pStrCur);				//空白消去
						strcpy(&aStr[0], pStrCur);										//文字列を取り出す
						int nWord = 0;

						m_aMotionAttack[nCntMotion].nAttackStartFrame = atoi(&aStr[0]);	//X座標の代入

						nWord = CLoadFileFunction::PopString(pStrCur, &aStr[0]);		//文字数カウント

						pStrCur += nWord;												//カウントされた分頭出し
						strcpy(&aStr[0], pStrCur);										//文字列を取り出す
						m_aMotionAttack[nCntMotion].nAttackEndFrame = atoi(&aStr[0]);	//X座標の代入
					}
					else if (memcmp(&aStr[0], TEXT_NUM_KEY, strlen(TEXT_NUM_KEY)) == 0)
					{//キーの総数を示す行の場合
						pStrCur += strlen(TEXT_NUM_KEY);			//頭出し
						pStrCur = CLoadFileFunction::GetLineTop(pStrCur);				//空白消去
						strcpy(&aStr[0], pStrCur);					//文字列を取り出す

						m_apMotionInfo[nCntMotion].nNumKey = atoi(&aStr[0]);//キーの総数を代入

						for (int nCntKey = 0; nCntKey < m_apMotionInfo[nCntMotion].nNumKey; nCntKey++)
						{//キーの総数分繰り返し
							while (1)
							{
								pStrCur = CLoadFileFunction::ReadLine(pFile, &aLine[0]);	//行の読み込み
								strcpy(&aStr[0], pStrCur);				//文字列を取り出す
								if (memcmp(&aStr[0], TEXT_KEYSET, strlen(TEXT_KEYSET)) == 0)
								{//キーの設置開始を示す行の場合
									for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
									{
										while (1)
										{
											pStrCur = CLoadFileFunction::ReadLine(pFile, &aLine[0]);
											strcpy(&aStr[0], pStrCur);				//文字列を取り出す

											if (memcmp(&aStr[0], TEXT_FRAME, strlen(TEXT_FRAME)) == 0)
											{//フレーム数を示す行の場合
												pStrCur += strlen(TEXT_FRAME);		//頭出し
												pStrCur = CLoadFileFunction::GetLineTop(pStrCur);		//行の読み込み
												strcpy(&aStr[0], pStrCur);			//文字列を取り出す
												m_apMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame = atoi(&aStr[0]);	//フレームの代入
												m_aMotionAttack[nCntMotion].nNumFrame += atoi(&aStr[0]);
											}
											else if (memcmp(&aStr[0], TEXT_KEY, strlen(TEXT_KEY)) == 0)
											{//キーの開始を示す行の場合

												while (1)
												{
													pStrCur = CLoadFileFunction::ReadLine(pFile, &aLine[0]);	//行の読み込み
													strcpy(&aStr[0], pStrCur);				//文字列を取り出す

													if (memcmp(&aStr[0], TEXT_POS, strlen(TEXT_POS)) == 0)
													{//位置の情報を示す行の場合
														int nWord = 0;
														pStrCur += strlen(TEXT_POS);	//頭出し

														strcpy(&aStr[0], pStrCur);			//文字列を取り出す
														m_apMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].fPosX = (float)atof(&aStr[0]);	//X座標の代入

														nWord = CLoadFileFunction::PopString(pStrCur, &aStr[0]);	//文字数カウント
														pStrCur += nWord;						//カウントされた分頭出し
														strcpy(&aStr[0], pStrCur);				//文字列を取り出す
														m_apMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].fPosY = (float)atof(&aStr[0]);	//Y座標の代入

														nWord = CLoadFileFunction::PopString(pStrCur, &aStr[0]);	//文字数カウント
														pStrCur += nWord;						//カウントされた分頭出し
														strcpy(&aStr[0], pStrCur);				//文字列を取り出す
														m_apMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].fPosZ = (float)atof(&aStr[0]);	//Z座標の代入
													}
													else if (memcmp(&aStr[0], TEXT_ROT, strlen(TEXT_ROT)) == 0)
													{//向きの情報を示す行の場合
														int nWord = 0;
														pStrCur += strlen(TEXT_ROT);		//頭出し

														strcpy(&aStr[0], pStrCur);			//文字列を取り出す
														m_apMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].fRotX = (float)atof(&aStr[0]);	//X座標の代入

														nWord = CLoadFileFunction::PopString(pStrCur, &aStr[0]);	//文字数カウント
														pStrCur += nWord;						//カウントされた分頭出し
														strcpy(&aStr[0], pStrCur);				//文字列を取り出す
														m_apMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].fRotY = (float)atof(&aStr[0]);	//Y座標の代入

														nWord = CLoadFileFunction::PopString(pStrCur, &aStr[0]);	//文字数カウント
														pStrCur += nWord;						//カウントされた分頭出し
														strcpy(&aStr[0], pStrCur);				//文字列を取り出す
														m_apMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].fRotZ = (float)atof(&aStr[0]);	//Z座標の代入
													}
													else if (memcmp(&aStr[0], TEXT_END_KEY, strlen(TEXT_END_KEY)) == 0)
													{//キーの終了を示す行の場合
														break;
													}
												}
												break;
											}

										}
									}
								}
								else if (memcmp(&aStr[0], TEXT_END_KEYSET, strlen(TEXT_END_KEYSET)) == 0)
								{//キーの設置終了を示す行の場合
									break;
								}
							}
						}
						nCntAttack++;
					}
					else if (memcmp(&aStr[0], TEXT_END_MOTIONSET, strlen(TEXT_END_MOTIONSET)) == 0)
					{//モーションの設置終了を示す行の場合
						break;
					}
				}
			}
		}
	}
	fclose(pFile);	//ファイルを閉じる
}

//=============================================================================
// モーションの設置処理
//=============================================================================
void CEnemy::SetMotion(MOTION_TYPE type)
{
	m_motionType = type;
}

//=============================================================================
//
// 鳥敵の処理
//
//=============================================================================

//=============================================================================
// 敵のコンストラクタ
//=============================================================================
CEnemyBird::CEnemyBird() : CEnemy(3,OBJTYPE_BIRD)
{
}
//=============================================================================
// 敵のデストラクタ
//=============================================================================
CEnemyBird::~CEnemyBird()
{

}

//=============================================================================
//敵の生成
//=============================================================================
CEnemyBird *CEnemyBird::Create(D3DXVECTOR3 pos)
{
	CEnemyBird *pEnemyBird = NULL;					//敵のポインタ
	if (pEnemyBird == NULL)
	{
		pEnemyBird = new CEnemyBird;				//シーンの動的確保
		//pEnemyBird->SetLife(20);
		//pEnemyBird->SetMaxLife(20);
		pEnemyBird->Init();							//初期化処理
		pEnemyBird->SetPos(pos);					//位置の設置処理
	}
	return pEnemyBird;					//値を返す
}

//=============================================================================
// 敵の初期化処理
//=============================================================================
HRESULT CEnemyBird::Init(void)
{
	CEnemy::Init();	//初期化処理

	//モーション情報を読み込み
	LoadMotion("data\\TEXT\\Enemy\\Bird.txt");

	return S_OK;
}

//=============================================================================
// 敵の終了処理
//=============================================================================
void CEnemyBird::Uninit(void)
{
	CEnemy::Uninit();	//敵の終了処理
}

//=============================================================================
// 敵の更新処理
//=============================================================================
void CEnemyBird::Update(void)
{
	//プレイヤーの取得処理
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();

	//敵の更新処理
	CEnemy::Update();


	if (pPlayer != NULL)
	{//プレイヤーがNULLでない場合
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//移動量

		D3DXVECTOR3 PlayerPos, EnemyPos;					//プレイヤーの位置
		PlayerPos = pPlayer->GetPos();						//位置の取得
		EnemyPos = GetPos();								//敵の位置取得
		ENEMY_STATE state = GetEnemyState();				//敵の状態取得

		if (state != EMEMY_STATE_DAMAGE)
		{//ダメージ状態の場合
			if (pPlayer != NULL)
			{//プレイヤーがNULLでない場合
				float fLength = powf((PlayerPos.x - EnemyPos.x)*(PlayerPos.x - EnemyPos.x) + (PlayerPos.z - EnemyPos.z)*(PlayerPos.z - EnemyPos.z), 0.5f);	//プレイヤーとの距離
				float fFind = powf(17.0f, 2.0f);			//見つける半径
				float fAttackFind = powf(7.5f, 2.0f);		//攻撃する半径

				if (fLength < fFind)
				{//対象が範囲内だったら
					D3DXVECTOR3 DiffPos;

					//プレイヤーと敵のベクトルを算出
					DiffPos = EnemyPos - PlayerPos;
					D3DXVec3Normalize(&DiffPos, &DiffPos);

					//ベクトル方向に向かって移動
					move.x -= DiffPos.x * 0.80f;
					move.z -= DiffPos.z * 0.80f;

					//敵の角度設定
					float fDestAngle = atan2f(EnemyPos.x - PlayerPos.x, EnemyPos.z - PlayerPos.z);
					SetDestAngle(fDestAngle);
					SetMove(move);
					if (fAttackFind > fLength)
					{//攻撃範囲がプレイヤーの距離より小さい場合
						SetMotion(MOTION_PANCH);	//攻撃モーションにする
						Attack();					//攻撃
					}
					else
					{//それ以外の場合
						SetMotion(MOTION_MOVE);	//移動モーションにする
					}
				}
				else
				{//それ以外の場合
					SetMotion(MOTION_NEUTRAL);	//ニュートラルモーションにする
					move.x = 0.0f;				//移動量初期化
					move.z = 0.0f;
					SetMove(move);				//移動量設置処理
				}

			}
		}
		else
		{
			move.x = 0.0f;	//移動量初期化
			move.z = 0.0f;
			SetMove(move);	//移動量設置処理
		}
	}
}

//=============================================================================
// 敵の描画処理
//=============================================================================
void CEnemyBird::Draw(void)
{
	CEnemy::Draw();	//描画処理
}

//=============================================================================
//
// かえる敵の処理
//
//=============================================================================

//=============================================================================
// 敵のコンストラクタ
//=============================================================================
CEnemyFrog::CEnemyFrog() : CEnemy(3,OBJTYPE_FROG)
{
	m_nCntInterval = 0;	//間隔カウンターの初期化
}
//=============================================================================
// 敵のデストラクタ
//=============================================================================
CEnemyFrog::~CEnemyFrog()
{

}

//=============================================================================
//敵の生成
//=============================================================================
CEnemyFrog *CEnemyFrog::Create(D3DXVECTOR3 pos)
{
	CEnemyFrog *pEnemyFrog = NULL;					//敵のポインタ
	if (pEnemyFrog == NULL)
	{
		pEnemyFrog = new CEnemyFrog;				//シーンの動的確保
		pEnemyFrog->Init();							//初期化処理
		pEnemyFrog->SetPos(pos);					//位置の設置処理
	}
	return pEnemyFrog;					//値を返す
}

//=============================================================================
// 敵の初期化処理
//=============================================================================
HRESULT CEnemyFrog::Init(void)
{
	CEnemy::Init();	//初期化処理

	//モーション情報を読み込み
	LoadMotion("data\\TEXT\\Enemy\\Frog.txt");

	return S_OK;
}

//=============================================================================
// 敵の終了処理
//=============================================================================
void CEnemyFrog::Uninit(void)
{
	CEnemy::Uninit();	//終了処理
}

//=============================================================================
// 敵の更新処理
//=============================================================================
void CEnemyFrog::Update(void)
{
	CEnemy::Update();	//敵の更新処理

	//プレイヤーの取得
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();

	if (pPlayer != NULL)
	{//プレイヤーがNULLでない場合
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		D3DXVECTOR3 PlayerPos, EnemyPos, EnemyRot;					//プレイヤーの位置
		PlayerPos = pPlayer->GetPos();								//位置の取得
		EnemyPos = GetPos();										//敵の位置取得
		EnemyRot = GetRot();										//敵の向き取得処理
		ENEMY_STATE state = GetEnemyState();						//敵の状態取得

		if (state != EMEMY_STATE_DAMAGE)
		{
			if (pPlayer != NULL)
			{//プレイヤーがNULLでない場合
				float fLength = powf((PlayerPos.x - EnemyPos.x)*(PlayerPos.x - EnemyPos.x) + (PlayerPos.z - EnemyPos.z)*(PlayerPos.z - EnemyPos.z), 0.5f);	//プレイヤーとの距離
				float fFind = powf(17.0f, 2.0f);			//見つける半径
				float fAttackFind = powf(15, 2.0f);			//攻撃する半径

				if (fLength < fFind)
				{//対象が範囲内だったら
					D3DXVECTOR3 DiffPos;

					//敵の角度設定
					float fDestAngle = atan2f(EnemyPos.x - PlayerPos.x, EnemyPos.z - PlayerPos.z);
					SetDestAngle(fDestAngle);	//角度の設置処理
					if (fAttackFind > fLength && state != ENEMY_STATE_ATTACK)
					{
						SetMotion(MOTION_PANCH);	//攻撃モーションにする
						SetEnemyState(ENEMY_STATE_ATTACK);	//攻撃状態にする
						CBullet::Create(D3DXVECTOR3(EnemyPos.x, EnemyPos.y + 30.0f, EnemyPos.z), 50, D3DXVECTOR3(sinf(EnemyRot.y) * -8.0f, 0.0f, cosf(EnemyRot.y) * -8.0f)); //弾の生成
					}
				}
			}
		}

		//状態の設定
		switch (state)
		{
		case ENEMY_STATE_ATTACK:	//攻撃状態の場合
			m_nCntInterval++;	//間隔カウンターを加算
			if (m_nCntInterval >= 100)
			{//間隔カウンターが１００以上の場合
				SetEnemyState(ENEMY_STATE_NOMAL);	//通常状態にする
			}
			break;
		case ENEMY_STATE_NOMAL://通常状態の場合
			m_nCntInterval = 0;	//間隔カウンターを初期化
			break;
		}
	}
}

//=============================================================================
// 敵の描画処理
//=============================================================================
void CEnemyFrog::Draw(void)
{
	CEnemy::Draw();	//敵の描画処理
}