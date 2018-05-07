//////////////////////////////////////////////////////////////////////////
// 2014. 06. 19. 
// 
// d3d관련  함수 모음
//
//////////////////////////////////////////////////////////////////////////

#include "DXUT.H"
#include "FrameWork.h"
#include "SHUtility.h"
#define MaxLen 2048
//////////////////////////////////////////////////////////////////////////
//
// 장면 렌더타깃 텍스쳐 ,, 일단 3개까지만들어놓기
LPDIRECT3DTEXTURE9		CFullScreenQuad::g_pRenderTargetTexResult = nullptr; // [2014.11.8 ]
LPDIRECT3DTEXTURE9		CFullScreenQuad::g_pRenderTargetTexPass[MaxRenderPass] ={ nullptr,}; //0번패스에 사용될 렌더타겟텍스쳐 [2014.11.8 ]

LPDIRECT3DSURFACE9		CFullScreenQuad::g_pHWBackBuffer = nullptr;		  //
LPDIRECT3DSURFACE9		CFullScreenQuad::g_pSceneSurface[MaxRenderPass] = {nullptr,};

// 전역변수 선언 [2014.11.8 ]
CFullScreenQuad			G_ScreenQuad;

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
// 여기서부터 SHUtill 정의
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
namespace SHUtil
{

	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color)
	{
		D3DLIGHT9 light;
		::ZeroMemory(&light, sizeof(light));

		light.Type      = D3DLIGHT_DIRECTIONAL;
		light.Ambient   = *color * 0.4f;
		light.Diffuse   = *color * 0.6f;
		light.Specular  = *color * 0.3f;
		light.Direction = *direction;

		return light;
	}

	D3DMATERIAL9 InitMaterial(D3DXCOLOR diffuse, D3DXCOLOR amb, D3DXCOLOR specular, D3DXCOLOR emissive,D3DXCOLOR power)
	{
		D3DMATERIAL9 retm ;

		retm.Ambient = amb;
		retm.Diffuse = diffuse;
		retm.Specular = specular;
		retm.Emissive = emissive*0.3f;
		retm.Power = power;

		return retm;

	}

	float getFPS(float _dTime)
	{
		static float count = 0.0f;
		static float ret = 0.0f ;
		if(count > 1.0f)
		{	
			count = 0.0f;
			ret = 1.0f/_dTime;
		}

		count+=_dTime;

		return ret;
	}


//-------------------------------------------------------------------------
//  [2014.7.27 ]
//	베지어 곡선의 변곡점을 랜덤으로 정해주는 함수 
//	_Bezier[] : 변곡점이 들어갈 배열
//  _Start    : 가장 첫번째 변곡점(시작점)
//  _End	  : 가장 마지막 변곡점(도착점)
//  _size	  : 변곡점 총 개수
//  _Second   : 두번째 변곡점,사용자가  정해주기 (발사하고 바로꺽이면 이상하니까)
//-------------------------------------------------------------------------
	void Rand_BezierPos(POSITION _Bezier[],D3DXVECTOR3 _Start, D3DXVECTOR3 _End  , int _size, D3DXVECTOR3 _Second)
	{
		//int sign ; //부호 결정
		float len_vecToEnd; //시작점에서 끝점을 향하는 벡터의 길이
		float len_vecRand; //랜덤벡터의 길이(이것도 랜덤으로 구해야함)  [2014.7.27 ]
		D3DXVECTOR3 vectoEnd; //시작점에서 끝점을 향하는 벡터   [2014.7.27 ]
		D3DXVECTOR3 vecRand;  //vectoEnd와 외적할 벡터 (아무벡터나상관없다.)

		D3DXVECTOR3 min = D3DXVECTOR3(-100.0f ,-100.0f,-100.0f );
		D3DXVECTOR3 max = D3DXVECTOR3(100.0f ,100.0f, 100.0f );

		vectoEnd =  _End -  _Start;
		len_vecToEnd = D3DXVec3Length(&vectoEnd); //시작점에서 끝점을 향하는 벡터의 길이
		D3DXVec3Normalize(&vectoEnd, &vectoEnd);
		for(int i=2; i<_size-1 ; i++)
		{
			GetRandomVector(&vecRand ,&min, &max );//아무벡터나상관없다  어차피외적하고 노말라이즈할거임
			len_vecRand = GetRandomFloat(-len_vecToEnd, len_vecToEnd);
			D3DXVec3Cross(&vecRand , &vectoEnd , &vecRand);
			D3DXVec3Normalize(&vecRand, &vecRand);

			_Bezier[i] = _Start + (vectoEnd * (len_vecToEnd * (_size-1) * 0.1f* i)) + vecRand * len_vecRand; 

		}

		_Bezier[0] = _Start;  //시작점과  
		_Bezier[1] = _Second; //두번째 변곡점
		_Bezier[_size-1] = _End; //끝점은 고정
	}


//-------------------------------------------------------------------------
// 베지어 곡선 함수 [2014.7.27 ]
// Bezier[] 배지어 변곡점이 있는 배열
// idx 에 Bezier[]배열의 가장 마지막 인덱스값 넣기 (size -1)
// float a 는 진행율( 0.0f ~ 1.0f 사이의 값)
//-------------------------------------------------------------------------
	D3DXVECTOR3  BezierCurb( POSITION Bezier[], int idx ,float a )//베 지어 곡선  ////////////////////////////////////////////////////////////////////////////// 
	{	
		D3DXVECTOR3 curr;
		D3DXVECTOR3 Low;
		D3DXVECTOR3 Result;

		if(a > 1.0f)
			a = 1.0f;

		curr.x = (LONG)(Bezier[idx-1].x * (1-a) + Bezier[idx].x * a);
		curr.y = (LONG)(Bezier[idx-1].y * (1-a) + Bezier[idx].y * a);
		curr.z = (LONG)(Bezier[idx-1].z * (1-a) + Bezier[idx].z * a);

		if(idx == 1)
			return curr;
		else
			Low	= BezierCurb(Bezier, idx-1, a);

		Result.x = (LONG)(Low.x * (1-a) + curr.x * a);
		Result.y = (LONG)(Low.y * (1-a) + curr.y * a);
		Result.z = (LONG)(Low.z * (1-a) + curr.z * a);;

		return Result;
	}

	
	//-------------------------------------------------------------------------
	// MsgBox for Unicode project
	//-------------------------------------------------------------------------
	void MsgBoxW(LPCWSTR format, ...)
	{
	   WCHAR buffer[1024];
		
		//CHAR Multi_str[Max_Path_Len]={NULL,};
		//WideCharToMultiByte(CP_ACP, NULL, format, -1, Multi_str, Max_Path_Len, NULL, FALSE);

		va_list vaList;
		va_start(vaList, format);
		int retval = StringCbVPrintfW(buffer, sizeof(wchar_t) * 1024, format, vaList); // retval == -2147024774
		va_end(vaList);

		MessageBoxW(NULL, buffer, L"", MB_OK);
	}

	//-------------------------------------------------------------------------
	// MsgBox for multibyte project
	//-------------------------------------------------------------------------
	void MsgBox(LPCSTR format, ...)
	{
		char buffer[1024];

		va_list vaList;
		va_start(vaList, format);
		int retval = StringCbVPrintfA(buffer, sizeof(wchar_t) * 1024, format, vaList); // retval == -2147024774
		va_end(vaList);


		MessageBoxA(NULL, buffer, "", MB_OK);
	}

}// myUtill 끝

//-------------------------------------------------------------
// 파일 경로라고 가정하고 '\'나 '/'뒤에있는 문자만 남기도록 지운다
//-------------------------------------------------------------
void cutSlash(WCHAR* str)
{	//int size = wcslen(str) * sizeof(WCHAR);
	if (wcschr(str,L'\\'))		wcscpy(str,  wcsrchr(str,L'\\')+1);	// 경로가 있을 경우
	else if (wcschr(str,L'/'))	wcscpy(str, wcsrchr(str,L'/') +1);	// 경로가 있을 경우
}


void cutSlash(const WCHAR* str, WCHAR* _name)
{  // int size = wcslen(str) * sizeof(WCHAR);
	if (wcschr(str,L'\\'))		wcscpy(_name, wcsrchr(str,L'\\')+1);	// 경로가 있을 경우
	else if (wcschr(str,L'/'))	wcscpy(_name, wcsrchr(str,L'/') +1);	// 경로가 있을 경우
}
//-----------------------------------------------------------------------------
/*float f=1.5f; 라고 선언합니다.	f를 비트로 표시하면 이렇습니다.	0011 1111 1100 0000 0000 0000 0000 0000
실수는 일반적인 2진수 계산 방법과 다릅니다.	그리고	int d; d=f; 라고 하면 d에는  1이라는 값이 대입됩니다. 소수점은 취급안하죠...
이때 d를 비트로 표시하면 	0000 0000 0000 0000 0000 0000 0000 0001 	일반적인 2진수로 보면 값은 1이죠...
실수를 정수 변수에 대입하면 메모리(비트)값 그대로 대입하지 않고	"소수점 이하를 뺀 값"으로서 대입을 합니다...
하지만 FtoDW()와 같이d=*(int*)&f; 로 대입하면 (말로 표현하면 f가 저장되어있는 메모리(비트)값 그대로 이제부터  
int형 처럼 취급한다 라고 선언하는겁니다.)	이때 d를 비트로 표시하면	0011 1111 1100 0000 0000 0000 0000 0000
실수에서 쓰인 비트 그대로 가져오죠...	메모리정보(비트)를 그대로 가져오기 위한 함수네요,,[2014.7.23 ]*/ 
//-----------------------------------------------------------------------------
DWORD FtoDw(float f)
{
	return *((DWORD*)&f);
}


//-----------------------------------------------------------------------------
// lowBound 와 highBound 사이의 float값을 랜덤으로 생성해낸다.
//-----------------------------------------------------------------------------
float GetRandomFloat(float lowBound, float highBound)
{
	if( lowBound >= highBound ) // bad input
		return lowBound;

	// get random float in [0, 1] interval
	float f = (rand() % 10000) * 0.0001f; 

	// return float in [lowBound, highBound] interval. 
	return (f * (highBound - lowBound)) + lowBound; 
}



//-----------------------------------------------------------------------------
// [2014.7.23 ]
// 벡터의 각 성분(x,y,z) 의 min과  max 사이의 값으로 새로운 랜덤백터를 만들어낸다.
// pOut : 반환값
//-----------------------------------------------------------------------------

void GetRandomVector(D3DXVECTOR3* pOut,	const D3DXVECTOR3* pMin, const D3DXVECTOR3* pMax)
{
	pOut->x = GetRandomFloat(pMin->x, pMax->x);
	pOut->y = GetRandomFloat(pMin->y, pMax->y);
	pOut->z = GetRandomFloat(pMin->z, pMax->z);
}


/************************************************************************
					class SH_Timer  // [2014.12.2 ]
				다양한곳에 쓰이기위한 타이머 
*/

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
// 여기서부터 SH_Timer 클래스 함수정의 // [2014.12.2 ]
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
//-----------------------------------------------------------------------------
// Arg    : 
// Return :
// Desc   :  시간이 다되었나?   // [2014.12.2 ]
//-----------------------------------------------------------------------------
bool	SH_Timer::isTimeUp()
{
	if(m_bisTimerOn == false)
		return false;


	if(m_Time>0)
	{	
		m_Time -= SHGlobal_GetEngineTime;  
		return false;
	}
	else
	{
		
		return true;
	}


}

//-----------------------------------------------------------------------------
// Arg    : 
// Return : 
// Desc   : 타이머를 꺼버린다.  //[2014.12.7 ]
//-----------------------------------------------------------------------------
void	SH_Timer::SetOff()
{
	m_bisTimerOn = false;
}

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
// 여기서부터 ZFrustum 클래스 함수정의
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

int ZFrustum::m_innerObjCnt = 0;

enum FRUSTUM_PLANE_TYPE
{ FPT_LEFT , FPT_RIGHT, FPT_TOP, FPT_BOTTOM, FPT_NEAR, FPT_FAR , FPT_MAX };

ZFrustum::ZFrustum()
{
	ZeroMemory( m_vtx, sizeof( m_vtx[0] ) * 8 );
	ZeroMemory( m_plane, sizeof( m_plane[0] ) * 6 );
}

//-----------------------------------------------------------------------------
//  카메라(view) * 프로젝션(projection)행렬을 입력받아 6개의 평면을 만든다.
//  카메라 이동이 있을경우에는 매프레임마다 계산해줘야한다.
//-----------------------------------------------------------------------------

bool ZFrustum::Make( D3DXMATRIX* pmatViewProj )
{
	

	D3DXMATRIX  mVPTranspos;

	//모니터좌표로 투영된 6개의 평면 생성
	m_plane[FPT_LEFT] = D3DXPLANE(1.0f, 0.0f, 0.0f, 1.0f);
	m_plane[FPT_RIGHT] = D3DXPLANE(-1.0f, 0.0f, 0.0f, 1.0f);
	m_plane[FPT_TOP] = D3DXPLANE(0.0f, -1.0f, 0.0f, 1.0f);
	m_plane[FPT_BOTTOM] = D3DXPLANE(0.0f, 1.0f, 0.0f, 1.0f);
	m_plane[FPT_NEAR] = D3DXPLANE(0.0f, 0.0f, 1.0f, 0.0f);	
	m_plane[FPT_FAR] = D3DXPLANE(0.0f, 0.0f, -1.0f, 1.0f);
	
	//월드상의 카메라와 똑같은 Frustum 을 생성하기

	D3DXMatrixTranspose(&mVPTranspos ,pmatViewProj);	
	for(int i=0; i< FPT_MAX ; i++)
	{
		D3DXPlaneTransform(&m_plane[i], &m_plane[i], &mVPTranspos );
	}
	
	m_innerObjCnt = 0;
	return TRUE;
}

/** 중심(v)와 반지름(radius)를 갖는 경계구(bounding sphere)가 프러스텀안에 있으면
 *  TRUE를 반환, 아니면 FALSE를 반환한다.
 */
bool ZFrustum::IsSphereIN( D3DXVECTOR3* pv, float radius )
{
	float		fDist;
	
	for(int i = 0 ; i<FPT_MAX ; i++)
	{
		fDist = D3DXPlaneDotCoord( &m_plane[i], pv );
		if( fDist < (-radius) ) return FALSE;	// 프러스텀에 없음
	}
	m_innerObjCnt++;
	return TRUE;
}

/// 프러스텀을 화면에 그려준다.
bool ZFrustum::Draw( LPDIRECT3DDEVICE9 pDev )
{
	return true;
}


///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//  여기서부터 바운딩스피어 Class 함수 정의
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------
// 바운딩스피어 생성자
//---------------------------------------------------------------------------------------------

BoundingSphere::BoundingSphere() : m_Pos(D3DXVECTOR3(0.0f, 0.0f,0.0f)) , m_Radius(1.0f), m_HaveMesh(FALSE), m_Mesh(NULL)
{
	D3DXMatrixIdentity(&matIdentity);
}

HRESULT BoundingSphere::init(float _radius,  bool _HaveMesh  ,D3DXVECTOR3 _pos /*= D3DXVECTOR3(0.0f, 0.0f, 0.0f)*/)
{
	m_Radius = _radius;
	m_PosOrigine = _pos;
	m_HaveMesh = _HaveMesh;

	HRESULT hr;
	if(m_HaveMesh == true)
	{		
		V_RETURN(D3DXCreateSphere(SHGameFrame->GetD3DDevice(), _radius, 20, 7, &m_Mesh, NULL));
	}

	return S_OK;
}

//----------------------------------------------------------------------------
// 소멸자
//----------------------------------------------------------------------------

BoundingSphere::~BoundingSphere()
{
	SAFE_RELEASE(m_Mesh)
}

//-----------------------------------------------------------------------------
//   BoundingSphere 그려주기
//   _WorldTM 를 넣어줘야  m_pos좌표가 변한다. 
//-----------------------------------------------------------------------------

void  BoundingSphere::Draw( const D3DXMATRIX* _WorldTM)
{
	if(_WorldTM != NULL)
	{
		m_Pos =D3DXVECTOR3(_WorldTM->_41, _WorldTM->_42,_WorldTM->_43);


		if(G_Frustum.IsSphereIN(&m_Pos, m_Radius) == true)
		{	
			if(m_HaveMesh == true) //메쉬를 가지고 있다면? [2014.7.26 ]
			{	
				SHGlobal_GetD3DDevice->SetTexture(0,NULL);
				SHGlobal_GetD3DDevice->SetMaterial(&G_DefaultMtrl);	
				SHGlobal_GetD3DDevice->SetTransform(D3DTS_WORLD, _WorldTM);
				m_Mesh->DrawSubset(0);
			}

		}

	}
}

//-----------------------------------------------------------------------------
// DeepCopy 깊은복사를 위한 함수
//-----------------------------------------------------------------------------
HRESULT  BoundingSphere::DeepCopy(const BoundingSphere& rhs)
{
	m_Radius = rhs.m_Radius; //반지름 
	m_Pos	= rhs.m_Pos;    // 월드상의 위치
	m_HaveMesh = rhs.m_HaveMesh;
	matIdentity = rhs.matIdentity;
	m_PosOrigine = rhs.m_PosOrigine;
	HRESULT hr;

	if(rhs.m_Mesh !=NULL)
	{
		V_RETURN(D3DXCreateSphere(SHGlobal_GetD3DDevice, rhs.get_Radius(),20, 7,&m_Mesh,NULL ));
	}

	return S_OK;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 여기서부터 SHEngine_Texthelper 멤버함수
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------
//  생성자, 소멸자
//-------------------------------------------------------------------------
SHEngine_TextHelper::SHEngine_TextHelper( ID3DXFont* pFont, ID3DXSprite* pSprite, int nLineHeight )     { Init( pFont, pSprite, nLineHeight ); }
SHEngine_TextHelper::~SHEngine_TextHelper()
{
}

//--------------------------------------------------------------------------------------
void SHEngine_TextHelper::Init( ID3DXFont* pFont9, ID3DXSprite* pSprite9, int nLineHeight )
{
	m_pFont9 = pFont9;
	m_pSprite9 = pSprite9;
	m_clr = D3DXCOLOR(1,1,1,1);
	m_pt.x = 0; 
	m_pt.y = 0; 
	m_nLineHeight = nLineHeight;
}


//--------------------------------------------------------------------------------------
HRESULT SHEngine_TextHelper::DrawFormattedTextLine( const WCHAR* strMsg, ... )
{
	WCHAR strBuffer[512];

	va_list args;
	va_start(args, strMsg);
	StringCchVPrintf( strBuffer, 512, strMsg, args );
	strBuffer[511] = L'\0';
	va_end(args);

	return DrawTextLine( strBuffer );
}


//--------------------------------------------------------------------------------------
HRESULT SHEngine_TextHelper::DrawTextLine( const WCHAR* strMsg )
{
	if( NULL == m_pFont9 ) 
		return DXUT_ERR_MSGBOX( L"DrawTextLine", E_INVALIDARG );

	HRESULT hr;
	RECT rc;
	SetRect( &rc, m_pt.x, m_pt.y, 0, 0 ); 
	if( m_pFont9 )
		hr = m_pFont9->DrawText( m_pSprite9, strMsg, -1, &rc, DT_NOCLIP, m_clr );
	//if( FAILED(hr) )
	//    return DXTRACE_ERR_MSGBOX( L"DrawText", hr );

	m_pt.y += m_nLineHeight;

	return S_OK;
}


HRESULT SHEngine_TextHelper::DrawFormattedTextLine( RECT &rc, DWORD dwFlags, const WCHAR* strMsg, ... )
{
	WCHAR strBuffer[512];

	va_list args;
	va_start(args, strMsg);
	StringCchVPrintf( strBuffer, 512, strMsg, args );
	strBuffer[511] = L'\0';
	va_end(args);

	return DrawTextLine( rc, dwFlags, strBuffer );
}


HRESULT SHEngine_TextHelper::DrawTextLine( RECT &rc, DWORD dwFlags, const WCHAR* strMsg )
{
	if( NULL == m_pFont9 ) 
		return DXUT_ERR_MSGBOX( L"DrawTextLine", E_INVALIDARG );

	HRESULT hr;
	if( m_pFont9 )
		hr = m_pFont9->DrawText( m_pSprite9, strMsg, -1, &rc, dwFlags, m_clr );
	//if( FAILED(hr) )
	//    return DXTRACE_ERR_MSGBOX( L"DrawText", hr );

	m_pt.y += m_nLineHeight;

	return S_OK;
}


//--------------------------------------------------------------------------------------
void SHEngine_TextHelper::Begin()
{
	if( m_pSprite9 )
		m_pSprite9->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );
}
void SHEngine_TextHelper::End()
{
	if( m_pSprite9 )
		m_pSprite9->End();
}



///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//
//  여기서 부터 class CFullScreenQuad  함수 정의 [2014.11.7 ]
//
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------
// 생성자  [2014.11.8 ]
//-------------------------------------------------------------------------
CFullScreenQuad::CFullScreenQuad() // 생성자
{
	m_pFullscreenQuadDecl = nullptr;
	m_pFullscreenQuadVB = nullptr;
	m_pFullscreenQuadIB = nullptr;
	m_pD3DDevice = nullptr;
}


//-------------------------------------------------------------------------
// 소멸자  [2014.11.8 ]
//-------------------------------------------------------------------------
CFullScreenQuad::~CFullScreenQuad() // 소멸자
{
	for(int i=0 ; i< MaxRenderPass ; i++)
	{
			SAFE_RELEASE(g_pRenderTargetTexPass[i])
			SAFE_RELEASE(g_pSceneSurface[i])
	}

	SAFE_RELEASE(g_pRenderTargetTexResult) //최종 화면에 그릴 렌더타겟 [2014.11.8 ]
	SAFE_RELEASE(g_pHWBackBuffer);		  // 현재 하드웨어 백버퍼 [2014.11.8 ]

	SAFE_RELEASE(m_pFullscreenQuadDecl)
	SAFE_RELEASE(m_pFullscreenQuadVB)
	SAFE_RELEASE(m_pFullscreenQuadIB)

	SAFE_RELEASE(m_pD3DDevice)
}


//-----------------------------------------------------------------------------
// Desc   :  초기화 함수  [2014.11.8 ]
//-----------------------------------------------------------------------------
int		CFullScreenQuad::Init()
{

	m_pD3DDevice = SHGlobal_GetD3DDevice;
	// 정점 선언을 만든다
	D3DVERTEXELEMENT9 vtxDesc[3];
	int offset = 0;
	int i = 0;

	// 위치
	vtxDesc[i].Stream     = 0;
	vtxDesc[i].Offset     = offset;
	vtxDesc[i].Type       = D3DDECLTYPE_FLOAT3;
	vtxDesc[i].Method     = D3DDECLMETHOD_DEFAULT;
	vtxDesc[i].Usage      = D3DDECLUSAGE_POSITION;
	vtxDesc[i].UsageIndex = 0;

	offset += sizeof(float) * 3;
	++i;

	// UV좌표 0
	vtxDesc[i].Stream     = 0;
	vtxDesc[i].Offset     = offset;
	vtxDesc[i].Type       = D3DDECLTYPE_FLOAT2;
	vtxDesc[i].Method     = D3DDECLMETHOD_DEFAULT;
	vtxDesc[i].Usage      = D3DDECLUSAGE_TEXCOORD;	
	vtxDesc[i].UsageIndex = 0;

	offset += sizeof(float) * 2;
	++i;

	// 정점포맷의 끝임을 표현 (D3DDECL_END())
	vtxDesc[i].Stream     = 0xFF;			
	vtxDesc[i].Offset     = 0;
	vtxDesc[i].Type       = D3DDECLTYPE_UNUSED;
	vtxDesc[i].Method     = 0;
	vtxDesc[i].Usage      = 0;
	vtxDesc[i].UsageIndex = 0;

	m_pD3DDevice->CreateVertexDeclaration(vtxDesc, &m_pFullscreenQuadDecl);

	// 정점버퍼를 만든다.
	m_pD3DDevice->CreateVertexBuffer(offset * 4, 0, 0, D3DPOOL_MANAGED, &m_pFullscreenQuadVB, NULL);
	void * vertexData = NULL;
	m_pFullscreenQuadVB->Lock(0, 0, &vertexData, 0);
	{
		float * data = (float*)vertexData;
		*data++ = -1.0f;	*data++ = 1.0f;		*data++ = 0.0f;
		*data++ = 0.0f;		*data++ = 0.0f;

		*data++ = 1.0f;		*data++ = 1.0f;		*data++ = 0.0f;
		*data++ = 1.0f;		*data++ = 0;

		*data++ = 1.0f;		*data++ = -1.0f;	*data++ = 0.0f;
		*data++ = 1.0f;		*data++ = 1.0f;

		*data++ = -1.0f;	*data++ = -1.0f;	*data++ = 0.0f;
		*data++ = 0.0f;		*data++ = 1.0f;
	}

	m_pFullscreenQuadVB->Unlock();

	// 색인버퍼를 만든다.
	m_pD3DDevice->CreateIndexBuffer(sizeof(short) * 6, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pFullscreenQuadIB, NULL);
	void * indexData = NULL;
	m_pFullscreenQuadIB->Lock(0, 0, &indexData, 0);
	{
		unsigned short * data = (unsigned short*)indexData;
		*data++ = 0;	*data++ = 3;	*data++ = 1;
		*data++ = 1;	*data++ = 3;	*data++ = 2;
	}
	m_pFullscreenQuadIB->Unlock();

	//////////////////////////////////////////////////////////////////////////
	// 렌더타깃 텍스쳐들을 만든다. [2014.11.8 ]
	if(FAILED(m_pD3DDevice->CreateTexture( SHGlobal_GetDeviceSetting.pp.BackBufferWidth, SHGlobal_GetDeviceSetting.pp.BackBufferHeight,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8,	D3DPOOL_DEFAULT, &g_pRenderTargetTexResult, NULL ) ))
	{		return S_FALSE;	assert(false);}

	for(int i = 0 ; i< MaxRenderPass ; i++)
	{
		if(FAILED(m_pD3DDevice->CreateTexture( SHGlobal_GetDeviceSetting.pp.BackBufferWidth, SHGlobal_GetDeviceSetting.pp.BackBufferHeight,
			1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8,	D3DPOOL_DEFAULT, &g_pRenderTargetTexPass[i], NULL ) ))
		{		return S_FALSE;	assert(false);}
		 
		// 서페이스들을 nullptr로 초기화시켜준다. 
		g_pSceneSurface[i] = nullptr; 
	}

	

	LOG_WRITE_A("CFullScreenQuad::Init() ,   return S_OK"); // 로그파일에 함수 성공을 알린다. [2014.11.8 ]

	return S_OK;
}


//-----------------------------------------------------------------------------
// Desc   :  그리기 함수  [2014.11.8 ]
//-----------------------------------------------------------------------------
void	CFullScreenQuad::Draw()
{
	// 화면가득한 사각형을 그린다.
	m_pD3DDevice->SetStreamSource(0, m_pFullscreenQuadVB, 0, sizeof(float)*5);
	m_pD3DDevice->SetIndices(m_pFullscreenQuadIB);
	m_pD3DDevice->SetVertexDeclaration(m_pFullscreenQuadDecl);
	m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);
}

//-----------------------------------------------------------------------------
// Arg    : 
// Return : HRESULT (S_OK) , (S_FALSE)
// Desc   : 전프레임에 그렸던 모든 장면을 지운다. 
//-----------------------------------------------------------------------------
HRESULT		CFullScreenQuad::ClearAllSurface()
{
	HRESULT hr = S_OK;

	for(int i = 0  ; i < MaxRenderPass ; i++)
	{
		m_pD3DDevice->SetRenderTarget( 0, G_ScreenQuad.g_pSceneSurface[i] );
		hr = m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, 0xFF000000, 1.0f, 0 );
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
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
// 여기서부터 ...함수정의
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


//---------------------------------------------------------------------------------------------
// 2014.7.16
// 
// 픽킹함수
// 게임에있는 모든 오브젝트를 검사할 예정
//
//---------------------------------------------------------------------------------------------
/*
HRESULT Pick(float _dTime)
{
	size_t   Size_t;

	D3DXVECTOR3 vPickRayDir;
	D3DXVECTOR3 vPickRayOrig;
	static GEOMOBJECT* pBone = nullptr;
	//swprintf_s(G_SelectedObjName,L"NONE");

	if(GetCapture())
	{		
		D3DXMATRIX View_Inv, Proj_Inv, MatInv; // view의 역행렬,  Proj의 역행렬, MatInv = proj역행렬 * view의 역행렬
		POINT ptCursor;
		GetCursorPos( &ptCursor );
		ScreenToClient( SHGlobal_GetHwnd, &ptCursor );

		// Compute the vector of the Pick ray in screen space,
		D3DXVECTOR3 p_near, p_far ;
		p_near.x = ( ( ( 2.0f * ptCursor.x ) / SHGlobal_GetScreenWidth ) - 1 ) ;
		p_near.y = -( ( ( 2.0f * ptCursor.y ) / SHGlobal_GetScreenHeight ) - 1 );
		p_near.z = 0.0f;

		p_far.x =p_near.x;
		p_far.y =p_near.y;
		p_far.z =1.0f;

		D3DXMatrixInverse( &Proj_Inv, NULL, &SHGlobal_GetCurScene_Camera->Get_ProjMat() );
		D3DXMatrixInverse( &View_Inv, NULL, &SHGlobal_GetCurScene_Camera->Get_ViewMat() );

		MatInv = Proj_Inv * View_Inv;
		D3DXVec3TransformCoord(&vPickRayOrig, &p_near, &MatInv);
		D3DXVec3TransformCoord(&p_far, &p_far, &MatInv);
		vPickRayDir = p_far - vPickRayOrig;


		//////////////////////////////////////////////////////////////////////////
		// 여기서부터 캐릭터내의 경계구들에 대한 픽킹검사
		pBone = G_AirCraft.GetASEData()->Piking(vPickRayOrig,vPickRayDir) ;
		if(pBone!= nullptr)
		{
			mbstowcs_s(&Size_t, G_SelectedObjName , pBone->NodeTM.Name,  _TRUNCATE );

		}
		else if(pBone == nullptr)
		{
			swprintf_s(G_SelectedObjName,L"NONE");
		}


		// 
		// 		if(Collision_Sphere_Line(G_BS.get_Pos(), G_BS.get_Radius(), vPickRayOrig, vPickRayDir ) == true)
		// 		{
		// 			//mbstowcs_s(&Size_t, G_SelectedObjName, L"myBall",  _TRUNCATE );
		// 			swprintf_s(G_SelectedObjName, L"myBall");
		// 		}
	}//if(GetCapture()) end

	// 	if(pBone != nullptr)
	// 	{
	// 		ArcBall(pBone);
	// 	}

	return S_OK;
}
*/


//---------------------------------------------------------------------------------------------
// 2014.07.17
// 
// 아크볼 함수
// 오른쪽마우스를 드래그 해서 회전시킨다.
// D3DXMATRIX* _pOut  :  반환값(회전된 매트릭스)
//
//---------------------------------------------------------------------------------------------
/*
void ArcBall(GEOMOBJECT* _pBone )
{
	D3DXMATRIX* _pOut = &_pBone->m_ArcBallTM ;
	if(_pOut == nullptr)
		return ;

	D3DXVECTOR3 FrontVec = SHGlobal_GetCurScene_Camera->GetLookVec();
	D3DXVECTOR3 DragVec;//Drag해서 생긴 벡터
	D3DXVECTOR3 RotAxis;
	float		RotAngle; //라디안
	static POINT ptFirstClick;
	POINT ptLastClick;
	static D3DXMATRIX  preTM , InvTM , WorldTM;//2014.07.18
	static bool RbuttonOnce = false;
	D3DXMATRIX RotTM;

	if(KEY_DOWN(VK_RBUTTON) && (RbuttonOnce == false))//LButtonDown 한번만 클릭??  이전에 누른 적이 없고 호출시점에 눌린 상태
	{
		GetCursorPos( &ptFirstClick );
		ScreenToClient( G_hWnd, &ptFirstClick );	
		preTM = *_pOut;
		WorldTM = _pBone->getWorldTM();
		D3DXMatrixInverse(&InvTM,NULL,&WorldTM);
		RbuttonOnce = true;
	}

	if(KEY_UP(VK_RBUTTON)){ RbuttonOnce = false;}

	if(KEY_DOWN(VK_RBUTTON))
	{
		GetCursorPos( &ptLastClick );
		ScreenToClient( G_hWnd, &ptLastClick );	//마우스좌표받아오기

		DragVec = D3DXVECTOR3(ptLastClick.x - ptFirstClick.x,   ptFirstClick.y - ptLastClick.y,  0.0f);
		RotAngle = D3DXVec3Length(&DragVec);
		D3DXVec3Normalize(&DragVec, &DragVec);
		D3DXVec3Cross(&RotAxis, &FrontVec , &DragVec);
		D3DXVec3TransformNormal(&RotAxis, &RotAxis, &InvTM) ;
		D3DXMatrixRotationAxis(&RotTM, &RotAxis, (-2.0f*RotAngle/(float)G_ScreenX) );
		*_pOut = preTM * RotTM;

	}


}
*/
//******************************************  끝  **************************************************//