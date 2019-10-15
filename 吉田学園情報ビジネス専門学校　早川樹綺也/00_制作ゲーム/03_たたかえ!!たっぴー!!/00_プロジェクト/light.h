//*****************************************************************************
//
//     ���C�g�̏���[light.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _LIGHT_H_
#define _LIGHT_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    ���C�g�N���X�̒�`
//*****************************************************************************
class CLight
{
public:    // �N�ł��A�N�Z�X�\
	typedef enum
	{
		TYPE_NONE = -1,    // ����
		TYPE_DIRECTIONAL,  // �f�B���N�V���i��
		TYPE_POINT,        // �|�C���g
		TYPE_SPOT,         // �X�|�b�g
		TYPE_MAX
	}TYPE;

	static CLight *ChangeLightType(TYPE type);

	CLight();
	virtual ~CLight();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void);
	virtual void Update(void) = 0;

	void SetType(const TYPE type);
	void SetLight(const D3DLIGHT9 Light);
	void SetSwitch(const bool bSwitch);

	TYPE GetType(void);
	D3DLIGHT9 GetLight(void);
	bool GetSwitch(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	TYPE      m_Type;    // ���C�g�̎��
	D3DLIGHT9 m_Light;   // ���C�g�̏��
	bool      m_bSwitch; // ���C�g��ON�ɂȂ��Ă��邩�ǂ���
};

//*****************************************************************************
//    �f�B���N�V���i�����C�g�N���X�̒�`
//*****************************************************************************
class CDirectionalLight : public CLight
{
public:    // �N�ł��A�N�Z�X�\
	CDirectionalLight();
	~CDirectionalLight();

	static CDirectionalLight *Create(D3DXVECTOR3 Dir, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, bool bSwitch = true);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void CreateLight(D3DXVECTOR3 Dir, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, bool bSwitch);
};

//*****************************************************************************
//    �|�C���g���C�g�N���X�̒�`
//*****************************************************************************
class CPointLight : public CLight
{
public:    // �N�ł��A�N�Z�X�\
	CPointLight();
	~CPointLight();

	static CPointLight *Create(D3DXVECTOR3 pos, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, float fAttenuation0, float fAttenuation1, float fAttenuation2, float fRange, bool bSwitch = true);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void CreateLight(D3DXVECTOR3 pos, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, float fAttenuation0, float fAttenuation1, float fAttenuation2, float fRange, bool bSwitch);
};

//*****************************************************************************
//    �X�|�b�g���C�g�N���X�̒�`
//*****************************************************************************
class CSpotLight : public CLight
{
public:    // �N�ł��A�N�Z�X�\
	CSpotLight();
	~CSpotLight();

	static CSpotLight *Create(D3DXVECTOR3 pos, D3DXVECTOR3 Dir, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, float fAttenuation0, float fAttenuation1, float fAttenuation2, float fRange, float fFalloff, float fTheta, float fPhi, bool bSwitch = true);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void CreateLight(D3DXVECTOR3 pos, D3DXVECTOR3 Dir, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, float fAttenuation0, float fAttenuation1, float fAttenuation2, float fRange, float fFalloff, float fTheta, float fPhi, bool bSwitch);
};

#endif