

#include "DXUT.H"
// #include "SDKmisc.h"
// #include "Camera.h"
#include "SHGame.h"
// #include "GameObject.h"
// #include "Missile.h"
// #include "ASEData.h"
// #include "ASEParser.h"
//---------------------------------------------------------------------------------------------
// 게임전체에서 사용될 전역변수
//---------------------------------------------------------------------------------------------

ZFrustum						G_Frustum;

D3DMATERIAL9					G_DefaultMtrl;

// 포인트라이트
D3DXVECTOR4						G_PointLightPosition;  // 포인트라이트의 위치 [2014.11.12 ]
float							G_PointLightRadius;  // 포인트라이트의 위치 [2014.11.12 ]
// 빛의 방향 (directional Light)
D3DXVECTOR3						G_vecDirLight;  // [2014.7.27 ]
D3DXCOLOR						G_LightDiff;	// 빛의 디퓨즈[2014.7.28 ]
D3DXCOLOR						G_LightAmbi;	// 빛의 환경광[2014.7.28 ] 

 
//selected Object;
TCHAR							G_SelectedObjName[MAX_NUM_STRING]; //픽킹할때 사용할 변수(이름 잠깐 저장할떄사용)

BoundingSphere					G_BS;//바운딩스피어  
// 나무 빌보드
cSHBillBoard					G_BBoardTree;
//환경맵핑할때 사용할 스카이박스의 큐브맵을 전역변수로 놓는다.   [2014.8.3 ]
LPDIRECT3DCUBETEXTURE9			G_pCubeTex = nullptr;   



//---------------------------------------------------------------------------------------------
// 여기서부터 셰이더 객체 선언
//---------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXSHADER9			G_pVtxShader      = nullptr; //[2014.7.27 ]
LPDIRECT3DVERTEXSHADER9			G_pVtxShader_Skin = nullptr; //[2014.7.29 ]
LPDIRECT3DPIXELSHADER9			G_pPStest		  = nullptr; /// 픽셀 쉐이더 테스트용  [2014.7.28 ]

//---------------------------------------------------------------------------
// [2014.7.25 ]
// 여기서부터 게임내에서 사용될 오브젝트들을 선언
//----------------------------------------------------------------------------
// AirCraft						G_AirCraft; //비행기	[2014.7.25 ]
// cMissile						G_Missile; //미사일	[2014.7.25 ]

cSkyBox							G_SkyBox;  //스카이박스  [2014.8.2 ]	
// [2014.11.2 ]
CDXUTDialog						g_MainPlayUI;             // dialog for sample specific controls
CDXUTDialog						g_MainTitle_UI;           // 메인타이틀에 사용될 다이얼로그


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
// 여기서부터 SHGame class 구현
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
SHGame* SHGame::pInstance = nullptr;


//-----------------------------------------------------------------------------
//생성자 소멸자
//-----------------------------------------------------------------------------
SHGame::SHGame()
{

}


SHGame::~SHGame()
{

}


//-------------------------------------------------------------------------
// main에서 사용되는 함수
// 
// 
//-------------------------------------------------------------------------
int		SHGame::Create(HINSTANCE _hInstance)
{
	HRESULT hr = S_OK ;

	SHWindowInfo tempWinInfo;


	//스크립트에서 화면 가로세로 비율 가져오기 [2014.11.19 ]
	SH_Lua templua("./Script/Camera_Info.lua");
	templua.GetGlobal_int("ScreenWidth", tempWinInfo.nScreenWidth );
	templua.GetGlobal_int("ScreenHeight", tempWinInfo.nScreenHeight );

	
	tempWinInfo.hInst = _hInstance;
	//tempWinInfo.nScreenWidth = 1270;
	//tempWinInfo.nScreenHeight = 880;
	tempWinInfo.WindowTitleName = L"성환 게임1";
	tempWinInfo.WindowMode = true ; 

	// 어플리케이션초기화 , 이곳에서 윈도우를 생성하고 d3d device를 생성한다.  
	hr = InitApplication(&tempWinInfo);
	if(hr == S_FALSE){SHUtil::MsgBox("Fail : InitApplication() "); }

	// 게임씬 생성, UI 생성 ,및 등록 [2014.11.2 ]
	hr = InitGame(); 
	if(hr == S_FALSE){SHUtil::MsgBox("Fail : InitObject() ");      }
	
	return hr;
	
}



 //-------------------------------------------------------------------------
 // 싱글톤 객체 반환
 //-------------------------------------------------------------------------
 SHGame* SHGame::GetSHGame()
{
	if( nullptr == pInstance  )  pInstance = new SHGame();
		 
	 return pInstance;
}




//-----------------------------------------------------------------------------
// 프레임워크 업데이트의 훅 함수
//-----------------------------------------------------------------------------
void	SHGame::OnUpdateStart(float _dTime)
{	

}

//-----------------------------------------------------------------------------
// Name   : OnUpdateEnd
// Arg    : delta time
//-----------------------------------------------------------------------------
void	SHGame::OnUpdateEnd(float _dTime)
{
}


//-----------------------------------------------------------------------------
// 렌더 함수
//-----------------------------------------------------------------------------
 void	SHGame::OnRenderStart()
 {
	
 }


 //-----------------------------------------------------------------------------
 // 렌더 함수
 //-----------------------------------------------------------------------------
 void	SHGame::OnRenderEnd()
 {

 }


 //-----------------------------------------------------------------------------
 // application 종료될때 호출되는 함수
 //-----------------------------------------------------------------------------
 void	SHGame::OnCloseApp()
 {
	 CDialogRscMgr::GetInstance()->OnD3D9DestroyDevice(); // [2014.11.3 ]


	 write_log_file("SHGame::CloseApplication() 호출");

 }



//-----------------------------------------------------------------------------
// Name: InitObject()
// Desc: Creates the scene geometry
//-----------------------------------------------------------------------------
HRESULT SHGame::InitGame()
{
	HRESULT hr = S_OK;

	//게임이 시작되기 전에 초기에 한번만 셰이더를 초기화한다.
 //	G_FXShader.Create(L"StandardFX" , L"./Shader/Standard_Fx_SHG2.fx");// [2014.10.29 ] 
	G_FXShader.Create(L"StandardFX" , L"./Shader/Standard_Fx_SHG2_retouch.fx");// [2014.10.29 ] 
	// 상수테이블을 게임이 시작하기 전에 한번 초기화 한다. [2014.10.30 ] 
	hr = G_ConstantTable.Initialize();  // [2014.10.30 ]

	// 1) 사용될 유아이 전부 이함수에서 미리 만들어주기 [2014.11.5 ]
 	hr = Init_UI();

	// 2) Scene 생성
	// 생성자에서 붙을 UI의 이름을 가지고 있고 이걸로 Scene과 다이얼로그를 서로 연결시켜 준다. [2014.11.5 ]
	CScene* pTempScene = new CScene_MainGame();	 
	// 3) 씬메니저에 "Scene_GameMain" 등록
	hr = m_pSceneMgr->AddScene(L"Scene_GameMain", pTempScene);

	pTempScene = new CScene_Title();	 
	// 3) 씬메니저에 "CScene_Title" 등록
	hr = m_pSceneMgr->AddScene(L"Scene_MainTitle", pTempScene);

	pTempScene = new CScene_Openning();	 
	// 3) 씬메니저에 "CScene_Title" 등록
	hr = m_pSceneMgr->AddScene(L"Scene_Loading", pTempScene);

	pTempScene = new CScene_Fail();	 
	// 3) 씬메니저에 "CScene_Title" 등록
	hr = m_pSceneMgr->AddScene(L"Scene_Fail", pTempScene);

	pTempScene = new CScene_Success();	 
	// 3) 씬메니저에 "CScene_Title" 등록
	hr = m_pSceneMgr->AddScene(L"Scene_Success", pTempScene);

	pTempScene = new CScene_Credit();	 
	// 3) 씬메니저에 "CScene_Title" 등록
	hr = m_pSceneMgr->AddScene(L"Scene_Credit", pTempScene);

	// 4) 현재씬 설정
	hr =m_pSceneMgr->OpenScene(L"Scene_MainTitle");
	G_ButtonAble.SetTimer(1.0f);
	//hr =m_pSceneMgr->OpenScene(L"Scene_GameMain");
	
	// sound메니져 초기화
	G_SHSoundMgr.Init(SHGlobal_GetHwnd);

	G_SHSoundMgr.PlayWave(eSound_BGM_Title, true);

	return hr;
}

//-----------------------------------------------------------------------------
// SHGame UI 초기화 해주는 함수  [2014.11.3 ]
//-----------------------------------------------------------------------------
HRESULT	  SHGame::Init_UI()
{
	HRESULT hr = S_OK;
	//  g_DialogResourceManager 생성및 초기화
	V_RETURN( CDialogRscMgr::GetInstance()->OnD3D9CreateDevice( m_pd3dDevice ) );  // [2014.11.2 ]
	V_RETURN( CDialogRscMgr::GetInstance()->OnD3D9ResetDevice() );  // [2014.11.3 ]

	// 다이얼로그 초기화  [2014.11.2 ]
	g_MainPlayUI.Init( CDialogRscMgr::GetInstance() , true, L"./image/menu.png", "UI_Play");  
	//g_MainPlayUI.SetMediator(CSceneMgr::getInstance()->GetSceneByName(L"Scene_GameMain"));
	g_MainPlayUI.SetLocation( 0, 0 );								// 위치 화면의 좌측상단.
	g_MainPlayUI.SetSize( m_WinInfo.nScreenWidth, m_WinInfo.nScreenHeight ); //크기, 클라이언트 영역으로 
	g_MainPlayUI.SetFont( 1, L"Comic Sans MS", 24, FW_NORMAL );  // 폰트1번  저장
	g_MainPlayUI.SetFont( 2, L"Courier New", 56, FW_NORMAL );    // 폰트 2번 저장

	//////////////////////////////////////////////////////////////////////////
	// static 을 다이얼로그에 붙이기 [2014.11.2 ]
//	g_MainPlayUI.AddStatic( eUI_ID_STATIC, L"This is a static control.", 200, 20, 220, 30 );

	//////////////////////////////////////////////////////////////////////////
	// Buttons1
	//g_MainPlayUI.AddButton( eUI_ID_StartButton , L"Toggle", 500, 20, 100, 35, L'T' );
	// 현재 버튼의 엘리먼트는 0번과 1번이 있는 상태인데,,
	// 0번 상태의 텍스쳐ID를 1(eTEX_ID_SHUI)로 설정
	//g_MainPlayUI.GetButton(eUI_ID_StartButton)->GetElement(0)->iTexture = eTEX_ID_SHUI;

	//////////////////////////////////////////////////////////////////////////
	// Buttons2
// 	g_MainPlayUI.AddButton( eUI_ID_BUTTON2 , L"BUTTON2", 500, 60, 100, 35, L'L' );
// 	// 현재 버튼의 엘리먼트는 0번과 1번이 있는 상태인데,,
// 	// 0번 상태의 텍스쳐ID를 1(eTEX_ID_CustomUI)로 설정
// 	g_MainPlayUI.GetButton(eUI_ID_BUTTON2)->GetElement(0)->iTexture = eTEX_ID_CustomUI;

	// 여기서부터 메인게임 UI 스크립트에서 읽어들인후 다이얼로그에 UI 붙이기  [2014.12.1 ]

	int PosX,PosY,Width,Height;
	// 메인타이틀 UI 초기화 [2014.12.4 ]
	g_MainTitle_UI.Init( CDialogRscMgr::GetInstance() , true, L"./image/menu.png", "UI_Title");  
	g_MainTitle_UI.SetCallback(OnGUIEvent_Title);

	{
		//////////////////////////////////////////////////////////////////////////
		// progress bar [2014.11.5 ]
		SH_Lua TempLua("./Script/UI_Attr.lua");

		TempLua.GetGlobal_int("G_SkillPoinBar_Pos_X", PosX);
		TempLua.GetGlobal_int("G_SkillPoinBar_Pos_Y", PosY);
		TempLua.GetGlobal_int("G_SkillPoinBar_Width", Width);
		TempLua.GetGlobal_int("G_SkillPoinBar_Height", Height);
		g_MainPlayUI.AddProgressBar(eUI_ID_SkillPointBar , PosX , PosY,Width,Height,0,1);
		
		TempLua.GetGlobal_int("G_HPBar_Pos_X", PosX);
		TempLua.GetGlobal_int("G_HPBar_Pos_Y", PosY);
		TempLua.GetGlobal_int("G_HPBar_Width", Width);
		TempLua.GetGlobal_int("G_HPBar_Height", Height);
		g_MainPlayUI.AddProgressBar(eUI_ID_HPBar , PosX , PosY, Width, Height, 2,3);

		// 스킬1번 이미지 추가 [2014.12.4 ]
		TempLua.GetGlobal_int("G_Skill1_Pos_X", PosX);
		TempLua.GetGlobal_int("G_Skill1_Pos_Y", PosY);
		TempLua.GetGlobal_int("G_Skill1_Width", Width);
		TempLua.GetGlobal_int("G_Skill1_Height", Height);
		g_MainPlayUI.AddImage(eUI_ID_SkillImg1 , NULL, PosX , PosY, Width, Height, eEle_Skill1_Image1,eEle_Skill1_Image2);
		// 각 엘리먼트들의 텍스쳐ID를 1(eTEX_ID_SHUI)로 설정
		g_MainPlayUI.GetImage(eUI_ID_SkillImg1)->GetElement(eEle_Skill1_Image1)->iTexture = eTEX_ID_SHUI;
		g_MainPlayUI.GetImage(eUI_ID_SkillImg1)->GetElement(eEle_Skill1_Image2)->iTexture = eTEX_ID_SHUI;
		g_MainPlayUI.GetImage(eUI_ID_SkillImg1)->SetEnabled(false);
		// 스킬2번 이미지 추가 [2014.12.4 ]
		TempLua.GetGlobal_int("G_Skill2_Pos_X", PosX);
		TempLua.GetGlobal_int("G_Skill2_Pos_Y", PosY);
		TempLua.GetGlobal_int("G_Skill2_Width", Width);
		TempLua.GetGlobal_int("G_Skill2_Height", Height);
		g_MainPlayUI.AddImage(eUI_ID_SkillImg2 , NULL, PosX , PosY, Width, Height, eEle_Skill2_Image1,eEle_Skill2_Image2);
		// 각 엘리먼트들의 텍스쳐ID를 1(eTEX_ID_SHUI)로 설정
		g_MainPlayUI.GetImage(eUI_ID_SkillImg2)->GetElement(eEle_Skill2_Image1)->iTexture = eTEX_ID_SHUI;
		g_MainPlayUI.GetImage(eUI_ID_SkillImg2)->GetElement(eEle_Skill2_Image2)->iTexture = eTEX_ID_SHUI;
		g_MainPlayUI.GetImage(eUI_ID_SkillImg2)->SetEnabled(false);

		// 스킬3번 이미지 추가 [2014.12.4 ]
		TempLua.GetGlobal_int("G_Skill3_Pos_X", PosX);
		TempLua.GetGlobal_int("G_Skill3_Pos_Y", PosY);
		TempLua.GetGlobal_int("G_Skill3_Width", Width);
		TempLua.GetGlobal_int("G_Skill3_Height", Height);
		g_MainPlayUI.AddImage(eUI_ID_SkillImg3 , NULL, PosX , PosY, Width, Height, eEle_Skill3_Image1,eEle_Skill3_Image2);
		// 각 엘리먼트들의 텍스쳐ID를 1(eTEX_ID_SHUI)로 설정
		g_MainPlayUI.GetImage(eUI_ID_SkillImg3)->GetElement(eEle_Skill3_Image1)->iTexture = eTEX_ID_SHUI;
		g_MainPlayUI.GetImage(eUI_ID_SkillImg3)->GetElement(eEle_Skill3_Image2)->iTexture = eTEX_ID_SHUI;
		g_MainPlayUI.GetImage(eUI_ID_SkillImg3)->SetEnabled(false); //[2014.12.4 ]

		// 모자 이미지 추가 [2014.12.4 ]
		TempLua.GetGlobal_int("G_Hat_Pos_X", PosX);
		TempLua.GetGlobal_int("G_Hat_Pos_Y", PosY);
		TempLua.GetGlobal_int("G_Hat_Width", Width);
		TempLua.GetGlobal_int("G_Hat_Height", Height);
		g_MainPlayUI.AddImage(eUI_ID_HatImg , NULL, PosX , PosY, Width, Height, eEle_Hat,eEle_Hat);
		// 각 엘리먼트들의 텍스쳐ID를 1(eTEX_ID_SHUI)로 설정
		g_MainPlayUI.GetImage(eUI_ID_HatImg)->GetElement(eEle_Hat)->iTexture = eTEX_ID_SHUI;
		
		// 타임슬라이더 추가 [2014.12.4 ]
		TempLua.GetGlobal_int("G_TimerSlider_Pos_X", PosX);
		TempLua.GetGlobal_int("G_TimerSlider_Pos_Y", PosY);
		TempLua.GetGlobal_int("G_TimerSlider_Width", Width);
		TempLua.GetGlobal_int("G_TimerSlider_Height", Height);
		g_MainPlayUI.AddSlider(eUI_ID_TimerSlider , PosX , PosY, Width, Height, eEle_Timer_Stick, eEle_Timer_Button);
		// 각 엘리먼트들의 텍스쳐ID를 1(eTEX_ID_SHUI)로 설정
		g_MainPlayUI.GetSlider(eUI_ID_TimerSlider)->GetElement(eEle_Timer_Stick)->iTexture = eTEX_ID_SHUI;
		g_MainPlayUI.GetSlider(eUI_ID_TimerSlider)->GetElement(eEle_Timer_Button)->iTexture = eTEX_ID_SHUI;


	}
	
	{
		// 시작버튼.  [2014.12.3 ]
		SH_Lua TempLua("./Script/UI_Attr.lua");
		TempLua.GetGlobal_int("G_StartButton_Pos_X", PosX);
		TempLua.GetGlobal_int("G_StartButton_Pos_Y", PosY);
		TempLua.GetGlobal_int("G_StartButton_Width", Width);
		TempLua.GetGlobal_int("G_StartButton_Height", Height);
		g_MainTitle_UI.AddButton(eUI_ID_StartButton ,NULL, PosX , PosY, Width, Height, eEle_StartButton1, eEle_StartButton2); // [2014.12.3 ]
		// eEle_StartButton1 번 상태의 텍스쳐ID를 1(eTEX_ID_SHUI)로 설정
		g_MainTitle_UI.GetButton(eUI_ID_StartButton)->GetElement(eEle_StartButton1)->iTexture = eTEX_ID_SHUI;
		g_MainTitle_UI.GetButton(eUI_ID_StartButton)->GetElement(eEle_StartButton2)->iTexture = eTEX_ID_SHUI;

		// 크래딧 버튼  [2014.12.3 ]
		TempLua.GetGlobal_int("G_CreditButton_Pos_X", PosX);
		TempLua.GetGlobal_int("G_CreditButton_Pos_Y", PosY);
		TempLua.GetGlobal_int("G_CreditButton_Width", Width);
		TempLua.GetGlobal_int("G_CreditButton_Height", Height);
		g_MainTitle_UI.AddButton(eUI_ID_CreditButton ,NULL, PosX , PosY, Width, Height, eEle_CreditButton1 , eEle_CreditButton2); // [2014.12.3 ]
		
		// 0번 상태의 텍스쳐ID를 1(eTEX_ID_SHUI)로 설정
		g_MainTitle_UI.GetButton(eUI_ID_CreditButton)->GetElement(eEle_CreditButton1)->iTexture = eTEX_ID_SHUI;
		g_MainTitle_UI.GetButton(eUI_ID_CreditButton)->GetElement(eEle_CreditButton2)->iTexture = eTEX_ID_SHUI;

		// exit 버튼  [2014.12.3 ]
		TempLua.GetGlobal_int("G_ExitButton_Pos_X", PosX);
		TempLua.GetGlobal_int("G_ExitButton_Pos_Y", PosY);
		TempLua.GetGlobal_int("G_ExitButton_Width", Width);
		TempLua.GetGlobal_int("G_ExitButton_Height", Height);
		g_MainTitle_UI.AddButton(eUI_ID_ExitButton , NULL, PosX , PosY, Width, Height, eEle_ExitButton1 , eEle_ExitButton2); // [2014.12.3 ]

		// 0번 상태의 텍스쳐ID를 1(eTEX_ID_SHUI)로 설정
		g_MainTitle_UI.GetButton(eUI_ID_ExitButton)->GetElement(eEle_ExitButton1)->iTexture = eTEX_ID_SHUI;
		g_MainTitle_UI.GetButton(eUI_ID_ExitButton)->GetElement(eEle_ExitButton2)->iTexture = eTEX_ID_SHUI;


		// 스킬3번 이미지 추가 [2014.12.4 ]
		TempLua.GetGlobal_int("G_Logo_Pos_X", PosX);
		TempLua.GetGlobal_int("G_Logo_Pos_Y", PosY);
		TempLua.GetGlobal_int("G_Logo_Width", Width);
		TempLua.GetGlobal_int("G_Logo_Height", Height);
		g_MainTitle_UI.AddImage(eUI_ID_Logo , NULL, PosX , PosY, Width, Height, eEle_TeddyVSNight, eEle_TeddyVSNight);
		// 각 엘리먼트들의 텍스쳐ID를 1(eTEX_ID_SHUI)로 설정
		g_MainTitle_UI.GetImage(eUI_ID_Logo)->GetElement(eEle_TeddyVSNight)->iTexture = eTEX_ID_SHUI;


	}
	
	LOG_WRITE_A(" SHGame::Init_UI() , S_OK 반환");
	return hr;

}


//-----------------------------------------------------------------------------
// SHGame 메세지 처리 
// 훅함수
//-----------------------------------------------------------------------------
void	  SHGame::OnGameMsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// 다이얼로그 메세지 처리
	g_MainPlayUI.MsgProc( hWnd, message, wParam, lParam );//  [2014.11.2 ]	

	if(G_CurSceneType == eSceneType_MainTitle)
	{
		
		
		if(G_ButtonAble.isTimeUp() == true)
		g_MainTitle_UI.MsgProc( hWnd, message, wParam, lParam );

	}//  [2014.11.2 ]	
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

	}



}




//--------------------------------------------------------------------------------------
// Render the help and statistics text. This function uses the ID3DXFont interface for 
// efficient text rendering.
//--------------------------------------------------------------------------------------
// void SHGame::RenderText( float fTime )
// {
// 	
// 	// The helper object simply helps keep track of text position, and color
// 	// and then it calls pFont->DrawText( m_pSprite, strMsg, -1, &rc, DT_NOCLIP, m_clr );
// 	// If NULL is passed in as the sprite object, then it will work fine however the 
// 	// pFont->DrawText() will not be batched together.  Batching calls will improves perf.
// 
// 
// 	// Output statistics
// 	m_txtHelper.Begin();
// 
// 	m_txtHelper.SetInsertionPos( 5, 0 );
// 	m_txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
// 	m_txtHelper.DrawFormattedTextLine(L"FPS : %.2f" , SHUtil::getFPS(fTime)); // Show FPS
// 	//m_txtHelper.DrawFormattedTextLine(L"Count of Object In Frustum : %d" , G_Frustum.Get_InnerObjCnt());
// 	//m_txtHelper.DrawFormattedTextLine(L"%s",G_SelectedObjName);
// 
// 	m_txtHelper.End();
// 	
// }
// 

//-------------------------------------------------------------------------
// UI그려주는 함수 재정의   [2014.11.3 ]
//-------------------------------------------------------------------------
// void  SHGame::RenderUI( float DTime )
// {
// 	 g_MainPlayUI.OnRender( DTime ) ;
// }


/*************************************  end of "SHGame.cpp" ****************************************/