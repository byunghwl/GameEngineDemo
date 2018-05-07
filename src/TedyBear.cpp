
#include "DXUT.H"
#include "ASEMesh.h"
#include "TedyBear.h"

//////////////////////////////////////////////////////////////////////////
//외부 전역변수 추가



#define ROT_VELO 0.02f //회전 각속도

CBear*		 G_pTedyBear = nullptr;
// 곰돌이가 생성되었을때 맨처음 바라보고 있는 방향 
D3DXVECTOR3  G_Vec_Start = D3DXVECTOR3(0.0f, 0.0f, -1.0f); 
// 8방향벡터 미리 만들어 놓기 
D3DXVECTOR3  G_Vec_Right = D3DXVECTOR3(1.0f, 0.0f, 0.0f); // 오른쪽
D3DXVECTOR3  G_Vec_Left = D3DXVECTOR3(-1.0f, 0.0f, 0.0f); // 왼쪽
D3DXVECTOR3  G_Vec_Front = D3DXVECTOR3(0.0f, 0.0f, 1.0f); // 앞쪽
D3DXVECTOR3  G_Vec_Back = D3DXVECTOR3(0.0f, 0.0f, -1.0f); // 뒷쪽
D3DXVECTOR3  G_Vec_FR = D3DXVECTOR3(0.707f, 0.0f, 0.707f); // 앞쪽+오른쪽
D3DXVECTOR3  G_Vec_FL = D3DXVECTOR3(-0.707f, 0.0f, 0.707f); // 앞쪽+왼쪽
D3DXVECTOR3  G_Vec_BR = D3DXVECTOR3(0.707f, 0.0f, -0.707f); // 뒷쪽+오른쪽
D3DXVECTOR3  G_Vec_BL = D3DXVECTOR3(-0.707f, 0.0f, -0.707f); // 뒷+왼쪽


//////////////////////////////////////////////////////////////////////////

CString		CBear::m_EntityType = "Monster"; // 엔티티별 타입 이름
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------
// struct BEAR_DATA 의 생성자   [2014.7.29 ]
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
//여기서부터 CBear 함수정의
// 생성자
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
	// 현재 곰돌이가 바라보고있는방향
	m_VecSee = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	m_Vec_DestVector = m_VecSee; 
	m_Angle_Yaw = 0.0f;  // up vector를 기준으로 회전한 각도

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
	write_log_file("메모리 해제 : ~CBear() 호출");

	SAFE_DELETE_(m_pEffMagicStick )

}



//-------------------------------------------------------------------------
// Arg	  : D3DXVECTOR3 _SPos	 : 시작위치 (기본값 D3DXVECTOR3(0,0,200))
//			BEAR_DATA* pMData : 몬스터 속성 데이터를 가지고있는 구조체
// Return : 성공(S_OK), 실패(S_FALSE)
// Desc   : CBear 클래스는 반드시 이함수로 초기화시켜 줘야한다. [2014.10.7 ]
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
// 여기서 메쉬 생성해주기!!!!!!!!!!!!
//-------------------------------------------------------------------------
int CBear::InitGameObject()
{


	return S_OK;

}

//-------------------------------------------------------------------------
// Arg	  : 
// Return : HRESULT S_OK , S_FALSE
// Desc   : 이곳에서 콜라이더를 생성해야함(바운드스피어로)//[2014.11.11 ]
//-------------------------------------------------------------------------
// from ICollidable
HRESULT		CBear::InitCollider() 
{
	HRESULT hr = S_OK;

	m_pCollidObj = new CBoundSphere();

	return hr;
}

//-------------------------------------------------------------------------
// 그리기 함수
//-------------------------------------------------------------------------

void CBear::Draw()
{	

	//셰이더상수 설정
	_OnDrawStart();
	
	m_RscPack.Draw(m_pDevice);

	m_SkillCard.Draw();
	m_SkillRockPunch.Draw(); 
	m_SkillHat.Draw(); 

	//m_EffMagicStick.Draw(); 


}
//-----------------------------------------------------------------------------
// Desc   : 업데이트 하기 직전 호출되는 함수 ,,, 이곳에서 입력 처리
//-----------------------------------------------------------------------------
void CBear::_OnUpdateStart(float _dTime)
{

	////////////////////////////////////////////////////////////////////////// //[2014.11.24 ]
	// 카메라 회전에따라 기준방향이 달라져야 한다. 
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
	// 일반타격1 [2014.11.25 ]
	if(SHG_Input.MouseDown(0) ||( m_RscPack.GetAnimator().GetAniState_By_Name("ATTACK") == eAniState_Play) )// 왼쪽버튼 다운
	{
		if( ((m_SkillbitFlag & ~BitFlag_LClick) & BitFlag_Skill ) <= 0) // 다른스킬이 사용중이 아니라면!! 
		{	
			if(CAnimator::RVT_OK == m_RscPack.GetAnimator().Change_Ani_Clip("ATTACK")) // 애니클립 바꾸기 성공했다면   
			{	
				m_NormalHit_Timer.SetTimer(0.3f);  G_SHSoundMgr.PlayWave(eSound_Tedy_Atk, false);
			} 
			
		
			m_SkillbitFlag |= BitFlag_LClick;
		}
	}
	else
	{	m_SkillbitFlag &= ~BitFlag_LClick; 	}// 비틀플레그 해제
	

	//////////////////////////////////////////////////////////////////////////
	// 스킬공격1  카드날리기!!!!!!!!!!!!!!! 
	
	if(  ( SHG_Input.key(DIK_1) && (m_SkillPoint > S_POINT*3.99999f) ) || ( m_RscPack.GetAnimator().GetAniState_By_Name("SKILL1") == eAniState_Play)  )// 왼쪽버튼 다운
	{
		if(((m_SkillbitFlag & ~BitFlag_1) & BitFlag_Skill ) <= 0 ) // 다른스킬이 사용중이 아니라면!! [2014.11.27 ]
		{
			if((CAnimator::RVT_OK == m_RscPack.GetAnimator().Change_Ani_Clip("SKILL1") ) && (m_SkillCard.isReady()== true)) // 
			{	
				// 스킬지연시작함수 ,,, 스킬을 정해진 시간만큼 지연시킨후 발동한다.!! 			
				m_SkillCard.SetDelayTime_for_Begin(m_PosStick,m_VecSee, m_MWorld , m_RscPack.GetAnimator().GetCurClip_EffectDelayTime() ); 
				m_SkillPoint-= S_POINT*4.0f; 
				
			} 

			m_SkillbitFlag |= BitFlag_1	; // 
		}
	}
	else
	{	m_SkillbitFlag &= ~BitFlag_1; 	}//비트플래그 해제


	//////////////////////////////////////////////////////////////////////////
	// 스킬공격2 로켓펀치 !!!!!!!!!!!!!!  [2014.11.26 ]
	if((SHG_Input.key(DIK_2)&& (m_SkillPoint > S_POINT*7.99999f) )||( m_RscPack.GetAnimator().GetAniState_By_Name("SKILL2") == eAniState_Play) )//숫자2키 누름
	{
		if(((m_SkillbitFlag & ~BitFlag_2) & BitFlag_Skill ) <= 0 ) // 다른스킬이 사용중이 아니라면!! [2014.11.27 ]
		{
			if((CAnimator::RVT_OK == m_RscPack.GetAnimator().Change_Ani_Clip("SKILL2")) && (m_SkillRockPunch.isReady()== true)) // 애니클립 바꾸기 성공했다면   [2014.11.25 ]
			{
				// 스킬지연시작함수 ,,, 스킬을 정해진 시간만큼 지연시킨후 발동한다.!!  //[2014.11.29 ]
				m_SkillRockPunch.SetDelayTime_for_Begin(m_PosStick,m_VecSee, m_MWorld , m_RscPack.GetAnimator().GetCurClip_EffectDelayTime() ,m_BoneMesh_Hat); 
						
				m_SkillPoint-= S_POINT*8.0f;  // [2014.12.2 ]
			} 

			m_SkillbitFlag |= BitFlag_2	; // [2014.11.27 ]
		}
	}
	else
	{	m_SkillbitFlag &= ~BitFlag_2; 	}//비트플래그 해제


	//////////////////////////////////////////////////////////////////////////
	// 스킬공격3 모자떨구기 !!!!!!!!!!!!!!!!! 
	if((SHG_Input.key(DIK_3) && (m_SkillPoint > S_POINT*11.99999f)) || ( m_RscPack.GetAnimator().GetAniState_By_Name("SKILL3") == eAniState_Play) )// 숫자3 누름
	{
		if(((m_SkillbitFlag & ~BitFlag_3) & BitFlag_Skill ) <= 0) // 다른스킬이 사용중이 아니라면!! 
		{
			if((CAnimator::RVT_OK == m_RscPack.GetAnimator().Change_Ani_Clip("SKILL3")) && (m_SkillHat.isReady()== true)) // 애니클립 바꾸기 성공했다면 
			{					
				// 스킬지연시작함수 ,,, 스킬을 정해진 시간만큼 지연시킨후 발동한다.!! 
				m_SkillHat.SetDelayTime_for_Begin(m_Pos  , m_VecSee, m_MWorld , m_RscPack.GetAnimator().GetCurClip_EffectDelayTime() ); 
				m_SkillPoint-= S_POINT*12.0f; 
			} 

			m_SkillbitFlag |= BitFlag_3	;
		}
	}
	else
	{	m_SkillbitFlag &= ~BitFlag_3; 	}//비트플래그 해제


	//////////////////////////////////////////////////////////////////////////
	// 이동중일때 [2014.11.25 ]
	if( (SHG_Input.key(DIK_W) || SHG_Input.key(DIK_D) || SHG_Input.key(DIK_A) || SHG_Input.key(DIK_S)) && (BitFlag_Using_AnySkill(m_SkillbitFlag)  == 0) )
	{	
		
			if(CAnimator::RVT_OK == m_RscPack.GetAnimator().Change_Ani_Clip("RUN")) // 애니클립 바꾸기 성공했다면 
			{	m_RscPack.GetAnimator().SetAniTimeFirst();	} // 클립을 첫프레임으로 맞춰주기

			
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

			// 비트플레그를 켜준다. [2014.11.13 ]
			m_SkillbitFlag |= BitFlag_Move;
		
		

	}	
	else // 이동키 안눌렀을때는 비트플래그 해제
	{ m_SkillbitFlag &= ~BitFlag_Move;	}


	//////////////////////////////////////////////////////////////////////////
	// 아무것도 안눌렀을때 IDLE [2014.11.25 ]
	if(m_SkillbitFlag == 0)
	{	
		// 아무것도 안눌렀을때는 숨쉬기
	    if(CAnimator::RVT_OK == m_RscPack.GetAnimator().Change_Ani_Clip("IDLE"))// 애니클립 바꾸기 성공했다면 
		{	m_RscPack.GetAnimator().SetAniTimeFirst();	} // 클립을 첫프레임으로 맞춰주기
	}


	





	
// 방향키 업 버튼 눌렀을때
// 	else if(SHG_Input.key(DIK_SPACE))
// 	{	
// 		if(CAnimator::RVT_OK == m_RscPack.GetAnimator().Change_Ani_Clip("Jump")) // 애니클립 바꾸기 성공했다면 
// 		{	m_RscPack.GetAnimator().SetAniTimeFirst();	} // 클립을 첫프레임으로 맞춰주기
// 	}
// 	else 
// 	{	
// 		// 아무것도 안눌렀을때는 숨쉬기
// 		if(CAnimator::RVT_OK == m_RscPack.GetAnimator().Change_Ani_Clip("IDLE"))// 애니클립 바꾸기 성공했다면 
// 		{	m_RscPack.GetAnimator().SetAniTimeFirst();	} // 클립을 첫프레임으로 맞춰주기
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
// 업데이트 함수
//-------------------------------------------------------------------------

void CBear::Update(float _dTime)
{

	_OnUpdateStart(_dTime);// 주인공이라면 여기서 키보드인풋 처리

	D3DXMatrixRotationY(&m_MRot , m_Angle_Yaw  );
	// 시선벡터도 누적된 각도 만큼 회전시켜준다. [2014.11.13 ]
	D3DXVec3TransformNormal(&m_VecSee , &G_Vec_Start, &m_MRot);

	//////////////////////////////////////////////////////////////////////////
	// 여기서부터 DestVector를 기준으로 회전행렬 계산하기

	// 두벡터사이의 각도 구하기
	D3DXVec3Normalize(&m_Vec_DestVector , &m_Vec_DestVector);
	D3DXVec3Normalize(&m_VecSee, &m_VecSee);
	float TempDot = D3DXVec3Dot(&m_Vec_DestVector , &m_VecSee);

	
	if(TempDot < -0.9f)//  회전하려는 각도가 180도 완전히 정반대일때는 바로 회전시킨다. //[2014.11.30 ]
	{
		m_Angle_Yaw+=D3DX_PI;
	}
	else if (TempDot < 0.99f) // 겹쳐지기 전까지만 회전 [2014.11.13 ]
	{
		if(  ( BitFlag_isMoving(m_SkillbitFlag) > 0 )  && ( BitFlag_Using_AnySkill(m_SkillbitFlag) <= 0 )   )// 아무키나 눌려있다면 [2014.11.13 ] && 스킬을 안누르고 있다면
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
	else//TempDot > 0.99f 이어서 두벡터가 동일하다고 판단될때는 강제로 벡터를 일치시킨다.
	{
		m_VecSee = m_Vec_DestVector;
	}
	


	// 이동키를 누르고 있을 때만 시선방향으로 이동한다. [2014.11.13 ]
	// 이동키를 누르고 있을때만 룸의 오브젝트와 충돌체크 //[2014.11.25 ]
	if(BitFlag_isMoving(m_SkillbitFlag))
	{
		// [2014.11.25 ] 충돌체크
		m_PrePos += m_VecSee *  m_BearData.MoveSpeed * _dTime;

		if((m_PrePos.x >-229.0f) && (m_PrePos.x < 229.0f) && (m_PrePos.z > -175.0f) && (m_PrePos.z < 175.0f) )
		{
		}
		else
		{ m_PrePos = m_Pos;
		}
			
			m_pBoundSphere_Body->SetCenter(m_PrePos + D3DXVECTOR3(0.0f, 8.0f, 0.0f));// 충돌구_바디 위치조정  [2014.11.25 ]

			if(ColliChk_Using_Tag(eCollide_Tag_BoxesInRoom) == true)
			{			
				m_PrePos = m_Pos;
				m_Pos += m_Vec_Modified_See *  m_BearData.MoveSpeed * _dTime;
			}
			else // 충돌체크 안됬다면?  [2014.11.25 ]
			{
				//m_Pos += m_VecSee * m_BearData.MoveSpeed * _dTime;
				m_Pos = m_PrePos; /*Pos를 업데이트 함*/ // [2014.11.25 ]
			}
		

		
		
	
		
	}
	

	//////////////////////////////////////////////////////////////////////////
	// 여기서부터 월드행렬 계산하기 
	D3DXMatrixIdentity(&m_MTrans);
	D3DXMatrixTranslation(&m_MTrans, m_Pos.x , m_Pos.y , m_Pos.z);

	m_MWorld = m_MRot * m_MTrans;//최종행렬 계산
		
	m_RscPack.Update(_dTime, &m_MWorld);

	// 충돌구_스틱 위치조정  [2014.11.25 ]
	D3DXMATRIX BSTM = m_BSMesh_Stick->GetWorldTM();
	m_PosStick = D3DXVECTOR3(BSTM._41,BSTM._42, BSTM._43);
	m_pBoundSphere_Stick->SetCenter(m_PosStick);

	// 

	if(BitFlag_isLBtnClick(m_SkillbitFlag))
	{
		//////////////////////////////////////////////////////////////////////////
		//	고스트들과 충돌체크 !!!!!!!!!!!!!!!!!!!!!!    [2014.11.26 ]
		if(m_NormalHit_Timer.isTimeUp() == true)
		{ m_NormalHit_Timer.SetOff();// 타이머 꺼줘야 한번만 이 루프안으로 들어온다. !! [2014.12.7 ]
			for(int i = 0 ; i< GHOSTNUM_MAX ; i++)
			{
				if(G_pGhost_Arr[i]->m_bAlive == true)
				{
					if(G_pGhost_Arr[i]->m_pBoundSphere_Body->Colli_Check(m_pBoundSphere_Stick) == true)
					{
						if(true == G_pGhost_Arr[i]->DamagedFromHit(1))//HP 가 1만큼 달게하기 
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
	// 여기서 부터 가지고있는 스킬들 업데이트 
	//m_SkillCard.SetPosition(PosStick);
	m_SkillCard.Update(_dTime);
	m_SkillRockPunch.Update(_dTime); 
	m_SkillHat.Update(_dTime); 
	
	m_pEffMagicStick->Update(_dTime, m_PosStick); 
// 
	// 여기서부터 스킬 포인트에따라 스킬 UI 비활성화 
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
void CBear::Rot_Pitch(float _Angle)// 우향벡터를 기준으로 회전
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

void CBear::Rot_Roll(float _Angle)//전방벡터 기준으로 회전
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
void CBear::Rot_Yaw(float _Angle)//상향벡터기준으로 회전
{
	m_Angle_Yaw += _Angle; //카메라 회전을 위해 얼만큼 회전했는지 각도를 누적해준다. 

	D3DXMatrixIdentity(&m_MRotYaw);
	D3DXMatrixRotationAxis(&m_MRotYaw, &g_AXIS_Y, _Angle);
	m_MRot = m_MRotYaw * m_MRot;
	D3DXVec3TransformNormal(&m_VecRight ,&g_AXIS_X ,&m_MRot);// 우측방향 벡터를 회전시켜준다.
	D3DXVec3TransformNormal(&m_VecSee ,&g_AXIS_Z ,&m_MRot);// 시선벡터를 회전시켜 준다.
}


//-----------------------------------------------------------------------------
// Desc   : draw 하기 직전 호출되는 함수
//-----------------------------------------------------------------------------
void	CBear::_OnDrawStart()
{
	// 셰이더 상수 설정
	{

		// 어떤테크닉을 쓸꺼냐 시작할때 반드시 결정 해서 핸들을 저장해 놓는다.[2014.10.29 ]
// 		if(SHG_Input.key(DIK_P))
// 			m_hTech_Skin = G_FXShader.GetPointer()->GetTechniqueByName("Skinning_DirLight_SpecularMap");
// 		else
// 			m_hTech_Skin = G_FXShader.GetPointer()->GetTechniqueByName("Skinning_DirLight");
		// 1. 이펙트 객체 가져오기 [2014.10.29 ]
		LPD3DXEFFECT pEff = G_FXShader.GetPointer();                             

		CCamera* pThisSceneCamera = SHGlobal_GetCurScene_Camera;
		D3DXMATRIX tempViewProjTM; //
		tempViewProjTM = pThisSceneCamera->Get_ViewProjMat();
		// viewPorj TM 을 핸들로 셰이더에 전달 [2014.10.27 ]
		pEff->SetMatrix(G_ConstantTable.h_ViewProjTM , &tempViewProjTM);

		//외각선 칼라  [2014.11.13 ]
		D3DXCOLOR OutLineColor = D3DXCOLOR(0.1f,0.1f,0.1f,1);
		pEff->SetVector(G_ConstantTable.h_LineColor , (D3DXVECTOR4*)&OutLineColor);
		 //외각선 두깨 [2014.11.13 ]
		pEff->SetFloat(G_ConstantTable.h_LineOffset , 0.2f);			
		//포인트라이트 위치  [2014.11.13 ]
		D3DXVECTOR4 TempPos = D3DXVECTOR4(m_Pos.x,  m_Pos.y + 100.0f ,m_Pos.z, 1.0f);
		pEff->SetVector(G_ConstantTable.h_PointLightPos , (D3DXVECTOR4*)&TempPos);

		pEff->SetFloat(G_ConstantTable.h_PointLightRadius , G_PointLightRadius);

		pEff->SetVector(G_ConstantTable.h_LightDiff , (D3DXVECTOR4*)&G_LightDiff);

		D3DXVECTOR4 TempLight = (D3DXVECTOR4)(G_vecDirLight * -1.0f);
		TempLight.w  = 0.0f;
		pEff->SetVector(G_ConstantTable.h_LightDir , (D3DXVECTOR4*)&TempLight);

		pEff->SetVector(G_ConstantTable.h_LightAmbi , (D3DXVECTOR4*)&G_LightAmbi);
// 		CCamera TempCam = *pThisSceneCamera; ★★★ 나중에 꼭 원인 확인하기 ★★★
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
// Arg    : int _tag , enum eCollide_Tag 중 하나
// Return :
// Desc   : 콜리드 테그를 인자로 충돌체크해주는 함수 [2014.11.25 ]
//-----------------------------------------------------------------------------
bool	CBear::ColliChk_Using_Tag(int _tag)
{
	bool Res = false;
	// 에러처리 // [2014.11.25 ]
	if((_tag >= eCollide_Tag_Max) || (_tag < 0) )  // 텍그 인덱스 검사
	{ SHUtil::MsgBox("잘못된 테그 인덱스");	return false;}

	std::list<ICollider*>::iterator iterCur , iterEnd;  // 이터레이터  [2014.11.25 ]
	iterEnd = CCollideMgr::getInstance()->m_Collider_List[_tag].end();// 이터레이터
	 
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
// Desc   :  루아 파일에서 테디베어 속성정보를 불러온다. // [2014.11.14 ]
//-----------------------------------------------------------------------------
int			CBear::Load_AttrInfo_from_Lua(const CHAR* _pPath)
{


	SH_Lua TempLua(_pPath);
	//int tableSize; 
	// [2014.10.20 ]
	try
	{
		// 루아 전역변수 Tedy_MoveSpeed 을 받아온다. 
		TempLua.GetGlobal_float("Tedy_MoveSpeed" ,m_BearData.MoveSpeed);
		// 루아 전역변수 Tedy_RotSpeed 을 받아온다. 
		TempLua.GetGlobal_float("Tedy_RotSpeed" , m_BearData.RotSpeed);
	
		/*TempLua.GetGlobal_int("Clip_Info_Size" , tableSize);

		lua_getglobal(TempLua.GetLuaState(), "Clip_Info");

		for(int i = 1 ; i<= tableSize ; i++)
		{
			char clipName[Max_Name_Len];						// 클립이름
			int STime, ETime;									// 클립 시작시간, 끝시간

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
		SHUtil::MsgBox("CAnimator::Load_ClipInfo_from_Lua() , 루아예외처리!!");
		return S_FALSE;
	}

	return S_OK;
}



//-----------------------------------------------------------------------------
// 키보드나 마우스 입력 처리
//-----------------------------------------------------------------------------
bool CBear::MsgProc( SH_MSG* _pMessage )
{
	static int toggle = true;

	//////////////////////////////////////////////////////////////////////////
	//메세지 타입이 UI 라면 
	if(_pMessage->GetMsgType() == eMsgType_UI)  
	{
		SH_MSG_from_UI* pUIMsg = static_cast<SH_MSG_from_UI*>(_pMessage); // 강제형변환 [2014.11.7 ]
		switch( pUIMsg->m_UI_id) // 유아이의 고유아이디로 어떤 유아이에서 온건지 검사 [2014.11.7 ]
		{
		case eUI_ID_StartButton:// 메세지가  토글버튼에서 왔다면
			{
				switch(pUIMsg->m_WinMsg)
				{
				case WM_LBUTTONUP:  //토글버튼에서 온 메세지가 WM_LBUTTONUP 이라면
					{
						toggle ^=1;
						if(toggle == 1)
						{	// 상수테이블에 저장된 테크닉 핸들로 현재 테크닉을 바꿔준다.
							//m_hTech_Skin = G_ConstantTable.h_Tech_Skin_SpecularMap_DirLight;
						}
						else
						{	// 상수테이블에 저장된 테크닉 핸들로 현재 테크닉을 바꿔준다.
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
	//메세지 타입이 일반메세지라면
	if(_pMessage->GetMsgType() == eMsgType_NORMAL)  //  
	{





	}



	return true;
}