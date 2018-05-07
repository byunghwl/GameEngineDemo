/************************************************************************
					  Camera.h
				      2014.8.24

************************************************************************/

#pragma  once



/************************************************************************
					 Smooth Follow 카메라 클래스                                                    
************************************************************************/

class CCamera : public CGameObject, 
				public IMessagable,
				public IProgressable
{

protected:

	ASECameraData*					m_pAseCData;	//ase 데이타에서 읽어들인 카메라 정보

	D3DXMATRIXA16					m_ViewMat;  //viewMatrix 
	D3DXMATRIXA16					m_ProjMat;  //프로젝션 메트릭스,
	D3DXMATRIXA16					m_ViewProjMat;  //view * Proj 
	float							m_FOV;   // field of view [2014.11.18 ]
	float							m_Aspect; // 가로 / 세로 [2014.11.18 ]

	float							m_AngleYaw; // y축으로 회전한 각도 [2014.11.23 ]

	D3DXVECTOR3						m_CurRight; 
	D3DXVECTOR3						m_CurUp;
	D3DXVECTOR3						m_CurLook;
	D3DXVECTOR3						m_pos;		//카메라 현재 위치

	int								m_CameraMode;

	D3DXVECTOR3					    m_Vec_BearToCamera ; // 곰의 위치에서  카메라를 향하는 방향벡터
	D3DXVECTOR3						m_CamLookVec;        // 카메라 Lookat 벡터
	
public:

	CCamera();
	virtual ~CCamera();

public: // operator overloading



public: // 가상함수

	virtual HRESULT					Init(LPDIRECT3DDEVICE9 _device);
	
	virtual void					Update(float _dTime);			//from IProgressable
	// from IMessagable
	virtual bool					MsgProc(SH_MSG* _pMessage );

public: // 일반함수
	
	// 카메라행렬 구하기
	int								MakeViewMatrix();
	// 투영행렬 계산하기
	int								CalProjMatrix();   
	// 루아 스크립트에서 카메라 정보를 불러온다. 
	int								Load_CameraInfo_from_Lua(const CHAR* _pPath);

public:// 접근자

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