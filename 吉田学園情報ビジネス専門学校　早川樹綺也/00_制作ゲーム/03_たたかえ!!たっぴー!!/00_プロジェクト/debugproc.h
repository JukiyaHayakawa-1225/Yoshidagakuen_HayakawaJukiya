//=============================================================================
//
// �f�o�b�O�p�v���V�[�W���̏��� [debugproc.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

//�O���錾
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_CHAR (10000)	//�����̍ő吔

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CDebugProc
{
public:	//�N������A�N�Z�X�\
	//�����o�֐�
	CDebugProc();
	~CDebugProc();
	void Init(void);
	void Uninit(void);

	//�ÓI�����o�֐�
	static void Print(int nNumber,char *fmt, ...);
	static void ClearString(void);
	static void Draw(void);
	static void SwitchDisp(void);

private:	//�����������A�N�Z�X�\
	//�ÓI�����o�ϐ�
	static LPD3DXFONT m_pFont;				//�t�H���g
	static char		  m_aStr[MAX_CHAR];		//������
	static bool       m_bDisp;              //�`�悷�邩�ǂ���
};
#endif
