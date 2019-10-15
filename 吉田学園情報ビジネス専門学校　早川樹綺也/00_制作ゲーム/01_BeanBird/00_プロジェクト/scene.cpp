//=============================================================================
//
// オブジェクト処理 [scene.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "scene.h"

//=============================================================================
//	静的メンバ変数宣言
//=============================================================================
int CScene::m_nNumAll = 0;					//オブジェクトの総数
CScene *CScene::m_apScene[NUM_PRIORITY][MAX_SCENE] = {};	//オブジェクトのポインタ

//=============================================================================
// オブジェクトのコンストラクタ
//=============================================================================
CScene::CScene(int nPriority)
{
	for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
	{
		if (m_apScene[nPriority][nCntScene] == NULL)
		{
			m_apScene[nPriority][nCntScene] = this;
			m_nNumAll++;	//総数を加算
			m_nID = nCntScene;
			m_nPriority = nPriority;
			break;
		}
	}
	m_objType = OBJTYPE_NONE;
}
//=============================================================================
// オブジェクトのデストラクタ
//=============================================================================
CScene::~CScene()
{

}

//=============================================================================
// 全てのオブジェクトの開放処理
//=============================================================================
void CScene::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{//最大数分繰り返し
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{//NULLでない場合
				if (m_apScene[nCntPriority][nCntScene]->GetObjType() != OBJTYPE_FADE)
				{
					//終了処理
					m_apScene[nCntPriority][nCntScene]->Uninit();
					delete m_apScene[nCntPriority][nCntScene];			//データの開放
					m_apScene[nCntPriority][nCntScene] = NULL;			//NULLにする
				}
			}
		}
	}
	m_nNumAll = 0;		//敵の総数を初期化
}

//=============================================================================
// 全てのオブジェクトの更新処理
//=============================================================================
void CScene::UpdateAll(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{//最大数分繰り返し
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{//NULLでない場合
				//更新処理
				m_apScene[nCntPriority][nCntScene]->Update();
			}
		}
	}
}

//=============================================================================
// 全てのオブジェクトの描画処理
//=============================================================================
void CScene::DrawAll(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{//最大数分繰り返し
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{//NULLでない場合
				//描画処理
				m_apScene[nCntPriority][nCntScene]->Draw();
			}
		}
	}
}

//=============================================================================
// オブジェクトの破棄
//=============================================================================
void CScene::Release(void)
{
	if (m_apScene[m_nPriority][m_nID] != NULL)
	{
		int nID;							//番号の保管
		nID = m_nID;						//deleteを行うと番号が消えてしまうためnIDに保管する
		int nPriority;						//番号の保管
		nPriority = m_nPriority;
		delete m_apScene[nPriority][nID];
		m_apScene[nPriority][nID] = NULL;
		m_nNumAll--;						//総数を減らす
	}
}
//=============================================================================
// 番号の取得
//=============================================================================
CScene *CScene::GetScene(int nPriority,int nIdx)
{
	return m_apScene[nPriority][nIdx];
}


//=============================================================================
// 種類の設置
//=============================================================================
void CScene::SetObjType(OBJTYPE objType)
{
	m_objType = objType;
}

//=============================================================================
// 種類の取得
//=============================================================================
CScene::OBJTYPE CScene::GetObjType(void)
{
	return m_objType;
}