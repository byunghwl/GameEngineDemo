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
// ���⼭���� CResourceMgr �Լ�����
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
// �̱��� CResourceMgr �ν��Ͻ� ��ȯ
//-----------------------------------------------------------------------------
CResourceMgr*	CResourceMgr::getInstance()
{
	if(m_pRscMgr == nullptr) m_pRscMgr = new CResourceMgr();

	return m_pRscMgr;
}


//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
CResourceMgr::CResourceMgr()
{
	
}


//-------------------------------------------------------------------------
// �Ҹ���
//-------------------------------------------------------------------------
CResourceMgr::~CResourceMgr()
{
	std::list<IResource*>::iterator	Iter;//m_List_IResource �� ���ͷ�����
	//�����ִ� ��� ���ҽ����� �����.
	for(Iter = m_List_IResource.begin() ; Iter != m_List_IResource.end(); ++Iter)
	{
		SAFE_DELETE_(*Iter)
	}
	m_List_IResource.clear();

}


//-------------------------------------------------------------------------
// Arg	  : _Path(���), _Name(���ҽ� �̸�), _RscType(���ҽ� Ÿ��)
// return : IResource*
// Desc   : �ش��ϴ� _Name�� ���ҽ��� ����Ʈ�� ��ϵǾ����� ������	Path�� �̿���
//			�ش�Ÿ�Կ� ���� �ε����� ����Ʈ�� �����, ���� IResource* ��ȯ
//			�̹� �ε�Ǿ� �ִٸ� �����޼��� �ڽ� ����� nullptr ��ȯ
//-------------------------------------------------------------------------
IResource*	CResourceMgr::LoadResource(const WCHAR* _Path, WCHAR* _Name, int _RscType)
{
	IResource* pRes = nullptr;

	// 1) ���� �ش� �̸��� ���ҽ��� �̹� �ε��Ǿ��ִ��� ã�ƺ���.
	for(std::list<IResource*>::iterator Iter = m_List_IResource.begin() ; Iter != m_List_IResource.end() ;++Iter )
	{
		if(wcscmp((*Iter)->m_Name, _Name) == 0)
		{
			SHUtil::MsgBox("�̹� �ε��Ǿ� �ִ� ���ҽ� �Դϴ�.~~!!!");
			return nullptr;
		}
	}

	// 2) �ε��Ǿ����� ������ ���� �ε��Ѵ�.
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
			SHR_VtxShader* pTempVS = new SHR_VtxShader();//���ؽ����̴� ����
			pTempVS->Create(_Name, _Path);				// .vs ���� �ε��� ���̴���ü ����
			pTempVS->eResourceState = eRS_TOBE_USED;
			m_List_IResource.push_back(pTempVS);				// ���ҽ� ����Ʈ�� ���
			pRes= pTempVS;
		}
		break;

	case eRT_PIXSHADER:
		{	
			SHR_PixShader* pTempPS = new SHR_PixShader();//�ȼ����̴� ����
			pTempPS->Create(_Name, _Path);				// .ps ���� �ε��� ���̴���ü ����
			pTempPS->eResourceState = eRS_TOBE_USED;
			m_List_IResource.push_back(pTempPS);				// ���ҽ� ����Ʈ�� ���
			pRes = pTempPS;
		}
		break;
	}



	return pRes;
}


//-----------------------------------------------------------------------------
// Arg    : const WCHAR* _Path, WCHAR* _Name, int _RscType
// Return : Succ(S_OK), Fail(S_FALSE)
// Desc   : �ش��ϴ� _Name�� ���ҽ��� ����Ʈ�� ��ϵǾ��ִ��� Ȯ���ϰ�	������ _Path�� �̿��� �ش�Ÿ�Կ� ���� �ε����� ����Ʈ(m_List_IResource)�� �����..
//			���� �ٸ������� �ٲ� ���� ���ҽ����� �о���̸鼭 ���������� ���ȴ�.
//-----------------------------------------------------------------------------
IResource* CResourceMgr::_LoadResource(const WCHAR* _Path, WCHAR* _Name, int _RscType)
{
	IResource* res = nullptr;

	// 1) ���� �ش� �̸��� ���ҽ��� �̹� �ε��Ǿ��ִ��� ã�ƺ���.
	for(std::list<IResource*>::iterator Iter = m_List_IResource.begin() ; Iter != m_List_IResource.end() ; ++Iter )
	{
		if(wcscmp((*Iter)->m_Name, _Name) == 0)
		{
			(*Iter)->eResourceState = eRS_TOBE_USED; // ���ҽ� Ÿ���� ��뿹������ �ٲ��ش�.
			return (*Iter);
		}
	}

	// 2) �ε��Ǿ����� ������ ���� �ε��Ѵ�.
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
			SHR_VtxShader* pTempVS = new SHR_VtxShader();//���ؽ����̴� ����
			pTempVS->Create(_Name, _Path);				// .vs ���� �ε��� ���̴���ü ����
			pTempVS->eResourceState = eRS_TOBE_USED;
			m_List_IResource.push_back(pTempVS);				// ���ҽ� ����Ʈ�� ���
			res =  pTempVS;
		}
		break;

	case eRT_PIXSHADER:
		{	
			SHR_PixShader* pTempPS = new SHR_PixShader();//�ȼ����̴� ����
			pTempPS->Create(_Name, _Path);				// .ps ���� �ε��� ���̴���ü ����
			pTempPS->eResourceState = eRS_TOBE_USED;
			m_List_IResource.push_back(pTempPS);				// ���ҽ� ����Ʈ�� ���
			res =  pTempPS;
		}
		break;
	}


	if(res == nullptr)
	{	SHUtil::MsgBox("CResourceMgr::_LoadResource() , ���ҽ��ε����");}

	return res;
}


//-------------------------------------------------------------------------
// Arg	  : WCHAR* _pPath
// Return : Success(S_Ok) , Fail(S_Fail)
// Desc   : ���� ���ҽ���(���� ��ĳ���Ϳ��� ����ϴ� ���ҽ����� ����)�� 
//			���ǵǾ��ִ� ��Ÿ���Ϸ� ���� ���ִ� ��� ���ҽ��� �о���δ�.
//-------------------------------------------------------------------------
int	 CResourceMgr::LoadResources_from_Metafile(const CHAR* _pPath)
{
	FILE* fp = NULL;
	errno_t err;
	err	= fopen_s(&fp, _pPath, "rt");
	if(err != 0)
	{
		//�ε� ���� ���� ����..
		SHUtil::MsgBox("CResourceMgr::LoadResources_from_Metafile() : ����!, �ε� ����");
		return S_FALSE;
	}
	
	CHAR mpath[Max_Path_Len] = {NULL,};
	CHAR mname[Max_Name_Len] = {NULL,};

	//�о���� ���ҽ� �� ���� 
	int rcnt;
	fscanf_s(fp, "%d", &rcnt);  fseek(fp, 2, SEEK_CUR);// ���ٳ���

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
// Desc   : ����ڰ� ���� ���ҽ��� ����Ʈ�� ������ִ� �Լ�
//-------------------------------------------------------------------------
int	  CResourceMgr::RegisterRsc(IResource* _pRsc)
{

	if(_pRsc == nullptr) 
	{	SHUtil::MsgBox(" CResourceMgr::RegisterRsc() , _pRsc �� Nullptr");
		return S_FALSE;
	}

	m_List_IResource.push_back(_pRsc);	
	return S_OK;

}


//-----------------------------------------------------------------------------
// Arg    : TCHAR*      (�̸�)
// Return : IResource*	(�̸����� �ش縮�ҽ� ã�Ƽ� ��ȯ)
//-----------------------------------------------------------------------------
IResource*	CResourceMgr::Find_Resource(const WCHAR* _pName)
{
	IResource* Res= nullptr;

	if(_pName==nullptr)
	{   LOG_WRITE_A("[<Fail> CResourceMgr::Find_Resource()] , _pName �� nullptr �Դϴ�.");
		SHUtil::MsgBox("<Fail> CResourceMgr::Find_Resource() , _pName �� nullptr �Դϴ�."); 
		assert(false);
	}
	

	std::list<IResource*>::iterator	Iter;
	std::list<IResource*>::iterator	IterEnd;
	IterEnd = m_List_IResource.end(); // end���� ������ ������ ������ ���� // [2014.10.30 ]

#ifdef SH_DEBUG_
	LOG_WRITE_A("���ҽ�����Ʈ");
#endif
	
	int aa = 0;
	for(Iter = m_List_IResource.begin() ; Iter != IterEnd; ++Iter)
	{

#ifdef SH_DEBUG_

		WCHAR str[Max_Name_Len] = {NULL,};
		wcscpy_s(str, (*Iter)->m_Name );

		LOG_WRITE_W(L"%d �����ҽ� : %s" , aa, str);
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
		SHUtil::MsgBoxW(L"%s �� ã�� �� �����ϴ�.", _pName); 
	}
	LOG_WRITE_A("[CResourceMgr::Find_Resource()] , return  Res");
	return Res;
}


//-----------------------------------------------------------------------------
// Desc   : ��� ���ҽ����� ���¸� eRS_TOBE_DISCARD �� �����.
//-----------------------------------------------------------------------------
void	CResourceMgr::ChangeStateDiscard()
{
	std::list<IResource*>::iterator	Iter;//m_List_IResource �� ���ͷ�����
	//�����ִ� ��� ���ҽ����� �����.
	for(Iter = m_List_IResource.begin() ; Iter != m_List_IResource.end(); ++Iter)
	{
		(*Iter)->eResourceState = eRS_TOBE_DISCARD;
	}
	
}


//-----------------------------------------------------------------------------
// Desc   : eRS_TOBE_DISCARD������ ��� ���ҽ��� �����Ѵ�.
//-----------------------------------------------------------------------------
void	CResourceMgr::Discard()
{
	std::list<IResource*>::iterator	Iter;//m_List_IResource �� ���ͷ�����
	//�����ִ� ��� ���ҽ����� �����.
// 	for(Iter = m_List_IResource.begin() ; Iter != m_List_IResource.end(); ++Iter)  // ����� ���߿� ���� �� ��� !! �ڡڡڡڡڡڡ�
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
// Desc   : ��� ���ҽ��� �����Ѵ�.  //[2014.12.6 ]
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
// ���⼭���� class IResource �Լ�����
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
// ������
//-----------------------------------------------------------------------------
IResource::IResource()
{
	eResourceState = eRS_TOBE_DISCARD;

	memset(m_Path, NULL,  sizeof(m_Path));
	memset(m_Name, NULL,  sizeof(m_Name));
}


//-----------------------------------------------------------------------------
// �Ҹ���
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
// ���⼭���� class IRefCounter �Լ�����
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
//������
//-----------------------------------------------------------------------------
IRefCounter::IRefCounter()
{
	m_RefCount = 1;
}
//-----------------------------------------------------------------------------
// �Ҹ���
//-----------------------------------------------------------------------------
IRefCounter::~IRefCounter()	
{

}



//-----------------------------------------------------------------------------
// ���۷��� ī��Ʈ�� ������Ű��
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