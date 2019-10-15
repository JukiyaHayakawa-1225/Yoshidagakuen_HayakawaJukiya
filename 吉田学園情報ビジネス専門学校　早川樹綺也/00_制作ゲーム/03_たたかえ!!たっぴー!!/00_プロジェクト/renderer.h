//=============================================================================
//
// �����_�����O���� [renderer.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CRenderer
{
public:	//�N������A�N�Z�X�\
	//�����o�֐�
	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetRenderTarget(const LPDIRECT3DTEXTURE9 pTexture);
	void ResetRenderTarget(void);
	void ReleaseMultRenderer(void);
	LPDIRECT3DDEVICE9 GetDevice(void);

private:	//�����������A�N�Z�X�\
	LPDIRECT3D9        m_pD3D;        // Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9  m_pD3DDevice;  // Device�I�u�W�F�N�g(�`��ɕK�v)
	LPDIRECT3DSURFACE9 m_pRenderOrg;  // �f�t�H���g�̃����_�����O�^�[�Q�b�g�ւ̃|�C���^
	LPDIRECT3DSURFACE9 m_pBuffOrg;    // �f�t�H���g�̊e��o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 m_pTextureMT;  // �}���`�����_�����O�Ɏg�p����e�N�X�`���ւ̃|�C���^
	LPDIRECT3DSURFACE9 m_pRenderMT;   // �}���`�����_�����O�Ɏg�p���郌���_�����O�^�[�Q�b�g�ւ̃|�C���^
	LPDIRECT3DSURFACE9 m_pBuffMT;     // �}���`�����_�����O�Ɏg�p����e��o�b�t�@�ւ̃|�C���^
};
#endif