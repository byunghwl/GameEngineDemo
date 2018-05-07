
#include "DXUT.H"
#include "GameObject.h"
#include "SkillCard.h"

///////////////////////////////////////////////////////////////////////////
// �������� ����
CCardSkill*				G_pCard = nullptr; // [2014.11.27 ]

//////////////////////////////////////////////////////////////////////////
// ����ƽ ���� �ʱ�ȭ
CString		CCardSkill::m_EntityType = "CardSkill"; // ��ƼƼ�� Ÿ�� �̸�


D3DXVECTOR3 G_UPVec = D3DXVECTOR3(0,1,0);

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
//���⼭���� CCardSkill �Լ�����
// ������
CCardSkill::CCardSkill() //:  m_pMesh(nullptr)
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

	// ���� ī�尡�� �ٶ󺸰��ִ¹���  // [2014.11.27 ]
	m_VecSee = D3DXVECTOR3(0.0f, 0.0f, -1.0f); // [2014.11.27 ]
	m_Vec_DestVector = m_VecSee; /// [2014.11.27 ]
	m_Angle_Yaw = 0.0f;  // up vector�� �������� ȸ���� ���� // [2014.11.27 ]

	
	m_CardData.MoveSpeed = 300.0f;  	// [2014.11.27 ]
	
	m_Vec_StartSee = D3DXVECTOR3(0,0, -1);

	m_pBoundSphere_Body = nullptr;

	m_bAlive = false;// [2014.11.27 ]

	m_DelayTime = 0.0f ;// [2014.11.27 ]

	m_b_is_TimerOn = false; // [2014.11.27 ]
	m_AccuTime = 0.0f; //�����ð�  // [2014.11.28 ]
	m_CoolTime = 0.0f;// [2014.11.28 ]
}

CCardSkill::~CCardSkill()
{
	write_log_file("�޸� ���� : ~CCardSkill() ȣ��");


}



//-------------------------------------------------------------------------
// Arg	  : D3DXVECTOR3 _SPos	 : ������ġ (�⺻�� D3DXVECTOR3(0,0,200))
//			CARD_DATA* pMData : ���� �Ӽ� �����͸� �������ִ� ����ü
// Return : ����(S_OK), ����(S_FALSE)
// Desc   : CCardSkill Ŭ������ �ݵ�� ���Լ��� �ʱ�ȭ���� ����Ѵ�. [2014.10.7 ]
//-------------------------------------------------------------------------

int  CCardSkill::Init(LPDIRECT3DDEVICE9 _pDevice, D3DXVECTOR3 _SPos /*= D3DXVECTOR3(0,0,200)*/, CARD_DATA* pBData )
{
	HRESULT RET = S_OK;
	if(FAILED(CEntity::Init(_pDevice)))		
	{
		return FALSE;
	}

	if(pBData != nullptr)
	{
		memcpy(&m_CardData, pBData, sizeof(CARD_DATA));// [2014.10.7 ]
	}

	m_VecRight = D3DXVECTOR3(1,0,0);
	m_VecSee   = D3DXVECTOR3(0,0,-1); 
	m_VecUp    = D3DXVECTOR3(0,1,0);

	m_Pos = _SPos;
	
	m_CardDataBk = m_CardData;

	return RET;

}

bool	CCardSkill::isReady()
{
	if(m_CoolTime > m_CardData.CoolTime)
		return true;
	else
		return false;
}//[2014.11.28 ]


//-------------------------------------------------------------------------
// Arg	  : 
// Return : HRESULT S_OK , S_FALSE
// Desc   : �̰����� �ݶ��̴��� �����ؾ���(�ٿ�彺�Ǿ��)//[2014.11.11 ]
//-------------------------------------------------------------------------
// from ICollidable
HRESULT		CCardSkill::InitCollider() 
{
	HRESULT hr = S_OK;

	m_pCollidObj = new CBoundSphere();

	return hr;
}

//-------------------------------------------------------------------------
// �׸��� �Լ�
//-------------------------------------------------------------------------

void CCardSkill::Draw()
{	
	if(m_bAlive == false) //[2014.11.17 ]
		return;

	//���̴���� ����
	_OnDrawStart();

	m_RscPack.Draw(m_pDevice);
//	int size =m_RscPack.m_MeshList.m_Vec_MeshList.size();
	/*LOG_WRITE_A(" ��Ʈ �޽�����Ʈ ���� !!!!!!!!! %d",  size);*/


}


//-----------------------------------------------------------------------------
// Desc   : ������Ʈ �ϱ� ���� ȣ��Ǵ� �Լ�
//-----------------------------------------------------------------------------
void CCardSkill::_OnUpdateStart(float _dTime)
{
	



	

}

//-------------------------------------------------------------------------
// ������Ʈ �Լ�
//-------------------------------------------------------------------------

void CCardSkill::Update(float _dTime)
{
	//m_AccuTime = 0.0f; //�����ð� 

	if(m_CoolTime < m_CardData.CoolTime)
	m_CoolTime += _dTime;

	if(m_b_is_TimerOn == true)
	{
		m_AccuTime += SHGlobal_GetEngineTime * 1000.0f ; //[2014.11.27 ]
	}

	if(m_DelayTime < m_AccuTime) // �����ð��� �����̽ð� �̻��� �Ǹ�..
	{
		m_b_is_TimerOn = false; // �ٽ� Ÿ�̸Ӹ� [2014.11.27 ]

		m_bAlive = true;
		m_AccuTime = 0.0f;
	}

	if(m_bAlive == false) //[2014.11.17 ]
		return;

	//////////////////////////////////////////////////////////////////////////
	
	D3DXVec3Normalize(&m_VecSee , &m_VecSee);

	// �̵��� ���
	m_Pos += m_VecSee * _dTime * m_CardData.MoveSpeed; // [2014.11.27 ]

	// ȸ����� ���
	m_MRot = m_CharacterWorldTM;
	m_MRot._41 = 0.0f; m_MRot._42 = 0.0f; m_MRot._43 = 0.0f; m_MRot._41 = 1.0f;

	D3DXMatrixIdentity(&m_MTrans);
	D3DXMatrixTranslation(&m_MTrans, m_Pos.x , m_Pos.y , m_Pos.z);

	m_MWorld = m_MRot * m_MTrans;//������� ���

	m_RscPack.Update(_dTime, &m_MWorld);
	m_pBoundSphere_Body->SetCenter(m_Pos);
	//////////////////////////////////////////////////////////////////////////
	//	��Ʈ��� �浹üũ !!!!!!!!!!!!!!!!!!!!!!    [2014.11.26 ]
	for(int i = 0 ; i< GHOSTNUM_MAX ; i++)
	{
		if(G_pGhost_Arr[i]->m_bAlive == true)
		{
			if(G_pGhost_Arr[i]->m_pBoundSphere_Body->Colli_Check(m_pBoundSphere_Body) == true)
			{
				G_pGhost_Arr[i]->DamagedFromHit(1);//HP �� 1��ŭ �ް��ϱ�
				m_bAlive = false;
			}
		}
	}


	//////////////////////////////////////////////////////////////////////////
	// �����Ÿ� �ٵǸ� ������� �ϱ� //[2014.11.27 ]
	{
		D3DXVECTOR3 TempVec = m_Pos - m_StartPos;
		float Dist = D3DXVec3Length(&TempVec);
		if(Dist > m_CardData.Range)
		{
			m_bAlive = false;
		}
	}
	
}




//-----------------------------------------------------------------------------
// Desc   : draw �ϱ� ���� ȣ��Ǵ� �Լ�
//-----------------------------------------------------------------------------
void	CCardSkill::_OnDrawStart()
{
	// ���̴� ��� ����
	{

		// ���ũ���� ������ �����Ҷ� �ݵ�� ���� �ؼ� �ڵ��� ������ ���´�.[2014.10.29 ]
// 		if(SHG_Input.key(DIK_P))
// 			m_hTech = G_FXShader.GetPointer()->GetTechniqueByName("Skinning_DirLight_SpecularMap");
// 		else
// 			m_hTech = G_FXShader.GetPointer()->GetTechniqueByName("Skinning_DirLight");
		// 1. ����Ʈ ��ü �������� [2014.10.29 ]
	//	LPD3DXEFFECT pEff = G_FXShader.GetPointer();                             

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


// 		if(m_hTech == nullptr )  // ��ũ �ڵ� Ȯ���غ��� [2014.10.30 ]
// 		{		assert(false);	} // [2014.10.30 ]

	//	G_FXShader.GetPointer()->SetTechnique(m_hTech);// [2014.10.30 ]
	}
}


//-----------------------------------------------------------------------------
// Arg    : D3DXVECTOR3 _StartPos, D3DXMATRIX _CharacterWorldTM , int DelayFrame 
// Return :
// Desc   : ī�彺ų �ߵ�!! DelayFrame ��ŭ ��ٷȴٰ� �ߵ��ȴ�.
//			_StartPos �ߵ��Ǿ����� ������ġ 
//			_CharacterWorldTM �̽�ų�� ����� ������ ĳ���� ����TM// [2014.11.27 ]
//-----------------------------------------------------------------------------
void	    CCardSkill::SetDelayTime_for_Begin(D3DXVECTOR3 _StartPos, D3DXVECTOR3 _SeeVec,
												D3DXMATRIX _CharacterWorldTM , int DelayFrame, const SHR_Mesh* _pRefMesh /*= nullptr*/) // [2014.11.27 ]
{
	m_DelayTime = DelayFrame*30; //SHGlobal_GetEngineTime * 1000.0f * m_Velo; 
	m_Pos = _StartPos;
	m_StartPos = _StartPos;
	m_CharacterWorldTM = _CharacterWorldTM;

	m_b_is_TimerOn = true; // [2014.11.27 ]
	m_VecSee =_SeeVec;// ��ġ [2014.11.27 ]
	m_CoolTime = 0.0f;
}


//-----------------------------------------------------------------------------
// Desc   :  ��� ���Ͽ��� �׵𺣾� �Ӽ������� �ҷ��´�. // [2014.11.14 ]
//-----------------------------------------------------------------------------
int			CCardSkill::Load_AttrInfo_from_Lua(const CHAR* _pPath)
{


	SH_Lua TempLua(_pPath); //(SH_Lua)�� �ݵ�� ���������� ����!!
	//int tableSize; 
	// [2014.10.20 ]
	try
	{
		// ��� �������� Tedy_MoveSpeed �� �޾ƿ´�. // [2014.11.14 ]
		TempLua.GetGlobal_float("Card_MoveSpeed" ,m_CardData.MoveSpeed);
		TempLua.GetGlobal_float("Card_Range" ,m_CardData.Range);
		TempLua.GetGlobal_float("Card_Cooltime" ,m_CardData.CoolTime); //[2014.11.28 ]
		m_CoolTime = m_CardData.CoolTime+1;
		// ��� �������� Tedy_RotSpeed �� �޾ƿ´�. // [2014.11.14 ]
		//TempLua.GetGlobal_float("Tedy_RotSpeed" , m_CardData.RotSpeed);
	
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
bool CCardSkill::MsgProc( SH_MSG* _pMessage )
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
				case WM_LBUTTONUP:  //��۹�ư���� �� �޼����� WM_LBUTTONUP �̶�� [2014.11.7 ]
					{
						toggle ^=1;
						if(toggle == 1)
						{	// ������̺� ����� ��ũ�� �ڵ�� ���� ��ũ���� �ٲ��ش�.
							m_hTech = G_ConstantTable.h_Tech_Skin_SpecularMap_DirLight;// [2014.11.6 ]
						}
						else
						{	// ������̺� ����� ��ũ�� �ڵ�� ���� ��ũ���� �ٲ��ش�.
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
	//�޼��� Ÿ���� �Ϲݸ޼������
	if(_pMessage->GetMsgType() == eMsgType_NORMAL)  //  
	{





	}



	return true;
}
