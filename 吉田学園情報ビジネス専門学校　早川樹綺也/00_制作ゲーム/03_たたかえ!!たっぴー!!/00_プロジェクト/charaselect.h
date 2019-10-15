//=============================================================================
//
// �L�����Z���N�g�̏��� [charaselect.h]
// Author : Hodaka Niwa & Jukiya Hayakawa
//
//=============================================================================
#ifndef _CHARASELECT_H_
#define _CHARASELECT_H_

#include "main.h"
#include "basemode.h"
#include "game.h"

//�O���錾
class CFileLoader;
class CFileSaver;
class CScene2D;
class CScene2DFlash;

// �v���C���[�p
class CPlayer;
class CPlayerManager;
class CScene3D;
class CCharaCylinder;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_CHARASELECT_PREPARATION_TEX (2)   // �������e�N�X�`���̐�

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CCharaSelect : public CBasemode
{
public:	// �N������A�N�Z�X�\
	//--------------
	// ���
	//--------------
	typedef enum
	{
		STATE_NONE = -1,
		STATE_SELECT,
		STATE_CHARACHANGE_TO_LEFT,
		STATE_CHARACHANGE_TO_RIGHT,
		STATE_WAIT_PARTNER,
		STATE_STAGE_SELECT,
		STATE_WAIT_STAGESELECT,
		STATE_END,
		STATE_END_TITLE,
		STATE_MAX
	}STATE;

	// �����o�֐�
	CCharaSelect();
	~CCharaSelect();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(const STATE state);
	STATE GetState(void);

	// �ÓI�����o�֐�
	static CCharaSelect *Create(void);
	static int GetPlayerNumber(int nIdx);
	static int GetStageType(void);
	static void SetStageType(const int nType);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:	// �����������A�N�Z�X�\
	void ClearVariable(void);

	// �����p�֐�
	void CreateTexture(int nNumTex);
	void CreateCamera(void);
	void CreatePlayer(void);
	void ChangePedestalTexPos(int nCntPlayer, int nCntType);
	void CreateRenderTexture(void);
	void CreateStagePolygon(void);
	void CreatePlayerDataPointer(void);
	void CreatePlayerManager(void);

	// �J���p�֐�
	void ReleaseBg(void);
	void ReleasePlayer(void);
	void ReleaseRenderTexture(void);
	void ReleaseRenderPolygon(void);
	void ReleasePreparation(void);
	void ReleasePlayerNumber(void);
	void ReleaseYouPolygon(void);
	void ReleaseStagePolygon(void);
	void ReleasePlayerDataPointer(void);
	void ReleasePlayerManager(void);

	// �T�[�o�[�Ƃ̌�M�p�֐�
	void SetDataToServer(void);
	void GetDataFromServer(void);

	// ��Ԃɂ�鏈�������p�֐�
	void SelectUpdate(int nIdx);
	void CharaChangeToLeftUpdate(int nIdx);
	void CharaChangeToRightUpdate(int nIdx);
	void WaitPartnerUpdate(int nIdx);
	void StageSelectUpdate(int nIdx);
	void WaitStageSelectUpdate(int nIdx);
	void EndUpdate(void);
	void EndTitleUpdate(void);
	void WaitInputToChangeChara(int nIdx);
	void WaitInputToStageSelect(int nIdx);
	void ChangeNoneStagePolygon(int nSelect);
	void ChangeSelectStagePolygon(int nSelect);
	void CheckCharaCylinderCol(int nIdx);
	void CircleRotation(int nIdx);
	void ChangeState_WaitPartnerToStageSelect(int nIdx);

	// �`��p�֐�
	void CharaDraw(int nCntTex);
	void TextureDraw(void);

	// �X�N���v�g�ǂݍ��ݗp�֐�
	void LoadSystem(void);
	void LoadSystemScript(CFileLoader *pFileLoader, char *pStr);
	void LoadTexFileName(char *pStr, int nCntTex);
	void LoadPlayerDataFileName(char *pStr, int nCntPlayerData);
	void LoadBgPolygon(CFileLoader *pFileLoader, char *pStr);
	void LoadPreparation(CFileLoader *pFileLoader, char *pStr, int nCntPre);
	void LoadPlayerNumber(CFileLoader *pFileLoader, char *pStr, int nCntPlayerNum);
	void LoadYouPolygon(CFileLoader *pFileLoader, char *pStr);

	// �����o�ϐ�
	STATE m_State[MAX_NUM_PLAYER];                              // ���
	int m_nStateCounter;                                        // ��Ԃ��Ǘ�����J�E���^�[
	LPDIRECT3DTEXTURE9 m_apRenderTexture[MAX_NUM_PLAYER];       // �����_�����O�Ɏg�p����e�N�X�`���ւ̃|�C���^
	float m_fSelectRot[MAX_NUM_PLAYER];                         // �~�`��]�Ɏg�p����x�N�g���̊p�x
	float m_fAddSelectRot[MAX_NUM_PLAYER];                      // �~�`��]���ǂꂭ�炢�i�񂾂�
	CScene2D *m_pBg;                                            // �w�i�p�|���S��
	CScene2D *m_apPolygonPlayer[MAX_NUM_PLAYER];                // �v���C���[��`�悵���e�N�X�`����`�悷��̂Ɏg�p
	CScene2D *m_apPlayerNumber[MAX_NUM_PLAYER];                 // �v���C���[�ԍ���\������|���S��
	CScene2D *m_apPreparation[MAX_NUM_PLAYER];                  // ���������ǂ����\������|���S��
	CScene2D *m_pYouPolygon;                                    // �����̃v���C���[�ԍ���\������|���S��
	CScene2DFlash *m_apStage[CGame::MAPTYPE_MAX];               // �}�b�v�̎�ނ�\������|���S��
	float m_fYouPolygonPosXDef;                                 // �����̃v���C���[�ԍ���\������|���S���̃f�t�H���gX���W
	int m_nPreparationTexIdx[MAX_CHARASELECT_PREPARATION_TEX];  // �������|���S���̎g�p����e�N�X�`���ԍ�
	int m_nPlayerNumberTexIdx[MAX_NUM_PLAYER];                  // �v���C���[�ԍ���\������|���S���̎g�p����e�N�X�`���ԍ�
	int m_nNumPlayerData;                                       // �ǂݍ��ރv���C���[�̎�ނ̐�
	char **m_aPlayerDataFileName;                               // �ǂݍ��ރv���C���[�f�[�^�̃t�@�C����

	// �ÓI�����o�ϐ�
	static int m_nSelectPlayer[MAX_NUM_PLAYER];                 // ���Ԃ̃v���C���[��I��ł��邩
	static int m_nSelectStage;                                  // �g�p����}�b�v�̎��

	// �v���C���[�\���p
	CPlayer **m_pPlayer[MAX_NUM_PLAYER];						// �v���C���[�̑I��p�̃|���S��
	CPlayerManager	**m_pPlayerManager[MAX_NUM_PLAYER];			// �v���C���[�}�l�[�W���[�̃|�C���^���
	CScene3D **m_pPlayerPedestal[MAX_NUM_PLAYER];               // �v���C���[�̑���\���p�|���S��
	CCharaCylinder **m_pCharaCylinder[MAX_NUM_PLAYER];          // �v���C���[�p�~���N���X�ւ̃|�C���^
};
#endif