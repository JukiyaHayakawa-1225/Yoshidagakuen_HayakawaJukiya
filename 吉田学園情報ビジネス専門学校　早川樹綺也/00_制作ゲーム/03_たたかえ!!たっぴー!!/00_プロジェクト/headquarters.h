//*****************************************************************************
//
//     �i�ߕ��̏���[headquarters.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _HEADQUARTERS_H_
#define _HEADQUARTERS_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "object3D.h"

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CModel;

//*****************************************************************************
//    �i�ߕ��N���X�̒�`
//*****************************************************************************
class CHeadQuarters : public CObject3D
{
public:    // �N�ł��A�N�Z�X�\
	CHeadQuarters(int nPriority = 3, OBJTYPE objType = OBJTYPE_HEADQUARTERS);
	~CHeadQuarters();

	static CHeadQuarters *Create(int nAreaX, int nAreaZ, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, int nPriority = 3);

	HRESULT Init(int nAreaX, int nAreaZ, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(CScene *pScene);

	void SetModel(CModel *pModel);
	void SetArea(int nAreaX, int nAreaZ);

	CModel *GetModel(void);
	int GetAreaX(void);
	int GetAreaZ(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int m_nAreaX, m_nAreaZ; // ���ǂ̃G���A�ɂ��邩�𐔒l����������
	CModel *m_pModel;       // ���f���N���X�^�̃|�C���^
};

#endif