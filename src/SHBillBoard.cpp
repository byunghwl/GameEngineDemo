/************************************************************************/
/*							2014.07.19                            */
//							  빌보드 클래스
//
/************************************************************************/

#include "DXUT.h"

#include "SHBillBoard.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 여기서부터 빌보드 멤버함수
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------
//  생성자 소멸자
//---------------------------------------------------------------------------------------------
cSHBillBoard::cSHBillBoard() : m_Width(0), m_Height(0), m_pTex(nullptr), pVertices(nullptr),
	                           pVtxbuffer(nullptr)
{
	D3DXMatrixIdentity(&m_WorldTM);
	D3DXMatrixIdentity(&m_RotTM);
	D3DXMatrixIdentity(&m_ScaleTM);
	D3DXMatrixIdentity(&m_PosTM);
	D3DXMatrixIdentity(&m_ViewTM_Inv);

	m_Mtrl.Emissive = D3DXCOLOR(1.0f,1.0f , 1.0f , 1.0f);
	m_Pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_UVAni_X = 0.0f;
	m_bVisible = false;

}


cSHBillBoard::~cSHBillBoard()
{
	SAFE_RELEASE(m_pTex)
	SAFE_RELEASE(pVtxbuffer)
		
}


//---------------------------------------------------------------------------------------------
//  빌보드 초기화 함수
//---------------------------------------------------------------------------------------------

void cSHBillBoard::Init(LPDIRECT3DDEVICE9 _pDevice, TCHAR* _TexPath, float Width , float FrameCnt , D3DXVECTOR3 _pos /*= D3DXVECTOR3(0.0f,0.0f,0.0f)*/)//  [2014.12.3 ]
{
	if(_pDevice == nullptr)
	{	SH_TRACE()   return;}

	SetDevice(_pDevice);
	
	D3DXCreateTextureFromFile(m_pDevice, _TexPath, &m_pTex);
	if(m_pTex == nullptr)
	{
		SHUtil::MsgBox("cSHBillBoard::Init( ) , m_pTex create fail!!!!!!!!!!!!!!!!!!!!!!!!!!"); // [2014.12.3 ]
		ASSERT_IF_FAIL_MemAlloc(m_pTex)
	}


	Set_hTech("Rigid_Tex");

	UVal_interval = 1.0f/FrameCnt;
	m_Pos = _pos;
	FrameNum =0;
	// 빌보드 정점
	CUSTOM_VTX_XYZ_TEX1_ tempvtx[4] = 
	{ 
		{ -Width/2.0f ,  -Width/2.0f ,0,			 0,	 1.0f},
		{ -Width/2.0f,   Width/2.0f,  0,				0,	 0 },
		{  Width/2.0f,  -Width/2.0f,  0,			1.0f/FrameCnt, 1.0f/*/FrameCnt*/ },
		{  Width/2.0f,   Width/2.0f,  0,			 1.0f/FrameCnt, 0 }
	};

	//pIdxbuffer;
	//버텍스버퍼 생성
	if( FAILED( _pDevice->CreateVertexBuffer( 4 * sizeof(CUSTOM_VTX_XYZ_TEX1_), 0, D3DFVF_XYZ_TEX1, D3DPOOL_DEFAULT, &pVtxbuffer, NULL ) ) )
	{	write_log_file("버텍스 버퍼 생성 실패"); assert(false);	}
	//버텍스버퍼 락
	if( FAILED( pVtxbuffer->Lock( 0, sizeof( CUSTOM_VTX_XYZ_TEX1_ )*	4, ( void** )&pVertices, 0 ) ) )
	{	write_log_file("버텍스 버퍼 락 실패"); assert(false);}

	memcpy( pVertices, tempvtx, sizeof( CUSTOM_VTX_XYZ_TEX1_ )*4 );
	//언락
	pVtxbuffer->Unlock();

}


//---------------------------------------------------------------------------------------------
//  Update 함수
//---------------------------------------------------------------------------------------------
int		cSHBillBoard::Update(float _dTime, D3DXVECTOR3 _Pos)
{
	//D3DXMATRIX viewTM = SHGlobal_GetCurScene_Camera->get_ViewMat();
	//카메라를 향한 룩벡터를 구한다.
	m_Pos = _Pos;
	m_Look = SHGlobal_GetCurScene_Camera->GetLookVec();// - m_Pos; //2014.07.20
	m_Up = SHGlobal_GetCurScene_Camera->GetUpVec();// [2014.12.3 ]

	D3DXMATRIX CamTM = SHGlobal_GetCurScene_Camera->Get_ViewProjMat();
	G_FXShader.m_pShader->SetMatrix(G_ConstantTable.h_ViewProjTM , &CamTM); // [2014.11.14 ]
//	m_Look.y = 0.0f;
	D3DXVec3Normalize(&m_Look, &m_Look);
	//m_Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3Cross(&m_Right, &m_Look, &m_Up);

	D3DXVec3Normalize(&m_Right, &m_Right);

// 	float x = -D3DXVec3Dot(&m_Right, &m_Pos);
// 	float y = -D3DXVec3Dot(&m_Up, &m_Pos);
// 	float z = -D3DXVec3Dot(&m_Look, &m_Pos);

	m_WorldTM(0,0) = m_Right.x; m_WorldTM(0, 1) = m_Up.x; m_WorldTM(0, 2) = m_Look.x; m_WorldTM(0, 3) = 0.0f;
	m_WorldTM(1,0) = m_Right.y; m_WorldTM(1, 1) = m_Up.y; m_WorldTM(1, 2) = m_Look.y; m_WorldTM(1, 3) = 0.0f;
	m_WorldTM(2,0) = m_Right.z; m_WorldTM(2, 1) = m_Up.z; m_WorldTM(2, 2) = m_Look.z; m_WorldTM(2, 3) = 0.0f;
	m_WorldTM(3,0) = _Pos.x;        m_WorldTM(3, 1) = _Pos.y;     m_WorldTM(3, 2) = _Pos.z;       m_WorldTM(3, 3) = 1.0f; //2014.07.20
	
	FrameNum++;

	if(FrameNum%2 == 0) // 4프레임마다 UV값 증가
	{
		if(m_UVAni_X >1)
			m_UVAni_X = 0.0f;

		m_UVAni_X += UVal_interval;
	}

	
	
	if(m_UVAni_X > 0.99999f)
	{	m_UVAni_X = 0.0f; return 1;	} // [2014.12.8 ]

	// 


	return 0;
}


//---------------------------------------------------------------------------------------------
// Draw 함수
//---------------------------------------------------------------------------------------------
void		cSHBillBoard::DrawAlpha()
{
	//m_pd3dDevice->SetLight(0,nullptr);
	
	if(m_bVisible != true)
		return ;


	// 알파채널을 사용해서 투명텍스처 효과를 낸다
// 	
	m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
	m_pDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE/*D3DBLEND_SRCALPHA*/);
	m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
	m_pDevice->SetRenderState( D3DRS_ALPHAREF,    0x02 );
	m_pDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
		
// 	if( SUCCEEDED( G_ScreenQuad.g_pRenderTargetTexPass[0/*PassNum 일단 0으로..[2014.11.19 ]*/]->GetSurfaceLevel( 0, &(G_ScreenQuad.g_pSceneSurface[0]) ) ) )
// 	{//gpSceneRenderTarget은 서페이스를 만들어줄때 필요한 녀석 [2014.11.7 ]
// 		m_pd3dDevice->SetRenderTarget( 0, G_ScreenQuad.g_pSceneSurface[0] ); // 만든 서페이스를 렌더타겟으로 지정(복사) [2014.11.7 ]
// 	}
	//재질과 텍스쳐 적용


	G_FXShader.m_pShader->SetTechnique(m_hTech);// [2014.10.30 ]

	UINT MaxPass = 0;
	G_FXShader.m_pShader->Begin(&MaxPass, NULL);//테크닉 시작 [2014.10.29 ]

	for(UINT PassNum = 0; PassNum < MaxPass ; PassNum++ ) // 패스별로 그리기 [2014.11.21 ]
	{
		G_FXShader.GetPointer()->BeginPass(PassNum);// [2014.10.29 ]

			m_pDevice->SetMaterial(&m_Mtrl);
			m_pDevice->SetTexture(0 , m_pTex);
			//월드행렬 설정
			//m_pd3dDevice->SetTransform(D3DTS_WORLD,&m_WorldTM);

			G_FXShader.m_pShader->SetFloat(G_ConstantTable.h_UVAni_Val , m_UVAni_X); // [2014.11.14 ]
				G_FXShader.m_pShader->SetMatrix(G_ConstantTable.h_WorldTM , &m_WorldTM); // [2014.11.14 ]

			G_FXShader.GetPointer()->CommitChanges(); // [2014.10.30 ] 
		//m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, false);	// 제트버퍼 끄고그리기
			// 컬모드 바꾸기
			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			m_pDevice->SetFVF(D3DFVF_XYZ_TEX1 );	
			m_pDevice->SetStreamSource(0, pVtxbuffer, 0, sizeof(CUSTOM_VTX_XYZ_TEX1_));
			m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
		//m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, true);	// 제트버퍼 다시키기
		G_FXShader.GetPointer()->EndPass();// 패스별로 그리기 끝 [2014.10.29 ]
	}

	G_FXShader.GetPointer()->End();//테크닉 끝 [2014.10.29 ]

	//알파설정 끄기
  	m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   false );
  	m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, false );
	//텍스쳐 끄기
	m_pDevice->SetTexture(0 , NULL);
	// 컬모드 바꾸기
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	

}


//-----------------------------------------------------------------------------
// Arg    : 
// Return :
// Desc   : from IAlphaBlendable [2014.12.8 ] 
//          _CamPos : 카메라 위치
//----------------------------------------------------------------------------
float		cSHBillBoard::CalDistToCam(D3DXVECTOR3 _CamPos)
{
	D3DXVECTOR3 TempVec = _CamPos - m_Pos;
	float Len = D3DXVec3Length(&TempVec);
	m_DistToCamera = Len;
	return Len;

}


//-----------------------------------------------------------------------------
// Arg    : 
// Return :
// Desc   : 테크닉을 이름으로 찾아 
//-----------------------------------------------------------------------------
void		cSHBillBoard::Set_hTech(const char* _szStr) // [2014.12.3 ]
{
	if(_szStr == nullptr)
	{
		SHUtil::MsgBox("cSHBillBoard::Set_hTech , _szStr is Nullptr");
	}


	m_hTech = G_FXShader.GetPointer()->GetTechniqueByName(_szStr); // [2014.12.3 ]

	if(m_hTech == nullptr)
	{
		SHUtil::MsgBox("cSHBillBoard::Set_hTech , m_hTech is Nullptr"); // [2014.12.3 ]
	}
}
/*************************************************** end of this file **************************************************************/