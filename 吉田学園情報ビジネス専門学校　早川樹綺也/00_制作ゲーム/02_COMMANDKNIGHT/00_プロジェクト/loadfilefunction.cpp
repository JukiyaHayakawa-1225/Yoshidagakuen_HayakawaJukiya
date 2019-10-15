//=============================================================================
//
// ファイルを読み込む際の関数 [loadfilefunction.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "loadfilefunction.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CLoadFileFunction::CLoadFileFunction()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CLoadFileFunction::~CLoadFileFunction()
{

}

//=============================================================================
// 行の読み込み処理
//=============================================================================
char *CLoadFileFunction::ReadLine(FILE *pFile, char *pDst)
{
	fgets(pDst, 256, pFile);
	while (1)
	{//情報がなくなるまで繰り返し

		if (memcmp(pDst, "#", strlen("#")) == 0)
		{//コメントを示す行の場合
			fgets(pDst, 256, pFile);
		}
		else if (memcmp(pDst, "\n", strlen("\n")) == 0)
		{//改行を示す行の場合
			fgets(pDst, 256, pFile);
		}
		else if (memcmp(pDst, "\t", strlen("\t")) == 0)
		{//タブを示す行の場合
			pDst += strlen("\t");	//頭出し

			while (1)
			{
				if (memcmp(pDst, "#", strlen("#")) == 0)
				{//コメントを示す行の場合
					fgets(pDst, 256, pFile);
					break;
				}
				else if (memcmp(pDst, "\n", strlen("\n")) == 0)
				{//改行を示す行の場合
					fgets(pDst, 256, pFile);
					break;
				}
				else if (memcmp(pDst, "\t", strlen("\t")) == 0)
				{//タブを示す行の場合
					pDst = GetLineTop(pDst);
				}
				else if (memcmp(pDst, " ", strlen(" ")) == 0)
				{//スペースを示す行の場合
					pDst = GetLineTop(pDst);
				}
				else
				{
					break;
				}
			}
		}
		else if (memcmp(pDst, " ", strlen(" ")) == 0)
		{//スペースを示す行の場合
			while (1)
			{
				if (memcmp(pDst, "#", strlen("#")) == 0)
				{//コメントを示す行の場合
					fgets(pDst, 256, pFile);
					break;
				}
				else if (memcmp(pDst, "\n", strlen("\n")) == 0)
				{//改行を示す行の場合
					fgets(pDst, 256, pFile);
					break;
				}
				else if (memcmp(pDst, "\t", strlen("\t")) == 0)
				{//タブを示す行の場合
					pDst = GetLineTop(pDst);
				}
				else if (memcmp(pDst, " ", strlen(" ")) == 0)
				{//スペースを示す行の場合
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
// 空白の処理
//=============================================================================
char *CLoadFileFunction::GetLineTop(char *pScr)
{
	while (1)
	{
		if (memcmp(pScr, " ", strlen(" ")) == 0)
		{//スペースを示す行の場合
			pScr += strlen(" ");
		}
		else if (memcmp(pScr, "\t", strlen("\t")) == 0)
		{//タブを示す行の場合
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
// 先頭文字を消して頭出し処理
//=============================================================================
char *CLoadFileFunction::HeadPutout(char * pDest, char * pHead)
{
	pDest = GetLineTop(pDest);
	pDest += strlen(pHead);

	return pDest;
}

//=============================================================================
// 必要な文字列抜き出し処理
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
// 同じ文字列か判定処理
//=============================================================================
int CLoadFileFunction::Memcmp(char * pScr, char * pCmp)
{
	return memcmp(pScr, pCmp, strlen(pCmp));
}

//=============================================================================
// 文字数カウント処理
//=============================================================================
int CLoadFileFunction::PopString(char *pScr, char *pDst)
{
	int nCount = 0;
	while (1)
	{
		nCount += 1;	//カウンターを加算
		pScr += 1;		//頭出し
		if (memcmp(pScr, " ", strlen(" ")) == 0)
		{//スペースを示す行の場合
			pScr = "\0";	//NULLにする
			nCount += 1;	//カウンターを加算
			break;
		}
		else if (memcmp(pScr, "\t", strlen("\t")) == 0)
		{//スペースを示す行の場合
			pScr = "\0";	//NULLにする
			nCount += 1;	//カウンターを加算する
			break;
		}
	}
	strcpy(pDst, pScr);
	return  nCount;
}

//=============================================================================
// 整数型の読み込み処理
//=============================================================================
int CLoadFileFunction::ReadInt(char * pScr, char * pHead)
{
	int nData = 0;
	pScr = HeadPutout(pScr, pHead);
	nData = atoi(pScr);
	return	nData;
}

//=============================================================================
// 浮動小数型の読み込み処理
//=============================================================================
float CLoadFileFunction::ReadFloat(char * pScr, char * pHead)
{
	float fData = 0;
	pScr = HeadPutout(pScr, pHead);
	fData = (float)atof(pScr);
	return	fData;
}

//=============================================================================
// Bool型の読み込み処理
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
// Vector3の読み込み処理
//=============================================================================
D3DXVECTOR3 CLoadFileFunction::ReadVector3(char * pScr, char * pHead)
{
	int nWord = 0;
	char aStr[256];
	D3DXVECTOR3 vector3 = INITIALIZE_D3DXVECTOR3;

	pScr = HeadPutout(pScr, pHead);
	//X座標
	vector3.x = (float)atof(pScr);

	//Y座標
	nWord = PopString(pScr, aStr);
	pScr += nWord;
	vector3.y = (float)atof(pScr);

	//Z座標
	nWord = PopString(pScr, aStr);
	pScr += nWord;
	vector3.z = (float)atof(pScr);
	return	vector3;
}
