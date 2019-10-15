//*****************************************************************************
//
//     �X�̏���[icefield.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _ICEFIELD_H_
#define _ICEFIELD_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "object3D.h"

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CMeshField;

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define ICE_DOWN_FIELD (-10.0f)   // �X�����ۂɃ��b�V���t�B�[���h���ǂꂾ�������邩

//*****************************************************************************
//    �X�̒n�ʃN���X�̒�`
//*****************************************************************************
class CIceField : public CObject3D
{
public:    // �N�ł��A�N�Z�X�\
	CIceField(int nPriority = 3, OBJTYPE objType = OBJTYPE_ICEFIELD);
	~CIceField();

	static CIceField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPDIRECT3DTEXTURE9 pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nXBlock, int nZBlock, int nTexIdx, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPDIRECT3DTEXTURE9 pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nXBlock, int nZBlock, int nTexIdx);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetTexIdx(const int nTexIdx);
	int GetTexIdx(void);
	CMeshField *GetMeshField(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void CreateBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth);

	int        m_nTexIdx;       // �g�p���Ă���e�N�X�`���̔ԍ�
	CMeshField *m_pMeshField;   // ���b�V���t�B�[���h�N���X�^�̃|�C���^
};

#endif