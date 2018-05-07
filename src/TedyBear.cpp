
#include "DXUT.H"
#include "ASEMesh.h"
#include "TedyBear.h"

//////////////////////////////////////////////////////////////////////////
//�ܺ� �������� �߰�



#define ROT_VELO 0.02f //ȸ�� ���ӵ�

CBear*		 G_pTedyBear = nullptr;
// �����̰� �����Ǿ����� ��ó�� �ٶ󺸰� �ִ� ���� 
D3DXVECTOR3  G_Vec_Start = D3DXVECTOR3(0.0f, 0.0f, -1.0f); 
// 8���⺤�� �̸� ����� ���� 
D3DXVECTOR3  G_Vec_Right = D3DXVECTOR3(1.0f, 0.0f, 0.0f); // ������
D3DXVECTOR3  G_Vec_Left = D3DXVECTOR3(-1.0f, 0.0f, 0.0f); // ����
D3DXVECTOR3  G_Vec_Front = D3DXVECTOR3(0.0f, 0.0f, 1.0f); // ����
D3DXVECTOR3  G_Vec_Back = D3DXVECTOR3(0.0f, 0.0f, -1.0f); // ����
D3DXVECTOR3  G_Vec_FR = D3DXVECTOR3(0.707f, 0.0f, 0.707f); // ����+������
D3DXVECTOR3  G_Vec_FL = D3DXVECTOR3(-0.707f, 0.0f, 0.707f); // ����+����
D3DXVECTOR3  G_Vec_BR = D3DXVECTOR3(0.707f, 0.0f, -0.707f); // ����+������
D3DXVECTOR3  G_Vec_BL = D3DXVECTOR3(-0.707f, 0.0f, -0.707f); // ��+����


//////////////////////////////////////////////////////////////////////////

CString		CBear::m_EntityType = "Monster"; // ��ƼƼ�� Ÿ�� �̸�
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------
// struct BEAR_DATA �� ������   [2014.7.29 ]
//-------------------------------------------------------------------------

BEAR_DATA::BEAR_DATA()
{

}



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
//���⼭���� CBear �Լ�����
// ������
CBear::CBear() //:  m_pMesh(nullptr)
{
	//m_hTech_Skin = G_FXShader.GetPointer()->GetTechniqueByName("Skinning_DirLight_SpecularMap");
	//m_hTech_Skin = G_FXShader.GetPointer()->GetTechniqueByName("Skinning_PointLight");
	//D3DXHANDLE TempHandle = G_FXShader.GetPointer()->GetTechniqueByName("Outline_PNT");
	//m_RscPack.m_pSkinMesh->Set_m_hTech("Outline_PNT");
	//m_RscPack.m_hTech_noSkin =  G_FXShader.GetPointer()->GetTechniqueByName("Rigid_Tex"); //[2014.11.20 ]
	//m_RscPack.m_MeshList.Set_hTech_AllMesh("Rigid_Tex");
	D3DXMatrixIdentity(&m_MTrans);

	D3DXMatrixIdentity(&m_MRot);

	D3DXMatrixIdentity(&m_MRotPitch);
	D3DXMatrixIdentity(&m_MRotRoll);
	D3DXMatrixIdentity(&m_MRotYaw);
	// ���� �����̰� �ٶ󺸰��ִ¹���
	m_VecSee = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	m_Vec_DestVector = m_VecSee; 
	m_Angle_Yaw = 0.0f;  // up vector�� �������� ȸ���� ����

	m_SkillbitFlag = 0; 
	m_BearData.MoveSpeed = 76.0f; 
	m_BearData.RotSpeed = 5.4f;

	m_pBoundSphere_Stick = nullptr; 		
	m_pBoundSphere_Body = nullptr;
	m_SkillPoint = 0;

	 	GUI_Base* pPrg = CDialogRscMgr::GetInstance()->GetControl(eUI_ID_SkillPointBar);
	 	pPrg->SetPercentage(m_SkillPoint);

		pPrg = CDialogRscMgr::GetInstance()->GetControl(eUI_ID_HPBar);
		pPrg->SetPercentage(G_GirlHP);

		m_pEffMagicStick = nullptr;
		
}

CBear::~CBear()
{
	write_log_file("�޸� ���� : ~CBear() ȣ��");

	SAFE_DELETE_(m_pEffMagicStick )

}



//-------------------------------------------------------------------------
// Arg	  : D3DXVECTOR3 _SPos	 : ������ġ (�⺻�� D3DXVECTOR3(0,0,200))
//			BEAR_DATA* pMData : ���� �Ӽ� �����͸� �������ִ� ����ü
// Return : ����(S_OK), ����(S_FALSE)
// Desc   : CBear Ŭ������ �ݵ�� ���Լ��� �ʱ�ȭ���� ����Ѵ�. [2014.10.7 ]
//-------------------------------------------------------------------------

int  CBear::Init(LPDIRECT3DDEVICE9 _pDevice, D3DXVECTOR3 _SPos /*= D3DXVECTOR3(0,0,200)*/, BEAR_DATA* pBData )
{
	HRESULT RET = S_OK;
	if(FAILED(CEntity::Init(_pDevice)))		
	{
		return FALSE;
	}

	if(pBData != nullptr)
	{
		memcpy(&m_BearData, pBData, sizeof(BEAR_DATA));// [2014.10.7 ]
	}

	m_VecRight = D3DXVECTOR3(1,0,0);
	m_VecSee   = D3DXVECTOR3(0,0,-1); 
	m_VecUp    = D3DXVECTOR3(0,1,0);

	m_Pos = _SPos;
	
	m_BearDatabk = m_BearData;

	return RET;

}




//-------------------------------------------------------------------------
// ���⼭ �޽� �������ֱ�!!!!!!!!!!!!
//-------------------------------------------------------------------------
int CBear::InitGameObject()
{


	return S_OK;

}

//-------------------------------------------------------------------------
// Arg	  : 
// Return : HRESULT S_OK , S_FALSE
// Desc   : �̰����� �ݶ��̴��� �����ؾ���(�ٿ�彺�Ǿ��)//[2014.11.11 ]
//-------------------------------------------------------------------------
// from ICollidable
HRESULT		CBear::InitCollider() 
{
	HRESULT hr = S_OK;

	m_pCollidObj = new CBoundSphere();

	return hr;
}

//-------------------------------------------------------------------------
// �׸��� �Լ�
//-------------------------------------------------------------------------

void CBear::Draw()
{	

	//���̴���� ����
	_OnDrawStart();
	
	m_RscPack.Draw(m_pDevice);

	m_SkillCard.Draw();
	m_SkillRockPunch.Draw(); 
	m_SkillHat.Draw(); 

	//m_EffMagicStick.Draw(); 


}
//-----------------------------------------------------------------------------
// Desc   : ������Ʈ �ϱ� ���� ȣ��Ǵ� �Լ� ,,, �̰����� �Է� ó��
//-----------------------------------------------------------------------------
void CBear::_OnUpdateStart(float _dTime)
{

	////////////////////////////////////////////////////////////////////////// //[2014.11.24 ]
	// ī�޶� ȸ�������� ���ع����� �޶����� �Ѵ�. 
	D3DXMATRIX RotTM;
	D3DXMatrixRotationY(&RotTM , SHGlobal_GetCurScene_Camera->Get_m_AngleYaw()  );
	D3DXVec3TransformNormal(&m_Vec_Front, &G_Vec_Front, &RotTM);
	D3DXVec3TransformNormal(&m_Vec_Right, &G_Vec_Right, &RotTM);
	D3DXVec3TransformNormal(&m_Vec_Left, &G_Vec_Left, &RotTM);
	D3DXVec3TransformNormal(&m_Vec_Back, &G_Vec_Back, &RotTM);
	D3DXVec3TransformNormal(&m_Vec_FR, &G_Vec_FR, &RotTM);
	D3DXVec3TransformNormal(&m_Vec_FL, &G_Vec_FL, &RotTM);
	D3DXVec3TransformNormal(&m_Vec_BL, &G_Vec_BL, &RotTM);
	D3DXVec3TransformNormal(&m_Vec_BR, &G_Vec_BR, &RotTM);


	//////////////////////////////////////////////////////////////////////////

	static float Perc = 0.0f;
	
	//////////////////////////////////////////////////////////////////////////
	// �Ϲ�Ÿ��1 [2014.11.25 ]
	if(SHG_Input.MouseDown(0) ||( m_RscPack.GetAnimator().GetAniState_By_Name("ATTACK") == eAniState_Play) )// ���ʹ�ư �ٿ�
	{
		if( ((m_SkillbitFlag & ~BitFlag_LClick) & BitFlag_Skill ) <= 0) // �ٸ���ų�� ������� �ƴ϶��!! 
		{	
			if(CAnimator::RVT_OK == m_RscPack.GetAnimator().Change_Ani_Clip("ATTACK")) // �ִ�Ŭ�� �ٲٱ� �����ߴٸ�   
			{	
				m_NormalHit_Timer.SetTimer(0.3f);  G_SHSoundMgr.PlayWave(eSound_Tedy_Atk, false);
			} 
			
		
			m_SkillbitFlag |= BitFlag_LClick;
		}
	}
	else
	{	m_SkillbitFlag &= ~BitFlag_LClick; 	}// ��Ʋ�÷��� ����
	

	//////////////////////////////////////////////////////////////////////////
	// ��ų����1  ī�峯����!!!!!!!!!!!!!!! 
	
	if(  ( SHG_Input.key(DIK_1) && (m_SkillPoint > S_POINT*3.99999f) ) || ( m_RscPack.GetAnimator().GetAniState_By_Name("SKILL1") == eAniState_Play)  )// ���ʹ�ư �ٿ�
	{
		if(((m_SkillbitFlag & ~BitFlag_1) & BitFlag_Skill ) <= 0 ) // �ٸ���ų�� ������� �ƴ϶��!! [2014.11.27 ]
		{
			if((CAnimator::RVT_OK == m_RscPack.GetAnimator().Change_Ani_Clip("SKILL1") ) && (m_SkillCard.isReady()== true)) // 
			{	
				// ��ų���������Լ� ,,, ��ų�� ������ �ð���ŭ ������Ų�� �ߵ��Ѵ�.!! 			
				m_SkillCard.SetDelayTime_for_Begin(m_PosStick,m_VecSee, m_MWorld , m_RscPack.GetAnimator().GetCurClip_EffectDelayTime() ); 
				m_SkillPoint-= S_POINT*4.0f; 
				
			} 

			m_SkillbitFlag |= BitFlag_1	; // 
		}
	}
	else
	{	m_SkillbitFlag &= ~BitFlag_1; 	}//��Ʈ�÷��� ����


	//////////////////////////////////////////////////////////////////////////
	// ��ų����2 ������ġ !!!!!!!!!!!!!!  [2014.11.26 ]
	if((SHG_Input.key(DIK_2)&& (m_SkillPoint > S_POINT*7.99999f) )||( m_RscPack.GetAnimator().GetAniState_By_Name("SKILL2") == eAniState_Play) )//����2Ű ����
	{
		if(((m_SkillbitFlag & ~BitFlag_2) & BitFlag_Skill ) <= 0 ) // �ٸ���ų�� ������� �ƴ϶��!! [2014.11.27 ]
		{
			if((CAnimator::RVT_OK == m_RscPack.GetAnimator().Change_Ani_Clip("SKILL2")) && (m_SkillRockPunch.isReady()== true)) // �ִ�Ŭ�� �ٲٱ� �����ߴٸ�   [2014.11.25 ]
			{
				// ��ų���������Լ� ,,, ��ų�� ������ �ð���ŭ ������Ų�� �ߵ��Ѵ�.!!  //[2014.11.29 ]
				m_SkillRockPunch.SetDelayTime_for_Begin(m_PosStick,m_VecSee, m_MWorld , m_RscPack.GetAnimator().GetCurClip_EffectDelayTime() ,m_BoneMesh_Hat); 
						
				m_SkillPoint-= S_POINT*8.0f;  // [2014.12.2 ]
			} 

			m_SkillbitFlag |= BitFlag_2	; // [2014.11.27 ]
		}
	}
	else
	{	m_SkillbitFlag &= ~BitFlag_2; 	}//��Ʈ�÷��� ����


	//////////////////////////////////////////////////////////////////////////
	// ��ų����3 ���ڶ����� !!!!!!!!!!!!!!!!! 
	if((SHG_Input.key(DIK_3) && (m_SkillPoint > S_POINT*11.99999f)) || ( m_RscPack.GetAnimator().GetAniState_By_Name("SKILL3") == eAniState_Play) )// ����3 ����
	{
		if(((m_SkillbitFlag & ~BitFlag_3) & BitFlag_Skill ) <= 0) // �ٸ���ų�� ������� �ƴ϶��!! 
		{
			if((CAnimator::RVT_OK == m_RscPack.GetAnimator().Change_Ani_Clip("SKILL3")) && (m_SkillHat.isReady()== true)) // �ִ�Ŭ�� �ٲٱ� �����ߴٸ� 
			{					
				// ��ų���������Լ� ,,, ��ų�� ������ �ð���ŭ ������Ų�� �ߵ��Ѵ�.!! 
				m_SkillHat.SetDelayTime_for_Begin(m_Pos  , m_VecSee, m_MWorld , m_RscPack.GetAnimator().GetCurClip_EffectDelayTime() ); 
				m_SkillPoint-= S_POINT*12.0f; 
			} 

			m_SkillbitFlag |= BitFlag_3	;
		}
	}
	else
	{	m_SkillbitFlag &= ~BitFlag_3; 	}//��Ʈ�÷��� ����


	//////////////////////////////////////////////////////////////////////////
	// �̵����϶� [2014.11.25 ]
	if( (SHG_Input.key(DIK_W) || SHG_Input.key(DIK_D) || SHG_Input.key(DIK_A) || SHG_Input.key(DIK_S)) && (BitFlag_Using_AnySkill(m_SkillbitFlag)  == 0) )
	{	
		
			if(CAnimator::RVT_OK == m_RscPack.GetAnimator().Change_Ani_Clip("RUN")) // �ִ�Ŭ�� �ٲٱ� �����ߴٸ� 
			{	m_RscPack.GetAnimator().SetAniTimeFirst();	} // Ŭ���� ù���������� �����ֱ�

			
			if(SHG_Input.key(DIK_W)) 
			{	m_Vec_DestVector = m_Vec_Front;	}
			else if(SHG_Input.key(DIK_D))
			{	m_Vec_DestVector = m_Vec_Right;		}
			else if(SHG_Input.key(DIK_A))
			{	m_Vec_DestVector = m_Vec_Left;		}
			else if(SHG_Input.key(DIK_S))
			{	m_Vec_DestVector = m_Vec_Back;		}
		
			if(SHG_Input.key(DIK_D) && SHG_Input.key(DIK_W) )
			{	m_Vec_DestVector = m_Vec_FR;		}
			else if(SHG_Input.key(DIK_A) && SHG_Input.key(DIK_W) )
			{	m_Vec_DestVector = m_Vec_FL	;		}
			else if(SHG_Input.key(DIK_D) && SHG_Input.key(DIK_S) )
			{	m_Vec_DestVector = m_Vec_BR	;		}
			else if(SHG_Input.key(DIK_A) && SHG_Input.key(DIK_S) )
			{	m_Vec_DestVector = m_Vec_BL	;		}

			// ��Ʈ�÷��׸� ���ش�. [2014.11.13 ]
			m_SkillbitFlag |= BitFlag_Move;
		
		

	}	
	else // �̵�Ű �ȴ��������� ��Ʈ�÷��� ����
	{ m_SkillbitFlag &= ~BitFlag_Move;	}


	//////////////////////////////////////////////////////////////////////////
	// �ƹ��͵� �ȴ������� IDLE [2014.11.25 ]
	if(m_SkillbitFlag == 0)
	{	
		// �ƹ��͵� �ȴ��������� ������
	    if(CAnimator::RVT_OK == m_RscPack.GetAnimator().Change_Ani_Clip("IDLE"))// �ִ�Ŭ�� �ٲٱ� �����ߴٸ� 
		{	m_RscPack.GetAnimator().SetAniTimeFirst();	} // Ŭ���� ù���������� �����ֱ�
	}


	





	
// ����Ű �� ��ư ��������
// 	else if(SHG_Input.key(DIK_SPACE))
// 	{	
// 		if(CAnimator::RVT_OK == m_RscPack.GetAnimator().Change_Ani_Clip("Jump")) // �ִ�Ŭ�� �ٲٱ� �����ߴٸ� 
// 		{	m_RscPack.GetAnimator().SetAniTimeFirst();	} // Ŭ���� ù���������� �����ֱ�
// 	}
// 	else 
// 	{	
// 		// �ƹ��͵� �ȴ��������� ������
// 		if(CAnimator::RVT_OK == m_RscPack.GetAnimator().Change_Ani_Clip("IDLE"))// �ִ�Ŭ�� �ٲٱ� �����ߴٸ� 
// 		{	m_RscPack.GetAnimator().SetAniTimeFirst();	} // Ŭ���� ù���������� �����ֱ�
// 	}
// 
// 	if(SHG_Input.key(DIK_RIGHT))  // [2014.11.5 ]
// 	{	
// 		Perc +=  0.01f;
// 		GUI_Base* pPrg = CDialogRscMgr::GetInstance()->GetControl(eUI_ID_SkillPointBar);
// 		pPrg->SetPercentage(Perc);
// 
// 	}

}

//-------------------------------------------------------------------------
// ������Ʈ �Լ�
//-------------------------------------------------------------------------

void CBear::Update(float _dTime)
{

	_OnUpdateStart(_dTime);// ���ΰ��̶�� ���⼭ Ű������ǲ ó��

	D3DXMatrixRotationY(&m_MRot , m_Angle_Yaw  );
	// �ü����͵� ������ ���� ��ŭ ȸ�������ش�. [2014.11.13 ]
	D3DXVec3TransformNormal(&m_VecSee , &G_Vec_Start, &m_MRot);

	//////////////////////////////////////////////////////////////////////////
	// ���⼭���� DestVector�� �������� ȸ����� ����ϱ�

	// �κ��ͻ����� ���� ���ϱ�
	D3DXVec3Normalize(&m_Vec_DestVector , &m_Vec_DestVector);
	D3DXVec3Normalize(&m_VecSee, &m_VecSee);
	float TempDot = D3DXVec3Dot(&m_Vec_DestVector , &m_VecSee);

	
	if(TempDot < -0.9f)//  ȸ���Ϸ��� ������ 180�� ������ ���ݴ��϶��� �ٷ� ȸ����Ų��. //[2014.11.30 ]
	{
		m_Angle_Yaw+=D3DX_PI;
	}
	else if (TempDot < 0.99f) // �������� �������� ȸ�� [2014.11.13 ]
	{
		if(  ( BitFlag_isMoving(m_SkillbitFlag) > 0 )  && ( BitFlag_Using_AnySkill(m_SkillbitFlag) <= 0 )   )// �ƹ�Ű�� �����ִٸ� [2014.11.13 ] && ��ų�� �ȴ����� �ִٸ�
		{
			if(IsLeft_Vec_Vec_3D(m_VecSee, m_Vec_DestVector) == true)
			{ 
				m_Angle_Yaw += -m_BearData.RotSpeed *_dTime;
			}
			else
			{
				m_Angle_Yaw += m_BearData.RotSpeed *_dTime;
			}
		}
	}	
	else//TempDot > 0.99f �̾ �κ��Ͱ� �����ϴٰ� �Ǵܵɶ��� ������ ���͸� ��ġ��Ų��.
	{
		m_VecSee = m_Vec_DestVector;
	}
	


	// �̵�Ű�� ������ ���� ���� �ü��������� �̵��Ѵ�. [2014.11.13 ]
	// �̵�Ű�� ������ �������� ���� ������Ʈ�� �浹üũ //[2014.11.25 ]
	if(BitFlag_isMoving(m_SkillbitFlag))
	{
		// [2014.11.25 ] �浹üũ
		m_PrePos += m_VecSee *  m_BearData.MoveSpeed * _dTime;

		if((m_PrePos.x >-229.0f) && (m_PrePos.x < 229.0f) && (m_PrePos.z > -175.0f) && (m_PrePos.z < 175.0f) )
		{
		}
		else
		{ m_PrePos = m_Pos;
		}
			
			m_pBoundSphere_Body->SetCenter(m_PrePos + D3DXVECTOR3(0.0f, 8.0f, 0.0f));// �浹��_�ٵ� ��ġ����  [2014.11.25 ]

			if(ColliChk_Using_Tag(eCollide_Tag_BoxesInRoom) == true)
			{			
				m_PrePos = m_Pos;
				m_Pos += m_Vec_Modified_See *  m_BearData.MoveSpeed * _dTime;
			}
			else // �浹üũ �ȉ�ٸ�?  [2014.11.25 ]
			{
				//m_Pos += m_VecSee * m_BearData.MoveSpeed * _dTime;
				m_Pos = m_PrePos; /*Pos�� ������Ʈ ��*/ // [2014.11.25 ]
			}
		

		
		
	
		
	}
	

	//////////////////////////////////////////////////////////////////////////
	// ���⼭���� ������� ����ϱ� 
	D3DXMatrixIdentity(&m_MTrans);
	D3DXMatrixTranslation(&m_MTrans, m_Pos.x , m_Pos.y , m_Pos.z);

	m_MWorld = m_MRot * m_MTrans;//������� ���
		
	m_RscPack.Update(_dTime, &m_MWorld);

	// �浹��_��ƽ ��ġ����  [2014.11.25 ]
	D3DXMATRIX BSTM = m_BSMesh_Stick->GetWorldTM();
	m_PosStick = D3DXVECTOR3(BSTM._41,BSTM._42, BSTM._43);
	m_pBoundSphere_Stick->SetCenter(m_PosStick);

	// 

	if(BitFlag_isLBtnClick(m_SkillbitFlag))
	{
		//////////////////////////////////////////////////////////////////////////
		//	��Ʈ��� �浹üũ !!!!!!!!!!!!!!!!!!!!!!    [2014.11.26 ]
		if(m_NormalHit_Timer.isTimeUp() == true)
		{ m_NormalHit_Timer.SetOff();// Ÿ�̸� ����� �ѹ��� �� ���������� ���´�. !! [2014.12.7 ]
			for(int i = 0 ; i< GHOSTNUM_MAX ; i++)
			{
				if(G_pGhost_Arr[i]->m_bAlive == true)
				{
					if(G_pGhost_Arr[i]->m_pBoundSphere_Body->Colli_Check(m_pBoundSphere_Stick) == true)
					{
						if(true == G_pGhost_Arr[i]->DamagedFromHit(1))//HP �� 1��ŭ �ް��ϱ� 
						{
						
								m_SkillPoint += S_POINT;

								if(m_SkillPoint>=100){ m_SkillPoint = 100;} //[2014.12.2 ]
								//GUI_Base* pPrg = CDialogRscMgr::GetInstance()->GetControl(eUI_ID_SkillPointBar);
								//pPrg->SetPercentage(m_SkillPoint);
											
						}
					}
				}

			}
		}
	}

	GUI_Base* pPrg = CDialogRscMgr::GetInstance()->GetControl(eUI_ID_SkillPointBar);
	pPrg->SetPercentage(m_SkillPoint);

	//////////////////////////////////////////////////////////////////////////
	// ���⼭ ���� �������ִ� ��ų�� ������Ʈ 
	//m_SkillCard.SetPosition(PosStick);
	m_SkillCard.Update(_dTime);
	m_SkillRockPunch.Update(_dTime); 
	m_SkillHat.Update(_dTime); 
	
	m_pEffMagicStick->Update(_dTime, m_PosStick); 
// 
	// ���⼭���� ��ų ����Ʈ������ ��ų UI ��Ȱ��ȭ 
	if(m_SkillPoint >= 99.8f)
		g_MainPlayUI.GetImage(eUI_ID_SkillImg3)->SetEnabled(true); 
	else
		g_MainPlayUI.GetImage(eUI_ID_SkillImg3)->SetEnabled(false); 


	if(m_SkillPoint >= 65.666666f)
		g_MainPlayUI.GetImage(eUI_ID_SkillImg2)->SetEnabled(true); //[2014.12.4 ]
	else
		g_MainPlayUI.GetImage(eUI_ID_SkillImg2)->SetEnabled(false); //[2014.12.4 ]
	

	if(m_SkillPoint >= 32.33332f)
		g_MainPlayUI.GetImage(eUI_ID_SkillImg1)->SetEnabled(true); //[2014.12.4 ]
	else
		g_MainPlayUI.GetImage(eUI_ID_SkillImg1)->SetEnabled(false); //[2014.12.4 ]


}



//////////////////////////////////////////////////////////////////////////
//
void CBear::MoveLeft(float& _dTime)
{	

	//m_Pos  -= m_VecRight*m_UData.fVelo*_dTime;

}

//////////////////////////////////////////////////////////////////////////
//
void CBear::MoveRight(float& _dTime)
{
	//m_Pos  += m_VecRight*m_UData.fVelo*_dTime;

}


//////////////////////////////////////////////////////////////////////////
void CBear::MoveDown(float& _dTime)
{
	//m_Pos  -= m_VecUp*m_UData.fVelo*_dTime;
}


//////////////////////////////////////////////////////////////////////////
void CBear::MoveUp(float& _dTime)
{
	//m_Pos  += m_VecUp*m_UData.fVelo*_dTime;
}


//////////////////////////////////////////////////////////////////////////
void CBear::MoveForward(float& _dTime)
{
	//m_Pos  += m_VecSee*m_UData.fVelo*_dTime;
}



//////////////////////////////////////////////////////////////////////////
void CBear::MoveBack(float& _dTime)
{
	//m_Pos  -= m_VecSee*m_UData.fVelo*_dTime;
}

//////////////////////////////////////////////////////////////////////////
//
void CBear::Rot_Pitch(float _Angle)// ���⺤�͸� �������� ȸ��
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

void CBear::Rot_Roll(float _Angle)//���溤�� �������� ȸ��
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
void CBear::Rot_Yaw(float _Angle)//���⺤�ͱ������� ȸ��
{
	m_Angle_Yaw += _Angle; //ī�޶� ȸ���� ���� ��ŭ ȸ���ߴ��� ������ �������ش�. 

	D3DXMatrixIdentity(&m_MRotYaw);
	D3DXMatrixRotationAxis(&m_MRotYaw, &g_AXIS_Y, _Angle);
	m_MRot = m_MRotYaw * m_MRot;
	D3DXVec3TransformNormal(&m_VecRight ,&g_AXIS_X ,&m_MRot);// �������� ���͸� ȸ�������ش�.
	D3DXVec3TransformNormal(&m_VecSee ,&g_AXIS_Z ,&m_MRot);// �ü����͸� ȸ������ �ش�.
}


//-----------------------------------------------------------------------------
// Desc   : draw �ϱ� ���� ȣ��Ǵ� �Լ�
//-----------------------------------------------------------------------------
void	CBear::_OnDrawStart()
{
	// ���̴� ��� ����
	{

		// ���ũ���� ������ �����Ҷ� �ݵ�� ���� �ؼ� �ڵ��� ������ ���´�.[2014.10.29 ]
// 		if(SHG_Input.key(DIK_P))
// 			m_hTech_Skin = G_FXShader.GetPointer()->GetTechniqueByName("Skinning_DirLight_SpecularMap");
// 		else
// 			m_hTech_Skin = G_FXShader.GetPointer()->GetTechniqueByName("Skinning_DirLight");
		// 1. ����Ʈ ��ü �������� [2014.10.29 ]
		LPD3DXEFFECT pEff = G_FXShader.GetPointer();                             

		CCamera* pThisSceneCamera = SHGlobal_GetCurScene_Camera;
		D3DXMATRIX tempViewProjTM; //
		tempViewProjTM = pThisSceneCamera->Get_ViewProjMat();
		// viewPorj TM �� �ڵ�� ���̴��� ���� [2014.10.27 ]
		pEff->SetMatrix(G_ConstantTable.h_ViewProjTM , &tempViewProjTM);

		//�ܰ��� Į��  [2014.11.13 ]
		D3DXCOLOR OutLineColor = D3DXCOLOR(0.1f,0.1f,0.1f,1);
		pEff->SetVector(G_ConstantTable.h_LineColor , (D3DXVECTOR4*)&OutLineColor);
		 //�ܰ��� �α� [2014.11.13 ]
		pEff->SetFloat(G_ConstantTable.h_LineOffset , 0.2f);			
		//����Ʈ����Ʈ ��ġ  [2014.11.13 ]
		D3DXVECTOR4 TempPos = D3DXVECTOR4(m_Pos.x,  m_Pos.y + 100.0f ,m_Pos.z, 1.0f);
		pEff->SetVector(G_ConstantTable.h_PointLightPos , (D3DXVECTOR4*)&TempPos);

		pEff->SetFloat(G_ConstantTable.h_PointLightRadius , G_PointLightRadius);

		pEff->SetVector(G_ConstantTable.h_LightDiff , (D3DXVECTOR4*)&G_LightDiff);

		D3DXVECTOR4 TempLight = (D3DXVECTOR4)(G_vecDirLight * -1.0f);
		TempLight.w  = 0.0f;
		pEff->SetVector(G_ConstantTable.h_LightDir , (D3DXVECTOR4*)&TempLight);

		pEff->SetVector(G_ConstantTable.h_LightAmbi , (D3DXVECTOR4*)&G_LightAmbi);
// 		CCamera TempCam = *pThisSceneCamera; �ڡڡ� ���߿� �� ���� Ȯ���ϱ� �ڡڡ�
// 		float a = TempCam.GetPos().x;
// 		float b =TempCam.GetPos().y;
// 		float c = TempCam.GetPos().z;
// 		float d = 1.0f;
		D3DXVECTOR4 ViewPos = D3DXVECTOR4(pThisSceneCamera->GetPos().x, pThisSceneCamera->GetPos().y, pThisSceneCamera->GetPos().z, 1.0f);
		
	//	ViewPos.w = 1.0f;
		pEff->SetVector(G_ConstantTable.h_ViewPos , (D3DXVECTOR4*)(&ViewPos));



		
	}
}


//-----------------------------------------------------------------------------
// Arg    : int _tag , enum eCollide_Tag �� �ϳ�
// Return :
// Desc   : �ݸ��� �ױ׸� ���ڷ� �浹üũ���ִ� �Լ� [2014.11.25 ]
//-----------------------------------------------------------------------------
bool	CBear::ColliChk_Using_Tag(int _tag)
{
	bool Res = false;
	// ����ó�� // [2014.11.25 ]
	if((_tag >= eCollide_Tag_Max) || (_tag < 0) )  // �ر� �ε��� �˻�
	{ SHUtil::MsgBox("�߸��� �ױ� �ε���");	return false;}

	std::list<ICollider*>::iterator iterCur , iterEnd;  // ���ͷ�����  [2014.11.25 ]
	iterEnd = CCollideMgr::getInstance()->m_Collider_List[_tag].end();// ���ͷ�����
	 
	for(iterCur = CCollideMgr::getInstance()->m_Collider_List[_tag].begin(); iterCur != iterEnd ; ++iterCur )
	{

		SH_CollideInfo TempInfo;

		Res = (*iterCur)->Colli_Check(m_pBoundSphere_Body, &TempInfo);

		if(TempInfo.X == true)
		{m_Vec_Modified_See = 	m_VecSee; m_Vec_Modified_See.x = 0.0f;  }
		if(TempInfo.Z == true)
		{m_Vec_Modified_See = 	m_VecSee; m_Vec_Modified_See.z = 0.0f;	}

		if(Res == true)
			return true;
	}
	


	return Res;
}



//-----------------------------------------------------------------------------
// Desc   :  ��� ���Ͽ��� �׵𺣾� �Ӽ������� �ҷ��´�. // [2014.11.14 ]
//-----------------------------------------------------------------------------
int			CBear::Load_AttrInfo_from_Lua(const CHAR* _pPath)
{


	SH_Lua TempLua(_pPath);
	//int tableSize; 
	// [2014.10.20 ]
	try
	{
		// ��� �������� Tedy_MoveSpeed �� �޾ƿ´�. 
		TempLua.GetGlobal_float("Tedy_MoveSpeed" ,m_BearData.MoveSpeed);
		// ��� �������� Tedy_RotSpeed �� �޾ƿ´�. 
		TempLua.GetGlobal_float("Tedy_RotSpeed" , m_BearData.RotSpeed);
	
		/*TempLua.GetGlobal_int("Clip_Info_Size" , tableSize);

		lua_getglobal(TempLua.GetLuaState(), "Clip_Info");

		for(int i = 1 ; i<= tableSize ; i++)
		{
			char clipName[Max_Name_Len];						// Ŭ���̸�
			int STime, ETime;									// Ŭ�� ���۽ð�, ���ð�

			lua_rawgeti(TempLua.GetLuaState(), 1, i);

			TempLua.Getfield_String("Name", clipName );
			TempLua.Getfield_int("Start", STime);
			TempLua.Getfield_int("End", ETime);

			CLIP_INFO tempClip(STime, ETime);
			m_stdMap_AniState.insert(std::pair<CString, CLIP_INFO>(clipName, tempClip));
		}
		*/
	}
	catch(...)
	{
		SHUtil::MsgBox("CAnimator::Load_ClipInfo_from_Lua() , ��ƿ���ó��!!");
		return S_FALSE;
	}

	return S_OK;
}



//-----------------------------------------------------------------------------
// Ű���峪 ���콺 �Է� ó��
//-----------------------------------------------------------------------------
bool CBear::MsgProc( SH_MSG* _pMessage )
{
	static int toggle = true;

	//////////////////////////////////////////////////////////////////////////
	//�޼��� Ÿ���� UI ��� 
	if(_pMessage->GetMsgType() == eMsgType_UI)  
	{
		SH_MSG_from_UI* pUIMsg = static_cast<SH_MSG_from_UI*>(_pMessage); // ��������ȯ [2014.11.7 ]
		switch( pUIMsg->m_UI_id) // �������� �������̵�� � �����̿��� �°��� �˻� [2014.11.7 ]
		{
		case eUI_ID_StartButton:// �޼�����  ��۹�ư���� �Դٸ�
			{
				switch(pUIMsg->m_WinMsg)
				{
				case WM_LBUTTONUP:  //��۹�ư���� �� �޼����� WM_LBUTTONUP �̶��
					{
						toggle ^=1;
						if(toggle == 1)
						{	// ������̺� ����� ��ũ�� �ڵ�� ���� ��ũ���� �ٲ��ش�.
							//m_hTech_Skin = G_ConstantTable.h_Tech_Skin_SpecularMap_DirLight;
						}
						else
						{	// ������̺� ����� ��ũ�� �ڵ�� ���� ��ũ���� �ٲ��ش�.
							//m_hTech_Skin = G_ConstantTable.h_Tech_Skin_DirLight;  
						}
					}
					break;
				}				
			}
			break;
		default:
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//�޼��� Ÿ���� �Ϲݸ޼������
	if(_pMessage->GetMsgType() == eMsgType_NORMAL)  //  
	{





	}



	return true;
}