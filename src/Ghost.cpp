
#include "DXUT.H"
#include "ASEMesh.h"
#include "GameObject.h"
#include "Ghost.h"



//////////////////////////////////////////////////////////////////////////
// �������� �߰� 
D3DXVECTOR3			G_BedPos = D3DXVECTOR3(-187.0f, 50.0f, 6.08f); // �������ʴ� ħ����ġ !! /-
// ��Ʈ ����Ÿ ���̽� ��������  //[2014.11.16 ]
CGhost_DataBase		G_GhostDataBase;

// ��Ʈ ����Ʈ �̸� 200�� ����
CGhost*				G_pGhost_Arr[GHOSTNUM_MAX];
CGhost*				G_pGhost_Origne = nullptr; //����� �����̵Ǵ�  ��Ʈ [2014.12.7 ]	
//////////////////////////////////////////////////////////////////////////
// ����ƽ ���� �ʱ�ȭ
CString		CGhost::m_EntityType = "Ghost"; // ��ƼƼ�� Ÿ�� �̸�
int			CGhost::m_CreatedGhostCnt =0; // ������ ��Ʈ �� ���� [2014.11.17 ]





//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------
// struct GHOST_DATA �� ������   [2014.7.29 ]
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
//���⼭���� CGhost �Լ�����
// ������
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

	// ���� ������ �ٶ󺸰��ִ¹��� 
	m_VecSee = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	m_Vec_DestVector = m_VecSee; // [2014.11.13 ]
	m_Angle_Yaw = 0.0f;  // up vector�� �������� ȸ���� ���� 

	
	m_GhostData.MoveSpeed = 36.0f;  //[2014.11.15 ]
	m_GhostData.RotSpeed = 5.4f; //[2014.11.15 ]
	m_GhostData.GhostType= 1; //  ��Ʈ ������ Ÿ�� 1�̸� ����1 2�� ����2 3�̸� ����3;
	m_GhostData.HP = 1; //[2014.11.15 ]

	m_Vec_StartSee = D3DXVECTOR3(0,0, -1);
	m_CurPassPoint = 0;  

	m_pBoundSphere_Body = nullptr;
	m_pBSMesh_Ghost[0] = nullptr; 
	m_bAlive = false;
	m_DamageTimer = 0.0f; 

	m_eActState = eGAS_MOVING;
	m_pEffDeath = nullptr;// ������ ������ ����Ʈ 
	m_pEffGoal = nullptr;// �������� ������ ������ ����Ʈ 
	m_bVisible = true;
}

CGhost::~CGhost()
{
	write_log_file("�޸� ���� : ~CGhost() ȣ��");

	SAFE_DELETE_(m_pEffDeath)// ������ ������ ����Ʈ 
	SAFE_DELETE_(m_pEffGoal)// �������� ������ ������ ����Ʈ // [2014.12.3 ]
}



//-------------------------------------------------------------------------
// Arg	  : D3DXVECTOR3 _SPos	 : ������ġ (�⺻�� D3DXVECTOR3(0,0,200))
//			GHOST_DATA* pMData : ���� �Ӽ� �����͸� �������ִ� ����ü
// Return : ����(S_OK), ����(S_FALSE)
// Desc   : CGhost Ŭ������ �ݵ�� ���Լ��� �ʱ�ȭ���� ����Ѵ�. [2014.10.7 ]
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
// Desc   :  �����ͺ��̽����ִ� ���������͸� ����� �ʱ�ȭ �ϴ� �Լ� [2014.11.17 ]
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
	// ������ġ�� ������ �ݰ� �̳��� �������� �����ֱ� 


    RandNum = rand()%(int)(_SpwnInfo.m_fSpawnRadius*2.0f); // 0 ~ 100 ������ ���ڸ� ����
	RandNum -= _SpwnInfo.m_fSpawnRadius;
	m_Pos.x = _SpwnInfo.Pos.x + RandNum;  // ������ġ
	m_Pos.z = _SpwnInfo.Pos.z + RandNum;  // ������ġ
	if(RandNum < 0) RandNum = 2.0f;      //  ������� �Ȼ�����ϱ� 
	m_Pos.y = _SpwnInfo.Pos.y + RandNum;  // ������ġ
	
	

	/////////////////////////////////////////////////////////////////////////////////
	D3DXMatrixIdentity(&m_MTrans);
	D3DXMatrixIdentity(&m_MRot);

	D3DXMatrixIdentity(&m_MRotPitch);
	D3DXMatrixIdentity(&m_MRotRoll);
	D3DXMatrixIdentity(&m_MRotYaw);

	
	m_Angle_Yaw = 0.0f;  // up vector�� �������� ȸ���� ���� 

	//////////////////////////////////////////////////////////////////////////////////
	//Ȯ�������� ���� ��������  �������ϱ�
	int Section[4] = {0,};

	Section[1] = _SpwnInfo.m_SpawnInfo[TimeSection].SpawnProbability_Lv1;
	Section[2] = Section[1] + _SpwnInfo.m_SpawnInfo[TimeSection].SpawnProbability_Lv2 ;
	Section[3] = Section[2] + _SpwnInfo.m_SpawnInfo[TimeSection].SpawnProbability_Lv3 ;
	
	RandNum = rand()%101; // 0 ~ 100 ������ ���ڸ� ����

	if( (Section[0] <= RandNum ) && (RandNum <= Section[1] )  )  // �������ڰ� 0 ~ 1 �������̸� ����1�� ���������� ���� //[2014.11.18 ] 
	{
		m_GhostData = G_GhostDataBase.m_GhostAttr_LV[0];   m_GhostData.GhostType = 1;   
		m_RscPack.GetAnimator().Change_Ani_Clip("IDLE");
	}
	else if( (Section[1] < RandNum ) && (RandNum <= Section[2] )  )  // �������ڰ� 1 ~ 2 �������̸� ����2�� ���������� ����
	{
		m_GhostData = G_GhostDataBase.m_GhostAttr_LV[1];   m_GhostData.GhostType = 2;  
		m_RscPack2.GetAnimator().Change_Ani_Clip("IDLE");
	}
	else if( (Section[2] < RandNum ) && (RandNum <= Section[3] )  )  // �������ڰ� 2 ~ 3 �������̸� ����3�� ���������� ���� 
	{
		m_GhostData = G_GhostDataBase.m_GhostAttr_LV[2];   m_GhostData.GhostType = 3;  
		m_RscPack3.GetAnimator().Change_Ani_Clip("IDLE");
	}
	

	m_VecSee = _SpwnInfo.m_GhostLookVec;//m_Vec_StartSee ; // //[2014.11.18 ] 

	m_Vec_DestVector = _SpwnInfo.m_GhostLookVec; // ���������ٴ� ��ó�� �����Ǿ�����  ������ �ٶ���� �� ���� [2014.11.18 ] 


	///////////////////////////////////////// �̸� ȸ�����ѳ��� ////////////////////////////////////////
	// �κ��ͻ����� ���� ���ϱ�
	//D3DXVec3Normalize(&m_Vec_StartSee , &m_Vec_StartSee);
	D3DXVec3Normalize(&m_VecSee, &m_VecSee);
	float TempDot = D3DXVec3Dot(&m_Vec_StartSee , &m_VecSee);
	float rad = acos(TempDot);
	
    m_Angle_Yaw += rad;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// ������� ����
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

	// �ִ�Ŭ��  �ٽ�  IDLE�� //[2014.11.30 ]
	m_eActState = eGAS_MOVING;
	m_bVisible = true;
	m_pEffDeath->Set_m_UVAni_X(0.0f); m_pEffDeath->Set_m_bVisible(false);

	m_pEffGoal->Set_m_UVAni_X(0.0f); m_pEffGoal->Set_m_bVisible(false);

	return hr;
}



//-------------------------------------------------------------------------
// Arg	  : const float& GameTimer , ���� 
// Return : HRESULT S_OK, S_FALSE
// Desc   : ������ ����..  �����ͺ��̽� ������ �̿��� ������ �ð��̳� ���ݿ� ����  ������ �ʱ�ȭ(����)�Ѵ�. [2014.11.17 ]
//			�ڡ� static �Լ� (�����Լ�)  �ڡ�
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


	////////////////////////////// ������1������ ���� �ʱ�ȭ(����) �ϱ� /////////////////////////////////////////////////////////
	for(int TimeSection_Idx = 0 ; TimeSection_Idx < G_GhostDataBase.m_FactoryArg_Point1.m_SpwnInfoCnt ; TimeSection_Idx++)
	{

		if(NoCreatingTime_pos1 > 0.3f)// ������ ������ ���� 0.3�ʰ��� �����ȵǰ� �Ѵ�.  [2014.11.17 ]
		{
			NoCreatingTime_pos1 =0.0f;  bJustCreated_Pos1 = false;
			break;
		}

		if(bJustCreated_Pos1 == true) // ������ ������ ���� 0.3�ʰ��� �����ȵǰ� �Ѵ�. 
		{
			NoCreatingTime_pos1+=_dTime;
			break;
		}
			

		if( (G_GhostDataBase.m_FactoryArg_Point1.m_SpawnInfo[TimeSection_Idx].StartTime < GameTimer) && //����ð��� ���������� �ð������ȿ� �����ִٸ� [2014.11.17 ]
			(G_GhostDataBase.m_FactoryArg_Point1.m_SpawnInfo[TimeSection_Idx].EndTime > GameTimer) 
		  ) 
		{
			SPAWN_INFO& currSpwnInfo = G_GhostDataBase.m_FactoryArg_Point1.m_SpawnInfo[TimeSection_Idx]; // ���� ��������

			tempTimer = (int)(GameTimer*10);

			if(tempTimer % (int)(currSpwnInfo.Interval*10) == 0 )
			{
				for(int Ghostidx = 0 ; Ghostidx < GHOSTNUM_MAX ; Ghostidx++)  //������ ���鼭 �����ȵȳ��� ã�� �����Ѵ�.
				{
					if(G_pGhost_Arr[Ghostidx]->m_bAlive == true) // �����ȵ��ִ� ���� ã������ �Ѿ
						 continue;

					G_pGhost_Arr[Ghostidx]->InitUsingSpawnData(G_GhostDataBase.m_FactoryArg_Point1, TimeSection_Idx , 1 ,  true);
					bJustCreated_Pos1 = true;
					break;
				}
			}
			
			
			break; //����ð��� ������ ã������ ���̻� ������ ���ʿ� ����. [2014.11.17 ]
		}
	}

	////////////////////////////// ������2������ ���� �ʱ�ȭ(����) �ϱ� /////////////////////////////////////////////////////////
	for(int TimeSection_Idx = 0 ; TimeSection_Idx < G_GhostDataBase.m_FactoryArg_Point2.m_SpwnInfoCnt ; TimeSection_Idx++)
	{
			if(NoCreatingTime_pos2 > 0.3f)// ������ ������ ���� 0.3�ʰ��� �����ȵǰ� �Ѵ�.  
		{
			NoCreatingTime_pos2 =0.0f;  bJustCreated_Pos2 = false;
			break;
		}
		
		
		if(bJustCreated_Pos2 == true) // ������ ������ ���� 0.3�ʰ��� �����ȵǰ� �Ѵ�. 
		{
			NoCreatingTime_pos2+=_dTime;  break;
		}


		if( (G_GhostDataBase.m_FactoryArg_Point2.m_SpawnInfo[TimeSection_Idx].StartTime < GameTimer) && //����ð��� ���������� �ð������ȿ� �����ִٸ� [2014.11.17 ]
			(G_GhostDataBase.m_FactoryArg_Point2.m_SpawnInfo[TimeSection_Idx].EndTime > GameTimer) 
		  ) 
		{
			SPAWN_INFO& currSpwnInfo = G_GhostDataBase.m_FactoryArg_Point2.m_SpawnInfo[TimeSection_Idx]; // ���� ��������

			tempTimer = (int)(GameTimer*10);

			if(tempTimer % (int)(currSpwnInfo.Interval*10) == 0 )
			{
				for(int Ghostidx = 0 ; Ghostidx < GHOSTNUM_MAX ; Ghostidx++)  //������ ���鼭 �����ȵȳ��� ã�� �����Ѵ�.
				{
					if(G_pGhost_Arr[Ghostidx]->m_bAlive == true) // �����ȵ��ִ� ���� ã������ �Ѿ
						continue;

					G_pGhost_Arr[Ghostidx]->InitUsingSpawnData(G_GhostDataBase.m_FactoryArg_Point2, TimeSection_Idx , 2,true);
					bJustCreated_Pos2 = true;
					break;
				}
			}


			break; //����ð��� ������ ã������ ���̻� ������ ���ʿ� ����. [2014.11.17 ]
		}
	}

	////////////////////////////// ������3������ ���� �ʱ�ȭ(����) �ϱ� /////////////////////////////////////////////////////////
	for(int TimeSection_Idx = 0 ; TimeSection_Idx < G_GhostDataBase.m_FactoryArg_Point3.m_SpwnInfoCnt ; TimeSection_Idx++)
	{
		
		if(NoCreatingTime_pos3 > 0.3f)// ������ ������ ���� 0.3�ʰ��� �����ȵǰ� �Ѵ�.  [2014.11.17 ]
		{
			NoCreatingTime_pos3 =0.0f;  bJustCreated_Pos3 = false;
			break;
		}
		
		if(bJustCreated_Pos3 == true) // ������ ������ ���� 0.3�ʰ��� �����ȵǰ� �Ѵ�. 
		{
			NoCreatingTime_pos3+=_dTime; break;
		}

	 
		if( (G_GhostDataBase.m_FactoryArg_Point3.m_SpawnInfo[TimeSection_Idx].StartTime < GameTimer) && //����ð��� ���������� �ð������ȿ� �����ִٸ� [2014.11.17 ]
			(G_GhostDataBase.m_FactoryArg_Point3.m_SpawnInfo[TimeSection_Idx].EndTime > GameTimer) 
			) 
		{
			SPAWN_INFO& currSpwnInfo = G_GhostDataBase.m_FactoryArg_Point3.m_SpawnInfo[TimeSection_Idx]; // ���� ��������

			tempTimer = (int)(GameTimer*10);

			if(tempTimer % (int)(currSpwnInfo.Interval*10) == 0 )
			{
				for(int Ghostidx = 0 ; Ghostidx < GHOSTNUM_MAX ; Ghostidx++)  //������ ���鼭 �����ȵȳ��� ã�� �����Ѵ�.
				{
					if(G_pGhost_Arr[Ghostidx]->m_bAlive == true) // �����ȵ��ִ� ���� ã������ �Ѿ
						continue;

					G_pGhost_Arr[Ghostidx]->InitUsingSpawnData(G_GhostDataBase.m_FactoryArg_Point3, TimeSection_Idx , 3 , true);
					bJustCreated_Pos3 = true;
					break;
				}
			}


			break; //����ð��� ������ ã������ ���̻� ������ ���ʿ� ����. [2014.11.17 ]
		}
	}

	////////////////////////////// ������2������ ���� �ʱ�ȭ(����) �ϱ� /////////////////////////////////////////////////////////
	for(int TimeSection_Idx = 0 ; TimeSection_Idx < G_GhostDataBase.m_FactoryArg_Point4.m_SpwnInfoCnt ; TimeSection_Idx++)
	{
		if(NoCreatingTime_pos4 > 0.3f)// ������ ������ ���� 0.3�ʰ��� �����ȵǰ� �Ѵ�.  [2014.11.17 ]
		{
			NoCreatingTime_pos4 =0.0f;  bJustCreated_Pos4 = false;
			break;
		}
		
		if(bJustCreated_Pos4 == true) // ������ ������ ���� 0.3�ʰ��� �����ȵǰ� �Ѵ�. 
		{
			NoCreatingTime_pos4+=_dTime;  break;
		}

	
		if( (G_GhostDataBase.m_FactoryArg_Point4.m_SpawnInfo[TimeSection_Idx].StartTime < GameTimer) && //����ð��� ���������� �ð������ȿ� �����ִٸ� [2014.11.17 ]
			(G_GhostDataBase.m_FactoryArg_Point4.m_SpawnInfo[TimeSection_Idx].EndTime > GameTimer) 
			) 
		{
			SPAWN_INFO& currSpwnInfo = G_GhostDataBase.m_FactoryArg_Point4.m_SpawnInfo[TimeSection_Idx]; // ���� ��������

			tempTimer = (int)(GameTimer*10);

			if(tempTimer % (int)(currSpwnInfo.Interval*10) == 0 )
			{
				for(int Ghostidx = 0 ; Ghostidx < GHOSTNUM_MAX ; Ghostidx++)  //������ ���鼭 �����ȵȳ��� ã�� �����Ѵ�.
				{
					if(G_pGhost_Arr[Ghostidx]->m_bAlive == true) // �����ȵ��ִ� ���� ã������ �Ѿ
						continue;

					G_pGhost_Arr[Ghostidx]->InitUsingSpawnData(G_GhostDataBase.m_FactoryArg_Point4, TimeSection_Idx , 4,true);
					bJustCreated_Pos4 = true;
					break;
				}
			}


			break; //����ð��� ������ ã������ ���̻� ������ ���ʿ� ����. [2014.11.17 ]
		}
	}

	return hr;
}


//-------------------------------------------------------------------------
// Arg	  : 
// Return : HRESULT S_OK , S_FALSE
// Desc   : �̰����� �ݶ��̴��� �����ؾ���(�ٿ�彺�Ǿ��)//[2014.11.11 ]
//-------------------------------------------------------------------------
// from ICollidable
HRESULT		CGhost::InitCollider() 
{
	HRESULT hr = S_OK;

	m_pCollidObj = new CBoundSphere();

	return hr;
}

//-------------------------------------------------------------------------
// �׸��� �Լ�
//-------------------------------------------------------------------------

void CGhost::Draw()
{	
	if((m_bAlive == false) || (m_bVisible == false)) //[2014.11.17 ]
		return;

	//���̴���� ����
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
// Desc   : ������Ʈ �ϱ� ���� ȣ��Ǵ� �Լ�
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
// ������Ʈ �Լ�
//-------------------------------------------------------------------------

void CGhost::Update(float _dTime)
{


	if(m_bAlive == false) //[2014.11.17 ]
		return;

	_OnUpdateStart(_dTime);//[2014.11.30 ]

	if(m_GhostData.HP<=0)
	{		m_eActState = eGAS_DIE;}  // HP��0�̸� �����ɷ�!!! [2014.11.30 ]
	
	

	if(m_DamageTimer>0)
	{	m_DamageTimer -= _dTime;	}//[2014.11.30 ]

	D3DXVECTOR3 tempVec;
	float TempY = 0.0f; //[2014.11.19 ]
	//_OnUpdateStart(_dTime);

	////////////////////////////////////////////////////////////////////////////
	// �н�����Ʈ �� ���� �������� �ٲ��ֱ�  //[2014.11.18 ]

	m_Vec_DestVector = m_GhostData.Vec_PassPointArr[m_CurPassPoint] - m_Pos;
	
	// �н�����Ʈ�� �Ÿ�üũ�� ����  [2014.11.19 ]
	float TempDist = D3DXVec3Length(&m_Vec_DestVector);
	D3DXVec3Normalize(&tempVec, &m_Vec_DestVector);
	TempY = tempVec.y; //  �̸� ���������� y���� ������ ���´�. //[2014.11.19 ]
	m_Vec_DestVector.y = 0.0f; //���͸� 2�������� �����.  [2014.11.19 ]

	// �Ÿ��� ���������Ϸ� �Ǹ� �н�����Ʈ �ٲٱ�   [2014.11.18 ]
	if(TempDist < 13.1f)
	{	++m_CurPassPoint;	}



	// ������ �н�����Ʈ(ħ��) ��� �����ɷ� ���� [2014.11.18 ]
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

	// �ü����͵� ������ ���� ��ŭ ȸ�������ش�. [2014.11.13 ]
	D3DXVec3TransformNormal(&m_VecSee , &m_Vec_StartSee, &m_MRot);

	//////////////////////////////////////////////////////////////////////////
	// ���⼭���� DestVector�� �������� ȸ����� ����ϱ�

	// �κ��ͻ����� ���� ���ϱ�
	D3DXVec3Normalize(&m_Vec_DestVector , &m_Vec_DestVector);
	D3DXVec3Normalize(&m_VecSee, &m_VecSee);
	float TempDot = D3DXVec3Dot(&m_Vec_DestVector , &m_VecSee);


	if (TempDot < 0.94f) // �������� �������� ȸ�� [2014.11.13 ]
	{		
			if(IsLeft_Vec_Vec_3D(m_VecSee, m_Vec_DestVector) == true)
			{ 		m_Angle_Yaw += -m_GhostData.RotSpeed *_dTime;			}
			else
			{		m_Angle_Yaw += m_GhostData.RotSpeed *_dTime;			}			
	}
	else//TempDot > 0.99f �̾ �κ��Ͱ� �����ϴٰ� �Ǵܵɶ��� ������ ���͸� ��ġ��Ų��.
	{		m_VecSee = m_Vec_DestVector;	}
	
	if((m_eActState == eGAS_MOVING) || (m_eActState == eGAS_ATTACK))
	{
		// ������ �ü����� �������� �̵� 
		tempVec = m_VecSee +  D3DXVECTOR3(0.0f, TempY , 0.0f );
		m_Pos += tempVec * m_GhostData.MoveSpeed * _dTime;// ������ �̵��ӵ��� �ü����Ϳ� ���� ��ġ�� �����ش�.[2014.11.15 ]
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

	m_MWorld = m_MRot * m_MTrans;//������� ���
		
	if(m_GhostData.GhostType == 1)
	{
		m_RscPack.Update(_dTime, &m_MWorld);
		// ħ��� ��������� ���� �ִ� ���ϱ�
		if((m_CurPassPoint == m_GhostData.PassPointCnt-1 )&& (TempDist < 20.1f))
		{	m_RscPack.GetAnimator().Change_Ani_Clip("ATTACK");		}

	} //[2014.11.29 ]
	else if(m_GhostData.GhostType == 2)
	{
		m_RscPack2.Update(_dTime, &m_MWorld);
		// ħ��� ��������� ���� �ִ� ���ϱ�
		if((m_CurPassPoint == m_GhostData.PassPointCnt-1 )&& (TempDist < 23.1f))
		{	m_RscPack2.GetAnimator().Change_Ani_Clip("ATTACK");		}
	}//[2014.11.29 ]
	else 
	{	
		m_RscPack3.Update(_dTime, &m_MWorld);	
		// ħ��� ��������� ���� �ִ� ���ϱ�
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
	// ���⼭ �浹ó�� 
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
void CGhost::Rot_Pitch(float _Angle)// ���⺤�͸� �������� ȸ��
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

void CGhost::Rot_Roll(float _Angle)//���溤�� �������� ȸ��
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
void CGhost::Rot_Yaw(float _Angle)//���⺤�ͱ������� ȸ��
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
void	CGhost::_OnDrawStart()
{
	// ���̴� ��� ����
	{

		// ���ũ���� ������ �����Ҷ� �ݵ�� ���� �ؼ� �ڵ��� ������ ���´�.[2014.10.29 ]
// 		if(SHG_Input.key(DIK_P))
// 			m_hTech = G_FXShader.GetPointer()->GetTechniqueByName("Skinning_DirLight_SpecularMap");
// 		else
// 			m_hTech = G_FXShader.GetPointer()->GetTechniqueByName("Skinning_DirLight");
		// 1. ����Ʈ ��ü �������� [2014.10.29 ]
		LPD3DXEFFECT pEff = G_FXShader.GetPointer();                             

// 		CCamera* pThisSceneCamera = SHGlobal_GetCurScene_Camera;
// 		D3DXMATRIX tempViewProjTM; //
// 		tempViewProjTM = pThisSceneCamera->Get_ViewProjMat();
// 		// viewPorj TM �� �ڵ�� ���̴��� ���� [2014.10.27 ]
// 		pEff->SetMatrix(G_ConstantTable.h_ViewProjTM , &tempViewProjTM);

		//�ܰ��� Į��  [2014.11.13 ]
// 		pEff->SetVector(G_ConstantTable.h_LineColor , (D3DXVECTOR4*)&G_LightDiff);
// 		 //�ܰ��� �α� [2014.11.13 ]
// 		pEff->SetFloat(G_ConstantTable.h_LineOffset , 0.2f);		

		//����Ʈ����Ʈ ��ġ   �̰� �������ʿ���  //[2014.11.15 ]
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

		if(m_hTech == nullptr )  // ��ũ �ڵ� Ȯ���غ��� 
		{		assert(false);	} 

		G_FXShader.m_pShader->SetFloat(G_ConstantTable.h_UVAni_Val , 0.0f); 
	//	G_FXShader.GetPointer()->SetTechnique(m_hTech);
	}
}

//-----------------------------------------------------------------------------
// Desc   :  ��� ���Ͽ��� �׵𺣾� �Ӽ������� �ҷ��´�. 
//-----------------------------------------------------------------------------
int			CGhost::Load_AttrInfo_from_Lua(const CHAR* _pPath)
{


	SH_Lua TempLua(_pPath); //(SH_Lua)�� �ݵ�� ���������� ����!!
	//int tableSize; 

	try
	{
		// ��� �������� Tedy_MoveSpeed �� �޾ƿ´�. 
		//TempLua.GetGlobal_float("Tedy_MoveSpeed" ,m_GhostData.MoveSpeed);
		// ��� �������� Tedy_RotSpeed �� �޾ƿ´�.
		//TempLua.GetGlobal_float("Tedy_RotSpeed" , m_GhostData.RotSpeed);
	
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
bool CGhost::MsgProc( SH_MSG* _pMessage )
{
	static int toggle = true;

	//////////////////////////////////////////////////////////////////////////
	//�޼��� Ÿ���� UI ��� 
	if(_pMessage->GetMsgType() == eMsgType_UI)  
	{
		SH_MSG_from_UI* pUIMsg = static_cast<SH_MSG_from_UI*>(_pMessage); // ��������ȯ [2014.11.7 ]
		switch( pUIMsg->m_UI_id) // �������� �������̵�� � �����̿��� �°��� �˻� [2014.11.7 ]
		{
		case eUI_ID_StartButton:// �޼�����  ��۹�ư���� �Դٸ� [2014.11.7 ]
			{
				switch(pUIMsg->m_WinMsg)
				{
				case WM_LBUTTONUP:  //��۹�ư���� �� �޼����� WM_LBUTTONUP �̶�� 
					{
						toggle ^=1;
						if(toggle == 1)
						{	// ������̺� ����� ��ũ�� �ڵ�� ���� ��ũ���� �ٲ��ش�.
							m_hTech = G_ConstantTable.h_Tech_Skin_SpecularMap_DirLight;
						}
						else
						{	// ������̺� ����� ��ũ�� �ڵ�� ���� ��ũ���� �ٲ��ش�.
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
	//�޼��� Ÿ���� �Ϲݸ޼������
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
// ���⼭���� CGhost_DataBase �Լ�����
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
// Arg    : ���ϰ�ε�
// Return : HRESULT S_OK, S_FALSE  
// Desc   : ��ƽ�ũ��Ʈ�� �����κ��� ��Ʈ �����ͺ��̽��� �����. //[2014.11.16 ]
//-----------------------------------------------------------------------------
int		CGhost_DataBase::Load_From_Lua(const CHAR* pPath_GhostAttr, const CHAR* pPath_GhostClipInfo,
									   const CHAR* pPath_Ghost_SpawnPoint)
{
	//////////////////////////////////////////////////////////////////////////
	// ù��°�� Ghost attr�� �о���δ�. [2014.11.16 ]
	{						
		SH_Lua TempLua(pPath_GhostAttr); //(SH_Lua)�� �ݵ�� ���������� ����!! [2014.11.16 ]
		//int tableSize; 
		try
		{
			// ��� �������� Ghost_LV1_MoveSpeed �� �޾ƿ´�. 
			TempLua.GetGlobal_float("Ghost_LV1_MoveSpeed" , m_GhostAttr_LV[0].MoveSpeed);
			// ��� �������� Ghost_LV1_RotSpeed �� �޾ƿ´�
			TempLua.GetGlobal_float("Ghost_LV1_RotSpeed" , m_GhostAttr_LV[0].RotSpeed);
			// ��� �������� Ghost_LV1_HP �� �޾ƿ´�. 
			TempLua.GetGlobal_int("Ghost_LV1_HP" , m_GhostAttr_LV[0].HP);
			// ��� �������� Ghost_LV1_HP �� �޾ƿ´�
			TempLua.GetGlobal_int("Ghost_LV1_ATTACK" , m_GhostAttr_LV[0].Attack);

			// ��� �������� Ghost_LV1_MoveSpeed �� �޾ƿ´�. 
			TempLua.GetGlobal_float("Ghost_LV2_MoveSpeed" , m_GhostAttr_LV[1].MoveSpeed);
			// ��� �������� Ghost_LV1_RotSpeed �� �޾ƿ´�
			TempLua.GetGlobal_float("Ghost_LV2_RotSpeed" , m_GhostAttr_LV[1].RotSpeed);
			// ��� �������� Ghost_LV1_HP �� �޾ƿ´�. 
			TempLua.GetGlobal_int("Ghost_LV2_HP" , m_GhostAttr_LV[1].HP);
			// ��� �������� Ghost_LV1_HP �� �޾ƿ´�.
			TempLua.GetGlobal_int("Ghost_LV2_ATTACK" , m_GhostAttr_LV[1].Attack);

			// ��� �������� Ghost_LV1_MoveSpeed �� �޾ƿ´�. 
			TempLua.GetGlobal_float("Ghost_LV3_MoveSpeed" , m_GhostAttr_LV[2].MoveSpeed);
			// ��� �������� Ghost_LV1_RotSpeed �� �޾ƿ´�.
			TempLua.GetGlobal_float("Ghost_LV3_RotSpeed" , m_GhostAttr_LV[2].RotSpeed);
			// ��� �������� Ghost_LV1_HP �� �޾ƿ´�.
			TempLua.GetGlobal_int("Ghost_LV3_HP" , m_GhostAttr_LV[2].HP);
			// ��� �������� Ghost_LV1_HP �� �޾ƿ´�
			TempLua.GetGlobal_int("Ghost_LV3_ATTACK" , m_GhostAttr_LV[2].Attack);

		}
		catch(...)
		{
			SHUtil::MsgBox("CAnimator::Load_ClipInfo_from_Lua() , ��ƿ���ó��!!");
			return S_FALSE;
		}																												
	}

	//////////////////////////////////////////////////////////////////////////
	// �ι����� GhostClipInfo�� �о���δ�. [2014.11.16 ]
	{/*
		SH_Lua TempLua(pPath_GhostClipInfo); //(SH_Lua)�� �ݵ�� ���������� ����!! [2014.11.16 ]
		int tableSize; 
		try
		{
			TempLua.GetGlobal_int("Clip_Info_Size" , tableSize);

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
				CString tempName = clipName;
				m_Animator.AddAniClip(tempName, STime, ETime);
			}

		}
		catch(...)
		{
			SHUtil::MsgBox("CAnimator::Load_ClipInfo_from_Lua() , ��ƿ���ó��!!");
			return S_FALSE;
		}	
		*/
	}

	//////////////////////////////////////////////////////////////////////////
	// ����°�� GhostClipInfo�� �о���δ�. [2014.11.16 ]
	{
		SH_Lua TempLua(pPath_Ghost_SpawnPoint); //(SH_Lua)�� �ݵ�� ���������� ����!! [2014.11.16 ]
	
		lua_getglobal(TempLua.GetLuaState(), "Respawn_Point_1");


		//////////////////////////////////////////////////////////////////////////
		// ������ ����Ʈ 1��
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
		SH_Lua TempLua(pPath_Ghost_SpawnPoint); //(SH_Lua)�� �ݵ�� ���������� ����!! [2014.11.16 ]
		lua_getglobal(TempLua.GetLuaState(), "Respawn_Point_2");

		//////////////////////////////////////////////////////////////////////////
		//	������ ����Ʈ 2��
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
		SH_Lua TempLua(pPath_Ghost_SpawnPoint); //(SH_Lua)�� �ݵ�� ���������� ����!! [2014.11.16 ]
		lua_getglobal(TempLua.GetLuaState(), "Respawn_Point_3");
		//////////////////////////////////////////////////////////////////////////
		//	������ ����Ʈ 3��
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
		SH_Lua TempLua(pPath_Ghost_SpawnPoint); //(SH_Lua)�� �ݵ�� ���������� ����!! [2014.11.16 ]
		lua_getglobal(TempLua.GetLuaState(), "Respawn_Point_4");
		//////////////////////////////////////////////////////////////////////////
		//	������ ����Ʈ 3��
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

		SH_Lua TempLua(pPath_Ghost_SpawnPoint); //(SH_Lua)�� �ݵ�� ���������� ����!! [2014.11.16 ]
		
		lua_getglobal(TempLua.GetLuaState(), "PassPointArray_1");//[2014.12.2 ]

		/////////////////////////////////////////////////////////////////////////////////
		// ������� �迭 ,  ������ġ 1���� ��Ÿ���� ������ �������[2014.11.18 ]

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
		// ������� �迭 ,  ������ġ 2���� ��Ÿ���� ������ �������[2014.11.18 ]
		{
			SH_Lua TempLua(pPath_Ghost_SpawnPoint); //(SH_Lua)�� �ݵ�� ���������� ����!! [2014.11.16 ]
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
		// ������� �迭 ,  ������ġ 3���� ��Ÿ���� ������ �������[2014.11.18 ]
			SH_Lua TempLua(pPath_Ghost_SpawnPoint); //(SH_Lua)�� �ݵ�� ���������� ����!! [2014.11.16 ]
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

		SH_Lua TempLua(pPath_Ghost_SpawnPoint); //(SH_Lua)�� �ݵ�� ���������� ����!! [2014.11.16 ]
		/////////////////////////////////////////////////////////////////////////////////
		// ������� �迭 ,  ������ġ 4���� ��Ÿ���� ������ �������[2014.11.18 ]
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
// Return : bool ������ ������ true,, �ƴϸ� false 
// Desc   :  �Ѵ� �¾����� HP�� �ް��ϴ� �Լ�
//-----------------------------------------------------------------------------
bool		CGhost::DamagedFromHit(int _Damage)//[2014.11.30 ]
{

	if(m_DamageTimer >0) return false;// ���������� �� �ȵǾ��ٸ� 
	
	G_SHSoundMgr.PlayWave(eSound_Enemy_Hit, false);

	m_DamageTimer = 0.7f; // ������ 0.7�ʵ��� ���� �ʰ� �ϱ�

	m_GhostData.HP -= _Damage; //[2014.11.30 ]

	m_RscPack2.GetAnimator().Change_Ani_Clip("BE_ATTACKED");//[2014.11.30 ]
	m_RscPack3.GetAnimator().Change_Ani_Clip("BE_ATTACKED");//[2014.11.30 ]
	m_eActState = eGAS_HITED; // FSM ���¹ٲٱ�

	

	return true;


}
/*************************************************** end of this file **************************************************************/