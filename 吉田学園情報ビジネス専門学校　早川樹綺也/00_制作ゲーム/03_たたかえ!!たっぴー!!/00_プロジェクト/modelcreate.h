//=============================================================================
//
// ���f���������� [modelcreate.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _MODELCREATE_H_
#define _MODELCREATE_H_

#include "main.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CModelCreate
{
public:    //�N������A�N�Z�X�\
	//�����o�֐�
	CModelCreate();
	~CModelCreate();
	HRESULT Init(void);
	void Uninit(void);

	void SetNumModel(int nNumModel);
	void SetFileName(char *pFileName, int nIdx);
	void SetMesh(LPD3DXMESH pMesh, int nIdx);
	void SetBuffMat(LPD3DXBUFFER pBuffMat, int nIdx);
	void SetNumMat(DWORD nNumMat, int nIdx);
	void SetVtxMax(D3DXVECTOR3 nNumMat, int nIdx);
	void SetVtxMin(D3DXVECTOR3 nNumMat, int nIdx);
	void SetMaterial(LPDIRECT3DDEVICE9 pDevice,LPD3DXBUFFER pBuffMat, DWORD nNumMat, int nIdx);

	int	GetNumModel(void);
	char *GetFileName(int nIdx);
	LPD3DXMESH GetMesh(int nIdx);
	LPD3DXBUFFER GetBuffMat(int nIdx);
	DWORD GetNumMat(int nIdx);
	D3DXVECTOR3 GetVtxMax(int nIdx);
	D3DXVECTOR3 GetVtxMin(int nIdx);
	LPDIRECT3DTEXTURE9	*GetTexture(int nIdx);

	//�ÓI�����o�ϐ�
	static CModelCreate *Create(int nNumModel);

private:   //�����������A�N�Z�X�\
	//�����o�ϐ�
	int                   m_nNumModel;    // ���f������
	char				  **m_pFileName;  // �t�@�C����
	LPD3DXMESH            *m_pMesh;       // ���b�V���̃|�C���^
	LPD3DXBUFFER          *m_pBuffMat;    // �}�e���A���̃|�C���^
	DWORD                 *m_pNumMat;     // �}�e���A�������̃|�C���^
	D3DXVECTOR3           *m_pVtxMax;     // ���_���W�ő�l�̃|�C���^
	D3DXVECTOR3           *m_pVtxMin;     // ���_���W�ŏ��l�̃|�C���^
	LPDIRECT3DTEXTURE9	  **m_pTexture;	  // �e�N�X�`���̃|�C���^
};

#endif