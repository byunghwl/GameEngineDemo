#include "DXUT.h"
#include "RscPack.h"


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
// 여기서부터 class CRscPack_NormalObj 함수정의
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


//-------------------------------------------------------------------------
// Arg	  : IDirect3DDevice9* device
// Return :
// Desc   : 리소스팩안에있는 노말오브젝트들을 그려준다.
//-------------------------------------------------------------------------
void		CRscPack_NormalObj::Draw(IDirect3DDevice9* device)  //[2014.11.11 ]
{


		m_pMeshList->Draw(device);


}

//-----------------------------------------------------------------------------
// Arg    : float _dTime(delta time),  D3DXMATRIX TM(부모노드의  변환 행렬)
// Desc   : f을 가지고있는 개체가 이함수를 호출한다.   //[2014.11.14 ]
//-----------------------------------------------------------------------------
void	CRscPack_NormalObj::Update(float _dTime,  D3DXMATRIX* TM)
{
	D3DXMATRIX TempTM;

	if(TM == nullptr)
	{	D3DXMatrixIdentity(&TempTM);	}
	else
	{
		TempTM = *TM;
	}

	if(m_pMeshList != nullptr)   // 본을 항상 먼저 업데이트 해야함
		m_pMeshList->Update(m_Animator, &TempTM);// [2014.10.13 ]

}

//-------------------------------------------------------------------------
// .rpi 확장자를 가진 파일을 가져와서 리소스팩을 빌드한다. [2014.11.11 ]
//-------------------------------------------------------------------------
int		CRscPack_NormalObj::Build(const WCHAR* _pPath)
{

	int hr = S_OK;

	FILE* fp = NULL;
	errno_t err;
	WCHAR Path[Max_Path_Len];
	wcscpy_s(Path, L"./rpi/");
	wcscat_s(Path, _pPath);
	err	= _wfopen_s(&fp, Path, L"rb");
	if(err != 0)
	{
		// 파일 읽기 실패
		LOG_WRITE_A("[CRscPack_Character::Build], 에러!, 파일읽기 실패");
		SHUtil::MsgBox("[CRscPack_Character::Build], 에러!, 파일읽기 실패");  assert(false);
		return S_FALSE;
	}

	int rescnt =0;
	fread(&rescnt, sizeof(rescnt),1, fp);// 리소스 총 개수 읽기
	int resType;//, RscPackNum; [2014.11.19 ]

	for(int i = 0 ; i< rescnt ; i++)
	{
		fread(&resType, sizeof(resType),1 ,fp);

		if(resType == eRT_MESH_LIST)
		{
			//int resType;//, RscPackNum;[2014.11.19 ]
			int NameSize = 0; WCHAR Name[Max_Name_Len] = {NULL , };

			fread(&NameSize, sizeof(NameSize),1, fp); // 이름 길이 읽기
			fread(Name, NameSize, 1, fp);
			IResource* pRsc = CResourceMgr::getInstance()->Find_Resource(Name);

			m_pMeshList = new SHR_MeshList();// [2014.10.12 ]
			SHR_MeshList* pTempML = dynamic_cast<SHR_MeshList*>(pRsc);
			if(pTempML == nullptr) 
			{		
				SHUtil::MsgBox("CRscPack_Character::Build(), 다이나믹캐스트실패"); 
				hr = S_FALSE;
			}

			(*m_pMeshList) = (*pTempML); // 내부의 리소스들을 복사 [2014.10.12 ]

		
		}

	}	


	fclose(fp);
	LOG_WRITE_A("[CRscPack_Character::Build], return %d!!!! " ,hr);
	return hr;


}

//--

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
// 여기서부터 class CRscPack 함수정의
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
// 생성자
//-----------------------------------------------------------------------------
CRscPack_Character::CRscPack_Character()
{
	for(int i = 0; i< eRPT_Max ; i++)
	{
		m_pUpdater[i] = nullptr;
		m_pRenderer[i] = nullptr;
		m_pAllRsc[i] = nullptr;
	}

	m_pBoneList = nullptr;
	m_pSkinMesh = nullptr;

}

//-----------------------------------------------------------------------------
//소멸자
//-----------------------------------------------------------------------------
CRscPack_Character::~CRscPack_Character()
{
// 	for(int i = 0; i< eRPT_Max ; i++)
// 	{
// 		SAFE_DELETE_( m_pAllRsc[i])	
// 	}

	//SAFE_DELETE_(m_pBoneList) // [2014.10.12 ]
}


//-----------------------------------------------------------------------------
// 그려주기
//-----------------------------------------------------------------------------
void	CRscPack_Character::Draw(IDirect3DDevice9* device)
{
// 	if(m_hTech_Skin == nullptr )  // 테크 핸들 확인해보기 [2014.10.30 ]
// 	{		assert(false);	} // [2014.10.30 ]
// 
// 	G_FXShader.GetPointer()->SetTechnique(m_hTech_Skin);// [2014.10.30 ]
// 
// 	UINT MaxPass = 0;
// 
// 	G_FXShader.m_pShader->Begin(&MaxPass, NULL);// [2014.10.29 ]
// 
// 	for(UINT PassNum = 0; PassNum < MaxPass ; PassNum++ )
// 	{
// 		G_FXShader.GetPointer()->BeginPass(PassNum);// [2014.10.29 ]
// 
// 		// 렌더타깃 위에 그린다.
// 		
// 		if( SUCCEEDED( G_ScreenQuad.g_pRenderTargetTexPass[0/*PassNum 일단 0으로..[2014.11.19 ]*/]->GetSurfaceLevel( 0, &(G_ScreenQuad.g_pSceneSurface[0]) ) ) )
// 		{//gpSceneRenderTarget은 서페이스를 만들어줄때 필요한 녀석 [2014.11.7 ]
// 			device->SetRenderTarget( 0, G_ScreenQuad.g_pSceneSurface[0] ); // 만든 서페이스를 렌더타겟으로 지정(복사) [2014.11.7 ]
// 		}

	//	G_FXShader.GetPointer()->CommitChanges(); // [2014.10.30 ] 
		for(int i = 0 ; i< eRPT_Max ; i++)
		{
			if(m_pRenderer[i] != nullptr)
				m_pRenderer[i]->Draw(device);
		}

// 		G_FXShader.GetPointer()->EndPass();// [2014.10.29 ]
// 	}
// 
// 	G_FXShader.GetPointer()->End();// [2014.10.29 ]

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 	if(m_hTech_noSkin == nullptr )  // 테크 핸들 확인해보기 [2014.10.30 ]
// 	{		assert(false);	} // [2014.10.30 ]
// 
// 	G_FXShader.GetPointer()->SetTechnique(m_hTech_noSkin);// [2014.10.30 ]
// 
// 

// 	MaxPass = 0;
// 
// 	G_FXShader.m_pShader->Begin(&MaxPass, NULL);// [2014.10.29 ]
// 
// 	for(UINT PassNum = 0; PassNum < MaxPass ; PassNum++ )
// 	{
// 		G_FXShader.GetPointer()->BeginPass(PassNum);// [2014.10.29 ]
// 
// 		// 렌더타깃 위에 그린다.
// 
// 		if( SUCCEEDED( G_ScreenQuad.g_pRenderTargetTexPass[0/*PassNum 일단 0으로..[2014.11.19 ]*/]->GetSurfaceLevel( 0, &(G_ScreenQuad.g_pSceneSurface[0]) ) ) )
// 		{//gpSceneRenderTarget은 서페이스를 만들어줄때 필요한 녀석 [2014.11.7 ]
// 			device->SetRenderTarget( 0, G_ScreenQuad.g_pSceneSurface[0] ); // 만든 서페이스를 렌더타겟으로 지정(복사) [2014.11.7 ]
// 		}

		
	    m_MeshList.Draw(device);
				

// 		G_FXShader.GetPointer()->EndPass();// [2014.10.29 ]
// 	}
// 
// 	G_FXShader.GetPointer()->End();// [2014.10.29 ]
// 

}



//-----------------------------------------------------------------------------
// Arg    : float _dTime(delta time),  D3DXMATRIX TM(부모노드의  변환 행렬)
// Desc   : f을 가지고있는 개체가 이함수를 호출한다.
//-----------------------------------------------------------------------------
void	CRscPack_Character::Update(float _dTime,  D3DXMATRIX* TM)
{
	D3DXMATRIX TempTM;

	if(TM == nullptr)
	{	D3DXMatrixIdentity(&TempTM);	}
	else
	{	TempTM = *TM;	}

	//1) 에니메이터를 업데이트 한다. [2014.11.23 ]
	m_Animator.AniTimeUpdate();


	//2) 본을 항상 먼저 업데이트 해야함
	if(m_pBoneList != nullptr)   
		m_pBoneList->Update(m_Animator, &TempTM);// [2014.10.13 ]

	//3) 그다음 메쉬리스트를 업데이트함 
	m_MeshList.Update(m_Animator, & TempTM); //반드시 본업데이트 한 다음에 업데이트하기 [2014.11.21 ]

	// 4) 가장 마지막에 스킨들을 업데이트함 
	for(int i = 0 ; i< eRPT_Max ; i++)
	{
		if(m_pUpdater[i] != nullptr)
			m_pUpdater[i]->Update(m_Animator , &TempTM);
	}
}



//-----------------------------------------------------------------------------
// Arg    : const WCHAR* _pPath( 파일경로명)
// Return : Succ(S_OK), Fail(S_Fail)
// Desc   : 리소스정보(.rpi)를 가지고 있는 파일을 읽고 빌드한다.
//-----------------------------------------------------------------------------
int	 CRscPack_Character::Build(const WCHAR* _pPath)
{
	int hr = S_OK;

	FILE* fp = NULL;
	errno_t err;
	WCHAR Path[Max_Path_Len];
	wcscpy_s(Path, L"./rpi/");
	wcscat_s(Path, _pPath);
	err	= _wfopen_s(&fp, Path, L"rb");
	if(err != 0)
	{
		// 파일 읽기 실패
		LOG_WRITE_A("[CRscPack_Character::Build], 에러!, 파일읽기 실패");
		SHUtil::MsgBox("[CRscPack_Character::Build], 에러!, 파일읽기 실패");  assert(false);
		return S_FALSE;
	}

	int rescnt =0;
	fread(&rescnt, sizeof(rescnt),1, fp);// 리소스 총 개수 읽기
	int resType;//, RscPackNum;[2014.11.19 ]

	for(int i = 0 ; i< rescnt ; i++)
	{
		fread(&resType, sizeof(resType),1 ,fp);

		if(resType == eRT_BONE_LIST)
		{
//			int resType, RscPackNum; [2014.11.19 ]`
			int NameSize = 0; WCHAR Name[Max_Name_Len] = {NULL , };

			fread(&NameSize, sizeof(NameSize),1, fp); // 이름 길이 읽기
			fread(Name, NameSize, 1, fp);
			IResource* pRsc = CResourceMgr::getInstance()->Find_Resource(Name);
			
			m_pBoneList = new SHR_BoneList();// [2014.10.12 ]
			SHR_BoneList* pTempBL = dynamic_cast<SHR_BoneList*>(pRsc);
			if(pTempBL == nullptr) 
			{		
				SHUtil::MsgBox("CRscPack_Character::Build(),eRT_BONE_LIST 에서  다이나믹캐스트실패"); 
				hr = S_FALSE;
			}

			(*m_pBoneList) = (*pTempBL); // 내부의 리소스들을 복사 [2014.10.12 ]

			//m_pRefPoint_BoundSphere = m_pBoneList->FindMeshByName("BS_RHand"); [2014.11.20 ]
		}
		else if(resType == eRT_SKIN_MESH)
		{
			int  RscPackNum;// resType, [2014.11.19 ]
			int NameSize = 0; WCHAR Name[Max_Name_Len] = {NULL , };

			fread(&RscPackNum, sizeof(int), 1, fp);
			fread(&NameSize, sizeof(NameSize),1, fp); // 이름 길이 읽기
			fread(Name, NameSize, 1, fp);
			IResource* pRsc = CResourceMgr::getInstance()->Find_Resource(Name);
			
			SHR_SkinMesh* pSkin = new SHR_SkinMesh(); //스킨 새로생성
			SHR_SkinMesh* TempSkin = dynamic_cast<SHR_SkinMesh*>(pRsc);//이름으로 찾은 스킨포인터 얻기
			assert(TempSkin != nullptr);// 캐스팅 검사
			(*pSkin) = (*TempSkin); //스킨이 가지고있는 내부 리소스들만 복사

			m_pAllRsc[RscPackNum] = pSkin;//new로 생성한 포인터는 모든 리소스를 관리하는 배열에 넣어 관리한다.
			m_pSkinMesh = pSkin; //[2014.11.21 ]

			hr = SetRsc_to_Renderer(pSkin, RscPackNum);
			hr = SetRsc_to_Updater(pSkin, RscPackNum);	
			// 스킨메쉬의 본리스트 설정
			Make_BoneList(RscPackNum);
			// 스킨메쉬의 본메트릭스 팔래트 설정
			Make_BoneMatPalette(RscPackNum);
		
		}
		else if(resType == eRT_MESH_LIST) //[2014.11.21 ]
		{
			//int resType, RscPackNum; //[2014.11.19 ]`
			int mNameSize = 0; WCHAR MeshListName[Max_Name_Len] = {NULL , };

			fread(&mNameSize, sizeof(mNameSize),1, fp); // 이름 길이 읽기
			fread(MeshListName, mNameSize, 1, fp);
			IResource* pRsc = CResourceMgr::getInstance()->Find_Resource(MeshListName);

			
			SHR_MeshList* pTempML = dynamic_cast<SHR_MeshList*>(pRsc);
			if(pTempML == nullptr) 
			{		
				SHUtil::MsgBox("CRscPack_Character::Build(),  eRT_MESH_LIST 에서 다이나믹캐스트실패"); 
				hr = S_FALSE;
			}

			m_MeshList = (*pTempML); // 내부의 리소스들을 복사 [2014.10.12 ]

			if(m_pBoneList != nullptr)
			{m_MeshList.ConnectParent_with_BoneList(m_pBoneList);} //[2014.11.21 ]
		}

	}	

	if(m_pBoneList != nullptr)
	{m_pBoneList->ConnectParent_From_MeshList(&m_MeshList);}// [2014.11.27 ]



	fclose(fp);
	LOG_WRITE_A("[CRscPack_Character::Build], return %d!!!! " ,hr);
	return hr;

}


//-------------------------------------------------------------------------
// 
//-------------------------------------------------------------------------
int		CRscPack_Character::SetRsc_to_Renderer(IResource* pRsc , int eRPTNum)
{
	assert(pRsc != nullptr);
	IRenderer* pRen = dynamic_cast<IRenderer*>(pRsc);
	if(pRen == NULL)
	{
		SHUtil::MsgBox("CRscPack_Character::SetRsc_to_Renderer(), dynamic_cast 실패");
		return S_FALSE;
	}

	m_pRenderer[eRPTNum]  = pRen;
	return S_OK;
}

//-------------------------------------------------------------------------
// 
//-------------------------------------------------------------------------
int		CRscPack_Character::SetRsc_to_Updater(IResource* pRsc , int eRPTNum)
{
	assert(pRsc != nullptr);
	IUpdater* pUp = dynamic_cast<IUpdater*>(pRsc);
	if(pUp == NULL)
	{
		SHUtil::MsgBox("CRscPack_Character::SetRsc_to_Updater, dynamic_cast 실패");
		return S_FALSE;
	}

	m_pUpdater[eRPTNum]  = pUp;
	return S_OK;
}



//-----------------------------------------------------------------------------
// Arg    : int _enumVal(), enum eRscName 
// Return : 실패시 Nullptr리턴 , 성공시 해당 리소스포인터 반환
// Desc   : _enumVal을 사용해서 스킨메쉬용 리소스를 타입캐스팅해서 반환합니다. 
//			실패시 메세지박스와 Nullptr을 리턴
//-----------------------------------------------------------------------------
SHR_SkinMesh*	CRscPack_Character::GetSkinMesh(int _enumVal)
{
	IRenderer* pRen = m_pRenderer[_enumVal];

	SHR_SkinMesh* pSkin = dynamic_cast<SHR_SkinMesh*>(pRen);

	if(pSkin== nullptr)
	{
		SHUtil::MsgBox("CRscPack_Character::GetSkinMesh(), 타입캐스팅 실패!!");
		return nullptr;
	}

	return pSkin;

}

//-----------------------------------------------------------------------------
// Arg    : int _enumVal(), enum eRscName 
// Return : 실패시 Nullptr리턴 , 성공시 해당 리소스포인터 반환
// Desc   : _enumVal을 사용해서 메쉬리스트용 리소스를 타입캐스팅해서 반환합니다
//          실패시 메세지박스와 Nullptr을 리턴
//-----------------------------------------------------------------------------
// SHR_BoneList*	CRscPack_Character::GetBoneList()
// {
// 	IRenderer* pRen = m_pRenderer[_enumVal];
// 
// 	SHR_BoneList* pMeshList = dynamic_cast<SHR_BoneList*>(pRen);
// 
// 	if(pMeshList== nullptr)
// 	{
// 		SHUtil::MsgBox("CRscPack_Character::GetMeshList(), 타입캐스팅 실패!!");
// 		return nullptr;
// 	}
// 
// 	return pMeshList;
// }

//-----------------------------------------------------------------------------
// Arg    : int _enumVal(), enum eRscName 
// Return : 실패시 Nullptr리턴 , 성공시 해당 리소스포인터 반환
// Desc   : _enumVal을 사용해서 메쉬용 리소스를 타입캐스팅해서 반환합니다. 
//			실패시 메세지박스와 Nullptr을 리턴
//-----------------------------------------------------------------------------

SHR_Mesh*	CRscPack_Character::GetMesh(int _enumVal)
{
	IResource* pRen = m_pAllRsc[_enumVal];

	SHR_Mesh* pMesh = dynamic_cast<SHR_Mesh*>(pRen);

	if(pMesh == nullptr)
	{
		SHUtil::MsgBox("CRscPack_Character::GetMesh(), 타입캐스팅 실패!!");
		return nullptr;
	}

	return pMesh;
}



//-----------------------------------------------------------------------------
// Desc   :  본리스트를 만든후 스킨드메쉬에 장착한다.
// Arg	  :  int eRPT , 본리스트를 만들어놓을 스킨메쉬 리소스팩 넘버
//-----------------------------------------------------------------------------
void	CRscPack_Character::Make_BoneList(int eRPT)
{
	std::vector<SHR_Mesh*>::iterator iter;
		
	SHR_SkinMesh* pSkin = dynamic_cast<SHR_SkinMesh*>(m_pRenderer[eRPT]);
	

	for(int i = 0; i< pSkin->GetBoneCnt() ; i++ )
	{
		for(iter = m_pBoneList->GetMeshList()->begin() ; iter != m_pBoneList->GetMeshList()->end() ; ++iter)
		{
			if(strcmp((*iter)->m_MeshInfo.Name, pSkin->m_pBoneName[i]) == 0)
			{
				pSkin->m_Vec_BoneList.push_back(*iter);
				break;
			}
			
		}
	}

}


//-----------------------------------------------------------------------------
// Desc   : 메트리스팔레트를 본개수만큼 동적생성한다.
// Arg	  : int eRPT , 본리스트를 만들어놓을 스킨메쉬 리소스팩 넘버
//-----------------------------------------------------------------------------
void	CRscPack_Character::Make_BoneMatPalette(int eRPT)
{
	SHR_SkinMesh* pSkin = dynamic_cast<SHR_SkinMesh*>(m_pRenderer[eRPT]);

	int BoneCnt = pSkin->m_Vec_BoneList.size();

	pSkin->m_pMatPalette = new D3DXMATRIX[BoneCnt];

}


//-------------------------------------------------------------------------
// Arg	  : 
// Return :
// Desc   :  연산자 오버로딩 [2014.11.17 ]
//-------------------------------------------------------------------------
CRscPack_Character&		CRscPack_Character::operator=(const CRscPack_Character& _rhs )
{
	if(_rhs.m_pBoneList != nullptr)
	{
		m_pBoneList = new SHR_BoneList();
		(*m_pBoneList) = (*_rhs.m_pBoneList);

	}
	
	m_MeshList = _rhs.m_MeshList; //[2014.11.24 ]

	for(int i = 0 ; i< eRPT_Max ; i++)
	{
		if(_rhs.m_pAllRsc[i] != nullptr)// [2014.11.27 ]
		{
			if(eRT_SKIN_MESH  == _rhs.m_pAllRsc[i]->eResourceType)
			{
				SHR_SkinMesh* pSkinMesh = new SHR_SkinMesh();
				SHR_SkinMesh* TempSkin = dynamic_cast<SHR_SkinMesh*>(_rhs.m_pAllRsc[i]);
				assert(TempSkin != nullptr);// 캐스팅 검사
				(*pSkinMesh) = (*TempSkin); //스킨이 가지고있는 내부 리소스들만 복사
				m_pAllRsc[i] = pSkinMesh;

				SetRsc_to_Renderer(pSkinMesh, eRPT_SkinBody);
				SetRsc_to_Updater(pSkinMesh, eRPT_SkinBody);
				Make_BoneMatPalette(eRPT_SkinBody);
			}
			else if(eRT_MESH_NORMAL  == _rhs.m_pAllRsc[i]->eResourceType)
			{
				m_pAllRsc[i] = _rhs.m_pAllRsc[i];
				//SHR_rhs.m_pAllRsc[i]->m 여기서 원래 부모 찾아줘야함 !!★★★ [2014.11.17 ]

				// 			SetRsc_to_Renderer(pSkinMesh, eRPT_SkinBody);
				// 			SetRsc_to_Updater(pSkinMesh, eRPT_SkinBody);
			}
		}
		


	}

	m_Animator = _rhs.m_Animator;

	 //여기서 원래 충돌구 포인터를  찾아줘야함 !!★★★ [2014.11.17 ]
	//m_pRefPoint_BoundSphere = 

	
	return *this;
}

/****************************************************** End *****************************************************/