#pragma once



class CEntity;
class ASECameraData;
//////////////////////////////////////////////////////////////////////////
enum CAMERA_MODE
{	
	//현재 CAMERA_MODE
	CM_SMOOTHFOLLOW_,
	CM_ANI_

};

/************************************************************************/
/*     Smooth Follow 카메라 클래스                                                    */
/************************************************************************/

class CCamera_SmoothFollow : CCamera

{

protected:

	CEntity*							m_pAirCraft;  //쫒아갈 비행기객체의 포인터
	
	
	//////////////////////////////////////////////////////////////////////////
	//여기서부터 카메라 애니메이션을 위한 데이터
	bool							isRotAni_Pitch;//현재 회전애니메이션 중인가?
	bool							isRotAni_Roll;//현재 회전애니메이션 중인가?
	bool							isRotAni_Yaw;//현재 회전애니메이션 중인가?

	float							m_CurAniProcess; // 현재 카메라 회전애니의 프레임 0~1사이의 값  0이면 시작, 1이면 끝

	D3DXVECTOR3						m_OldRight , m_NewRight;  //카메라 보간 애니메이션을 위해 Old와 New 가 있어야함
	D3DXVECTOR3						m_OldUp   , m_NewUp;
	D3DXVECTOR3						m_OldLook , m_NewLook;
	D3DXVECTOR3						m_pos;//카메라 현재 위치

	D3DXMATRIX						m_MCurRot, m_MNewRot, m_MOldRot;  // 현재 회전행렬 , 새로운 회전행렬 , 예전 회전행렬
	D3DXQUATERNION					m_Old_Q, m_New_Q; //이전 사원수, 새로운 사원수
	
	CCamera_SmoothFollow();
	//////////////////////////////////////////////////////////////////////////

public:

	
	virtual ~CCamera_SmoothFollow();

	void							Init(LPDIRECT3DDEVICE9 _device, CEntity* pGameObject , ASECameraData* pCameraData = nullptr, int _CameraMode = 0);

public: // virtual 함수 재정의

	virtual	void					Update(float _dTime);

private: // 내부 함수

	bool							isPitchAngle_Larger(float _Angle); //PitchAngle이 _Angle 보다 비행기의 회전값이 크다면 true
	bool							isRollAngle_Larger(float _Angle);  //RollAngle이 _Angle 보다 비행기의 회전값이 크다면 true
	bool							isYawAngle_Larger(float _Angle);   //YawAngle이 _Angle 보다 비행기의 회전값이 크다면 true

	void							Lerp(float dTime);
	void							Animation(float _dTime);
	
public://get,, set



};//End of class CCamera_SmoothFollow 




//************************************* 끝 *****************************************************//