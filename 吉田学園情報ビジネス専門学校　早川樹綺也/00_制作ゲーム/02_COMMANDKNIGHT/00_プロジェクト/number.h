//=============================================================================
//
// �����̏��� [number.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"

#define TEXTURE_NAME008 "data\\TEXTURE\\number001.png"
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CNumber
{
public:
	//�����o�֐�
	CNumber();
	~CNumber();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(D3DXVECTOR3 pos);
	void SetCol(D3DXCOLOR col);
	void SetWidth(float fWidth);
	void SetHeight(float fHeight);

	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR GetCol(void);
	float GetWidth(void);
	float GetHeight(void);

	void SetNumber(int nNumber);

	//�ÓI�����o�֐�
	static CNumber *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);
	static HRESULT Load(void);
	static void Unload(void);

private:
	//�����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9				m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3							m_pos;		// �ʒu
	D3DXCOLOR							m_col;		// �F
	float								m_fWidth;	// ��
	float								m_fHeight;	// ����

	//�ÓI�����o�ϐ�
	static LPDIRECT3DTEXTURE9			m_pTexture;	// �e�N�X�`���ւ̃|�C���^

protected:
};
#endif