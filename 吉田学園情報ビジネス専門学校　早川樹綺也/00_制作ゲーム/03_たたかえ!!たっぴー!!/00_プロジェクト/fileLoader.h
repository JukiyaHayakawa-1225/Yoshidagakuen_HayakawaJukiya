//*****************************************************************************
//
//     �t�@�C���ǂݍ��݂̏���[fileLoader.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _FILELOADER_H_
#define _FILELOADER_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �t�@�C���ǂݍ��݃N���X�̒�`
//*****************************************************************************
class CFileLoader
{
public:    // �N�ł��A�N�Z�X�\
	CFileLoader(char *pLoadFileName);
	~CFileLoader();

	static CFileLoader *Create(char *pLoadFileName);

	HRESULT Init(void);
	void Uninit(void);
	char *GetString(char *pSrc);

	void SetLoadfileName(char *pFileName);
	char *GetLoadfileName(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	FILE *m_pFile;               // �t�@�C���|�C���^
	char m_aLoadFileName[256];   // �ǂݍ��ރt�@�C����
};


#endif