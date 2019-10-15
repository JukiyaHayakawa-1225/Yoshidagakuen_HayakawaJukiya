//=============================================================================
//
// ���C������ [main.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		"AppClass"			            // �E�C���h�E�̃N���X��
#define WINDOW_NAME		"��������!!�����ҁ[!!"          // �E�C���h�E�̃L���v�V������
#define WINDOW_MODE     (false)                          // �E�B���h�E�̃��[�h(false�ɂ���ƃt���X�N���[��)
#define WINDOW_ICON     (MAKEINTRESOURCE(IDI_ICON1))    // �E�B���h�E�̃A�C�R�����ʎq
#define WINDOW_SM_ICON  (MAKEINTRESOURCE(IDI_ICON1))    // �E�B���h�E�̃X���[���A�C�R�����ʎq

#ifdef _DEBUG
#define MEMORY_CHECK    (false)                         // ���������[�N�`�F�b�N�����邩�ǂ���
#endif

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************


//=============================================================================
// ���C���֐�
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	if (MEMORY_CHECK)
	{// ���������[�N�`�F�b�N������Ȃ�
	 // ���������[�N�`�F�b�N�̃t���O�𗧂Ă�
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}
#endif

	CManager *pManager = NULL;	//�}�l�[�W���̃|�C���^

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		LoadIcon(hInstance, WINDOW_ICON),
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		(HICON)LoadImage(hInstance, WINDOW_SM_ICON,  IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED)
	};
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	HWND hWnd;
	MSG msg;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	int nCntFPS = 0;

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɕK�v�ȃE�B���h�E���W���v�Z
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindow(CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						(rect.right - rect.left),
						(rect.bottom - rect.top),
						NULL,
						NULL,
						hInstance,
						NULL);

	if (pManager == NULL)
	{
		pManager = new CManager;	//�}�l�[�W���̓��I�m��
	}

	if (pManager != NULL)
	{
		// �}�l�[�W���̏���������
		if (FAILED(pManager->Init(hInstance, hWnd, WINDOW_MODE)))
		{
			return -1;
		}
	}

	// ����\��ݒ�
	timeBeginPeriod(1);

	// �t���[���J�E���g������
	dwCurrentTime =
	dwFrameCount = 0;
	dwExecLastTime =
	dwFPSLastTime = timeGetTime();

	// �E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// ���b�Z�[�W���[�v
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();	// ���݂̎��Ԃ��擾
			if((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5�b���ƂɎ��s
				// FPS���Z�o
				nCntFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;	// ���݂̎��Ԃ�ۑ�
				dwFrameCount = 0;
			}

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 1/60�b�o��
				dwExecLastTime = dwCurrentTime;	// ���݂̎��Ԃ�ۑ�

				// FPS�`��
				CDebugProc::Print(1, "FPS:%d\n", nCntFPS);

				// �X�V����
				pManager->Update();

				// �`�揈��
				pManager->Draw();

				dwFrameCount++;
			}
		}
	}

	if (pManager != NULL)
	{
		// �I������
		pManager->Uninit();
		delete pManager;
		pManager = NULL;
	}

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// ����\��߂�
	timeEndPeriod(1);

#ifdef _DEBUG
	if (MEMORY_CHECK)
	{// ���������[�N�`�F�b�N�����Ă����Ȃ�
	 // ���̎��_�ŊJ������Ă��Ȃ��������̏��̕\��
		_CrtDumpMemoryLeaks();
	}
#endif

	return (int)msg.wParam;
}

//=============================================================================
// �E�C���h�E�v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE:
		break;

	case WM_DESTROY:
		CManager::ReleaseClient();
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:				// [ESC]�L�[�������ꂽ
			CManager::ReleaseClient();
			DestroyWindow(hWnd);	// �E�B���h�E��j������悤�w������
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}