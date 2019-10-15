//*****************************************************************************
//
//     �T�[�o�[�p�֐��̏���[serverlib.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SERVERLIB_H_
#define _SERVERLIB_H_

//*****************************************************************************
//     �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//     �T�[�o�[�p���C�u�����N���X�̒�`
//*****************************************************************************
class CServerLib
{
public:    // �N�ł��A�N�Z�X�\
	CServerLib();
	~CServerLib();

	static int Memcmp(char *pSrc, char *pComp);
	static char *GetLineTop(char *pSrc);
	static int PopString(char *pSrc, char *pDest);
	static char *HeadPutout(char *pDest, char *pHead);
	static int ReadInt(char *pSrc, char *pHead);
	static float ReadFloat(char *pSrc, char *pHead);
	static char *ReadFloat3(char *pSrc, char *pHead, float *pData);
	static bool ReadBool(char *pSrc, char *pHead);
	static char *ReadString(char *pSrc, char *pDest, char *pHead);
	static char *WriteInt(int nData, char *pDest);
	static char *WriteFloat(float fData, char *pDest);
	static char *WriteFloat3(float *pData, char *pDest);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\

};

#endif