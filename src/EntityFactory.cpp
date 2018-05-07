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
// 여기서 부터 CEntityFactory class 함수 구현
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
// 정적변수 초기화
CEntityFactory*		CEntityFactory::pInstance = nullptr;  // 싱글톤

//-------------------------------------------------------------------------
// 생성자
//-------------------------------------------------------------------------
CEntityFactory::CEntityFactory()
{
}


//-------------------------------------------------------------------------
// 소멸자
//-------------------------------------------------------------------------
CEntityFactory::~CEntityFactory()
{
	// 동적으로 생성된 메모리 해제
	std::map<CString, IEntityCreator*>::iterator Iter_Pos;
	
	for( Iter_Pos = m_Map.begin(); Iter_Pos != m_Map.end(); ++Iter_Pos)
	{
		IEntityCreator* ptemp = Iter_Pos->second;
		delete ptemp;
		m_Map.erase(Iter_Pos);
	}
}


//-------------------------------------------------------------------------
// Arg	  : const CHAR* _pCreatorName (크리에이터 클래스 네임), 
//			const CHAR* _RscPackInfoName (.rpi 확장자를가진 파일 이름)
//			EntityFactory_Arg &_EFArg (엔진사용자가 넘겨주고 싶은 값들을 모아놓은 구조체)
// Return : 성공 (S_OK), 실패(S_FALSE)
// Desc   : _pCreatorName 을 키값으로 해당 크리에이터를 찾아 엔티티를 생성한다.
//-------------------------------------------------------------------------
CGameObject*	CEntityFactory::CreateEntity(CString _pCreatorName, const WCHAR* _strRscPackInfo , EntityFactory_Arg &_EFArg)
{
	CGameObject* pObject;
	std::map<CString, IEntityCreator*>::iterator Iter_Pos;
	
	Iter_Pos =  m_Map.find(_pCreatorName);// 해당하는 Creator를 이름으로 찾는다. 

	 if(Iter_Pos != m_Map.end()) // 찾았다면
	 {	pObject = Iter_Pos->second->CreateEntity(_strRscPackInfo, _EFArg);  }
	 else						// 찾지 못했다면
	 {	
		 pObject = nullptr;  LOG_WRITE_A("CEntityFactory::CreateEntity() , nullptr 반환, 이름을 못찾았어!!!!!!!!!!!!!!!!!!"); assert(false);
	 }


	 return pObject;
}



//-----------------------------------------------------------------------------
// Arg    : CString _CreatorType(타입 이름),  IEntityCreator* _pNewCreator(IEntityCreator을 상속받은 클래스)
// Return : 실패시 S_FALSE  , 성공시 S_OK
// Desc   : IEntityCreator 를 상속받은 Creator를 등록한다. 이미 등록되어있는 이름이면 실패를 반환한다.
//			IEntityCreator* _pNewCreator 를 new로 생성해야함
//-----------------------------------------------------------------------------
int			CEntityFactory::Register(CString _CreatorType, IEntityCreator* _pNewCreator)
{
	if(_pNewCreator == nullptr)
	{
		SHUtil::MsgBox("CEntityFactory::Register() , nullptr 전달");
		assert(false);
		return S_FALSE;
	}

	std::map<CString, IEntityCreator*>::iterator Iter_Pos;

	Iter_Pos = m_Map.find(_CreatorType);

	if(Iter_Pos != m_Map.end()) //  찾았다면
	{
		return S_FALSE; //같은이름이 있으므로 S_FALSE를 리턴
	}

	
	m_Map.insert(std::pair<CString, IEntityCreator*>(_CreatorType, _pNewCreator));

	return S_OK;
}


//-----------------------------------------------------------------------------
// 싱글톤객체 생성
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
// 여기서 부터 CBearCreator class 함수 구현
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
// 생성자
//-------------------------------------------------------------------------
CBearCreator::CBearCreator()
{
	
}

//-------------------------------------------------------------------------
// 소멸자
//-------------------------------------------------------------------------
CBearCreator::~CBearCreator()
{ 
	
}


//-------------------------------------------------------------------------
// Arg	  : const WCHAR* _pRscPackName(.rpi 이름)
// Return : (성공)CGameObject* , (실패)nullptr
// Desc   : CBear 객체를 이곳에서 생성해서 포인터를 반환한다.
//-------------------------------------------------------------------------
CGameObject*	CBearCreator::CreateEntity(const WCHAR* _pRscPackName, EntityFactory_Arg &_EFArg)
{
	int res = S_OK;
	CBear* pBear = new CBear();

	res = pBear->GetRscPack()->Build(_pRscPackName);
	res = pBear->Init(SHGlobal_GetD3DDevice, _EFArg.Pos);// CBear 초기화 함수

	pBear->GetRscPack()->GetBoneList()->GetAniTimer().SetAniTime(_EFArg.starttick); 
	pBear->GetRscPack()->GetAnimator().SetAniTime( _EFArg.starttick);
	pBear->GetRscPack()->GetAnimator().Load_ClipInfo_from_Lua( _EFArg.KeyInfoPath);

	//////////////////////////////////////////////////////////////////////////
	// 여기서 부터 멤버변수에 스킬변수 장착하기
	
	pBear->m_SkillCard =  (*G_pScatter); //  멤버변수에 복사하기
	pBear->m_SkillHat = (*G_pHat); 
	pBear->m_SkillRockPunch = (*G_pRockPunch);
	
	//////////////////////////////////////////////////////////////////////////
	// 여기서부터 콜리더 정보 설정해주기  [2014.11.28 ]
	{
		SHR_MeshList& ML = pBear->m_SkillHat.GetRscPack()->GetMeshList();
		SHR_Mesh* pTempMesh =   ML.FindMeshByName("Hat"); // 바운딩스피어 바디정보
		pBear->m_SkillHat.m_pHatMesh = pTempMesh; //

		SHR_BoneList* pBL = pBear->m_SkillHat.GetRscPack()->GetBoneList();
		pTempMesh =	pBL->FindMeshByName("BoundSphere_Body");

		pBear->m_SkillHat.m_pBoundSphere_Body = new CBoundSphere();
		pBear->m_SkillHat.m_pBoundSphere_Body->SetRadius( 60.0f);

	}
	//////////////////////////////////////////////////////////////////////////
	// 여기서부터 콜리더 정보 설정해주기  

		SHR_BoneList* pBL = pBear->GetRscPack()->GetBoneList();

		const SHR_Mesh* pTempMesh = pBL->FindMeshByName("BoundSphere_Body"); // 바운딩스피어 바디정보  
		pBear->m_pBoundSphere_Body = new CBoundSphere();
		pBear->m_pBoundSphere_Body->SetRadius( pTempMesh->m_MeshInfo.BoundShpere_Radius);

		pTempMesh =  pBL->FindMeshByName("BoundSphere_Stick"); //바운딩스피어 스틱정보
		pBear->m_BSMesh_Stick = pTempMesh; 
		pBear->m_pBoundSphere_Stick = new CBoundSphere();
		pBear->m_pBoundSphere_Stick->SetRadius( pTempMesh->m_MeshInfo.BoundShpere_Radius+10);

		pTempMesh =  pBL->FindMeshByName("Bone_hat"); //모자 뼈대메쉬 포인터 //
		pBear->m_BoneMesh_Hat = pTempMesh; 

	}
	//////////////////////////////////////////////////////////////////////////
	// 여기서부터 테디베어 속성정보 루아스크립트에서 읽어들이기
	pBear->Load_AttrInfo_from_Lua( _EFArg.AttrInfoPath);
	pBear->GetRscPack()->GetAnimator().Change_Ani_Clip("IDLE");

	//////////////////////////////////////////////////////////////////////////
	// 여기서부터 셰이더 테크 설정해주기
	pBear->GetRscPack()->m_pSkinMesh->Set_m_hTech("Outline_SPNT_POINT_NOSPECULAR");
	//pBear->GetRscPack()->m_pSkinMesh->Set_m_hTech("Skinning_PointLight");
	pBear->GetRscPack()->m_MeshList.Set_hTech_AllMesh("Rigid_Tex");


	//////////////////////////////////////////////////////////////////////////
	// 마법봉 이펙트  // [2014.12.3 ]
	pBear->m_pEffMagicStick = new cSHBillBoard();
	pBear->m_pEffMagicStick->Init(SHGlobal_GetD3DDevice, L"./image/weapon.tga", 20.0f, 8.0f);
	CSceneMgr::getInstance()->GetSceneByName(L"Scene_GameMain")->AddObject(pBear->m_pEffMagicStick); //
	pBear->m_pEffMagicStick->Set_m_bVisible( true);

		if(res == S_FALSE)// 실패시 nullptr 반환
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
// 여기서 부터 CGhost_Creator class 함수 구현
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
// 생성자
//-------------------------------------------------------------------------
CGhost_Creator::CGhost_Creator()
{
}

//-------------------------------------------------------------------------
// 소멸자
//-------------------------------------------------------------------------
CGhost_Creator::~CGhost_Creator()
{
}

//-------------------------------------------------------------------------
// Arg	  : const WCHAR* _pRscPackName(.rpi 이름)
// Return : (성공)CGameObject* , (실패)nullptr
// Desc   : CBear 객체를 이곳에서 생성해서 포인터를 반환한다.
//-------------------------------------------------------------------------
CGameObject*	CGhost_Creator::CreateEntity(const WCHAR* _pRscPackName, EntityFactory_Arg &_EFArg)
{
	int res = S_OK;
	CGhost* pGhost = new CGhost();
	res = pGhost->Init(SHGlobal_GetD3DDevice, _EFArg.Pos);// CBear 초기화 함수
	// Ghost1의 리소스팩 만들기 //[2014.11.29 ]
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

	// Ghost2의 리소스팩 만들기 //[2014.11.29 ]
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

	// Ghost2의 리소스팩 만들기 //[2014.11.29 ]
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
	// 죽을때 이펙트  // [2014.12.3 ]
	pGhost->m_pEffDeath = new cSHBillBoard();
	pGhost->m_pEffDeath->Init(SHGlobal_GetD3DDevice, L"./image/EXdeath.tga", 40.0f, 18.0f);
	CSceneMgr::getInstance()->GetSceneByName(L"Scene_GameMain")->AddObject(pGhost->m_pEffDeath);

	// 아이한태 도착할때 나오는 이펙트 //[2014.12.5 ]
	pGhost->m_pEffGoal = new cSHBillBoard();
	pGhost->m_pEffGoal->Init(SHGlobal_GetD3DDevice, L"./image/EX_Goal.tga", 50.0f, 20.0f);
	CSceneMgr::getInstance()->GetSceneByName(L"Scene_GameMain")->AddObject(pGhost->m_pEffGoal);

	if(res == S_FALSE)// 실패시 nullptr 반환
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
// 여기서 부터 CCardSkill_Creator class 함수 구현 // 
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
// 생성자
//-------------------------------------------------------------------------
CCardSkill_Creator::CCardSkill_Creator()
{
}

//-------------------------------------------------------------------------
// 소멸자
//-------------------------------------------------------------------------
CCardSkill_Creator::~CCardSkill_Creator()
{
}

//-------------------------------------------------------------------------
// Arg	  : const WCHAR* _pRscPackName(.rpi 이름)
// Return : (성공)CGameObject* , (실패)nullptr
// Desc   : 카드 객체를 이곳에서 생성해서 포인터를 반환한다. 
//-------------------------------------------------------------------------
CGameObject*	CCardSkill_Creator::CreateEntity(const WCHAR* _pRscPackName, EntityFactory_Arg &_EFArg)
{
	int res = S_OK;
	CCardSkill* pCard = new CCardSkill();

	res = pCard->GetRscPack()->Build(_pRscPackName);
	res = pCard->Init(SHGlobal_GetD3DDevice, _EFArg.Pos);// CBear 초기화 함수

	//////////////////////////////////////////////////////////////////////////
	// 여기서부터 콜리더 정보 설정해주기  [2014.11.25 ]
	{
		SHR_BoneList* pBL = pCard->GetRscPack()->GetBoneList();

		const SHR_Mesh* pTempMesh =   pBL->FindMeshByName("BoundSphere_Body"); // 바운딩스피어 바디정보 
		pCard->m_pBoundSphere_Body = new CBoundSphere();
		pCard->m_pBoundSphere_Body->SetRadius( pTempMesh->m_MeshInfo.BoundShpere_Radius);


	}
	//pCard->GetRscPack()->GetBoneList()->GetAniTimer().SetAniTime(_EFArg.starttick); 
	//pCard->GetRscPack()->GetAnimator().SetAniTime( _EFArg.starttick); 
//	pCard->GetRscPack()->GetAnimator().Load_ClipInfo_from_Lua( _EFArg.KeyInfoPath);
	// 카드  속성정보 루아스크립트에서 읽어들이기
	pCard->Load_AttrInfo_from_Lua( _EFArg.AttrInfoPath); //[2014.11.27 ]
	//pCard->GetRscPack()->GetAnimator().Change_Ani_Clip("IDLE");

	//pCard->GetRscPack()->m_pSkinMesh->Set_m_hTech("Skinning_PointLight"); 

	pCard->GetRscPack()->m_MeshList.Set_hTech_AllMesh("Rigid_Tex"); 
	
	if(res == S_FALSE)// 실패시 nullptr 반환
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
// 여기서 부터 CRockPunchSkill_Creator class 함수 구현 
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
// 생성자
//-------------------------------------------------------------------------
CRockPunchSkill_Creator::CRockPunchSkill_Creator()
{
}

//-------------------------------------------------------------------------
// 소멸자
//-------------------------------------------------------------------------
CRockPunchSkill_Creator::~CRockPunchSkill_Creator()
{
}

//-------------------------------------------------------------------------
// Arg	  : const WCHAR* _pRscPackName(.rpi 이름)
// Return : (성공)CGameObject* , (실패)nullptr
// Desc   : 카드 객체를 이곳에서 생성해서 포인터를 반환한다.
//-------------------------------------------------------------------------
CGameObject*	CRockPunchSkill_Creator::CreateEntity(const WCHAR* _pRscPackName, EntityFactory_Arg &_EFArg)
{
	int res = S_OK;
	CRockPunchSkill* pRockPunch = new CRockPunchSkill();

	res = pRockPunch->GetRscPack()->Build(_pRscPackName);
	res = pRockPunch->Init(SHGlobal_GetD3DDevice, _EFArg.Pos);// CBear 초기화 함수

	//////////////////////////////////////////////////////////////////////////
	// 여기서부터 콜리더 정보 설정해주기  [2014.11.28 ]
	{
		pRockPunch->m_pBoundSphere_Body = new CBoundSphere();
		pRockPunch->m_pBoundSphere_Body->SetRadius( 14.0f);
	}
	//pCard->GetRscPack()->GetBoneList()->GetAniTimer().SetAniTime(_EFArg.starttick); 
	//pCard->GetRscPack()->GetAnimator().SetAniTime( _EFArg.starttick); 
	pRockPunch->GetRscPack()->GetAnimator().Load_ClipInfo_from_Lua( _EFArg.KeyInfoPath);
	// 카드  속성정보 루아스크립트에서 읽어들이기
	pRockPunch->Load_AttrInfo_from_Lua( _EFArg.AttrInfoPath);
	pRockPunch->GetRscPack()->GetAnimator().Change_Ani_Clip("IDLE");

	//pCard->GetRscPack()->m_pSkinMesh->Set_m_hTech("Skinning_PointLight");

	pRockPunch->GetRscPack()->m_MeshList.Set_hTech_AllMesh("Rigid_Tex"); 

	if(res == S_FALSE)// 실패시 nullptr 반환
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
// 여기서 부터 CHatSkill_Creator class 함수 구현 // [2014.11.27 ]
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
// 생성자
//-------------------------------------------------------------------------
CHatSkill_Creator::CHatSkill_Creator()
{
}

//-------------------------------------------------------------------------
// 소멸자
//-------------------------------------------------------------------------
CHatSkill_Creator::~CHatSkill_Creator()
{
}

//-------------------------------------------------------------------------
// Arg	  : const WCHAR* _pRscPackName(.rpi 이름)
// Return : (성공)CGameObject* , (실패)nullptr
// Desc   : 카드 객체를 이곳에서 생성해서 포인터를 반환한다. // [2014.11.27 ]
//-------------------------------------------------------------------------
CGameObject*	CHatSkill_Creator::CreateEntity(const WCHAR* _pRscPackName, EntityFactory_Arg &_EFArg)
{
	int res = S_OK;
	CHatSkill* pHat = new CHatSkill();

	res = pHat->GetRscPack()->Build(_pRscPackName);
	res = pHat->Init(SHGlobal_GetD3DDevice, _EFArg.Pos);// CBear 초기화 함수

	//////////////////////////////////////////////////////////////////////////
	// 여기서부터 콜리더 정보 설정해주기  [2014.11.28 ]
	{
		SHR_MeshList& ML = pHat->GetRscPack()->GetMeshList();
		SHR_Mesh* pTempMesh =   ML.FindMeshByName("Hat"); // 바운딩스피어 바디정보  [2014.11.25 ]
		pHat->m_pHatMesh = pTempMesh; // [2014.11.28 ]

		SHR_BoneList* pBL = pHat->GetRscPack()->GetBoneList();
		pTempMesh =	pBL->FindMeshByName("BoundSphere_Body");
	
		pHat->m_pBoundSphere_Body = new CBoundSphere();
		pHat->m_pBoundSphere_Body->SetRadius( 10.0f);
		
	} 

	
	//pCard->GetRscPack()->GetBoneList()->GetAniTimer().SetAniTime(_EFArg.starttick); // [2014.10.16 ]
	//pCard->GetRscPack()->GetAnimator().SetAniTime( _EFArg.starttick); // [2014.10.16 ]
	pHat->GetRscPack()->GetAnimator().Load_ClipInfo_from_Lua( _EFArg.KeyInfoPath); // [2014.11.28 ]
	// 카드  속성정보 루아스크립트에서 읽어들이기
	pHat->Load_AttrInfo_from_Lua( _EFArg.AttrInfoPath); //[2014.11.27 ]
	pHat->GetRscPack()->GetAnimator().Change_Ani_Clip("IDLE");

	//pCard->GetRscPack()->m_pSkinMesh->Set_m_hTech("Skinning_PointLight"); //[2014.11.21 ]

	pHat->GetRscPack()->m_MeshList.Set_hTech_AllMesh("Rigid_Tex"); //[2014.11.21 ]

	if(res == S_FALSE)// 실패시 nullptr 반환
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
// 여기서 부터 CRoom_Creator class 함수 구현 [2014.11.12 ]
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
// 생성자[2014.11.12 ]
//-------------------------------------------------------------------------
CRoom_Creator::CRoom_Creator()
{
}

//-------------------------------------------------------------------------
// 소멸자[2014.11.12 ]
//-------------------------------------------------------------------------
CRoom_Creator::~CRoom_Creator()
{
}

//-------------------------------------------------------------------------
// Arg	  : const WCHAR* _pRscPackName(.rpi 이름)
// Return : (성공)CGameObject* , (실패)nullptr
// Desc   : CBear 객체를 이곳에서 생성해서 포인터를 반환한다.  [2014.11.12 ]
//-------------------------------------------------------------------------
CGameObject*	CRoom_Creator::CreateEntity(const WCHAR* _pRscPackName, EntityFactory_Arg &_EFArg)
{
	int res = S_OK;
	CRoom* pRoom = new CRoom();

	res = pRoom->GetRscPack()->Build(_pRscPackName);
	res = pRoom->Init(SHGlobal_GetD3DDevice, _EFArg.Pos);// CBear 초기화 함수
	
	//////////////////////////////////////////////////////////////////////////
	// 여기서 부터 바운딩박스 만들어서 콜리드 메니져의 해당 테크에  포인터 등록해주기 //[2014.11.25 ]
	std::vector<SHR_Mesh*>* pVec = pRoom->GetRscPack()->m_pMeshList->GetMeshList();
	std::vector<SHR_Mesh*>::iterator iterCur, iterEnd;
	iterEnd = pVec->end();
	for(iterCur = pVec->begin(); iterCur != iterEnd ; ++iterCur)
	{
		//LOG_WRITE_A("nodeName %s : type %d" , (*iterCur)->m_MeshInfo.Name, (*iterCur)->m_MeshInfo.MeshType);
		if((*iterCur)->m_MeshInfo.MeshType == eMeshType_BoundBox)
		{	
			CBoundBox_AABB* pColBox = new CBoundBox_AABB();  //[2014.11.25 ]
			pColBox->SetMax((*iterCur)->m_MeshInfo.BoundingBox_Max);// 최대정점
			pColBox->SetMin((*iterCur)->m_MeshInfo.BoundingBox_Min);// 최소정점

			D3DXVECTOR3 CentPos = ((*iterCur)->m_MeshInfo.BoundingBox_Max + (*iterCur)->m_MeshInfo.BoundingBox_Min)/2.0f;
			pColBox->SetCenter(CentPos); //중점구해서 설정 [2014.11.25 ]

			CCollideMgr::getInstance()->m_Collider_List[eCollide_Tag_BoxesInRoom].push_back(pColBox);
 		}
	}




	//////////////////////////////////////////////////////////////////////////
	//G_FXShader.GetPointer()->GetTechniqueByName("PointLight_tex_ppl");// [2014.11.12 ]

	//pRoom->GetRscPack()->m_pMeshList->Set_hTech_AllMesh("Outline_PNT_POINT_NOSPECULAR");
	//pRoom->GetRscPack()->m_pMeshList->Set_hTech_AllMesh("PointLight_tex_ppl");
	pRoom->GetRscPack()->m_pMeshList->Set_hTech_AllMesh("Rigid_Tex");
	if(res == S_FALSE)// 실패시 nullptr 반환
		return nullptr;

	return pRoom;
}


/****************************************************** End *****************************************************/