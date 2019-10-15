//=============================================================================
//
// �I�u�W�F�N�g���� [scene.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "scene.h"
#include "debugproc.h"
//=============================================================================
//	�ÓI�����o�ϐ��錾
//=============================================================================
int CScene::m_nNumAll = 0;								//�I�u�W�F�N�g�̑���
int CScene::m_nNumPriority[NUM_PRIORITY] = {};			//�D�揇�ʕʂ̑���
CScene *CScene::m_apTop[NUM_PRIORITY] = {};				//�擪�̃I�u�W�F�N�g�ւ̃|�C���^
CScene *CScene::m_apCur[NUM_PRIORITY] = {};				//���݁i�Ō���j�̃I�u�W�F�N�g�ւ̃|�C���^

//=============================================================================
// �I�u�W�F�N�g�̃R���X�g���N�^
//=============================================================================
CScene::CScene(int nPriority, OBJTYPE objtype)
{
	m_bDeath = false;							//����ł��Ȃ���Ԃɂ���
	m_nPriority = nPriority;					//�D�揇�ʂ̐ݒ�

	if (m_apTop[m_nPriority] == NULL)
	{//�܂��擪�ɃI�u�W�F�N�g���Ȃ��ꍇ
		m_apTop[m_nPriority] = this;			//�擪�ɑ��
		m_apTop[m_nPriority]->m_pNext = NULL;
		m_apTop[m_nPriority]->m_pPrev = NULL;
	}
	if (m_apCur[m_nPriority] != NULL)
	{//�܂����݂ɃI�u�W�F�N�g���Ȃ��ꍇ
		m_apCur[m_nPriority]->m_pNext = this;			//���݂̎��ɍ��̃A�h���X����
	}
	this->m_pPrev = m_apCur[m_nPriority];				//���̑O�̃A�h���X�Ɍ��݂���
	this->m_pNext = NULL;								//���̎��̃A�h���X��NULL�ɂ���
	m_apCur[m_nPriority] = this;						//���݂ɍ��̃A�h���X����
	m_objType = objtype;
	m_nNumPriority[m_nPriority]++;						//�w�肳�ꂽ�D�揇�ʂ����Z
	m_nNumAll++;										//�����̏����l
}

//=============================================================================
// �I�u�W�F�N�g�̃f�X�g���N�^
//=============================================================================
CScene::~CScene()
{

}

//=============================================================================
// �S�ẴI�u�W�F�N�g�̊J������
//=============================================================================
void CScene::ReleaseAll(void)
{
	CScene *pSceneTop;				//�����s����ϐ�
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		pSceneTop = m_apTop[nCntPriority];						//���̎��s�ϐ��ɏ����ʒu����

		while (pSceneTop != NULL)
		{//NULL�ɂȂ�܂ŌJ��Ԃ�
			CScene *pSceneNext;									//���̕ϐ�
			pSceneNext = pSceneTop->m_pNext;					//���̕ϐ��ɍ��̎��̃A�h���X����
			if (pSceneTop->m_objType != OBJTYPE_FADE)
			{
				pSceneTop->Uninit();							//�I������
			}
			pSceneTop = pSceneNext;								//���Ɏ��s������̂����̕ϐ��ɑ������
		}

		pSceneTop = m_apTop[nCntPriority];						//���̎��s�ϐ��ɏ����ʒu����
		while (pSceneTop != NULL)
		{//NULL�ɂȂ�܂ŌJ��Ԃ�
			CScene *pSceneNext;									//���̕ϐ�
			pSceneNext = pSceneTop->m_pNext;					//���̕ϐ��ɍ��̎��̃A�h���X����
			if (pSceneTop->m_bDeath == true)
			{//���S�t���O�������Ă��������������
				pSceneTop->Death();
			}
			pSceneTop = pSceneNext;								//���Ɏ��s������̂����̕ϐ��ɑ������
		}
		m_apTop[nCntPriority] = NULL;
		m_apCur[nCntPriority] = NULL;
	}
	m_nNumAll = 0;		//�G�̑�����������
}

//=============================================================================
// �S�ẴI�u�W�F�N�g�̍X�V����
//=============================================================================
void CScene::UpdateAll(void)
{
	CScene *pSceneTop;				//�����s����ϐ�
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		pSceneTop = m_apTop[nCntPriority];			//���̎��s�ϐ��ɏ����ʒu����

		while (pSceneTop != NULL)
		{//NULL�ɂȂ�܂ŌJ��Ԃ�
			CScene *pSceneNext;					//���̕ϐ�
			pSceneNext = pSceneTop->m_pNext;	//���̕ϐ��ɍ��̎��̃A�h���X����
			pSceneTop->Update();				//�I������
			pSceneTop = pSceneNext;				//���Ɏ��s������̂����̕ϐ��ɑ������
		}
	}

	//�J���`�F�b�N
	DeathCheck();
}

//=============================================================================
// �S�ẴI�u�W�F�N�g�̕`�揈��
//=============================================================================
void CScene::DrawAll(void)
{
	CScene *pSceneTop;						//�����s����ϐ�
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		pSceneTop = m_apTop[nCntPriority];					//���̎��s�ϐ��ɏ����ʒu����

		while (pSceneTop != NULL)
		{//NULL�ɂȂ�܂ŌJ��Ԃ�
			CScene *pSceneNext;					//���̕ϐ�
			pSceneNext = pSceneTop->m_pNext;	//���̕ϐ��ɍ��̎��̃A�h���X����
			pSceneTop->Draw();					//�I������
			pSceneTop = pSceneNext;				//���Ɏ��s������̂����̕ϐ��ɑ������
		}
	}

}

//=============================================================================
// �S�ẴI�u�W�F�N�g�̊J���`�F�b�N����
//=============================================================================
void CScene::DeathCheck(void)
{
	CScene *pSceneTop;				//�����s����ϐ�
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		pSceneTop = m_apTop[nCntPriority];						//���̎��s�ϐ��ɏ����ʒu����
		while (pSceneTop != NULL)
		{//NULL�ɂȂ�܂ŌJ��Ԃ�
			CScene *pSceneNext;					//���̕ϐ�
			pSceneNext = pSceneTop->m_pNext;	//���̕ϐ��ɍ��̎��̃A�h���X����
			if (pSceneTop->m_bDeath == true)
			{//���S�t���O�������Ă��������������
				pSceneTop->Death();
			}
			pSceneTop = pSceneNext;				//���Ɏ��s������̂����̕ϐ��ɑ������
		}
	}
}

//=============================================================================
// �w�肳�ꂽ�D�揇�ʂ̃I�u�W�F�N�g�̊J���`�F�b�N����
//=============================================================================
void CScene::DeathCheck(int nPriority)
{
	CScene *pSceneTop;				//�����s����ϐ�
	CScene *pSceneNext;					//���̕ϐ�
	pSceneTop = m_apTop[nPriority];						//���̎��s�ϐ��ɏ����ʒu����
	while (pSceneTop != NULL)
	{//NULL�ɂȂ�܂ŌJ��Ԃ�
		pSceneNext = pSceneTop->m_pNext;	//���̕ϐ��ɍ��̎��̃A�h���X����
		if (pSceneTop->m_bDeath == true)
		{//���S�t���O�������Ă��������������
			pSceneTop->Death();
		}
		pSceneTop = pSceneNext;				//���Ɏ��s������̂����̕ϐ��ɑ������
	}
}

//=============================================================================
// �I�u�W�F�N�g�̐擪�擾����
//=============================================================================
CScene * CScene::GetTop(int nPriority)
{
	return m_apTop[nPriority];
}

//=============================================================================
// �I�u�W�F�N�g�̔j��
//=============================================================================
void CScene::Release(void)
{
	//�t���O�𗧂Ă�
	m_bDeath = true;
}

//=============================================================================
// �I�u�W�F�N�g�̎��S����
//=============================================================================
void CScene::Death(void)
{
	if (m_pPrev != NULL)
	{//�����̑O�̎��̃A�h���X�Ɏ����̎��̃A�h���X����
		m_pPrev->m_pNext = m_pNext;
	}
	if (m_pNext != NULL)
	{//�����̎��̑O�̃A�h���X�Ɏ����̑O�̃A�h���X����
		m_pNext->m_pPrev = m_pPrev;
	}
	if (this == m_apTop[m_nPriority])
	{//�擪�Ɠ����A�h���X�̏ꍇ�擪�̎��̃A�h���X�ɐ擪���ړ�
		m_apTop[m_nPriority] = m_apTop[m_nPriority]->m_pNext;
	}
	if (this == m_apCur[m_nPriority])
	{//���݂Ɠ����A�h���X�̏ꍇ���݂̎��̃A�h���X�Ɍ��݂��ړ�
		m_apCur[m_nPriority] = m_apCur[m_nPriority]->m_pPrev;
	}

	m_nNumPriority[m_nPriority]--;		//�w�肳�ꂽ�D�揇�ʂ̌��Z
	delete this;						//���̃A�h���X���J��
	m_nNumAll--;						//�����̌��Z
}

//=============================================================================
// ��ނ̐ݒu
//=============================================================================
void CScene::SetObjType(OBJTYPE objType)
{
	m_objType = objType;
}

//=============================================================================
// ��ނ̎擾
//=============================================================================
CScene::OBJTYPE CScene::GetObjType(void)
{
	return m_objType;
}

//=============================================================================
// ���̃I�u�W�F�N�g�̃A�h���X�擾����
//=============================================================================
CScene * CScene::GetNext(void)
{
	return m_pNext;
}

//=============================================================================
// �O�̃I�u�W�F�N�g�̃A�h���X�擾����
//=============================================================================
CScene * CScene::GetPrev(void)
{
	return m_pPrev;
}

//=============================================================================
// �����̗D�揇�ʎ擾����
//=============================================================================
int CScene::GetPriority(void)
{
	return m_nPriority;
}