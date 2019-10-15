//*****************************************************************************
//
//     �t�@�C���ۑ��̏���[fileSaver.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _FILESAVER_H_
#define _FILESAVER_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �t�@�C���ۑ��N���X�̒�`
//*****************************************************************************
class CFileSaver
{
public:    // �N�ł��A�N�Z�X�\
	CFileSaver(char *pSaveFileName);
	~CFileSaver();

	static CFileSaver *Create(char *pSaveFileName);

	HRESULT Init(void);
	void Uninit(void);
	void Print(char *fmt, ...);

	void SetSavefileName(char *pFileName);
	char *GetSavefileName(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	FILE *m_pFile;               // �t�@�C���|�C���^
	char m_aSaveFileName[256];   // �ۑ�����t�@�C����
};


#endif