//*****************************************************************************
//
//     �T�[�o�[�̏���[server.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "server.h"
#include "serverlib.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************


//*****************************************************************************
//    �ÓI�����o�ϐ�
//*****************************************************************************


//*****************************************************************************
//    CServer�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CServer::CServer()
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CServer::~CServer()
{

}

//=============================================================================
//    WinSock�̏���������
//=============================================================================
HRESULT CServer::ServerStartUp(void)
{
	WSADATA wsaData;  // winsock�̏��
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	return S_OK;
}

//=============================================================================
//    WinSock�̏I������
//=============================================================================
void CServer::ServerCleanUp(void)
{
	WSACleanup();
}

//=============================================================================
//    ��������
//=============================================================================
CServer *CServer::Create(void)
{
	CServer *pServer = NULL;      // �T�[�o�[�N���X�^�̃|�C���^
	if (pServer == NULL)
	{// ����������ɂȂ��Ă���
		pServer = new CServer;
		if (pServer != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pServer->Init()))
			{// �������Ɏ��s����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}
	else
	{// �C���X�^���X�𐶐��ł��Ȃ�����
		return NULL;
	}

	return pServer;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CServer::Init(void)
{
	// �ڑ��l��������
	m_nNumConnect = 0;

	// �n���h����������
	m_hWaitConnectAllHander = 0;

	// �\�P�b�g�̍쐬
	m_sockServer = socket(AF_INET, SOCK_STREAM, 0);

	// �\�P�b�g�̐ݒ�
	m_addrServer.sin_family = AF_INET;
	m_addrServer.sin_port = htons(12345);
	m_addrServer.sin_addr.S_un.S_addr = INADDR_ANY;

	// �\�P�b�g�ɖ��̂�t�^
	bind(m_sockServer, (struct sockaddr *)&m_addrServer, sizeof(m_addrServer));

	// �N���C�A���g����̐ڑ��v����҂Ă��Ԃɂ���
	listen(m_sockServer, 5);
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM; nCntConnect++)
	{
		m_hWaitConnectHander[nCntConnect] = 0;
		m_bConnected[nCntConnect] = false;
	}

	// �X���b�h��؂�ւ��Đڑ��ҋ@����������
	m_hWaitConnectAllHander = (HANDLE)_beginthreadex(NULL, 0, WaitConnectAllHandler, this, 0, NULL);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CServer::Uninit(void)
{
	// �}���`�X���b�h�I���̂��߂ɃN���C�A���g�𐶐����j������
	CClient *pClient[MAX_SERVER_CONNECTING_NUM] = {};
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM - m_nNumConnect; nCntConnect++)
	{
		pClient[nCntConnect] = CClient::Create("127.0.1.1");
	}
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM - m_nNumConnect; nCntConnect++)
	{
		if (pClient[nCntConnect] != NULL)
		{
			pClient[nCntConnect]->Uninit();
			delete pClient[nCntConnect];
			pClient[nCntConnect] = NULL;
		}
	}

	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM; nCntConnect++)
	{
		// �\�P�b�g(�N���C�A���g)�̊J��
		closesocket(m_sockClient[nCntConnect]);

		// �n���h���̊J��
		if (m_hWaitConnectHander[nCntConnect] != 0)
		{
			CloseHandle(m_hWaitConnectHander[nCntConnect]);
		}
	}

	// �n���h���̊J��
	if (m_hWaitConnectAllHander != 0)
	{
		CloseHandle(m_hWaitConnectAllHander);
	}

	// �\�P�b�g(�T�[�o�[)�̊J��
	closesocket(m_sockServer);
}

//=============================================================================
//    �X�V����
//=============================================================================
void CServer::Update(void)
{
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM; nCntConnect++)
	{
		if (m_bConnected[nCntConnect] == true)
		{// �N���C�A���g�Ɛڑ��ł���
			CDebugProc::Print(1, "%d�ԂƐڑ�����!!\n", nCntConnect);
			ExChangeMessage(nCntConnect);
		}
		else
		{// �N���C�A���g�Ɛڑ��ł��Ă��Ȃ�
			CDebugProc::Print(1, "�T�[�o�[��t��...\n");
		}
	}

	CDebugProc::Print(1, "�T�[�o�[�ڑ��l�� : %d�l", m_nNumConnect);
}

//=============================================================================
//    �ڑ��ҋ@����
//=============================================================================
void CServer::WaitConnect(void)
{
	// �N���C�A���g����̐ڑ��v�����󂯕t����
	SOCKET sockClient;
	struct sockaddr_in addrClient;
	int nLenghtAddrClient = 0;
	nLenghtAddrClient = sizeof(addrClient);
	sockClient = accept(m_sockServer, (struct sockaddr *)&addrClient, &nLenghtAddrClient);
	if (sockClient == INVALID_SOCKET)
	{
		return;
	}

	// �J���Ă���ꏊ�ɏ����i�[
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM; nCntConnect++)
	{
		if (m_bConnected[nCntConnect] == false)
		{
			m_sockClient[nCntConnect] = sockClient;
			m_addrClient[nCntConnect] = addrClient;
			m_nLengthAddrClient[nCntConnect] = nLenghtAddrClient;
			memset(m_aSendMessage[nCntConnect], 0, sizeof(m_aSendMessage[nCntConnect]));
			m_bConnected[nCntConnect] = true;
			m_nNumConnect++;
			break;
		}
	}

	return;
}

//=============================================================================
//    �ڑ��ҋ@����
//=============================================================================
void CServer::WaitConnectAll(void)
{
	// �N���C�A���g����̐ڑ��v�����󂯕t����
	for (int nCntConnect = 0; nCntConnect < MAX_SERVER_CONNECTING_NUM; nCntConnect++)
	{
		m_nLengthAddrClient[nCntConnect] = sizeof(m_addrClient);
		m_sockClient[nCntConnect] = accept(m_sockServer, (struct sockaddr *)&m_addrClient[nCntConnect], &m_nLengthAddrClient[nCntConnect]);
		if (m_sockClient[nCntConnect] == INVALID_SOCKET)
		{
			return;
		}
		memset(m_aSendMessage[nCntConnect], 0, sizeof(m_aSendMessage[nCntConnect]));
		m_bConnected[nCntConnect] = true;
		m_nNumConnect++;
	}

	return;
}

//=============================================================================
//    �ڑ��ҋ@�������ĂԊ֐�(�}���`�X���b�h�Ή��̂���)
//=============================================================================
unsigned int WINAPI CServer::WaitConnectHandler(void *pServer)
{
	reinterpret_cast<CServer*>(pServer)->WaitConnect();     // CServer�^�ɃL���X�g���ČĂ�
	return 0;
}

//=============================================================================
//    �ڑ��ҋ@�������ĂԊ֐�(�}���`�X���b�h�Ή��̂���)
//=============================================================================
unsigned int WINAPI CServer::WaitConnectAllHandler(void *pServer)
{
	reinterpret_cast<CServer*>(pServer)->WaitConnectAll();  // CServer�^�ɃL���X�g���ČĂ�
	return 0;
}

//=============================================================================
//    �N���C�A���g�Ƃ̃��b�Z�[�W��������
//=============================================================================
void CServer::ExChangeMessage(int nIdxClient)
{
	// �N���C�A���g����f�[�^(���b�Z�[�W)����M
	memset(m_aReceiveMessage[nIdxClient], 0, sizeof(m_aReceiveMessage[nIdxClient]));
	m_nLengthData[nIdxClient] = recv(m_sockClient[nIdxClient], m_aReceiveMessage[nIdxClient], MAX_SERVER_DATA, 0);

	// ���b�Z�[�W��\������
	CDebugProc::Print(1, "%s\n��M�o�C�g��[%d]\n", m_aReceiveMessage[nIdxClient], m_nLengthData[nIdxClient]);

	// ���b�Z�[�W����R�}���h��K�p
	if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_HELLO) == 0)
	{
		Command_Hello(nIdxClient);
	}
	else if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_DELETE) == 0)
	{
		Command_DeleteClient(nIdxClient);
	}
	else if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_SEND_CLIENT) == 0)
	{
		Command_Send_Client(nIdxClient);
		strcat(m_aSendMessage[nIdxClient], "���b�Z�[�W���M");
	}
	else if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_SE_DATA) == 0)
	{
		Command_SendDataClient(nIdxClient);

		strcat(m_aSendMessage[nIdxClient], " ");
	}
	else if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_USERID) == 0)
	{
		Command_UserIdClient(nIdxClient);
	}
	else if (CServerLib::Memcmp(m_aReceiveMessage[nIdxClient], CD_NONE) == 0
		&& CServerLib::Memcmp(m_aReceiveMessage[(nIdxClient + 1) % 2], CD_SEND_CLIENT) != 0)
	{
		strcat(m_aSendMessage[nIdxClient], "???");
	}

	// �N���C�A���g�փ��b�Z�[�W�𑗐M
	send(m_sockClient[nIdxClient], m_aSendMessage[nIdxClient], strlen(m_aSendMessage[nIdxClient]), 0);
	memset(m_aSendMessage[nIdxClient], 0, sizeof(m_aSendMessage[nIdxClient]));
}

//=============================================================================
//    Hello���b�Z�[�W���N���C�A���g�֑��M���鏈��
//=============================================================================
void CServer::Command_Hello(int nIdxClient)
{
	// �N���C�A���g�ւ̃��b�Z�[�W��ݒ�
	strcat(m_aSendMessage[nIdxClient], "Hello Client");
}

//=============================================================================
//    �N���C�A���g����N���C�A���g�փ��b�Z�[�W���M���鏈��
//=============================================================================
void CServer::Command_Send_Client(int nIdxClient)
{
}

//=============================================================================
//    �N���C�A���g�Ƃ̌�M���I�����鏈��
//=============================================================================
void CServer::Command_DeleteClient(int nIdxClient)
{
	// �\�P�b�g(�N���C�A���g)�̊J��
	closesocket(m_sockClient[nIdxClient]);
	m_bConnected[nIdxClient] = false;
	m_nNumConnect--;
	m_hWaitConnectHander[nIdxClient] = (HANDLE)_beginthreadex(NULL, 0, WaitConnectHandler, this, 0, NULL);
	CDebugProc::ClearString();
	CDebugProc::Print(1, "%d�Ԃ̐ڑ����r�₦�܂����E�E�E\n", nIdxClient);
	CDebugProc::Print(1, "�ڑ���t�J�n\n");
}

//=============================================================================
//    �N���C�A���g����f�[�^��ǂݎ�鏈��
//=============================================================================
void CServer::Command_SendDataClient(int nIdxClient)
{
	float aPlayerPos[3];        // �v���C���[�̍��W
	char aStr[MAX_SERVER_DATA];
	char *pStr;
	pStr = &m_aReceiveMessage[nIdxClient][0];

	// ��M�������b�Z�[�W����f�[�^��ǂݎ��
	pStr= CServerLib::ReadFloat3(pStr, CD_SE_DATA, &aPlayerPos[0]);

	// ��M�������W��ʃN���C�A���g�֑��M���邽�߂̃��b�Z�[�W���쐬
	strcpy(m_aSendMessage[(nIdxClient + 1) % 2], CServerLib::WriteFloat3(&aPlayerPos[0], aStr));
}

//=============================================================================
//    �N���C�A���g����L�����f�[�^��ǂݎ�鏈��
//=============================================================================
void CServer::Command_CharaDataClient(int nIdxClient)
{

}

//=============================================================================
//    �N���C�A���g����Q�[���f�[�^��ǂݎ�鏈��
//=============================================================================
void CServer::Command_GameDataClient(int nIdxClient)
{

}

//=============================================================================
//    �N���C�A���g�֔ԍ��𑗐M���鏈��
//=============================================================================
void CServer::Command_UserIdClient(int nIdxClient)
{
	// �N���C�A���g�ԍ��𑗐M
	char aStr[MAX_SERVER_DATA];
	strcpy(m_aSendMessage[nIdxClient], USER_ID);
	strcat(m_aSendMessage[nIdxClient], CServerLib::WriteInt(nIdxClient, aStr));
}


//*****************************************************************************
//    CClient�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CClient::CClient()
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CClient::~CClient()
{

}

//=============================================================================
//    ��������
//=============================================================================
CClient *CClient::Create(char *pSeverAddr)
{
	CClient *pClient = NULL;      // �N���C�A���g�N���X�^�̃|�C���^
	if (pClient == NULL)
	{// ����������ɂȂ��Ă���
		pClient = new CClient;
		if (pClient != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pClient->Init(pSeverAddr)))
			{// �������Ɏ��s����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}
	else
	{// �C���X�^���X�𐶐��ł��Ȃ�����
		return NULL;
	}

	return pClient;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CClient::Init(char *pSeverAddr)
{
	// �N���C�A���gID������
	m_nClientId = 0;

	// �n���h����������
	m_hWaitConnectHander = 0;

	// �f�[�^�N���A
	memset(m_aData, 0, MAX_SERVER_DATA);
	memset(m_aReceiveMessage, 0, MAX_SERVER_DATA);
	memset(m_aSendMessage, 0, MAX_SERVER_DATA);

	// IP�A�h���X��ݒ�
	if (pSeverAddr == NULL) return E_FAIL;
	SetServerAddr(pSeverAddr);

	// �X���b�h��؂�ւ��Đڑ��ҋ@����������
	m_hWaitConnectHander = (HANDLE)_beginthreadex(NULL, 0, WaitConnectHandler, this, 0, NULL);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CClient::Uninit(void)
{
	// �T�[�o�[�Ƃ̐ڑ����I��
	Command_DeleteClient();

	// �n���h���̊J��
	ReleaseHandller();

	// �\�P�b�g(�N���C�A���g)�̊J��
	closesocket(m_sockClient);
}

//=============================================================================
//    �X�V����
//=============================================================================
void CClient::Update(void)
{
	if (m_bConnected == true)
	{// �T�[�o�[�Ɛڑ��ł���
		ExChangeMessage();
	}
	else
	{// �T�[�o�[�Ɛڑ��ł��Ă��Ȃ�
		CDebugProc::Print(1, "�T�[�o�[��������܂���E�E�E\n");
	}

	// �f�[�^�N���A
	memset(m_aData, 0, MAX_SERVER_DATA);
}

//=============================================================================
//    �}���`�X���b�h�n���h�����J�����鏈��
//=============================================================================
void CClient::ReleaseHandller()
{
	// �n���h���̊J��
	if (m_hWaitConnectHander != 0)
	{
		CloseHandle(m_hWaitConnectHander);
	}
}

//=============================================================================
//    �ڑ��ҋ@�������ĂԊ֐�(�}���`�X���b�h�Ή��̂���)
//=============================================================================
unsigned int WINAPI CClient::WaitConnectHandler(void *pClient)
{
	reinterpret_cast<CClient*>(pClient)->WaitConnect();     // CClient�^�ɃL���X�g���ČĂ�
	return 0;
}

//=============================================================================
//    �T�[�o�[IP�A�h���X��ݒ肷�鏈��
//=============================================================================
void CClient::SetServerAddr(char *pServerAddr)
{
	strcpy(m_aServerAddr, pServerAddr);
}

//=============================================================================
//    �T�[�o�[�Ƃ̐ڑ��ҋ@����
//=============================================================================
void CClient::WaitConnect(void)
{
	// �\�P�b�g�̍쐬
	m_sockClient = socket(AF_INET, SOCK_STREAM, 0);

	// �\�P�b�g�̐ݒ�
	m_addrServer.sin_family = AF_INET;
	m_addrServer.sin_port = htons(12345);
	m_addrServer.sin_addr.S_un.S_addr = inet_addr(m_aServerAddr);

	if (connect(m_sockClient, (struct sockaddr *)&m_addrServer, sizeof(m_addrServer)) == SOCKET_ERROR)
	{// �T�[�o�[�Ɛڑ����s
		CDebugProc::Print(1, "�ڑ����r�₦�܂����E�E�E\n");
		CDebugProc::Print(1, "�ڑ��J�n\n");
		m_bConnected = false;
	}
	else
	{// �T�[�o�[�Ɛڑ�����
		m_nLengthData = 0;

		// �N���C�A���gID���擾����
		SetClientUserId();
		m_bConnected = true;
	}
}

//=============================================================================
//   �����̃N���C�A���g�ԍ����T�[�o�[�����M���鏈��
//=============================================================================
void CClient::SetClientUserId(void)
{
	m_nClientId = 0;  // ���[�U�[ID������

	// �T�[�o�[�փf�[�^(���b�Z�[�W)�𑗐M
	memset(m_aSendMessage, 0, sizeof(m_aSendMessage));
	strcpy(m_aSendMessage, CD_USERID);
	send(m_sockClient, m_aSendMessage, strlen(m_aSendMessage), 0);

	// �T�[�o�[����f�[�^(���b�Z�[�W)����M
	memset(m_aReceiveMessage, 0, MAX_SERVER_DATA);
	m_nLengthData = recv(m_sockClient, &m_aReceiveMessage[0], MAX_SERVER_DATA, 0);

	// ���b�Z�[�W���N���C�A���g�ԍ��ɕϊ�
	m_nClientId = CServerLib::ReadInt(m_aReceiveMessage, USER_ID);
}

//=============================================================================
//    �T�[�o�[�Ƃ̃��b�Z�[�W��������
//=============================================================================
void CClient::ExChangeMessage(void)
{
	// �T�[�o�[�֑��郁�b�Z�[�W���쐬
	memset(m_aSendMessage, 0, sizeof(m_aSendMessage));

	// ��ʂɂ���ď����킯
	if (CManager::GetMode() == CManager::MODE_CHARASELECT)
	{// �L�����I����ʂ�����
		Command_CharaClient();
	}
	else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{// �`���[�g���A����ʂ�����
		Command_TutorialClient();
	}
	else if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[����ʂ�����
		Command_GameClient();
	}

	// �T�[�o�[�փf�[�^(���b�Z�[�W)�𑗐M
	CDebugProc::Print(1, "���������b�Z�[�W : %s\n", m_aSendMessage);
	send(m_sockClient, m_aSendMessage, strlen(m_aSendMessage), 0);

	// �T�[�o�[����f�[�^(���b�Z�[�W)����M
	memset(m_aReceiveMessage, 0, MAX_SERVER_DATA);
	m_nLengthData = recv(m_sockClient, &m_aReceiveMessage[0], MAX_SERVER_DATA, 0);

	if (m_nLengthData > 1)
	{
		// ���b�Z�[�W��\������
		CDebugProc::Print(1, "�󂯎�������b�Z�[�W : %s\n", m_aReceiveMessage);

		// ���b�Z�[�W��\������
		CDebugProc::Print(1, "��M�o�C�g��[%d]\n", m_nLengthData);
	}
}

//=============================================================================
//    �f�[�^���o�C�i�������鏈��
//=============================================================================
void CClient::Print(char *fmt, ...)
{
	va_list list;              // �ϒ�����(... �̈������󂯎�邱�Ƃ��ł���)
	char aStr[MAX_SERVER_DATA];// ������i�[�p�ϐ�

	va_start(list, fmt);       // �ϒ��������X�g��������(fmt�ȉ��̈�����o�^)
	vsprintf(aStr, fmt, list); // �������W���o�͂ɂ̂��Ƃ��ď����o��
	va_end(list);              // �������X�g���N���A
	strcat(m_aData, aStr);     // �i�[����������������o�ϐ��̌��ɂ�������
}

//=============================================================================
//    Hello���b�Z�[�W���󂯎�鏈��
//=============================================================================
void CClient::Command_Hello(void)
{
	strcat(m_aSendMessage, CD_HELLO);
}

//=============================================================================
//    �ʂ̃N���C�A���g�փ��b�Z�[�W�𑗐M���鏈��
//=============================================================================
void CClient::Command_Send_Client(void)
{
	strcat(m_aSendMessage, CD_SEND_CLIENT);
}

//=============================================================================
//    �N���C�A���g�փ��b�Z�[�W�𑗐M���鏈��(�L�����I����ʂ̎�)
//=============================================================================
void CClient::Command_CharaClient(void)
{
	strcpy(m_aSendMessage, CD_CHARA_DATA);
	strcat(m_aSendMessage, m_aData);
}

//=============================================================================
//    �N���C�A���g�փ��b�Z�[�W�𑗐M���鏈��(�`���[�g���A����ʂ̎�)
//=============================================================================
void CClient::Command_TutorialClient(void)
{
	strcpy(m_aSendMessage, CD_TUTO_DATA);
	strcat(m_aSendMessage, m_aData);
}

//=============================================================================
//    �N���C�A���g�փ��b�Z�[�W�𑗐M���鏈��(�Q�[����ʂ̎�)
//=============================================================================
void CClient::Command_GameClient(void)
{
	strcpy(m_aSendMessage, CD_GAME_DATA);
	strcat(m_aSendMessage, m_aData);
}

//=============================================================================
//    �T�[�o�[�Ƃ̐ڑ����I�����鏈��
//=============================================================================
void CClient::Command_DeleteClient(void)
{
	// �T�[�o�[�֑��郁�b�Z�[�W���쐬
	memset(m_aSendMessage, 0, sizeof(m_aSendMessage));
	strcpy(m_aSendMessage, CD_DELETE);

	// �T�[�o�[�փf�[�^(���b�Z�[�W)�𑗐M
	send(m_sockClient, m_aSendMessage, strlen(m_aSendMessage), 0);

	// �T�[�o�[����f�[�^(���b�Z�[�W)����M
	memset(m_aReceiveMessage, 0, MAX_SERVER_DATA);
	m_nLengthData = recv(m_sockClient, &m_aReceiveMessage[0], MAX_SERVER_DATA, 0);
}

//=============================================================================
//    �T�[�o�[�ɐڑ��ł������ǂ����擾���鏈��
//=============================================================================
bool CClient::GetConnected(void)
{
	return m_bConnected;
}

//=============================================================================
//    �N���C�A���gID���擾���鏈��
//=============================================================================
int CClient::GetClientId(void)
{
	return m_nClientId;
}

//=============================================================================
//    �N���C�A���gID���擾���鏈��
//=============================================================================
char *CClient::GetReceiveMessage(void)
{
	return m_aReceiveMessage;
}

//=============================================================================
//    ��M�o�C�g�����擾���鏈��
//=============================================================================
int CClient::GetLengthData(void)
{
	return m_nLengthData;
}