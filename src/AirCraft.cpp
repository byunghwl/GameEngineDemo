
#include "DXUT.H"
#include "ASEMesh.h"
#include "Missile.h"
#include "GameObject.h"
#include "AirCraft.h"

//////////////////////////////////////////////////////////////////////////
//외부 전역변수 추가



#define ROT_VELO 0.02f //회전 각속도


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//여기서부터 AirCraft 함수정의
// 생성자
AirCraft::AirCraft()
	:  m_pAseMesh(NULL), m_pHFirePos_R(nullptr), m_pHFirePos_L(nullptr)
{
	m_pAseMesh = new ASEMesh();// 메쉬데이타 동적생성
	ASSERT_IF_FAIL_MemAlloc(m_pAseMesh)


}

AirCraft::~AirCraft()
{
	
	write_log_file("메모리 해제 : ~AirCraft() 호출");
	SAFE_DELETE_(m_pAseMesh)
}



//////////////////////////////////////////////////////////////////////////
// 초기화 함수

HRESULT AirCraft::Init(LPDIRECT3DDEVICE9 _pDevice, A_DATA* pAData, UNIT_DATA* pUData )
{
	HRESULT RET = S_OK;
	if(FAILED(CUnit::Init(_pDevice, pUData )))		
	{
		return FALSE;
	}

	if(pAData != nullptr)
	{
			memcpy(&m_AData, pAData, sizeof(A_DATA)); //[2014.7.26 배성환]
			
	}
	
	for(int i=0 ;i<ATT_MAX ; i++)

	m_VecRight = g_AXIS_X;
	m_VecSee   = g_AXIS_Z;
	m_VecUp    = g_AXIS_Y;

	m_Pos = D3DXVECTOR3(0,0,40);
	
	RET = InitGameObject(); //ase파일 읽어와서 버텍스와 인덱스버퍼를 설정 
	m_ADatabk = m_AData;
	return RET;
}




//////////////////////////////////////////////////////////////////////////
//

int AirCraft::InitGameObject()
{
	
	if(m_pAseMesh ==NULL) {write_log_file("InitGameObject() 함수 실패, m_pAseMesh가 NULL 입니다.");  return S_FALSE;}
		
	m_pAseMesh->createMesh(m_pDevice); //메쉬생성해주기


	//발사위치를 가지고 있는 헬퍼오브젝트를 찾아서 그 포인터를 멤버변수로 저장한다.  [2014.7.26 배성환]
	const HELPEROBJECT *pH;
	pH = m_pAseMesh->Get_HelperObject("FirePos_L");   //파이어포지션 왼쪽 찾기
	if(pH !=NULL)
	{ 
		m_pHFirePos_L = pH;
	}
	else
	{ LOG_WRITE("FirePos_L 찾기 실패!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");	 return S_FALSE;}

	pH = m_pAseMesh->Get_HelperObject("FirePos_R");//파이어포지션 오른쪽 찾기
	if(pH !=NULL)
	{ 
		m_pHFirePos_R = pH;
	}
	else
	{ LOG_WRITE("FirePos_R 찾기 실패!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");	
		return S_FALSE;
	}
	

	return S_OK;

}


//////////////////////////////////////////////////////////////////////////
//

void AirCraft::Draw()
{	
	//월드행렬 설정
	//m_pDevice->SetTransform(D3DTS_WORLD,&m_MWorld);
	m_pAseMesh->Draw(m_pDevice);

}


//////////////////////////////////////////////////////////////////////////
//
void AirCraft::Update(float _dTime)
{
	m_isRotating = false;
	//키입력에 따라 이동행렬을 더해줌
	if( GetAsyncKeyState('A')) 
	{		MoveLeft(_dTime); 	}//왼쪽으로 이동

	if( GetAsyncKeyState('D')) 
	{		MoveRight(_dTime);	}// 오른쪽으로 이동

	if( GetAsyncKeyState('S')) 
	{		MoveBack(_dTime); 	}   //뒤로 이동

	if( GetAsyncKeyState('W')) 
	{		MoveForward(_dTime);	} //앞으로 이동

	if( GetAsyncKeyState('Z')) 
	{		MoveUp(_dTime);	}  // 위로 이동

	if( GetAsyncKeyState('X')) 
	{		MoveDown(_dTime);	} //아래로 이동

	if( GetAsyncKeyState(VK_DOWN)) 
	{		Rot_Pitch(-1.0f*ROT_VELO);	m_isRotating = true;} //pitch 회전

	if( GetAsyncKeyState(VK_UP)) 
	{		Rot_Pitch(ROT_VELO);	m_isRotating = true;}//pitch 회전

	if( GetAsyncKeyState(VK_LEFT)) 
	{		Rot_Roll(ROT_VELO);	 m_isRotating = true;}   //Roll 회전

	if( GetAsyncKeyState(VK_RIGHT)) 
	{		Rot_Roll(-1.0f*ROT_VELO); m_isRotating = true;	}//Roll 회전

	if( GetAsyncKeyState(VK_NUMPAD4)) 
	{		Rot_Yaw(ROT_VELO);	m_isRotating = true;}		// yaw 회전

	if( GetAsyncKeyState(VK_NUMPAD6)) 
	{		Rot_Yaw(-1.0f*ROT_VELO);	m_isRotating = true;}// yaw 회전


	D3DXMatrixIdentity(&m_MTrans);
	D3DXMatrixTranslation(&m_MTrans, m_Pos.x , m_Pos.y , m_Pos.z);

	m_MWorld = m_MRot * m_MTrans;//최종행렬 계산
	
	m_pAseMesh->Update(_dTime,m_MWorld);
	
}

//////////////////////////////////////////////////////////////////////////
//
void AirCraft::MoveLeft(float& _dTime)
{	

	m_Pos  -= m_VecRight*m_UData.fVelo*_dTime;
	
}

//////////////////////////////////////////////////////////////////////////
//
void AirCraft::MoveRight(float& _dTime)
{
	m_Pos  += m_VecRight*m_UData.fVelo*_dTime;

}


//////////////////////////////////////////////////////////////////////////
void AirCraft::MoveDown(float& _dTime)
{
	m_Pos  -= m_VecUp*m_UData.fVelo*_dTime;
}


//////////////////////////////////////////////////////////////////////////
void AirCraft::MoveUp(float& _dTime)
{
	m_Pos  += m_VecUp*m_UData.fVelo*_dTime;
}


//////////////////////////////////////////////////////////////////////////
void AirCraft::MoveForward(float& _dTime)
{
		m_Pos  += m_VecSee*m_UData.fVelo*_dTime;
}



//////////////////////////////////////////////////////////////////////////
void AirCraft::MoveBack(float& _dTime)
{
	m_Pos  -= m_VecSee*m_UData.fVelo*_dTime;
}

//////////////////////////////////////////////////////////////////////////
//
void AirCraft::Rot_Pitch(float _Angle)// 우향벡터를 기준으로 회전
{
	m_Angle_Pitch += _Angle; //카메라 회전을 위해 얼만큼 회전했는지 각도를 누적해준다. 

	D3DXMatrixIdentity(&m_MRotPitch);

	D3DXMatrixRotationAxis(&m_MRotPitch, &g_AXIS_X, _Angle);
	m_MRot = m_MRotPitch * m_MRot;
 	D3DXVec3TransformNormal(&m_VecSee ,&g_AXIS_Z ,&m_MRot);//바라보는벡터를 회전시켜준다.
	D3DXVec3TransformNormal(&m_VecUp ,&g_AXIS_Y ,&m_MRot);// 업벡터를 회전시켜 준다.

}

//////////////////////////////////////////////////////////////////////////
//

void AirCraft::Rot_Roll(float _Angle)//전방벡터 기준으로 회전
{
	m_Angle_Roll += _Angle; //카메라 회전을 위해 얼만큼 회전했는지 각도를 누적해준다. 

	D3DXMatrixIdentity(&m_MRotRoll);

	D3DXMatrixRotationAxis(&m_MRotRoll, &g_AXIS_Z,_Angle);
	m_MRot = m_MRotRoll * m_MRot;
	D3DXVec3TransformNormal(&m_VecRight ,&g_AXIS_X ,&m_MRot);// 우측방향 벡터를 회전시켜준다.
	D3DXVec3TransformNormal(&m_VecUp ,&g_AXIS_Y ,&m_MRot);// 업벡터를 회전시켜 준다.

}

///////////////////////////////////////////////////////////////////////////
//

void AirCraft::Rot_Yaw(float _Angle)//상향벡터기준으로 회전
{
	m_Angle_Yaw += _Angle; //카메라 회전을 위해 얼만큼 회전했는지 각도를 누적해준다. 

	D3DXMatrixIdentity(&m_MRotYaw);
	D3DXMatrixRotationAxis(&m_MRotYaw, &g_AXIS_Y, _Angle);
	m_MRot = m_MRotYaw * m_MRot;
 	D3DXVec3TransformNormal(&m_VecRight ,&g_AXIS_X ,&m_MRot);// 우측방향 벡터를 회전시켜준다.
 	D3DXVec3TransformNormal(&m_VecSee ,&g_AXIS_Z ,&m_MRot);// 시선벡터를 회전시켜 준다.
}

//-----------------------------------------------------------------------------
// 키보드나 마우스 입력 처리
//-----------------------------------------------------------------------------
bool AirCraft::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
// 	case WM_LBUTTONDOWN:
// 		{
// 			SetCapture( hWnd );
// 			return TRUE;
// 		}
// 
// 	case WM_LBUTTONUP:
// 		{
// 			ReleaseCapture();
// 			break;
// 		}
// 	case WM_MOUSEMOVE : 
// 		{
// 
// 		}
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_SPACE:
			{
				if(m_AData.MissileCnt >0)
				{
					m_AData.MissileCnt--;
					SHUtil::Rand_BezierPos(G_Missile.m_BezierPoint , G_Missile.m_Pos, D3DXVECTOR3(0,400,1300), 4,
						G_Missile.m_Pos + G_Missile.m_LookVec* 100.0f );
					G_Missile.SetFire(true);
				}
			}
			break;
		}
		break;

	}
	return true;
}

