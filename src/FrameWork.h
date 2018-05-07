
/************************************************************************/
/* 
*/
/************************************************************************/

#include "Object.h"

#pragma  once



/************************************************************************
			현재 윈도우에대한 정보를 가지고 있는 구조체
************************************************************************/
struct SHWindowInfo{
	
	HINSTANCE					hInst;
	HWND						hWnd;
	WNDCLASSEX					WndClassEx;
	int							nScreenWidth;  //클라이언트영역 가로
	int							nScreenHeight; //클라이언트영역 세로
	LPCWSTR						WindowTitleName; // 타이틀바 이름
	bool						WindowMode;     //true : 창모드, false : 전체화면

	//생성자
	SHWindowInfo()
	{
		 nScreenWidth = 800; nScreenHeight = 600; 
		 hInst = NULL; hWnd = NULL; WindowMode = true; WindowTitleName = L"None";
	}

};



/************************************************************************/
/*                    class CWindowFrameWork 
					CD3DFrameWork의 Base class
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
							
							디바이스 세팅값       

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
			CWindowFrameWork를 상속class CWindowFrameWork

************************************************************************/
class CD3DFrameWork : CWindowFrameWork
{

protected:// 멤버변수
	static CD3DFrameWork*				m_pInstance;
	// 윈도우 관련					
	bool								m_bWindowMode;// 윈도우창모드,,(false면 풀스크린 모드)
	// D3D 관련
	LPDIRECT3D9							m_pD3D ;	   // Used to create the D3DDevice
	D3DCAPS9							m_D3DCaps;
	D3DDISPLAYMODE						m_D3DDisplay;
	

	bool								m_bRun;         //어플리케이션이 실행중인가?
	float								m_DTime;		//DeltaTime

	CSceneMgr*							m_pSceneMgr;    // 씬메니저
	CResourceMgr*						m_pResourceMgr; // 리소스 메니저

	//SHEngine_TextHelper					m_txtHelper;// InitApplication 에서 반드시 초기화

	static LPDIRECT3DDEVICE9			m_pd3dDevice ; // Our rendering device
	
	static DXDeviceSettings				m_DeviceSetting;// 현재 디바이스 setting정보 저장
	static SHWindowInfo					m_WinInfo;      //윈도우 정보


private:


public: // 가상함수
	
	//게임 전체 루프 
	virtual void						ProcessRun();

protected:
	//-----------------------------------------------------------------------------
	// hInst : 현재 어플리케이션의 인스턴스   // pWinTitleName : 윈도우 타이틀바 이름	// ClntRectWidth : 클라이언트 영역 가로
	// ClntRectHeight: 클라이언트 영역 세로	// _lpfnWndProc  : 메시지프로시져로 등록할 함수 포인터
	//-----------------------------------------------------------------------------
	virtual int						    InitApplication(const SHWindowInfo*   _pWinInfo = nullptr);
	
	// 종료하기 직전 호출되는 함수
	virtual void						CloseApplication();
	virtual void						OnCloseApp() PURE;
	// 업데이트함수
	virtual void						ProcessUpdate();
	virtual void						OnUpdateStart(float _dTime){};
	virtual void						OnUpdateEnd(float _dTime){};
	// 렌더함수
	virtual void						ProcessRender();

	virtual void						OnRenderStart();

	virtual void						OnRenderEnd();

	// 메세지 처리 함수
	virtual void						ProcessMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	// UI를 그려주는 함수 // 사용자가 재정의 해서 사용  [2014.11.3 ]
	virtual void						RenderUI( float DTime ){};

	// 자식클래스에서 재정의되어야 하는 함수
	virtual void						OnGameMsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) PURE;

private: // 내부 함수
	// directX환경설정과 장치얻기
	HRESULT								DXCreate3DEnvironment();

public: //일반함수
	//생성자
	CD3DFrameWork();
	//소멸자
	virtual ~CD3DFrameWork();	

	// 실행중인가?
	bool								IsRunning(){return m_bRun;}


public:  // 접근자

	// 인스턴스 반환
	HINSTANCE							GetHINSTANCE();
	// 윈도우핸들 반환
	HWND								GetHWND();
	// d3d장치 반환
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