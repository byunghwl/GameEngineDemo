
/************************************************************************/
/*				  [2014.8.19 ] 
				FrameWork.cpp
*/
/************************************************************************/
#include "DXUT.h"
#include "FrameWork.h"




//////////////////////////////////////////////////////////////////////////
// static 변수 초기화

LPDIRECT3DDEVICE9		CD3DFrameWork::m_pd3dDevice = nullptr;
DXDeviceSettings		CD3DFrameWork::m_DeviceSetting;// 현재 디바이스 setting정보 저장
SHWindowInfo			CD3DFrameWork::m_WinInfo;      //윈도우 정보

ID3DXFont*				SHGlobal_pFontSystem = nullptr;  // Font for drawing text  
ID3DXSprite*			SHGlobal_pSprite_BDT = nullptr;  // Sprite for batching draw text calls

CD3DFrameWork*			GFrameWork = nullptr;

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
// 여기서부터 CD3DFrameWork 함수 구현
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

//-------------------------------------------------------------------------
// CD3DFrameWork 생성자 [2014.8.21 ]
//-------------------------------------------------------------------------
CD3DFrameWork::CD3DFrameWork() 	:  m_bWindowMode(true), m_pD3D(nullptr), m_pSceneMgr(nullptr), m_bRun(FALSE)
{	
	//SH로그파일 초기화
	create_log_file();

	GFrameWork =this;

	m_pSceneMgr = CSceneMgr::getInstance();
	 
	
}


//-------------------------------------------------------------------------
// CD3DFrameWork 소멸자 [2014.8.21 ]
//-------------------------------------------------------------------------
CD3DFrameWork::~CD3DFrameWork()
{


	SAFE_RELEASE(m_pD3D);	   
	SAFE_RELEASE(m_pd3dDevice)

	GFrameWork = nullptr;

	 write_log_file("CD3DFrameWork::~CD3DFrameWork() 호출");
}


//-----------------------------------------------------------------------------
// Name   : InitApplication
// Arg    : SHWindowInf* 
// Return : 
//-----------------------------------------------------------------------------
int 	CD3DFrameWork::InitApplication( const SHWindowInfo*   _pWinInfo /*= nullptr*/ )
{		
	HRESULT hr = S_OK;

		if(_pWinInfo == nullptr)
			SH_RETURN(S_FALSE) 

		m_WinInfo = *_pWinInfo;

		m_bWindowMode = m_WinInfo.WindowMode;
		//////////////////////////////////////////////////////////////////////////
		// 윈도우 생성

		// Register the engine default's WNDCLASSEX
		// 클래스 등록
		WNDCLASSEX wcex = {0,};
		wcex.cbSize			= sizeof(WNDCLASSEX);
		wcex.style			= CS_HREDRAW | CS_VREDRAW ;
		wcex.lpfnWndProc	= GFrameWork->fnWindProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= m_WinInfo.hInst;
		wcex.hIcon			= LoadIcon(NULL,IDI_APPLICATION);
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH); 
		wcex.lpszMenuName	= NULL;
		wcex.lpszClassName	= L"Pear Engine";
		wcex.hIconSm		= NULL;
		RegisterClassEx(&wcex);
	
		m_WinInfo.WndClassEx = wcex;//구조체 정보 백업

		RECT rect = {0,0, m_WinInfo.nScreenWidth, m_WinInfo.nScreenHeight};
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

		m_WinInfo.hWnd = CreateWindow( L"Pear Engine", m_WinInfo.WindowTitleName,  WS_OVERLAPPEDWINDOW, 
									   0, 0, rect.right -  rect.left, rect.bottom - rect.top, NULL, NULL, m_WinInfo.hInst, NULL );
		//MessageBox(NULL,PrintErrorMessage( GetLastError()), L"Error", MB_OK);
		assert(m_WinInfo.hWnd);

		ShowWindow(m_WinInfo.hWnd, SW_SHOW);
		UpdateWindow(m_WinInfo.hWnd);
		
		///////////////////////////////////////////////////////////////////////////////////
		// d3d device 생성     
		// d3d 디바이스 생성
		if(FAILED(DXCreate3DEnvironment() ) ){ SHUtil::MsgBox("FAILED DXCreate3DEnvironment()!!!"); assert(false);}
	

		//////////////////////////////////////////////////////////////////////////
		// 폰트만들기
	
		if(FAILED( D3DXCreateFont( m_pd3dDevice, 20, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET,OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
								   L"Arial", &SHGlobal_pFontSystem )
				 )
		  )
		{SHUtil::MsgBox("FAILED D3DXCreateFont()!!!");assert(false);		}
		//////////////////////////////////////////////////////////////////////////
		// Create a sprite to help batch calls when drawing many lines of text
		if(FAILED(D3DXCreateSprite( m_pd3dDevice, &SHGlobal_pSprite_BDT )) )
		{SHUtil::MsgBox("FAILED D3DXCreateSprite()!!!"); assert(false);		}
		
		
	
		SHG_Input.Initialize(m_WinInfo.hInst, m_WinInfo.hWnd);// 반드시 d3dDevice생성한후 초기화 한번해주기

		// ★★★ IResource 인터페이스의 m_pd3dDevice는 static 멤버변수이기 때문에 디바이스 객체가 생성된 후
		// 프로그램이 맨처음 한번 실행될때 반드시 디바이스 객체 포인터를 복사시켜야한다.  ★★★ 
		IResource::SetDevice( SHGlobal_GetD3DDevice); // [2014.10.30 ]


		// 화면전체크기에 맞는 사각형을 정의 ..post effect를 위해 [2014.11.7 ]
		hr = G_ScreenQuad.Init();




		m_bRun = true;

		return hr;
}


//-----------------------------------------------------------------------------
// //게임 전체 루프 
//-----------------------------------------------------------------------------
void	CD3DFrameWork::ProcessRun()
{
	
	// Enter the message loop
	MSG msg;
	ZeroMemory( &msg, sizeof( msg ) );

	while( msg.message != WM_QUIT )
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		
		if(m_bRun == true)
		{
			ProcessUpdate();
			ProcessRender();
		}
	
		
	}

	

}


//-------------------------------------------------------------------------
// directX환경설정과 장치얻기
//-------------------------------------------------------------------------
HRESULT	CD3DFrameWork::DXCreate3DEnvironment()
{	
	
	// d3d 오브젝트 생성.
	if(!(m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ))) { SHUtil::MsgBox("FAILED Direct3DCreate9()!!!");		return E_FAIL; }

		//////////////////////////////////////////////////////////////////////////
		// Caps 정보 얻어오기.
		if(FAILED(m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_D3DCaps))) {
			SHUtil::MsgBox("FAILED GetDeviceCaps()");
			return E_FAIL;
		}

		//////////////////////////////////////////////////////////////////////////
		// T&L 하드웨어 체크
		DWORD vertexProcessing = 0;
		if(m_D3DCaps.VertexProcessingCaps != 0)
		{
			vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
			// PURE 디바이스 검사 (Device can support rasterization, transform, lighting, and shading in hardware.)
			if(m_D3DCaps.DevCaps & D3DDEVCAPS_PUREDEVICE )
				vertexProcessing |= D3DCREATE_PUREDEVICE; 
		} 
		else 
		{
			vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}

		//////////////////////////////////////////////////////////////////////////
		//디스플레이 정보를 얻어온다.
		m_pD3D->GetAdapterDisplayMode(m_D3DCaps.AdapterOrdinal, &m_D3DDisplay);
		UINT width		= m_WinInfo.nScreenWidth;
		UINT height		= m_WinInfo.nScreenHeight;
		D3DFORMAT fmat	= (m_bWindowMode) ? m_D3DDisplay.Format : D3DFMT_X8R8G8B8; //BackBufferFormat

		// 전체화면 일때는 그래픽카드가 지원하는 해상도를 검사하여 바꿔준다.
		if(!m_bWindowMode)
		{
			UINT cnt = m_pD3D->GetAdapterModeCount(m_D3DCaps.AdapterOrdinal, fmat);
			for(UINT i=0; i<cnt; ++i)
			{
				D3DDISPLAYMODE pmode;
				m_pD3D->EnumAdapterModes(m_D3DCaps.AdapterOrdinal, fmat, i, &pmode);
				if(pmode.Width >= width || pmode.Height >= height)
				{
					if(m_D3DDisplay.RefreshRate == pmode.RefreshRate)
					{
						width = pmode.Width;
						height= pmode.Height;
						break;
					}
				}
				//위 조건문에 걸리지 않았다면 현재 모니터 해상도로 설정한다.
				if(i == cnt-1)
				{
					width = m_D3DDisplay.Width;
					height= m_D3DDisplay.Height;
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// 디바이스 속성을 채워준다.
		D3DPRESENT_PARAMETERS pp;
		memset(&pp, 0, sizeof(D3DPRESENT_PARAMETERS));	
		ZeroMemory( &pp, sizeof(pp) );
		pp.hDeviceWindow			= m_WinInfo.hWnd;
		pp.Windowed					= m_bWindowMode;
		pp.BackBufferCount			= 1;
		pp.BackBufferWidth			= width;
		pp.BackBufferHeight			= height;
		pp.BackBufferFormat			= fmat;						//후면버퍼의 픽셀 포멧
		pp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	//플리핑(프론트버퍼와 백버퍼를 바꿔주는거)체인의 버퍼가 교환되는 방법을 지정하는 D3DSWAPEFFECT열거형 멤버
		pp.MultiSampleType			= D3DMULTISAMPLE_NONE;
		pp.MultiSampleQuality		= 0;
		pp.EnableAutoDepthStencil	= TRUE;
		pp.FullScreen_RefreshRateInHz = (m_bWindowMode) ? 0 : m_D3DDisplay.RefreshRate;
		pp.PresentationInterval		= D3DPRESENT_INTERVAL_DEFAULT;	// 프레임 고정.

		// AutoDepthStencilFormat 환경에 맞게 초기화
		if (SUCCEEDED(m_pD3D->CheckDeviceFormat(m_D3DCaps.AdapterOrdinal, m_D3DCaps.DeviceType, fmat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8)))
		{
			pp.AutoDepthStencilFormat = D3DFMT_D24S8;
		}
		else if (SUCCEEDED(m_pD3D->CheckDeviceFormat(m_D3DCaps.AdapterOrdinal, m_D3DCaps.DeviceType, fmat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8)))
		{
			pp.AutoDepthStencilFormat = D3DFMT_D24X8;
		}
		else if (SUCCEEDED(m_pD3D->CheckDeviceFormat(m_D3DCaps.AdapterOrdinal, m_D3DCaps.DeviceType, fmat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16)))
		{
			pp.AutoDepthStencilFormat = D3DFMT_D16;
		}
		else
		{
			SHUtil::MsgBox("FAILED AutoDepthStencilFormat");	return E_FAIL;
		}

		//////////////////////////////////////////////////////////////////////////
		// 디바이스 Setting 정보 저장.

		memset(&m_DeviceSetting, 0, sizeof(DXDeviceSettings));
		m_DeviceSetting.pp				= pp;
		m_DeviceSetting.AdapterFormat	= pp.BackBufferFormat;
		m_DeviceSetting.DeviceType		= m_D3DCaps.DeviceType;
		m_DeviceSetting.AdapterOrdinal	= m_D3DCaps.AdapterOrdinal;
		m_DeviceSetting.BehaviorFlags	= vertexProcessing;

		//////////////////////////////////////////////////////////////////////////
		// 디바이스 생성 & 복구.
		if(!m_pd3dDevice)	{
			if(FAILED(m_pD3D->CreateDevice(m_D3DCaps.AdapterOrdinal, m_D3DCaps.DeviceType, m_WinInfo.hWnd, vertexProcessing, &pp, &m_pd3dDevice))) {
				SHUtil::MsgBox("FAILED Device Create!!!");	
				return E_FAIL;	
			}
		} 
		else {
			HRESULT a=m_pd3dDevice->Reset( &pp );
			if(FAILED(a)) {
				if(a==D3DERR_DEVICELOST)
				{
					SHUtil::MsgBox("FAILED Device Reset!!! D3DERR_DEVICELOST");	
				}

				else if(a==D3DERR_DRIVERINTERNALERROR)
				{
					SHUtil::MsgBox("FAILED Device Reset!!! D3DERR_DRIVERINTERNALERROR");	
				}
				else if(a==D3DERR_INVALIDCALL)
				{
					SHUtil::MsgBox("FAILED Device Reset!!!D3DERR_INVALIDCALL%%%%%%%%%%");
				}
				else if(a==D3DERR_OUTOFVIDEOMEMORY)
				{
					SHUtil::MsgBox("FAILED Device Reset!!!D3DERR_OUTOFVIDEOMEMORY");

				}
				else if(a==E_OUTOFMEMORY)
				{
					SHUtil::MsgBox("FAILED Device Reset!!!E_OUTOFMEMORY");
				}
				else
				{	
					SHUtil::MsgBox("FAILED Device Reset!!!");	

				}

				return E_FAIL;	
			}
		}

		//////////////////////////////////////////////////////////////////////////
		//메트리스 팔레트를 위한 검사
		m_pd3dDevice->GetDeviceCaps( &m_D3DCaps );
		int IndexedMatrixMaxSize = m_D3DCaps.MaxVertexBlendMatrixIndex;
		if(IndexedMatrixMaxSize == 0) 
		{
			write_log_file("메트릭스팔레트지원안해~!");
			MessageBox(NULL,  TEXT("메트릭스팔레트지원안해"), TEXT("Failed"), MB_OK); 
		}
	
		//----------------------------------
		// 윈도우 크기 재설정.
		/*
		RECT oldrc;				::GetWindowRect(m_hWnd, &oldrc);
		DWORD sytle   = (DWORD) ::GetWindowLong(m_hWnd, GWL_STYLE);
		DWORD exstyle = (DWORD) ::GetWindowLong(m_hWnd, GWL_EXSTYLE);	//exstyle &= ~WS_EX_TOPMOST;
		// 새로 생성될 윈도우의 '클라이언트' 영역 크기 계산하기.(창모드-현재 해상도, 전체화면-디스플레이 해상도)
		RECT newrc;	  SetRect(&newrc, 0, 0, width, height);
		// newrc 만큼의 클라이언트 영역을 포함하는 윈도우 '전체' 크기를 구한다.('메뉴'가 없다는 가정하에 처리.)
		::AdjustWindowRectEx(&newrc, sytle, NULL, exstyle);
		// 보정된 윈도우의 너비와 폭을 구한다. 
		width  = (newrc.right  - newrc.left);
		height = (newrc.bottom - newrc.top);
		//새로운 크기를 윈도우에 설정한다.
		::SetWindowPos(m_hWnd, HWND_NOTOPMOST, oldrc.left, oldrc.top, width, height, SWP_SHOWWINDOW);
		
		if(gWindowed) ShowWindow(m_hWnd, SW_SHOW);	// 윈도우 창 보이기.
		GetClientRect(m_hWnd, &gRect);				// 클라이언트 영역 크기를 얻어온다.
		*/
		return S_OK;

}

//-----------------------------------------------------------------------------
// 업데이트함수
//-----------------------------------------------------------------------------
void CD3DFrameWork::ProcessUpdate()
{
	// engine Timer
	m_DTime = DXUTGetGlobalTimer()->GetElapsedTime();
	if(m_DTime >1.0f) m_DTime  = 0;

	
	// direct input을 사용하기 위한 업데이트 반드시 이곳에서호출
	SHG_Input.OnFrameMove(m_DTime);

	//사용자 재정의 함수
	OnUpdateStart(m_DTime);

	m_pSceneMgr->UpdateCurScene(m_DTime);

	//사용자 재정의 함수
	OnUpdateEnd(m_DTime);
	

}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CD3DFrameWork::ProcessRender()
{
	//하드웨어 백버퍼를 저장해놓는다. [2014.11.8 ]
	m_pd3dDevice->GetRenderTarget(0, &(G_ScreenQuad.g_pHWBackBuffer));
	
	m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER , D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );

	//////////////////////////////////////////////////////////////////////////
	// 일단 서페이스 0번의 텍스쳐만 매프레임마다 그리기 전에 파란색으로 클리어해준다.  [2014.11.19 ]
	if( SUCCEEDED( G_ScreenQuad.g_pRenderTargetTexPass[0]->GetSurfaceLevel( 0, &(G_ScreenQuad.g_pSceneSurface[0]) ) ) )
	{//gpSceneRenderTarget은 서페이스를 만들어줄때 필요한 녀석 [2014.11.7 ]
		m_pd3dDevice->SetRenderTarget( 0, G_ScreenQuad.g_pSceneSurface[0] ); // 만든 서페이스를 렌더타겟으로 지정(복사) [2014.11.7 ]
		m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER , D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );
	}
	 
	// CD3DFrameWork 을 상속받은 클래스에서 오버라이딩 할수있는 함수 
	OnRenderStart();

	if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
	{	
		//전프레임에 그렸던 모든 장면을 지운다.  // [2014.11.8 ]
		assert(G_ScreenQuad.ClearAllSurface() == S_OK); // 전장면을 모두 지운다. [2014.11.8 ]
		
		// 현재씬의 모든 그릴수 있는것들을 그린다.
		m_pSceneMgr->RenderCurScene();		
		m_pSceneMgr->RenderUI_of_CulScene(m_DTime); // [2014.12.1 ]
// 		RenderText( m_DTime );//시스템 텍스트 그리기
// 		RenderUI(m_DTime); // UI를 제일 마지막에 그려주기


		// 하드웨어 백버퍼를 다시 사용한다. [2014.11.8 ]
		 m_pd3dDevice->SetRenderTarget( 0, G_ScreenQuad.g_pHWBackBuffer);
		//여기서 포스트이펙트를 위한 테크닉으로 바꿔준다.  [2014.11.8 ]
		 G_FXShader.GetPointer()->SetTechnique(G_ConstantTable.h_Tech_NoEffect);
		// 모든것이 다그려지 ㄴ최종 택스쳐(랜더타겟)를  셰이더에 설정한다.
	    G_FXShader.GetPointer()->SetTexture(G_ConstantTable.h_Tex_Pass0,  G_ScreenQuad.g_pRenderTargetTexPass[0]);

		UINT numPasses = 0;
		G_FXShader.GetPointer()->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i )
			{
				G_FXShader.GetPointer()->BeginPass(i);
				{
					// 화면에 맞는 사각형을 그린다.
					G_ScreenQuad.Draw();
				}
				G_FXShader.GetPointer()->EndPass();
			}
		}
		G_FXShader.GetPointer()->End();



		m_pd3dDevice->EndScene();
	}

	// Present the backbuffer contents to the display
	m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
	
	OnRenderEnd();
	
	
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void	CD3DFrameWork::OnRenderStart()
{
	

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CD3DFrameWork::OnRenderEnd()
{
	

}

//-----------------------------------------------------------------------------
// 종료되기직전에 호출되는 함수
//-----------------------------------------------------------------------------
void CD3DFrameWork::CloseApplication()
{
		SAFE_RELEASE(m_pd3dDevice)

		SAFE_RELEASE(m_pD3D)

	 	SAFE_RELEASE(SHGlobal_pFontSystem)
			 	
	 	SAFE_RELEASE(SHGlobal_pSprite_BDT)

		//사용자 재정의 함수
		OnCloseApp();
}


//-----------------------------------------------------------------------------
// 인스턴스 반환
//-----------------------------------------------------------------------------
HINSTANCE CD3DFrameWork::GetHINSTANCE()
{
	return m_WinInfo.hInst;
}



//-----------------------------------------------------------------------------
// 윈도우핸들 반환
//-----------------------------------------------------------------------------
HWND CD3DFrameWork::GetHWND()
{
	return m_WinInfo.hWnd;
}


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void	CD3DFrameWork::ProcessMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	//현재씬에서의 메세지 처리
	m_pSceneMgr->ProcessCurSceneMsg(hWnd,message,wParam,lParam);

	//사용자 재정의 함수
	OnGameMsgProc(hWnd,  message,wParam,  lParam);

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
		m_bRun = false;
		CloseApplication();
		PostQuitMessage(0);
		break;
	default:
		break;
	}

}



//-----------------------------------------------------------------------------
// Name   : 윈도우 메시지 프로시져
//-----------------------------------------------------------------------------
LRESULT WINAPI CD3DFrameWork::fnWindProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	if(GFrameWork->IsRunning())GFrameWork->ProcessMessage(hWnd,message,wParam,lParam);
	
	return DefWindowProc(hWnd, message, wParam, lParam);


}