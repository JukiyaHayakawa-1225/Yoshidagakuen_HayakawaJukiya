//=============================================================================
//
// �X�R�A�̏��� [score.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"
#include "scene.h"

class CNumber;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CScore : public CScene
{
public:
	//�����o�֐�
	CScore();
	~CScore();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(D3DXVECTOR3 pos);
	void SetCol(D3DXCOLOR col);
	void SetWidth(float fWidth);
	void SetHeight(float fHeight);
	void SetDigit(int nDigit);
	void SetScore(int nScore);

	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR GetCol(void);
	float GetWidth(void);
	float GetHeight(void);
	int GetDigit(void);
	int GetScore(void);

	void AddScore(int nValue);

	//�ÓI�����o�֐�
	static CScore *Create(D3DXVECTOR3 pos, float fWidth, float fHeight,int nDigit);

private:
	D3DXVECTOR3		m_pos;				//�ʒu
	D3DXCOLOR		m_col;				//�F
	float			m_fWidth;			//��
	float			m_fHeight;			//����
	int				m_nDigit;			//����
	int				m_nScore;			//�X�R�A��

	//�ÓI�����o�ϐ�
	CNumber			**m_apNumber;
protected:
};
#endif