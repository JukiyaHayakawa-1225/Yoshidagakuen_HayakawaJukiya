//=============================================================================
//
// ���C������ [main.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//�O���錾
class CScene;
class CRenderer;
class CManager;

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS			//�x���Ώ�
#define _WINSOCK_DEPRECATED_NO_WARNINGS // Winsock�x���΍�}�N��
#include <windows.h>
#include <process.h>                    // �}���`�X���b�h�v���O�����ɕK�v
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "d3dx9.h"						//�`�揈���ɕK�v
#include "xaudio2.h"					//���y�����ɕK�v
#define DIRECTINPUT_VERSION (0x0800)	// �x���Ώ��p
#include "dinput.h"						//���͏����ɕK�v
#include <XInput.h>
#include "resource.h"

//�f�o�b�O�����ɕK�v
#include <crtdbg.h>
#ifdef _DEBUG
#define _malloc_dbg(X, _NORMAL_BLOCK, __FILE__, __LINE__)
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define _CRTDBG_MAP_ALLOC
#endif // _DEBUG

//*****************************************************************************
// ���C�u�����t�@�C���̃����N
//*****************************************************************************
#pragma comment (lib, "ws2_32.lib")     // winsock�ɕK�v
#pragma comment(lib,"d3d9.lib")			// �`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")		// [d3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")		// DirectX�R���|�[�l���g�i���i�j�g�p�ɕK�v
#pragma comment(lib,"winmm.lib")		// �V�X�e�������擾�ɕK�v
#pragma comment (lib,"dinput8.lib")		// ���͏����ɕK�v
#pragma comment (lib, "XInput.lib")     // XInput�ɕK�v

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCREEN_WIDTH	 (1280)		           // �E�C���h�E�̕�
#define SCREEN_HEIGHT	 (720)		           // �E�C���h�E�̍���
#define MASS_BLOCK_X     (19)                  // �}�X�̕�����(��)
#define MASS_BLOCK_Z     (15)                  // �}�X�̕�����(�c)
#define MASS_SIZE_X      (75.0f)               // �}�X�̃T�C�Y(��)
#define MASS_SIZE_Z      (75.0f)               // �}�X�̃T�C�Y(�c)
#define MASS_SIZE_X_HALF (MASS_SIZE_X / 2.0f)  // �}�X�̃T�C�Y�̔���(��)
#define MASS_SIZE_Z_HALF (MASS_SIZE_Z / 2.0f)  // �}�X�̃T�C�Y�̔���(�c)
#define MAX_NUM_PLAYER   (2)                   // �v���C���[�̑���
#define INITIALIZE_D3DXVECTOR3 (D3DXVECTOR3(0.0f,0.0f,0.0f))

// 2D�|���S���p���_�t�H�[�}�b�g( ���_���W[2D] / ���_�J���[ / �e�N�X�`�����W )
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// �R�c�|���S�����_�t�H�[�}�b�g( ���_���W[3D]/ ���_�@�� / ���_�J���[ / �e�N�X�`�����W )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// DirectX�ō쐬���ꂽ�|�C���^�̊J��(�}�N���Ŋy�ɂ��Ă���)
#define DIRECT_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	float rhw;			//1.0f�ŌŒ�
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`�����W
} VERTEX_2D;

// �R�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 nor;	//�@���x�N�g��
	D3DCOLOR col;		// ���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`��
} VERTEX_3D;

#endif
