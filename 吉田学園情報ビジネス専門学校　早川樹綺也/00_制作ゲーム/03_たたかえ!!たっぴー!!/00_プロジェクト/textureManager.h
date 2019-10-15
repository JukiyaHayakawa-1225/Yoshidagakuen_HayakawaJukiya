//=============================================================================
//
// �e�N�X�`���}�l�[�W���[�̏��� [textureManager.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

//�O���錾
#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTextureManager
{
public:	//�N������A�N�Z�X�\
		//�����o�֐�
	CTextureManager();
	~CTextureManager();
	void Init(void);
	void Uninit(void);

	void SetTexture(LPDIRECT3DTEXTURE9 pTexture, int nIdx);
	void SetNumTexture(int nNumTexture);
	void SetFileName(char *pFileName, int nIdx);

	LPDIRECT3DTEXTURE9 GetTexture(int nIdx);
	int GetNumTexture(void);
	char *GetFileName(int nIdx);

	//�ÓI�����o�֐�
	static CTextureManager *Create(int nNumTexture);

private:	//�����������A�N�Z�X�\
	//�����o�ϐ�
	LPDIRECT3DTEXTURE9		*m_pTexture;	//�e�N�X�`���̃|�C���^
	char					**m_pFileName;	//�t�@�C����
	int						m_nNumTexture;	//�e�N�X�`���̑���

};
#endif
