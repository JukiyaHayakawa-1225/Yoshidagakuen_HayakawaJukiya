//*****************************************************************************
//
//     �T�[�o�[�̏���[server.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SERVER_H_
#define _SERVER_H_

//*****************************************************************************
//     �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define MAX_SERVER_CONNECTING_NUM  (2)     // �T�[�o�[�ɃA�N�Z�X�ł���ő�l��
#define MAX_SERVER_DATA            (4096)  // �T�[�o�[���Ǘ��ł���f�[�^��

// �T�[�o�[�̃R�}���h
#define CD_NONE                "CD_NONE"
#define CD_HELLO               "CD_HELLO"
#define CD_SEND_CLIENT         "CD_SEND_CLIENT"
#define CD_DELETE              "CD_DELETE"
#define CD_SE_DATA             "CD_SE_DATA"
#define CD_CHARA_DATA          "CD_CHARA_DATA"
#define CD_TUTO_DATA           "CD_TUTO_DATA"
#define CD_GAME_DATA           "CD_GAME_DATA"
#define CD_USERID              "CD_USERID"

// �N���C�A���gID�ǂݎ��p
#define USER_ID                "USER_ID"

//*****************************************************************************
//     �T�[�o�[�N���X�̒�`
//*****************************************************************************
class CServer
{
public:    // �N�ł��A�N�Z�X�\
	CServer();
	~CServer();

	static HRESULT ServerStartUp(void);
	static void ServerCleanUp(void);
	static CServer *Create(void);
	static unsigned int WINAPI WaitConnectAllHandler(void *pServer);
	static unsigned int WINAPI WaitConnectHandler(void *pServer);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void WaitConnect(void);
	void WaitConnectAll(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void ExChangeMessage(int nIdxClient);
	void Command_Hello(int nIdxClient);
	void Command_Send_Client(int nIdxClient);
	void Command_DeleteClient(int nIdxClient);
	void Command_SendDataClient(int nIdxClient);
	void Command_CharaDataClient(int nIdxClient);
	void Command_GameDataClient(int nIdxClient);
	void Command_UserIdClient(int nIdxClient);

	int m_nNumConnect;                                                     // ���ݐڑ�����Ă���N���C�A���g�̐l��
	struct sockaddr_in m_addrServer;                                       // �\�P�b�g�A�h���X(�T�[�o�[)�̏��
	struct sockaddr_in m_addrClient[MAX_SERVER_CONNECTING_NUM];            // �\�P�b�g�A�h���X(�N���C�A���g)�̏��
	SOCKET m_sockServer;                                                   // �\�P�b�g(�T�[�o�[�p)
	SOCKET m_sockClient[MAX_SERVER_CONNECTING_NUM];                        // �\�P�b�g(�N���C�A���g�p)
	int m_nLengthAddrClient[MAX_SERVER_CONNECTING_NUM];                    // �\�P�b�g�A�h���X(�N���C�A���g)�̃T�C�Y
	char m_aReceiveMessage[MAX_SERVER_CONNECTING_NUM][MAX_SERVER_DATA];    // ��M�f�[�^
	char m_aSendMessage[MAX_SERVER_CONNECTING_NUM][MAX_SERVER_DATA];       // ���M�f�[�^
	int m_nLengthData[MAX_SERVER_CONNECTING_NUM];                          // ��M�f�[�^�̃T�C�Y
	bool m_bConnected[MAX_SERVER_CONNECTING_NUM];                          // �N���C�A���g�Ɛڑ��������ǂ���

	// �}���`�X���b�h�v���O�����Ɏg�p
	HANDLE m_hWaitConnectAllHander;
	HANDLE m_hWaitConnectHander[MAX_SERVER_CONNECTING_NUM];
};

//*****************************************************************************
//    �N���C�A���g�N���X�̒�`
//*****************************************************************************
class CClient
{
public:    // �N�ł��A�N�Z�X�\
	CClient();
	~CClient();

	static CClient *Create(char *pSeverAddr);
	static unsigned int WINAPI WaitConnectHandler(void *pClient);

	HRESULT Init(char *pSeverAddr);
	void Uninit(void);
	void Update(void);
	void Print(char *fmt, ...);

	bool GetConnected(void);
	int GetClientId(void);
	char *GetReceiveMessage(void);
	int GetLengthData(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void SetServerAddr(char *pServerAddr);
	void SetClientUserId(void);
	void WaitConnect(void);
	void ExChangeMessage(void);
	void Command_Hello(void);
	void Command_Send_Client(void);
	void Command_CharaClient(void);
	void Command_TutorialClient(void);
	void Command_GameClient(void);
	void Command_DeleteClient(void);

	void ReleaseHandller();

	int m_nClientId;                          // �����̃N���C�A���g�ԍ�(�v���C���[�̔ԍ��ƕR�Â�)
	char m_aServerAddr[64];                   // �T�[�o�[��IP�A�h���X
	struct sockaddr_in m_addrServer;          // �\�P�b�g�A�h���X(�T�[�o�[)�̏��
	SOCKET m_sockClient;                      // �\�P�b�g(�N���C�A���g�p)
	char m_aReceiveMessage[MAX_SERVER_DATA];  // ��M�f�[�^
	char m_aSendMessage[MAX_SERVER_DATA];     // ���M�f�[�^
	int m_nLengthData;                        // ��M�f�[�^�̃T�C�Y
	bool m_bConnected;                        // �T�[�o�[�ɐڑ��������ǂ���
	char m_aData[MAX_SERVER_DATA];            // ���M������e��f�[�^(�e��f�[�^������Ƀ��b�Z�[�W������)

	// �}���`�X���b�h�v���O�����Ɏg�p
	HANDLE m_hWaitConnectHander;
};

#endif
