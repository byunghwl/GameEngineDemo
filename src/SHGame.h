/**********************************************************************************************/
/* 게임 전체에서  전역객체에 접근할 수있게 전역변수들의 관리를 위한 헤더파일				  */
/*																							  */
/**********************************************************************************************/

#pragma  once

//////////////////////////////////////////////////////////////////////////
// 선행선언
class Camera;
class BoundingSphere;
class GEOMOBJECT;
class cSHBillBoard;
class cSmoke;
class AirCraft;
class cMissile;
class CBear;
class cSkyBox;
#define MAX_NUM_STRING 512


enum {
	// 키보드 처리를 위해 가상키 종류 
	eNumKey = 128, 
};

//-----------------------------------------------------------------------------
// 전역,, 외부 변수 선언
//-----------------------------------------------------------------------------
extern   ZFrustum						G_Frustum;  //프러스텀 컬링할때 사용할 객체 ,, 


		

//////////////////////////////////////////////////////////////////////////

extern   D3DMATERIAL9					G_DefaultMtrl; //기본 메테리얼 (white)
// 포인트라이트
extern D3DXVECTOR4						G_PointLightPosition;  // 포인트라이트의 위치 [2014.11.12 ]
extern float							G_PointLightRadius;    // 포인트라이트의 위치 [2014.11.12 ]
// 빛의 방향 (directional Light)
extern D3DXVECTOR3						G_vecDirLight;  
// 빛의 디퓨즈색
extern D3DXCOLOR						G_LightDiff;	
// 빛의 엠비언트
extern D3DXCOLOR						G_LightAmbi;	



//환경맵핑할때 사용할 스카이박스의 큐브맵을 전역변수로 놓는다.   [2014.8.3 ]
extern   LPDIRECT3DCUBETEXTURE9			G_pCubeTex;   
//----------------------------------------------------------------------------
// 여기서부터 셰이더 객체 선언
//----------------------------------------------------------------------------
extern		LPDIRECT3DVERTEXSHADER9			G_pVtxShader ; //[2014.7.27 ]
extern		LPDIRECT3DPIXELSHADER9			G_pPStest;     // 픽셀 쉐이더 테스트용  [2014.7.28 ]
extern      LPDIRECT3DVERTEXSHADER9			G_pVtxShader_Skin; //[2014.7.29 ]

//-----------------------------------------------------------------------------
// 게임오브젝트를 전역변수로 선언
//-----------------------------------------------------------------------------
extern		AirCraft						G_AirCraft; //비행기  
extern      cMissile						G_Missile;  //미사일

extern		cSkyBox							G_SkyBox;  //스카이박스  [2014.8.2 ]		

//-----------------------------------------------------------------------------
// 게임오브젝트들을 이함수안에서 초기화한다. [2014.7.25 ]
//-----------------------------------------------------------------------------
HRESULT Init_GameObjects(void);
//-----------------------------------------------------------------------------
// 셰이더 초기화 함수 (main 에서 사용된다.)  [2014.7.27 ]
// 셰이더 객체를 가지고 있는 게임오브젝트들은
// 이곳에서 GameObject::SetVtxShader()함수를 사용해 포인터를 저장해야한다.
//-----------------------------------------------------------------------------
HRESULT initShader( void );
//-----------------------------------------------------------------------------
// 함수선언
//-----------------------------------------------------------------------------



/************************************************************************/
/* 
*/
/************************************************************************/
class SHGame : public CD3DFrameWork
{

private:
	SHGame();
	~SHGame();
protected:
	
	static SHGame*				    pInstance;

public: //
	
public:  // 외부함수

	int 							Create(HINSTANCE _hInstance);

private:  //내부 함수 and 재정의 함수

	// 이곳에서 씬만들기 , 등록하기, 다이얼로드 만들기 초기화 ,, [2014.11.2 ]
	HRESULT							InitGame();
	//  UI 초기화 [2014.11.3 ]
	HRESULT							Init_UI();
	virtual void					OnUpdateStart(float _dTime);
	virtual void				    OnUpdateEnd(float _dTime);

	virtual void					OnRenderStart();
	virtual void					OnRenderEnd();

	virtual void					OnCloseApp();
	// 텍스트 출력함수 재정의
	//void							RenderText( float DTime );
	// UI그려주는 함수 재정의 
	//void							RenderUI( float DTime );


public:  //상태 확인 함수

	

public: //접근자

	//싱글톤 객체 리턴
	 static SHGame*					GetSHGame();

public:

	// SHGame 메세지 처리 훅함수
	virtual void					OnGameMsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};



namespace __SH_GAME_FRAME__
{
#define SHGameFrame			SHGame::GetSHGame()

};



// [2014.11.2 ]
extern CDXUTDialog						g_MainPlayUI;             // dialog for sample specific controls
extern CDXUTDialog						g_MainTitle_UI;           // 메인타이틀에 사용될 다이얼로그
extern CDialogRscMgr      g_DialogResourceManager; // manager for shared resources of dialogs
/*************************************  end of "SHGame.h" ****************************************/