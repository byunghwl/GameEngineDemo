
#include "DXUT.H"
#include "GameObject.h"
#include "SkillScatter.h"

///////////////////////////////////////////////////////////////////////////
// 전역변수 선언
CScatterSkill*				G_pScatter = nullptr; // [2014.11.27 ]

//////////////////////////////////////////////////////////////////////////
// 스테틱 변수 초기화
CString		CScatterSkill::m_EntityType = "Skill_Scatter"; // 엔티티별 타입 이름




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
//여기서부터 CScatterSkill 함수정의
// 생성자
CScatterSkill::CScatterSkill() //:  m_pMesh(nullptr)
{
	m_pDevice = SHGlobal_GetD3DDevice;

	D3DXMatrixIdentity(&m_MTrans);
	D3DXMatrixIdentity(&m_MRot);

	D3DXMatrixIdentity(&m_MRotPitch);
	D3DXMatrixIdentity(&m_MRotRoll);
	D3DXMatrixIdentity(&m_MRotYaw);

	// 현재 카드가이 바라보고있는방향  // [2014.11.27 ]
	m_VecSee = D3DXVECTOR3(0.0f, 0.0f, -1.0f); // [2014.11.27 ]
	m_Vec_DestVector = m_VecSee; /// [2014.11.27 ]
	m_Angle_Yaw = 0.0f;  // up vector를 기준으로 회전한 각도 // [2014.11.27 ]
		
	m_Vec_StartSee = D3DXVECTOR3(0,0, -1);

	m_bAlive = false;// [2014.11.27 ]

	m_DelayTime = 0.0f ;// [2014.11.27 ]

	m_b_is_TimerOn = false; // [2014.11.27 ]
	m_AccuTime = 0.0f; //누적시간  // [2014.11.28 ]
	m_CoolTime = 0.0f;// [2014.11.28 ]
	m_ScatterData.EachAngle = 0.785f;// [2014.12.1 ]
}

CScatterSkill::~CScatterSkill()
{
	write_log_file("메모리 해제 : ~CScatterSkill() 호출");


}



//-------------------------------------------------------------------------
// Arg	  : D3DXVECTOR3 _SPos	 : 시작위치 (기본값 D3DXVECTOR3(0,0,200))
//			SCATTER_DATA* pMData : 몬스터 속성 데이터를 가지고있는 구조체
// Return : 성공(S_OK), 실패(S_FALSE)
// Desc   : CScatterSkill 클래스는 반드시 이함수로 초기화시켜 줘야한다. [2014.10.7 ]
//-------------------------------------------------------------------------

int  CScatterSkill::Init(LPDIRECT3DDEVICE9 _pDevice, D3DXVECTOR3 _SPos /*= D3DXVECTOR3(0,0,200)*/, SCATTER_DATA* pScatData )
{
	HRESULT RET = S_OK;
	if(FAILED(CEntity::Init(_pDevice)))		
	{
		return FALSE;
	}

	if(pScatData != nullptr)
	{
		memcpy(&m_ScatterData, pScatData, sizeof(SCATTER_DATA));// [2014.10.7 ]
	}

	m_VecRight = D3DXVECTOR3(1,0,0);
	m_VecSee   = D3DXVECTOR3(0,0,-1); 
	m_VecUp    = D3DXVECTOR3(0,1,0);

	m_Pos = _SPos;
	
	m_ScatterDataBk = m_ScatterData;

	for(int i =0 ; i<MAXCARD_NUM ; i++) // [2014.12.1 ]
	{
		m_Card[i] = (*G_pCard); 
	}

	return RET;

}

bool	CScatterSkill::isReady()
{
	for(int i = 0 ; i<MAXCARD_NUM ; i++ )
	{
		if(m_Card[i].isReady() == false)
			return false;
	}

	return true;

}//[2014.11.28 ]


//-------------------------------------------------------------------------
// Arg	  : 
// Return : HRESULT S_OK , S_FALSE
// Desc   : 이곳에서 콜라이더를 생성해야함(바운드스피어로)//[2014.11.11 ]
//-------------------------------------------------------------------------
// from ICollidable
HRESULT		CScatterSkill::InitCollider() 
{
	HRESULT hr = S_OK;



	return hr;
}

//-------------------------------------------------------------------------
// 그리기 함수
//-------------------------------------------------------------------------

void CScatterSkill::Draw()
{	


	//셰이더상수 설정
	_OnDrawStart();
	for(int i= 0 ; i<MAXCARD_NUM ; i++)
	{
		m_Card[i].Draw();
	}
//	int size =m_RscPack.m_MeshList.m_Vec_MeshList.size();
	/*LOG_WRITE_A(" 고스트 메쉬리스트 개수 !!!!!!!!! %d",  size);*/


}


//-----------------------------------------------------------------------------
// Desc   : 업데이트 하기 직전 호출되는 함수
//-----------------------------------------------------------------------------
void CScatterSkill::_OnUpdateStart(float _dTime)
{
	



	

}

//-------------------------------------------------------------------------
// 업데이트 함수
//-------------------------------------------------------------------------

void CScatterSkill::Update(float _dTime)
{
// 	m_AccuTime = 0.0f; //누적시간 

// 	if(m_CoolTime < m_ScatterData.CoolTime)
// 	m_CoolTime += _dTime;
// 
// 	if(m_b_is_TimerOn == true)
// 	{
// 		m_AccuTime += SHGlobal_GetEngineTime * 1000.0f ; //[2014.11.27 ]
// 	}
// 
// 	if(m_DelayTime < m_AccuTime) // 누적시간이 딜레이시간 이상이 되면..
// 	{
// 		m_b_is_TimerOn = false; // 다시 타이머를 [2014.11.27 ]
// 		m_bAlive = true;
// 		m_AccuTime = 0.0f;
// 	}
// 
// 	if(m_bAlive == false) //[2014.11.17 ]
// 		return;


	for(int i= 0 ; i<MAXCARD_NUM ; i++)
	{
		m_Card[i].Update(_dTime);  // [2014.12.1 ]
	}


	
}




//-----------------------------------------------------------------------------
// Desc   : draw 하기 직전 호출되는 함수
//-----------------------------------------------------------------------------
void	CScatterSkill::_OnDrawStart()
{
	// 셰이더 상수 설정
	//{

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
	//}
}


//-----------------------------------------------------------------------------
// Arg    : D3DXVECTOR3 _StartPos, D3DXMATRIX _CharacterWorldTM , int DelayFrame 
// Return :
// Desc   : 카드스킬 발동!! DelayFrame 만큼 기다렸다가 발동된다.
//			_StartPos 발동되었을때 시작위치 
//			_CharacterWorldTM 이스킬을 사용한 시점의 캐릭터 월드TM// [2014.11.27 ]
//-----------------------------------------------------------------------------
void	    CScatterSkill::SetDelayTime_for_Begin(D3DXVECTOR3 _StartPos, D3DXVECTOR3 _SeeVec,
												D3DXMATRIX _CharacterWorldTM , int DelayFrame, const SHR_Mesh* _pRefMesh /*= nullptr*/) // [2014.11.27 ]
{
// 	m_DelayTime = DelayFrame*30; //SHGlobal_GetEngineTime * 1000.0f * m_Velo; 
// 	m_Pos = _StartPos;
// 	m_StartPos = _StartPos;
// 	m_CharacterWorldTM = _CharacterWorldTM;
// 
// 	m_b_is_TimerOn = true; // [2014.11.27 ]
// 	m_VecSee =_SeeVec;// 위치 [2014.11.27 ]
// 	m_CoolTime = 0.0f;

	D3DXMATRIX RotTM_Plus,RotTM_Minus;
	D3DXMATRIX ResTM_Plus,ResTM_Minus; // [2014.12.1 ]
	D3DXVECTOR3 ResVec_Plus,ResVec_Minus;

	D3DXMatrixIdentity(&ResTM_Plus); D3DXMatrixIdentity(&ResTM_Minus);
// 
// 	D3DXMatrixRotationY(&RotTM_Plus , m_ScatterData.EachAngle);
// 	D3DXMatrixRotationY(&RotTM_Minus , -1.0f*m_ScatterData.EachAngle); // [2014.12.1 ]

	m_Card[0].SetDelayTime_for_Begin(_StartPos, _SeeVec, _CharacterWorldTM , DelayFrame, _pRefMesh);
	int j =1;
	for(int i = 1; i < MAXCARD_NUM ; i+=2)
	{
		D3DXMatrixRotationY(&RotTM_Plus , (float)j * m_ScatterData.EachAngle);
		D3DXMatrixRotationY(&RotTM_Minus ,(float)j * -1.0f * m_ScatterData.EachAngle); // [2014.12.1 ]

		ResTM_Plus = RotTM_Plus * _CharacterWorldTM ;
		D3DXVec3TransformNormal(&ResVec_Plus , &_SeeVec, &RotTM_Plus);
		m_Card[i].SetDelayTime_for_Begin(_StartPos, ResVec_Plus, ResTM_Plus, DelayFrame, _pRefMesh);

		ResTM_Minus = RotTM_Minus * _CharacterWorldTM ;
		D3DXVec3TransformNormal(&ResVec_Minus , &_SeeVec, &RotTM_Minus);
		m_Card[i+1].SetDelayTime_for_Begin(_StartPos, ResVec_Minus,ResTM_Minus,DelayFrame, _pRefMesh );

		j++;
	}
}


//-----------------------------------------------------------------------------
// Desc   :  루아 파일에서 테디베어 속성정보를 불러온다. // [2014.11.14 ]
//-----------------------------------------------------------------------------
int			CScatterSkill::Load_AttrInfo_from_Lua(const CHAR* _pPath)
{


	SH_Lua TempLua(_pPath); //(SH_Lua)는 반드시 지역변수로 선언!!
	//int tableSize; 
	// [2014.10.20 ]
	try
	{
		// 루아 전역변수 Tedy_MoveSpeed 을 받아온다. // [2014.11.14 ]
		TempLua.GetGlobal_float("Card_MoveSpeed" ,m_ScatterData.MoveSpeed);
		TempLua.GetGlobal_float("Card_Range" ,m_ScatterData.Range);
		TempLua.GetGlobal_float("Card_Cooltime" ,m_ScatterData.CoolTime); //[2014.11.28 ]
		TempLua.GetGlobal_float("Card_Angle" ,m_ScatterData.EachAngle); // [2014.12.1 ]
		
		m_CoolTime = m_ScatterData.CoolTime+1;
		
		// 루아 전역변수 Tedy_RotSpeed 을 받아온다. // [2014.11.14 ]
		//TempLua.GetGlobal_float("Tedy_RotSpeed" , m_ScatterData.RotSpeed);
	
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
bool CScatterSkill::MsgProc( SH_MSG* _pMessage )
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
