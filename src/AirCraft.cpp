
#include "DXUT.H"
#include "ASEMesh.h"
#include "Missile.h"
#include "GameObject.h"
#include "AirCraft.h"

//////////////////////////////////////////////////////////////////////////
//�ܺ� �������� �߰�



#define ROT_VELO 0.02f //ȸ�� ���ӵ�


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
//���⼭���� AirCraft �Լ�����
// ������
AirCraft::AirCraft()
	:  m_pAseMesh(NULL), m_pHFirePos_R(nullptr), m_pHFirePos_L(nullptr)
{
	m_pAseMesh = new ASEMesh();// �޽�����Ÿ ��������
	ASSERT_IF_FAIL_MemAlloc(m_pAseMesh)


}

AirCraft::~AirCraft()
{
	
	write_log_file("�޸� ���� : ~AirCraft() ȣ��");
	SAFE_DELETE_(m_pAseMesh)
}



//////////////////////////////////////////////////////////////////////////
// �ʱ�ȭ �Լ�

HRESULT AirCraft::Init(LPDIRECT3DDEVICE9 _pDevice, A_DATA* pAData, UNIT_DATA* pUData )
{
	HRESULT RET = S_OK;
	if(FAILED(CUnit::Init(_pDevice, pUData )))		
	{
		return FALSE;
	}

	if(pAData != nullptr)
	{
			memcpy(&m_AData, pAData, sizeof(A_DATA)); //[2014.7.26 �輺ȯ]
			
	}
	
	for(int i=0 ;i<ATT_MAX ; i++)

	m_VecRight = g_AXIS_X;
	m_VecSee   = g_AXIS_Z;
	m_VecUp    = g_AXIS_Y;

	m_Pos = D3DXVECTOR3(0,0,40);
	
	RET = InitGameObject(); //ase���� �о�ͼ� ���ؽ��� �ε������۸� ���� 
	m_ADatabk = m_AData;
	return RET;
}




//////////////////////////////////////////////////////////////////////////
//

int AirCraft::InitGameObject()
{
	
	if(m_pAseMesh ==NULL) {write_log_file("InitGameObject() �Լ� ����, m_pAseMesh�� NULL �Դϴ�.");  return S_FALSE;}
		
	m_pAseMesh->createMesh(m_pDevice); //�޽��������ֱ�


	//�߻���ġ�� ������ �ִ� ���ۿ�����Ʈ�� ã�Ƽ� �� �����͸� ��������� �����Ѵ�.  [2014.7.26 �輺ȯ]
	const HELPEROBJECT *pH;
	pH = m_pAseMesh->Get_HelperObject("FirePos_L");   //���̾������� ���� ã��
	if(pH !=NULL)
	{ 
		m_pHFirePos_L = pH;
	}
	else
	{ LOG_WRITE("FirePos_L ã�� ����!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");	 return S_FALSE;}

	pH = m_pAseMesh->Get_HelperObject("FirePos_R");//���̾������� ������ ã��
	if(pH !=NULL)
	{ 
		m_pHFirePos_R = pH;
	}
	else
	{ LOG_WRITE("FirePos_R ã�� ����!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");	
		return S_FALSE;
	}
	

	return S_OK;

}


//////////////////////////////////////////////////////////////////////////
//

void AirCraft::Draw()
{	
	//������� ����
	//m_pDevice->SetTransform(D3DTS_WORLD,&m_MWorld);
	m_pAseMesh->Draw(m_pDevice);

}


//////////////////////////////////////////////////////////////////////////
//
void AirCraft::Update(float _dTime)
{
	m_isRotating = false;
	//Ű�Է¿� ���� �̵������ ������
	if( GetAsyncKeyState('A')) 
	{		MoveLeft(_dTime); 	}//�������� �̵�

	if( GetAsyncKeyState('D')) 
	{		MoveRight(_dTime);	}// ���������� �̵�

	if( GetAsyncKeyState('S')) 
	{		MoveBack(_dTime); 	}   //�ڷ� �̵�

	if( GetAsyncKeyState('W')) 
	{		MoveForward(_dTime);	} //������ �̵�

	if( GetAsyncKeyState('Z')) 
	{		MoveUp(_dTime);	}  // ���� �̵�

	if( GetAsyncKeyState('X')) 
	{		MoveDown(_dTime);	} //�Ʒ��� �̵�

	if( GetAsyncKeyState(VK_DOWN)) 
	{		Rot_Pitch(-1.0f*ROT_VELO);	m_isRotating = true;} //pitch ȸ��

	if( GetAsyncKeyState(VK_UP)) 
	{		Rot_Pitch(ROT_VELO);	m_isRotating = true;}//pitch ȸ��

	if( GetAsyncKeyState(VK_LEFT)) 
	{		Rot_Roll(ROT_VELO);	 m_isRotating = true;}   //Roll ȸ��

	if( GetAsyncKeyState(VK_RIGHT)) 
	{		Rot_Roll(-1.0f*ROT_VELO); m_isRotating = true;	}//Roll ȸ��

	if( GetAsyncKeyState(VK_NUMPAD4)) 
	{		Rot_Yaw(ROT_VELO);	m_isRotating = true;}		// yaw ȸ��

	if( GetAsyncKeyState(VK_NUMPAD6)) 
	{		Rot_Yaw(-1.0f*ROT_VELO);	m_isRotating = true;}// yaw ȸ��


	D3DXMatrixIdentity(&m_MTrans);
	D3DXMatrixTranslation(&m_MTrans, m_Pos.x , m_Pos.y , m_Pos.z);

	m_MWorld = m_MRot * m_MTrans;//������� ���
	
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
void AirCraft::Rot_Pitch(float _Angle)// ���⺤�͸� �������� ȸ��
{
	m_Angle_Pitch += _Angle; //ī�޶� ȸ���� ���� ��ŭ ȸ���ߴ��� ������ �������ش�. 

	D3DXMatrixIdentity(&m_MRotPitch);

	D3DXMatrixRotationAxis(&m_MRotPitch, &g_AXIS_X, _Angle);
	m_MRot = m_MRotPitch * m_MRot;
 	D3DXVec3TransformNormal(&m_VecSee ,&g_AXIS_Z ,&m_MRot);//�ٶ󺸴º��͸� ȸ�������ش�.
	D3DXVec3TransformNormal(&m_VecUp ,&g_AXIS_Y ,&m_MRot);// �����͸� ȸ������ �ش�.

}

//////////////////////////////////////////////////////////////////////////
//

void AirCraft::Rot_Roll(float _Angle)//���溤�� �������� ȸ��
{
	m_Angle_Roll += _Angle; //ī�޶� ȸ���� ���� ��ŭ ȸ���ߴ��� ������ �������ش�. 

	D3DXMatrixIdentity(&m_MRotRoll);

	D3DXMatrixRotationAxis(&m_MRotRoll, &g_AXIS_Z,_Angle);
	m_MRot = m_MRotRoll * m_MRot;
	D3DXVec3TransformNormal(&m_VecRight ,&g_AXIS_X ,&m_MRot);// �������� ���͸� ȸ�������ش�.
	D3DXVec3TransformNormal(&m_VecUp ,&g_AXIS_Y ,&m_MRot);// �����͸� ȸ������ �ش�.

}

///////////////////////////////////////////////////////////////////////////
//

void AirCraft::Rot_Yaw(float _Angle)//���⺤�ͱ������� ȸ��
{
	m_Angle_Yaw += _Angle; //ī�޶� ȸ���� ���� ��ŭ ȸ���ߴ��� ������ �������ش�. 

	D3DXMatrixIdentity(&m_MRotYaw);
	D3DXMatrixRotationAxis(&m_MRotYaw, &g_AXIS_Y, _Angle);
	m_MRot = m_MRotYaw * m_MRot;
 	D3DXVec3TransformNormal(&m_VecRight ,&g_AXIS_X ,&m_MRot);// �������� ���͸� ȸ�������ش�.
 	D3DXVec3TransformNormal(&m_VecSee ,&g_AXIS_Z ,&m_MRot);// �ü����͸� ȸ������ �ش�.
}

//-----------------------------------------------------------------------------
// Ű���峪 ���콺 �Է� ó��
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

