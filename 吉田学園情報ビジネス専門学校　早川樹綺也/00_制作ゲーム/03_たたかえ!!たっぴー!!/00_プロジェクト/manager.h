//=============================================================================
//
// �}�l�[�W������ [manager.h]
// Author : Jukiya Hayakawa & Hodaka Niwa
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

//�O���錾
class CFileLoader;
class CInputKeyboard;
class CInputMouse;
class CInputJoyStick;
class CXInput;
class CSound;
class CCamera;
class CLight;
class CDebugProc;
class CClient;
class CBasemode;
class CTitle;
class CCharaSelect;
class CTutorial;
class CGame;
class CResult;
class CFade;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CManager
{
public:	//�N������A�N�Z�X�\
	typedef enum
	{
		MODE_NONE = -1,
		MODE_TITLE,
		MODE_CHARASELECT,
		MODE_GAME,
		MODE_TUTORIAL,
		MODE_RESULT,
		MODE_MAX,
	}MODE;

	//�����o�֐�
	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�ÓI�����o�֐�
	static void CreateClient(void);
	static void ClientUpdate(void);
	static void ReleaseClient(void);

	static CRenderer		*GetRenderer(void);
	static CInputKeyboard	*GetKeyboard(void);
	static CInputMouse      *GetMouse(void);
	static CInputJoyStick   *GetJoypad(void);
	static CXInput          *GetXInput(void);
	static CSound			*GetSound(void);
	static CClient          *GetClient(void);
	static CBasemode        *GetBaseMode(void);
	static CTitle           *GetTitle(void);
	static CCharaSelect     *GetCharaSelect(void);
	static CTutorial        *GetTutorial(void);
	static CGame            *GetGame(void);
	static CResult          *GetResult(void);
	static MODE				GetMode(void);
	static void				SetMode(MODE mode);
	static CFade			*GetFade(void);

protected:	//�����Ɣh���N���X�������A�N�Z�X�\

private:	//�����������A�N�Z�X�\
	void LoadSystem(HWND hWnd);
	void LoadSystemScript(HWND hWnd, CFileLoader *pFileLoader, char *pStr);
	void CreateRenderer(HWND hWnd, bool bWindow);
	void CreateInput(HINSTANCE hInstance, HWND hWnd);
	void CreateKeyboard(HINSTANCE hInstance, HWND hWnd);
	void CreateMouse(HINSTANCE hInstance, HWND hWnd);
	void CreateJoyStick(HINSTANCE hInstance, HWND hWnd);
	void CreateXInput(void);
	void CreateSound(HWND hWnd, char *pFileName);
	void CreateFade(void);
	void CreateDebugProc(void);

	//�ÓI�����o�ϐ�
	static CRenderer		*m_pRenderer;		// �����_�����O�̃|�C���^
	static CInputKeyboard	*m_pInputKeyboard;	// �L�[�{�[�g�̃|�C���^
	static CInputMouse      *m_pMouse;          // �}�E�X�N���X�^�̃|�C���^
	static CInputJoyStick   *m_pInputJoypad;	// �W���C�p�b�h�̃|�C���^
	static CXInput          *m_pXInput;         // XInput�N���X�^�̃|�C���^
	static CSound			*m_pSound;			// �T�E���h�̃|�C���^
	static CClient          *m_pClient;         // �N���C�A���g�̃|�C���^
	static CDebugProc		*m_pDebugproc;		// �f�o�b�O�̃|�C���^
	static MODE				m_mode;				// ���[�h�̏��
	static CBasemode		*m_pBasemode;		// �x�[�X���[�h�̃|�C���^
	static CFade			*m_pFade;			// �t�F�[�h�̃|�C���^
	static char             m_aServerAddr[256]; // �T�[�o�[��IP�A�h���X
};
#endif