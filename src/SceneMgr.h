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
	SHEngine_TextHelper					    m_txtHelper;// InitApplication ���� �ݵ�� �ʱ�ȭ // [2014.12.1 ]

	WCHAR									m_SceneName[Max_Name_Len];//scene �̸�
	//���ҽ��� ����Ǿ��ִ� 
	std::list<WCHAR*>						m_List_ResourcePath;

	std::list<CGameObject*>					m_list_GameObjs;
	std::list<IRenderable*>					m_list_RenderObjs;
	std::list<IMessagable*>					m_list_MessagableObjs;
	std::list<IProgressable*>				m_list_ProgressObjs;
	std::list<IPickable*>					m_list_PickableObjs;
	std::vector<IAlphaBlendable*>			m_Vec_AlphaBlendObjs; // ���ļ����� ����!! [2014.12.8 ]
	std::priority_queue<IAlphaBlendable*, std::vector<IAlphaBlendable*> , CompareForAlphaSort >   m_PriQue_AlphaSort; // ���ļ����� ����!! [2014.12.8 ]

	// ���̾�α� �̸����� ����� ���� 
	// �� �̸���� ���̾�α׿� ���� ���� ��ó���� �ѹ�(�Ǵ� ���� �ٲ�) ��������ش�. [2014.11.5 ]
	std::vector<CString>				    m_stlVec_DialogNames; 

	CCamera*								m_pCamera;      // ������� ������ ī�޶� ������

public://����, �Ҹ���

	CScene();
	virtual ~CScene();

public://������ �Լ�

	virtual int								InitObjects() PURE;
	virtual void							OnRenderStart(){};
	virtual void							OnRenderEnd(){};
	virtual void							OnUpdateStart(float _dTime){};
	virtual void							OnUpdateEnd(){};
	// override
	virtual DWORD							Notify(SH_MSG* _pMessage );

	// �ؽ�Ʈ ȣ���ϴ� �Լ�
	virtual void							RenderText( float DTime ){};

	// UI�׷��ִ� �Լ� ������  // [2014.12.1 ]
	virtual void							RenderUI( float DTime ){};


public:

	void									Update(float _dTime);

	void									Render();
	void									ProcessMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void									Release();

	void									AlphaSorting(CCamera* _pCam ); // [2014.12.8 ]

protected://�����Լ�

	int										_SetCamera(CCamera* _pCamera);

public:
	// ���ӿ�����Ʈ�� �߰����ִ� �Լ�
	bool									AddObject(CGameObject* pObject ) ;
	// ���Ϸκ��� �׾��������� ��� ���ҽ������ �о�´�.
	int										LoadRsc_FromFile(const CHAR* _pPath);
public://������




};

/***********************************************************************
                         
						 class CSceneMgr
					    
***********************************************************************/
class CSceneMgr
{
	friend class CScene;

protected:

	static CSceneMgr*					m_pSceneMgr;//�̱���
	std::list<CScene*>					m_List_Scene;
	std::list<CScene*>::iterator		SceneIter;
	CScene*								m_pCurScene;

public://���� �Ҹ���

	CSceneMgr();
	~CSceneMgr();

public:

	//�̱��� �ν��Ͻ� ��ȯ
	static CSceneMgr*					getInstance();
	//�� �߰�
	int									AddScene(const WCHAR* _pName, CScene* _pScene);	

	int									OpenScene(const WCHAR* _pName);

	void								RenderCurScene();
	
	//���� ���������ִ� ���� ������ UI�� �׸���. // [2014.12.1 ]
	void								RenderUI_of_CulScene(float _DTime);

	void								UpdateCurScene(float _DTime);
	// ������� �޽��� ó��
	void								ProcessCurSceneMsg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:// �����Լ�	


public://get, set
	//����� ������ ��ȯ
	CScene*								CurScene(){return m_pCurScene;}
	// ������� �������ִ� ī�޶� ������ ��������
	CCamera*							GetCamera(){return m_pCurScene->m_pCamera;}
	// �̸��� �ش��ϴ� Scene�� ��ȯ�Ѵ�.
	CScene*								GetSceneByName(const WCHAR* _pName);

	
	


};


#include "Scene.h"
/****************************************************** End *****************************************************/