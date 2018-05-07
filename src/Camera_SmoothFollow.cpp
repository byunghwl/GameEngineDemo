#include "DXUT.H"
#include "ASEData.h"
#include "GameObject.h"
#include "Camera_SmoothFollow.h"


//////////////////////////////////////////////////////////////////////////
//전역변수 선언



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 여기서부터 CCamera_SmoothFollow 클래스 멤버함수
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



// 생성자
CCamera_SmoothFollow::CCamera_SmoothFollow() :m_pAirCraft(nullptr)
{
	
	
}

//////////////////////////////////////////////////////////////////////////
// 소멸자
CCamera_SmoothFollow::~CCamera_SmoothFollow()
{
	SAFE_DELETE_(m_pAseCData)
	write_log_file("메모리 해제 : camera.cpp,  ~CCamera_SmoothFollow() 호출");
}


//-----------------------------------------------------------------------------
// 카메라 초기화 함수 
// CGameObject* pAirCraft  : 게임오브젝트 포인터
// int _Mode : 카메라 모드 (enum값) , 기본값 =0
//-----------------------------------------------------------------------------

void CCamera_SmoothFollow::Init(LPDIRECT3DDEVICE9 _device,
								CEntity* pGameObject ,
								ASECameraData* pCameraData  /*= nullptr*/,
								int _CameraMode /*= 0*/
							   )
{

	CCamera::SetDevice(_device);

	m_OldRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_OldUp    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_OldLook  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	
	m_pAirCraft = pGameObject;

	isRotAni_Pitch = false;
	isRotAni_Roll= false;
	isRotAni_Yaw= false;

	D3DXMatrixIdentity(&m_MNewRot);
	D3DXMatrixIdentity(&m_MCurRot);
	D3DXMatrixIdentity(&m_MOldRot);

	m_CurAniProcess=0.0f;//
	m_CameraMode = _CameraMode;
	

}


//////////////////////////////////////////////////////////////////////////
// 업데이트 함수

void CCamera_SmoothFollow::Update(float _dTime)
{
	CCamera::Update(_dTime);//반드시 부모클래스의 업데이트 함수를 먼저 사용해야한다.

	if(m_CameraMode == CM_SMOOTHFOLLOW_ )
	{
		Animation(_dTime);  //보간애니메이션 시켜주는 함수
		m_pos =  m_pAirCraft->GetPos() + -69.0f*m_CurLook + m_CurUp*50.0f;//카메라 위치는 비행기의 시선벡터의 반대방향으로 더해준다

		//카메라 행렬 설정 and 셋팅
	
		
		m_pDevice->SetTransform(D3DTS_VIEW, &m_ViewMat);
	}
	

}


//////////////////////////////////////////////////////////////////////////
//PitchAngledl이 _Angle 보다 비행기의 회전값이 크다면 true

bool CCamera_SmoothFollow::isPitchAngle_Larger(float _Angle)
{
	if( (m_pAirCraft->GetAngle_Pitch() > _Angle) || (m_pAirCraft->GetAngle_Pitch() < -_Angle) ) //pitch각의 절대값이 _Angle 이상이라면
	{
		m_pAirCraft->SetAngle_Pitch(0.0f);//누적시켰던 각도를 다시 0으로 만들어준다.
		m_pAirCraft->SetAngle_Roll(0.0f);//누적시켰던 각도를 다시 0으로 만들어준다.
		m_pAirCraft->SetAngle_Yaw(0.0f);//누적시켰던 각도를 다시 0으로 만들어준다.
		return true;
	}	 
	return false;
}



//////////////////////////////////////////////////////////////////////////
//RollAngledl이 _Angle 보다 비행기의 회전값이 크다면 true

bool CCamera_SmoothFollow::isRollAngle_Larger(float _Angle)
{
	if( (m_pAirCraft->GetAngle_Roll() > _Angle) || (m_pAirCraft->GetAngle_Roll() < -_Angle) ) //Roll각의 절대값이 _Angle 이상이라면
	{
		m_pAirCraft->SetAngle_Pitch(0.0f);//누적시켰던 각도를 다시 0으로 만들어준다.
		m_pAirCraft->SetAngle_Roll(0.0f);//누적시켰던 각도를 다시 0으로 만들어준다.
		m_pAirCraft->SetAngle_Yaw(0.0f);//누적시켰던 각도를 다시 0으로 만들어준다.
		return true;
	}
	return false;
}



//////////////////////////////////////////////////////////////////////////
//YawAngle이 _Angle 보다 비행기의 회전값이 크다면 true

bool CCamera_SmoothFollow::isYawAngle_Larger(float _Angle)
{
	if( (m_pAirCraft->GetAngle_Yaw() > _Angle) || (m_pAirCraft->GetAngle_Yaw() < -_Angle) ) //Yaw각의 절대값이 _Angle 이상이라면
	{
		m_pAirCraft->SetAngle_Pitch(0.0f);//누적시켰던 각도를 다시 0으로 만들어준다.
		m_pAirCraft->SetAngle_Roll(0.0f);//누적시켰던 각도를 다시 0으로 만들어준다.
		m_pAirCraft->SetAngle_Yaw(0.0f);//누적시켰던 각도를 다시 0으로 만들어준다.
		return true;
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////
// 보간애니메이션 함수

void CCamera_SmoothFollow::Lerp(float dTime)
{

	D3DXQUATERNION CurQ;//현재 보간중인 쿼터니언, 예전사원수, 새로운 사원수

	if(m_pAirCraft->GetIsRotating() ) //계속 회전하고 있는 상태라면 
	{
		m_MNewRot = m_pAirCraft->GetMRot(); //회전된 행렬을 가져온다.
		
// 		m_pAirCraft->SetAngle_Pitch(0.0f);//누적시켰던 각도를 다시 0으로 만들어준다.
// 		m_pAirCraft->SetAngle_Roll(0.0f); //누적시켰던 각도를 다시 0으로 만들어준다.
// 		m_pAirCraft->SetAngle_Yaw(0.0f);  //누적시켰던 각도를 다시 0으로 만들어준다.
		D3DXQuaternionRotationMatrix(&m_New_Q , &m_MNewRot);
	}
	
	m_CurAniProcess += dTime;//*dTime;//현재 보간 진행 상황 ,, 0~1사이의 값 
	
	
	if(m_CurAniProcess >1.0f)
	{
		D3DXVec3TransformNormal(&m_CurLook, &m_OldLook,  &m_MNewRot);
		D3DXVec3TransformNormal(&m_CurUp, &m_OldUp,  &m_MNewRot);
		D3DXVec3TransformNormal(&m_CurRight, &m_OldRight,  &m_MNewRot);

		isRotAni_Pitch =isRotAni_Roll =isRotAni_Yaw =false;
		m_CurAniProcess = 0.0f;
	
		return ;
	}
	
	// old사원수와 new 사원수의 구면 선형 보간을  해준다.
	D3DXQuaternionSlerp(&CurQ, &m_Old_Q, &m_New_Q,m_CurAniProcess);

	// cur사원수를 현재적용할 회전행렬로 바꾼다.
	D3DXMatrixRotationQuaternion(&m_MCurRot, &CurQ);

	D3DXVec3TransformNormal(&m_CurLook, &m_OldLook,  &m_MCurRot);
	D3DXVec3TransformNormal(&m_CurUp, &m_OldUp,  &m_MCurRot);
	D3DXVec3TransformNormal(&m_CurRight, &m_OldRight,  &m_MCurRot);
	
	
}

//-----------------------------------------------------------------------------
// smooth follow 애니메이션
//-----------------------------------------------------------------------------

void CCamera_SmoothFollow::Animation(float _dTime)
{

	if(isPitchAngle_Larger(0.523f)   ) // pitch 회전각도가 30도 이상이라면
	{
	
		m_MNewRot = m_pAirCraft->GetMRot(); //회전된 행렬을 가져온다.

		isRotAni_Pitch = true; //pitch 회전 애니메이션 플래그를 true 로 만든다. (보간 애니메이션 시작)

		m_MOldRot = m_MCurRot; //현재 회전행렬을 old 회전행렬에 넣어준다.
	
		m_CurAniProcess =0.0f; //현재 애니메이션 진행상황(0~1 사이의 값)

		//예전 회전행렬을 old 사원수로 만들어준다.
		D3DXQuaternionRotationMatrix(&m_Old_Q , &m_MOldRot);
		//새로운 회전행렬을 new 사원수로 만들어준다.
		D3DXQuaternionRotationMatrix(&m_New_Q , &m_MNewRot);
	  
	}

	if(isRollAngle_Larger(0.523f)  )// roll 회전각도가 45도 이상이라면
	{	
		m_MNewRot = m_pAirCraft->GetMRot();
		isRotAni_Roll = true;  //pitch 회전 애니메이션 플래그를 true 로 만든다. 
		m_MOldRot = m_MCurRot;
		m_CurAniProcess =0.0f;
		
		//예전 회전행렬을 old 사원수로 만들어준다.
		D3DXQuaternionRotationMatrix(&m_Old_Q , &m_MOldRot);
		//새로운 회전행렬을 new 사원수로 만들어준다.
		D3DXQuaternionRotationMatrix(&m_New_Q , &m_MNewRot);	    
	}

	if(isYawAngle_Larger(0.523f)  )//yaw 회전각도가 45도 이상이라면
	{
		m_MNewRot = m_pAirCraft->GetMRot();
		isRotAni_Yaw = true; //pitch 회전 애니메이션 플래그를 true 로 만든다. 
		m_MOldRot = m_MCurRot;
		m_CurAniProcess = 0.0f;		

		//예전 회전행렬을 old 사원수로 만들어준다.
		D3DXQuaternionRotationMatrix(&m_Old_Q , &m_MOldRot);
		//새로운 회전행렬을 new 사원수로 만들어준다.
		D3DXQuaternionRotationMatrix(&m_New_Q , &m_MNewRot);
		//m_OldLook = m_CurLook;    m_OldRight = m_CurRight;    m_OldUp = m_CurUp;     
	}

	if(isRotAni_Pitch || isRotAni_Roll || isRotAni_Yaw)  // 셋중 하나라도 각도가 일정각도 이상이 된다면 ..
	{
		Lerp(_dTime);// 보간시켜주는 함수 ,,(이곳에서 카메라의 _look, _up, _right를 업데이트시켜줌)
	}
}


//****************************************** 끝 ************************************************//