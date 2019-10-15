//=============================================================================
//
// ���f�������̏��� [modelcreate.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "modelcreate.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CModelCreate::CModelCreate()
{
	m_nNumModel = 0;    //���f���̑���������
	m_pFileName = NULL; //�t�@�C������������
	m_pMesh = NULL;     //���b�V��������
	m_pBuffMat = NULL;  //�}�e���A��������
	m_pNumMat = NULL;   //�}�e���A������������
	m_pVtxMax = NULL;   //���_���W�ő�l������
	m_pVtxMin = NULL;   //���_���W�ŏ��l������
	m_pTexture = NULL;	//�e�N�X�`���̏�����
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CModelCreate::~CModelCreate()
{

}

//=============================================================================
// ��������
//=============================================================================
CModelCreate *CModelCreate::Create(int nNumModel)
{
	CModelCreate *pModelCreate = NULL;  //���f�������|�C���^
	if (pModelCreate == NULL)
	{//NULL�̏ꍇ
		pModelCreate = new CModelCreate;	//���I�m��
		if (pModelCreate != NULL)
		{//NULL�łȂ��ꍇ
			pModelCreate->SetNumModel(nNumModel);  //���f�������ݒu����
			pModelCreate->Init();					//����������
		}
	}
	return pModelCreate;
}

//=============================================================================
// ���f���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CModelCreate::Init(void)
{
	//���b�V���𑍐������I�m��
	m_pMesh = new LPD3DXMESH[m_nNumModel];

	//�}�e���A���𑍐������I�m��
	m_pBuffMat = new LPD3DXBUFFER[m_nNumModel];

	//�}�e���A�������𑍐������I�m��
	m_pNumMat = new DWORD[m_nNumModel];

	//���_���W�ő�l�𑍐������I�m��
	m_pVtxMax = new D3DXVECTOR3[m_nNumModel];

	//���_���W�ŏ��l�𑍐������I�m��
	m_pVtxMin = new D3DXVECTOR3[m_nNumModel];

	if (m_pTexture == NULL)
	{
		//�e�N�X�`���𑍐������I�m��
		m_pTexture = new LPDIRECT3DTEXTURE9*[m_nNumModel];

		if (m_pTexture != NULL)
		{
			for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
			{
				m_pTexture[nCnt] = NULL;
			}
		}
	}

	if (m_pFileName == NULL)
	{//�t�@�C�����̓��I�m��
		m_pFileName = new char*[m_nNumModel];
		if (m_pFileName != NULL)
		{
			for (int nCntTexture = 0; nCntTexture < m_nNumModel; nCntTexture++)
			{//������̓��I�m��
				m_pFileName[nCntTexture] = new char[256];
			}
		}
	}

	return S_OK;
}

//=============================================================================
// ���f���̊J������
//=============================================================================
void CModelCreate::Uninit(void)
{
	//�t�@�C�����̔j��
	if (m_pFileName != NULL)
	{
		for (int nCntTexture = 0; nCntTexture < m_nNumModel; nCntTexture++)
		{//�e�N�X�`���̑������J��Ԃ�
			delete[] m_pFileName[nCntTexture];
			m_pFileName[nCntTexture] = NULL;
		}
		delete[] m_pFileName;
		m_pFileName = NULL;
	}

	// �e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
		{
			if (m_pTexture[nCnt] != NULL)
			{
				for (int nCntMat = 0; nCntMat < (int)m_pNumMat[nCnt]; nCntMat++)
				{
					if (m_pTexture[nCnt][nCntMat] != NULL)
					{
						m_pTexture[nCnt][nCntMat]->Release();
						m_pTexture[nCnt][nCntMat] = NULL;
					}
				}
			}
			delete[] m_pTexture[nCnt];
			m_pTexture[nCnt] = NULL;
		}
		delete[] m_pTexture;
		m_pTexture = NULL;
	}

	// ���b�V���J��
	if (m_pMesh != NULL)
	{
		for (int nCntMesh = 0; nCntMesh < m_nNumModel; nCntMesh++)
		{
			m_pMesh[nCntMesh]->Release();
		}
		delete[] m_pMesh;
		m_pMesh = NULL;
	}

	// �}�e���A���J��
	if (m_pBuffMat != NULL)
	{
		for (int nCntBuffMat = 0; nCntBuffMat < m_nNumModel; nCntBuffMat++)
		{
			m_pBuffMat[nCntBuffMat]->Release();
		}
		delete[] m_pBuffMat;
		m_pBuffMat = NULL;
	}

	// �}�e���A�������J��
	if (m_pNumMat != NULL)
	{
		delete[] m_pNumMat;
		m_pNumMat = NULL;
	}

	// ���_���W�̍ő�l�J��
	if (m_pVtxMax != NULL)
	{
		delete[] m_pVtxMax;
		m_pVtxMax = NULL;
	}

	// ���_�̍ŏ��l���̊J��
	if (m_pVtxMin != NULL)
	{
		delete[] m_pVtxMin;
		m_pVtxMin = NULL;
	}
}

//=============================================================================
// �ǂݍ��ރ��f���̐���ݒ肷��
//=============================================================================
void CModelCreate::SetNumModel(int nNumModel)
{
	m_nNumModel = nNumModel;
}

//=============================================================================
// �t�@�C�����̐ݒu����
//=============================================================================
void CModelCreate::SetFileName(char *pFileName, int nIdx)
{
	strcpy(m_pFileName[nIdx], pFileName);
}

//=============================================================================
// ���f���̃��b�V������ݒ肷��
//=============================================================================
void CModelCreate::SetMesh(LPD3DXMESH pMesh, int nIdx)
{
	m_pMesh[nIdx] = pMesh;

	if (m_pMesh[nIdx] != NULL)
	{// ���f�����ǂݍ��߂�
	 // �ŏ��l�ƍő�l��������
		m_pVtxMin[nIdx] = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
		m_pVtxMax[nIdx] = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);

		int nNumVtx;     // ���_��
		DWORD sizeFVF;   // ���_�t�H�[�}�b�g�̃T�C�Y
		BYTE *pVtxBuff;  // ���_�o�b�t�@�ւ̃|�C���^

		// ���_�����l��
		nNumVtx = m_pMesh[nIdx]->GetNumVertices();

		// ���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(m_pMesh[nIdx]->GetFVF());

		// ���_�o�b�t�@�����b�N
		m_pMesh[nIdx]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{// ���ׂĂ̒��_���r���ă��f���̍ŏ��l,�ő�l�𔲂��o��
			D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;   // ���_���W�̑��

			if (Vtx.x < m_pVtxMin[nIdx].x)
			{// ���o����X���W�̍ŏ��l�����܂ł̂���������
				m_pVtxMin[nIdx].x = Vtx.x;  // �ŏ��l�X�V
			}
			if (Vtx.y < m_pVtxMin[nIdx].y)
			{// ���o����Y���W�̍ŏ��l�����܂ł̂���������
				m_pVtxMin[nIdx].y = Vtx.y;  // �ŏ��l�X�V
			}
			if (Vtx.z < m_pVtxMin[nIdx].z)
			{// ���o����Z���W�̍ŏ��l�����܂ł̂���������
				m_pVtxMin[nIdx].z = Vtx.z;  // �ŏ��l�X�V
			}
			if (Vtx.x > m_pVtxMax[nIdx].x)
			{// ���o����X���W�̍ő�l�����܂ł̂����傫��
				m_pVtxMax[nIdx].x = Vtx.x;  // �ő�l�X�V
			}
			if (Vtx.y > m_pVtxMax[nIdx].y)
			{// ���o����Y���W�̍ő�l�����܂ł̂����傫��
				m_pVtxMax[nIdx].y = Vtx.y;  // �ő�l�X�V
			}
			if (Vtx.z > m_pVtxMax[nIdx].z)
			{// ���o����Z���W�̍ő�l�����܂ł̂����傫��
				m_pVtxMax[nIdx].z = Vtx.z;  // �ő�l�X�V
			}

			pVtxBuff += sizeFVF;  // �T�C�Y���|�C���^��i�߂�
		}
		// ���_�o�b�t�@���A�����b�N
		m_pMesh[nIdx]->UnlockVertexBuffer();
	}
}

//=============================================================================
// ���f���̃}�e���A������ݒ肷��
//=============================================================================
void CModelCreate::SetBuffMat(LPD3DXBUFFER pBuffMat, int nIdx)
{
	m_pBuffMat[nIdx] = pBuffMat;
}

//=============================================================================
// ���f���̃}�e���A���̐�����ݒ肷��
//=============================================================================
void CModelCreate::SetNumMat(DWORD nNumMat, int nIdx)
{
	m_pNumMat[nIdx] = nNumMat;
}

//=============================================================================
// ���f���̒��_�̍ő�l����ݒ肷��
//=============================================================================
void CModelCreate::SetVtxMax(D3DXVECTOR3 VtxMax, int nIdx)
{
	m_pVtxMax[nIdx] = VtxMax;
}

//=============================================================================
// ���f���̒��_�̍ŏ��l����ݒ肷��
//=============================================================================
void CModelCreate::SetVtxMin(D3DXVECTOR3 VtxMin, int nIdx)
{
	m_pVtxMin[nIdx] = VtxMin;
}

//=============================================================================
// �}�e���A���̐ݒu����
//=============================================================================
void CModelCreate::SetMaterial(LPDIRECT3DDEVICE9 pDevice,LPD3DXBUFFER pBuffMat, DWORD nNumMat, int nIdx)
{
	m_pBuffMat[nIdx] = pBuffMat;	//�}�e���A���̑��
	m_pNumMat[nIdx] = nNumMat;		//�}�e���A���̑����̑��

	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	if (pBuffMat != NULL)
	{
		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();

		//�e�N�X�`���̐����𓮓I�m��
		m_pTexture[nIdx] = new LPDIRECT3DTEXTURE9[nNumMat];

		for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
		{
			m_pTexture[nIdx][nCntMat] = NULL;

			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&m_pTexture[nIdx][nCntMat]);
			}
		}
	}
}

//=============================================================================
// ���f���̑����擾
//=============================================================================
int CModelCreate::GetNumModel(void)
{
	return m_nNumModel;
}

//=============================================================================
// �t�@�C�����̎擾����
//=============================================================================
char *CModelCreate::GetFileName(int nIdx)
{
	return m_pFileName[nIdx];
}

//=============================================================================
// ���f���̃��b�V�������擾����
//=============================================================================
LPD3DXMESH CModelCreate::GetMesh(int nIdx)
{
	LPD3DXMESH pMesh = NULL;  // ���b�V�����

	if (m_pMesh != NULL)
	{// ���������m�ۂ���Ă�
		pMesh = m_pMesh[nIdx];   // �ۑ����ꂽ���b�V��������
	}

	return pMesh;  // ���b�V������Ԃ�
}

//=============================================================================
// ���f���̃}�e���A�������擾����
//=============================================================================
LPD3DXBUFFER CModelCreate::GetBuffMat(int nIdx)
{
	LPD3DXBUFFER pBuffMat = NULL;  // �}�e���A�����

	if (m_pBuffMat != NULL)
	{// ���������m�ۂ���Ă�
		pBuffMat = m_pBuffMat[nIdx];   // �ۑ����ꂽ�}�e���A��������
	}

	return pBuffMat;  // �}�e���A������Ԃ�
}

//=============================================================================
// ���f���̃}�e���A���̐������擾����
//=============================================================================
DWORD CModelCreate::GetNumMat(int nIdx)
{
	DWORD nNumMat = 0;  // �}�e���A���̐����

	if (m_pNumMat != NULL)
	{// ���������m�ۂ���Ă�
		nNumMat = m_pNumMat[nIdx];   // �ۑ����ꂽ�}�e���A���̐�������
	}

	return nNumMat;  // �}�e���A���̐�����Ԃ�
}

//=============================================================================
// ���f���̒��_�̍ő�l�����擾����
//=============================================================================
D3DXVECTOR3 CModelCreate::GetVtxMax(int nIdx)
{
	D3DXVECTOR3 VtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ���_�̍ő�l���

	if (m_pVtxMax != NULL)
	{// ���������m�ۂ���Ă�
		VtxMax = m_pVtxMax[nIdx];   // �ۑ����ꂽ���_�̍ő�l������
	}

	return VtxMax;  // ���_�̍ő�l����Ԃ�
}

//=============================================================================
// ���f���̒��_�̍ŏ��l�����擾����
//=============================================================================
D3DXVECTOR3 CModelCreate::GetVtxMin(int nIdx)
{
	D3DXVECTOR3 VtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ���_�̍ŏ��l���

	if (m_pVtxMin != NULL)
	{// ���������m�ۂ���Ă�
		VtxMin = m_pVtxMin[nIdx];   // �ۑ����ꂽ���_�̍ŏ��l������
	}

	return VtxMin;  // ���_�̍ŏ��l����Ԃ�
}

//=============================================================================
// ���f���̃e�N�X�`�����擾
//=============================================================================
LPDIRECT3DTEXTURE9 *CModelCreate::GetTexture(int nIdx)
{
	return m_pTexture[nIdx];
}
