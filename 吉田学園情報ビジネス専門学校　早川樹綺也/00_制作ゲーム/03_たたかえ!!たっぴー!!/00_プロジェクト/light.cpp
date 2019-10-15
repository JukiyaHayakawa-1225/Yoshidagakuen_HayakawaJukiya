//*****************************************************************************
//
//     ���C�g�̏���[light.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "light.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#ifdef _DEBUG  // �f�o�b�O���̏���
#define LIGHT_DEBUG_VECTOR_UPDOWN  (0.01f)  // ���C�g�̌��������ǂꂭ�炢��������
#define LIGHT_DEBUG_COLOR_UPDOWN   (0.01f)  // ���C�g�̐F�����ǂꂭ�炢�ω������邩
#endif

//*****************************************************************************
//    CLight�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CLight::CLight()
{
	// �e��l�̃N���A
	m_Type = TYPE_NONE;                       // ���C�g�̎��
	ZeroMemory(&m_Light, sizeof(D3DLIGHT9));  // ���C�g���
	m_bSwitch = false;                        // ���C�g��ON/OFF
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CLight::~CLight()
{

}

//=============================================================================
//    ���C�g�̎�ނ�ύX���A�h���X��Ԃ�����
//=============================================================================
CLight *CLight::ChangeLightType(TYPE type)
{
	CLight *pLight = NULL;

	// ���C�g�𐶐�
	if (type == TYPE_DIRECTIONAL)
	{// �f�B���N�V���i�����C�g��������
		pLight = CDirectionalLight::Create(D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else if (type == TYPE_POINT)
	{// �|�C���g���C�g��������
		pLight = CPointLight::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.1f, 0.01f, 0.0f, 300.0f);
	}
	else if (type == TYPE_SPOT)
	{// �|�C���g���C�g��������
		pLight = CSpotLight::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			0.1f, 0.01f, 0.0f, 300.0f, 1.0f, D3DX_PI * 0.5f, D3DX_PI * 2.0f);
	}

	return pLight;
}

//=============================================================================
//    �I������
//=============================================================================
void CLight::Uninit(void)
{
	// ���C�g���N���A����
	ZeroMemory(&m_Light, sizeof(D3DLIGHT9));
}

//=============================================================================
//    ���C�g�̎�ނ�ݒ肷��
//=============================================================================
void CLight::SetType(const TYPE type)
{
	m_Type = type;
}

//=============================================================================
//    ���C�g����ݒ肷��
//=============================================================================
void CLight::SetLight(const D3DLIGHT9 Light)
{
	m_Light = Light;
}

//=============================================================================
//    ���C�g��ON/OFF��ݒ肷��
//=============================================================================
void CLight::SetSwitch(const bool bSwitch)
{
	m_bSwitch = bSwitch;
}

//=============================================================================
//    ���C�g�̎�ނ��擾����
//=============================================================================
CLight::TYPE CLight::GetType(void)
{
	return m_Type;
}

//=============================================================================
//    ���C�g�����擾����
//=============================================================================
D3DLIGHT9 CLight::GetLight(void)
{
	return m_Light;
}

//=============================================================================
//    ���C�g��ON/OFF���擾����
//=============================================================================
bool CLight::GetSwitch(void)
{
	return m_bSwitch;
}

//*****************************************************************************
//    CDirectionalLight�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CDirectionalLight::CDirectionalLight()
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CDirectionalLight::~CDirectionalLight()
{

}


//=============================================================================
//    ��������
//=============================================================================
CDirectionalLight *CDirectionalLight::Create(D3DXVECTOR3 Dir, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, bool bSwitch)
{
	CDirectionalLight *pDirLight = NULL;  // �f�B���N�V���i�����C�g�N���X�^�̃|�C���^
	if (pDirLight == NULL)
	{// ����������ɂȂ��Ă���
		pDirLight = new CDirectionalLight;
		if (pDirLight != NULL)
		{// �C���X�^���X�𐶐��ł���
			// ���C�g����ݒ�
			pDirLight->CreateLight(Dir, Diffuse, Ambient, Specular, bSwitch);

			if (FAILED(pDirLight->Init()))
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

	return pDirLight;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CDirectionalLight::Init(void)
{
	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CDirectionalLight::Uninit(void)
{

}

//=============================================================================
//    �X�V����
//=============================================================================
void CDirectionalLight::Update(void)
{

}

//=============================================================================
//    ���C�g����ݒ肷�鏈��
//=============================================================================
void CDirectionalLight::CreateLight(D3DXVECTOR3 Dir, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, bool bSwitch)
{
	D3DLIGHT9 Light;   // ���C�g���

	// �e��l�̐ݒ�
	// ���C�g�̎��
	Light.Type = D3DLIGHT_DIRECTIONAL;
	SetType(TYPE_DIRECTIONAL);

	// ���C�g�̊g�U��
	Light.Diffuse = Diffuse;

	// ���C�g�̊���
	Light.Ambient = Ambient;

	// ���C�g�̔��ˌ�
	Light.Specular = Specular;

	// ���C�g�̌���
	Light.Direction = Dir;

	// ���C�g����ݒ�
	SetLight(Light);

	// ���C�g��ON/OFF��ݒ�
	SetSwitch(bSwitch);
}

//*****************************************************************************
//    CPointLight�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CPointLight::CPointLight()
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CPointLight::~CPointLight()
{

}

//=============================================================================
//    ��������
//=============================================================================
CPointLight *CPointLight::Create(D3DXVECTOR3 pos, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, float fAttenuation0, float fAttenuation1, float fAttenuation2, float fRange, bool bSwitch)
{
	CPointLight *pPointLight = NULL;  // �|�C���g���C�g�N���X�^�̃|�C���^
	if (pPointLight == NULL)
	{// ����������ɂȂ��Ă���
		pPointLight = new CPointLight;
		if (pPointLight != NULL)
		{// �C���X�^���X�𐶐��ł���
		    // ���C�g����ݒ�
			pPointLight->CreateLight(pos, Diffuse, Ambient, Specular, fAttenuation0, fAttenuation1, fAttenuation2, fRange, bSwitch);

			if (FAILED(pPointLight->Init()))
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

	return pPointLight;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CPointLight::Init(void)
{
	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CPointLight::Uninit(void)
{

}

//=============================================================================
//    �X�V����
//=============================================================================
void CPointLight::Update(void)
{

}

//=============================================================================
//    ���C�g����ݒ肷�鏈��
//=============================================================================
void CPointLight::CreateLight(D3DXVECTOR3 pos, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, float fAttenuation0, float fAttenuation1, float fAttenuation2,  float fRange, bool bSwitch)
{
	D3DLIGHT9 Light;   // ���C�g���

	// �e��l�̐ݒ�
	// ���C�g�̎��
	Light.Type = D3DLIGHT_POINT;
	SetType(TYPE_POINT);

	// ���C�g�̈ʒu
	Light.Position = pos;

	// ���C�g�̊g�U��
	Light.Diffuse = Diffuse;

	// ���C�g�̎��͐F
	Light.Ambient = Ambient;

	// ���C�g�̋��ʐF
	Light.Specular = Specular;

	// ���C�g�̌����l
	Light.Attenuation0 = fAttenuation0;	   // ���`�����W��
	Light.Attenuation1 = fAttenuation1;	   // 2�������W��
	Light.Attenuation2 = fAttenuation2;	   // �w�������W��

	// ���C�g�͈̔�
	Light.Range = fRange;

	// ���C�g����ݒ�
	SetLight(Light);

	// ���C�g��ON/OFF��ݒ�
	SetSwitch(bSwitch);
}

//*****************************************************************************
//    CSpotLight�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CSpotLight::CSpotLight()
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CSpotLight::~CSpotLight()
{

}

//=============================================================================
//    ��������
//=============================================================================
CSpotLight *CSpotLight::Create(D3DXVECTOR3 pos, D3DXVECTOR3 Dir, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, float fAttenuation0, float fAttenuation1, float fAttenuation2, float fRange, float fFalloff, float fTheta, float fPhi, bool bSwitch)
{
	CSpotLight *pSpotLight = NULL;  // �X�|�b�g���C�g�N���X�^�̃|�C���^
	if (pSpotLight == NULL)
	{// ����������ɂȂ��Ă���
		pSpotLight = new CSpotLight;
		if (pSpotLight != NULL)
		{// �C���X�^���X�𐶐��ł���
		    // ���C�g����ݒ�
			pSpotLight->CreateLight(pos, Dir, Diffuse, Ambient, Specular, fAttenuation0, fAttenuation1, fAttenuation2, fRange, fFalloff, fTheta, fPhi, bSwitch);

			if (FAILED(pSpotLight->Init()))
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

	return pSpotLight;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CSpotLight::Init(void)
{
	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CSpotLight::Uninit(void)
{

}

//=============================================================================
//    �X�V����
//=============================================================================
void CSpotLight::Update(void)
{

}

//=============================================================================
//    ���C�g����ݒ肷�鏈��
//=============================================================================
void CSpotLight::CreateLight(D3DXVECTOR3 pos, D3DXVECTOR3 Dir, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, float fAttenuation0, float fAttenuation1, float fAttenuation2, float fRange, float fFalloff, float fTheta, float fPhi, bool bSwitch)
{
	D3DLIGHT9 Light;   // ���C�g���

	// �e��l�̐ݒ�
	// ���C�g�̎��
	Light.Type = D3DLIGHT_SPOT;
	SetType(TYPE_SPOT);

	// ���C�g�̈ʒu
	Light.Position = pos;

	// ���C�g�̌���
	Light.Direction = Dir;

	// ���C�g�̊g�U��
	Light.Diffuse = Diffuse;

	// ���C�g�̎��͐F
	Light.Ambient = Ambient;

	// ���C�g�̋��ʐF
	Light.Specular = Specular;

	// ���C�g�̌����l
	Light.Attenuation0 = fAttenuation0;	  // ���`�����W��
	Light.Attenuation1 = fAttenuation1;	  // 2�������W��
	Light.Attenuation2 = fAttenuation2;	  // �w�������W��

	// ���C�g�͈̔�
	Light.Range = fRange;

	// ���C�g�̃t�H�[���I�t���
	Light.Falloff = fFalloff;

	// ���C�g�̓����R�[���̊p�x
	Light.Theta = fTheta;

	// ���C�g�̊O���R�[���̊p�x
	Light.Phi = fPhi;

	// ���C�g����ݒ�
	SetLight(Light);

	// ���C�g��ON/OFF��ݒ�
	SetSwitch(bSwitch);
}