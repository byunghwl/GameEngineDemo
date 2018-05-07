/***********************************************************************
							SceneMgr.h
						  
************************************************************************/
#pragma once

class CGameObject;
class CCamera;
/***********************************************************************
                          class   CScene                 
						  [2014.8.27 ]
************************************************************************/

class CScene : public INotifiable
{

	friend class CSceneMgr;

protected:
	SHEngine_TextHelper					    m_txtHelper;// InitApplication 에서 반드시 초기화 // [2014.12.1 ]

	WCHAR									m_SceneName[Max_Name_Len];//scene 이름
	//리소스가 저장되어있는 
	std::list<WCHAR*>						m_List_ResourcePath;

	std::list<CGameObject*>					m_list_GameObjs;
	std::list<IRenderable*>					m_list_RenderObjs;
	std::list<IMessagable*>					m_list_MessagableObjs;
	std::list<IProgressable*>				m_list_ProgressObjs;
	std::list<IPickable*>					m_list_PickableObjs;
	std::vector<IAlphaBlendable*>			m_Vec_AlphaBlendObjs; // 알파소팅을 위해!! [2014.12.8 ]
	std::priority_queue<IAlphaBlendable*, std::vector<IAlphaBlendable*> , CompareForAlphaSort >   m_PriQue_AlphaSort; // 알파소팅을 위해!! [2014.12.8 ]

	// 다이얼로그 이름들이 저장될 공간 
	// 이 이름들로 다이얼로그와 현재 씬을 맨처음에 한번(또는 씬이 바뀔때) 연결시켜준다. [2014.11.5 ]
	std::vector<CString>				    m_stlVec_DialogNames; 

	CCamera*								m_pCamera;      // 현재씬에 설정된 카메라 포인터

public://생성, 소멸자

	CScene();
	virtual ~CScene();

public://재정의 함수

	virtual int								InitObjects() PURE;
	virtual void							OnRenderStart(){};
	virtual void							OnRenderEnd(){};
	virtual void							OnUpdateStart(float _dTime){};
	virtual void							OnUpdateEnd(){};
	// override
	virtual DWORD							Notify(SH_MSG* _pMessage );

	// 텍스트 호출하는 함수
	virtual void							RenderText( float DTime ){};

	// UI그려주는 함수 재정의  // [2014.12.1 ]
	virtual void							RenderUI( float DTime ){};


public:

	void									Update(float _dTime);

	void									Render();
	void									ProcessMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void									Release();

	void									AlphaSorting(CCamera* _pCam ); // [2014.12.8 ]

protected://내부함수

	int										_SetCamera(CCamera* _pCamera);

public:
	// 게임오브젝트를 추가해주는 함수
	bool									AddObject(CGameObject* pObject ) ;
	// 파일로부터 그씬에서사용될 모든 리소스목록을 읽어온다.
	int										LoadRsc_FromFile(const CHAR* _pPath);
public://접근자




};

/***********************************************************************
                         
						 class CSceneMgr
					    
***********************************************************************/
class CSceneMgr
{
	friend class CScene;

protected:

	static CSceneMgr*					m_pSceneMgr;//싱글톤
	std::list<CScene*>					m_List_Scene;
	std::list<CScene*>::iterator		SceneIter;
	CScene*								m_pCurScene;

public://생성 소멸자

	CSceneMgr();
	~CSceneMgr();

public:

	//싱글톤 인스턴스 반환
	static CSceneMgr*					getInstance();
	//씬 추가
	int									AddScene(const WCHAR* _pName, CScene* _pScene);	

	int									OpenScene(const WCHAR* _pName);

	void								RenderCurScene();
	
	//현재 보여지고있는 씬에 부착된 UI를 그린다. // [2014.12.1 ]
	void								RenderUI_of_CulScene(float _DTime);

	void								UpdateCurScene(float _DTime);
	// 현재씬의 메시지 처리
	void								ProcessCurSceneMsg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:// 내부함수	


public://get, set
	//현재씬 포인터 반환
	CScene*								CurScene(){return m_pCurScene;}
	// 현재씬이 가지고있는 카메라 포인터 가져오기
	CCamera*							GetCamera(){return m_pCurScene->m_pCamera;}
	// 이름에 해당하는 Scene을 반환한다.
	CScene*								GetSceneByName(const WCHAR* _pName);

	
	


};


#include "Scene.h"
/****************************************************** End *****************************************************/