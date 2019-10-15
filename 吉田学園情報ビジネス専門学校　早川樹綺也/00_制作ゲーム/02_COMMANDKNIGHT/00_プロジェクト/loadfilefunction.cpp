//=============================================================================
//
// �t�@�C����ǂݍ��ލۂ̊֐� [loadfilefunction.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "loadfilefunction.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLoadFileFunction::CLoadFileFunction()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLoadFileFunction::~CLoadFileFunction()
{

}

//=============================================================================
// �s�̓ǂݍ��ݏ���
//=============================================================================
char *CLoadFileFunction::ReadLine(FILE *pFile, char *pDst)
{
	fgets(pDst, 256, pFile);
	while (1)
	{//��񂪂Ȃ��Ȃ�܂ŌJ��Ԃ�

		if (memcmp(pDst, "#", strlen("#")) == 0)
		{//�R�����g�������s�̏ꍇ
			fgets(pDst, 256, pFile);
		}
		else if (memcmp(pDst, "\n", strlen("\n")) == 0)
		{//���s�������s�̏ꍇ
			fgets(pDst, 256, pFile);
		}
		else if (memcmp(pDst, "\t", strlen("\t")) == 0)
		{//�^�u�������s�̏ꍇ
			pDst += strlen("\t");	//���o��

			while (1)
			{
				if (memcmp(pDst, "#", strlen("#")) == 0)
				{//�R�����g�������s�̏ꍇ
					fgets(pDst, 256, pFile);
					break;
				}
				else if (memcmp(pDst, "\n", strlen("\n")) == 0)
				{//���s�������s�̏ꍇ
					fgets(pDst, 256, pFile);
					break;
				}
				else if (memcmp(pDst, "\t", strlen("\t")) == 0)
				{//�^�u�������s�̏ꍇ
					pDst = GetLineTop(pDst);
				}
				else if (memcmp(pDst, " ", strlen(" ")) == 0)
				{//�X�y�[�X�������s�̏ꍇ
					pDst = GetLineTop(pDst);
				}
				else
				{
					break;
				}
			}
		}
		else if (memcmp(pDst, " ", strlen(" ")) == 0)
		{//�X�y�[�X�������s�̏ꍇ
			while (1)
			{
				if (memcmp(pDst, "#", strlen("#")) == 0)
				{//�R�����g�������s�̏ꍇ
					fgets(pDst, 256, pFile);
					break;
				}
				else if (memcmp(pDst, "\n", strlen("\n")) == 0)
				{//���s�������s�̏ꍇ
					fgets(pDst, 256, pFile);
					break;
				}
				else if (memcmp(pDst, "\t", strlen("\t")) == 0)
				{//�^�u�������s�̏ꍇ
					pDst = GetLineTop(pDst);
				}
				else if (memcmp(pDst, " ", strlen(" ")) == 0)
				{//�X�y�[�X�������s�̏ꍇ
					pDst = GetLineTop(pDst);
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			break;
		}
	}
	return pDst;
}

//=============================================================================
// �󔒂̏���
//=============================================================================
char *CLoadFileFunction::GetLineTop(char *pScr)
{
	while (1)
	{
		if (memcmp(pScr, " ", strlen(" ")) == 0)
		{//�X�y�[�X�������s�̏ꍇ
			pScr += strlen(" ");
		}
		else if (memcmp(pScr, "\t", strlen("\t")) == 0)
		{//�^�u�������s�̏ꍇ
			pScr += strlen("\t");
		}
		else
		{
			break;
		}
	}

	return pScr;
}

//=============================================================================
// �擪�����������ē��o������
//=============================================================================
char *CLoadFileFunction::HeadPutout(char * pDest, char * pHead)
{
	pDest = GetLineTop(pDest);
	pDest += strlen(pHead);

	return pDest;
}

//=============================================================================
// �K�v�ȕ����񔲂��o������
//=============================================================================
char * CLoadFileFunction::ReadString(char * pScr, char * pDest, char * pHead)
{
	int nWord = 0;
	pScr = HeadPutout(pScr, pHead);
	nWord = PopString(pScr, pDest);
	strcpy(pDest, "\0");
	strcpy(pDest, pScr);
	strcpy(&pDest[nWord - 1], "\0");
	return pDest;
}

//=============================================================================
// ���������񂩔��菈��
//=============================================================================
int CLoadFileFunction::Memcmp(char * pScr, char * pCmp)
{
	return memcmp(pScr, pCmp, strlen(pCmp));
}

//=============================================================================
// �������J�E���g����
//=============================================================================
int CLoadFileFunction::PopString(char *pScr, char *pDst)
{
	int nCount = 0;
	while (1)
	{
		nCount += 1;	//�J�E���^�[�����Z
		pScr += 1;		//���o��
		if (memcmp(pScr, " ", strlen(" ")) == 0)
		{//�X�y�[�X�������s�̏ꍇ
			pScr = "\0";	//NULL�ɂ���
			nCount += 1;	//�J�E���^�[�����Z
			break;
		}
		else if (memcmp(pScr, "\t", strlen("\t")) == 0)
		{//�X�y�[�X�������s�̏ꍇ
			pScr = "\0";	//NULL�ɂ���
			nCount += 1;	//�J�E���^�[�����Z����
			break;
		}
	}
	strcpy(pDst, pScr);
	return  nCount;
}

//=============================================================================
// �����^�̓ǂݍ��ݏ���
//=============================================================================
int CLoadFileFunction::ReadInt(char * pScr, char * pHead)
{
	int nData = 0;
	pScr = HeadPutout(pScr, pHead);
	nData = atoi(pScr);
	return	nData;
}

//=============================================================================
// ���������^�̓ǂݍ��ݏ���
//=============================================================================
float CLoadFileFunction::ReadFloat(char * pScr, char * pHead)
{
	float fData = 0;
	pScr = HeadPutout(pScr, pHead);
	fData = (float)atof(pScr);
	return	fData;
}

//=============================================================================
// Bool�^�̓ǂݍ��ݏ���
//=============================================================================
bool CLoadFileFunction::ReadBool(char * pScr, char * pHead)
{
	bool bBool = false;
	int nBool = ReadInt(pScr, pHead);
	switch (nBool)
	{
	case 0:
		bBool = false;
		break;
	case 1:
		bBool = true;
		break;
	}
	return bBool;
}

//=============================================================================
// Vector3�̓ǂݍ��ݏ���
//=============================================================================
D3DXVECTOR3 CLoadFileFunction::ReadVector3(char * pScr, char * pHead)
{
	int nWord = 0;
	char aStr[256];
	D3DXVECTOR3 vector3 = INITIALIZE_D3DXVECTOR3;

	pScr = HeadPutout(pScr, pHead);
	//X���W
	vector3.x = (float)atof(pScr);

	//Y���W
	nWord = PopString(pScr, aStr);
	pScr += nWord;
	vector3.y = (float)atof(pScr);

	//Z���W
	nWord = PopString(pScr, aStr);
	pScr += nWord;
	vector3.z = (float)atof(pScr);
	return	vector3;
}
