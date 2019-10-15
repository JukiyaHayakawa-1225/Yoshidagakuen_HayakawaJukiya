//*****************************************************************************
//
//     �����蔻��p�����f���̏���[boxCollider.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _BOXCOLLIDER_H_
#define _BOXCOLLIDER_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �����蔻��p�����f���N���X�̒�`
//*****************************************************************************
class CBoxCollider
{
public:    // �N�ł��A�N�Z�X�\
	CBoxCollider();
	~CBoxCollider();

	static CBoxCollider *Create(D3DXVECTOR3 pos, float fWidth, float fHeight, float fDepth, bool bReturnFlag);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 ColRange, bool *pLand = NULL);

	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetDepth(const float fDepth);
	void SetReturnFlag(const bool bReturnFlag);

	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXMATRIX GetMtxWorld(void);
	float GetWidth(void);
	float GetHeight(void);
	float GetDepth(void);
	bool GetReturnFlag(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	D3DXVECTOR3             m_Pos;          // ���W
	D3DXVECTOR3             m_Rot;          // ����
	D3DXMATRIX              m_MtxWorld;     // ���[���h�}�g���b�N�X
	float                   m_fWidth;       // ���̉���
	float                   m_fHeight;      // ���̍���
	float                   m_fDepth;       // ���̉��s
	bool                    m_bReturnFlag;  // ���������ۂɖ߂����ǂ���
	D3DXVECTOR3             m_VecPos[8];    // �����f���̒��_���W
	WORD                    m_Index[24];    // �����f���̒��_�ԍ�
	ID3DXLine*              m_pLine;        // ���`��p�C���X�^���X�ւ̃|�C���^
};

#endif