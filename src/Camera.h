/************************************************************************
					  Camera.h
				      2014.8.24

************************************************************************/

#pragma  once



/************************************************************************
					 Smooth Follow ī�޶� Ŭ����                                                    
************************************************************************/

class CCamera : public CGameObject, 
				public IMessagable,
				public IProgressable
{

protected:

	ASECameraData*					m_pAseCData;	//ase ����Ÿ���� �о���� ī�޶� ����

	D3DXMATRIXA16					m_ViewMat;  //viewMatrix 
	D3DXMATRIXA16					m_ProjMat;  //�������� ��Ʈ����,
	D3DXMATRIXA16					m_ViewProjMat;  //view * Proj 
	float							m_FOV;   // field of view [2014.11.18 ]
	float							m_Aspect; // ���� / ���� [2014.11.18 ]

	float							m_AngleYaw; // y������ ȸ���� ���� [2014.11.23 ]

	D3DXVECTOR3						m_CurRight; 
	D3DXVECTOR3						m_CurUp;
	D3DXVECTOR3						m_CurLook;
	D3DXVECTOR3						m_pos;		//ī�޶� ���� ��ġ

	int								m_CameraMode;

	D3DXVECTOR3					    m_Vec_BearToCamera ; // ���� ��ġ����  ī�޶� ���ϴ� ���⺤��
	D3DXVECTOR3						m_CamLookVec;        // ī�޶� Lookat ����
	
public:

	CCamera();
	virtual ~CCamera();

public: // operator overloading



public: // �����Լ�

	virtual HRESULT					Init(LPDIRECT3DDEVICE9 _device);
	
	virtual void					Update(float _dTime);			//from IProgressable
	// from IMessagable
	virtual bool					MsgProc(SH_MSG* _pMessage );

public: // �Ϲ��Լ�
	
	// ī�޶���� ���ϱ�
	int								MakeViewMatrix();
	// ������� ����ϱ�
	int								CalProjMatrix();   
	// ��� ��ũ��Ʈ���� ī�޶� ������ �ҷ��´�. 
	int								Load_CameraInfo_from_Lua(const CHAR* _pPath);

public:// ������

	ASECameraData*    				GetAseCData(); 
	const D3DXMATRIXA16&			Get_ViewMat() const {return m_ViewMat;}
	const D3DXMATRIXA16&			Get_ProjMat() const {return m_ProjMat;}
	const D3DXMATRIXA16&			Get_ViewProjMat() const {return m_ViewProjMat;}
	float							GetFOV();
	D3DXVECTOR3						GetPos() const {return m_pos;}
	D3DXVECTOR3						GetUpVec() const {return m_CurUp;} 

	D3DXVECTOR3						GetLookVec() const {return m_CurLook;}
	float&							Get_m_AngleYaw()  {return m_AngleYaw;}


	void							SetCameraMode(int _Mode){m_CameraMode = _Mode;}

};


#include "Camera_SmoothFollow.h"



/*************************************************** end **************************************************************/