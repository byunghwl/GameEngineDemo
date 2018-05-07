/***********************************************************************
                                SceneMgr.cpp                                   
************************************************************************/


#include "DXUT.h"
#include "Camera.h"
#include "FrameWork.h"
#include "SceneMgr.h"


//////////////////////////////////////////////////////////////////////////
CSceneMgr*		CSceneMgr::m_pSceneMgr = nullptr;//�̱���




///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
// class CScene  �Լ� ����
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
CScene::CScene()
{
	memset(m_SceneName, NULL, sizeof(m_SceneName));

	m_pCamera = nullptr;


}


//-----------------------------------------------------------------------------
// �Ҹ���
//-----------------------------------------------------------------------------
CScene::~CScene()
{
	for( std::list<CGameObject*>::iterator iterPos = m_list_GameObjs.begin(); iterPos != m_list_GameObjs.end(); )
	{		
		CGameObject* temp = m_list_GameObjs.front();
		if(temp != nullptr)
		{
			iterPos = m_list_GameObjs.erase(iterPos++);
			delete temp;
		}

	}
}

//-----------------------------------------------------------------------------
// Arg    : 
// Return : 
// Desc   : �̾��� �ڿ��� ��ü���� ����
//-----------------------------------------------------------------------------
void	CScene::Release()
{
	for( std::list<CGameObject*>::iterator iterPos = m_list_GameObjs.begin(); iterPos != m_list_GameObjs.end(); ++iterPos)
	{	
		
		CGhost* pG = dynamic_cast<CGhost*>((*iterPos));  // �ӽù��� ���߿� SharedPtr �����ؾ��ذ��
		if(pG == nullptr)// �ӽù��� ���߿� SharedPtr �����ؾ��ذ�� ����������Ͼ ����.. �ڡڡڡ�
		{
			CGameObject* pTemp = (*iterPos);
			delete pTemp;
			(*iterPos) = NULL;
		}// �ӽù��� ���߿� SharedPtr �����ؾ��ذ��
	}
	SAFE_DELETE_(G_pGhost_Origne)
	m_list_GameObjs.clear();
	m_list_RenderObjs.clear();
	m_list_MessagableObjs.clear();
	m_list_ProgressObjs.clear();
	m_list_PickableObjs.clear();

}

//-----------------------------------------------------------------------------
// Object���� ������ �°� ����Ѵ�.
//-----------------------------------------------------------------------------
bool	CScene::AddObject(CGameObject* pObject )
{
	IRenderable* pIR = dynamic_cast<IRenderable*>(pObject);
	if(pIR != NULL)
	{
		m_list_RenderObjs.push_back(pIR);
	}

	IProgressable* pPR = dynamic_cast<IProgressable*>(pObject);
	if(pPR != NULL)
	{
		m_list_ProgressObjs.push_back(pPR);
	}

	IMessagable* pCt = dynamic_cast<IMessagable*>(pObject);
	if(pCt != NULL)
	{
		m_list_MessagableObjs.push_back(pCt);
	}

	CGameObject* pGO = dynamic_cast<CGameObject*>(pObject);
	if(pCt != NULL)
	{
		m_list_GameObjs.push_back(pGO);
	}

	IPickable* pPk = dynamic_cast<IPickable*>(pObject);
	if(pCt != NULL)
	{
		m_list_PickableObjs.push_back(pPk);
	}

	IAlphaBlendable* pAlpha = dynamic_cast<IAlphaBlendable*>(pObject);
	if(pAlpha != nullptr)
	{
		m_Vec_AlphaBlendObjs.push_back(pAlpha);		
	}
	

	return true;
}



//-----------------------------------------------------------------------------
// Arg    : 
// Return : 
// Desc   :
//-----------------------------------------------------------------------------
DWORD	CScene::Notify(SH_MSG* _pMessage)
{
	HRESULT hr = S_OK;

	


	return hr;
}

//-------------------------------------------------------------------------
// Arg	  : const WCHAR* _pPath , ���ϰ��
// Return : Succ(S_OK), Fail(S_FALSE)
// Desc   : ���Ϸκ��� ���ҽ������ �о� ���ҽ��� �ε��Ѵ�.
//-------------------------------------------------------------------------
int		CScene::LoadRsc_FromFile(const CHAR* _pPath)
{
	int hr = S_OK;

	FILE* fp = NULL;
	errno_t err;

	CHAR pPath[Max_Path_Len];
	strcpy_s(pPath, "./scene/");
	strcat_s(pPath, _pPath);

	err	= fopen_s(&fp, pPath, "rt");

	if(err != 0)
	{
		// ���� �б� ����
		SHUtil::MsgBox("SHR_SkinMesh::LoadRsc_FromFile(),  �����б� ����");assert(false);
		fclose(fp);
		return S_FALSE;
	}
	
	CHAR  RscMeta[Max_Path_Len] = {NULL,};
	
	int RscCnt; // ���ҽ� ��Ÿ���� ����
	
	fscanf_s(fp, "%d", &RscCnt);
	fseek(fp, 2, SEEK_CUR);
	for(int i = 0; i< RscCnt ; i++)
	{
		fscanf_s(fp,"%s", RscMeta, Max_Path_Len );
		fseek(fp,2,SEEK_CUR);
			// ���� ���ҽ���(���� ��ĳ���Ϳ��� ����ϴ� ���ҽ����� ����)�� ���ǵǾ��ִ� ��Ÿ���Ϸ� ���� ���ҽ��� �о���δ�.
		hr = CResourceMgr::getInstance()->LoadResources_from_Metafile(RscMeta);
	}

	//////////////////////////////////////////////////////////////////////////
	fclose(fp);
	return hr;
}



//-----------------------------------------------------------------------------
// ����� ������Ʈ
//-----------------------------------------------------------------------------
void	CScene::Update(float _dTime)
{
	OnUpdateStart(_dTime);
						
	for( std::list<IProgressable*>::iterator iterPos = m_list_ProgressObjs.begin(); iterPos != m_list_ProgressObjs.end(); ++iterPos )
	{
		(*iterPos)->Update(_dTime);
	}



	OnUpdateEnd();
}


//-----------------------------------------------------------------------------
// current Scene render
//-----------------------------------------------------------------------------
void	CScene::Render()
{	
	// ����� ������ �Լ�
	OnRenderStart();
	
	for( std::list<IRenderable*>::iterator iterPos = m_list_RenderObjs.begin(); iterPos != m_list_RenderObjs.end(); ++iterPos )
	{
		(*iterPos)->Draw();
	}

	AlphaSorting(m_pCamera);


	while(!m_PriQue_AlphaSort.empty())
	{
		IAlphaBlendable* pAlpha = m_PriQue_AlphaSort.top();
		pAlpha->DrawAlpha();
		
		m_PriQue_AlphaSort.pop(); // [2014.12.8 ]
		
	}


// 	for( std::list<IAlphaBlendable*>::iterator iterPos = m_List_AlphaBlendObjs.begin(); iterPos != m_List_AlphaBlendObjs.end(); ++iterPos )
// 	{
// 		(*iterPos)->DrawAlpha();
// 	}
	
	// ����� ������ �Լ�
	OnRenderEnd();
}


//-----------------------------------------------------------------------------
// Arg    : 
// Return :
// Desc   : m_List_AlphaBlendObjs �� ������ִ� ��ƼƼ���� ī�޶��� �Ÿ��� ������ؼ� 
//          �װ��� �������ش�. �׸��� �켱����ť�� �־��ִ� ���ұ��� �Ѵ�.. 
//-----------------------------------------------------------------------------
void	CScene::AlphaSorting(CCamera* _pCam  )
{
	//	static_cast<IAlphaBlendable*>::
	std::vector<IAlphaBlendable*>::iterator IterEnd, IterCur;
	IterEnd = m_Vec_AlphaBlendObjs.end();
	
	for(IterCur = m_Vec_AlphaBlendObjs.begin() ; IterCur != IterEnd ; ++IterCur)
	{
		(*IterCur)->CalDistToCam(_pCam->GetPos());
		m_PriQue_AlphaSort.push((*IterCur));   // �켱����ť�� �־��ֱ�  [2014.12.8 ]
	
	}


    // std::sort(m_PriQue_AlphaSort.begin(), m_PriQue_AlphaSort.end(), CompareForAlphaSort);
}

//-----------------------------------------------------------------------------
// ���� �޽��� ó�� �Լ�
//-----------------------------------------------------------------------------
void	CScene::ProcessMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SH_MSG tempMsg;
	tempMsg.m_hWnd = hWnd;
	tempMsg.m_WinMsg = uMsg;
	tempMsg.m_wParam = wParam;
	tempMsg.m_lParam = lParam;

	for( std::list<IMessagable*>::iterator iterPos = m_list_MessagableObjs.begin(); iterPos != m_list_MessagableObjs.end(); ++iterPos )
	{
		(*iterPos)->MsgProc(&tempMsg);
	}
}

//-----------------------------------------------------------------------------
// Arg    : CCamera* 
// Return : Success (S_OK) , Fail(S_FALSE)
// Desc   : ������� ī�޶� �����Ѵ�.
//-----------------------------------------------------------------------------
int		CScene::_SetCamera(CCamera* _pCamera)
{
	if(_pCamera == nullptr)
	{	SHUtil::MsgBox("Fail : CScene::_SetCamera()~~");   return S_FALSE;}

	m_pCamera = _pCamera ;

	return S_OK;
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
// ���⼭���� class CSceneMgr  �Լ� ����
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
//������
//-------------------------------------------------------------------------
CSceneMgr::CSceneMgr() : m_pCurScene(nullptr)
{
	
}



//-------------------------------------------------------------------------
//�Ҹ���
//-------------------------------------------------------------------------
CSceneMgr::~CSceneMgr()
{

	for( std::list<CScene*>::iterator iterPos = m_List_Scene.begin(); iterPos != m_List_Scene.end(); )
	{		
		CScene* temp = m_List_Scene.front();
		if(temp != nullptr)
		{
			iterPos = m_List_Scene.erase(iterPos++);
			delete temp;
		}

	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int	 CSceneMgr::AddScene(const WCHAR* _pName,  CScene* _pScene)
{
	if(	(_pName == NULL)	 ||		(wcslen(_pName) <= 0)	||	(_pScene == nullptr)	)
	{	SHUtil::MsgBox("Fail AddScene() ");   return S_FALSE;     }

	wcscpy_s(_pScene->m_SceneName, _pName);

	m_List_Scene.push_back(_pScene);

	return S_OK;

}

//-------------------------------------------------------------------------
// �̱��� �ν��Ͻ� ��ȯ
//-------------------------------------------------------------------------
CSceneMgr*	CSceneMgr::getInstance()
{	
	if (m_pSceneMgr == nullptr) m_pSceneMgr = new CSceneMgr();
		
	return m_pSceneMgr; 
}



//-------------------------------------------------------------------------
// ����� �׸���
//-------------------------------------------------------------------------
void	CSceneMgr::RenderCurScene()
{
	if(m_pCurScene != nullptr)
		m_pCurScene->Render();
}

//-----------------------------------------------------------------------------
// Arg    : 
// Return :
// Desc   : ���� ���������ִ� ���� ������ UI�� �׸���. // [2014.12.1 ]
//-----------------------------------------------------------------------------
void	CSceneMgr::RenderUI_of_CulScene(float _DTime)
{
	if(m_pCurScene == nullptr){  LOG_WRITE_A("m_pCurScene is Nullptr !@!@!@!");  return;}

		m_pCurScene->RenderText(_DTime);


		// UI�׷��ִ� �Լ�  // [2014.12.1 ]
		m_pCurScene->RenderUI( _DTime );


}

//-----------------------------------------------------------------------------
// ������� ������Ʈ
//-----------------------------------------------------------------------------
void	CSceneMgr::UpdateCurScene(float _DTime)
{
	if(m_pCurScene != nullptr)
		m_pCurScene->Update(_DTime);
}


//-----------------------------------------------------------------------------
// ������� �޽����� ó���Ѵ�
//-----------------------------------------------------------------------------
void	CSceneMgr::ProcessCurSceneMsg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(m_pCurScene != nullptr)
		m_pCurScene->ProcessMsg(hWnd, message, wParam,lParam);

	switch (message)
	{
	case WM_LBUTTONDOWN:

		break;
	case WM_LBUTTONUP:

		break;
	case WM_MOUSEMOVE:		

		break;
	case WM_KEYDOWN:		

		break;
	case WM_KEYUP:	

		break;

	case WM_DESTROY:

		break;
	default:
		break;
	}
}



//-----------------------------------------------------------------------------
// Name   : OpenScene
// Arg    : const WCHAR*	(�� �̸�)
// Return : int				(���� return S_FALSE , ���� return S_OK )
// Desc   : _pName ���� �ش���� ã�� ��������� �ٲ۴�. �̷̹ε�Ǿ��ִ� ���ҽ��� �ε��� �ʿ� ����. [2014.9.3 ]
//-----------------------------------------------------------------------------
int		CSceneMgr::OpenScene(const WCHAR* _pName)
{
	int res = S_OK;
	//����ó��
	if(_pName == nullptr)
	{	
		SHUtil::MsgBox("CSceneMgr::OpenScene() , _pName �� ���Դϴ�."); assert(false);
		return S_FALSE;
	}

	// ����ó��
	if(m_pCurScene == nullptr) //��ϵȾ��� �ϳ��� ���ٸ�
	{	
		m_pCurScene = GetSceneByName(_pName);  //�̸����� ã��
		
		////////////////////////////////////////////////////////////////////////////////////////
		// ��������� �ٲ� ���� Dialog��  ��������ش�. [2014.11.5 ]
// 		std::vector<CString>::iterator iter, endIter ;
// 		endIter = m_pCurScene->m_stlVec_DialogNames.end();
// 		int DlgsSize = CDialogRscMgr::GetInstance()->m_Dialogs.GetSize();

// 		for(iter = m_pCurScene->m_stlVec_DialogNames.begin() ; iter != endIter ; ++iter)
// 		{
// 			for(int i = 0; i < DlgsSize ; i++ )
// 			{
// 				CDXUTDialog* pDlg = CDialogRscMgr::GetInstance()->m_Dialogs.GetAt(i);
// 				if((*iter) == pDlg->GetName() ) // �����̸��� �ִٸ� 
// 					pDlg->SetMediator(m_pCurScene);  // �̰��ٽ� !! [2014.11.5 ]
// 			}
// 
// 		}
		
		return m_pCurScene->InitObjects();	//
	}
	
	//////////////////////////////////////////////////////////////////////////
	// ������� nullptr�� �ƴ϶�� ���ο������ �ٲٱ����� ���������ϵ� ��� ��ü���
	// �ڿ����� ������������Ѵ�.  !!!! [2014.12.6 ]

	m_pCurScene->Release();
	CResourceMgr::getInstance()->Release();


	///////////////////////////////////////////////////////////////////////////////////
	// ������� �ٸ����� �̹� ��ϵǾ� �ִٸ� �� �ٲ��ֱ�
	
	// 1) ��� ���ҽ��� eRS_TOBE_DISCARD ���·� �ٲ۴�.
	CResourceMgr::getInstance()->ChangeStateDiscard();
	// 2) �ٲܾ��� ã�ƿ´�.
	m_pCurScene = GetSceneByName(_pName);  //�̸����� ã��
	// 3) �ٲܾ��� �ʿ��� ��縮�ҽ��� �ε��ϸ鼭 �̹� �ε�Ǿ��ִ°��� �ٽ� ���¸� eRS_TOBE_USED�� �ٲ۴�.
	res = m_pCurScene->InitObjects();
	// 4) eRS_TOBE_DISCARD �� ���·� �����ִ� ��� ���ҽ��� �޸𸮸� �����Ѵ�.
	CResourceMgr::getInstance()->Discard();

	////////////////////////////////////////////////////////////////////////////////////////
	// ��������� �ٲ� ���� Dialog��  ��������ش�. [2014.11.5 ]
// 	std::vector<CString>::iterator iter, endIter ;
// 	endIter = m_pCurScene->m_stlVec_DialogNames.end();
// 	int DlgsSize = CDialogRscMgr::GetInstance()->m_Dialogs.GetSize();
// 
// 	for(iter = m_pCurScene->m_stlVec_DialogNames.begin() ; iter != endIter ; ++iter)
// 	{
// 		for(int i = 0; i < DlgsSize ; i++ )
// 		{
// 			CDXUTDialog* pDlg = CDialogRscMgr::GetInstance()->m_Dialogs.GetAt(i);
// 			if((*iter) == pDlg->GetName() ) // �����̸��� �ִٸ� 
// 			pDlg->SetMediator(m_pCurScene);  // �̰��ٽ� !! [2014.11.5 ]
// 		}
// 				
// 	}
	

	return res;

}

//-----------------------------------------------------------------------------
// Arg    : const WCHAR*
// Return : Success = CScene* ,  Fail = nullptr
// Desc   : _pName���� ã�����ϴ� ���� ã�Ƽ� �����͸� ��ȯ�Ѵ�.
//-----------------------------------------------------------------------------

CScene*		CSceneMgr::GetSceneByName(const WCHAR* _pName)
{
	for(SceneIter= m_List_Scene.begin(); SceneIter != m_List_Scene.end() ; ++SceneIter)
	{
		if(wcscmp( (*SceneIter)->m_SceneName, _pName) == 0 ) //�̸��� ���ٸ�
		{
			return (*SceneIter);
		}
	}
	SHUtil::MsgBox("��ã�� ����~~ �߸��� �� �̸�~~");
	return nullptr;
}