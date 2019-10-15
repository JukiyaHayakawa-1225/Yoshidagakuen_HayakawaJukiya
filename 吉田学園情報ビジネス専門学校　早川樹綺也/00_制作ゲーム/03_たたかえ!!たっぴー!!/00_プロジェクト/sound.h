//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CFileLoader;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CSound
{
public:    // �N�ł��A�N�Z�X�\
	CSound();
	~CSound();
	HRESULT Init(HWND hWnd, char *pFileName);
	void Uninit(void);
	HRESULT PlaySound(int nIdx, bool bPause = false);
	void StopSound(int nIdx, bool bPause = false);
	void StopSound(void);
	void SetVoluem(int nIdx, float fValue);

private:   // ���̃N���X�������A�N�Z�X�\
	HRESULT LoadSoundData(void);
	void LoadSoundScript(CFileLoader *pFileLoader, char *pStr);
	void LoadSoundSet(CFileLoader *pFileLoader, char *pStr, int nCntSound);
	HRESULT CreatePointer(void);
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	char m_aFileName[256];                      // �T�E���h�f�[�^�̃X�N���v�g�t�@�C����
	IXAudio2 *m_pXAudio2;						// XAudio2�I�u�W�F�N�g�ւ̃|�C���^
	IXAudio2MasteringVoice *m_pMasteringVoice;	// �}�X�^�[�{�C�X�ւ̃|�C���^
	IXAudio2SourceVoice **m_apSourceVoice;    	// �\�[�X�{�C�X�ւ̃|�C���^
	BYTE **m_apDataAudio;					    // �I�[�f�B�I�f�[�^�ւ̃|�C���^
	DWORD *m_apSizeAudio;					    // �I�[�f�B�I�f�[�^�T�C�Y�ւ̃|�C���^
	int m_nNumSound;                            // �T�E���h�f�[�^�̐�

	//------------------------
	//  �e���f�ނ̃p�����[�^
	//------------------------
	typedef struct
	{
		char *pFilename;	     // �t�@�C����
		int nCntLoop;		     // ���[�v�J�E���g(-1�Ń��[�v�Đ�)
		int nPlayPositioned;     // �O��̍Đ��ʒu
	} SOUNDPARAM;
	SOUNDPARAM *m_apSoundParam;   // ���f�ނ̃p�����[�^�ւ̃|�C���^
};
#endif
