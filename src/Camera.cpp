#include "DXUT.H"
#include "Camera.h"

D3DXVECTOR3 G_Vec_BearToCamera = D3DXVECTOR3(0.0f, 70.0f, -110.0f);
D3DXVECTOR3 G_CamLookVec = D3DXVECTOR3(0.0f,  -65.0f, 140.0f);
D3DXVECTOR3 G_Vec_WorldY = D3DXVECTOR3(0.0f,  1.0f, 0.0f);
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 여기서부터 CCamera 클래스 멤버함수
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------
//  생성자
//-------------------------------------------------------------------------
CCamera::CCamera() : m_pAseCData(NULL)
{
	
}


//-------------------------------------------------------------------------
// 소멸자
//-------------------------------------------------------------------------
CCamera::~CCamera()
{
	
}



//-------------------------------------------------------------------------
// 카메라는 반드시 초기화 하고나서 써야됨
//
//-------------------------------------------------------------------------
HRESULT	CCamera::Init(LPDIRECT3DDEVICE9 _device)
{
	int ret = S_OK;
	m_pos      = D3DXVECTOR3(0.0f, 400.0f, -700.0f);
	m_CurRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_CurUp    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_CurLook  = D3DXVECTOR3(0.0f, -40.0f, 140.0f);
	m_AngleYaw  = 0.0f;
	//m_Vec_BearToCamera = G_Vec_BearToCamera;
	//m_CamLookVec = 	 G_CamLookVec;

	if(_device == nullptr)
	{
		ret= S_FALSE; 	
	}
	else
	{
		m_pDevice = _device;
	}

	CalProjMatrix();//투영행렬 계산

	return ret;
}


//-------------------------------------------------------------------------
//  업데이트 함수
//-------------------------------------------------------------------------
void CCamera::Update(float _dTime)
{
	//m_pos.x += 80.0f * m_DTime;
// 	if(SHG_Input.key(DIK_D))
// 		m_pos += m_CurRight* 300.0f * _dTime;
// 
// 	if(SHG_Input.key(DIK_A))
// 		m_pos -= m_CurRight* 300.0f * _dTime;
// 
// 	if(SHG_Input.key(DIK_W))
// 		m_pos += m_CurLook* 300.0f * _dTime;
// 
// 	if(SHG_Input.key(DIK_S))
// 		m_pos -= m_CurLook * 300.0f * _dTime;
// 	
// 	if(SHG_Input.key(DIK_E))
// 		m_pos += m_CurUp * 300.0f * _dTime;
// 
// 	if(SHG_Input.key(DIK_Q))
// 		m_pos -= m_CurUp * 300.0f * _dTime;



	// 카메라 바라보는 방향 정하기 
	//m_CurLook =  m_CamLookVec; 



 	//D3DXVECTOR3 MouseDelta = SHG_Input.GetMouseDelta();



// 	if(MouseDelta.x > 0.1f)
// 		TempDelta = 4.0f;
// 	else if(MouseDelta.x < -0.1f)
// 		TempDelta = -4.0f;
// 	else
// 		TempDelta = 0.0f;
	//TempDelta = MouseDelta.x * 0.1f;


 	//m_CurLook += m_CurRight * MouseDelta.x *_dTime;
// 	m_CurLook -= m_CurUp * MouseDelta.y *_dTime;

	// 마우스를 윈도우의 중앙으로 초기화
	//SetCursor( NULL );	// 마우스를 나타나지 않게 않다.


	//m_AngleYaw += SHG_Input.GetMouseDelta().x * _dTime * 0.1f; input의 업데이트로 옮김
	//LOG_WRITE_A("TempDelta x : %f",TempDelta);
	D3DXMATRIX Roty;
	
	D3DXMatrixRotationY(&Roty , m_AngleYaw);
	D3DXVec3TransformNormal(&m_Vec_BearToCamera , &G_Vec_BearToCamera , &Roty);
	D3DXVec3TransformNormal(&m_CurLook , &G_CamLookVec , &Roty);
	
	// 카메라 포지션 정하기 
	if(G_pTedyBear != nullptr)
	{
		m_pos = G_pTedyBear->GetPos() + m_Vec_BearToCamera; 
	}
	

	if((m_pAseCData != NULL) && (m_CameraMode  == CM_ANI_))
	{	
		m_pAseCData->Animation(_dTime);
		//카메라 행렬 설정 and 셋팅
		D3DXMATRIX V;
		m_pAseCData->MakeViewMatrix(&V);
		m_pDevice->SetTransform(D3DTS_VIEW, &V);
	}
	else
	{		
		MakeViewMatrix();
		m_pDevice->SetTransform(D3DTS_VIEW, &m_ViewMat);
	}

	m_ViewProjMat = m_ViewMat * m_ProjMat;

}

//-------------------------------------------------------------------------
// 카메라행렬 구하기
// 포인터로 뷰행렬을 직접 변환
// 반환값 : 확장성을 위해 일단 int형으로.. 
// V(_Out) : 계산될 메트릭스
//-------------------------------------------------------------------------
int		CCamera::MakeViewMatrix()
{
	if(m_CameraMode == CM_ANI_)
	{
		m_pAseCData->MakeViewMatrix(&m_ViewMat);
		return 0;
	}

	D3DXVec3Normalize(&m_CurLook, &m_CurLook);

	D3DXVec3Cross(&m_CurRight, &G_Vec_WorldY, &m_CurLook);
	D3DXVec3Normalize(&m_CurRight, &m_CurRight);

 	D3DXVec3Cross(&m_CurUp, &m_CurLook, &m_CurRight);
 	D3DXVec3Normalize(&m_CurUp, &m_CurUp);
	
	float x = -D3DXVec3Dot(&m_CurRight, &m_pos);
	float y = -D3DXVec3Dot(&m_CurUp, &m_pos);
	float z = -D3DXVec3Dot(&m_CurLook, &m_pos);

	m_ViewMat(0,0) = m_CurRight.x; m_ViewMat(0, 1) = m_CurUp.x; m_ViewMat(0, 2) = m_CurLook.x; m_ViewMat(0, 3) = 0.0f;
	m_ViewMat(1,0) = m_CurRight.y; m_ViewMat(1, 1) = m_CurUp.y; m_ViewMat(1, 2) = m_CurLook.y; m_ViewMat(1, 3) = 0.0f;
	m_ViewMat(2,0) = m_CurRight.z; m_ViewMat(2, 1) = m_CurUp.z; m_ViewMat(2, 2) = m_CurLook.z; m_ViewMat(2, 3) = 0.0f;
	m_ViewMat(3,0) = x;        m_ViewMat(3, 1) = y;     m_ViewMat(3, 2) = z;       m_ViewMat(3, 3) = 1.0f;

	return 0;
}



//-------------------------------------------------------------------------
// 투영행렬 계산하기
//-------------------------------------------------------------------------// 
int		CCamera::CalProjMatrix()
{	
	//////////////////////////////////////////////////////////////////////////
	//투영변환	
	DXDeviceSettings temp =SHGlobal_GetDeviceSetting;
	assert(m_pDevice != nullptr);
	if((SHGlobal_GetDeviceSetting.pp.BackBufferWidth == 0) || (SHGlobal_GetDeviceSetting.pp.BackBufferHeight == 0) ) {assert(false);}
	D3DXMatrixPerspectiveFovLH( &m_ProjMat, m_FOV, /*m_Aspect*/( (float)SHGlobal_GetDeviceSetting.pp.BackBufferWidth / (float)SHGlobal_GetDeviceSetting.pp.BackBufferHeight   ), 10.0f, 12000.0f );
	SHGlobal_GetD3DDevice->SetTransform( D3DTS_PROJECTION, &m_ProjMat );

	return true;

}



//---------------------------------------------------------------------------------------------
// Field of View 구하기
//---------------------------------------------------------------------------------------------
float CCamera::GetFOV()
{
	if(m_CameraMode == CM_ANI_)
		return  m_pAseCData->GetFov();

	return (D3DX_PI/3.7f);
}


//-----------------------------------------------------------------------------
// m_pAseCData 를 반환, 없으면 동적생성해서 반환
//-----------------------------------------------------------------------------
ASECameraData*	CCamera::GetAseCData() 
{
	if(m_pAseCData == NULL)
	{
		m_pAseCData = new ASECameraData;
		ASSERT_IF_FAIL_MemAlloc(m_pAseCData); 
	}

	m_CameraMode = CM_ANI_;
	return m_pAseCData;
}	 

//-----------------------------------------------------------------------------
// Desc   :  루아 스크립트에서 카메라 정보를  불러온다.
//-----------------------------------------------------------------------------
int		CCamera::Load_CameraInfo_from_Lua(const CHAR* _pPath)
{


	SH_Lua TempLua(_pPath);
	//int tableSize; 

	try
	{
		// 루아 전역변수 m_Vec_BearToCamera.x 을 받아온다. 
		TempLua.GetGlobal_float("Vec_BearToCam_X" , m_Vec_BearToCamera.x);
		// 루아 전역변수 m_Vec_BearToCamera.y 을 받아온다. 
		TempLua.GetGlobal_float("Vec_BearToCam_Y" , m_Vec_BearToCamera.y);
		// 루아 전역변수 m_Vec_BearToCamera.z 을 받아온다.
		TempLua.GetGlobal_float("Vec_BearToCam_Z" , m_Vec_BearToCamera.z);


		// 루아 전역변수 Vec_CameraLookAt_X 을 받아온다.
		TempLua.GetGlobal_float("Vec_CameraLookAt_X" , m_CamLookVec.x);
		// 루아 전역변수 Vec_CameraLookAt_Y 을 받아온다.
		TempLua.GetGlobal_float("Vec_CameraLookAt_Y" , m_CamLookVec.y);
		// 루아 전역변수 Vec_CameraLookAt_Z 을 받아온다. 
		TempLua.GetGlobal_float("Vec_CameraLookAt_Z" , m_CamLookVec.z);
	
		// 루아 전역변수 FOV 을 받아온다.
		TempLua.GetGlobal_float("FOV" , m_FOV);
	
		// 가로세로 비율
	//	TempLua.GetGlobal_float("Aspect" , m_Aspect);
		
	
	}
	catch(...)
	{
		SHUtil::MsgBox("CAnimator::Load_ClipInfo_from_Lua() , 루아예외처리!!");
		return S_FALSE;
	}

	return S_OK;
}




//-----------------------------------------------------------------------------
// 기본카메라 메시지 처리 함수
//-----------------------------------------------------------------------------
bool	CCamera::MsgProc(SH_MSG* _pMessage )
{
	switch (_pMessage->m_WinMsg)
	{
	case WM_LBUTTONDOWN:

		break;
	case WM_LBUTTONUP:

		break;
	case WM_MOUSEMOVE:		

		break;
	case WM_KEYDOWN:		
// 		switch(wParam)
// 		{
// 		case 'S':
// 			m_pos.z -= 80.0f * m_DTime;
// 			break;
// 		case 'W':
// 			m_pos.z += 80.0f * m_DTime;
// 			break;
// 		case 'D':
// 			m_pos.x += 80.0f* m_DTime;
// 			break;
// 		case 'A':
// 			m_pos.x -= 80.0f* m_DTime;
// 			break;

// 
// 		}
		break;
	case WM_KEYUP:	

		break;	

	}

	return true;
}

/*************************************************** end of this file **************************************************************/