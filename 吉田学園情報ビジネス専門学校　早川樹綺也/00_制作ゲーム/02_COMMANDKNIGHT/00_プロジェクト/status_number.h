//=============================================================================
//
// ステータス数字の処理 [status_number.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _STATUS_NUMBER_H_
#define _STATUS_NUMBER_H_

#include "main.h"
#include "scene.h"

#define MAX_STATUS_DIGIT (3)	//ステータスの最大桁数
#define MAX_SKILL (3)
#define MAX_ITEM (2)
#define MAX_QUANTITY_DIGIT (2)
#define MAX_STATUS		 (STATUS_MAX)
class CNumber;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CStatus_Number : public CScene
{
public:
	//*************************************************************************
	// 構造体定義
	//*************************************************************************
	typedef enum
	{
		STATUS_LIFE = 0,
		STATUS_SKILL,
		STATUS_MAX,
	}STATUS;

	//メンバ関数
	CStatus_Number();
	~CStatus_Number();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetStatus_Number(int nStatus_Number);
	int GetStatus_Number(void);
	void AddStatus_Number(int nValue,int nType);
	void SetCol(D3DXCOLOR col);
	//静的メンバ関数
	static CStatus_Number *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);

private:
	D3DXVECTOR3							m_pos;						// 位置
	float								m_fWidth;
	float								m_fHeight;
	//静的メンバ変数
	CNumber			*m_apNumber[MAX_STATUS][MAX_STATUS_DIGIT];	// テクスチャへのポインタ
	static int		m_nStatus_Number;
	D3DXCOLOR		m_col;
protected:
};

class CSkill_Quantity_Number : public CScene
{
public:
	//メンバ関数
	CSkill_Quantity_Number();
	~CSkill_Quantity_Number();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	//静的メンバ関数
	static CSkill_Quantity_Number *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);

private:
	D3DXVECTOR3							m_pos;						// 位置
	float								m_fWidth;
	float								m_fHeight;
	//静的メンバ変数
	CNumber			*m_apNumber[MAX_SKILL][MAX_QUANTITY_DIGIT];	// テクスチャへのポインタ
	static int		m_nNumber;
	D3DXCOLOR		m_col;
protected:
};

class CItem_Quantity_Number : public CScene
{
public:
	//メンバ関数
	CItem_Quantity_Number();
	~CItem_Quantity_Number();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	//静的メンバ関数
	static CItem_Quantity_Number *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);

private:
	D3DXVECTOR3							m_pos;						// 位置
	float								m_fWidth;
	float								m_fHeight;
	//静的メンバ変数
	CNumber			*m_apNumber[MAX_ITEM][MAX_QUANTITY_DIGIT];	// テクスチャへのポインタ
	static int		m_nNumber;
	D3DXCOLOR		m_col;
protected:
};
#endif