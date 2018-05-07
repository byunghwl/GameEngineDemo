

#include "DXUT.H"
// #include "SDKmisc.h"
// #include "Camera.h"
#include "SHGame.h"
// #include "GameObject.h"
// #include "Missile.h"
// #include "ASEData.h"
// #include "ASEParser.h"
//---------------------------------------------------------------------------------------------
// ������ü���� ���� ��������
//---------------------------------------------------------------------------------------------

ZFrustum						G_Frustum;

D3DMATERIAL9					G_DefaultMtrl;

// ����Ʈ����Ʈ
D3DXVECTOR4						G_PointLightPosition;  // ����Ʈ����Ʈ�� ��ġ [2014.11.12 ]
float							G_PointLightRadius;  // ����Ʈ����Ʈ�� ��ġ [2014.11.12 ]
// ���� ���� (directional Light)
D3DXVECTOR3						G_vecDirLight;  // [2014.7.27 ]
D3DXCOLOR						G_LightDiff;	// ���� ��ǻ��[2014.7.28 ]
D3DXCOLOR						G_LightAmbi;	// ���� ȯ�汤[2014.7.28 ] 

 
//selected Object;
TCHAR							G_SelectedObjName[MAX_NUM_STRING]; //��ŷ�Ҷ� ����� ����(�̸� ��� �����ҋ����)

BoundingSphere					G_BS;//�ٿ�����Ǿ�  
// ���� ������
cSHBillBoard					G_BBoardTree;
//ȯ������Ҷ� ����� ��ī�̹ڽ��� ť����� ���������� ���´�.   [2014.8.3 ]
LPDIRECT3DCUBETEXTURE9			G_pCubeTex = nullptr;   



//---------------------------------------------------------------------------------------------
// ���⼭���� ���̴� ��ü ����
//---------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXSHADER9			G_pVtxShader      = nullptr; //[2014.7.27 ]
LPDIRECT3DVERTEXSHADER9			G_pVtxShader_Skin = nullptr; //[2014.7.29 ]
LPDIRECT3DPIXELSHADER9			G_pPStest		  = nullptr; /// �ȼ� ���̴� �׽�Ʈ��  [2014.7.28 ]

//---------------------------------------------------------------------------
// [2014.7.25 ]
// ���⼭���� ���ӳ����� ���� ������Ʈ���� ����
//----------------------------------------------------------------------------
// AirCraft						G_AirCraft; //�����	[2014.7.25 ]
// cMissile						G_Missile; //�̻���	[2014.7.25 ]

cSkyBox							G_SkyBox;  //��ī�̹ڽ�  [2014.8.2 ]	
// [2014.11.2 ]
CDXUTDialog						g_MainPlayUI;             // dialog for sample specific controls
CDXUTDialog						g_MainTitle_UI;           // ����Ÿ��Ʋ�� ���� ���̾�α�


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
// ���⼭���� SHGame class ����
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
//������ �Ҹ���
//-----------------------------------------------------------------------------
SHGame::SHGame()
{

}


SHGame::~SHGame()
{

}


//-------------------------------------------------------------------------
// main���� ���Ǵ� �Լ�
// 
// 
//-------------------------------------------------------------------------
int		SHGame::Create(HINSTANCE _hInstance)
{
	HRESULT hr = S_OK ;

	SHWindowInfo tempWinInfo;


	//��ũ��Ʈ���� ȭ�� ���μ��� ���� �������� [2014.11.19 ]
	SH_Lua templua("./Script/Camera_Info.lua");
	templua.GetGlobal_int("ScreenWidth", tempWinInfo.nScreenWidth );
	templua.GetGlobal_int("ScreenHeight", tempWinInfo.nScreenHeight );

	
	tempWinInfo.hInst = _hInstance;
	//tempWinInfo.nScreenWidth = 1270;
	//tempWinInfo.nScreenHeight = 880;
	tempWinInfo.WindowTitleName = L"��ȯ ����1";
	tempWinInfo.WindowMode = true ; 

	// ���ø����̼��ʱ�ȭ , �̰����� �����츦 �����ϰ� d3d device�� �����Ѵ�.  
	hr = InitApplication(&tempWinInfo);
	if(hr == S_FALSE){SHUtil::MsgBox("Fail : InitApplication() "); }

	// ���Ӿ� ����, UI ���� ,�� ��� [2014.11.2 ]
	hr = InitGame(); 
	if(hr == S_FALSE){SHUtil::MsgBox("Fail : InitObject() ");      }
	
	return hr;
	
}



 //-------------------------------------------------------------------------
 // �̱��� ��ü ��ȯ
 //-------------------------------------------------------------------------
 SHGame* SHGame::GetSHGame()
{
	if( nullptr == pInstance  )  pInstance = new SHGame();
		 
	 return pInstance;
}




//-----------------------------------------------------------------------------
// �����ӿ�ũ ������Ʈ�� �� �Լ�
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
// ���� �Լ�
//-----------------------------------------------------------------------------
 void	SHGame::OnRenderStart()
 {
	
 }


 //-----------------------------------------------------------------------------
 // ���� �Լ�
 //-----------------------------------------------------------------------------
 void	SHGame::OnRenderEnd()
 {

 }


 //-----------------------------------------------------------------------------
 // application ����ɶ� ȣ��Ǵ� �Լ�
 //-----------------------------------------------------------------------------
 void	SHGame::OnCloseApp()
 {
	 CDialogRscMgr::GetInstance()->OnD3D9DestroyDevice(); // [2014.11.3 ]


	 write_log_file("SHGame::CloseApplication() ȣ��");

 }



//-----------------------------------------------------------------------------
// Name: InitObject()
// Desc: Creates the scene geometry
//-----------------------------------------------------------------------------
HRESULT SHGame::InitGame()
{
	HRESULT hr = S_OK;

	//������ ���۵Ǳ� ���� �ʱ⿡ �ѹ��� ���̴��� �ʱ�ȭ�Ѵ�.
 //	G_FXShader.Create(L"StandardFX" , L"./Shader/Standard_Fx_SHG2.fx");// [2014.10.29 ] 
	G_FXShader.Create(L"StandardFX" , L"./Shader/Standard_Fx_SHG2_retouch.fx");// [2014.10.29 ] 
	// ������̺��� ������ �����ϱ� ���� �ѹ� �ʱ�ȭ �Ѵ�. [2014.10.30 ] 
	hr = G_ConstantTable.Initialize();  // [2014.10.30 ]

	// 1) ���� ������ ���� ���Լ����� �̸� ������ֱ� [2014.11.5 ]
 	hr = Init_UI();

	// 2) Scene ����
	// �����ڿ��� ���� UI�� �̸��� ������ �ְ� �̰ɷ� Scene�� ���̾�α׸� ���� ������� �ش�. [2014.11.5 ]
	CScene* pTempScene = new CScene_MainGame();	 
	// 3) ���޴����� "Scene_GameMain" ���
	hr = m_pSceneMgr->AddScene(L"Scene_GameMain", pTempScene);

	pTempScene = new CScene_Title();	 
	// 3) ���޴����� "CScene_Title" ���
	hr = m_pSceneMgr->AddScene(L"Scene_MainTitle", pTempScene);

	pTempScene = new CScene_Openning();	 
	// 3) ���޴����� "CScene_Title" ���
	hr = m_pSceneMgr->AddScene(L"Scene_Loading", pTempScene);

	pTempScene = new CScene_Fail();	 
	// 3) ���޴����� "CScene_Title" ���
	hr = m_pSceneMgr->AddScene(L"Scene_Fail", pTempScene);

	pTempScene = new CScene_Success();	 
	// 3) ���޴����� "CScene_Title" ���
	hr = m_pSceneMgr->AddScene(L"Scene_Success", pTempScene);

	pTempScene = new CScene_Credit();	 
	// 3) ���޴����� "CScene_Title" ���
	hr = m_pSceneMgr->AddScene(L"Scene_Credit", pTempScene);

	// 4) ����� ����
	hr =m_pSceneMgr->OpenScene(L"Scene_MainTitle");
	G_ButtonAble.SetTimer(1.0f);
	//hr =m_pSceneMgr->OpenScene(L"Scene_GameMain");
	
	// sound�޴��� �ʱ�ȭ
	G_SHSoundMgr.Init(SHGlobal_GetHwnd);

	G_SHSoundMgr.PlayWave(eSound_BGM_Title, true);

	return hr;
}

//-----------------------------------------------------------------------------
// SHGame UI �ʱ�ȭ ���ִ� �Լ�  [2014.11.3 ]
//-----------------------------------------------------------------------------
HRESULT	  SHGame::Init_UI()
{
	HRESULT hr = S_OK;
	//  g_DialogResourceManager ������ �ʱ�ȭ
	V_RETURN( CDialogRscMgr::GetInstance()->OnD3D9CreateDevice( m_pd3dDevice ) );  // [2014.11.2 ]
	V_RETURN( CDialogRscMgr::GetInstance()->OnD3D9ResetDevice() );  // [2014.11.3 ]

	// ���̾�α� �ʱ�ȭ  [2014.11.2 ]
	g_MainPlayUI.Init( CDialogRscMgr::GetInstance() , true, L"./image/menu.png", "UI_Play");  
	//g_MainPlayUI.SetMediator(CSceneMgr::getInstance()->GetSceneByName(L"Scene_GameMain"));
	g_MainPlayUI.SetLocation( 0, 0 );								// ��ġ ȭ���� �������.
	g_MainPlayUI.SetSize( m_WinInfo.nScreenWidth, m_WinInfo.nScreenHeight ); //ũ��, Ŭ���̾�Ʈ �������� 
	g_MainPlayUI.SetFont( 1, L"Comic Sans MS", 24, FW_NORMAL );  // ��Ʈ1��  ����
	g_MainPlayUI.SetFont( 2, L"Courier New", 56, FW_NORMAL );    // ��Ʈ 2�� ����

	//////////////////////////////////////////////////////////////////////////
	// static �� ���̾�α׿� ���̱� [2014.11.2 ]
//	g_MainPlayUI.AddStatic( eUI_ID_STATIC, L"This is a static control.", 200, 20, 220, 30 );

	//////////////////////////////////////////////////////////////////////////
	// Buttons1
	//g_MainPlayUI.AddButton( eUI_ID_StartButton , L"Toggle", 500, 20, 100, 35, L'T' );
	// ���� ��ư�� ������Ʈ�� 0���� 1���� �ִ� �����ε�,,
	// 0�� ������ �ؽ���ID�� 1(eTEX_ID_SHUI)�� ����
	//g_MainPlayUI.GetButton(eUI_ID_StartButton)->GetElement(0)->iTexture = eTEX_ID_SHUI;

	//////////////////////////////////////////////////////////////////////////
	// Buttons2
// 	g_MainPlayUI.AddButton( eUI_ID_BUTTON2 , L"BUTTON2", 500, 60, 100, 35, L'L' );
// 	// ���� ��ư�� ������Ʈ�� 0���� 1���� �ִ� �����ε�,,
// 	// 0�� ������ �ؽ���ID�� 1(eTEX_ID_CustomUI)�� ����
// 	g_MainPlayUI.GetButton(eUI_ID_BUTTON2)->GetElement(0)->iTexture = eTEX_ID_CustomUI;

	// ���⼭���� ���ΰ��� UI ��ũ��Ʈ���� �о������ ���̾�α׿� UI ���̱�  [2014.12.1 ]

	int PosX,PosY,Width,Height;
	// ����Ÿ��Ʋ UI �ʱ�ȭ [2014.12.4 ]
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

		// ��ų1�� �̹��� �߰� [2014.12.4 ]
		TempLua.GetGlobal_int("G_Skill1_Pos_X", PosX);
		TempLua.GetGlobal_int("G_Skill1_Pos_Y", PosY);
		TempLua.GetGlobal_int("G_Skill1_Width", Width);
		TempLua.GetGlobal_int("G_Skill1_Height", Height);
		g_MainPlayUI.AddImage(eUI_ID_SkillImg1 , NULL, PosX , PosY, Width, Height, eEle_Skill1_Image1,eEle_Skill1_Image2);
		// �� ������Ʈ���� �ؽ���ID�� 1(eTEX_ID_SHUI)�� ����
		g_MainPlayUI.GetImage(eUI_ID_SkillImg1)->GetElement(eEle_Skill1_Image1)->iTexture = eTEX_ID_SHUI;
		g_MainPlayUI.GetImage(eUI_ID_SkillImg1)->GetElement(eEle_Skill1_Image2)->iTexture = eTEX_ID_SHUI;
		g_MainPlayUI.GetImage(eUI_ID_SkillImg1)->SetEnabled(false);
		// ��ų2�� �̹��� �߰� [2014.12.4 ]
		TempLua.GetGlobal_int("G_Skill2_Pos_X", PosX);
		TempLua.GetGlobal_int("G_Skill2_Pos_Y", PosY);
		TempLua.GetGlobal_int("G_Skill2_Width", Width);
		TempLua.GetGlobal_int("G_Skill2_Height", Height);
		g_MainPlayUI.AddImage(eUI_ID_SkillImg2 , NULL, PosX , PosY, Width, Height, eEle_Skill2_Image1,eEle_Skill2_Image2);
		// �� ������Ʈ���� �ؽ���ID�� 1(eTEX_ID_SHUI)�� ����
		g_MainPlayUI.GetImage(eUI_ID_SkillImg2)->GetElement(eEle_Skill2_Image1)->iTexture = eTEX_ID_SHUI;
		g_MainPlayUI.GetImage(eUI_ID_SkillImg2)->GetElement(eEle_Skill2_Image2)->iTexture = eTEX_ID_SHUI;
		g_MainPlayUI.GetImage(eUI_ID_SkillImg2)->SetEnabled(false);

		// ��ų3�� �̹��� �߰� [2014.12.4 ]
		TempLua.GetGlobal_int("G_Skill3_Pos_X", PosX);
		TempLua.GetGlobal_int("G_Skill3_Pos_Y", PosY);
		TempLua.GetGlobal_int("G_Skill3_Width", Width);
		TempLua.GetGlobal_int("G_Skill3_Height", Height);
		g_MainPlayUI.AddImage(eUI_ID_SkillImg3 , NULL, PosX , PosY, Width, Height, eEle_Skill3_Image1,eEle_Skill3_Image2);
		// �� ������Ʈ���� �ؽ���ID�� 1(eTEX_ID_SHUI)�� ����
		g_MainPlayUI.GetImage(eUI_ID_SkillImg3)->GetElement(eEle_Skill3_Image1)->iTexture = eTEX_ID_SHUI;
		g_MainPlayUI.GetImage(eUI_ID_SkillImg3)->GetElement(eEle_Skill3_Image2)->iTexture = eTEX_ID_SHUI;
		g_MainPlayUI.GetImage(eUI_ID_SkillImg3)->SetEnabled(false); //[2014.12.4 ]

		// ���� �̹��� �߰� [2014.12.4 ]
		TempLua.GetGlobal_int("G_Hat_Pos_X", PosX);
		TempLua.GetGlobal_int("G_Hat_Pos_Y", PosY);
		TempLua.GetGlobal_int("G_Hat_Width", Width);
		TempLua.GetGlobal_int("G_Hat_Height", Height);
		g_MainPlayUI.AddImage(eUI_ID_HatImg , NULL, PosX , PosY, Width, Height, eEle_Hat,eEle_Hat);
		// �� ������Ʈ���� �ؽ���ID�� 1(eTEX_ID_SHUI)�� ����
		g_MainPlayUI.GetImage(eUI_ID_HatImg)->GetElement(eEle_Hat)->iTexture = eTEX_ID_SHUI;
		
		// Ÿ�ӽ����̴� �߰� [2014.12.4 ]
		TempLua.GetGlobal_int("G_TimerSlider_Pos_X", PosX);
		TempLua.GetGlobal_int("G_TimerSlider_Pos_Y", PosY);
		TempLua.GetGlobal_int("G_TimerSlider_Width", Width);
		TempLua.GetGlobal_int("G_TimerSlider_Height", Height);
		g_MainPlayUI.AddSlider(eUI_ID_TimerSlider , PosX , PosY, Width, Height, eEle_Timer_Stick, eEle_Timer_Button);
		// �� ������Ʈ���� �ؽ���ID�� 1(eTEX_ID_SHUI)�� ����
		g_MainPlayUI.GetSlider(eUI_ID_TimerSlider)->GetElement(eEle_Timer_Stick)->iTexture = eTEX_ID_SHUI;
		g_MainPlayUI.GetSlider(eUI_ID_TimerSlider)->GetElement(eEle_Timer_Button)->iTexture = eTEX_ID_SHUI;


	}
	
	{
		// ���۹�ư.  [2014.12.3 ]
		SH_Lua TempLua("./Script/UI_Attr.lua");
		TempLua.GetGlobal_int("G_StartButton_Pos_X", PosX);
		TempLua.GetGlobal_int("G_StartButton_Pos_Y", PosY);
		TempLua.GetGlobal_int("G_StartButton_Width", Width);
		TempLua.GetGlobal_int("G_StartButton_Height", Height);
		g_MainTitle_UI.AddButton(eUI_ID_StartButton ,NULL, PosX , PosY, Width, Height, eEle_StartButton1, eEle_StartButton2); // [2014.12.3 ]
		// eEle_StartButton1 �� ������ �ؽ���ID�� 1(eTEX_ID_SHUI)�� ����
		g_MainTitle_UI.GetButton(eUI_ID_StartButton)->GetElement(eEle_StartButton1)->iTexture = eTEX_ID_SHUI;
		g_MainTitle_UI.GetButton(eUI_ID_StartButton)->GetElement(eEle_StartButton2)->iTexture = eTEX_ID_SHUI;

		// ũ���� ��ư  [2014.12.3 ]
		TempLua.GetGlobal_int("G_CreditButton_Pos_X", PosX);
		TempLua.GetGlobal_int("G_CreditButton_Pos_Y", PosY);
		TempLua.GetGlobal_int("G_CreditButton_Width", Width);
		TempLua.GetGlobal_int("G_CreditButton_Height", Height);
		g_MainTitle_UI.AddButton(eUI_ID_CreditButton ,NULL, PosX , PosY, Width, Height, eEle_CreditButton1 , eEle_CreditButton2); // [2014.12.3 ]
		
		// 0�� ������ �ؽ���ID�� 1(eTEX_ID_SHUI)�� ����
		g_MainTitle_UI.GetButton(eUI_ID_CreditButton)->GetElement(eEle_CreditButton1)->iTexture = eTEX_ID_SHUI;
		g_MainTitle_UI.GetButton(eUI_ID_CreditButton)->GetElement(eEle_CreditButton2)->iTexture = eTEX_ID_SHUI;

		// exit ��ư  [2014.12.3 ]
		TempLua.GetGlobal_int("G_ExitButton_Pos_X", PosX);
		TempLua.GetGlobal_int("G_ExitButton_Pos_Y", PosY);
		TempLua.GetGlobal_int("G_ExitButton_Width", Width);
		TempLua.GetGlobal_int("G_ExitButton_Height", Height);
		g_MainTitle_UI.AddButton(eUI_ID_ExitButton , NULL, PosX , PosY, Width, Height, eEle_ExitButton1 , eEle_ExitButton2); // [2014.12.3 ]

		// 0�� ������ �ؽ���ID�� 1(eTEX_ID_SHUI)�� ����
		g_MainTitle_UI.GetButton(eUI_ID_ExitButton)->GetElement(eEle_ExitButton1)->iTexture = eTEX_ID_SHUI;
		g_MainTitle_UI.GetButton(eUI_ID_ExitButton)->GetElement(eEle_ExitButton2)->iTexture = eTEX_ID_SHUI;


		// ��ų3�� �̹��� �߰� [2014.12.4 ]
		TempLua.GetGlobal_int("G_Logo_Pos_X", PosX);
		TempLua.GetGlobal_int("G_Logo_Pos_Y", PosY);
		TempLua.GetGlobal_int("G_Logo_Width", Width);
		TempLua.GetGlobal_int("G_Logo_Height", Height);
		g_MainTitle_UI.AddImage(eUI_ID_Logo , NULL, PosX , PosY, Width, Height, eEle_TeddyVSNight, eEle_TeddyVSNight);
		// �� ������Ʈ���� �ؽ���ID�� 1(eTEX_ID_SHUI)�� ����
		g_MainTitle_UI.GetImage(eUI_ID_Logo)->GetElement(eEle_TeddyVSNight)->iTexture = eTEX_ID_SHUI;


	}
	
	LOG_WRITE_A(" SHGame::Init_UI() , S_OK ��ȯ");
	return hr;

}


//-----------------------------------------------------------------------------
// SHGame �޼��� ó�� 
// ���Լ�
//-----------------------------------------------------------------------------
void	  SHGame::OnGameMsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// ���̾�α� �޼��� ó��
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
// UI�׷��ִ� �Լ� ������   [2014.11.3 ]
//-------------------------------------------------------------------------
// void  SHGame::RenderUI( float DTime )
// {
// 	 g_MainPlayUI.OnRender( DTime ) ;
// }


/*************************************  end of "SHGame.cpp" ****************************************/