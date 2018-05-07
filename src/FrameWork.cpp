
/************************************************************************/
/*				  [2014.8.19 ] 
				FrameWork.cpp
*/
/************************************************************************/
#include "DXUT.h"
#include "FrameWork.h"




//////////////////////////////////////////////////////////////////////////
// static ���� �ʱ�ȭ

LPDIRECT3DDEVICE9		CD3DFrameWork::m_pd3dDevice = nullptr;
DXDeviceSettings		CD3DFrameWork::m_DeviceSetting;// ���� ����̽� setting���� ����
SHWindowInfo			CD3DFrameWork::m_WinInfo;      //������ ����

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
// ���⼭���� CD3DFrameWork �Լ� ����
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
// CD3DFrameWork ������ [2014.8.21 ]
//-------------------------------------------------------------------------
CD3DFrameWork::CD3DFrameWork() 	:  m_bWindowMode(true), m_pD3D(nullptr), m_pSceneMgr(nullptr), m_bRun(FALSE)
{	
	//SH�α����� �ʱ�ȭ
	create_log_file();

	GFrameWork =this;

	m_pSceneMgr = CSceneMgr::getInstance();
	 
	
}


//-------------------------------------------------------------------------
// CD3DFrameWork �Ҹ��� [2014.8.21 ]
//-------------------------------------------------------------------------
CD3DFrameWork::~CD3DFrameWork()
{


	SAFE_RELEASE(m_pD3D);	   
	SAFE_RELEASE(m_pd3dDevice)

	GFrameWork = nullptr;

	 write_log_file("CD3DFrameWork::~CD3DFrameWork() ȣ��");
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
		// ������ ����

		// Register the engine default's WNDCLASSEX
		// Ŭ���� ���
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
	
		m_WinInfo.WndClassEx = wcex;//����ü ���� ���

		RECT rect = {0,0, m_WinInfo.nScreenWidth, m_WinInfo.nScreenHeight};
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

		m_WinInfo.hWnd = CreateWindow( L"Pear Engine", m_WinInfo.WindowTitleName,  WS_OVERLAPPEDWINDOW, 
									   0, 0, rect.right -  rect.left, rect.bottom - rect.top, NULL, NULL, m_WinInfo.hInst, NULL );
		//MessageBox(NULL,PrintErrorMessage( GetLastError()), L"Error", MB_OK);
		assert(m_WinInfo.hWnd);

		ShowWindow(m_WinInfo.hWnd, SW_SHOW);
		UpdateWindow(m_WinInfo.hWnd);
		
		///////////////////////////////////////////////////////////////////////////////////
		// d3d device ����     
		// d3d ����̽� ����
		if(FAILED(DXCreate3DEnvironment() ) ){ SHUtil::MsgBox("FAILED DXCreate3DEnvironment()!!!"); assert(false);}
	

		//////////////////////////////////////////////////////////////////////////
		// ��Ʈ�����
	
		if(FAILED( D3DXCreateFont( m_pd3dDevice, 20, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET,OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
								   L"Arial", &SHGlobal_pFontSystem )
				 )
		  )
		{SHUtil::MsgBox("FAILED D3DXCreateFont()!!!");assert(false);		}
		//////////////////////////////////////////////////////////////////////////
		// Create a sprite to help batch calls when drawing many lines of text
		if(FAILED(D3DXCreateSprite( m_pd3dDevice, &SHGlobal_pSprite_BDT )) )
		{SHUtil::MsgBox("FAILED D3DXCreateSprite()!!!"); assert(false);		}
		
		
	
		SHG_Input.Initialize(m_WinInfo.hInst, m_WinInfo.hWnd);// �ݵ�� d3dDevice�������� �ʱ�ȭ �ѹ����ֱ�

		// �ڡڡ� IResource �������̽��� m_pd3dDevice�� static ��������̱� ������ ����̽� ��ü�� ������ ��
		// ���α׷��� ��ó�� �ѹ� ����ɶ� �ݵ�� ����̽� ��ü �����͸� ������Ѿ��Ѵ�.  �ڡڡ� 
		IResource::SetDevice( SHGlobal_GetD3DDevice); // [2014.10.30 ]


		// ȭ����üũ�⿡ �´� �簢���� ���� ..post effect�� ���� [2014.11.7 ]
		hr = G_ScreenQuad.Init();




		m_bRun = true;

		return hr;
}


//-----------------------------------------------------------------------------
// //���� ��ü ���� 
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
// directXȯ�漳���� ��ġ���
//-------------------------------------------------------------------------
HRESULT	CD3DFrameWork::DXCreate3DEnvironment()
{	
	
	// d3d ������Ʈ ����.
	if(!(m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ))) { SHUtil::MsgBox("FAILED Direct3DCreate9()!!!");		return E_FAIL; }

		//////////////////////////////////////////////////////////////////////////
		// Caps ���� ������.
		if(FAILED(m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_D3DCaps))) {
			SHUtil::MsgBox("FAILED GetDeviceCaps()");
			return E_FAIL;
		}

		//////////////////////////////////////////////////////////////////////////
		// T&L �ϵ���� üũ
		DWORD vertexProcessing = 0;
		if(m_D3DCaps.VertexProcessingCaps != 0)
		{
			vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
			// PURE ����̽� �˻� (Device can support rasterization, transform, lighting, and shading in hardware.)
			if(m_D3DCaps.DevCaps & D3DDEVCAPS_PUREDEVICE )
				vertexProcessing |= D3DCREATE_PUREDEVICE; 
		} 
		else 
		{
			vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}

		//////////////////////////////////////////////////////////////////////////
		//���÷��� ������ ���´�.
		m_pD3D->GetAdapterDisplayMode(m_D3DCaps.AdapterOrdinal, &m_D3DDisplay);
		UINT width		= m_WinInfo.nScreenWidth;
		UINT height		= m_WinInfo.nScreenHeight;
		D3DFORMAT fmat	= (m_bWindowMode) ? m_D3DDisplay.Format : D3DFMT_X8R8G8B8; //BackBufferFormat

		// ��üȭ�� �϶��� �׷���ī�尡 �����ϴ� �ػ󵵸� �˻��Ͽ� �ٲ��ش�.
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
				//�� ���ǹ��� �ɸ��� �ʾҴٸ� ���� ����� �ػ󵵷� �����Ѵ�.
				if(i == cnt-1)
				{
					width = m_D3DDisplay.Width;
					height= m_D3DDisplay.Height;
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// ����̽� �Ӽ��� ä���ش�.
		D3DPRESENT_PARAMETERS pp;
		memset(&pp, 0, sizeof(D3DPRESENT_PARAMETERS));	
		ZeroMemory( &pp, sizeof(pp) );
		pp.hDeviceWindow			= m_WinInfo.hWnd;
		pp.Windowed					= m_bWindowMode;
		pp.BackBufferCount			= 1;
		pp.BackBufferWidth			= width;
		pp.BackBufferHeight			= height;
		pp.BackBufferFormat			= fmat;						//�ĸ������ �ȼ� ����
		pp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	//�ø���(����Ʈ���ۿ� ����۸� �ٲ��ִ°�)ü���� ���۰� ��ȯ�Ǵ� ����� �����ϴ� D3DSWAPEFFECT������ ���
		pp.MultiSampleType			= D3DMULTISAMPLE_NONE;
		pp.MultiSampleQuality		= 0;
		pp.EnableAutoDepthStencil	= TRUE;
		pp.FullScreen_RefreshRateInHz = (m_bWindowMode) ? 0 : m_D3DDisplay.RefreshRate;
		pp.PresentationInterval		= D3DPRESENT_INTERVAL_DEFAULT;	// ������ ����.

		// AutoDepthStencilFormat ȯ�濡 �°� �ʱ�ȭ
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
		// ����̽� Setting ���� ����.

		memset(&m_DeviceSetting, 0, sizeof(DXDeviceSettings));
		m_DeviceSetting.pp				= pp;
		m_DeviceSetting.AdapterFormat	= pp.BackBufferFormat;
		m_DeviceSetting.DeviceType		= m_D3DCaps.DeviceType;
		m_DeviceSetting.AdapterOrdinal	= m_D3DCaps.AdapterOrdinal;
		m_DeviceSetting.BehaviorFlags	= vertexProcessing;

		//////////////////////////////////////////////////////////////////////////
		// ����̽� ���� & ����.
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
		//��Ʈ���� �ȷ�Ʈ�� ���� �˻�
		m_pd3dDevice->GetDeviceCaps( &m_D3DCaps );
		int IndexedMatrixMaxSize = m_D3DCaps.MaxVertexBlendMatrixIndex;
		if(IndexedMatrixMaxSize == 0) 
		{
			write_log_file("��Ʈ�����ȷ�Ʈ��������~!");
			MessageBox(NULL,  TEXT("��Ʈ�����ȷ�Ʈ��������"), TEXT("Failed"), MB_OK); 
		}
	
		//----------------------------------
		// ������ ũ�� �缳��.
		/*
		RECT oldrc;				::GetWindowRect(m_hWnd, &oldrc);
		DWORD sytle   = (DWORD) ::GetWindowLong(m_hWnd, GWL_STYLE);
		DWORD exstyle = (DWORD) ::GetWindowLong(m_hWnd, GWL_EXSTYLE);	//exstyle &= ~WS_EX_TOPMOST;
		// ���� ������ �������� 'Ŭ���̾�Ʈ' ���� ũ�� ����ϱ�.(â���-���� �ػ�, ��üȭ��-���÷��� �ػ�)
		RECT newrc;	  SetRect(&newrc, 0, 0, width, height);
		// newrc ��ŭ�� Ŭ���̾�Ʈ ������ �����ϴ� ������ '��ü' ũ�⸦ ���Ѵ�.('�޴�'�� ���ٴ� �����Ͽ� ó��.)
		::AdjustWindowRectEx(&newrc, sytle, NULL, exstyle);
		// ������ �������� �ʺ�� ���� ���Ѵ�. 
		width  = (newrc.right  - newrc.left);
		height = (newrc.bottom - newrc.top);
		//���ο� ũ�⸦ �����쿡 �����Ѵ�.
		::SetWindowPos(m_hWnd, HWND_NOTOPMOST, oldrc.left, oldrc.top, width, height, SWP_SHOWWINDOW);
		
		if(gWindowed) ShowWindow(m_hWnd, SW_SHOW);	// ������ â ���̱�.
		GetClientRect(m_hWnd, &gRect);				// Ŭ���̾�Ʈ ���� ũ�⸦ ���´�.
		*/
		return S_OK;

}

//-----------------------------------------------------------------------------
// ������Ʈ�Լ�
//-----------------------------------------------------------------------------
void CD3DFrameWork::ProcessUpdate()
{
	// engine Timer
	m_DTime = DXUTGetGlobalTimer()->GetElapsedTime();
	if(m_DTime >1.0f) m_DTime  = 0;

	
	// direct input�� ����ϱ� ���� ������Ʈ �ݵ�� �̰�����ȣ��
	SHG_Input.OnFrameMove(m_DTime);

	//����� ������ �Լ�
	OnUpdateStart(m_DTime);

	m_pSceneMgr->UpdateCurScene(m_DTime);

	//����� ������ �Լ�
	OnUpdateEnd(m_DTime);
	

}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CD3DFrameWork::ProcessRender()
{
	//�ϵ���� ����۸� �����س��´�. [2014.11.8 ]
	m_pd3dDevice->GetRenderTarget(0, &(G_ScreenQuad.g_pHWBackBuffer));
	
	m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER , D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );

	//////////////////////////////////////////////////////////////////////////
	// �ϴ� �����̽� 0���� �ؽ��ĸ� �������Ӹ��� �׸��� ���� �Ķ������� Ŭ�������ش�.  [2014.11.19 ]
	if( SUCCEEDED( G_ScreenQuad.g_pRenderTargetTexPass[0]->GetSurfaceLevel( 0, &(G_ScreenQuad.g_pSceneSurface[0]) ) ) )
	{//gpSceneRenderTarget�� �����̽��� ������ٶ� �ʿ��� �༮ [2014.11.7 ]
		m_pd3dDevice->SetRenderTarget( 0, G_ScreenQuad.g_pSceneSurface[0] ); // ���� �����̽��� ����Ÿ������ ����(����) [2014.11.7 ]
		m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER , D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );
	}
	 
	// CD3DFrameWork �� ��ӹ��� Ŭ�������� �������̵� �Ҽ��ִ� �Լ� 
	OnRenderStart();

	if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
	{	
		//�������ӿ� �׷ȴ� ��� ����� �����.  // [2014.11.8 ]
		assert(G_ScreenQuad.ClearAllSurface() == S_OK); // ������� ��� �����. [2014.11.8 ]
		
		// ������� ��� �׸��� �ִ°͵��� �׸���.
		m_pSceneMgr->RenderCurScene();		
		m_pSceneMgr->RenderUI_of_CulScene(m_DTime); // [2014.12.1 ]
// 		RenderText( m_DTime );//�ý��� �ؽ�Ʈ �׸���
// 		RenderUI(m_DTime); // UI�� ���� �������� �׷��ֱ�


		// �ϵ���� ����۸� �ٽ� ����Ѵ�. [2014.11.8 ]
		 m_pd3dDevice->SetRenderTarget( 0, G_ScreenQuad.g_pHWBackBuffer);
		//���⼭ ����Ʈ����Ʈ�� ���� ��ũ������ �ٲ��ش�.  [2014.11.8 ]
		 G_FXShader.GetPointer()->SetTechnique(G_ConstantTable.h_Tech_NoEffect);
		// ������ �ٱ׷��� ������ �ý���(����Ÿ��)��  ���̴��� �����Ѵ�.
	    G_FXShader.GetPointer()->SetTexture(G_ConstantTable.h_Tex_Pass0,  G_ScreenQuad.g_pRenderTargetTexPass[0]);

		UINT numPasses = 0;
		G_FXShader.GetPointer()->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i )
			{
				G_FXShader.GetPointer()->BeginPass(i);
				{
					// ȭ�鿡 �´� �簢���� �׸���.
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
// ����Ǳ������� ȣ��Ǵ� �Լ�
//-----------------------------------------------------------------------------
void CD3DFrameWork::CloseApplication()
{
		SAFE_RELEASE(m_pd3dDevice)

		SAFE_RELEASE(m_pD3D)

	 	SAFE_RELEASE(SHGlobal_pFontSystem)
			 	
	 	SAFE_RELEASE(SHGlobal_pSprite_BDT)

		//����� ������ �Լ�
		OnCloseApp();
}


//-----------------------------------------------------------------------------
// �ν��Ͻ� ��ȯ
//-----------------------------------------------------------------------------
HINSTANCE CD3DFrameWork::GetHINSTANCE()
{
	return m_WinInfo.hInst;
}



//-----------------------------------------------------------------------------
// �������ڵ� ��ȯ
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
	//����������� �޼��� ó��
	m_pSceneMgr->ProcessCurSceneMsg(hWnd,message,wParam,lParam);

	//����� ������ �Լ�
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
// Name   : ������ �޽��� ���ν���
//-----------------------------------------------------------------------------
LRESULT WINAPI CD3DFrameWork::fnWindProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	if(GFrameWork->IsRunning())GFrameWork->ProcessMessage(hWnd,message,wParam,lParam);
	
	return DefWindowProc(hWnd, message, wParam, lParam);


}