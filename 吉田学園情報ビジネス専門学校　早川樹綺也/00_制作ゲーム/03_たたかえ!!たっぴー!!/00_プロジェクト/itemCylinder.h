//*****************************************************************************
//
//     �A�C�e���p�̉~������[itemCylinder.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _ITEMCYLINDER_H_
#define _ITEMCYLINDER_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "meshCylinder.h"

//*****************************************************************************
//    �A�C�e���p�̉~���N���X�̒�`
//*****************************************************************************
class CItemCylinder : public CMeshCylinder
{
public:    // �N�ł��A�N�Z�X�\
	typedef enum
	{
		STATE_NONE = 0,
		STATE_DEATH,
		STATE_MAX
	}STATE;

	CItemCylinder(int nPriority = 3, OBJTYPE objType = OBJTYPE_ITEMCYLINDER);
	~CItemCylinder();

	static CItemCylinder *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void SetState(const STATE state);
	STATE GetState(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void Normal(void);
	void Death(void);

	STATE m_State;    // ���
};

#endif