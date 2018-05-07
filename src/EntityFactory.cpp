#include "DXUT.h"
#include "EntityFactory.h"





///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//
// ���⼭ ���� CEntityFactory class �Լ� ����
//
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// �������� �ʱ�ȭ
CEntityFactory*		CEntityFactory::pInstance = nullptr;  // �̱���

//-------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------
CEntityFactory::CEntityFactory()
{
}


//-------------------------------------------------------------------------
// �Ҹ���
//-------------------------------------------------------------------------
CEntityFactory::~CEntityFactory()
{
	// �������� ������ �޸� ����
	std::map<CString, IEntityCreator*>::iterator Iter_Pos;
	
	for( Iter_Pos = m_Map.begin(); Iter_Pos != m_Map.end(); ++Iter_Pos)
	{
		IEntityCreator* ptemp = Iter_Pos->second;
		delete ptemp;
		m_Map.erase(Iter_Pos);
	}
}


//-------------------------------------------------------------------------
// Arg	  : const CHAR* _pCreatorName (ũ�������� Ŭ���� ����), 
//			const CHAR* _RscPackInfoName (.rpi Ȯ���ڸ����� ���� �̸�)
//			EntityFactory_Arg &_EFArg (��������ڰ� �Ѱ��ְ� ���� ������ ��Ƴ��� ����ü)
// Return : ���� (S_OK), ����(S_FALSE)
// Desc   : _pCreatorName �� Ű������ �ش� ũ�������͸� ã�� ��ƼƼ�� �����Ѵ�.
//-------------------------------------------------------------------------
CGameObject*	CEntityFactory::CreateEntity(CString _pCreatorName, const WCHAR* _strRscPackInfo , EntityFactory_Arg &_EFArg)
{
	CGameObject* pObject;
	std::map<CString, IEntityCreator*>::iterator Iter_Pos;
	
	Iter_Pos =  m_Map.find(_pCreatorName);// �ش��ϴ� Creator�� �̸����� ã�´�. 

	 if(Iter_Pos != m_Map.end()) // ã�Ҵٸ�
	 {	pObject = Iter_Pos->second->CreateEntity(_strRscPackInfo, _EFArg);  }
	 else						// ã�� ���ߴٸ�
	 {	
		 pObject = nullptr;  LOG_WRITE_A("CEntityFactory::CreateEntity() , nullptr ��ȯ, �̸��� ��ã�Ҿ�!!!!!!!!!!!!!!!!!!"); assert(false);
	 }


	 return pObject;
}



//-----------------------------------------------------------------------------
// Arg    : CString _CreatorType(Ÿ�� �̸�),  IEntityCreator* _pNewCreator(IEntityCreator�� ��ӹ��� Ŭ����)
// Return : ���н� S_FALSE  , ������ S_OK
// Desc   : IEntityCreator �� ��ӹ��� Creator�� ����Ѵ�. �̹� ��ϵǾ��ִ� �̸��̸� ���и� ��ȯ�Ѵ�.
//			IEntityCreator* _pNewCreator �� new�� �����ؾ���
//-----------------------------------------------------------------------------
int			CEntityFactory::Register(CString _CreatorType, IEntityCreator* _pNewCreator)
{
	if(_pNewCreator == nullptr)
	{
		SHUtil::MsgBox("CEntityFactory::Register() , nullptr ����");
		assert(false);
		return S_FALSE;
	}

	std::map<CString, IEntityCreator*>::iterator Iter_Pos;

	Iter_Pos = m_Map.find(_CreatorType);

	if(Iter_Pos != m_Map.end()) //  ã�Ҵٸ�
	{
		return S_FALSE; //�����̸��� �����Ƿ� S_FALSE�� ����
	}

	
	m_Map.insert(std::pair<CString, IEntityCreator*>(_CreatorType, _pNewCreator));

	return S_OK;
}


//-----------------------------------------------------------------------------
// �̱��水ü ����
//-----------------------------------------------------------------------------
CEntityFactory*		CEntityFactory::GetInstance()
{
	if( nullptr == pInstance  )
	{
		pInstance = new CEntityFactory();

	}
	return pInstance;
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//
// ���⼭ ���� CBearCreator class �Լ� ����
//
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------
CBearCreator::CBearCreator()
{
	
}

//-------------------------------------------------------------------------
// �Ҹ���
//-------------------------------------------------------------------------
CBearCreator::~CBearCreator()
{ 
	
}


//-------------------------------------------------------------------------
// Arg	  : const WCHAR* _pRscPackName(.rpi �̸�)
// Return : (����)CGameObject* , (����)nullptr
// Desc   : CBear ��ü�� �̰����� �����ؼ� �����͸� ��ȯ�Ѵ�.
//-------------------------------------------------------------------------
CGameObject*	CBearCreator::CreateEntity(const WCHAR* _pRscPackName, EntityFactory_Arg &_EFArg)
{
	int res = S_OK;
	CBear* pBear = new CBear();

	res = pBear->GetRscPack()->Build(_pRscPackName);
	res = pBear->Init(SHGlobal_GetD3DDevice, _EFArg.Pos);// CBear �ʱ�ȭ �Լ�

	pBear->GetRscPack()->GetBoneList()->GetAniTimer().SetAniTime(_EFArg.starttick); 
	pBear->GetRscPack()->GetAnimator().SetAniTime( _EFArg.starttick);
	pBear->GetRscPack()->GetAnimator().Load_ClipInfo_from_Lua( _EFArg.KeyInfoPath);

	//////////////////////////////////////////////////////////////////////////
	// ���⼭ ���� ��������� ��ų���� �����ϱ�
	
	pBear->m_SkillCard =  (*G_pScatter); //  ��������� �����ϱ�
	pBear->m_SkillHat = (*G_pHat); 
	pBear->m_SkillRockPunch = (*G_pRockPunch);
	
	//////////////////////////////////////////////////////////////////////////
	// ���⼭���� �ݸ��� ���� �������ֱ�  [2014.11.28 ]
	{
		SHR_MeshList& ML = pBear->m_SkillHat.GetRscPack()->GetMeshList();
		SHR_Mesh* pTempMesh =   ML.FindMeshByName("Hat"); // �ٿ�����Ǿ� �ٵ�����
		pBear->m_SkillHat.m_pHatMesh = pTempMesh; //

		SHR_BoneList* pBL = pBear->m_SkillHat.GetRscPack()->GetBoneList();
		pTempMesh =	pBL->FindMeshByName("BoundSphere_Body");

		pBear->m_SkillHat.m_pBoundSphere_Body = new CBoundSphere();
		pBear->m_SkillHat.m_pBoundSphere_Body->SetRadius( 60.0f);

	}
	//////////////////////////////////////////////////////////////////////////
	// ���⼭���� �ݸ��� ���� �������ֱ�  

		SHR_BoneList* pBL = pBear->GetRscPack()->GetBoneList();

		const SHR_Mesh* pTempMesh = pBL->FindMeshByName("BoundSphere_Body"); // �ٿ�����Ǿ� �ٵ�����  
		pBear->m_pBoundSphere_Body = new CBoundSphere();
		pBear->m_pBoundSphere_Body->SetRadius( pTempMesh->m_MeshInfo.BoundShpere_Radius);

		pTempMesh =  pBL->FindMeshByName("BoundSphere_Stick"); //�ٿ�����Ǿ� ��ƽ����
		pBear->m_BSMesh_Stick = pTempMesh; 
		pBear->m_pBoundSphere_Stick = new CBoundSphere();
		pBear->m_pBoundSphere_Stick->SetRadius( pTempMesh->m_MeshInfo.BoundShpere_Radius+10);

		pTempMesh =  pBL->FindMeshByName("Bone_hat"); //���� ����޽� ������ //
		pBear->m_BoneMesh_Hat = pTempMesh; 

	}
	//////////////////////////////////////////////////////////////////////////
	// ���⼭���� �׵𺣾� �Ӽ����� ��ƽ�ũ��Ʈ���� �о���̱�
	pBear->Load_AttrInfo_from_Lua( _EFArg.AttrInfoPath);
	pBear->GetRscPack()->GetAnimator().Change_Ani_Clip("IDLE");

	//////////////////////////////////////////////////////////////////////////
	// ���⼭���� ���̴� ��ũ �������ֱ�
	pBear->GetRscPack()->m_pSkinMesh->Set_m_hTech("Outline_SPNT_POINT_NOSPECULAR");
	//pBear->GetRscPack()->m_pSkinMesh->Set_m_hTech("Skinning_PointLight");
	pBear->GetRscPack()->m_MeshList.Set_hTech_AllMesh("Rigid_Tex");


	//////////////////////////////////////////////////////////////////////////
	// ������ ����Ʈ  // [2014.12.3 ]
	pBear->m_pEffMagicStick = new cSHBillBoard();
	pBear->m_pEffMagicStick->Init(SHGlobal_GetD3DDevice, L"./image/weapon.tga", 20.0f, 8.0f);
	CSceneMgr::getInstance()->GetSceneByName(L"Scene_GameMain")->AddObject(pBear->m_pEffMagicStick); //
	pBear->m_pEffMagicStick->Set_m_bVisible( true);

		if(res == S_FALSE)// ���н� nullptr ��ȯ
		return nullptr;

	return pBear;
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//
// ���⼭ ���� CGhost_Creator class �Լ� ����
//
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------
CGhost_Creator::CGhost_Creator()
{
}

//-------------------------------------------------------------------------
// �Ҹ���
//-------------------------------------------------------------------------
CGhost_Creator::~CGhost_Creator()
{
}

//-------------------------------------------------------------------------
// Arg	  : const WCHAR* _pRscPackName(.rpi �̸�)
// Return : (����)CGameObject* , (����)nullptr
// Desc   : CBear ��ü�� �̰����� �����ؼ� �����͸� ��ȯ�Ѵ�.
//-------------------------------------------------------------------------
CGameObject*	CGhost_Creator::CreateEntity(const WCHAR* _pRscPackName, EntityFactory_Arg &_EFArg)
{
	int res = S_OK;
	CGhost* pGhost = new CGhost();
	res = pGhost->Init(SHGlobal_GetD3DDevice, _EFArg.Pos);// CBear �ʱ�ȭ �Լ�
	// Ghost1�� ���ҽ��� ����� //[2014.11.29 ]
	{
		res = pGhost->GetRscPack()->Build(_pRscPackName);
		pGhost->GetRscPack()->GetBoneList()->GetAniTimer().SetAniTime(_EFArg.starttick); 
		pGhost->GetRscPack()->GetAnimator().SetAniTime( _EFArg.starttick); 
		pGhost->GetRscPack()->GetAnimator().Load_ClipInfo_from_Lua( _EFArg.KeyInfoPath);
		pGhost->GetRscPack()->GetAnimator().Change_Ani_Clip("IDLE");
		pGhost->m_eActState = eGAS_MOVING;
		//pGhost->GetRscPack()->m_pSkinMesh->Set_m_hTech("Skinning_PointLight"); 
		pGhost->GetRscPack()->m_pSkinMesh->Set_m_hTech("SPNT_POINT_NOSPECULAR_withLim_FOG"); 
		pGhost->GetRscPack()->m_MeshList.Set_hTech_AllMesh("Rigid_Tex_WithFOG");
	}

	// Ghost2�� ���ҽ��� ����� //[2014.11.29 ]
	{
		res = pGhost->m_RscPack2.Build(L"Ghost2.rpi");
		pGhost->m_RscPack2.GetBoneList()->GetAniTimer().SetAniTime(_EFArg.starttick); // [2014.10.16 ]
		pGhost->m_RscPack2.GetAnimator().SetAniTime( _EFArg.starttick); // [2014.10.16 ]
		pGhost->m_RscPack2.GetAnimator().Load_ClipInfo_from_Lua( _EFArg.KeyInfoPath);
		pGhost->m_RscPack2.GetAnimator().Change_Ani_Clip("IDLE");
		pGhost->m_eActState = eGAS_MOVING;
		//pGhost->m_RscPack2.m_pSkinMesh->Set_m_hTech("Skinning_PointLight"); //[2014.11.21 ]	
		pGhost->m_RscPack2.m_pSkinMesh->Set_m_hTech("SPNT_POINT_NOSPECULAR_withLim_FOG"); //[2014.11.21 ]	
		pGhost->m_RscPack2.m_MeshList.Set_hTech_AllMesh("Rigid_Tex_WithFOG"); //[2014.11.21 ]
	}

	// Ghost2�� ���ҽ��� ����� //[2014.11.29 ]
	{
		res = pGhost->m_RscPack3.Build(L"Ghost3.rpi");
		pGhost->m_RscPack3.GetBoneList()->GetAniTimer().SetAniTime(_EFArg.starttick); // [2014.10.16 ]
		pGhost->m_RscPack3.GetAnimator().SetAniTime( _EFArg.starttick); // [2014.10.16 ]
		pGhost->m_RscPack3.GetAnimator().Load_ClipInfo_from_Lua( _EFArg.KeyInfoPath);
		pGhost->m_RscPack3.GetAnimator().Change_Ani_Clip("IDLE");
		pGhost->m_eActState = eGAS_MOVING;
		//pGhost->m_RscPack3.m_pSkinMesh->Set_m_hTech("Skinning_PointLight"); //[2014.11.21 ]	
		pGhost->m_RscPack3.m_pSkinMesh->Set_m_hTech("SPNT_POINT_NOSPECULAR_withLim_FOG"); //[2014.11.21 ]	
		pGhost->m_RscPack3.m_MeshList.Set_hTech_AllMesh("Rigid_Tex_WithFOG"); //[2014.11.21 ]
	}

	//////////////////////////////////////////////////////////////////////////
	// ������ ����Ʈ  // [2014.12.3 ]
	pGhost->m_pEffDeath = new cSHBillBoard();
	pGhost->m_pEffDeath->Init(SHGlobal_GetD3DDevice, L"./image/EXdeath.tga", 40.0f, 18.0f);
	CSceneMgr::getInstance()->GetSceneByName(L"Scene_GameMain")->AddObject(pGhost->m_pEffDeath);

	// �������� �����Ҷ� ������ ����Ʈ //[2014.12.5 ]
	pGhost->m_pEffGoal = new cSHBillBoard();
	pGhost->m_pEffGoal->Init(SHGlobal_GetD3DDevice, L"./image/EX_Goal.tga", 50.0f, 20.0f);
	CSceneMgr::getInstance()->GetSceneByName(L"Scene_GameMain")->AddObject(pGhost->m_pEffGoal);

	if(res == S_FALSE)// ���н� nullptr ��ȯ
		return nullptr;

	return pGhost;
}


///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//
// ���⼭ ���� CCardSkill_Creator class �Լ� ���� // 
//
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------
CCardSkill_Creator::CCardSkill_Creator()
{
}

//-------------------------------------------------------------------------
// �Ҹ���
//-------------------------------------------------------------------------
CCardSkill_Creator::~CCardSkill_Creator()
{
}

//-------------------------------------------------------------------------
// Arg	  : const WCHAR* _pRscPackName(.rpi �̸�)
// Return : (����)CGameObject* , (����)nullptr
// Desc   : ī�� ��ü�� �̰����� �����ؼ� �����͸� ��ȯ�Ѵ�. 
//-------------------------------------------------------------------------
CGameObject*	CCardSkill_Creator::CreateEntity(const WCHAR* _pRscPackName, EntityFactory_Arg &_EFArg)
{
	int res = S_OK;
	CCardSkill* pCard = new CCardSkill();

	res = pCard->GetRscPack()->Build(_pRscPackName);
	res = pCard->Init(SHGlobal_GetD3DDevice, _EFArg.Pos);// CBear �ʱ�ȭ �Լ�

	//////////////////////////////////////////////////////////////////////////
	// ���⼭���� �ݸ��� ���� �������ֱ�  [2014.11.25 ]
	{
		SHR_BoneList* pBL = pCard->GetRscPack()->GetBoneList();

		const SHR_Mesh* pTempMesh =   pBL->FindMeshByName("BoundSphere_Body"); // �ٿ�����Ǿ� �ٵ����� 
		pCard->m_pBoundSphere_Body = new CBoundSphere();
		pCard->m_pBoundSphere_Body->SetRadius( pTempMesh->m_MeshInfo.BoundShpere_Radius);


	}
	//pCard->GetRscPack()->GetBoneList()->GetAniTimer().SetAniTime(_EFArg.starttick); 
	//pCard->GetRscPack()->GetAnimator().SetAniTime( _EFArg.starttick); 
//	pCard->GetRscPack()->GetAnimator().Load_ClipInfo_from_Lua( _EFArg.KeyInfoPath);
	// ī��  �Ӽ����� ��ƽ�ũ��Ʈ���� �о���̱�
	pCard->Load_AttrInfo_from_Lua( _EFArg.AttrInfoPath); //[2014.11.27 ]
	//pCard->GetRscPack()->GetAnimator().Change_Ani_Clip("IDLE");

	//pCard->GetRscPack()->m_pSkinMesh->Set_m_hTech("Skinning_PointLight"); 

	pCard->GetRscPack()->m_MeshList.Set_hTech_AllMesh("Rigid_Tex"); 
	
	if(res == S_FALSE)// ���н� nullptr ��ȯ
		return nullptr;

	return pCard;
}



///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//
// ���⼭ ���� CRockPunchSkill_Creator class �Լ� ���� 
//
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------
CRockPunchSkill_Creator::CRockPunchSkill_Creator()
{
}

//-------------------------------------------------------------------------
// �Ҹ���
//-------------------------------------------------------------------------
CRockPunchSkill_Creator::~CRockPunchSkill_Creator()
{
}

//-------------------------------------------------------------------------
// Arg	  : const WCHAR* _pRscPackName(.rpi �̸�)
// Return : (����)CGameObject* , (����)nullptr
// Desc   : ī�� ��ü�� �̰����� �����ؼ� �����͸� ��ȯ�Ѵ�.
//-------------------------------------------------------------------------
CGameObject*	CRockPunchSkill_Creator::CreateEntity(const WCHAR* _pRscPackName, EntityFactory_Arg &_EFArg)
{
	int res = S_OK;
	CRockPunchSkill* pRockPunch = new CRockPunchSkill();

	res = pRockPunch->GetRscPack()->Build(_pRscPackName);
	res = pRockPunch->Init(SHGlobal_GetD3DDevice, _EFArg.Pos);// CBear �ʱ�ȭ �Լ�

	//////////////////////////////////////////////////////////////////////////
	// ���⼭���� �ݸ��� ���� �������ֱ�  [2014.11.28 ]
	{
		pRockPunch->m_pBoundSphere_Body = new CBoundSphere();
		pRockPunch->m_pBoundSphere_Body->SetRadius( 14.0f);
	}
	//pCard->GetRscPack()->GetBoneList()->GetAniTimer().SetAniTime(_EFArg.starttick); 
	//pCard->GetRscPack()->GetAnimator().SetAniTime( _EFArg.starttick); 
	pRockPunch->GetRscPack()->GetAnimator().Load_ClipInfo_from_Lua( _EFArg.KeyInfoPath);
	// ī��  �Ӽ����� ��ƽ�ũ��Ʈ���� �о���̱�
	pRockPunch->Load_AttrInfo_from_Lua( _EFArg.AttrInfoPath);
	pRockPunch->GetRscPack()->GetAnimator().Change_Ani_Clip("IDLE");

	//pCard->GetRscPack()->m_pSkinMesh->Set_m_hTech("Skinning_PointLight");

	pRockPunch->GetRscPack()->m_MeshList.Set_hTech_AllMesh("Rigid_Tex"); 

	if(res == S_FALSE)// ���н� nullptr ��ȯ
		return nullptr;

	return pRockPunch;
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//
// ���⼭ ���� CHatSkill_Creator class �Լ� ���� // [2014.11.27 ]
//
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------
CHatSkill_Creator::CHatSkill_Creator()
{
}

//-------------------------------------------------------------------------
// �Ҹ���
//-------------------------------------------------------------------------
CHatSkill_Creator::~CHatSkill_Creator()
{
}

//-------------------------------------------------------------------------
// Arg	  : const WCHAR* _pRscPackName(.rpi �̸�)
// Return : (����)CGameObject* , (����)nullptr
// Desc   : ī�� ��ü�� �̰����� �����ؼ� �����͸� ��ȯ�Ѵ�. // [2014.11.27 ]
//-------------------------------------------------------------------------
CGameObject*	CHatSkill_Creator::CreateEntity(const WCHAR* _pRscPackName, EntityFactory_Arg &_EFArg)
{
	int res = S_OK;
	CHatSkill* pHat = new CHatSkill();

	res = pHat->GetRscPack()->Build(_pRscPackName);
	res = pHat->Init(SHGlobal_GetD3DDevice, _EFArg.Pos);// CBear �ʱ�ȭ �Լ�

	//////////////////////////////////////////////////////////////////////////
	// ���⼭���� �ݸ��� ���� �������ֱ�  [2014.11.28 ]
	{
		SHR_MeshList& ML = pHat->GetRscPack()->GetMeshList();
		SHR_Mesh* pTempMesh =   ML.FindMeshByName("Hat"); // �ٿ�����Ǿ� �ٵ�����  [2014.11.25 ]
		pHat->m_pHatMesh = pTempMesh; // [2014.11.28 ]

		SHR_BoneList* pBL = pHat->GetRscPack()->GetBoneList();
		pTempMesh =	pBL->FindMeshByName("BoundSphere_Body");
	
		pHat->m_pBoundSphere_Body = new CBoundSphere();
		pHat->m_pBoundSphere_Body->SetRadius( 10.0f);
		
	} 

	
	//pCard->GetRscPack()->GetBoneList()->GetAniTimer().SetAniTime(_EFArg.starttick); // [2014.10.16 ]
	//pCard->GetRscPack()->GetAnimator().SetAniTime( _EFArg.starttick); // [2014.10.16 ]
	pHat->GetRscPack()->GetAnimator().Load_ClipInfo_from_Lua( _EFArg.KeyInfoPath); // [2014.11.28 ]
	// ī��  �Ӽ����� ��ƽ�ũ��Ʈ���� �о���̱�
	pHat->Load_AttrInfo_from_Lua( _EFArg.AttrInfoPath); //[2014.11.27 ]
	pHat->GetRscPack()->GetAnimator().Change_Ani_Clip("IDLE");

	//pCard->GetRscPack()->m_pSkinMesh->Set_m_hTech("Skinning_PointLight"); //[2014.11.21 ]

	pHat->GetRscPack()->m_MeshList.Set_hTech_AllMesh("Rigid_Tex"); //[2014.11.21 ]

	if(res == S_FALSE)// ���н� nullptr ��ȯ
		return nullptr;

	return pHat;
}



///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//
// ���⼭ ���� CRoom_Creator class �Լ� ���� [2014.11.12 ]
//
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------
// ������[2014.11.12 ]
//-------------------------------------------------------------------------
CRoom_Creator::CRoom_Creator()
{
}

//-------------------------------------------------------------------------
// �Ҹ���[2014.11.12 ]
//-------------------------------------------------------------------------
CRoom_Creator::~CRoom_Creator()
{
}

//-------------------------------------------------------------------------
// Arg	  : const WCHAR* _pRscPackName(.rpi �̸�)
// Return : (����)CGameObject* , (����)nullptr
// Desc   : CBear ��ü�� �̰����� �����ؼ� �����͸� ��ȯ�Ѵ�.  [2014.11.12 ]
//-------------------------------------------------------------------------
CGameObject*	CRoom_Creator::CreateEntity(const WCHAR* _pRscPackName, EntityFactory_Arg &_EFArg)
{
	int res = S_OK;
	CRoom* pRoom = new CRoom();

	res = pRoom->GetRscPack()->Build(_pRscPackName);
	res = pRoom->Init(SHGlobal_GetD3DDevice, _EFArg.Pos);// CBear �ʱ�ȭ �Լ�
	
	//////////////////////////////////////////////////////////////////////////
	// ���⼭ ���� �ٿ���ڽ� ���� �ݸ��� �޴����� �ش� ��ũ��  ������ ������ֱ� //[2014.11.25 ]
	std::vector<SHR_Mesh*>* pVec = pRoom->GetRscPack()->m_pMeshList->GetMeshList();
	std::vector<SHR_Mesh*>::iterator iterCur, iterEnd;
	iterEnd = pVec->end();
	for(iterCur = pVec->begin(); iterCur != iterEnd ; ++iterCur)
	{
		//LOG_WRITE_A("nodeName %s : type %d" , (*iterCur)->m_MeshInfo.Name, (*iterCur)->m_MeshInfo.MeshType);
		if((*iterCur)->m_MeshInfo.MeshType == eMeshType_BoundBox)
		{	
			CBoundBox_AABB* pColBox = new CBoundBox_AABB();  //[2014.11.25 ]
			pColBox->SetMax((*iterCur)->m_MeshInfo.BoundingBox_Max);// �ִ�����
			pColBox->SetMin((*iterCur)->m_MeshInfo.BoundingBox_Min);// �ּ�����

			D3DXVECTOR3 CentPos = ((*iterCur)->m_MeshInfo.BoundingBox_Max + (*iterCur)->m_MeshInfo.BoundingBox_Min)/2.0f;
			pColBox->SetCenter(CentPos); //�������ؼ� ���� [2014.11.25 ]

			CCollideMgr::getInstance()->m_Collider_List[eCollide_Tag_BoxesInRoom].push_back(pColBox);
 		}
	}




	//////////////////////////////////////////////////////////////////////////
	//G_FXShader.GetPointer()->GetTechniqueByName("PointLight_tex_ppl");// [2014.11.12 ]

	//pRoom->GetRscPack()->m_pMeshList->Set_hTech_AllMesh("Outline_PNT_POINT_NOSPECULAR");
	//pRoom->GetRscPack()->m_pMeshList->Set_hTech_AllMesh("PointLight_tex_ppl");
	pRoom->GetRscPack()->m_pMeshList->Set_hTech_AllMesh("Rigid_Tex");
	if(res == S_FALSE)// ���н� nullptr ��ȯ
		return nullptr;

	return pRoom;
}


/****************************************************** End *****************************************************/