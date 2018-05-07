#include "DXUT.h"
#include "ResourceMgr.h"


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
// 여기서부터 CResourceMgr 함수정의
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
// 싱글톤 CResourceMgr 인스턴스 반환
//-----------------------------------------------------------------------------
CResourceMgr*	CResourceMgr::getInstance()
{
	if(m_pRscMgr == nullptr) m_pRscMgr = new CResourceMgr();

	return m_pRscMgr;
}


//-----------------------------------------------------------------------------
// 생성자
//-----------------------------------------------------------------------------
CResourceMgr::CResourceMgr()
{
	
}


//-------------------------------------------------------------------------
// 소멸자
//-------------------------------------------------------------------------
CResourceMgr::~CResourceMgr()
{
	std::list<IResource*>::iterator	Iter;//m_List_IResource 의 이터레이터
	//남아있는 모든 리소스들을 지운다.
	for(Iter = m_List_IResource.begin() ; Iter != m_List_IResource.end(); ++Iter)
	{
		SAFE_DELETE_(*Iter)
	}
	m_List_IResource.clear();

}


//-------------------------------------------------------------------------
// Arg	  : _Path(경로), _Name(리소스 이름), _RscType(리소스 타입)
// return : IResource*
// Desc   : 해당하는 _Name의 리소스가 리스트에 등록되어있지 않으면	Path를 이용해
//			해당타입에 따라 로드한후 리스트에 등록함, 그후 IResource* 반환
//			이미 로드되어 있다면 에러메세지 박스 출력후 nullptr 반환
//-------------------------------------------------------------------------
IResource*	CResourceMgr::LoadResource(const WCHAR* _Path, WCHAR* _Name, int _RscType)
{
	IResource* pRes = nullptr;

	// 1) 먼저 해당 이름의 리소스가 이미 로딩되어있는지 찾아본다.
	for(std::list<IResource*>::iterator Iter = m_List_IResource.begin() ; Iter != m_List_IResource.end() ;++Iter )
	{
		if(wcscmp((*Iter)->m_Name, _Name) == 0)
		{
			SHUtil::MsgBox("이미 로딩되어 있는 리소스 입니다.~~!!!");
			return nullptr;
		}
	}

	// 2) 로딩되어있지 않으면 새로 로딩한다.
	switch(_RscType)
	{
	case eRT_BONE_LIST:
		{
 			SHR_BoneList* TempRsc = new SHR_BoneList(_Name, _Path);
			TempRsc->eResourceState = eRS_TOBE_USED;
			TempRsc->LoadResource();
			m_List_IResource.push_back(TempRsc);
			pRes = TempRsc;
		}
		break;

	case eRT_SKIN_MESH:
		{
			SHR_SkinMesh* pTempSkin = new SHR_SkinMesh(_Name, _Path);
			pTempSkin->eResourceState = eRS_TOBE_USED;
			pTempSkin->LoadResource();
			m_List_IResource.push_back(pTempSkin);
			pRes = pTempSkin;
		}
		break;

	case eRT_TEXTURE:
		{
			SHR_D3D9Texture* pTempTex  = new SHR_D3D9Texture();
			pTempTex->Create(_Name, _Path);
			pTempTex->eResourceState = eRS_TOBE_USED;
			m_List_IResource.push_back(pTempTex);
			pRes = pTempTex;
		
		}
		break;

	case eRT_VTXSHADER:
		{	
			SHR_VtxShader* pTempVS = new SHR_VtxShader();//버텍스셰이더 생성
			pTempVS->Create(_Name, _Path);				// .vs 파일 로딩후 셰이더객체 생성
			pTempVS->eResourceState = eRS_TOBE_USED;
			m_List_IResource.push_back(pTempVS);				// 리소스 리스트에 등록
			pRes= pTempVS;
		}
		break;

	case eRT_PIXSHADER:
		{	
			SHR_PixShader* pTempPS = new SHR_PixShader();//픽셀셰이더 생성
			pTempPS->Create(_Name, _Path);				// .ps 파일 로딩후 셰이더객체 생성
			pTempPS->eResourceState = eRS_TOBE_USED;
			m_List_IResource.push_back(pTempPS);				// 리소스 리스트에 등록
			pRes = pTempPS;
		}
		break;
	}



	return pRes;
}


//-----------------------------------------------------------------------------
// Arg    : const WCHAR* _Path, WCHAR* _Name, int _RscType
// Return : Succ(S_OK), Fail(S_FALSE)
// Desc   : 해당하는 _Name의 리소스가 리스트에 등록되어있는지 확인하고	없으면 _Path를 이용해 해당타입에 따라 로드한후 리스트(m_List_IResource)에 등록함..
//			씬이 다른씬으로 바뀔때 여러 리소스들을 읽어들이면서 내부적으로 사용된다.
//-----------------------------------------------------------------------------
IResource* CResourceMgr::_LoadResource(const WCHAR* _Path, WCHAR* _Name, int _RscType)
{
	IResource* res = nullptr;

	// 1) 먼저 해당 이름의 리소스가 이미 로딩되어있는지 찾아본다.
	for(std::list<IResource*>::iterator Iter = m_List_IResource.begin() ; Iter != m_List_IResource.end() ; ++Iter )
	{
		if(wcscmp((*Iter)->m_Name, _Name) == 0)
		{
			(*Iter)->eResourceState = eRS_TOBE_USED; // 리소스 타입을 사용예정으로 바꿔준다.
			return (*Iter);
		}
	}

	// 2) 로딩되어있지 않으면 새로 로딩한다.
	switch(_RscType)
	{
	case eRT_BONE_LIST:
		{
			SHR_BoneList* TempRsc = new SHR_BoneList(_Name, _Path);
			TempRsc->eResourceState = eRS_TOBE_USED;
			TempRsc->LoadResource();
			m_List_IResource.push_back(TempRsc);
			res = TempRsc;
		}
		break;
	case eRT_MESH_LIST:
		{
			SHR_MeshList* TempRsc = new SHR_MeshList(_Name, _Path);
			TempRsc->eResourceState = eRS_TOBE_USED;
			TempRsc->LoadResource();
			m_List_IResource.push_back(TempRsc);
			res = TempRsc;
		}
		break;

	case eRT_SKIN_MESH:
		{
			SHR_SkinMesh* pTempSkin = new SHR_SkinMesh(_Name, _Path);
			pTempSkin->eResourceState = eRS_TOBE_USED;
			pTempSkin->LoadResource();
			m_List_IResource.push_back(pTempSkin);
			res =  pTempSkin;
		}
		break;

	case eRT_TEXTURE:
		{
			SHR_D3D9Texture* pTempTex  = new SHR_D3D9Texture();
			pTempTex->Create(_Name, _Path);
			pTempTex->eResourceState = eRS_TOBE_USED;
			m_List_IResource.push_back(pTempTex);
			res =   pTempTex;
		}
		break;

	case eRT_VTXSHADER:
		{	
			SHR_VtxShader* pTempVS = new SHR_VtxShader();//버텍스셰이더 생성
			pTempVS->Create(_Name, _Path);				// .vs 파일 로딩후 셰이더객체 생성
			pTempVS->eResourceState = eRS_TOBE_USED;
			m_List_IResource.push_back(pTempVS);				// 리소스 리스트에 등록
			res =  pTempVS;
		}
		break;

	case eRT_PIXSHADER:
		{	
			SHR_PixShader* pTempPS = new SHR_PixShader();//픽셀셰이더 생성
			pTempPS->Create(_Name, _Path);				// .ps 파일 로딩후 셰이더객체 생성
			pTempPS->eResourceState = eRS_TOBE_USED;
			m_List_IResource.push_back(pTempPS);				// 리소스 리스트에 등록
			res =  pTempPS;
		}
		break;
	}


	if(res == nullptr)
	{	SHUtil::MsgBox("CResourceMgr::_LoadResource() , 리소스로드실패");}

	return res;
}


//-------------------------------------------------------------------------
// Arg	  : WCHAR* _pPath
// Return : Success(S_Ok) , Fail(S_Fail)
// Desc   : 사용될 리소스들(보통 한캐릭터에서 사용하는 리소스들의 집합)이 
//			정의되어있는 메타파일로 부터 써있는 모든 리소스를 읽어들인다.
//-------------------------------------------------------------------------
int	 CResourceMgr::LoadResources_from_Metafile(const CHAR* _pPath)
{
	FILE* fp = NULL;
	errno_t err;
	err	= fopen_s(&fp, _pPath, "rt");
	if(err != 0)
	{
		//로드 파일 생성 실패..
		SHUtil::MsgBox("CResourceMgr::LoadResources_from_Metafile() : 에러!, 로드 실패");
		return S_FALSE;
	}
	
	CHAR mpath[Max_Path_Len] = {NULL,};
	CHAR mname[Max_Name_Len] = {NULL,};

	//읽어들일 리소스 총 개수 
	int rcnt;
	fscanf_s(fp, "%d", &rcnt);  fseek(fp, 2, SEEK_CUR);// 한줄내림

	for(int i = 0; i< rcnt ; i++)
	{
		int RscType;//, PathSize, NameSize; [2014.11.19 ]
		WCHAR wPath[Max_Path_Len] = {NULL,};CHAR mpath[Max_Path_Len] = {NULL,};
		WCHAR wName[Max_Name_Len] = {NULL,};CHAR mname[Max_Name_Len] = {NULL,};

		fscanf_s(fp, "%d", &RscType);
		fscanf_s(fp, "%s", mpath, Max_Path_Len); fseek(fp, 2, SEEK_CUR);
		fscanf_s(fp, "%s", mname, Max_Name_Len); fseek(fp, 2, SEEK_CUR);
		MultiByteToWideChar(CP_ACP, NULL, mpath, -1, wPath, sizeof(WCHAR)*Max_Path_Len);
		MultiByteToWideChar(CP_ACP, NULL, mname, -1, wName, sizeof(WCHAR)*Max_Path_Len);
		_LoadResource(wPath, wName, RscType);
	}

	fclose(fp);

	return S_OK;
}

//-------------------------------------------------------------------------
// Arg	  : IResource*
// Return : Success(S_Ok) , Fail(S_Fail)
// Desc   : 사용자가 만든 리소스를 리스트에 등록해주는 함수
//-------------------------------------------------------------------------
int	  CResourceMgr::RegisterRsc(IResource* _pRsc)
{

	if(_pRsc == nullptr) 
	{	SHUtil::MsgBox(" CResourceMgr::RegisterRsc() , _pRsc 가 Nullptr");
		return S_FALSE;
	}

	m_List_IResource.push_back(_pRsc);	
	return S_OK;

}


//-----------------------------------------------------------------------------
// Arg    : TCHAR*      (이름)
// Return : IResource*	(이름으로 해당리소스 찾아서 반환)
//-----------------------------------------------------------------------------
IResource*	CResourceMgr::Find_Resource(const WCHAR* _pName)
{
	IResource* Res= nullptr;

	if(_pName==nullptr)
	{   LOG_WRITE_A("[<Fail> CResourceMgr::Find_Resource()] , _pName 이 nullptr 입니다.");
		SHUtil::MsgBox("<Fail> CResourceMgr::Find_Resource() , _pName 이 nullptr 입니다."); 
		assert(false);
	}
	

	std::list<IResource*>::iterator	Iter;
	std::list<IResource*>::iterator	IterEnd;
	IterEnd = m_List_IResource.end(); // end값이 변하지 않을땐 성능을 위해 // [2014.10.30 ]

#ifdef SH_DEBUG_
	LOG_WRITE_A("리소스리스트");
#endif
	
	int aa = 0;
	for(Iter = m_List_IResource.begin() ; Iter != IterEnd; ++Iter)
	{

#ifdef SH_DEBUG_

		WCHAR str[Max_Name_Len] = {NULL,};
		wcscpy_s(str, (*Iter)->m_Name );

		LOG_WRITE_W(L"%d 번리소스 : %s" , aa, str);
		aa++;
#endif
		if(wcscmp((*Iter)->m_Name , _pName) == 0)
		{
			Res = (*Iter);
		}
	}

	if(Res == nullptr)
	{
		LOG_WRITE_A("[CResourceMgr::Find_Resource()] , Fail !!!!!!!!! "  );
		SHUtil::MsgBoxW(L"%s 을 찾을 수 없습니다.", _pName); 
	}
	LOG_WRITE_A("[CResourceMgr::Find_Resource()] , return  Res");
	return Res;
}


//-----------------------------------------------------------------------------
// Desc   : 모든 리소스들의 상태를 eRS_TOBE_DISCARD 로 만든다.
//-----------------------------------------------------------------------------
void	CResourceMgr::ChangeStateDiscard()
{
	std::list<IResource*>::iterator	Iter;//m_List_IResource 의 이터레이터
	//남아있는 모든 리소스들을 지운다.
	for(Iter = m_List_IResource.begin() ; Iter != m_List_IResource.end(); ++Iter)
	{
		(*Iter)->eResourceState = eRS_TOBE_DISCARD;
	}
	
}


//-----------------------------------------------------------------------------
// Desc   : eRS_TOBE_DISCARD상태인 모든 리소스를 제거한다.
//-----------------------------------------------------------------------------
void	CResourceMgr::Discard()
{
	std::list<IResource*>::iterator	Iter;//m_List_IResource 의 이터레이터
	//남아있는 모든 리소스들을 지운다.
// 	for(Iter = m_List_IResource.begin() ; Iter != m_List_IResource.end(); ++Iter)  // 제기랄 나중에 에러 꼭 잡기 !! ★★★★★★★
// 	{
// 		if((*Iter)->eResourceState == eRS_TOBE_DISCARD)
// 		{	IResource* tempRsc = (*Iter);
// 			m_List_IResource.erase(Iter);
// 			delete tempRsc;
// 		}
// 	}


}

//-----------------------------------------------------------------------------
// Arg    : 
// Return : 
// Desc   : 모든 리소스를 제거한다.  //[2014.12.6 ]
//-----------------------------------------------------------------------------
void	CResourceMgr::Release()
{
	std::list<IResource*>::iterator IterIRsc, IterIRscEnd;
	IterIRscEnd = m_List_IResource.end();

	for(IterIRsc = m_List_IResource.begin() ; IterIRsc != IterIRscEnd ; ++IterIRsc)
	{
		SAFE_DELETE_((*IterIRsc))
	}
	m_List_IResource.clear();
	m_List_Restoreable.clear();
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
// 여기서부터 class IResource 함수정의
//
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
IResource::IResource()
{
	eResourceState = eRS_TOBE_DISCARD;

	memset(m_Path, NULL,  sizeof(m_Path));
	memset(m_Name, NULL,  sizeof(m_Name));
}


//-----------------------------------------------------------------------------
// 소멸자
//-----------------------------------------------------------------------------
IResource::~IResource()
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
//
// 여기서부터 class IRefCounter 함수정의
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
//생성자
//-----------------------------------------------------------------------------
IRefCounter::IRefCounter()
{
	m_RefCount = 1;
}
//-----------------------------------------------------------------------------
// 소멸자
//-----------------------------------------------------------------------------
IRefCounter::~IRefCounter()	
{

}



//-----------------------------------------------------------------------------
// 레퍼런스 카운트를 증가시키기
//-----------------------------------------------------------------------------
void	IRefCounter::AddRef()
{
	++m_RefCount;
}
	


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void	IRefCounter::Release()
{
	--m_RefCount;
	if(m_RefCount <= 0)
		delete this;
}



/****************************************************** End *****************************************************/