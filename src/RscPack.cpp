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
// ���⼭���� class CRscPack_NormalObj �Լ�����
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
// Desc   : ���ҽ��Ѿȿ��ִ� �븻������Ʈ���� �׷��ش�.
//-------------------------------------------------------------------------
void		CRscPack_NormalObj::Draw(IDirect3DDevice9* device)  //[2014.11.11 ]
{


		m_pMeshList->Draw(device);


}

//-----------------------------------------------------------------------------
// Arg    : float _dTime(delta time),  D3DXMATRIX TM(�θ�����  ��ȯ ���)
// Desc   : f�� �������ִ� ��ü�� ���Լ��� ȣ���Ѵ�.   //[2014.11.14 ]
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

	if(m_pMeshList != nullptr)   // ���� �׻� ���� ������Ʈ �ؾ���
		m_pMeshList->Update(m_Animator, &TempTM);// [2014.10.13 ]

}

//-------------------------------------------------------------------------
// .rpi Ȯ���ڸ� ���� ������ �����ͼ� ���ҽ����� �����Ѵ�. [2014.11.11 ]
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
		// ���� �б� ����
		LOG_WRITE_A("[CRscPack_Character::Build], ����!, �����б� ����");
		SHUtil::MsgBox("[CRscPack_Character::Build], ����!, �����б� ����");  assert(false);
		return S_FALSE;
	}

	int rescnt =0;
	fread(&rescnt, sizeof(rescnt),1, fp);// ���ҽ� �� ���� �б�
	int resType;//, RscPackNum; [2014.11.19 ]

	for(int i = 0 ; i< rescnt ; i++)
	{
		fread(&resType, sizeof(resType),1 ,fp);

		if(resType == eRT_MESH_LIST)
		{
			//int resType;//, RscPackNum;[2014.11.19 ]
			int NameSize = 0; WCHAR Name[Max_Name_Len] = {NULL , };

			fread(&NameSize, sizeof(NameSize),1, fp); // �̸� ���� �б�
			fread(Name, NameSize, 1, fp);
			IResource* pRsc = CResourceMgr::getInstance()->Find_Resource(Name);

			m_pMeshList = new SHR_MeshList();// [2014.10.12 ]
			SHR_MeshList* pTempML = dynamic_cast<SHR_MeshList*>(pRsc);
			if(pTempML == nullptr) 
			{		
				SHUtil::MsgBox("CRscPack_Character::Build(), ���̳���ĳ��Ʈ����"); 
				hr = S_FALSE;
			}

			(*m_pMeshList) = (*pTempML); // ������ ���ҽ����� ���� [2014.10.12 ]

		
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
// ���⼭���� class CRscPack �Լ�����
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
// ������
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
//�Ҹ���
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
// �׷��ֱ�
//-----------------------------------------------------------------------------
void	CRscPack_Character::Draw(IDirect3DDevice9* device)
{
// 	if(m_hTech_Skin == nullptr )  // ��ũ �ڵ� Ȯ���غ��� [2014.10.30 ]
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
// 		// ����Ÿ�� ���� �׸���.
// 		
// 		if( SUCCEEDED( G_ScreenQuad.g_pRenderTargetTexPass[0/*PassNum �ϴ� 0����..[2014.11.19 ]*/]->GetSurfaceLevel( 0, &(G_ScreenQuad.g_pSceneSurface[0]) ) ) )
// 		{//gpSceneRenderTarget�� �����̽��� ������ٶ� �ʿ��� �༮ [2014.11.7 ]
// 			device->SetRenderTarget( 0, G_ScreenQuad.g_pSceneSurface[0] ); // ���� �����̽��� ����Ÿ������ ����(����) [2014.11.7 ]
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

// 	if(m_hTech_noSkin == nullptr )  // ��ũ �ڵ� Ȯ���غ��� [2014.10.30 ]
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
// 		// ����Ÿ�� ���� �׸���.
// 
// 		if( SUCCEEDED( G_ScreenQuad.g_pRenderTargetTexPass[0/*PassNum �ϴ� 0����..[2014.11.19 ]*/]->GetSurfaceLevel( 0, &(G_ScreenQuad.g_pSceneSurface[0]) ) ) )
// 		{//gpSceneRenderTarget�� �����̽��� ������ٶ� �ʿ��� �༮ [2014.11.7 ]
// 			device->SetRenderTarget( 0, G_ScreenQuad.g_pSceneSurface[0] ); // ���� �����̽��� ����Ÿ������ ����(����) [2014.11.7 ]
// 		}

		
	    m_MeshList.Draw(device);
				

// 		G_FXShader.GetPointer()->EndPass();// [2014.10.29 ]
// 	}
// 
// 	G_FXShader.GetPointer()->End();// [2014.10.29 ]
// 

}



//-----------------------------------------------------------------------------
// Arg    : float _dTime(delta time),  D3DXMATRIX TM(�θ�����  ��ȯ ���)
// Desc   : f�� �������ִ� ��ü�� ���Լ��� ȣ���Ѵ�.
//-----------------------------------------------------------------------------
void	CRscPack_Character::Update(float _dTime,  D3DXMATRIX* TM)
{
	D3DXMATRIX TempTM;

	if(TM == nullptr)
	{	D3DXMatrixIdentity(&TempTM);	}
	else
	{	TempTM = *TM;	}

	//1) ���ϸ����͸� ������Ʈ �Ѵ�. [2014.11.23 ]
	m_Animator.AniTimeUpdate();


	//2) ���� �׻� ���� ������Ʈ �ؾ���
	if(m_pBoneList != nullptr)   
		m_pBoneList->Update(m_Animator, &TempTM);// [2014.10.13 ]

	//3) �״��� �޽�����Ʈ�� ������Ʈ�� 
	m_MeshList.Update(m_Animator, & TempTM); //�ݵ�� ��������Ʈ �� ������ ������Ʈ�ϱ� [2014.11.21 ]

	// 4) ���� �������� ��Ų���� ������Ʈ�� 
	for(int i = 0 ; i< eRPT_Max ; i++)
	{
		if(m_pUpdater[i] != nullptr)
			m_pUpdater[i]->Update(m_Animator , &TempTM);
	}
}



//-----------------------------------------------------------------------------
// Arg    : const WCHAR* _pPath( ���ϰ�θ�)
// Return : Succ(S_OK), Fail(S_Fail)
// Desc   : ���ҽ�����(.rpi)�� ������ �ִ� ������ �а� �����Ѵ�.
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
		// ���� �б� ����
		LOG_WRITE_A("[CRscPack_Character::Build], ����!, �����б� ����");
		SHUtil::MsgBox("[CRscPack_Character::Build], ����!, �����б� ����");  assert(false);
		return S_FALSE;
	}

	int rescnt =0;
	fread(&rescnt, sizeof(rescnt),1, fp);// ���ҽ� �� ���� �б�
	int resType;//, RscPackNum;[2014.11.19 ]

	for(int i = 0 ; i< rescnt ; i++)
	{
		fread(&resType, sizeof(resType),1 ,fp);

		if(resType == eRT_BONE_LIST)
		{
//			int resType, RscPackNum; [2014.11.19 ]`
			int NameSize = 0; WCHAR Name[Max_Name_Len] = {NULL , };

			fread(&NameSize, sizeof(NameSize),1, fp); // �̸� ���� �б�
			fread(Name, NameSize, 1, fp);
			IResource* pRsc = CResourceMgr::getInstance()->Find_Resource(Name);
			
			m_pBoneList = new SHR_BoneList();// [2014.10.12 ]
			SHR_BoneList* pTempBL = dynamic_cast<SHR_BoneList*>(pRsc);
			if(pTempBL == nullptr) 
			{		
				SHUtil::MsgBox("CRscPack_Character::Build(),eRT_BONE_LIST ����  ���̳���ĳ��Ʈ����"); 
				hr = S_FALSE;
			}

			(*m_pBoneList) = (*pTempBL); // ������ ���ҽ����� ���� [2014.10.12 ]

			//m_pRefPoint_BoundSphere = m_pBoneList->FindMeshByName("BS_RHand"); [2014.11.20 ]
		}
		else if(resType == eRT_SKIN_MESH)
		{
			int  RscPackNum;// resType, [2014.11.19 ]
			int NameSize = 0; WCHAR Name[Max_Name_Len] = {NULL , };

			fread(&RscPackNum, sizeof(int), 1, fp);
			fread(&NameSize, sizeof(NameSize),1, fp); // �̸� ���� �б�
			fread(Name, NameSize, 1, fp);
			IResource* pRsc = CResourceMgr::getInstance()->Find_Resource(Name);
			
			SHR_SkinMesh* pSkin = new SHR_SkinMesh(); //��Ų ���λ���
			SHR_SkinMesh* TempSkin = dynamic_cast<SHR_SkinMesh*>(pRsc);//�̸����� ã�� ��Ų������ ���
			assert(TempSkin != nullptr);// ĳ���� �˻�
			(*pSkin) = (*TempSkin); //��Ų�� �������ִ� ���� ���ҽ��鸸 ����

			m_pAllRsc[RscPackNum] = pSkin;//new�� ������ �����ʹ� ��� ���ҽ��� �����ϴ� �迭�� �־� �����Ѵ�.
			m_pSkinMesh = pSkin; //[2014.11.21 ]

			hr = SetRsc_to_Renderer(pSkin, RscPackNum);
			hr = SetRsc_to_Updater(pSkin, RscPackNum);	
			// ��Ų�޽��� ������Ʈ ����
			Make_BoneList(RscPackNum);
			// ��Ų�޽��� ����Ʈ���� �ȷ�Ʈ ����
			Make_BoneMatPalette(RscPackNum);
		
		}
		else if(resType == eRT_MESH_LIST) //[2014.11.21 ]
		{
			//int resType, RscPackNum; //[2014.11.19 ]`
			int mNameSize = 0; WCHAR MeshListName[Max_Name_Len] = {NULL , };

			fread(&mNameSize, sizeof(mNameSize),1, fp); // �̸� ���� �б�
			fread(MeshListName, mNameSize, 1, fp);
			IResource* pRsc = CResourceMgr::getInstance()->Find_Resource(MeshListName);

			
			SHR_MeshList* pTempML = dynamic_cast<SHR_MeshList*>(pRsc);
			if(pTempML == nullptr) 
			{		
				SHUtil::MsgBox("CRscPack_Character::Build(),  eRT_MESH_LIST ���� ���̳���ĳ��Ʈ����"); 
				hr = S_FALSE;
			}

			m_MeshList = (*pTempML); // ������ ���ҽ����� ���� [2014.10.12 ]

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
		SHUtil::MsgBox("CRscPack_Character::SetRsc_to_Renderer(), dynamic_cast ����");
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
		SHUtil::MsgBox("CRscPack_Character::SetRsc_to_Updater, dynamic_cast ����");
		return S_FALSE;
	}

	m_pUpdater[eRPTNum]  = pUp;
	return S_OK;
}



//-----------------------------------------------------------------------------
// Arg    : int _enumVal(), enum eRscName 
// Return : ���н� Nullptr���� , ������ �ش� ���ҽ������� ��ȯ
// Desc   : _enumVal�� ����ؼ� ��Ų�޽��� ���ҽ��� Ÿ��ĳ�����ؼ� ��ȯ�մϴ�. 
//			���н� �޼����ڽ��� Nullptr�� ����
//-----------------------------------------------------------------------------
SHR_SkinMesh*	CRscPack_Character::GetSkinMesh(int _enumVal)
{
	IRenderer* pRen = m_pRenderer[_enumVal];

	SHR_SkinMesh* pSkin = dynamic_cast<SHR_SkinMesh*>(pRen);

	if(pSkin== nullptr)
	{
		SHUtil::MsgBox("CRscPack_Character::GetSkinMesh(), Ÿ��ĳ���� ����!!");
		return nullptr;
	}

	return pSkin;

}

//-----------------------------------------------------------------------------
// Arg    : int _enumVal(), enum eRscName 
// Return : ���н� Nullptr���� , ������ �ش� ���ҽ������� ��ȯ
// Desc   : _enumVal�� ����ؼ� �޽�����Ʈ�� ���ҽ��� Ÿ��ĳ�����ؼ� ��ȯ�մϴ�
//          ���н� �޼����ڽ��� Nullptr�� ����
//-----------------------------------------------------------------------------
// SHR_BoneList*	CRscPack_Character::GetBoneList()
// {
// 	IRenderer* pRen = m_pRenderer[_enumVal];
// 
// 	SHR_BoneList* pMeshList = dynamic_cast<SHR_BoneList*>(pRen);
// 
// 	if(pMeshList== nullptr)
// 	{
// 		SHUtil::MsgBox("CRscPack_Character::GetMeshList(), Ÿ��ĳ���� ����!!");
// 		return nullptr;
// 	}
// 
// 	return pMeshList;
// }

//-----------------------------------------------------------------------------
// Arg    : int _enumVal(), enum eRscName 
// Return : ���н� Nullptr���� , ������ �ش� ���ҽ������� ��ȯ
// Desc   : _enumVal�� ����ؼ� �޽��� ���ҽ��� Ÿ��ĳ�����ؼ� ��ȯ�մϴ�. 
//			���н� �޼����ڽ��� Nullptr�� ����
//-----------------------------------------------------------------------------

SHR_Mesh*	CRscPack_Character::GetMesh(int _enumVal)
{
	IResource* pRen = m_pAllRsc[_enumVal];

	SHR_Mesh* pMesh = dynamic_cast<SHR_Mesh*>(pRen);

	if(pMesh == nullptr)
	{
		SHUtil::MsgBox("CRscPack_Character::GetMesh(), Ÿ��ĳ���� ����!!");
		return nullptr;
	}

	return pMesh;
}



//-----------------------------------------------------------------------------
// Desc   :  ������Ʈ�� ������ ��Ų��޽��� �����Ѵ�.
// Arg	  :  int eRPT , ������Ʈ�� �������� ��Ų�޽� ���ҽ��� �ѹ�
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
// Desc   : ��Ʈ�����ȷ�Ʈ�� ��������ŭ ���������Ѵ�.
// Arg	  : int eRPT , ������Ʈ�� �������� ��Ų�޽� ���ҽ��� �ѹ�
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
// Desc   :  ������ �����ε� [2014.11.17 ]
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
				assert(TempSkin != nullptr);// ĳ���� �˻�
				(*pSkinMesh) = (*TempSkin); //��Ų�� �������ִ� ���� ���ҽ��鸸 ����
				m_pAllRsc[i] = pSkinMesh;

				SetRsc_to_Renderer(pSkinMesh, eRPT_SkinBody);
				SetRsc_to_Updater(pSkinMesh, eRPT_SkinBody);
				Make_BoneMatPalette(eRPT_SkinBody);
			}
			else if(eRT_MESH_NORMAL  == _rhs.m_pAllRsc[i]->eResourceType)
			{
				m_pAllRsc[i] = _rhs.m_pAllRsc[i];
				//SHR_rhs.m_pAllRsc[i]->m ���⼭ ���� �θ� ã������� !!�ڡڡ� [2014.11.17 ]

				// 			SetRsc_to_Renderer(pSkinMesh, eRPT_SkinBody);
				// 			SetRsc_to_Updater(pSkinMesh, eRPT_SkinBody);
			}
		}
		


	}

	m_Animator = _rhs.m_Animator;

	 //���⼭ ���� �浹�� �����͸�  ã������� !!�ڡڡ� [2014.11.17 ]
	//m_pRefPoint_BoundSphere = 

	
	return *this;
}

/****************************************************** End *****************************************************/