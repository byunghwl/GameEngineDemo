/***********************************************************************
                                SceneMgr.cpp                                   
************************************************************************/


#include "DXUT.h"
#include "Camera.h"
#include "FrameWork.h"
#include "SceneMgr.h"


//////////////////////////////////////////////////////////////////////////
CSceneMgr*		CSceneMgr::m_pSceneMgr = nullptr;//싱글톤




///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
// class CScene  함수 구현
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
// 생성자
//-----------------------------------------------------------------------------
CScene::CScene()
{
	memset(m_SceneName, NULL, sizeof(m_SceneName));

	m_pCamera = nullptr;


}


//-----------------------------------------------------------------------------
// 소멸자
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
// Desc   : 이씬의 자원과 객체들을 해제
//-----------------------------------------------------------------------------
void	CScene::Release()
{
	for( std::list<CGameObject*>::iterator iterPos = m_list_GameObjs.begin(); iterPos != m_list_GameObjs.end(); ++iterPos)
	{	
		
		CGhost* pG = dynamic_cast<CGhost*>((*iterPos));  // 임시방편 나중에 SharedPtr 적용해야해결됨
		if(pG == nullptr)// 임시방편 나중에 SharedPtr 적용해야해결됨 얕은복사로일어난 문제.. ★★★★
		{
			CGameObject* pTemp = (*iterPos);
			delete pTemp;
			(*iterPos) = NULL;
		}// 임시방편 나중에 SharedPtr 적용해야해결됨
	}
	SAFE_DELETE_(G_pGhost_Origne)
	m_list_GameObjs.clear();
	m_list_RenderObjs.clear();
	m_list_MessagableObjs.clear();
	m_list_ProgressObjs.clear();
	m_list_PickableObjs.clear();

}

//-----------------------------------------------------------------------------
// Object들을 종류에 맞게 등록한다.
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
// Arg	  : const WCHAR* _pPath , 파일경로
// Return : Succ(S_OK), Fail(S_FALSE)
// Desc   : 파일로부터 리소스목록을 읽어 리소스를 로딩한다.
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
		// 파일 읽기 실패
		SHUtil::MsgBox("SHR_SkinMesh::LoadRsc_FromFile(),  파일읽기 실패");assert(false);
		fclose(fp);
		return S_FALSE;
	}
	
	CHAR  RscMeta[Max_Path_Len] = {NULL,};
	
	int RscCnt; // 리소스 메타파일 개수
	
	fscanf_s(fp, "%d", &RscCnt);
	fseek(fp, 2, SEEK_CUR);
	for(int i = 0; i< RscCnt ; i++)
	{
		fscanf_s(fp,"%s", RscMeta, Max_Path_Len );
		fseek(fp,2,SEEK_CUR);
			// 사용될 리소스들(보통 한캐릭터에서 사용하는 리소스들의 집합)이 정의되어있는 메타파일로 부터 리소스를 읽어들인다.
		hr = CResourceMgr::getInstance()->LoadResources_from_Metafile(RscMeta);
	}

	//////////////////////////////////////////////////////////////////////////
	fclose(fp);
	return hr;
}



//-----------------------------------------------------------------------------
// 현재씬 업데이트
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
	// 사용자 재정의 함수
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
	
	// 사용자 재정의 함수
	OnRenderEnd();
}


//-----------------------------------------------------------------------------
// Arg    : 
// Return :
// Desc   : m_List_AlphaBlendObjs 에 담겨져있는 엔티티들을 카메라의 거리와 계산을해서 
//          그값을 저장해준다. 그리고 우선순위큐에 넣어주는 역할까지 한다.. 
//-----------------------------------------------------------------------------
void	CScene::AlphaSorting(CCamera* _pCam  )
{
	//	static_cast<IAlphaBlendable*>::
	std::vector<IAlphaBlendable*>::iterator IterEnd, IterCur;
	IterEnd = m_Vec_AlphaBlendObjs.end();
	
	for(IterCur = m_Vec_AlphaBlendObjs.begin() ; IterCur != IterEnd ; ++IterCur)
	{
		(*IterCur)->CalDistToCam(_pCam->GetPos());
		m_PriQue_AlphaSort.push((*IterCur));   // 우선순위큐에 넣어주기  [2014.12.8 ]
	
	}


    // std::sort(m_PriQue_AlphaSort.begin(), m_PriQue_AlphaSort.end(), CompareForAlphaSort);
}

//-----------------------------------------------------------------------------
// 씬의 메시지 처리 함수
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
// Desc   : 현재씬의 카메라를 설정한다.
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
// 여기서부터 class CSceneMgr  함수 구현
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
//생성자
//-------------------------------------------------------------------------
CSceneMgr::CSceneMgr() : m_pCurScene(nullptr)
{
	
}



//-------------------------------------------------------------------------
//소멸자
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
// 싱글톤 인스턴스 반환
//-------------------------------------------------------------------------
CSceneMgr*	CSceneMgr::getInstance()
{	
	if (m_pSceneMgr == nullptr) m_pSceneMgr = new CSceneMgr();
		
	return m_pSceneMgr; 
}



//-------------------------------------------------------------------------
// 현재씬 그리기
//-------------------------------------------------------------------------
void	CSceneMgr::RenderCurScene()
{
	if(m_pCurScene != nullptr)
		m_pCurScene->Render();
}

//-----------------------------------------------------------------------------
// Arg    : 
// Return :
// Desc   : 현재 보여지고있는 씬에 부착된 UI를 그린다. // [2014.12.1 ]
//-----------------------------------------------------------------------------
void	CSceneMgr::RenderUI_of_CulScene(float _DTime)
{
	if(m_pCurScene == nullptr){  LOG_WRITE_A("m_pCurScene is Nullptr !@!@!@!");  return;}

		m_pCurScene->RenderText(_DTime);


		// UI그려주는 함수  // [2014.12.1 ]
		m_pCurScene->RenderUI( _DTime );


}

//-----------------------------------------------------------------------------
// 현재씬의 업데이트
//-----------------------------------------------------------------------------
void	CSceneMgr::UpdateCurScene(float _DTime)
{
	if(m_pCurScene != nullptr)
		m_pCurScene->Update(_DTime);
}


//-----------------------------------------------------------------------------
// 현재씬의 메시지를 처리한다
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
// Arg    : const WCHAR*	(씬 이름)
// Return : int				(에러 return S_FALSE , 성공 return S_OK )
// Desc   : _pName 으로 해당씬을 찾아 현재씬으로 바꾼다. 이미로드되어있는 리소스는 로드할 필요 없다. [2014.9.3 ]
//-----------------------------------------------------------------------------
int		CSceneMgr::OpenScene(const WCHAR* _pName)
{
	int res = S_OK;
	//예외처리
	if(_pName == nullptr)
	{	
		SHUtil::MsgBox("CSceneMgr::OpenScene() , _pName 이 널입니다."); assert(false);
		return S_FALSE;
	}

	// 예외처리
	if(m_pCurScene == nullptr) //등록된씬이 하나도 없다면
	{	
		m_pCurScene = GetSceneByName(_pName);  //이름으로 찾기
		
		////////////////////////////////////////////////////////////////////////////////////////
		// 현재씬으로 바뀔때 사용될 Dialog도  연결시켜준다. [2014.11.5 ]
// 		std::vector<CString>::iterator iter, endIter ;
// 		endIter = m_pCurScene->m_stlVec_DialogNames.end();
// 		int DlgsSize = CDialogRscMgr::GetInstance()->m_Dialogs.GetSize();

// 		for(iter = m_pCurScene->m_stlVec_DialogNames.begin() ; iter != endIter ; ++iter)
// 		{
// 			for(int i = 0; i < DlgsSize ; i++ )
// 			{
// 				CDXUTDialog* pDlg = CDialogRscMgr::GetInstance()->m_Dialogs.GetAt(i);
// 				if((*iter) == pDlg->GetName() ) // 같은이름이 있다면 
// 					pDlg->SetMediator(m_pCurScene);  // 이게핵심 !! [2014.11.5 ]
// 			}
// 
// 		}
		
		return m_pCurScene->InitObjects();	//
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 현재씬이 nullptr이 아니라면 새로운씬으로 바꾸기전에 현재씬에등록된 모든 객체들과
	// 자원들을 삭제시켜줘야한다.  !!!! [2014.12.6 ]

	m_pCurScene->Release();
	CResourceMgr::getInstance()->Release();


	///////////////////////////////////////////////////////////////////////////////////
	// 현재씬에 다른씬이 이미 등록되어 있다면 씬 바꿔주기
	
	// 1) 모든 리소스를 eRS_TOBE_DISCARD 상태로 바꾼다.
	CResourceMgr::getInstance()->ChangeStateDiscard();
	// 2) 바꿀씬을 찾아온다.
	m_pCurScene = GetSceneByName(_pName);  //이름으로 찾기
	// 3) 바꿀씬에 필요한 모든리소스를 로드하면서 이미 로드되어있는것은 다시 상태를 eRS_TOBE_USED로 바꾼다.
	res = m_pCurScene->InitObjects();
	// 4) eRS_TOBE_DISCARD 의 상태로 남아있는 모든 리소스의 메모리를 해제한다.
	CResourceMgr::getInstance()->Discard();

	////////////////////////////////////////////////////////////////////////////////////////
	// 현재씬으로 바뀔때 사용될 Dialog도  연결시켜준다. [2014.11.5 ]
// 	std::vector<CString>::iterator iter, endIter ;
// 	endIter = m_pCurScene->m_stlVec_DialogNames.end();
// 	int DlgsSize = CDialogRscMgr::GetInstance()->m_Dialogs.GetSize();
// 
// 	for(iter = m_pCurScene->m_stlVec_DialogNames.begin() ; iter != endIter ; ++iter)
// 	{
// 		for(int i = 0; i < DlgsSize ; i++ )
// 		{
// 			CDXUTDialog* pDlg = CDialogRscMgr::GetInstance()->m_Dialogs.GetAt(i);
// 			if((*iter) == pDlg->GetName() ) // 같은이름이 있다면 
// 			pDlg->SetMediator(m_pCurScene);  // 이게핵심 !! [2014.11.5 ]
// 		}
// 				
// 	}
	

	return res;

}

//-----------------------------------------------------------------------------
// Arg    : const WCHAR*
// Return : Success = CScene* ,  Fail = nullptr
// Desc   : _pName으로 찾고자하는 씬을 찾아서 포인터를 반환한다.
//-----------------------------------------------------------------------------

CScene*		CSceneMgr::GetSceneByName(const WCHAR* _pName)
{
	for(SceneIter= m_List_Scene.begin(); SceneIter != m_List_Scene.end() ; ++SceneIter)
	{
		if(wcscmp( (*SceneIter)->m_SceneName, _pName) == 0 ) //이름이 같다면
		{
			return (*SceneIter);
		}
	}
	SHUtil::MsgBox("씬찾기 실패~~ 잘못된 씬 이름~~");
	return nullptr;
}