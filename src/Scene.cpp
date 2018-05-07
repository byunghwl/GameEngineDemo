#include "DXUT.h"
#include "SHR_Mesh.h"
#include "Scene.h"


//////////////////////////////////////////////////////////////////////////
// �Ϲ��Լ�



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
// ���⼭���� class CScene_MainGame �Լ�����
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



//-----------------------------------------------------------------------------
// ������ 
//-----------------------------------------------------------------------------
CScene_MainGame::CScene_MainGame()
{
	///////////////////////////////////////////////////////////////////////////
	//����Ʈ����Ʈ ����
// 	G_PointLightPosition =  D3DXVECTOR4(0.0f, 40.0f, 0.0f, 1.0f);  // ����Ʈ����Ʈ�� ��ġ [2014.11.13 ]
// 	G_PointLightRadius = 300.0f;    // ����Ʈ����Ʈ�� ��ġ [2014.11.13 ]
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	// directional light  ����<�ӽ�>
 	G_vecDirLight = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
// 	G_LightDiff = D3DCOLOR_RGBA(255,202,95,255);
// 	G_LightAmbi = D3DCOLOR_RGBA(93,92,250,80);
	// ���⼭ ���� ������ ��ũ��Ʈ���� �������� [2014.11.29 ]
	{ //
		SH_Lua TempLua("./Script/Light_Attr.lua");
		int R,G,B,A;
		TempLua.GetGlobal_int("G_PointLigth_Color_R" ,R );
		TempLua.GetGlobal_int("G_PointLigth_Color_G" ,G );
		TempLua.GetGlobal_int("G_PointLigth_Color_B" ,B );
		TempLua.GetGlobal_int("G_PointLigth_Color_A" ,A );

		G_LightDiff = D3DCOLOR_RGBA(R,G,B,A);

		TempLua.GetGlobal_float("G_PointLight_Radius" ,G_PointLightRadius );

		TempLua.GetGlobal_int("G_AmbientLight_Color_R" ,R );
		TempLua.GetGlobal_int("G_AmbientLight_Color_G" ,G );
		TempLua.GetGlobal_int("G_AmbientLight_Color_B" ,B );
		TempLua.GetGlobal_int("G_AmbientLight_Color_A" ,A );

		G_LightAmbi = D3DCOLOR_RGBA(R,G,B,A);

	}

	

	D3DLIGHT9 dirLight = SHUtil::InitDirectionalLight(&G_vecDirLight, &G_LightDiff);

	// light�� �����ϰ� ��밡���ϰ� ���ش�.<�ӽ�>
	SHGlobal_GetD3DDevice->SetLight(0, &dirLight);
	SHGlobal_GetD3DDevice->LightEnable(0, true);

	//���÷�������Ʈ ����
	SHGlobal_GetD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	SHGlobal_GetD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	SHGlobal_GetD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	SHGlobal_GetD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID); //fill��弳��
	SHGlobal_GetD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW  );  //�ø�弳��
	
	CString TempDlgName  = "UI_Play";// UI_Play��� �̸��� ���� ���̾�α� [2014.11.5 ]
	m_stlVec_DialogNames.push_back(TempDlgName);

	

}



//-----------------------------------------------------------------------------
// �Ҹ���
//-----------------------------------------------------------------------------
CScene_MainGame::~CScene_MainGame()
{

}

//-----------------------------------------------------------------------------
// Arg    : 
// Return :
// Desc   :  �ؽ�Ʈ ȣ���ϴ� �Լ�  [2014.12.1 ]
//-----------------------------------------------------------------------------
void	CScene_MainGame::RenderText( float DTime )
{
	// The helper object simply helps keep track of text position, and color
	// and then it calls pFont->DrawText( m_pSprite, strMsg, -1, &rc, DT_NOCLIP, m_clr );
	// If NULL is passed in as the sprite object, then it will work fine however the 
	// pFont->DrawText() will not be batched together.  Batching calls will improves perf.


	// Output statistics
	m_txtHelper.Begin();

	m_txtHelper.SetInsertionPos( 5, 0 );
	m_txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	m_txtHelper.DrawFormattedTextLine(L"FPS : %.2f" , SHUtil::getFPS(DTime)); // Show FPS
	m_txtHelper.SetInsertionPos( 160, 0 );
	m_txtHelper.DrawFormattedTextLine(L"HP : %d" ,(int)G_GirlHP); // Show HP
	m_txtHelper.SetInsertionPos( 250, 0 );
	m_txtHelper.DrawFormattedTextLine(L"�����ð� : %d" ,(int)(180 - G_GameTimer)); // Show HP
	//m_txtHelper.DrawFormattedTextLine(L"Count of Object In Frustum : %d" , G_Frustum.Get_InnerObjCnt());
	//m_txtHelper.DrawFormattedTextLine(L"%s",G_SelectedObjName);

	m_txtHelper.End();
}

//-----------------------------------------------------------------------------
// Arg    : float DTime 
// Return :
// Desc   : ������� ��Ÿ�� UI�� �׷��ش�. // [2014.12.1 ]
//-----------------------------------------------------------------------------
void	CScene_MainGame::RenderUI( float DTime )
{
	 g_MainPlayUI.OnRender( DTime ) ;
}

//-----------------------------------------------------------------------------
// �����ִ� ��� ������Ʈ�� �ʱ�ȭ, ���ҽ��� ������Ʈ ����
//-----------------------------------------------------------------------------
int	CScene_MainGame::InitObjects() 
{
	HRESULT hr = S_OK;	

	m_txtHelper.Init(SHGlobal_pFontSystem, SHGlobal_pSprite_BDT, 16 );
	/////////////////////////////////////////////////////////////////////////
	// 1)Creator ���� CEntityFactory�� ����Ѵ�. 

	// CBearCreator �� CEntityFactory �� ���
	CEntityFactory::GetInstance()->Register(CBear::GetCreatorName(), new CBearCreator());
	// CRoomCreator �� CEntityFactory �� ���
	CEntityFactory::GetInstance()->Register(CRoom::GetCreatorName(), new CRoom_Creator());  //[2014.11.12 ]
	// CGhost�� CEntityFactory �� ���
	CEntityFactory::GetInstance()->Register(CGhost::GetCreatorName(), new CGhost_Creator());  //[2014.11.15 ]
	// CCardSkill_Creator �� CEntityFactory �� ��� // [2014.11.27 ]
	CEntityFactory::GetInstance()->Register(CCardSkill::GetCreatorName(), new CCardSkill_Creator()); // [2014.11.27 ]
	// CHatSkill_Creator �� CEntityFactory �� ��� // [2014.11.27 ]
	CEntityFactory::GetInstance()->Register(CHatSkill::GetCreatorName(), new CHatSkill_Creator()); // [2014.11.28 ]
	// CRockPunchSkill_Creator �� CEntityFactory �� ��� // [2014.11.29 ]
	CEntityFactory::GetInstance()->Register(CRockPunchSkill::GetCreatorName(), new CRockPunchSkill_Creator()); // [2014.11.29 ]

	//////////////////////////////////////////////////////////////////////////
	// 2) �̾����� ���� ��縮�ҽ��� �ҷ��´�. 
	LoadRsc_FromFile("Scene_MainGame.SI");

	//////////////////////////////////////////////////////////////////////////
	// 3) �����ͺ��̽��� �ʿ��� ���� �̸� �ε��ϱ�

	//(���ɵ���Ÿ���̽� �а� �����س���) //[2014.11.16 ]
	G_GhostDataBase.Load_From_Lua("./Script/Ghost_Attr.lua", 
								  "./Script/Ghost_ClipInfo.lua",
								  "./Script/Respawn_Point_Info.lua");
	//////////////////////////////////////////////////////////////////////////
	// Create RockPunchSkill  // [2014.11.29 ]
	{
		EntityFactory_Arg EFArg;           //createEntity�� �Ѱ��� ����ü
		EFArg.Pos = D3DXVECTOR3(0, 4, 0);
		strcpy_s(EFArg.AttrInfoPath , "./Script/Skill_Attr.lua");
		strcpy_s(EFArg.KeyInfoPath , "./Script/Punch_ClipInfo.lua");
		CGameObject *pObj = CEntityFactory::GetInstance()->CreateEntity(CRockPunchSkill::GetCreatorName(),  L"RocketPunch.rpi",  EFArg);
		G_pRockPunch = dynamic_cast<CRockPunchSkill*>(pObj); // [2014.11.29 ]
		//AddObject(pObj); 
	}
	//////////////////////////////////////////////////////////////////////////
	// Create HatSkill  // [2014.11.27 ]
	{
		EntityFactory_Arg EFArg;           //createEntity�� �Ѱ��� ����ü
		EFArg.Pos = D3DXVECTOR3(0, 0, 0);
		strcpy_s(EFArg.AttrInfoPath , "./Script/Skill_Attr.lua");
		strcpy_s(EFArg.KeyInfoPath , "./Script/Hat_ClipInfo.lua");
		CGameObject *pObj = CEntityFactory::GetInstance()->CreateEntity(CHatSkill::GetCreatorName(),  L"Hat.rpi",  EFArg);
		G_pHat = dynamic_cast<CHatSkill*>(pObj); // [2014.11.27 ]
		//AddObject(pObj); 
	}

	//////////////////////////////////////////////////////////////////////////
	// Create CardSkill  // [2014.11.27 ]
	{	
		
		EntityFactory_Arg EFArg;           //createEntity�� �Ѱ��� ����ü
		EFArg.Pos = D3DXVECTOR3(0, 4, 0);
		strcpy_s(EFArg.AttrInfoPath , "./Script/Skill_Attr.lua");
		CGameObject *pObj = CEntityFactory::GetInstance()->CreateEntity(CCardSkill::GetCreatorName(),  L"Card.rpi",  EFArg);
		G_pCard = dynamic_cast<CCardSkill*>(pObj); // [2014.11.27 ]

		G_pScatter = new CScatterSkill();
		G_pScatter->Load_AttrInfo_from_Lua("./Script/Skill_Attr.lua");
		G_pScatter->Init(SHGlobal_GetD3DDevice);
		//AddObject(pObj); 
	}

	//////////////////////////////////////////////////////////////////////////
	// Create Room  [2014.11.12 ]
	{	
		EntityFactory_Arg EFArg;           //createEntity�� �Ѱ��� ����ü
		EFArg.Pos = D3DXVECTOR3(0, 0, 0);
		CGameObject *pObj = CEntityFactory::GetInstance()->CreateEntity(CRoom::GetCreatorName(),  L"Room.rpi",  EFArg);

		AddObject(pObj); 
	}

	//////////////////////////////////////////////////////////////////////////
	// create Ghost  // [2014.11.14 ]
	EntityFactory_Arg EFArg_Ghost; 
	EFArg_Ghost.Pos = D3DXVECTOR3(20, 0, 10);
	EFArg_Ghost.starttick =1.0f;
	strcpy_s(EFArg_Ghost.KeyInfoPath , "./Script/Ghost_ClipInfo.lua");
	CGameObject *pTempGhost = CEntityFactory::GetInstance()->CreateEntity(CGhost::GetCreatorName(), L"Ghost.rpi",EFArg_Ghost);
	G_pGhost_Origne =  static_cast<CGhost*>(pTempGhost);// �ӽù��� [2014.12.7 ]
	for(int i = 0 ; i< GHOSTNUM_MAX ; i++)
	{
 		// max num ��ŭ ��Ʈ ����
		CGhost* pG =  static_cast<CGhost*>(pTempGhost);
		G_pGhost_Arr[i] = new CGhost();
		(*G_pGhost_Arr[i]) = (*pG);
		AddObject(G_pGhost_Arr[i]);  // [2014.11.17 ]	

		//////////////////////////////////////////////////////////////////////////
		// �浹�� ���
		SHR_BoneList* pBL = G_pGhost_Arr[i]->GetRscPack()->GetBoneList();
		const SHR_Mesh* pTempMesh =   pBL->FindMeshByName("BoundSphere_Body"); // �ٿ�����Ǿ� �ٵ�����  [2014.11.25 ]
		G_pGhost_Arr[i]->m_pBSMesh_Ghost[0] = pTempMesh;

		pBL = G_pGhost_Arr[i]->m_RscPack2.GetBoneList();
		pTempMesh =   pBL->FindMeshByName("BoundSphere_Body"); // �ٿ�����Ǿ� �ٵ�����  [2014.11.25 ]
		G_pGhost_Arr[i]->m_pBSMesh_Ghost[1] = pTempMesh;

		pBL = G_pGhost_Arr[i]->m_RscPack3.GetBoneList();
		pTempMesh =   pBL->FindMeshByName("BoundSphere_Body"); // �ٿ�����Ǿ� �ٵ�����  [2014.11.25 ]
		G_pGhost_Arr[i]->m_pBSMesh_Ghost[2] = pTempMesh;

		G_pGhost_Arr[i]->m_pBoundSphere_Body = new CBoundSphere();
		G_pGhost_Arr[i]->m_pBoundSphere_Body->SetRadius( pTempMesh->m_MeshInfo.BoundShpere_Radius+9);

		//CCollideMgr::getInstance()->m_Collider_List[eCollide_Tag_Ghost].push_back(G_pGhost_Arr[i]->m_pBoundSphere_Body);

		G_pGhost_Arr[i]->GetRscPack()->GetAnimator().Change_Ani_Clip("IDLE");
	}
	

	//////////////////////////////////////////////////////////////////////////
	// Create Teady Bear  // [2014.11.14 ]
	{
		EntityFactory_Arg EFArg3;           //createEntity �Լ��� �Ѱ��� ����ü
		EFArg3.Pos = D3DXVECTOR3(0, 0, 0);
		EFArg3.starttick =0.0f;
		strcpy_s(EFArg3.KeyInfoPath , "./Script/TedyBear_ClipInfo.lua");
		strcpy_s(EFArg3.AttrInfoPath , "./Script/TedyBear_Attr.lua");


		CGameObject *pObj3 = CEntityFactory::GetInstance()->CreateEntity(CBear::GetCreatorName(), L"TedyBear.rpi", EFArg3);
		AddObject(pObj3);

		G_pTedyBear = static_cast<CBear*>(pObj3);  // [2014.11.13 ]
	}

	//������Ʈ ����
	CCamera* TempCam =  new CCamera();
	TempCam->Load_CameraInfo_from_Lua("./Script/Camera_Info.lua"); // [2014.11.14 ]
	TempCam->Init(SHGlobal_GetD3DDevice);
	

	// 5) ���� ī�޶� ������ ���
	hr = _SetCamera(TempCam);
	
	//Scene�� ������Ʈ ���
	AddObject(TempCam);

	
	assert(m_pCamera != nullptr);
	
	return hr;
}

//-----------------------------------------------------------------------------
// �� ���� �Լ� ���� ���Լ�
//-----------------------------------------------------------------------------
void CScene_MainGame::OnRenderStart()
{
	SetCursor( NULL );	// ���콺�� ��Ÿ���� �ʰ� �ʴ�.
	//////////////////////////////////////////////////////////////////////////
	// ���ؽ� ���̴� ��� ����                                [2014.7.28 ]
// 	D3DXVECTOR4	tempVector(0.0f, 1.0f, -1.0f, 3.0f);   
// 	SHGlobal_GetD3DDevice->SetVertexShaderConstantF( 0, (float*) tempVector, 1); [2014.10.30 ]
// 
// 	// Load the combined model- G_LightDiff in registers c12
// 	SHGlobal_GetD3DDevice->SetVertexShaderConstantF( 12, (float*)G_LightDiff, 1 );
// 	SHGlobal_GetD3DDevice->SetPixelShaderConstantF( 12, (float*)G_LightDiff, 1 );
// 	// Load the combined model- G_vecDirLight in registers c13
// 	SHGlobal_GetD3DDevice->SetVertexShaderConstantF( 13, (float*)(G_vecDirLight*-1.0f), 1 );
// 	SHGlobal_GetD3DDevice->SetPixelShaderConstantF( 13, (float*)(G_vecDirLight*-1.0f), 1 );
// 	// Load the combined model- G_LightAmbi in registers c14
// 	SHGlobal_GetD3DDevice->SetVertexShaderConstantF( 14, (float*)G_LightAmbi, 1 );
// 	SHGlobal_GetD3DDevice->SetPixelShaderConstantF( 14, (float*)G_LightAmbi, 1 );

	SHGlobal_GetD3DDevice->SetRenderState(D3DRS_SPECULARENABLE,  TRUE); //����ŧ�� ���̶���Ʈ ����Ұ��ΰ�?
	SHGlobal_GetD3DDevice->SetRenderState(D3DRS_LIGHTING , true);    //����������Ʈ�� ����Ʈ������ ���ش�.[2014.7.28 ]
}

//-----------------------------------------------------------------------------
// �� ���� �Լ� ���� ���Լ�
//-----------------------------------------------------------------------------
void CScene_MainGame::OnRenderEnd()
{

}

//-----------------------------------------------------------------------------
// Arg    : 
// Return : 
// Desc   :  CScene�� OnUpdateStart() �Լ��� ������,   CScene����ϵ� ������Ʈ���� 
//			 ������ ���鼭 update() �� ȣ��Ǳ� ������ ���Լ��� ȣ��ȴ�.
//-----------------------------------------------------------------------------
void	CScene_MainGame::OnUpdateStart(float _dTime)
{
	SetCursor( NULL );	// ���콺�� ��Ÿ���� �ʰ� �ʴ�.
	int SliderVal;
	// ����Ÿ�̸� �ð� ������Ʈ
	G_GameTimer +=_dTime;

	if((G_GameTimer > 180) && (G_GirlHP > 0))
	{
		CSceneMgr::getInstance()->OpenScene(L"Scene_Success"); //[2014.12.5 ]
		G_CurSceneType = eSceneType_Clear;
		G_SHSoundMgr.StopWave(eSound_BGM_Play);
		G_SHSoundMgr.PlayWave(eSound_BGM_Clear, true); //[2014.12.7 ]
	}

	if(G_GirlHP  < 0.0001f )
	{	
		CSceneMgr::getInstance()->OpenScene(L"Scene_Fail"); //[2014.12.5 ]
		G_CurSceneType = eSceneType_Fail;
		G_SHSoundMgr.StopWave(eSound_BGM_Play);
		G_SHSoundMgr.PlayWave(eSound_BGM_Fail, true); //[2014.12.7 ]
	}

	SliderVal = (G_GameTimer / 180.0f)*100.0f;
	g_MainPlayUI.GetSlider(eUI_ID_TimerSlider)->SetValue(SliderVal);
	//////////////////////////////////////////////////////////////////////////
	// spawn point 1 
	CGhost::InitGhost_in_RealTime(G_GameTimer , _dTime);
	

}

//-----------------------------------------------------------------------------
// Arg    : 
// Return :
// Desc   : 
//-----------------------------------------------------------------------------
void		CScene_MainGame::OnUpdateEnd(float _dTime)
{
	
}

//-----------------------------------------------------------------------------
// Arg    : SH_MSG _Message  
// Return : 
// Desc   :
//-----------------------------------------------------------------------------
DWORD	CScene_MainGame::Notify(SH_MSG* _pMessage )
{
	HRESULT hr = S_OK;
	
	switch(_pMessage->GetMsgType())
	{
	case eMsgType_NORMAL:
		{

		}
		break;
	case eMsgType_UI:
		{
			SH_MSG_from_UI* pMsg = static_cast<SH_MSG_from_UI*>(_pMessage);
			std::list<IMessagable*>::iterator iter, iterEnd;
			iterEnd = m_list_MessagableObjs.end();   // [2014.11.6 ]

			for(iter = m_list_MessagableObjs.begin(); iter != iterEnd ; ++iter )// [2014.11.6 ]
			{
				(*iter)->MsgProc(_pMessage);

			} 
			
		}
		break;

	}





	

	return hr;
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
// ���⼭���� class CScene_Title �Լ����� /=
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



//-----------------------------------------------------------------------------
// ������ 
//-----------------------------------------------------------------------------
CScene_Title::CScene_Title()
{
	G_SpriteTitle.Init(SHGlobal_GetD3DDevice , L"./image/title.png");// [2014.12.2 ]
			
	SHGlobal_GetD3DDevice->LightEnable(0, true);

	//���÷�������Ʈ ����
	SHGlobal_GetD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	SHGlobal_GetD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	SHGlobal_GetD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	SHGlobal_GetD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID); //fill��弳��
	SHGlobal_GetD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW  );  //�ø�弳��

// 	CString TempDlgName  = "UI_Play";// UI_Play��� �̸��� ���� ���̾�α� [2014.11.5 ]
// 	m_stlVec_DialogNames.push_back(TempDlgName);

}



//-----------------------------------------------------------------------------
// �Ҹ���
//-----------------------------------------------------------------------------
CScene_Title::~CScene_Title()
{
}

//-----------------------------------------------------------------------------
// Arg    : 
// Return :
// Desc   :  �ؽ�Ʈ ȣ���ϴ� �Լ�  [2014.12.1 ]
//-----------------------------------------------------------------------------
void	CScene_Title::RenderText( float DTime )
{
	// The helper object simply helps keep track of text position, and color
	// and then it calls pFont->DrawText( m_pSprite, strMsg, -1, &rc, DT_NOCLIP, m_clr );
	// If NULL is passed in as the sprite object, then it will work fine however the 
	// pFont->DrawText() will not be batched together.  Batching calls will improves perf.
	
	// Output statistics
	m_txtHelper.Begin();

	m_txtHelper.SetInsertionPos( 5, 0 );
	m_txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	m_txtHelper.DrawFormattedTextLine(L"FPS : %.2f" , SHUtil::getFPS(DTime)); // Show FPS
	m_txtHelper.SetInsertionPos( 160, 0 );
	m_txtHelper.DrawFormattedTextLine(L"HP : %d" ,(int)G_GirlHP); // Show HP
	//m_txtHelper.DrawFormattedTextLine(L"Count of Object In Frustum : %d" , G_Frustum.Get_InnerObjCnt());
	//m_txtHelper.DrawFormattedTextLine(L"%s",G_SelectedObjName);

	m_txtHelper.End();
}

//-----------------------------------------------------------------------------
// Arg    : float DTime 
// Return :
// Desc   : ������� ��Ÿ�� UI�� �׷��ش�. // [2014.12.1 ]
//-----------------------------------------------------------------------------
void	CScene_Title::RenderUI( float DTime )
{
	G_SpriteTitle.Draw();
	g_MainTitle_UI.OnRender( DTime ) ;
}

//-----------------------------------------------------------------------------
// �����ִ� ��� ������Ʈ�� �ʱ�ȭ, ���ҽ��� ������Ʈ ����
//-----------------------------------------------------------------------------
int	CScene_Title::InitObjects() 
{
	HRESULT hr = S_OK;	

	m_txtHelper.Init(SHGlobal_pFontSystem, SHGlobal_pSprite_BDT, 16 );
	

	//������Ʈ ����
	CCamera* TempCam =  new CCamera();
	TempCam->Load_CameraInfo_from_Lua("./Script/Camera_Info.lua"); // [2014.11.14 ]
	TempCam->Init(SHGlobal_GetD3DDevice);


	// 5) ���� ī�޶� ������ ���
	hr = _SetCamera(TempCam);

	//Scene�� ������Ʈ ���
	AddObject(TempCam);


	assert(m_pCamera != nullptr);

	return hr;
}

//-----------------------------------------------------------------------------
// �� ���� �Լ� ���� ���Լ�
//-----------------------------------------------------------------------------
void CScene_Title::OnRenderStart()
{}

//-----------------------------------------------------------------------------
// �� ���� �Լ� ���� ���Լ�
//-----------------------------------------------------------------------------
void CScene_Title::OnRenderEnd()
{}

//-----------------------------------------------------------------------------
// Arg    : 
// Return : 
// Desc   :  CScene�� OnUpdateStart() �Լ��� ������,   CScene����ϵ� ������Ʈ���� 
//			 ������ ���鼭 update() �� ȣ��Ǳ� ������ ���Լ��� ȣ��ȴ�.
//-----------------------------------------------------------------------------
void	CScene_Title::OnUpdateStart(float _dTime)
{
}

//-----------------------------------------------------------------------------
// Arg    : 
// Return :
// Desc   : 
//-----------------------------------------------------------------------------
void		CScene_Title::OnUpdateEnd(float _dTime)
{}

//-----------------------------------------------------------------------------
// Arg    : SH_MSG _Message  
// Return : 
// Desc   :
//-----------------------------------------------------------------------------
DWORD	CScene_Title::Notify(SH_MSG* _pMessage )
{
	HRESULT hr = S_OK;

	switch(_pMessage->GetMsgType())
	{
	case eMsgType_NORMAL:
		{

		}
		break;
	case eMsgType_UI:
		{
			SH_MSG_from_UI* pMsg = static_cast<SH_MSG_from_UI*>(_pMessage);
			std::list<IMessagable*>::iterator iter, iterEnd;
			iterEnd = m_list_MessagableObjs.end();   // [2014.11.6 ]

			for(iter = m_list_MessagableObjs.begin(); iter != iterEnd ; ++iter )// [2014.11.6 ]
			{
				(*iter)->MsgProc(_pMessage);

			} 

		}
		break;

	}

	return hr;
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
// ���⼭���� class CScene_Openning �Լ����� [2014.12.3 ]
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// ������  //[2014.12.3 ]
//-----------------------------------------------------------------------------
CScene_Openning::CScene_Openning()
{
	m_LoadImage.Init(SHGlobal_GetD3DDevice , L"./image/Loading.png");// [2014.12.2 ]

	SHGlobal_GetD3DDevice->LightEnable(0, true);

	//���÷�������Ʈ ����
	SHGlobal_GetD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	SHGlobal_GetD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	SHGlobal_GetD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	SHGlobal_GetD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID); //fill��弳��
	SHGlobal_GetD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW  );  //�ø�弳��

	// 	CString TempDlgName  = "UI_Play";// UI_Play��� �̸��� ���� ���̾�α� [2014.11.5 ]
	// 	m_stlVec_DialogNames.push_back(TempDlgName);

}



//-----------------------------------------------------------------------------
// �Ҹ���
//-----------------------------------------------------------------------------
CScene_Openning::~CScene_Openning()
{
}

//-----------------------------------------------------------------------------
// Arg    : 
// Return :
// Desc   :  �ؽ�Ʈ ȣ���ϴ� �Լ�  [2014.12.1 ]
//-----------------------------------------------------------------------------
void	CScene_Openning::RenderText( float DTime )
{
	// The helper object simply helps keep track of text position, and color
	// and then it calls pFont->DrawText( m_pSprite, strMsg, -1, &rc, DT_NOCLIP, m_clr );
	// If NULL is passed in as the sprite object, then it will work fine however the 
	// pFont->DrawText() will not be batched together.  Batching calls will improves perf.

	// Output statistics
// 	m_txtHelper.Begin();
// 
// 	m_txtHelper.SetInsertionPos( 5, 0 );
// 	m_txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
// 	m_txtHelper.DrawFormattedTextLine(L"FPS : %.2f" , SHUtil::getFPS(DTime)); // Show FPS
// 	m_txtHelper.SetInsertionPos( 160, 0 );
// 	m_txtHelper.DrawFormattedTextLine(L"HP : %d" ,(int)G_GirlHP); // Show HP
	//m_txtHelper.DrawFormattedTextLine(L"Count of Object In Frustum : %d" , G_Frustum.Get_InnerObjCnt());
	//m_txtHelper.DrawFormattedTextLine(L"%s",G_SelectedObjName);

/*	m_txtHelper.End();*/
}

//-----------------------------------------------------------------------------
// Arg    : float DTime 
// Return :
// Desc   : ������� ��Ÿ�� UI�� �׷��ش�. // [2014.12.1 ]
//-----------------------------------------------------------------------------
void	CScene_Openning::RenderUI( float DTime )
{
	m_LoadImage.Draw();
	//g_MainTitle_UI.OnRender( DTime ) ;
}

//-----------------------------------------------------------------------------
// �����ִ� ��� ������Ʈ�� �ʱ�ȭ, ���ҽ��� ������Ʈ ����
//-----------------------------------------------------------------------------
int	CScene_Openning::InitObjects() 
{
	HRESULT hr = S_OK;	

//	m_txtHelper.Init(SHGlobal_pFontSystem, SHGlobal_pSprite_BDT, 16 );
	
	//������Ʈ ����
	CCamera* TempCam =  new CCamera();
	TempCam->Load_CameraInfo_from_Lua("./Script/Camera_Info.lua"); // [2014.11.14 ]
	TempCam->Init(SHGlobal_GetD3DDevice);


	// 5) ���� ī�޶� ������ ���
	hr = _SetCamera(TempCam);

	//Scene�� ������Ʈ ���
	AddObject(TempCam);


	assert(m_pCamera != nullptr);

	return hr;
}

//-----------------------------------------------------------------------------
// �� ���� �Լ� ���� ���Լ�
//-----------------------------------------------------------------------------
void CScene_Openning::OnRenderStart()
{}

//-----------------------------------------------------------------------------
// �� ���� �Լ� ���� ���Լ�
//-----------------------------------------------------------------------------
void CScene_Openning::OnRenderEnd()
{}

//-----------------------------------------------------------------------------
// Arg    : 
// Return : 
// Desc   :  CScene�� OnUpdateStart() �Լ��� ������,   CScene����ϵ� ������Ʈ���� 
//			 ������ ���鼭 update() �� ȣ��Ǳ� ������ ���Լ��� ȣ��ȴ�.
//-----------------------------------------------------------------------------
void	CScene_Openning::OnUpdateStart(float _dTime)
{
	if((SHG_Input.key(DIK_RETURN) == true)  || (SHG_Input.MouseDown(0) == true))
	{
		
		 CSceneMgr::getInstance()->OpenScene(L"Scene_GameMain");    G_CurSceneType = eSceneType_MainGame;
		 G_SHSoundMgr.StopWave(eSound_BGM_Intro);
		 G_SHSoundMgr.PlayWave(eSound_BGM_Play, true);
		 G_GameTimer =0.0f;
		 G_GirlHP = 100.0f; // ������ ü�� 
	}
}

//-----------------------------------------------------------------------------
// Arg    : 
// Return :
// Desc   : 
//-----------------------------------------------------------------------------
void		CScene_Openning::OnUpdateEnd(float _dTime)
{}

//-----------------------------------------------------------------------------
// Arg    : SH_MSG _Message  
// Return : 
// Desc   :
//-----------------------------------------------------------------------------
DWORD	CScene_Openning::Notify(SH_MSG* _pMessage )
{
	HRESULT hr = S_OK;

	switch(_pMessage->GetMsgType())
	{
	case eMsgType_NORMAL:
		{

		}
		break;
	case eMsgType_UI:
		{
// 			SH_MSG_from_UI* pMsg = static_cast<SH_MSG_from_UI*>(_pMessage);
// 			std::list<IMessagable*>::iterator iter, iterEnd;
// 			iterEnd = m_list_MessagableObjs.end();   // [2014.11.6 ]
// 
// 			for(iter = m_list_MessagableObjs.begin(); iter != iterEnd ; ++iter )// [2014.11.6 ]
// 			{
// 				(*iter)->MsgProc(_pMessage);
// 
// 			} 

		}
		break;

	}

	return hr;
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
// ���⼭���� class CScene_Fail �Լ����� [2014.12.3 ]
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// ������  //[2014.12.3 ]
//-----------------------------------------------------------------------------
CScene_Fail::CScene_Fail()
{
	m_LoadImage.Init(SHGlobal_GetD3DDevice , L"./image/fail.png");// [2014.12.2 ]

	SHGlobal_GetD3DDevice->LightEnable(0, true);

	//���÷�������Ʈ ����
	SHGlobal_GetD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	SHGlobal_GetD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	SHGlobal_GetD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	SHGlobal_GetD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID); //fill��弳��
	SHGlobal_GetD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW  );  //�ø�弳��

	// 	CString TempDlgName  = "UI_Play";// UI_Play��� �̸��� ���� ���̾�α� [2014.11.5 ]
	// 	m_stlVec_DialogNames.push_back(TempDlgName);

}



//-----------------------------------------------------------------------------
// �Ҹ���
//-----------------------------------------------------------------------------
CScene_Fail::~CScene_Fail()
{
}

//-----------------------------------------------------------------------------
// Arg    : 
// Return :
// Desc   :  �ؽ�Ʈ ȣ���ϴ� �Լ�  [2014.12.1 ]
//-----------------------------------------------------------------------------
void	CScene_Fail::RenderText( float DTime )
{
	// The helper object simply helps keep track of text position, and color
	// and then it calls pFont->DrawText( m_pSprite, strMsg, -1, &rc, DT_NOCLIP, m_clr );
	// If NULL is passed in as the sprite object, then it will work fine however the 
	// pFont->DrawText() will not be batched together.  Batching calls will improves perf.

	// Output statistics
	// 	m_txtHelper.Begin();
	// 
	// 	m_txtHelper.SetInsertionPos( 5, 0 );
	// 	m_txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	// 	m_txtHelper.DrawFormattedTextLine(L"FPS : %.2f" , SHUtil::getFPS(DTime)); // Show FPS
	// 	m_txtHelper.SetInsertionPos( 160, 0 );
	// 	m_txtHelper.DrawFormattedTextLine(L"HP : %d" ,(int)G_GirlHP); // Show HP
	//m_txtHelper.DrawFormattedTextLine(L"Count of Object In Frustum : %d" , G_Frustum.Get_InnerObjCnt());
	//m_txtHelper.DrawFormattedTextLine(L"%s",G_SelectedObjName);

	/*	m_txtHelper.End();*/
}

//-----------------------------------------------------------------------------
// Arg    : float DTime 
// Return :
// Desc   : ������� ��Ÿ�� UI�� �׷��ش�. // [2014.12.1 ]
//-----------------------------------------------------------------------------
void	CScene_Fail::RenderUI( float DTime )
{
	m_LoadImage.Draw();
	//g_MainTitle_UI.OnRender( DTime ) ;
}

//-----------------------------------------------------------------------------
// �����ִ� ��� ������Ʈ�� �ʱ�ȭ, ���ҽ��� ������Ʈ ����
//-----------------------------------------------------------------------------
int	CScene_Fail::InitObjects() 
{
	HRESULT hr = S_OK;	

	//	m_txtHelper.Init(SHGlobal_pFontSystem, SHGlobal_pSprite_BDT, 16 );

	//������Ʈ ����
	CCamera* TempCam =  new CCamera();
	TempCam->Load_CameraInfo_from_Lua("./Script/Camera_Info.lua"); // [2014.11.14 ]
	TempCam->Init(SHGlobal_GetD3DDevice);


	// 5) ���� ī�޶� ������ ���
	hr = _SetCamera(TempCam);

	//Scene�� ������Ʈ ���
	AddObject(TempCam);


	assert(m_pCamera != nullptr);

	return hr;
}

//-----------------------------------------------------------------------------
// �� ���� �Լ� ���� ���Լ�
//-----------------------------------------------------------------------------
void CScene_Fail::OnRenderStart()
{}

//-----------------------------------------------------------------------------
// �� ���� �Լ� ���� ���Լ�
//-----------------------------------------------------------------------------
void CScene_Fail::OnRenderEnd()
{}

//-----------------------------------------------------------------------------
// Arg    : 
// Return : 
// Desc   :  CScene�� OnUpdateStart() �Լ��� ������,   CScene����ϵ� ������Ʈ���� 
//			 ������ ���鼭 update() �� ȣ��Ǳ� ������ ���Լ��� ȣ��ȴ�.
//-----------------------------------------------------------------------------
void	CScene_Fail::OnUpdateStart(float _dTime)
{
	if((SHG_Input.key(DIK_RETURN) == true) || (SHG_Input.MouseDown(0) == true))
	{
		CSceneMgr::getInstance()->OpenScene(L"Scene_MainTitle");
		G_CurSceneType = eSceneType_MainTitle;
		G_ButtonAble.SetTimer(1.0f);
		G_SHSoundMgr.StopWave(eSound_BGM_Fail);
		G_SHSoundMgr.PlayWave(eSound_BGM_Title, true);
	}
}

//-----------------------------------------------------------------------------
// Arg    : 
// Return :
// Desc   : 
//-----------------------------------------------------------------------------
void		CScene_Fail::OnUpdateEnd(float _dTime)
{}

//-----------------------------------------------------------------------------
// Arg    : SH_MSG _Message  
// Return : 
// Desc   :
//-----------------------------------------------------------------------------
DWORD	CScene_Fail::Notify(SH_MSG* _pMessage )
{
	HRESULT hr = S_OK;

	switch(_pMessage->GetMsgType())
	{
	case eMsgType_NORMAL:
		{

		}
		break;
	case eMsgType_UI:
		{
			// 			SH_MSG_from_UI* pMsg = static_cast<SH_MSG_from_UI*>(_pMessage);
			// 			std::list<IMessagable*>::iterator iter, iterEnd;
			// 			iterEnd = m_list_MessagableObjs.end();   // [2014.11.6 ]
			// 
			// 			for(iter = m_list_MessagableObjs.begin(); iter != iterEnd ; ++iter )// [2014.11.6 ]
			// 			{
			// 				(*iter)->MsgProc(_pMessage);
			// 
			// 			} 

		}
		break;

	}

	return hr;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
// ���⼭���� class CScene_Success �Լ����� [2014.12.3 ]
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// ������  //[2014.12.3 ]
//-----------------------------------------------------------------------------
CScene_Success::CScene_Success()
{
	m_LoadImage.Init(SHGlobal_GetD3DDevice , L"./image/success.png");// [2014.12.2 ]

	SHGlobal_GetD3DDevice->LightEnable(0, true);

	//���÷�������Ʈ ����
	SHGlobal_GetD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	SHGlobal_GetD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	SHGlobal_GetD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	SHGlobal_GetD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID); //fill��弳��
	SHGlobal_GetD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW  );  //�ø�弳��

	// 	CString TempDlgName  = "UI_Play";// UI_Play��� �̸��� ���� ���̾�α� [2014.11.5 ]
	// 	m_stlVec_DialogNames.push_back(TempDlgName);

}



//-----------------------------------------------------------------------------
// �Ҹ���
//-----------------------------------------------------------------------------
CScene_Success::~CScene_Success()
{
}

//-----------------------------------------------------------------------------
// Arg    : 
// Return :
// Desc   :  �ؽ�Ʈ ȣ���ϴ� �Լ�  [2014.12.1 ]
//-----------------------------------------------------------------------------
void	CScene_Success::RenderText( float DTime )
{
	// The helper object simply helps keep track of text position, and color
	// and then it calls pFont->DrawText( m_pSprite, strMsg, -1, &rc, DT_NOCLIP, m_clr );
	// If NULL is passed in as the sprite object, then it will work fine however the 
	// pFont->DrawText() will not be batched together.  Batching calls will improves perf.

	// Output statistics
	// 	m_txtHelper.Begin();
	// 
	// 	m_txtHelper.SetInsertionPos( 5, 0 );
	// 	m_txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	// 	m_txtHelper.DrawFormattedTextLine(L"FPS : %.2f" , SHUtil::getFPS(DTime)); // Show FPS
	// 	m_txtHelper.SetInsertionPos( 160, 0 );
	// 	m_txtHelper.DrawFormattedTextLine(L"HP : %d" ,(int)G_GirlHP); // Show HP
	//m_txtHelper.DrawFormattedTextLine(L"Count of Object In Frustum : %d" , G_Frustum.Get_InnerObjCnt());
	//m_txtHelper.DrawFormattedTextLine(L"%s",G_SelectedObjName);

	/*	m_txtHelper.End();*/
}

//-----------------------------------------------------------------------------
// Arg    : float DTime 
// Return :
// Desc   : ������� ��Ÿ�� UI�� �׷��ش�. // [2014.12.1 ]
//-----------------------------------------------------------------------------
void	CScene_Success::RenderUI( float DTime )
{
	m_LoadImage.Draw();
	//g_MainTitle_UI.OnRender( DTime ) ;
}

//-----------------------------------------------------------------------------
// �����ִ� ��� ������Ʈ�� �ʱ�ȭ, ���ҽ��� ������Ʈ ����
//-----------------------------------------------------------------------------
int	CScene_Success::InitObjects() 
{
	HRESULT hr = S_OK;	

	//	m_txtHelper.Init(SHGlobal_pFontSystem, SHGlobal_pSprite_BDT, 16 );

	//������Ʈ ����
	CCamera* TempCam =  new CCamera();
	TempCam->Load_CameraInfo_from_Lua("./Script/Camera_Info.lua"); // [2014.11.14 ]
	TempCam->Init(SHGlobal_GetD3DDevice);


	// 5) ���� ī�޶� ������ ���
	hr = _SetCamera(TempCam);

	//Scene�� ������Ʈ ���
	AddObject(TempCam);


	assert(m_pCamera != nullptr);

	return hr;
}

//-----------------------------------------------------------------------------
// �� ���� �Լ� ���� ���Լ�
//-----------------------------------------------------------------------------
void CScene_Success::OnRenderStart()
{}

//-----------------------------------------------------------------------------
// �� ���� �Լ� ���� ���Լ�
//-----------------------------------------------------------------------------
void CScene_Success::OnRenderEnd()
{}

//-----------------------------------------------------------------------------
// Arg    : 
// Return : 
// Desc   :  CScene�� OnUpdateStart() �Լ��� ������,   CScene����ϵ� ������Ʈ���� 
//			 ������ ���鼭 update() �� ȣ��Ǳ� ������ ���Լ��� ȣ��ȴ�.
//-----------------------------------------------------------------------------
void	CScene_Success::OnUpdateStart(float _dTime)
{
	if((SHG_Input.key(DIK_RETURN) == true ))
	{
		CSceneMgr::getInstance()->OpenScene(L"Scene_MainTitle");    G_CurSceneType = eSceneType_MainTitle;
		G_ButtonAble.SetTimer(1.0f);
		G_SHSoundMgr.StopWave(eSound_BGM_Clear);
		G_SHSoundMgr.PlayWave(eSound_BGM_Title, true);//[2014.12.7 ]

	}
}

//-----------------------------------------------------------------------------
// Arg    : 
// Return :
// Desc   : 
//-----------------------------------------------------------------------------
void		CScene_Success::OnUpdateEnd(float _dTime)
{}

//-----------------------------------------------------------------------------
// Arg    : SH_MSG _Message  
// Return : 
// Desc   :
//-----------------------------------------------------------------------------
DWORD	CScene_Success::Notify(SH_MSG* _pMessage )
{
	HRESULT hr = S_OK;

	switch(_pMessage->GetMsgType())
	{
	case eMsgType_NORMAL:
		{

		}
		break;
	case eMsgType_UI:
		{
			// 			SH_MSG_from_UI* pMsg = static_cast<SH_MSG_from_UI*>(_pMessage);
			// 			std::list<IMessagable*>::iterator iter, iterEnd;
			// 			iterEnd = m_list_MessagableObjs.end();   // [2014.11.6 ]
			// 
			// 			for(iter = m_list_MessagableObjs.begin(); iter != iterEnd ; ++iter )// [2014.11.6 ]
			// 			{
			// 				(*iter)->MsgProc(_pMessage);
			// 
			// 			} 

		}
		break;

	}

	return hr;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
// ���⼭���� class CScene_Credit �Լ����� [2014.12.3 ]
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// ������  //[2014.12.3 ]
//-----------------------------------------------------------------------------
CScene_Credit::CScene_Credit()
{
	m_LoadImage.Init(SHGlobal_GetD3DDevice , L"./image/credit.png");// [2014.12.2 ]

	//SHGlobal_GetD3DDevice->LightEnable(0, true);

	//���÷�������Ʈ ����
	SHGlobal_GetD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	SHGlobal_GetD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	SHGlobal_GetD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	SHGlobal_GetD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID); //fill��弳��
	SHGlobal_GetD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW  );  //�ø�弳��

	// 	CString TempDlgName  = "UI_Play";// UI_Play��� �̸��� ���� ���̾�α� [2014.11.5 ]
	// 	m_stlVec_DialogNames.push_back(TempDlgName);

}



//-----------------------------------------------------------------------------
// �Ҹ���
//-----------------------------------------------------------------------------
CScene_Credit::~CScene_Credit()
{
}

//-----------------------------------------------------------------------------
// Arg    : 
// Return :
// Desc   :  �ؽ�Ʈ ȣ���ϴ� �Լ�  [2014.12.1 ]
//-----------------------------------------------------------------------------
void	CScene_Credit::RenderText( float DTime )
{
	// The helper object simply helps keep track of text position, and color
	// and then it calls pFont->DrawText( m_pSprite, strMsg, -1, &rc, DT_NOCLIP, m_clr );
	// If NULL is passed in as the sprite object, then it will work fine however the 
	// pFont->DrawText() will not be batched together.  Batching calls will improves perf.

	// Output statistics
	// 	m_txtHelper.Begin();
	// 
	// 	m_txtHelper.SetInsertionPos( 5, 0 );
	// 	m_txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	// 	m_txtHelper.DrawFormattedTextLine(L"FPS : %.2f" , SHUtil::getFPS(DTime)); // Show FPS
	// 	m_txtHelper.SetInsertionPos( 160, 0 );
	// 	m_txtHelper.DrawFormattedTextLine(L"HP : %d" ,(int)G_GirlHP); // Show HP
	//m_txtHelper.DrawFormattedTextLine(L"Count of Object In Frustum : %d" , G_Frustum.Get_InnerObjCnt());
	//m_txtHelper.DrawFormattedTextLine(L"%s",G_SelectedObjName);

	/*	m_txtHelper.End();*/
}

//-----------------------------------------------------------------------------
// Arg    : float DTime 
// Return :
// Desc   : ������� ��Ÿ�� UI�� �׷��ش�. // [2014.12.1 ]
//-----------------------------------------------------------------------------
void	CScene_Credit::RenderUI( float DTime )
{
	m_LoadImage.Draw();
	//g_MainTitle_UI.OnRender( DTime ) ;
}

//-----------------------------------------------------------------------------
// �����ִ� ��� ������Ʈ�� �ʱ�ȭ, ���ҽ��� ������Ʈ ����
//-----------------------------------------------------------------------------
int	CScene_Credit::InitObjects() 
{
	HRESULT hr = S_OK;	

	//	m_txtHelper.Init(SHGlobal_pFontSystem, SHGlobal_pSprite_BDT, 16 );

	//������Ʈ ����
	CCamera* TempCam =  new CCamera();
	TempCam->Load_CameraInfo_from_Lua("./Script/Camera_Info.lua"); // [2014.11.14 ]
	TempCam->Init(SHGlobal_GetD3DDevice);


	// 5) ���� ī�޶� ������ ���
	hr = _SetCamera(TempCam);

	//Scene�� ������Ʈ ���
	AddObject(TempCam);


	assert(m_pCamera != nullptr);

	return hr;
}

//-----------------------------------------------------------------------------
// �� ���� �Լ� ���� ���Լ�
//-----------------------------------------------------------------------------
void CScene_Credit::OnRenderStart()
{}

//-----------------------------------------------------------------------------
// �� ���� �Լ� ���� ���Լ�
//-----------------------------------------------------------------------------
void CScene_Credit::OnRenderEnd()
{}

//-----------------------------------------------------------------------------
// Arg    : 
// Return : 
// Desc   :  CScene�� OnUpdateStart() �Լ��� ������,   CScene����ϵ� ������Ʈ���� 
//			 ������ ���鼭 update() �� ȣ��Ǳ� ������ ���Լ��� ȣ��ȴ�.
//-----------------------------------------------------------------------------
void	CScene_Credit::OnUpdateStart(float _dTime)
{
	if((SHG_Input.key(DIK_RETURN) == true )|| (SHG_Input.MouseDown(0) == true))
	{
		CSceneMgr::getInstance()->OpenScene(L"Scene_MainTitle");    G_CurSceneType = eSceneType_MainTitle;
		G_ButtonAble.SetTimer(1.0f);
		G_SHSoundMgr.StopWave(eSound_BGM_Credit);

		G_SHSoundMgr.PlayWave(eSound_BGM_Title , true);
	}
}

//-----------------------------------------------------------------------------
// Arg    : 
// Return :
// Desc   : 
//-----------------------------------------------------------------------------
void		CScene_Credit::OnUpdateEnd(float _dTime)
{}

//-----------------------------------------------------------------------------
// Arg    : SH_MSG _Message  
// Return : 
// Desc   :
//-----------------------------------------------------------------------------
DWORD	CScene_Credit::Notify(SH_MSG* _pMessage )
{
	HRESULT hr = S_OK;

	switch(_pMessage->GetMsgType())
	{
	case eMsgType_NORMAL:
		{

		}
		break;
	case eMsgType_UI:
		{
			// 			SH_MSG_from_UI* pMsg = static_cast<SH_MSG_from_UI*>(_pMessage);
			// 			std::list<IMessagable*>::iterator iter, iterEnd;
			// 			iterEnd = m_list_MessagableObjs.end();   // [2014.11.6 ]
			// 
			// 			for(iter = m_list_MessagableObjs.begin(); iter != iterEnd ; ++iter )// [2014.11.6 ]
			// 			{
			// 				(*iter)->MsgProc(_pMessage);
			// 
			// 			} 

		}
		break;

	}

	return hr;
}
/*************************************  end  ****************************************/