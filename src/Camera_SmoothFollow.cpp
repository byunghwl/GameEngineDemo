#include "DXUT.H"
#include "ASEData.h"
#include "GameObject.h"
#include "Camera_SmoothFollow.h"


//////////////////////////////////////////////////////////////////////////
//�������� ����



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// ���⼭���� CCamera_SmoothFollow Ŭ���� ����Լ�
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



// ������
CCamera_SmoothFollow::CCamera_SmoothFollow() :m_pAirCraft(nullptr)
{
	
	
}

//////////////////////////////////////////////////////////////////////////
// �Ҹ���
CCamera_SmoothFollow::~CCamera_SmoothFollow()
{
	SAFE_DELETE_(m_pAseCData)
	write_log_file("�޸� ���� : camera.cpp,  ~CCamera_SmoothFollow() ȣ��");
}


//-----------------------------------------------------------------------------
// ī�޶� �ʱ�ȭ �Լ� 
// CGameObject* pAirCraft  : ���ӿ�����Ʈ ������
// int _Mode : ī�޶� ��� (enum��) , �⺻�� =0
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
// ������Ʈ �Լ�

void CCamera_SmoothFollow::Update(float _dTime)
{
	CCamera::Update(_dTime);//�ݵ�� �θ�Ŭ������ ������Ʈ �Լ��� ���� ����ؾ��Ѵ�.

	if(m_CameraMode == CM_SMOOTHFOLLOW_ )
	{
		Animation(_dTime);  //�����ִϸ��̼� �����ִ� �Լ�
		m_pos =  m_pAirCraft->GetPos() + -69.0f*m_CurLook + m_CurUp*50.0f;//ī�޶� ��ġ�� ������� �ü������� �ݴ�������� �����ش�

		//ī�޶� ��� ���� and ����
	
		
		m_pDevice->SetTransform(D3DTS_VIEW, &m_ViewMat);
	}
	

}


//////////////////////////////////////////////////////////////////////////
//PitchAngledl�� _Angle ���� ������� ȸ������ ũ�ٸ� true

bool CCamera_SmoothFollow::isPitchAngle_Larger(float _Angle)
{
	if( (m_pAirCraft->GetAngle_Pitch() > _Angle) || (m_pAirCraft->GetAngle_Pitch() < -_Angle) ) //pitch���� ���밪�� _Angle �̻��̶��
	{
		m_pAirCraft->SetAngle_Pitch(0.0f);//�������״� ������ �ٽ� 0���� ������ش�.
		m_pAirCraft->SetAngle_Roll(0.0f);//�������״� ������ �ٽ� 0���� ������ش�.
		m_pAirCraft->SetAngle_Yaw(0.0f);//�������״� ������ �ٽ� 0���� ������ش�.
		return true;
	}	 
	return false;
}



//////////////////////////////////////////////////////////////////////////
//RollAngledl�� _Angle ���� ������� ȸ������ ũ�ٸ� true

bool CCamera_SmoothFollow::isRollAngle_Larger(float _Angle)
{
	if( (m_pAirCraft->GetAngle_Roll() > _Angle) || (m_pAirCraft->GetAngle_Roll() < -_Angle) ) //Roll���� ���밪�� _Angle �̻��̶��
	{
		m_pAirCraft->SetAngle_Pitch(0.0f);//�������״� ������ �ٽ� 0���� ������ش�.
		m_pAirCraft->SetAngle_Roll(0.0f);//�������״� ������ �ٽ� 0���� ������ش�.
		m_pAirCraft->SetAngle_Yaw(0.0f);//�������״� ������ �ٽ� 0���� ������ش�.
		return true;
	}
	return false;
}



//////////////////////////////////////////////////////////////////////////
//YawAngle�� _Angle ���� ������� ȸ������ ũ�ٸ� true

bool CCamera_SmoothFollow::isYawAngle_Larger(float _Angle)
{
	if( (m_pAirCraft->GetAngle_Yaw() > _Angle) || (m_pAirCraft->GetAngle_Yaw() < -_Angle) ) //Yaw���� ���밪�� _Angle �̻��̶��
	{
		m_pAirCraft->SetAngle_Pitch(0.0f);//�������״� ������ �ٽ� 0���� ������ش�.
		m_pAirCraft->SetAngle_Roll(0.0f);//�������״� ������ �ٽ� 0���� ������ش�.
		m_pAirCraft->SetAngle_Yaw(0.0f);//�������״� ������ �ٽ� 0���� ������ش�.
		return true;
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////
// �����ִϸ��̼� �Լ�

void CCamera_SmoothFollow::Lerp(float dTime)
{

	D3DXQUATERNION CurQ;//���� �������� ���ʹϾ�, ���������, ���ο� �����

	if(m_pAirCraft->GetIsRotating() ) //��� ȸ���ϰ� �ִ� ���¶�� 
	{
		m_MNewRot = m_pAirCraft->GetMRot(); //ȸ���� ����� �����´�.
		
// 		m_pAirCraft->SetAngle_Pitch(0.0f);//�������״� ������ �ٽ� 0���� ������ش�.
// 		m_pAirCraft->SetAngle_Roll(0.0f); //�������״� ������ �ٽ� 0���� ������ش�.
// 		m_pAirCraft->SetAngle_Yaw(0.0f);  //�������״� ������ �ٽ� 0���� ������ش�.
		D3DXQuaternionRotationMatrix(&m_New_Q , &m_MNewRot);
	}
	
	m_CurAniProcess += dTime;//*dTime;//���� ���� ���� ��Ȳ ,, 0~1������ �� 
	
	
	if(m_CurAniProcess >1.0f)
	{
		D3DXVec3TransformNormal(&m_CurLook, &m_OldLook,  &m_MNewRot);
		D3DXVec3TransformNormal(&m_CurUp, &m_OldUp,  &m_MNewRot);
		D3DXVec3TransformNormal(&m_CurRight, &m_OldRight,  &m_MNewRot);

		isRotAni_Pitch =isRotAni_Roll =isRotAni_Yaw =false;
		m_CurAniProcess = 0.0f;
	
		return ;
	}
	
	// old������� new ������� ���� ���� ������  ���ش�.
	D3DXQuaternionSlerp(&CurQ, &m_Old_Q, &m_New_Q,m_CurAniProcess);

	// cur������� ���������� ȸ����ķ� �ٲ۴�.
	D3DXMatrixRotationQuaternion(&m_MCurRot, &CurQ);

	D3DXVec3TransformNormal(&m_CurLook, &m_OldLook,  &m_MCurRot);
	D3DXVec3TransformNormal(&m_CurUp, &m_OldUp,  &m_MCurRot);
	D3DXVec3TransformNormal(&m_CurRight, &m_OldRight,  &m_MCurRot);
	
	
}

//-----------------------------------------------------------------------------
// smooth follow �ִϸ��̼�
//-----------------------------------------------------------------------------

void CCamera_SmoothFollow::Animation(float _dTime)
{

	if(isPitchAngle_Larger(0.523f)   ) // pitch ȸ�������� 30�� �̻��̶��
	{
	
		m_MNewRot = m_pAirCraft->GetMRot(); //ȸ���� ����� �����´�.

		isRotAni_Pitch = true; //pitch ȸ�� �ִϸ��̼� �÷��׸� true �� �����. (���� �ִϸ��̼� ����)

		m_MOldRot = m_MCurRot; //���� ȸ������� old ȸ����Ŀ� �־��ش�.
	
		m_CurAniProcess =0.0f; //���� �ִϸ��̼� �����Ȳ(0~1 ������ ��)

		//���� ȸ������� old ������� ������ش�.
		D3DXQuaternionRotationMatrix(&m_Old_Q , &m_MOldRot);
		//���ο� ȸ������� new ������� ������ش�.
		D3DXQuaternionRotationMatrix(&m_New_Q , &m_MNewRot);
	  
	}

	if(isRollAngle_Larger(0.523f)  )// roll ȸ�������� 45�� �̻��̶��
	{	
		m_MNewRot = m_pAirCraft->GetMRot();
		isRotAni_Roll = true;  //pitch ȸ�� �ִϸ��̼� �÷��׸� true �� �����. 
		m_MOldRot = m_MCurRot;
		m_CurAniProcess =0.0f;
		
		//���� ȸ������� old ������� ������ش�.
		D3DXQuaternionRotationMatrix(&m_Old_Q , &m_MOldRot);
		//���ο� ȸ������� new ������� ������ش�.
		D3DXQuaternionRotationMatrix(&m_New_Q , &m_MNewRot);	    
	}

	if(isYawAngle_Larger(0.523f)  )//yaw ȸ�������� 45�� �̻��̶��
	{
		m_MNewRot = m_pAirCraft->GetMRot();
		isRotAni_Yaw = true; //pitch ȸ�� �ִϸ��̼� �÷��׸� true �� �����. 
		m_MOldRot = m_MCurRot;
		m_CurAniProcess = 0.0f;		

		//���� ȸ������� old ������� ������ش�.
		D3DXQuaternionRotationMatrix(&m_Old_Q , &m_MOldRot);
		//���ο� ȸ������� new ������� ������ش�.
		D3DXQuaternionRotationMatrix(&m_New_Q , &m_MNewRot);
		//m_OldLook = m_CurLook;    m_OldRight = m_CurRight;    m_OldUp = m_CurUp;     
	}

	if(isRotAni_Pitch || isRotAni_Roll || isRotAni_Yaw)  // ���� �ϳ��� ������ �������� �̻��� �ȴٸ� ..
	{
		Lerp(_dTime);// ���������ִ� �Լ� ,,(�̰����� ī�޶��� _look, _up, _right�� ������Ʈ������)
	}
}


//****************************************** �� ************************************************//