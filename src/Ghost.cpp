
#include "DXUT.H"
#include "ASEMesh.h"
#include "GameObject.h"
#include "Ghost.h"



//////////////////////////////////////////////////////////////////////////
// 전역변수 추가 
D3DXVECTOR3			G_BedPos = D3DXVECTOR3(-187.0f, 50.0f, 6.08f); // 변하지않는 침대위치 !! /-
// 고스트 데이타 베이스 전역변수  //[2014.11.16 ]
CGhost_DataBase		G_GhostDataBase;

// 고스트 포인트 미리 200개 생성
CGhost*				G_pGhost_Arr[GHOSTNUM_MAX];
CGhost*				G_pGhost_Origne = nullptr; //복사될 원본이되는  고스트 [2014.12.7 ]	
//////////////////////////////////////////////////////////////////////////
// 스테틱 변수 초기화
CString		CGhost::m_EntityType = "Ghost"; // 엔티티별 타입 이름
int			CGhost::m_CreatedGhostCnt =0; // 생성된 고스트 총 개수 [2014.11.17 ]





//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------
// struct GHOST_DATA 의 생성자   [2014.7.29 ]
//-------------------------------------------------------------------------

GHOST_DATA::GHOST_DATA()
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
//여기서부터 CGhost 함수정의
// 생성자
CGhost::CGhost() //:  m_pMesh(nullptr)
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

	// 현재 유령이 바라보고있는방향 
	m_VecSee = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	m_Vec_DestVector = m_VecSee; // [2014.11.13 ]
	m_Angle_Yaw = 0.0f;  // up vector를 기준으로 회전한 각도 

	
	m_GhostData.MoveSpeed = 36.0f;  //[2014.11.15 ]
	m_GhostData.RotSpeed = 5.4f; //[2014.11.15 ]
	m_GhostData.GhostType= 1; //  고스트 레벨별 타입 1이면 레벨1 2면 레벨2 3이면 레벨3;
	m_GhostData.HP = 1; //[2014.11.15 ]

	m_Vec_StartSee = D3DXVECTOR3(0,0, -1);
	m_CurPassPoint = 0;  

	m_pBoundSphere_Body = nullptr;
	m_pBSMesh_Ghost[0] = nullptr; 
	m_bAlive = false;
	m_DamageTimer = 0.0f; 

	m_eActState = eGAS_MOVING;
	m_pEffDeath = nullptr;// 죽을때 나오는 이펙트 
	m_pEffGoal = nullptr;// 아이한테 갔을때 나오는 이펙트 
	m_bVisible = true;
}

CGhost::~CGhost()
{
	write_log_file("메모리 해제 : ~CGhost() 호출");

	SAFE_DELETE_(m_pEffDeath)// 죽을때 나오는 이펙트 
	SAFE_DELETE_(m_pEffGoal)// 아이한테 갔을때 나오는 이펙트 // [2014.12.3 ]
}



//-------------------------------------------------------------------------
// Arg	  : D3DXVECTOR3 _SPos	 : 시작위치 (기본값 D3DXVECTOR3(0,0,200))
//			GHOST_DATA* pMData : 몬스터 속성 데이터를 가지고있는 구조체
// Return : 성공(S_OK), 실패(S_FALSE)
// Desc   : CGhost 클래스는 반드시 이함수로 초기화시켜 줘야한다. [2014.10.7 ]
//-------------------------------------------------------------------------

int  CGhost::Init(LPDIRECT3DDEVICE9 _pDevice, D3DXVECTOR3 _SPos /*= D3DXVECTOR3(0,0,200)*/, GHOST_DATA* pBData )
{
	HRESULT RET = S_OK;
	if(FAILED(CEntity::Init(_pDevice)))		
	{
		return FALSE;
	}

	if(pBData != nullptr)
	{
		memcpy(&m_GhostData, pBData, sizeof(GHOST_DATA));

	m_VecRight = D3DXVECTOR3(1,0,0);
	m_VecSee   = D3DXVECTOR3(0,0,-1); 
	m_VecUp    = D3DXVECTOR3(0,1,0);

	m_Pos = _SPos;
	
	m_GhostDatabk = m_GhostData;

	return RET;

}

//-------------------------------------------------------------------------
// Arg	  : 
// Return :
// Desc   :  데이터베이스에있는 스폰데이터를 사용해 초기화 하는 함수 [2014.11.17 ]
//-------------------------------------------------------------------------
int		CGhost::InitUsingSpawnData(const GhostFactory_Arg& _SpwnInfo ,int TimeSection , int PositionNum, bool Alive)
{
	HRESULT hr = S_OK;
	int RandNum;

	srand((unsigned)time(NULL));

	
	m_bAlive = Alive;
	m_VecSee   = _SpwnInfo.m_GhostLookVec;
	m_CurPassPoint = 0;        

	/////////////////////////////////////////////////////////////////////////////////
	// 스폰위치를 정해진 반경 이내로 랜덤으로 정해주기 


    RandNum = rand()%(int)(_SpwnInfo.m_fSpawnRadius*2.0f); // 0 ~ 100 사이의 숫자를 생성
	RandNum -= _SpwnInfo.m_fSpawnRadius;
	m_Pos.x = _SpwnInfo.Pos.x + RandNum;  // 스폰위치
	m_Pos.z = _SpwnInfo.Pos.z + RandNum;  // 스폰위치
	if(RandNum < 0) RandNum = 2.0f;      //  방밑으로 안생기게하기 
	m_Pos.y = _SpwnInfo.Pos.y + RandNum;  // 스폰위치
	
	

	/////////////////////////////////////////////////////////////////////////////////
	D3DXMatrixIdentity(&m_MTrans);
	D3DXMatrixIdentity(&m_MRot);

	D3DXMatrixIdentity(&m_MRotPitch);
	D3DXMatrixIdentity(&m_MRotRoll);
	D3DXMatrixIdentity(&m_MRotYaw);

	
	m_Angle_Yaw = 0.0f;  // up vector를 기준으로 회전한 각도 

	//////////////////////////////////////////////////////////////////////////////////
	//확률정보에 따라 랜덤으로  레벨정하기
	int Section[4] = {0,};

	Section[1] = _SpwnInfo.m_SpawnInfo[TimeSection].SpawnProbability_Lv1;
	Section[2] = Section[1] + _SpwnInfo.m_SpawnInfo[TimeSection].SpawnProbability_Lv2 ;
	Section[3] = Section[2] + _SpwnInfo.m_SpawnInfo[TimeSection].SpawnProbability_Lv3 ;
	
	RandNum = rand()%101; // 0 ~ 100 사이의 숫자를 생성

	if( (Section[0] <= RandNum ) && (RandNum <= Section[1] )  )  // 랜덤숫자가 0 ~ 1 구간사이면 레벨1의 유령정보를 복사 //[2014.11.18 ] 
	{
		m_GhostData = G_GhostDataBase.m_GhostAttr_LV[0];   m_GhostData.GhostType = 1;   
		m_RscPack.GetAnimator().Change_Ani_Clip("IDLE");
	}
	else if( (Section[1] < RandNum ) && (RandNum <= Section[2] )  )  // 랜덤숫자가 1 ~ 2 구간사이면 레벨2의 유령정보를 복사
	{
		m_GhostData = G_GhostDataBase.m_GhostAttr_LV[1];   m_GhostData.GhostType = 2;  
		m_RscPack2.GetAnimator().Change_Ani_Clip("IDLE");
	}
	else if( (Section[2] < RandNum ) && (RandNum <= Section[3] )  )  // 랜덤숫자가 2 ~ 3 구간사이면 레벨3의 유령정보를 복사 
	{
		m_GhostData = G_GhostDataBase.m_GhostAttr_LV[2];   m_GhostData.GhostType = 3;  
		m_RscPack3.GetAnimator().Change_Ani_Clip("IDLE");
	}
	

	m_VecSee = _SpwnInfo.m_GhostLookVec;//m_Vec_StartSee ; // //[2014.11.18 ] 

	m_Vec_DestVector = _SpwnInfo.m_GhostLookVec; // 스폰지역바다 맨처음 스폰되었을때  유령이 바라봐야 할 방향 [2014.11.18 ] 


	///////////////////////////////////////// 미리 회전시켜놓기 ////////////////////////////////////////
	// 두벡터사이의 각도 구하기
	//D3DXVec3Normalize(&m_Vec_StartSee , &m_Vec_StartSee);
	D3DXVec3Normalize(&m_VecSee, &m_VecSee);
	float TempDot = D3DXVec3Dot(&m_Vec_StartSee , &m_VecSee);
	float rad = acos(TempDot);
	
    m_Angle_Yaw += rad;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// 통과지점 복사
	if(PositionNum == 1)
	{
		memcpy( m_GhostData.Vec_PassPointArr , G_GhostDataBase.m_Vec_PassPointArr1 , sizeof(D3DXVECTOR3) * PASSPOINTNUM_MAX  );
		m_GhostData.PassPointCnt = G_GhostDataBase.m_PassPointArr1_Size;
	}
	else if(PositionNum == 2)
	{
		memcpy( m_GhostData.Vec_PassPointArr , G_GhostDataBase.m_Vec_PassPointArr2 , sizeof(D3DXVECTOR3) * PASSPOINTNUM_MAX  );
		m_GhostData.PassPointCnt = G_GhostDataBase.m_PassPointArr2_Size;
	}
	else if(PositionNum == 3)
	{
		memcpy( m_GhostData.Vec_PassPointArr , G_GhostDataBase.m_Vec_PassPointArr3 , sizeof(D3DXVECTOR3) * PASSPOINTNUM_MAX  );
		m_GhostData.PassPointCnt = G_GhostDataBase.m_PassPointArr3_Size;
	}
	else if(PositionNum == 4)
	{
		memcpy( m_GhostData.Vec_PassPointArr , G_GhostDataBase.m_Vec_PassPointArr4 , sizeof(D3DXVECTOR3) * PASSPOINTNUM_MAX  );
		m_GhostData.PassPointCnt = G_GhostDataBase.m_PassPointArr4_Size;
	}

	// 애니클립  다시  IDLE로 //[2014.11.30 ]
	m_eActState = eGAS_MOVING;
	m_bVisible = true;
	m_pEffDeath->Set_m_UVAni_X(0.0f); m_pEffDeath->Set_m_bVisible(false);

	m_pEffGoal->Set_m_UVAni_X(0.0f); m_pEffGoal->Set_m_bVisible(false);

	return hr;
}



//-------------------------------------------------------------------------
// Arg	  : const float& GameTimer , 게임 
// Return : HRESULT S_OK, S_FALSE
// Desc   : 씬에서 사용됨..  데이터베이스 정보를 이용해 정해진 시간이나 간격에 따라  유령을 초기화(생성)한다. [2014.11.17 ]
//			★★ static 함수 (전역함수)  ★★
//-------------------------------------------------------------------------

int	CGhost::InitGhost_in_RealTime(const float& GameTimer  , float _dTime)
{
	HRESULT hr = S_OK;
	int tempTimer = 0;
	static float NoCreatingTime_pos1 = 0;
	static float NoCreatingTime_pos2 = 0;
	static float NoCreatingTime_pos3 = 0;
	static float NoCreatingTime_pos4 = 0;

	static bool bJustCreated_Pos1 = false;
	static bool bJustCreated_Pos2 = false;
	static bool bJustCreated_Pos3 = false;
	static bool bJustCreated_Pos4 = false;


	////////////////////////////// 포지션1에서의 유령 초기화(생성) 하기 /////////////////////////////////////////////////////////
	for(int TimeSection_Idx = 0 ; TimeSection_Idx < G_GhostDataBase.m_FactoryArg_Point1.m_SpwnInfoCnt ; TimeSection_Idx++)
	{

		if(NoCreatingTime_pos1 > 0.3f)// 유령이 생성된 직후 0.3초간은 생성안되게 한다.  [2014.11.17 ]
		{
			NoCreatingTime_pos1 =0.0f;  bJustCreated_Pos1 = false;
			break;
		}

		if(bJustCreated_Pos1 == true) // 유령이 생성된 직후 0.3초간은 생성안되게 한다. 
		{
			NoCreatingTime_pos1+=_dTime;
			break;
		}
			

		if( (G_GhostDataBase.m_FactoryArg_Point1.m_SpawnInfo[TimeSection_Idx].StartTime < GameTimer) && //현재시간이 스폰인포의 시간범위안에 들어와있다면 [2014.11.17 ]
			(G_GhostDataBase.m_FactoryArg_Point1.m_SpawnInfo[TimeSection_Idx].EndTime > GameTimer) 
		  ) 
		{
			SPAWN_INFO& currSpwnInfo = G_GhostDataBase.m_FactoryArg_Point1.m_SpawnInfo[TimeSection_Idx]; // 현재 스폰인포

			tempTimer = (int)(GameTimer*10);

			if(tempTimer % (int)(currSpwnInfo.Interval*10) == 0 )
			{
				for(int Ghostidx = 0 ; Ghostidx < GHOSTNUM_MAX ; Ghostidx++)  //루프를 돌면서 생성안된놈을 찾아 생성한다.
				{
					if(G_pGhost_Arr[Ghostidx]->m_bAlive == true) // 생성안되있는 놈을 찾기위해 넘어감
						 continue;

					G_pGhost_Arr[Ghostidx]->InitUsingSpawnData(G_GhostDataBase.m_FactoryArg_Point1, TimeSection_Idx , 1 ,  true);
					bJustCreated_Pos1 = true;
					break;
				}
			}
			
			
			break; //현재시간의 구간을 찾았으니 더이상 루프를 돌필요 없다. [2014.11.17 ]
		}
	}

	////////////////////////////// 포지션2에서의 유령 초기화(생성) 하기 /////////////////////////////////////////////////////////
	for(int TimeSection_Idx = 0 ; TimeSection_Idx < G_GhostDataBase.m_FactoryArg_Point2.m_SpwnInfoCnt ; TimeSection_Idx++)
	{
			if(NoCreatingTime_pos2 > 0.3f)// 유령이 생성된 직후 0.3초간은 생성안되게 한다.  
		{
			NoCreatingTime_pos2 =0.0f;  bJustCreated_Pos2 = false;
			break;
		}
		
		
		if(bJustCreated_Pos2 == true) // 유령이 생성된 직후 0.3초간은 생성안되게 한다. 
		{
			NoCreatingTime_pos2+=_dTime;  break;
		}


		if( (G_GhostDataBase.m_FactoryArg_Point2.m_SpawnInfo[TimeSection_Idx].StartTime < GameTimer) && //현재시간이 스폰인포의 시간범위안에 들어와있다면 [2014.11.17 ]
			(G_GhostDataBase.m_FactoryArg_Point2.m_SpawnInfo[TimeSection_Idx].EndTime > GameTimer) 
		  ) 
		{
			SPAWN_INFO& currSpwnInfo = G_GhostDataBase.m_FactoryArg_Point2.m_SpawnInfo[TimeSection_Idx]; // 현재 스폰인포

			tempTimer = (int)(GameTimer*10);

			if(tempTimer % (int)(currSpwnInfo.Interval*10) == 0 )
			{
				for(int Ghostidx = 0 ; Ghostidx < GHOSTNUM_MAX ; Ghostidx++)  //루프를 돌면서 생성안된놈을 찾아 생성한다.
				{
					if(G_pGhost_Arr[Ghostidx]->m_bAlive == true) // 생성안되있는 놈을 찾기위해 넘어감
						continue;

					G_pGhost_Arr[Ghostidx]->InitUsingSpawnData(G_GhostDataBase.m_FactoryArg_Point2, TimeSection_Idx , 2,true);
					bJustCreated_Pos2 = true;
					break;
				}
			}


			break; //현재시간의 구간을 찾았으니 더이상 루프를 돌필요 없다. [2014.11.17 ]
		}
	}

	////////////////////////////// 포지션3에서의 유령 초기화(생성) 하기 /////////////////////////////////////////////////////////
	for(int TimeSection_Idx = 0 ; TimeSection_Idx < G_GhostDataBase.m_FactoryArg_Point3.m_SpwnInfoCnt ; TimeSection_Idx++)
	{
		
		if(NoCreatingTime_pos3 > 0.3f)// 유령이 생성된 직후 0.3초간은 생성안되게 한다.  [2014.11.17 ]
		{
			NoCreatingTime_pos3 =0.0f;  bJustCreated_Pos3 = false;
			break;
		}
		
		if(bJustCreated_Pos3 == true) // 유령이 생성된 직후 0.3초간은 생성안되게 한다. 
		{
			NoCreatingTime_pos3+=_dTime; break;
		}

	 
		if( (G_GhostDataBase.m_FactoryArg_Point3.m_SpawnInfo[TimeSection_Idx].StartTime < GameTimer) && //현재시간이 스폰인포의 시간범위안에 들어와있다면 [2014.11.17 ]
			(G_GhostDataBase.m_FactoryArg_Point3.m_SpawnInfo[TimeSection_Idx].EndTime > GameTimer) 
			) 
		{
			SPAWN_INFO& currSpwnInfo = G_GhostDataBase.m_FactoryArg_Point3.m_SpawnInfo[TimeSection_Idx]; // 현재 스폰인포

			tempTimer = (int)(GameTimer*10);

			if(tempTimer % (int)(currSpwnInfo.Interval*10) == 0 )
			{
				for(int Ghostidx = 0 ; Ghostidx < GHOSTNUM_MAX ; Ghostidx++)  //루프를 돌면서 생성안된놈을 찾아 생성한다.
				{
					if(G_pGhost_Arr[Ghostidx]->m_bAlive == true) // 생성안되있는 놈을 찾기위해 넘어감
						continue;

					G_pGhost_Arr[Ghostidx]->InitUsingSpawnData(G_GhostDataBase.m_FactoryArg_Point3, TimeSection_Idx , 3 , true);
					bJustCreated_Pos3 = true;
					break;
				}
			}


			break; //현재시간의 구간을 찾았으니 더이상 루프를 돌필요 없다. [2014.11.17 ]
		}
	}

	////////////////////////////// 포지션2에서의 유령 초기화(생성) 하기 /////////////////////////////////////////////////////////
	for(int TimeSection_Idx = 0 ; TimeSection_Idx < G_GhostDataBase.m_FactoryArg_Point4.m_SpwnInfoCnt ; TimeSection_Idx++)
	{
		if(NoCreatingTime_pos4 > 0.3f)// 유령이 생성된 직후 0.3초간은 생성안되게 한다.  [2014.11.17 ]
		{
			NoCreatingTime_pos4 =0.0f;  bJustCreated_Pos4 = false;
			break;
		}
		
		if(bJustCreated_Pos4 == true) // 유령이 생성된 직후 0.3초간은 생성안되게 한다. 
		{
			NoCreatingTime_pos4+=_dTime;  break;
		}

	
		if( (G_GhostDataBase.m_FactoryArg_Point4.m_SpawnInfo[TimeSection_Idx].StartTime < GameTimer) && //현재시간이 스폰인포의 시간범위안에 들어와있다면 [2014.11.17 ]
			(G_GhostDataBase.m_FactoryArg_Point4.m_SpawnInfo[TimeSection_Idx].EndTime > GameTimer) 
			) 
		{
			SPAWN_INFO& currSpwnInfo = G_GhostDataBase.m_FactoryArg_Point4.m_SpawnInfo[TimeSection_Idx]; // 현재 스폰인포

			tempTimer = (int)(GameTimer*10);

			if(tempTimer % (int)(currSpwnInfo.Interval*10) == 0 )
			{
				for(int Ghostidx = 0 ; Ghostidx < GHOSTNUM_MAX ; Ghostidx++)  //루프를 돌면서 생성안된놈을 찾아 생성한다.
				{
					if(G_pGhost_Arr[Ghostidx]->m_bAlive == true) // 생성안되있는 놈을 찾기위해 넘어감
						continue;

					G_pGhost_Arr[Ghostidx]->InitUsingSpawnData(G_GhostDataBase.m_FactoryArg_Point4, TimeSection_Idx , 4,true);
					bJustCreated_Pos4 = true;
					break;
				}
			}


			break; //현재시간의 구간을 찾았으니 더이상 루프를 돌필요 없다. [2014.11.17 ]
		}
	}

	return hr;
}


//-------------------------------------------------------------------------
// Arg	  : 
// Return : HRESULT S_OK , S_FALSE
// Desc   : 이곳에서 콜라이더를 생성해야함(바운드스피어로)//[2014.11.11 ]
//-------------------------------------------------------------------------
// from ICollidable
HRESULT		CGhost::InitCollider() 
{
	HRESULT hr = S_OK;

	m_pCollidObj = new CBoundSphere();

	return hr;
}

//-------------------------------------------------------------------------
// 그리기 함수
//-------------------------------------------------------------------------

void CGhost::Draw()
{	
	if((m_bAlive == false) || (m_bVisible == false)) //[2014.11.17 ]
		return;

	//셰이더상수 설정
	_OnDrawStart();


//	if(eGAS_DIE == m_eActState)
//	{
	//	m_EffDeath->DrawAlpha();

//	} //[2014.12.5 ]
	if(m_GhostData.GhostType == 1)
	{	m_RscPack.Draw(m_pDevice);} //[2014.11.29 ]
	else if(m_GhostData.GhostType == 2)
	{	m_RscPack2.Draw(m_pDevice);}//[2014.11.29 ]
	else 
	{	m_RscPack3.Draw(m_pDevice);	}//[2014.11.29 ]

// 	if(eGAS_ATTACK == m_eActState)
// 	{
// 		m_EffGoal->DrawAlpha();
// 	} 
}


//-----------------------------------------------------------------------------
// Desc   : 업데이트 하기 직전 호출되는 함수
//-----------------------------------------------------------------------------
void CGhost::_OnUpdateStart(float _dTime)
{
	

	if(m_eActState == eGAS_HITED)
	{
		if(m_RscPack3.GetAnimator().GetCurClipInfo()->eAniState == eAniState_Stop)
		{
			m_RscPack3.GetAnimator().Change_Ani_Clip("IDLE");
			m_RscPack3.GetAnimator().SetAniTimeFirst();
			m_eActState = eGAS_MOVING;

		}

		if(m_RscPack2.GetAnimator().GetCurClipInfo()->eAniState == eAniState_Stop)
		{
			m_RscPack2.GetAnimator().Change_Ani_Clip("IDLE");
			m_RscPack2.GetAnimator().SetAniTimeFirst();
			m_eActState = eGAS_MOVING;

		}
	}

	

}

//-------------------------------------------------------------------------
// 업데이트 함수
//-------------------------------------------------------------------------

void CGhost::Update(float _dTime)
{


	if(m_bAlive == false) //[2014.11.17 ]
		return;

	_OnUpdateStart(_dTime);//[2014.11.30 ]

	if(m_GhostData.HP<=0)
	{		m_eActState = eGAS_DIE;}  // HP가0이면 죽은걸로!!! [2014.11.30 ]
	
	

	if(m_DamageTimer>0)
	{	m_DamageTimer -= _dTime;	}//[2014.11.30 ]

	D3DXVECTOR3 tempVec;
	float TempY = 0.0f; //[2014.11.19 ]
	//_OnUpdateStart(_dTime);

	////////////////////////////////////////////////////////////////////////////
	// 패스포인트 에 따라 목적벡터 바꿔주기  //[2014.11.18 ]

	m_Vec_DestVector = m_GhostData.Vec_PassPointArr[m_CurPassPoint] - m_Pos;
	
	// 패스포인트와 거리체크를 위해  [2014.11.19 ]
	float TempDist = D3DXVec3Length(&m_Vec_DestVector);
	D3DXVec3Normalize(&tempVec, &m_Vec_DestVector);
	TempY = tempVec.y; //  미리 목적벡터의 y값을 저장해 놓는다. //[2014.11.19 ]
	m_Vec_DestVector.y = 0.0f; //벡터를 2차원으로 만든다.  [2014.11.19 ]

	// 거리가 일정값이하로 되면 패스포인트 바꾸기   [2014.11.18 ]
	if(TempDist < 13.1f)
	{	++m_CurPassPoint;	}



	// 마지막 패스포인트(침대) 라면 죽은걸로 설정 [2014.11.18 ]
	if(m_CurPassPoint == m_GhostData.PassPointCnt)
	{	
		m_bAlive = false;
		G_GirlHP -= m_GhostData.Attack;

		GUI_Base* pPrg = nullptr;
		pPrg = CDialogRscMgr::GetInstance()->GetControl(eUI_ID_HPBar);
		pPrg->SetPercentage(G_GirlHP);
	}
	///////////////////////////////////////////////////////////////////////////
	//m_MRotPitch,  m_MRotRoll,  m_MRotYaw;

	D3DXMatrixRotationY(&m_MRot , m_Angle_Yaw  );
	//D3DXMatrixRotation(&m_MRotYaw , m_Angle_Yaw  );

	// 시선벡터도 누적된 각도 만큼 회전시켜준다. [2014.11.13 ]
	D3DXVec3TransformNormal(&m_VecSee , &m_Vec_StartSee, &m_MRot);

	//////////////////////////////////////////////////////////////////////////
	// 여기서부터 DestVector를 기준으로 회전행렬 계산하기

	// 두벡터사이의 각도 구하기
	D3DXVec3Normalize(&m_Vec_DestVector , &m_Vec_DestVector);
	D3DXVec3Normalize(&m_VecSee, &m_VecSee);
	float TempDot = D3DXVec3Dot(&m_Vec_DestVector , &m_VecSee);


	if (TempDot < 0.94f) // 겹쳐지기 전까지만 회전 [2014.11.13 ]
	{		
			if(IsLeft_Vec_Vec_3D(m_VecSee, m_Vec_DestVector) == true)
			{ 		m_Angle_Yaw += -m_GhostData.RotSpeed *_dTime;			}
			else
			{		m_Angle_Yaw += m_GhostData.RotSpeed *_dTime;			}			
	}
	else//TempDot > 0.99f 이어서 두벡터가 동일하다고 판단될때는 강제로 벡터를 일치시킨다.
	{		m_VecSee = m_Vec_DestVector;	}
	
	if((m_eActState == eGAS_MOVING) || (m_eActState == eGAS_ATTACK))
	{
		// 유령이 시선벡터 방향으로 이동 
		tempVec = m_VecSee +  D3DXVECTOR3(0.0f, TempY , 0.0f );
		m_Pos += tempVec * m_GhostData.MoveSpeed * _dTime;// 유령의 이동속도를 시선벡터에 곱해 위치에 더해준다.[2014.11.15 ]
	}
	

	//////////////////////////////////////////////////////////////////////////
	// 

	if(eGAS_DIE == m_eActState)
	{	m_pEffDeath->Set_m_bVisible(true); m_bVisible = false;} 


	if(eGAS_ATTACK == m_eActState)
	{	m_pEffGoal->Set_m_bVisible(true); } 


	//////////////////////////////////////////////////////////////////////////

	D3DXMatrixIdentity(&m_MTrans);
	D3DXMatrixTranslation(&m_MTrans, m_Pos.x , m_Pos.y , m_Pos.z);

	m_MWorld = m_MRot * m_MTrans;//최종행렬 계산
		
	if(m_GhostData.GhostType == 1)
	{
		m_RscPack.Update(_dTime, &m_MWorld);
		// 침대와 가까워지면 유령 애니 변하기
		if((m_CurPassPoint == m_GhostData.PassPointCnt-1 )&& (TempDist < 20.1f))
		{	m_RscPack.GetAnimator().Change_Ani_Clip("ATTACK");		}

	} //[2014.11.29 ]
	else if(m_GhostData.GhostType == 2)
	{
		m_RscPack2.Update(_dTime, &m_MWorld);
		// 침대와 가까워지면 유령 애니 변하기
		if((m_CurPassPoint == m_GhostData.PassPointCnt-1 )&& (TempDist < 23.1f))
		{	m_RscPack2.GetAnimator().Change_Ani_Clip("ATTACK");		}
	}//[2014.11.29 ]
	else 
	{	
		m_RscPack3.Update(_dTime, &m_MWorld);	
		// 침대와 가까워지면 유령 애니 변하기
		if((m_CurPassPoint == m_GhostData.PassPointCnt-1 )&& (TempDist < 23.1f))
		{	m_RscPack2.GetAnimator().Change_Ani_Clip("ATTACK");		}

	}//[2014.11.29 ]
	

	D3DXMATRIX BSTM = m_pBSMesh_Ghost[m_GhostData.GhostType-1]->GetWorldTM();
	m_pBoundSphere_Body->SetCenter(D3DXVECTOR3(BSTM._41,BSTM._42, BSTM._43));

	tempVec = G_BedPos - m_Pos ;
	TempDist = D3DXVec3Length(&tempVec);

	if(TempDist <30)
	{	
		m_eActState = eGAS_ATTACK; //[2014.12.5 ]
		int res = m_pEffGoal->Update(_dTime,D3DXVECTOR3(BSTM._41,BSTM._42+10, BSTM._43) );

		if(res == 1) 
		{
			m_bAlive = false;  G_GirlHP -= m_GhostData.Attack;
		}
	}

	if(eGAS_DIE == m_eActState)
	{
		int res = m_pEffDeath->Update(_dTime,D3DXVECTOR3(BSTM._41,BSTM._42, BSTM._43));
		
		if(res == 1) 
		{		
			m_bAlive = false;	m_pEffDeath->Set_m_bVisible(false);
		}
	} //[2014.12.5 ]
	//////////////////////////////////////////////////////////////////////////
	// 여기서 충돌처리 
	//m_RscPack.m_pRefPoint_BoundSphere->m
	//CCollideMgr::getInstance()->m_Collider_List[eCollide_Tag_Ghost].be
	

}



//////////////////////////////////////////////////////////////////////////
//
void CGhost::MoveLeft(float& _dTime)
{	

	//m_Pos  -= m_VecRight*m_UData.fVelo*_dTime;

}

//////////////////////////////////////////////////////////////////////////
//
void CGhost::MoveRight(float& _dTime)
{
	//m_Pos  += m_VecRight*m_UData.fVelo*_dTime;

}


//////////////////////////////////////////////////////////////////////////
void CGhost::MoveDown(float& _dTime)
{
	//m_Pos  -= m_VecUp*m_UData.fVelo*_dTime;
}


//////////////////////////////////////////////////////////////////////////
void CGhost::MoveUp(float& _dTime)
{
	//m_Pos  += m_VecUp*m_UData.fVelo*_dTime;
}


//////////////////////////////////////////////////////////////////////////
void CGhost::MoveForward(float& _dTime)
{
	//m_Pos  += m_VecSee*m_UData.fVelo*_dTime;
}



//////////////////////////////////////////////////////////////////////////
void CGhost::MoveBack(float& _dTime)
{
	//m_Pos  -= m_VecSee*m_UData.fVelo*_dTime;
}

//////////////////////////////////////////////////////////////////////////
//
void CGhost::Rot_Pitch(float _Angle)// 우향벡터를 기준으로 회전
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

void CGhost::Rot_Roll(float _Angle)//전방벡터 기준으로 회전
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
void CGhost::Rot_Yaw(float _Angle)//상향벡터기준으로 회전
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
void	CGhost::_OnDrawStart()
{
	// 셰이더 상수 설정
	{

		// 어떤테크닉을 쓸꺼냐 시작할때 반드시 결정 해서 핸들을 저장해 놓는다.[2014.10.29 ]
// 		if(SHG_Input.key(DIK_P))
// 			m_hTech = G_FXShader.GetPointer()->GetTechniqueByName("Skinning_DirLight_SpecularMap");
// 		else
// 			m_hTech = G_FXShader.GetPointer()->GetTechniqueByName("Skinning_DirLight");
		// 1. 이펙트 객체 가져오기 [2014.10.29 ]
		LPD3DXEFFECT pEff = G_FXShader.GetPointer();                             

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

		D3DXVECTOR4 temp = D3DXVECTOR4(0.8f, 0.1f,0.7f , 1.0f);
		pEff->SetVector(G_ConstantTable.h_LimLightColor , &temp);

		pEff->SetFloat(G_ConstantTable.h_LimOffset , 1.9f);

		if(m_hTech == nullptr )  // 테크 핸들 확인해보기 
		{		assert(false);	} 

		G_FXShader.m_pShader->SetFloat(G_ConstantTable.h_UVAni_Val , 0.0f); 
	//	G_FXShader.GetPointer()->SetTechnique(m_hTech);
	}
}

//-----------------------------------------------------------------------------
// Desc   :  루아 파일에서 테디베어 속성정보를 불러온다. 
//-----------------------------------------------------------------------------
int			CGhost::Load_AttrInfo_from_Lua(const CHAR* _pPath)
{


	SH_Lua TempLua(_pPath); //(SH_Lua)는 반드시 지역변수로 선언!!
	//int tableSize; 

	try
	{
		// 루아 전역변수 Tedy_MoveSpeed 을 받아온다. 
		//TempLua.GetGlobal_float("Tedy_MoveSpeed" ,m_GhostData.MoveSpeed);
		// 루아 전역변수 Tedy_RotSpeed 을 받아온다.
		//TempLua.GetGlobal_float("Tedy_RotSpeed" , m_GhostData.RotSpeed);
	
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
bool CGhost::MsgProc( SH_MSG* _pMessage )
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
				case WM_LBUTTONUP:  //토글버튼에서 온 메세지가 WM_LBUTTONUP 이라면 
					{
						toggle ^=1;
						if(toggle == 1)
						{	// 상수테이블에 저장된 테크닉 핸들로 현재 테크닉을 바꿔준다.
							m_hTech = G_ConstantTable.h_Tech_Skin_SpecularMap_DirLight;
						}
						else
						{	// 상수테이블에 저장된 테크닉 핸들로 현재 테크닉을 바꿔준다.
							m_hTech = G_ConstantTable.h_Tech_Skin_DirLight;  
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
//
// 여기서부터 CGhost_DataBase 함수정의
//
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
//-----------------------------------------------------------------------------
// Arg    : 파일경로들
// Return : HRESULT S_OK, S_FALSE  
// Desc   : 루아스크립트의 정보로부터 고스트 데이터베이스를 만든다. //[2014.11.16 ]
//-----------------------------------------------------------------------------
int		CGhost_DataBase::Load_From_Lua(const CHAR* pPath_GhostAttr, const CHAR* pPath_GhostClipInfo,
									   const CHAR* pPath_Ghost_SpawnPoint)
{
	//////////////////////////////////////////////////////////////////////////
	// 첫번째로 Ghost attr을 읽어들인다. [2014.11.16 ]
	{						
		SH_Lua TempLua(pPath_GhostAttr); //(SH_Lua)는 반드시 지역변수로 선언!! [2014.11.16 ]
		//int tableSize; 
		try
		{
			// 루아 전역변수 Ghost_LV1_MoveSpeed 을 받아온다. 
			TempLua.GetGlobal_float("Ghost_LV1_MoveSpeed" , m_GhostAttr_LV[0].MoveSpeed);
			// 루아 전역변수 Ghost_LV1_RotSpeed 을 받아온다
			TempLua.GetGlobal_float("Ghost_LV1_RotSpeed" , m_GhostAttr_LV[0].RotSpeed);
			// 루아 전역변수 Ghost_LV1_HP 을 받아온다. 
			TempLua.GetGlobal_int("Ghost_LV1_HP" , m_GhostAttr_LV[0].HP);
			// 루아 전역변수 Ghost_LV1_HP 을 받아온다
			TempLua.GetGlobal_int("Ghost_LV1_ATTACK" , m_GhostAttr_LV[0].Attack);

			// 루아 전역변수 Ghost_LV1_MoveSpeed 을 받아온다. 
			TempLua.GetGlobal_float("Ghost_LV2_MoveSpeed" , m_GhostAttr_LV[1].MoveSpeed);
			// 루아 전역변수 Ghost_LV1_RotSpeed 을 받아온다
			TempLua.GetGlobal_float("Ghost_LV2_RotSpeed" , m_GhostAttr_LV[1].RotSpeed);
			// 루아 전역변수 Ghost_LV1_HP 을 받아온다. 
			TempLua.GetGlobal_int("Ghost_LV2_HP" , m_GhostAttr_LV[1].HP);
			// 루아 전역변수 Ghost_LV1_HP 을 받아온다.
			TempLua.GetGlobal_int("Ghost_LV2_ATTACK" , m_GhostAttr_LV[1].Attack);

			// 루아 전역변수 Ghost_LV1_MoveSpeed 을 받아온다. 
			TempLua.GetGlobal_float("Ghost_LV3_MoveSpeed" , m_GhostAttr_LV[2].MoveSpeed);
			// 루아 전역변수 Ghost_LV1_RotSpeed 을 받아온다.
			TempLua.GetGlobal_float("Ghost_LV3_RotSpeed" , m_GhostAttr_LV[2].RotSpeed);
			// 루아 전역변수 Ghost_LV1_HP 을 받아온다.
			TempLua.GetGlobal_int("Ghost_LV3_HP" , m_GhostAttr_LV[2].HP);
			// 루아 전역변수 Ghost_LV1_HP 을 받아온다
			TempLua.GetGlobal_int("Ghost_LV3_ATTACK" , m_GhostAttr_LV[2].Attack);

		}
		catch(...)
		{
			SHUtil::MsgBox("CAnimator::Load_ClipInfo_from_Lua() , 루아예외처리!!");
			return S_FALSE;
		}																												
	}

	//////////////////////////////////////////////////////////////////////////
	// 두번쨰로 GhostClipInfo을 읽어들인다. [2014.11.16 ]
	{/*
		SH_Lua TempLua(pPath_GhostClipInfo); //(SH_Lua)는 반드시 지역변수로 선언!! [2014.11.16 ]
		int tableSize; 
		try
		{
			TempLua.GetGlobal_int("Clip_Info_Size" , tableSize);

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
				CString tempName = clipName;
				m_Animator.AddAniClip(tempName, STime, ETime);
			}

		}
		catch(...)
		{
			SHUtil::MsgBox("CAnimator::Load_ClipInfo_from_Lua() , 루아예외처리!!");
			return S_FALSE;
		}	
		*/
	}

	//////////////////////////////////////////////////////////////////////////
	// 세번째로 GhostClipInfo을 읽어들인다. [2014.11.16 ]
	{
		SH_Lua TempLua(pPath_Ghost_SpawnPoint); //(SH_Lua)는 반드시 지역변수로 선언!! [2014.11.16 ]
	
		lua_getglobal(TempLua.GetLuaState(), "Respawn_Point_1");


		//////////////////////////////////////////////////////////////////////////
		// 리스폰 포인트 1번
			TempLua.GetGlobal_float("Pos1_x",  m_FactoryArg_Point1.Pos.x);
			TempLua.GetGlobal_float("Pos1_y",  m_FactoryArg_Point1.Pos.y);
			TempLua.GetGlobal_float("Pos1_z",  m_FactoryArg_Point1.Pos.z);
			TempLua.GetGlobal_float("Pos1_Radius",  m_FactoryArg_Point1.m_fSpawnRadius);
			TempLua.GetGlobal_int("Respawn_Point_1_Size" , m_FactoryArg_Point1.m_SpwnInfoCnt);
			TempLua.GetGlobal_float("Pos1_LookVec_X",  m_FactoryArg_Point1.m_GhostLookVec.x);
			TempLua.GetGlobal_float("Pos1_LookVec_Y",  m_FactoryArg_Point1.m_GhostLookVec.y);
			TempLua.GetGlobal_float("Pos1_LookVec_Z",  m_FactoryArg_Point1.m_GhostLookVec.z);
		
			for(int i = 1 ; i<=  m_FactoryArg_Point1.m_SpwnInfoCnt ; i++)
			{

				lua_rawgeti(TempLua.GetLuaState(), 1, i);							
								
				TempLua.Getfield_float("start_time", m_FactoryArg_Point1.m_SpawnInfo[i-1].StartTime );
				TempLua.Getfield_float("end_time", m_FactoryArg_Point1.m_SpawnInfo[i-1].EndTime);
				TempLua.Getfield_float("interval", m_FactoryArg_Point1.m_SpawnInfo[i-1].Interval);
				TempLua.Getfield_int("LV1", m_FactoryArg_Point1.m_SpawnInfo[i-1].SpawnProbability_Lv1);
				TempLua.Getfield_int("LV2", m_FactoryArg_Point1.m_SpawnInfo[i-1].SpawnProbability_Lv2);
				TempLua.Getfield_int("LV3", m_FactoryArg_Point1.m_SpawnInfo[i-1].SpawnProbability_Lv3);
			}
	}


	{
		SH_Lua TempLua(pPath_Ghost_SpawnPoint); //(SH_Lua)는 반드시 지역변수로 선언!! [2014.11.16 ]
		lua_getglobal(TempLua.GetLuaState(), "Respawn_Point_2");

		//////////////////////////////////////////////////////////////////////////
		//	리스폰 포인트 2번
			TempLua.GetGlobal_float("Pos2_x",  m_FactoryArg_Point2.Pos.x);
			TempLua.GetGlobal_float("Pos2_y",  m_FactoryArg_Point2.Pos.y);
			TempLua.GetGlobal_float("Pos2_z",  m_FactoryArg_Point2.Pos.z);
			TempLua.GetGlobal_float("Pos2_Radius",  m_FactoryArg_Point2.m_fSpawnRadius);
			TempLua.GetGlobal_int("Respawn_Point_2_Size" ,  m_FactoryArg_Point2.m_SpwnInfoCnt);
			TempLua.GetGlobal_float("Pos2_LookVec_X",  m_FactoryArg_Point2.m_GhostLookVec.x);
			TempLua.GetGlobal_float("Pos2_LookVec_Y",  m_FactoryArg_Point2.m_GhostLookVec.y);
			TempLua.GetGlobal_float("Pos2_LookVec_Z",  m_FactoryArg_Point2.m_GhostLookVec.z);
			
			for(int i = 1 ; i<= m_FactoryArg_Point2.m_SpwnInfoCnt ; i++)
			{

				lua_rawgeti(TempLua.GetLuaState(), 1, i);

				TempLua.Getfield_float("start_time", m_FactoryArg_Point2.m_SpawnInfo[i-1].StartTime );
				TempLua.Getfield_float("end_time", m_FactoryArg_Point2.m_SpawnInfo[i-1].EndTime);
				TempLua.Getfield_float("interval", m_FactoryArg_Point2.m_SpawnInfo[i-1].Interval);
				TempLua.Getfield_int("LV1", m_FactoryArg_Point2.m_SpawnInfo[i-1].SpawnProbability_Lv1);
				TempLua.Getfield_int("LV2", m_FactoryArg_Point2.m_SpawnInfo[i-1].SpawnProbability_Lv2);
				TempLua.Getfield_int("LV3", m_FactoryArg_Point2.m_SpawnInfo[i-1].SpawnProbability_Lv3);
			}

	}


	{
		SH_Lua TempLua(pPath_Ghost_SpawnPoint); //(SH_Lua)는 반드시 지역변수로 선언!! [2014.11.16 ]
		lua_getglobal(TempLua.GetLuaState(), "Respawn_Point_3");
		//////////////////////////////////////////////////////////////////////////
		//	리스폰 포인트 3번
			TempLua.GetGlobal_float("Pos3_x",  m_FactoryArg_Point3.Pos.x);
			TempLua.GetGlobal_float("Pos3_y",  m_FactoryArg_Point3.Pos.y);
			TempLua.GetGlobal_float("Pos3_z",  m_FactoryArg_Point3.Pos.z);
			TempLua.GetGlobal_float("Pos3_Radius",  m_FactoryArg_Point3.m_fSpawnRadius);
			TempLua.GetGlobal_int("Respawn_Point_3_Size" , m_FactoryArg_Point3.m_SpwnInfoCnt);
			TempLua.GetGlobal_float("Pos3_LookVec_X",  m_FactoryArg_Point3.m_GhostLookVec.x);
			TempLua.GetGlobal_float("Pos3_LookVec_Y",  m_FactoryArg_Point3.m_GhostLookVec.y);
			TempLua.GetGlobal_float("Pos3_LookVec_Z",  m_FactoryArg_Point3.m_GhostLookVec.z);


			for(int i = 1 ; i<= m_FactoryArg_Point3.m_SpwnInfoCnt ; i++)
			{
				lua_rawgeti(TempLua.GetLuaState(), 1, i);

				TempLua.Getfield_float("start_time", m_FactoryArg_Point3.m_SpawnInfo[i-1].StartTime );
				TempLua.Getfield_float("end_time", m_FactoryArg_Point3.m_SpawnInfo[i-1].EndTime);
				TempLua.Getfield_float("interval", m_FactoryArg_Point3.m_SpawnInfo[i-1].Interval);
				TempLua.Getfield_int("LV1", m_FactoryArg_Point3.m_SpawnInfo[i-1].SpawnProbability_Lv1);
				TempLua.Getfield_int("LV2", m_FactoryArg_Point3.m_SpawnInfo[i-1].SpawnProbability_Lv2);
				TempLua.Getfield_int("LV3", m_FactoryArg_Point3.m_SpawnInfo[i-1].SpawnProbability_Lv3);
			}

	}

	{
		SH_Lua TempLua(pPath_Ghost_SpawnPoint); //(SH_Lua)는 반드시 지역변수로 선언!! [2014.11.16 ]
		lua_getglobal(TempLua.GetLuaState(), "Respawn_Point_4");
		//////////////////////////////////////////////////////////////////////////
		//	리스폰 포인트 3번
			TempLua.GetGlobal_float("Pos4_x",  m_FactoryArg_Point4.Pos.x);
			TempLua.GetGlobal_float("Pos4_y",  m_FactoryArg_Point4.Pos.y);
			TempLua.GetGlobal_float("Pos4_z",  m_FactoryArg_Point4.Pos.z);
			TempLua.GetGlobal_float("Pos4_Radius",  m_FactoryArg_Point4.m_fSpawnRadius);
			TempLua.GetGlobal_int("Respawn_Point_4_Size" , m_FactoryArg_Point4.m_SpwnInfoCnt);
			TempLua.GetGlobal_float("Pos4_LookVec_X",  m_FactoryArg_Point4.m_GhostLookVec.x);
			TempLua.GetGlobal_float("Pos4_LookVec_Y",  m_FactoryArg_Point4.m_GhostLookVec.y);
			TempLua.GetGlobal_float("Pos4_LookVec_Z",  m_FactoryArg_Point4.m_GhostLookVec.z);


			for(int i = 1 ; i<= m_FactoryArg_Point4.m_SpwnInfoCnt ; i++)
			{
				lua_rawgeti(TempLua.GetLuaState(), 1, i);

				TempLua.Getfield_float("start_time", m_FactoryArg_Point4.m_SpawnInfo[i-1].StartTime );
				TempLua.Getfield_float("end_time", m_FactoryArg_Point4.m_SpawnInfo[i-1].EndTime);
				TempLua.Getfield_float("interval", m_FactoryArg_Point4.m_SpawnInfo[i-1].Interval);
				TempLua.Getfield_int("LV1", m_FactoryArg_Point4.m_SpawnInfo[i-1].SpawnProbability_Lv1);
				TempLua.Getfield_int("LV2", m_FactoryArg_Point4.m_SpawnInfo[i-1].SpawnProbability_Lv2);
				TempLua.Getfield_int("LV3", m_FactoryArg_Point4.m_SpawnInfo[i-1].SpawnProbability_Lv3);
			}

		}


		{

		SH_Lua TempLua(pPath_Ghost_SpawnPoint); //(SH_Lua)는 반드시 지역변수로 선언!! [2014.11.16 ]
		
		lua_getglobal(TempLua.GetLuaState(), "PassPointArray_1");//[2014.12.2 ]

		/////////////////////////////////////////////////////////////////////////////////
		// 통과지점 배열 ,  스폰위치 1에서 나타나는 유령의 통과지점[2014.11.18 ]

		TempLua.GetGlobal_int("PassPointArray_1_Size" , m_PassPointArr1_Size);

			for(int i = 1 ; i<= m_PassPointArr1_Size ; i++)
			{
				lua_rawgeti(TempLua.GetLuaState(), 1, i);

				TempLua.Getfield_float("X", m_Vec_PassPointArr1[i-1].x );
				TempLua.Getfield_float("Y", m_Vec_PassPointArr1[i-1].y );
				TempLua.Getfield_float("Z", m_Vec_PassPointArr1[i-1].z );
			}
		}
		/////////////////////////////////////////////////////////////////////////////////
		// 통과지점 배열 ,  스폰위치 2에서 나타나는 유령의 통과지점[2014.11.18 ]
		{
			SH_Lua TempLua(pPath_Ghost_SpawnPoint); //(SH_Lua)는 반드시 지역변수로 선언!! [2014.11.16 ]
			lua_getglobal(TempLua.GetLuaState(), "PassPointArray_2");

			TempLua.GetGlobal_int("PassPointArray_2_Size" , m_PassPointArr2_Size);

			for(int i = 1 ; i<= m_PassPointArr2_Size ; i++)
			{
				lua_rawgeti(TempLua.GetLuaState(), 1, i);

				TempLua.Getfield_float("X", m_Vec_PassPointArr2[i-1].x );
				TempLua.Getfield_float("Y", m_Vec_PassPointArr2[i-1].y );
				TempLua.Getfield_float("Z", m_Vec_PassPointArr2[i-1].z );
			}
		}

		{

		
		/////////////////////////////////////////////////////////////////////////////////
		// 통과지점 배열 ,  스폰위치 3에서 나타나는 유령의 통과지점[2014.11.18 ]
			SH_Lua TempLua(pPath_Ghost_SpawnPoint); //(SH_Lua)는 반드시 지역변수로 선언!! [2014.11.16 ]
			lua_getglobal(TempLua.GetLuaState(), "PassPointArray_3");
		
			TempLua.GetGlobal_int("PassPointArray_3_Size" , m_PassPointArr3_Size);

			for(int i = 1 ; i<= m_PassPointArr3_Size ; i++)
			{
				lua_rawgeti(TempLua.GetLuaState(), 1, i);

				TempLua.Getfield_float("X", m_Vec_PassPointArr3[i-1].x );
				TempLua.Getfield_float("Y", m_Vec_PassPointArr3[i-1].y );
				TempLua.Getfield_float("Z", m_Vec_PassPointArr3[i-1].z );
			}
		}

		{

		SH_Lua TempLua(pPath_Ghost_SpawnPoint); //(SH_Lua)는 반드시 지역변수로 선언!! [2014.11.16 ]
		/////////////////////////////////////////////////////////////////////////////////
		// 통과지점 배열 ,  스폰위치 4에서 나타나는 유령의 통과지점[2014.11.18 ]
		lua_getglobal(TempLua.GetLuaState(), "PassPointArray_4");
		TempLua.GetGlobal_int("PassPointArray_4_Size" , m_PassPointArr4_Size);

			for(int i = 1 ; i<= m_PassPointArr4_Size ; i++)
			{
				lua_rawgeti(TempLua.GetLuaState(), 1, i);

				TempLua.Getfield_float("X", m_Vec_PassPointArr4[i-1].x );
				TempLua.Getfield_float("Y", m_Vec_PassPointArr4[i-1].y );
				TempLua.Getfield_float("Z", m_Vec_PassPointArr4[i-1].z );
			}

		}
	
	return S_OK;
}

//-----------------------------------------------------------------------------
// Arg    : int _Damage
// Return : bool 데미지 들어갔으면 true,, 아니면 false 
// Desc   :  한대 맞았을때 HP를 달게하는 함수
//-----------------------------------------------------------------------------
bool		CGhost::DamagedFromHit(int _Damage)//[2014.11.30 ]
{

	if(m_DamageTimer >0) return false;// 아직맞은지 얼마 안되었다면 
	
	G_SHSoundMgr.PlayWave(eSound_Enemy_Hit, false);

	m_DamageTimer = 0.7f; // 맞은뒤 0.7초동안 맞지 않게 하기

	m_GhostData.HP -= _Damage; //[2014.11.30 ]

	m_RscPack2.GetAnimator().Change_Ani_Clip("BE_ATTACKED");//[2014.11.30 ]
	m_RscPack3.GetAnimator().Change_Ani_Clip("BE_ATTACKED");//[2014.11.30 ]
	m_eActState = eGAS_HITED; // FSM 상태바꾸기

	

	return true;


}
/*************************************************** end of this file **************************************************************/