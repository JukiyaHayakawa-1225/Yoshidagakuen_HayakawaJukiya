//=============================================================================
//
// �I�u�W�F�N�g���� [scene.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_PRIORITY (10)		//�D�揇�ʂ̑���

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CScene
{
public:	//�N������A�N�Z�X�\
	//=========================================================================
	// �\���̂̒�`
	//=========================================================================
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_SCENE2D,		// �I�u�W�F�N�g2D
		OBJTYPE_BLOSSOMS,       // �Ԃт�
		OBJTYPE_SCENE3D,		// �I�u�W�F�N�g3D
		OBJTYPE_SCENEX,			// �I�u�W�F�N�gX
		OBJTYPE_NUMBER,         // ����
		OBJTYPE_FADE,			// �t�F�[�h
		OBJTYPE_MESHFIELD,		// ���b�V���t�B�[���h
		OBJTYPE_MESHCYLINDER,   // ���b�V���V�����_�[
		OBJTYPE_GOALCYLINDER,   // �S�[���p�~��
		OBJTYPE_ITEMCYLINDER,   // �A�C�e���p�~��
		OBJTYPE_CHARACYLINDER,  // �L�����Z���N�g�p�~��
		OBJTYPE_BILLBOARD,		// �r���{�[�h
		OBJTYPE_OBJECT,			// �I�u�W�F�N�g
		OBJTYPE_3D,             // 3D�I�u�W�F�N�g
		OBJTYPE_BLOCK,          // �u���b�N
		OBJTYPE_CHARACTER,      // �L�����N�^�[
		OBJTYPE_PLAYER,			// �v���C���[
		OBJTYPE_BULLET,         // �e
		OBJTYPE_BULLETEFFECT,   // �e�̃G�t�F�N�g
		OBJTYPE_ENEMY,          // �G
		OBJTYPE_RIVER,          // ��
		OBJTYPE_ICEFIELD,       // �X�̒n��
		OBJTYPE_SPLASH,         // ��
		OBJTYPE_OBJBILLBOARD,   // �r���{�[�h�I�u�W�F�N�g
		OBJTYPE_HEADQUARTERS,   // �i�ߕ�
		OBJTYPE_MESHSPHERE,     // ���b�V���X�t�B�A
		OBJTYPE_MESHRING,       // ���b�V�������O
		OBJTYPE_ITEM,           // �A�C�e��
		OBJTYPE_EMITTER,        // �G�~�b�^�[
		OBJTYPE_PAREMITTER,     // �p�[�e�B�N���G�~�b�^�[
		OBJTYPE_RINGEMITTER,    // �����O�G�t�F�N�g�G�~�b�^�[
		OBJTYPE_PARTICLE,       // �p�[�e�B�N��
		OBJTYPE_EFFECTMANAGER,  // �G�t�F�N�g�}�l�[�W���[
		OBJTYPE_HINAARARE,      // �ЂȂ����
		OBJTYPE_MIST,           // ����
		OBJTYPE_POWERMAP,       // �p���[�}�b�v
		OBJTYPE_MAX,
	} OBJTYPE;

	//�����o�֐�
	CScene(int nPriority, OBJTYPE objtype);
	void SetObjType(OBJTYPE objType);
	OBJTYPE GetObjType(void);
	CScene *GetNext(void);
	CScene *GetPrev(void);
	int GetPriority(void);

	//�������z�֐�
	virtual ~CScene();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	//�ÓI�����o�֐�
	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static void DeathCheck(void);
	static void DeathCheck(int nPriority);
	static CScene *GetTop(int nPriority);

private:	//�����������A�N�Z�X�\
	//�����o�֐�
	void Death(void);

	//�����o�ϐ�
	CScene			*m_pPrev;		//�O�̃I�u�W�F�N�g�ւ̃|�C���^
	CScene			*m_pNext;		//���̃I�u�W�F�N�g�ւ̃|�C���^
	bool			m_bDeath;		//���S�t���O
	int				m_nPriority;	//�D�揇�ʂ̔ԍ�
	OBJTYPE			m_objType;		//�I�u�W�F�N�g�̎��

	//�ÓI�����o�ϐ�
	static CScene	*m_apTop[NUM_PRIORITY];			//�擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CScene	*m_apCur[NUM_PRIORITY];			//���݁i�Ō���j�̃I�u�W�F�N�g�ւ̃|�C���^
	static int		m_nNumAll;						//�I�u�W�F�N�g�̑���
	static int		m_nNumPriority[NUM_PRIORITY];	//�D�揇�ʂ̏��
protected:	//�����Ɣh���N���X�������A�N�Z�X�\
	void Release(void);								//�f�[�^�J��
};
#endif