
/************************************************************************/
/* 
*/
/************************************************************************/

#include "Object.h"

#pragma  once



/************************************************************************
			���� �����쿡���� ������ ������ �ִ� ����ü
************************************************************************/
struct SHWindowInfo{
	
	HINSTANCE					hInst;
	HWND						hWnd;
	WNDCLASSEX					WndClassEx;
	int							nScreenWidth;  //Ŭ���̾�Ʈ���� ����
	int							nScreenHeight; //Ŭ���̾�Ʈ���� ����
	LPCWSTR						WindowTitleName; // Ÿ��Ʋ�� �̸�
	bool						WindowMode;     //true : â���, false : ��üȭ��

	//������
	SHWindowInfo()
	{
		 nScreenWidth = 800; nScreenHeight = 600; 
		 hInst = NULL; hWnd = NULL; WindowMode = true; WindowTitleName = L"None";
	}

};



/************************************************************************/
/*                    class CWindowFrameWork 
					CD3DFrameWork�� Base class
*/
/************************************************************************/

class CWindowFrameWork
{
public:
 	virtual void			ProcessRun()		= 0;
	virtual int				InitApplication(const SHWindowInfo*   _pWinInfo = nullptr )	= 0;
 	virtual void			CloseApplication()  = 0;
 	virtual void			ProcessUpdate()     = 0;
 	virtual void			ProcessRender()		= 0;
	virtual void			ProcessMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)	= 0;


};



/***********************************************************************
							
							����̽� ���ð�       

************************************************************************/
struct DXDeviceSettings
{	
	UINT					AdapterOrdinal;
	D3DDEVTYPE	DeviceType;
	D3DFORMAT	AdapterFormat;
	DWORD		BehaviorFlags;
	D3DPRESENT_PARAMETERS pp;
	 
	DXDeviceSettings(){memset(this, 0,sizeof(DXDeviceSettings)) ;}
};



/************************************************************************
                    
						class CD3DFrameWork
			CWindowFrameWork�� ���class CWindowFrameWork

************************************************************************/
class CD3DFrameWork : CWindowFrameWork
{

protected:// �������
	static CD3DFrameWork*				m_pInstance;
	// ������ ����					
	bool								m_bWindowMode;// ������â���,,(false�� Ǯ��ũ�� ���)
	// D3D ����
	LPDIRECT3D9							m_pD3D ;	   // Used to create the D3DDevice
	D3DCAPS9							m_D3DCaps;
	D3DDISPLAYMODE						m_D3DDisplay;
	

	bool								m_bRun;         //���ø����̼��� �������ΰ�?
	float								m_DTime;		//DeltaTime

	CSceneMgr*							m_pSceneMgr;    // ���޴���
	CResourceMgr*						m_pResourceMgr; // ���ҽ� �޴���

	//SHEngine_TextHelper					m_txtHelper;// InitApplication ���� �ݵ�� �ʱ�ȭ

	static LPDIRECT3DDEVICE9			m_pd3dDevice ; // Our rendering device
	
	static DXDeviceSettings				m_DeviceSetting;// ���� ����̽� setting���� ����
	static SHWindowInfo					m_WinInfo;      //������ ����


private:


public: // �����Լ�
	
	//���� ��ü ���� 
	virtual void						ProcessRun();

protected:
	//-----------------------------------------------------------------------------
	// hInst : ���� ���ø����̼��� �ν��Ͻ�   // pWinTitleName : ������ Ÿ��Ʋ�� �̸�	// ClntRectWidth : Ŭ���̾�Ʈ ���� ����
	// ClntRectHeight: Ŭ���̾�Ʈ ���� ����	// _lpfnWndProc  : �޽������ν����� ����� �Լ� ������
	//-----------------------------------------------------------------------------
	virtual int						    InitApplication(const SHWindowInfo*   _pWinInfo = nullptr);
	
	// �����ϱ� ���� ȣ��Ǵ� �Լ�
	virtual void						CloseApplication();
	virtual void						OnCloseApp() PURE;
	// ������Ʈ�Լ�
	virtual void						ProcessUpdate();
	virtual void						OnUpdateStart(float _dTime){};
	virtual void						OnUpdateEnd(float _dTime){};
	// �����Լ�
	virtual void						ProcessRender();

	virtual void						OnRenderStart();

	virtual void						OnRenderEnd();

	// �޼��� ó�� �Լ�
	virtual void						ProcessMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	// UI�� �׷��ִ� �Լ� // ����ڰ� ������ �ؼ� ���  [2014.11.3 ]
	virtual void						RenderUI( float DTime ){};

	// �ڽ�Ŭ�������� �����ǵǾ�� �ϴ� �Լ�
	virtual void						OnGameMsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) PURE;

private: // ���� �Լ�
	// directXȯ�漳���� ��ġ���
	HRESULT								DXCreate3DEnvironment();

public: //�Ϲ��Լ�
	//������
	CD3DFrameWork();
	//�Ҹ���
	virtual ~CD3DFrameWork();	

	// �������ΰ�?
	bool								IsRunning(){return m_bRun;}


public:  // ������

	// �ν��Ͻ� ��ȯ
	HINSTANCE							GetHINSTANCE();
	// �������ڵ� ��ȯ
	HWND								GetHWND();
	// d3d��ġ ��ȯ
	static const LPDIRECT3DDEVICE9&		GetD3DDevice()  {return m_pd3dDevice;}

	const SHWindowInfo&					GetWindowInfo(){return m_WinInfo;}

	const DXDeviceSettings&				GetDeviceSetting(){return m_DeviceSetting;}
	
	int									GetScreenWidth(){return m_WinInfo.nScreenWidth;}
	
	int									GetScreenHeight(){return m_WinInfo.nScreenHeight;}

	float								GetEngineTime(){return m_DTime;}
	
	
public:

	static LRESULT WINAPI    			fnWindProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	
};


#define  SHGlobal_GetDeviceSetting  GFrameWork->GetDeviceSetting()
#define  SHGlobal_GetD3DDevice		GFrameWork->GetD3DDevice()
#define  SHGlobal_GetCurScene_Camera			CSceneMgr::getInstance()->GetCamera()
#define  SHGlobal_GetHwnd			GFrameWork->GetHWND()
#define  SHGlobal_GetHinstance		GFrameWork->GetHINSTANCE()	
#define  SHGlobal_GetScreenWidth	GFrameWork->GetScreenWidth()
#define  SHGlobal_GetScreenHeight	GFrameWork->GetScreenHeight()
#define  SHGlobal_GetEngineTime		GFrameWork->GetEngineTime()
extern ID3DXFont*					SHGlobal_pFontSystem ;  // Font for drawing text  
extern ID3DXSprite*					SHGlobal_pSprite_BDT ;  // Sprite for batching draw text calls
extern CD3DFrameWork*				GFrameWork ;

/*************************************************** end of this file **************************************************************/