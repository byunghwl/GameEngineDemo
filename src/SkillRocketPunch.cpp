
#include "DXUT.H"
#include "GameObject.h"
#include "SkillRocketPunch.h"

///////////////////////////////////////////////////////////////////////////
// 전역변수 선언
CRockPunchSkill*				G_pRockPunch = nullptr; // [2014.11.27 ]

//////////////////////////////////////////////////////////////////////////
// 스테틱 변수 초기화
CString		CRockPunchSkill::m_EntityType = "RockPunchSkill"; // 엔티티별 타입 이름




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
//여기서부터 CRockPunchSkill 함수정의
// 생성자
CRockPunchSkill::CRockPunchSkill() //:  m_pMesh(nullptr)
{
	m_pDevice = SHGlobal_GetD3DDevice;
	//m_hTech = G_FXShader.GetPointer()->GetTechniqueByName("Skinning_DirLight_SpecularMap");
	//D3DXHANDLE TempHandle = G_FXShader.GetPointer()->GetTechniqueByName("Skinning_PointLight");

	//m_RscPack.m_pSkinMesh->Set_m_hTech("Skinning_PointLight"); //[2014.11.21 ]
	//TempHandle = G_FXShader.GetPointer()->GetTechniqueByName("Rigid_Tex"); //[2014.11.21 ]
	//m_RscPack.m_MeshList.Set_hTech_AllMesh("Rigid_Tex"); //[2014.11.21 ]


	//m_hTech = G_FXShader.GetPointer()->GetTechniqueByName("Outline_PNT");
	D3DXMatrixIdentity(&m_MTrans);
	D3DXMatrixIdentity(&m_MRot);

	D3DXMatrixIdentity(&m_MRotPitch);
	D3DXMatrixIdentity(&m_MRotRoll);
	D3DXMatrixIdentity(&m_MRotYaw);

	// 현재 카드가이 바라보고있는방향  // [2014.11.27 ]
	m_VecSee = D3DXVECTOR3(0.0f, 0.0f, -1.0f); // [2014.11.27 ]
	m_Vec_DestVector = m_VecSee; /// [2014.11.27 ]
	m_Angle_Yaw = 0.0f;  // up vector를 기준으로 회전한 각도 // [2014.11.27 ]

	m_RockPunchData.MoveSpeed = 300.0f;  	// [2014.11.27 ]
	
	m_Vec_StartSee = D3DXVECTOR3(0,0, -1);

	m_pBoundSphere_Body = nullptr;

	m_bAlive = false;// [2014.11.27 ]

	m_DelayTime = 0.0f ;// [2014.11.27 ]

	m_b_is_TimerOn = false; // [2014.11.27 ]
	m_AccuTime = 0.0f; //누적시간  // [2014.11.28 ]
	m_CoolTime = 0.0f;// [2014.11.28 ]
}

CRockPunchSkill::~CRockPunchSkill()
{
	write_log_file("메모리 해제 : ~CRockPunchSkill() 호출");


}



//-------------------------------------------------------------------------
// Arg	  : D3DXVECTOR3 _SPos	 : 시작위치 (기본값 D3DXVECTOR3(0,0,200))
//			ROCKPUNCH_DATA* pMData : 몬스터 속성 데이터를 가지고있는 구조체
// Return : 성공(S_OK), 실패(S_FALSE)
// Desc   : CRockPunchSkill 클래스는 반드시 이함수로 초기화시켜 줘야한다. [2014.10.7 ]
//-------------------------------------------------------------------------

int  CRockPunchSkill::Init(LPDIRECT3DDEVICE9 _pDevice, D3DXVECTOR3 _SPos /*= D3DXVECTOR3(0,0,200)*/, ROCKPUNCH_DATA* pBData )
{
	HRESULT RET = S_OK;
	if(FAILED(CEntity::Init(_pDevice)))		
	{
		return FALSE;
	}

	if(pBData != nullptr)
	{
		memcpy(&m_RockPunchData, pBData, sizeof(ROCKPUNCH_DATA));// [2014.10.7 ]
	}

	m_VecRight = D3DXVECTOR3(1,0,0);
	m_VecSee   = D3DXVECTOR3(0,0,-1); 
	m_VecUp    = D3DXVECTOR3(0,1,0);

	m_Pos = _SPos;
	
	m_RockPunchDataBk = m_RockPunchData;

	return RET;

}

bool	CRockPunchSkill::isReady()
{
	if(m_CoolTime > m_RockPunchData.CoolTime)
		return true;
	else
		return false;
}//[2014.11.28 ]


//-------------------------------------------------------------------------
// Arg	  : 
// Return : HRESULT S_OK , S_FALSE
// Desc   : 이곳에서 콜라이더를 생성해야함(바운드스피어로)//[2014.11.11 ]
//-------------------------------------------------------------------------
// from ICollidable
HRESULT		CRockPunchSkill::InitCollider() 
{
	HRESULT hr = S_OK;

	m_pCollidObj = new CBoundSphere();

	return hr;
}

//-------------------------------------------------------------------------
// 그리기 함수
//-------------------------------------------------------------------------

void CRockPunchSkill::Draw()
{	
	if(m_bAlive == false) //[2014.11.17 ]
		return;

	//셰이더상수 설정
	_OnDrawStart();

	m_RscPack.Draw(m_pDevice);
//	int size =m_RscPack.m_MeshList.m_Vec_MeshList.size();
	/*LOG_WRITE_A(" 고스트 메쉬리스트 개수 !!!!!!!!! %d",  size);*/


}


//-----------------------------------------------------------------------------
// Desc   : 업데이트 하기 직전 호출되는 함수
//-----------------------------------------------------------------------------
void CRockPunchSkill::_OnUpdateStart(float _dTime)
{
	



	

}

//-------------------------------------------------------------------------
// 업데이트 함수
//-------------------------------------------------------------------------

void CRockPunchSkill::Update(float _dTime)
{
// 	m_AccuTime = 0.0f; //누적시간 

	if(m_CoolTime < m_RockPunchData.CoolTime)
	m_CoolTime += _dTime;

	if(m_b_is_TimerOn == true)
	{
		m_AccuTime += SHGlobal_GetEngineTime * 1000.0f ; //[2014.11.27 ]
	}

	if(m_DelayTime < m_AccuTime) // 누적시간이 딜레이시간 이상이 되면..
	{
		m_b_is_TimerOn = false; // 다시 타이머를 [2014.11.27 ]
		m_bAlive = true;
		m_AccuTime = 0.0f;
		D3DXMATRIX TempMat = m_BoneMesh_Hat->GetWorldTM();
		m_Pos = D3DXVECTOR3(TempMat._41, TempMat._42, TempMat._43); //[2014.11.29 ]
		m_RscPack.GetAnimator().SetAniTimeFirst(); // [2014.11.28 ]
		m_RscPack.GetAnimator().Set_m_BlendStart(false);
		m_RscPack.GetAnimator().GetCurClipInfo()->eAniState = eAniState_Play; //애니메이션의 처음프레임으로 맞춰준다. [2014.11.28 ]
	}

	if(m_bAlive == false) //[2014.11.17 ]
		return;

	//////////////////////////////////////////////////////////////////////////
	
	D3DXVec3Normalize(&m_VecSee , &m_VecSee);

	// 이동량 계산
	m_Pos += m_VecSee * _dTime * m_RockPunchData.MoveSpeed; // [2014.11.27 ]

	// 회전행렬 계산
	m_MRot = m_CharacterWorldTM;
	m_MRot._41 = 0.0f; m_MRot._42 = 0.0f; m_MRot._43 = 0.0f; m_MRot._41 = 1.0f;

	D3DXMatrixIdentity(&m_MTrans);
	D3DXMatrixTranslation(&m_MTrans, m_Pos.x , m_Pos.y , m_Pos.z);

	m_MWorld = m_MRot * m_MTrans;//최종행렬 계산

	m_RscPack.Update(_dTime, &m_MWorld);
	m_pBoundSphere_Body->SetCenter(m_Pos);
	//////////////////////////////////////////////////////////////////////////
	//	고스트들과 충돌체크 !!!!!!!!!!!!!!!!!!!!!!    [2014.11.26 ]
	for(int i = 0 ; i< GHOSTNUM_MAX ; i++)
	{
		if(G_pGhost_Arr[i]->m_bAlive == true)
		{
			if(G_pGhost_Arr[i]->m_pBoundSphere_Body->Colli_Check(m_pBoundSphere_Body) == true)
			{
				G_pGhost_Arr[i]->DamagedFromHit(2);//HP 가 1만큼 달게하기
			//	m_bAlive = false;
			}
		}
	}


	if(m_RscPack.GetAnimator().GetCurClipInfo()->eAniState == eAniState_Stop)
	{
		m_bAlive = false;
		m_RscPack.GetAnimator().SetAniTimeFirst(); // [2014.11.28 ]
	}

	//////////////////////////////////////////////////////////////////////////
	// 사정거리 다되면 사라지게 하기 //[2014.11.27 ]
// 	{
// 		D3DXVECTOR3 TempVec = m_Pos - m_StartPos;
// 		float Dist = D3DXVec3Length(&TempVec);
// 		if(Dist > m_RockPunchData.Range)
// 		{
// 			m_bAlive = false;
// 		}
// 	}
	
}




//-----------------------------------------------------------------------------
// Desc   : draw 하기 직전 호출되는 함수
//-----------------------------------------------------------------------------
void	CRockPunchSkill::_OnDrawStart()
{
	// 셰이더 상수 설정
	{

		// 어떤테크닉을 쓸꺼냐 시작할때 반드시 결정 해서 핸들을 저장해 놓는다.[2014.10.29 ]
// 		if(SHG_Input.key(DIK_P))
// 			m_hTech = G_FXShader.GetPointer()->GetTechniqueByName("Skinning_DirLight_SpecularMap");
// 		else
// 			m_hTech = G_FXShader.GetPointer()->GetTechniqueByName("Skinning_DirLight");
		// 1. 이펙트 객체 가져오기 [2014.10.29 ]
	//	LPD3DXEFFECT pEff = G_FXShader.GetPointer();                             

// 		CCamera* pThisSceneCamera = SHGlobal_GetCurScene_Camera;
// 		D3DXMATRIX tempViewProjTM; //
// 		tempViewProjTM = pThisSceneCamera->Get_ViewProjMat();
// 		// viewPorj TM 을 핸들로 셰이더에 전달 [2014.10.27 ]
// 		pEff->SetMatrix(G_ConstantTable.h_ViewProjTM , &tempViewProjTM);

		//외각선 칼라  [2014.11.13 ]
// 		pEff->SetVector(G_ConstantTable.h_LineColor , (D3DXVECTOR4*)&G_LightDiff);
// 		 //외각선 두깨 [2014.11.13 ]
// 		pEff->SetFloat(G_ConstantTable.h_LineOffset , 0.2f);		

		//포인트라이트 위치   이건 곰돌이쪽에서  //[2014.11.15 ]
// 		D3DXVECTOR4 TempPos = D3DXVECTOR4(m_Pos.x,  m_Pos.y + 100.0f ,m_Pos.z, 1.0f);
// 		pEff->SetVector(G_ConstantTable.h_PointLightPos , (D3DXVECTOR4*)&TempPos);

		//pEff->SetVector(G_ConstantTable.h_LightDiff , (D3DXVECTOR4*)&G_LightDiff);

// 		D3DXVECTOR4 TempLight = (D3DXVECTOR4)(G_vecDirLight * -1.0f);
// 		TempLight.w  = 0.0f;
// 		pEff->SetVector(G_ConstantTable.h_LightDir , (D3DXVECTOR4*)&TempLight);
// 
// 		pEff->SetVector(G_ConstantTable.h_LightAmbi , (D3DXVECTOR4*)&G_LightAmbi);

// 		D3DXVECTOR4 ViewPos = pThisSceneCamera->GetPos();
// 		ViewPos.w = 1.0f;
// 		pEff->SetVector(G_ConstantTable.h_ViewPos , (D3DXVECTOR4*)(&ViewPos));


// 		if(m_hTech == nullptr )  // 테크 핸들 확인해보기 [2014.10.30 ]
// 		{		assert(false);	} // [2014.10.30 ]

	//	G_FXShader.GetPointer()->SetTechnique(m_hTech);// [2014.10.30 ]
	}
}


//-----------------------------------------------------------------------------
// Arg    : D3DXVECTOR3 _StartPos, D3DXMATRIX _CharacterWorldTM , int DelayFrame 
// Return :
// Desc   : 카드스킬 발동!! DelayFrame 만큼 기다렸다가 발동된다.
//			_StartPos 발동되었을때 시작위치 
//			_CharacterWorldTM 이스킬을 사용한 시점의 캐릭터 월드TM// [2014.11.27 ]
//-----------------------------------------------------------------------------
void	    CRockPunchSkill::SetDelayTime_for_Begin(D3DXVECTOR3 _StartPos, D3DXVECTOR3 _SeeVec,
													D3DXMATRIX _CharacterWorldTM , int DelayFrame, const SHR_Mesh* _pRefMesh /*= nullptr*/) // [2014.11.27 ]
{
	m_DelayTime = DelayFrame*30; //SHGlobal_GetEngineTime * 1000.0f * m_Velo; 
	m_Pos = _StartPos;
	m_StartPos = _StartPos;
	m_CharacterWorldTM = _CharacterWorldTM;
	m_BoneMesh_Hat = _pRefMesh;
	m_b_is_TimerOn = true; // [2014.11.27 ]
	m_VecSee =_SeeVec;// 위치 [2014.11.27 ]
	m_CoolTime = 0.0f;
}


//-----------------------------------------------------------------------------
// Desc   :  루아 파일에서 테디베어 속성정보를 불러온다. // [2014.11.14 ]
//-----------------------------------------------------------------------------
int			CRockPunchSkill::Load_AttrInfo_from_Lua(const CHAR* _pPath)
{


	SH_Lua TempLua(_pPath); //(SH_Lua)는 반드시 지역변수로 선언!!
	//int tableSize; 
	// [2014.10.20 ]
	try
	{
		// 루아 전역변수 Tedy_MoveSpeed 을 받아온다. // [2014.11.14 ]
		TempLua.GetGlobal_float("Rocket_Punch_MoveSpeed" ,m_RockPunchData.MoveSpeed);
		TempLua.GetGlobal_float("Rocket_Punch_Range" ,m_RockPunchData.Range);
		TempLua.GetGlobal_float("Rocket_Punch_Cooltime" ,m_RockPunchData.CoolTime); //[2014.11.28 ]
		m_CoolTime = m_RockPunchData.CoolTime+1;
		// 루아 전역변수 Tedy_RotSpeed 을 받아온다. // [2014.11.14 ]
		//TempLua.GetGlobal_float("Tedy_RotSpeed" , m_RockPunchData.RotSpeed);
	
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
bool CRockPunchSkill::MsgProc( SH_MSG* _pMessage )
{
	static int toggle = true;

	//////////////////////////////////////////////////////////////////////////
	//메세지 타입이 UI 라면 
	if(_pMessage->GetMsgType() == eMsgType_UI)  
	{
		SH_MSG_from_UI* pUIMsg = static_cast<SH_MSG_from_UI*>(_pMessage); // 강제형변환 [2014.11.7 ]
		switch( pUIMsg->m_UI_id) // 유아이의 고유아이디로 어떤 유아이에서 온건지 검사 [2014.11.7 ]
		{
		case eUI_ID_StartButton:// 메세지가  토글버튼에서 왔다면 [2014.11.7 ]
			{
				switch(pUIMsg->m_WinMsg)
				{
				case WM_LBUTTONUP:  //토글버튼에서 온 메세지가 WM_LBUTTONUP 이라면 [2014.11.7 ]
					{
						toggle ^=1;
						if(toggle == 1)
						{	// 상수테이블에 저장된 테크닉 핸들로 현재 테크닉을 바꿔준다.
							m_hTech = G_ConstantTable.h_Tech_Skin_SpecularMap_DirLight;// [2014.11.6 ]
						}
						else
						{	// 상수테이블에 저장된 테크닉 핸들로 현재 테크닉을 바꿔준다.
							m_hTech = G_ConstantTable.h_Tech_Skin_DirLight;  // [2014.11.6 ]
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
