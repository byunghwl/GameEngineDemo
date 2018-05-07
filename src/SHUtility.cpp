//////////////////////////////////////////////////////////////////////////
// 2014. 06. 19. 
// 
// d3d����  �Լ� ����
//
//////////////////////////////////////////////////////////////////////////

#include "DXUT.H"
#include "FrameWork.h"
#include "SHUtility.h"
#define MaxLen 2048
//////////////////////////////////////////////////////////////////////////
//
// ��� ����Ÿ�� �ؽ��� ,, �ϴ� 3��������������
LPDIRECT3DTEXTURE9		CFullScreenQuad::g_pRenderTargetTexResult = nullptr; // [2014.11.8 ]
LPDIRECT3DTEXTURE9		CFullScreenQuad::g_pRenderTargetTexPass[MaxRenderPass] ={ nullptr,}; //0���н��� ���� ����Ÿ���ؽ��� [2014.11.8 ]

LPDIRECT3DSURFACE9		CFullScreenQuad::g_pHWBackBuffer = nullptr;		  //
LPDIRECT3DSURFACE9		CFullScreenQuad::g_pSceneSurface[MaxRenderPass] = {nullptr,};

// �������� ���� [2014.11.8 ]
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
// ���⼭���� SHUtill ����
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
//	������ ��� �������� �������� �����ִ� �Լ� 
//	_Bezier[] : �������� �� �迭
//  _Start    : ���� ù��° ������(������)
//  _End	  : ���� ������ ������(������)
//  _size	  : ������ �� ����
//  _Second   : �ι�° ������,����ڰ�  �����ֱ� (�߻��ϰ� �ٷβ��̸� �̻��ϴϱ�)
//-------------------------------------------------------------------------
	void Rand_BezierPos(POSITION _Bezier[],D3DXVECTOR3 _Start, D3DXVECTOR3 _End  , int _size, D3DXVECTOR3 _Second)
	{
		//int sign ; //��ȣ ����
		float len_vecToEnd; //���������� ������ ���ϴ� ������ ����
		float len_vecRand; //���������� ����(�̰͵� �������� ���ؾ���)  [2014.7.27 ]
		D3DXVECTOR3 vectoEnd; //���������� ������ ���ϴ� ����   [2014.7.27 ]
		D3DXVECTOR3 vecRand;  //vectoEnd�� ������ ���� (�ƹ����ͳ��������.)

		D3DXVECTOR3 min = D3DXVECTOR3(-100.0f ,-100.0f,-100.0f );
		D3DXVECTOR3 max = D3DXVECTOR3(100.0f ,100.0f, 100.0f );

		vectoEnd =  _End -  _Start;
		len_vecToEnd = D3DXVec3Length(&vectoEnd); //���������� ������ ���ϴ� ������ ����
		D3DXVec3Normalize(&vectoEnd, &vectoEnd);
		for(int i=2; i<_size-1 ; i++)
		{
			GetRandomVector(&vecRand ,&min, &max );//�ƹ����ͳ��������  �����ǿ����ϰ� �븻�������Ұ���
			len_vecRand = GetRandomFloat(-len_vecToEnd, len_vecToEnd);
			D3DXVec3Cross(&vecRand , &vectoEnd , &vecRand);
			D3DXVec3Normalize(&vecRand, &vecRand);

			_Bezier[i] = _Start + (vectoEnd * (len_vecToEnd * (_size-1) * 0.1f* i)) + vecRand * len_vecRand; 

		}

		_Bezier[0] = _Start;  //��������  
		_Bezier[1] = _Second; //�ι�° ������
		_Bezier[_size-1] = _End; //������ ����
	}


//-------------------------------------------------------------------------
// ������ � �Լ� [2014.7.27 ]
// Bezier[] ������ �������� �ִ� �迭
// idx �� Bezier[]�迭�� ���� ������ �ε����� �ֱ� (size -1)
// float a �� ������( 0.0f ~ 1.0f ������ ��)
//-------------------------------------------------------------------------
	D3DXVECTOR3  BezierCurb( POSITION Bezier[], int idx ,float a )//�� ���� �  ////////////////////////////////////////////////////////////////////////////// 
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

}// myUtill ��

//-------------------------------------------------------------
// ���� ��ζ�� �����ϰ� '\'�� '/'�ڿ��ִ� ���ڸ� ���⵵�� �����
//-------------------------------------------------------------
void cutSlash(WCHAR* str)
{	//int size = wcslen(str) * sizeof(WCHAR);
	if (wcschr(str,L'\\'))		wcscpy(str,  wcsrchr(str,L'\\')+1);	// ��ΰ� ���� ���
	else if (wcschr(str,L'/'))	wcscpy(str, wcsrchr(str,L'/') +1);	// ��ΰ� ���� ���
}


void cutSlash(const WCHAR* str, WCHAR* _name)
{  // int size = wcslen(str) * sizeof(WCHAR);
	if (wcschr(str,L'\\'))		wcscpy(_name, wcsrchr(str,L'\\')+1);	// ��ΰ� ���� ���
	else if (wcschr(str,L'/'))	wcscpy(_name, wcsrchr(str,L'/') +1);	// ��ΰ� ���� ���
}
//-----------------------------------------------------------------------------
/*float f=1.5f; ��� �����մϴ�.	f�� ��Ʈ�� ǥ���ϸ� �̷����ϴ�.	0011 1111 1100 0000 0000 0000 0000 0000
�Ǽ��� �Ϲ����� 2���� ��� ����� �ٸ��ϴ�.	�׸���	int d; d=f; ��� �ϸ� d����  1�̶�� ���� ���Ե˴ϴ�. �Ҽ����� ��޾�����...
�̶� d�� ��Ʈ�� ǥ���ϸ� 	0000 0000 0000 0000 0000 0000 0000 0001 	�Ϲ����� 2������ ���� ���� 1����...
�Ǽ��� ���� ������ �����ϸ� �޸�(��Ʈ)�� �״�� �������� �ʰ�	"�Ҽ��� ���ϸ� �� ��"���μ� ������ �մϴ�...
������ FtoDW()�� ����d=*(int*)&f; �� �����ϸ� (���� ǥ���ϸ� f�� ����Ǿ��ִ� �޸�(��Ʈ)�� �״�� ��������  
int�� ó�� ����Ѵ� ��� �����ϴ°̴ϴ�.)	�̶� d�� ��Ʈ�� ǥ���ϸ�	0011 1111 1100 0000 0000 0000 0000 0000
�Ǽ����� ���� ��Ʈ �״�� ��������...	�޸�����(��Ʈ)�� �״�� �������� ���� �Լ��׿�,,[2014.7.23 ]*/ 
//-----------------------------------------------------------------------------
DWORD FtoDw(float f)
{
	return *((DWORD*)&f);
}


//-----------------------------------------------------------------------------
// lowBound �� highBound ������ float���� �������� �����س���.
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
// ������ �� ����(x,y,z) �� min��  max ������ ������ ���ο� �������͸� ������.
// pOut : ��ȯ��
//-----------------------------------------------------------------------------

void GetRandomVector(D3DXVECTOR3* pOut,	const D3DXVECTOR3* pMin, const D3DXVECTOR3* pMax)
{
	pOut->x = GetRandomFloat(pMin->x, pMax->x);
	pOut->y = GetRandomFloat(pMin->y, pMax->y);
	pOut->z = GetRandomFloat(pMin->z, pMax->z);
}


/************************************************************************
					class SH_Timer  // [2014.12.2 ]
				�پ��Ѱ��� ���̱����� Ÿ�̸� 
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
// ���⼭���� SH_Timer Ŭ���� �Լ����� // [2014.12.2 ]
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
// Desc   :  �ð��� �ٵǾ���?   // [2014.12.2 ]
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
// Desc   : Ÿ�̸Ӹ� ��������.  //[2014.12.7 ]
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
// ���⼭���� ZFrustum Ŭ���� �Լ�����
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
//  ī�޶�(view) * ��������(projection)����� �Է¹޾� 6���� ����� �����.
//  ī�޶� �̵��� ������쿡�� �������Ӹ��� ���������Ѵ�.
//-----------------------------------------------------------------------------

bool ZFrustum::Make( D3DXMATRIX* pmatViewProj )
{
	

	D3DXMATRIX  mVPTranspos;

	//�������ǥ�� ������ 6���� ��� ����
	m_plane[FPT_LEFT] = D3DXPLANE(1.0f, 0.0f, 0.0f, 1.0f);
	m_plane[FPT_RIGHT] = D3DXPLANE(-1.0f, 0.0f, 0.0f, 1.0f);
	m_plane[FPT_TOP] = D3DXPLANE(0.0f, -1.0f, 0.0f, 1.0f);
	m_plane[FPT_BOTTOM] = D3DXPLANE(0.0f, 1.0f, 0.0f, 1.0f);
	m_plane[FPT_NEAR] = D3DXPLANE(0.0f, 0.0f, 1.0f, 0.0f);	
	m_plane[FPT_FAR] = D3DXPLANE(0.0f, 0.0f, -1.0f, 1.0f);
	
	//������� ī�޶�� �Ȱ��� Frustum �� �����ϱ�

	D3DXMatrixTranspose(&mVPTranspos ,pmatViewProj);	
	for(int i=0; i< FPT_MAX ; i++)
	{
		D3DXPlaneTransform(&m_plane[i], &m_plane[i], &mVPTranspos );
	}
	
	m_innerObjCnt = 0;
	return TRUE;
}

/** �߽�(v)�� ������(radius)�� ���� ��豸(bounding sphere)�� �������Ҿȿ� ������
 *  TRUE�� ��ȯ, �ƴϸ� FALSE�� ��ȯ�Ѵ�.
 */
bool ZFrustum::IsSphereIN( D3DXVECTOR3* pv, float radius )
{
	float		fDist;
	
	for(int i = 0 ; i<FPT_MAX ; i++)
	{
		fDist = D3DXPlaneDotCoord( &m_plane[i], pv );
		if( fDist < (-radius) ) return FALSE;	// �������ҿ� ����
	}
	m_innerObjCnt++;
	return TRUE;
}

/// ���������� ȭ�鿡 �׷��ش�.
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
//  ���⼭���� �ٿ�����Ǿ� Class �Լ� ����
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
// �ٿ�����Ǿ� ������
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
// �Ҹ���
//----------------------------------------------------------------------------

BoundingSphere::~BoundingSphere()
{
	SAFE_RELEASE(m_Mesh)
}

//-----------------------------------------------------------------------------
//   BoundingSphere �׷��ֱ�
//   _WorldTM �� �־����  m_pos��ǥ�� ���Ѵ�. 
//-----------------------------------------------------------------------------

void  BoundingSphere::Draw( const D3DXMATRIX* _WorldTM)
{
	if(_WorldTM != NULL)
	{
		m_Pos =D3DXVECTOR3(_WorldTM->_41, _WorldTM->_42,_WorldTM->_43);


		if(G_Frustum.IsSphereIN(&m_Pos, m_Radius) == true)
		{	
			if(m_HaveMesh == true) //�޽��� ������ �ִٸ�? [2014.7.26 ]
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
// DeepCopy �������縦 ���� �Լ�
//-----------------------------------------------------------------------------
HRESULT  BoundingSphere::DeepCopy(const BoundingSphere& rhs)
{
	m_Radius = rhs.m_Radius; //������ 
	m_Pos	= rhs.m_Pos;    // ������� ��ġ
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
// ���⼭���� SHEngine_Texthelper ����Լ�
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------
//  ������, �Ҹ���
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
//  ���⼭ ���� class CFullScreenQuad  �Լ� ���� [2014.11.7 ]
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
// ������  [2014.11.8 ]
//-------------------------------------------------------------------------
CFullScreenQuad::CFullScreenQuad() // ������
{
	m_pFullscreenQuadDecl = nullptr;
	m_pFullscreenQuadVB = nullptr;
	m_pFullscreenQuadIB = nullptr;
	m_pD3DDevice = nullptr;
}


//-------------------------------------------------------------------------
// �Ҹ���  [2014.11.8 ]
//-------------------------------------------------------------------------
CFullScreenQuad::~CFullScreenQuad() // �Ҹ���
{
	for(int i=0 ; i< MaxRenderPass ; i++)
	{
			SAFE_RELEASE(g_pRenderTargetTexPass[i])
			SAFE_RELEASE(g_pSceneSurface[i])
	}

	SAFE_RELEASE(g_pRenderTargetTexResult) //���� ȭ�鿡 �׸� ����Ÿ�� [2014.11.8 ]
	SAFE_RELEASE(g_pHWBackBuffer);		  // ���� �ϵ���� ����� [2014.11.8 ]

	SAFE_RELEASE(m_pFullscreenQuadDecl)
	SAFE_RELEASE(m_pFullscreenQuadVB)
	SAFE_RELEASE(m_pFullscreenQuadIB)

	SAFE_RELEASE(m_pD3DDevice)
}


//-----------------------------------------------------------------------------
// Desc   :  �ʱ�ȭ �Լ�  [2014.11.8 ]
//-----------------------------------------------------------------------------
int		CFullScreenQuad::Init()
{

	m_pD3DDevice = SHGlobal_GetD3DDevice;
	// ���� ������ �����
	D3DVERTEXELEMENT9 vtxDesc[3];
	int offset = 0;
	int i = 0;

	// ��ġ
	vtxDesc[i].Stream     = 0;
	vtxDesc[i].Offset     = offset;
	vtxDesc[i].Type       = D3DDECLTYPE_FLOAT3;
	vtxDesc[i].Method     = D3DDECLMETHOD_DEFAULT;
	vtxDesc[i].Usage      = D3DDECLUSAGE_POSITION;
	vtxDesc[i].UsageIndex = 0;

	offset += sizeof(float) * 3;
	++i;

	// UV��ǥ 0
	vtxDesc[i].Stream     = 0;
	vtxDesc[i].Offset     = offset;
	vtxDesc[i].Type       = D3DDECLTYPE_FLOAT2;
	vtxDesc[i].Method     = D3DDECLMETHOD_DEFAULT;
	vtxDesc[i].Usage      = D3DDECLUSAGE_TEXCOORD;	
	vtxDesc[i].UsageIndex = 0;

	offset += sizeof(float) * 2;
	++i;

	// ���������� ������ ǥ�� (D3DDECL_END())
	vtxDesc[i].Stream     = 0xFF;			
	vtxDesc[i].Offset     = 0;
	vtxDesc[i].Type       = D3DDECLTYPE_UNUSED;
	vtxDesc[i].Method     = 0;
	vtxDesc[i].Usage      = 0;
	vtxDesc[i].UsageIndex = 0;

	m_pD3DDevice->CreateVertexDeclaration(vtxDesc, &m_pFullscreenQuadDecl);

	// �������۸� �����.
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

	// ���ι��۸� �����.
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
	// ����Ÿ�� �ؽ��ĵ��� �����. [2014.11.8 ]
	if(FAILED(m_pD3DDevice->CreateTexture( SHGlobal_GetDeviceSetting.pp.BackBufferWidth, SHGlobal_GetDeviceSetting.pp.BackBufferHeight,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8,	D3DPOOL_DEFAULT, &g_pRenderTargetTexResult, NULL ) ))
	{		return S_FALSE;	assert(false);}

	for(int i = 0 ; i< MaxRenderPass ; i++)
	{
		if(FAILED(m_pD3DDevice->CreateTexture( SHGlobal_GetDeviceSetting.pp.BackBufferWidth, SHGlobal_GetDeviceSetting.pp.BackBufferHeight,
			1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8,	D3DPOOL_DEFAULT, &g_pRenderTargetTexPass[i], NULL ) ))
		{		return S_FALSE;	assert(false);}
		 
		// �����̽����� nullptr�� �ʱ�ȭ�����ش�. 
		g_pSceneSurface[i] = nullptr; 
	}

	

	LOG_WRITE_A("CFullScreenQuad::Init() ,   return S_OK"); // �α����Ͽ� �Լ� ������ �˸���. [2014.11.8 ]

	return S_OK;
}


//-----------------------------------------------------------------------------
// Desc   :  �׸��� �Լ�  [2014.11.8 ]
//-----------------------------------------------------------------------------
void	CFullScreenQuad::Draw()
{
	// ȭ�鰡���� �簢���� �׸���.
	m_pD3DDevice->SetStreamSource(0, m_pFullscreenQuadVB, 0, sizeof(float)*5);
	m_pD3DDevice->SetIndices(m_pFullscreenQuadIB);
	m_pD3DDevice->SetVertexDeclaration(m_pFullscreenQuadDecl);
	m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);
}

//-----------------------------------------------------------------------------
// Arg    : 
// Return : HRESULT (S_OK) , (S_FALSE)
// Desc   : �������ӿ� �׷ȴ� ��� ����� �����. 
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
// ���⼭���� ...�Լ�����
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
// ��ŷ�Լ�
// ���ӿ��ִ� ��� ������Ʈ�� �˻��� ����
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
		D3DXMATRIX View_Inv, Proj_Inv, MatInv; // view�� �����,  Proj�� �����, MatInv = proj����� * view�� �����
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
		// ���⼭���� ĳ���ͳ��� ��豸�鿡 ���� ��ŷ�˻�
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
// ��ũ�� �Լ�
// �����ʸ��콺�� �巡�� �ؼ� ȸ����Ų��.
// D3DXMATRIX* _pOut  :  ��ȯ��(ȸ���� ��Ʈ����)
//
//---------------------------------------------------------------------------------------------
/*
void ArcBall(GEOMOBJECT* _pBone )
{
	D3DXMATRIX* _pOut = &_pBone->m_ArcBallTM ;
	if(_pOut == nullptr)
		return ;

	D3DXVECTOR3 FrontVec = SHGlobal_GetCurScene_Camera->GetLookVec();
	D3DXVECTOR3 DragVec;//Drag�ؼ� ���� ����
	D3DXVECTOR3 RotAxis;
	float		RotAngle; //����
	static POINT ptFirstClick;
	POINT ptLastClick;
	static D3DXMATRIX  preTM , InvTM , WorldTM;//2014.07.18
	static bool RbuttonOnce = false;
	D3DXMATRIX RotTM;

	if(KEY_DOWN(VK_RBUTTON) && (RbuttonOnce == false))//LButtonDown �ѹ��� Ŭ��??  ������ ���� ���� ���� ȣ������� ���� ����
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
		ScreenToClient( G_hWnd, &ptLastClick );	//���콺��ǥ�޾ƿ���

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
//******************************************  ��  **************************************************//