/***********************************************************************
                      GUI_Base.cpp
			      // [2014.10.31 ]
************************************************************************/


#include "DXUT.h"
#include "GUI_Base.h"
#include "./DXUT/DXUTres.h"
//////////////////////////////////////////////////////////////////////////  [2014.11.1 ]
// 버텍스 구조체 선언

struct DXUT_SCREEN_VERTEX
{
	float x, y, z, h;
	D3DCOLOR color;
	float tu, tv;

	static DWORD FVF;
};
DWORD                   DXUT_SCREEN_VERTEX::FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

struct DXUT_SCREEN_VERTEX_UNTEX
{
	float x, y, z, h;
	D3DCOLOR color;

	static DWORD FVF;
};
DWORD                   DXUT_SCREEN_VERTEX_UNTEX::FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;


struct DXUT_SCREEN_VERTEX_10
{
	float x, y, z;
	D3DCOLORVALUE color;
	float tu, tv;
};


//////////////////////////////////////////////////////////////////////////
// 일반함수 [2014.11.2 ]
D3DCOLORVALUE D3DCOLOR_TO_D3DCOLORVALUE( D3DCOLOR c )
{
	D3DCOLORVALUE cv = { ( ( c >> 16 ) & 0xFF )/ 255.0f, ( ( c >> 8 ) & 0xFF ) / 255.0f,
						 ( c & 0xFF ) / 255.0f,	 ( ( c >> 24 ) & 0xFF ) / 255.0f 
					   };
	return cv;
}

inline int RectWidth( RECT& rc )
{
	return ( ( rc ).right - ( rc ).left );
}
inline int RectHeight( RECT& rc )
{
	return ( ( rc ).bottom - ( rc ).top );
}


//////////////////////////////////////////////////////////////////////////
// DXUT_MAX_EDITBOXLENGTH is the maximum string length allowed in edit boxes,
// including the NULL terminator.
// 
// Uniscribe does not support strings having bigger-than-16-bits length.
// This means that the string must be less than 65536 characters long,
// including the NULL terminator.
#define DXUT_MAX_EDITBOXLENGTH 0xFFFF


double                  CDXUTDialog::s_fTimeRefresh = 0.0f;
GUI_Base*				CDXUTDialog::s_pControlFocus = NULL;        // The control which has focus
GUI_Base*				CDXUTDialog::s_pControlPressed = NULL;      // The control currently pressed
CDialogRscMgr*		    CDialogRscMgr::pInstance = nullptr;	       // for single ton [2014.11.5 ]


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
// 여기서부터 class GUI_Base 함수정의  [2014.11.1 ]
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
// 생성자
//-----------------------------------------------------------------------------[2014.11.1 ]
GUI_Base::GUI_Base(CDXUTDialog* pDialog  )
{
	m_Type = DXUT_CONTROL_BUTTON;
	m_pDialog = pDialog;
	m_ID = 0;
	m_Index = 0;
	m_pUserData = NULL;

	m_bEnabled = true;
	m_bVisible = true;
	m_bMouseOver = false;
	m_bHasFocus = false;
	m_bIsDefault = false;

	m_pDialog = NULL;

	m_x = 0;
	m_y = 0;
	m_width = 0;
	m_height = 0;

	ZeroMemory( &m_rcBoundingBox, sizeof( m_rcBoundingBox ) );
}

//-----------------------------------------------------------------------------
// 소멸자
//-----------------------------------------------------------------------------[2014.11.1 ]
GUI_Base::~GUI_Base()
{
	for( int i = 0; i < m_Elements.GetSize(); ++i )
	{
		delete m_Elements[i];
	}
	m_Elements.RemoveAll();
}



//-----------------------------------------------------------------------------
// Arg    : D3DCOLOR Color 
// Return : 
// Desc   :
//----------------------------------------------------------------------------- [2014.11.1 ]
void GUI_Base::SetTextColor( D3DCOLOR Color )
{
	CDXUTElement* pElement = m_Elements.GetAt( 0 );

	if( pElement )
		pElement->FontColor.States[DXUT_STATE_NORMAL] = Color;
}


//--------------------------------------------------------------------------------------
HRESULT GUI_Base::SetElement( UINT iElement, CDXUTElement* pElement )
{
	HRESULT hr = S_OK;

	if( pElement == NULL )
		return E_INVALIDARG;

	// Make certain the array is this large
	for( UINT i = m_Elements.GetSize(); i <= iElement; i++ )
	{
		CDXUTElement* pNewElement = new CDXUTElement();
		if( pNewElement == NULL )
			return E_OUTOFMEMORY;

		hr = m_Elements.Add( pNewElement );

		if( FAILED( hr ) )
		{
			SAFE_DELETE( pNewElement );
			return hr;
		}
	}

	// Update the data
	CDXUTElement* pCurElement = m_Elements.GetAt( iElement );
	*pCurElement = *pElement;

	return S_OK;
}


//--------------------------------------------------------------------------------------
void GUI_Base::Refresh()
{
	m_bMouseOver = false;
	m_bHasFocus = false;

	for( int i = 0; i < m_Elements.GetSize(); i++ )
	{
		CDXUTElement* pElement = m_Elements.GetAt( i );
		pElement->Refresh();
	}
}


//--------------------------------------------------------------------------------------
void GUI_Base::UpdateRects()
{
	SetRect( &m_rcBoundingBox, m_x, m_y, m_x + m_width, m_y + m_height );
}

//-----------------------------------------------------------------------------
// Arg    : int ID 고유 아이디
// Return : HRESULT S_OK(성공), 실패 (S_FALSE)
// Desc   : 고유아이디이기 떄문에 다른 UI에 이미 할당되어 있는 아이디라면 실패를 반환한다. 
//-----------------------------------------------------------------------------
HRESULT	GUI_Base::SetID( int ID )
{
	CDialogRscMgr* pMgr = CDialogRscMgr::GetInstance();
	int Dialogsize = pMgr->m_Dialogs.GetSize();
	int controlSize;

	// 같은아이디가 하나라도 있는지 검사 [2014.11.5 ]
	for(int i = 0; i < Dialogsize ; ++i)
	{
		CDXUTDialog* pDlg  = pMgr->m_Dialogs.GetAt(i);
		controlSize = pDlg->m_Controls.GetSize();

		for(int ControlNum = 0 ; ControlNum < controlSize ; ControlNum++)
		{
			GUI_Base* pGB = pDlg->m_Controls.GetAt(ControlNum);

			if(ID == pGB->GetID())
			{
				SHUtil::MsgBox("이미 같은아이디를 가진 컨트롤이 있다!!!!!!!!!!! 에러고쳐!!!!!!!!!!!!!!");
				assert(false);
				return S_FALSE;
			}
		}
	}

	//같은 아이가 없다면 아이디를 넣어준다. [2014.11.5 ]
	m_ID = ID;

	return S_OK;
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
// 여기서부터 struct DXUTBlendColor 함수정의  [2014.11.1 ]
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
// Arg    : D3DCOLOR defaultColor, D3DCOLOR disabledColor, D3DCOLOR hiddenColor 
// Return : 
// Desc   : 상태에 따라 다른 색을 UI 에 적용하기 위한 초기화
//          일단 두가지상태만 (disabledColor, hiddenColor) 디폴트색과 다르게  [2014.11.1 ]
//-----------------------------------------------------------------------------
void DXUTBlendColor::Init( D3DCOLOR defaultColor, D3DCOLOR disabledColor, D3DCOLOR hiddenColor )
{
	for( int i = 0; i < DXUT_CONTROL_STATE_MAX; i++ )
	{
		States[ i ] = defaultColor;
	}

	States[ DXUT_STATE_DISABLED ] = disabledColor;
	States[ DXUT_STATE_HIDDEN ] = hiddenColor;
	Current = hiddenColor;

}



//-----------------------------------------------------------------------------
// Arg    : UINT iState, float fElapsedTime, float fRate
// Return : 
// Desc   : 실시간 랜더할때 색이 천천히 바뀌는 효과 ,,  [2014.11.1 ]
//-----------------------------------------------------------------------------
void DXUTBlendColor::Blend( UINT iState, float fElapsedTime, float fRate )
{
	D3DXCOLOR destColor = States[ iState ];

	D3DXColorLerp( &Current, &Current, &destColor, 1.0f - powf( fRate, 20 * fElapsedTime ) );
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
// 여기서부터 class CDXUTElement 함수정의  [2014.11.1 ]
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
// Desc   : 
//-----------------------------------------------------------------------------
void CDXUTElement::SetTexture( UINT iTexture, RECT* prcTexture, D3DCOLOR defaultTextureColor )
{
	this->iTexture = iTexture;

	if( prcTexture )
		rcTexture = *prcTexture;
	else
		SetRectEmpty( &rcTexture );  // 네모서리 다  0으로 만들어주는 함수 [2014.11.1 ]

	TextureColor.Init( defaultTextureColor );  // 디폴트 칼라로 초기화 [2014.11.1 ]
}


//-----------------------------------------------------------------------------
// Arg    : 
// Return : 
// Desc   :
//-----------------------------------------------------------------------------  [2014.11.1 ]
void CDXUTElement::SetFont( UINT iFont, D3DCOLOR defaultFontColor, DWORD dwTextFormat )
{
	this->iFont = iFont;
	this->dwTextFormat = dwTextFormat;

	FontColor.Init( defaultFontColor );
}


//--------------------------------------------------------------------------------------
void CDXUTElement::Refresh()
{
	TextureColor.Current = TextureColor.States[ DXUT_STATE_HIDDEN ];
	FontColor.Current = FontColor.States[ DXUT_STATE_HIDDEN ];
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
// 여기서부터 class CDXUTDialog 함수정의   [2014.11.1 ]
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
// 생성자  [2014.11.1 ]
//-----------------------------------------------------------------------------
CDXUTDialog::CDXUTDialog()
{
	m_x = 0;
	m_y = 0;
	m_width = 0;
	m_height = 0;

	m_pManager = NULL;
	m_bVisible = true;
	m_bCaption = false;
	m_bMinimized = false;
	m_bDrag = false;
	m_wszCaption[0] = L'\0';
	m_nCaptionHeight = 18;

	m_colorTopLeft =0;//D3DCOLOR_ARGB(200,200,0,0);
	m_colorTopRight = 0;
	m_colorBottomLeft = 0;
	m_colorBottomRight = 0;

	m_pCallbackEvent = NULL;
	m_pCallbackEventUserContext = NULL;

	m_fTimeLastRefresh = 0;

	m_pControlMouseOver = NULL;

	m_pNextDialog = this;
	m_pPrevDialog = this;

	m_nDefaultControlID = 0xffff;
	m_bNonUserEvents = false;
	m_bKeyboardInput = false;
	m_bMouseInput = true;
}


//--------------------------------------------------------------------------------------
CDXUTDialog::~CDXUTDialog()
{
	int i = 0;

	RemoveAllControls();

	m_Fonts.RemoveAll();
	m_Textures.RemoveAll();

	for( i = 0; i < m_DefaultElements.GetSize(); i++ )
	{
		DXUTElementHolder* pElementHolder = m_DefaultElements.GetAt( i );
		SAFE_DELETE( pElementHolder );
	}

	m_DefaultElements.RemoveAll();
}


//--------------------------------------------------------------------------------------
void CDXUTDialog::Init( CDialogRscMgr* pManager, bool bRegisterDialog )
{
	m_pManager = pManager;
	if( bRegisterDialog )
		pManager->RegisterDialog( this );

	SetTexture( 0, MAKEINTRESOURCE( 0xFFFF ), ( HMODULE )0xFFFF );
	InitDefaultElements();
}


//--------------------------------------------------------------------------------------
void CDXUTDialog::Init( CDialogRscMgr* pManager , bool bRegisterDialog , LPCWSTR pszControlTextureFilename
						, LPCSTR pszDialogName) // [2014.11.5 ]
{
	m_pManager = pManager;
	if( bRegisterDialog )
		pManager->RegisterDialog( this );

	if(pszDialogName == nullptr) 
	{
		SHUtil::MsgBox("CDXUTDialog::Init() , 이름이 nullptr임!!!!!!!!!!!!!!!");
		LOG_WRITE_A("CDXUTDialog::Init() , 이름이 nullptr임!!!!!!!!!!!!!!!");
		assert(false);
	}

	m_DlgName =  pszDialogName;
	// CustomUI 에서 제공해주는 기본텍스쳐 
	SetTexture( eTEX_ID_CustomUI, MAKEINTRESOURCE( 0xFFFF ), ( HMODULE )0xFFFF );
	// 내가만든 텍스쳐 [2014.11.3 ]
	SetTexture( eTEX_ID_SHUI , pszControlTextureFilename );

	InitDefaultElements();
}


//--------------------------------------------------------------------------------------
void CDXUTDialog::Init( CDialogRscMgr* pManager, bool bRegisterDialog,
	LPCWSTR szControlTextureResourceName, HMODULE hControlTextureResourceModule )
{
	m_pManager = pManager;
	if( bRegisterDialog )
		pManager->RegisterDialog( this );

	SetTexture( 0, szControlTextureResourceName, hControlTextureResourceModule );
	InitDefaultElements();
}


//--------------------------------------------------------------------------------------
void CDXUTDialog::SetCallback( PCALLBACKDXUTGUIEVENT pCallback, void* pUserContext )
{
	// If this assert triggers, you need to call CDXUTDialog::Init() first.  This change
	// was made so that the DXUT's GUI could become seperate and optional from DXUT's core.  The 
	// creation and interfacing with CDialogRscMgr is now the responsibility 
	// of the application if it wishes to use DXUT's GUI.
	assert( m_pManager != NULL && L"To fix call CDXUTDialog::Init() first.  See comments for details." );

	m_pCallbackEvent = pCallback;
	m_pCallbackEventUserContext = pUserContext;
}


//--------------------------------------------------------------------------------------
void CDXUTDialog::RemoveControl( int ID )
{
	for( int i = 0; i < m_Controls.GetSize(); i++ )
	{
		GUI_Base* pControl = m_Controls.GetAt( i );
		if( pControl->GetID() == ID )
		{
			// Clean focus first
			ClearFocus();

			// Clear references to this control
			if( s_pControlFocus == pControl )
				s_pControlFocus = NULL;
			if( s_pControlPressed == pControl )
				s_pControlPressed = NULL;
			if( m_pControlMouseOver == pControl )
				m_pControlMouseOver = NULL;

			SAFE_DELETE( pControl );
			m_Controls.Remove( i );

			return;
		}
	}
}


//--------------------------------------------------------------------------------------
void CDXUTDialog::RemoveAllControls()
{
	if( s_pControlFocus && s_pControlFocus->m_pDialog == this )
		s_pControlFocus = NULL;
	if( s_pControlPressed && s_pControlPressed->m_pDialog == this )
		s_pControlPressed = NULL;
	m_pControlMouseOver = NULL;

	for( int i = 0; i < m_Controls.GetSize(); i++ )
	{
		GUI_Base* pControl = m_Controls.GetAt( i );
		SAFE_DELETE( pControl );
	}

	m_Controls.RemoveAll();
}


//--------------------------------------------------------------------------------------
void CDXUTDialog::Refresh()
{
	if( s_pControlFocus )
		s_pControlFocus->OnFocusOut();

	if( m_pControlMouseOver )
		m_pControlMouseOver->OnMouseLeave();

	s_pControlFocus = NULL;
	s_pControlPressed = NULL;
	m_pControlMouseOver = NULL;

	for( int i = 0; i < m_Controls.GetSize(); i++ )
	{
		GUI_Base* pControl = m_Controls.GetAt( i );
		pControl->Refresh();
	}

	if( m_bKeyboardInput )
		FocusDefaultControl();
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTDialog::OnRender( float fElapsedTime )
{
	if( m_pManager->GetD3D9Device() )
		return OnRender9( fElapsedTime );
	else
		return OnRender10( fElapsedTime );
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTDialog::OnRender9( float fElapsedTime )
{
	// If this assert triggers, you need to call CDialogRscMgr::On*Device() from inside
	// the application's device callbacks.  See the SDK samples for an example of how to do this.
	assert( m_pManager->GetD3D9Device() && m_pManager->m_pStateBlock &&
		L"To fix hook up CDialogRscMgr to device callbacks.  See comments for details" );

	// See if the dialog needs to be refreshed
	if( m_fTimeLastRefresh < s_fTimeRefresh )
	{
		m_fTimeLastRefresh = DXUTGetTime();
		Refresh();
	}

	// For invisible dialog, out now.
	if( !m_bVisible ||	( m_bMinimized && !m_bCaption ) )
		return S_OK;

	IDirect3DDevice9* pd3dDevice = m_pManager->GetD3D9Device();

	// Set up a state block here and restore it when finished drawing all the controls
	m_pManager->m_pStateBlock->Capture();

	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	pd3dDevice->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, FALSE );
	pd3dDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
	pd3dDevice->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_BLUE |
								D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED );
	pd3dDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
	pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );
	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_RESULTARG, D3DTA_CURRENT );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

	BOOL bBackgroundIsVisible = ( m_colorTopLeft | m_colorTopRight | m_colorBottomRight | m_colorBottomLeft ) &
		0xff000000;
	if( !m_bMinimized && bBackgroundIsVisible )
	{
		DXUT_SCREEN_VERTEX_UNTEX vertices[4] =
		{
			( float )m_x,           ( float )m_y,            0.5f, 1.0f, m_colorTopLeft,
			( float )m_x + m_width, ( float )m_y,            0.5f, 1.0f, m_colorTopRight,
			( float )m_x + m_width, ( float )m_y + m_height, 0.5f, 1.0f, m_colorBottomRight,
			( float )m_x,           ( float )m_y + m_height, 0.5f, 1.0f, m_colorBottomLeft,
		};

		pd3dDevice->SetVertexShader( NULL );
		pd3dDevice->SetPixelShader( NULL );

		pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );

		pd3dDevice->SetFVF( DXUT_SCREEN_VERTEX_UNTEX::FVF );
		pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, vertices, sizeof( DXUT_SCREEN_VERTEX_UNTEX ) );
	}

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

	pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );

	DXUTTextureNode* pTextureNode = GetTexture( 0 );
	pd3dDevice->SetTexture( 0, pTextureNode->pTexture9 );

	m_pManager->m_pSprite->Begin( D3DXSPRITE_DONOTSAVESTATE );

	// Render the caption if it's enabled.
	if( m_bCaption )
	{
		// DrawSprite will offset the rect down by
		// m_nCaptionHeight, so adjust the rect higher
		// here to negate the effect.
		RECT rc = { 0, -m_nCaptionHeight, m_width, 0 };
		DrawSprite9( &m_CapElement, &rc );
		rc.left += 5; // Make a left margin
		WCHAR wszOutput[256];
		wcscpy_s( wszOutput, 256, m_wszCaption );
		if( m_bMinimized )
			wcscat_s( wszOutput, 256, L" (Minimized)" );
		DrawText9( wszOutput, &m_CapElement, &rc, true );
	}

	// If the dialog is minimized, skip rendering
	// its controls.
	if( !m_bMinimized )
	{
		for( int i = 0; i < m_Controls.GetSize(); i++ )
		{
			GUI_Base* pControl = m_Controls.GetAt( i );

			// Focused control is drawn last
			if( pControl == s_pControlFocus )
				continue;

			pControl->Render( fElapsedTime );
		}

		if( s_pControlFocus != NULL && s_pControlFocus->m_pDialog == this )
			s_pControlFocus->Render( fElapsedTime );
	}

	m_pManager->m_pSprite->End();

	m_pManager->m_pStateBlock->Apply();

	return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTDialog::OnRender10( float fElapsedTime )
{
	// If this assert triggers, you need to call CDialogRscMgr::On*Device() from inside
	// the application's device callbacks.  See the SDK samples for an example of how to do this.
	assert( m_pManager->GetD3D10Device() &&
		L"To fix hook up CDialogRscMgr to device callbacks.  See comments for details" );

	// See if the dialog needs to be refreshed
	if( m_fTimeLastRefresh < s_fTimeRefresh )
	{
		m_fTimeLastRefresh = DXUTGetTime();
		Refresh();
	}

	// For invisible dialog, out now.
	if( !m_bVisible ||
		( m_bMinimized && !m_bCaption ) )
		return S_OK;

	ID3D10Device* pd3dDevice = m_pManager->GetD3D10Device();

	// Set up a state block here and restore it when finished drawing all the controls
	m_pManager->m_pStateBlock10->Capture();

	BOOL bBackgroundIsVisible = ( m_colorTopLeft | m_colorTopRight | m_colorBottomRight | m_colorBottomLeft ) &
		0xff000000;
	if( !m_bMinimized && bBackgroundIsVisible )
	{
		// Convert the draw rectangle from screen coordinates to clip space coordinates.
		float Left, Right, Top, Bottom;
		Left = m_x * 2.0f / m_pManager->m_nBackBufferWidth - 1.0f;
		Right = ( m_x + m_width ) * 2.0f / m_pManager->m_nBackBufferWidth - 1.0f;
		Top = 1.0f - m_y * 2.0f / m_pManager->m_nBackBufferHeight;
		Bottom = 1.0f - ( m_y + m_height ) * 2.0f / m_pManager->m_nBackBufferHeight;

		DXUT_SCREEN_VERTEX_10 vertices[4] =
		{
			Left,  Top,    0.5f, D3DCOLOR_TO_D3DCOLORVALUE( m_colorTopLeft ), 0.0f, 0.0f,
			Right, Top,    0.5f, D3DCOLOR_TO_D3DCOLORVALUE( m_colorTopRight ), 1.0f, 0.0f,
			Left,  Bottom, 0.5f, D3DCOLOR_TO_D3DCOLORVALUE( m_colorBottomLeft ), 0.0f, 1.0f,
			Right, Bottom, 0.5f, D3DCOLOR_TO_D3DCOLORVALUE( m_colorBottomRight ), 1.0f, 1.0f,
		};

		DXUT_SCREEN_VERTEX_10* pVB;
		if( SUCCEEDED( m_pManager->m_pVBScreenQuad10->Map( D3D10_MAP_WRITE_DISCARD, 0, ( LPVOID* )&pVB ) ) )
		{
			CopyMemory( pVB, vertices, sizeof( vertices ) );
			m_pManager->m_pVBScreenQuad10->Unmap();
		}

		// Set the quad VB as current
		UINT stride = sizeof( DXUT_SCREEN_VERTEX_10 );
		UINT offset = 0;
		pd3dDevice->IASetVertexBuffers( 0, 1, &m_pManager->m_pVBScreenQuad10, &stride, &offset );
		pd3dDevice->IASetInputLayout( m_pManager->m_pInputLayout10 );
		pd3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

		// Draw dialog background
		D3D10_TECHNIQUE_DESC techDesc;
		m_pManager->m_pTechRenderUIUntex10->GetDesc( &techDesc );
		for( UINT p = 0; p < techDesc.Passes; ++p )
		{
			m_pManager->m_pTechRenderUIUntex10->GetPassByIndex( p )->Apply( 0 );
			pd3dDevice->Draw( 4, 0 );
		}
	}

	DXUTTextureNode* pTextureNode = GetTexture( 0 );
	m_pManager->m_pFxTexture10->SetResource( pTextureNode->pTexResView );

	// Sort depth back to front
	m_pManager->m_pSprite10->Begin( 0 );

	m_pManager->m_pTechRenderUI10->GetPassByIndex( 0 )->Apply( 0 );

	// Render the caption if it's enabled.
	if( m_bCaption )
	{
		// DrawSprite will offset the rect down by
		// m_nCaptionHeight, so adjust the rect higher
		// here to negate the effect.
		RECT rc = { 0, -m_nCaptionHeight, m_width, 0 };
		DrawSprite10( &m_CapElement, &rc, 0.99f );
		rc.left += 5; // Make a left margin
		WCHAR wszOutput[256];
		wcscpy_s( wszOutput, 256, m_wszCaption );
		if( m_bMinimized )
			wcscat_s( wszOutput, 256, L" (Minimized)" );
		DrawText10( wszOutput, &m_CapElement, &rc, true );
	}

	// If the dialog is minimized, skip rendering
	// its controls.
	if( !m_bMinimized )
	{
		for( int i = 0; i < m_Controls.GetSize(); i++ )
		{
			GUI_Base* pControl = m_Controls.GetAt( i );

			// Focused control is drawn last
			if( pControl == s_pControlFocus )
				continue;

			pControl->Render( fElapsedTime );
		}

		if( s_pControlFocus != NULL && s_pControlFocus->m_pDialog == this )
			s_pControlFocus->Render( fElapsedTime );
	}

	m_pManager->m_pSprite10->End();

	m_pManager->m_pStateBlock10->Apply();
	// Restore depth stencil state
	m_pManager->m_pEffect10->GetTechniqueByName( "RestoreState" )->GetPassByIndex( 0 )->Apply( 0 );

	return S_OK;
}

//--------------------------------------------------------------------------------------
VOID CDXUTDialog::SendEvent( UINT nEvent, bool bTriggeredByUser, GUI_Base* pControl )
{
	// If no callback has been registered there's nowhere to send the event to
	if( m_pCallbackEvent == NULL )
		return;

	// Discard events triggered programatically if these types of events haven't been
	// enabled
	if( !bTriggeredByUser && !m_bNonUserEvents )
		return;

	m_pCallbackEvent( nEvent, pControl->GetID(), pControl, m_pCallbackEventUserContext );
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTDialog::SetFont( UINT index, LPCWSTR strFaceName, LONG height, LONG weight )
{
	// If this assert triggers, you need to call CDXUTDialog::Init() first.  This change
	// was made so that the DXUT's GUI could become seperate and optional from DXUT's core.  The 
	// creation and interfacing with CDialogRscMgr is now the responsibility 
	// of the application if it wishes to use DXUT's GUI.
	assert( m_pManager != NULL && L"To fix call CDXUTDialog::Init() first.  See comments for details." );

	// Make sure the list is at least as large as the index being set
	UINT i;
	for( i = m_Fonts.GetSize(); i <= index; i++ )
	{
		m_Fonts.Add( -1 );
	}

	int iFont = m_pManager->AddFont( strFaceName, height, weight );
	m_Fonts.SetAt( index, iFont );

	return S_OK;
}


//--------------------------------------------------------------------------------------
DXUTFontNode* CDXUTDialog::GetFont( UINT index )
{
	if( NULL == m_pManager )
		return NULL;
	return m_pManager->GetFontNode( m_Fonts.GetAt( index ) );
}



//--------------------------------------------------------------------------------------
HRESULT CDXUTDialog::SetTexture( UINT index, LPCWSTR strFilename )
{
	// If this assert triggers, you need to call CDXUTDialog::Init() first.  This change
	// was made so that the DXUT's GUI could become seperate and optional from DXUT's core.  The 
	// creation and interfacing with CDialogRscMgr is now the responsibility 
	// of the application if it wishes to use DXUT's GUI.
	assert( m_pManager != NULL && L"To fix this, call CDXUTDialog::Init() first.  See comments for details." );

	// Make sure the list is at least as large as the index being set
	for( UINT i = m_Textures.GetSize(); i <= index; i++ )
	{
		m_Textures.Add( -1 );
	}

	int iTexture = m_pManager->AddTexture( strFilename );

	m_Textures.SetAt( index, iTexture );
	return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTDialog::SetTexture( UINT index, LPCWSTR strResourceName, HMODULE hResourceModule )
{
	// If this assert triggers, you need to call CDXUTDialog::Init() first.  This change
	// was made so that the DXUT's GUI could become seperate and optional from DXUT's core.  The 
	// creation and interfacing with CDialogRscMgr is now the responsibility 
	// of the application if it wishes to use DXUT's GUI.
	assert( m_pManager != NULL && L"To fix this, call CDXUTDialog::Init() first.  See comments for details." );

	// Make sure the list is at least as large as the index being set
	for( UINT i = m_Textures.GetSize(); i <= index; i++ )
	{
		m_Textures.Add( -1 );
	}
	// 반환값 iTexture는 DialogRscManager에 등록한 리소스 순서이다. 이 순서가 TexID가 된다. 
	int iTexture = m_pManager->AddTexture( strResourceName, hResourceModule );// 리소스 이름과  리소스모듈 핸들을 인자로 전달한다. [2014.11.3 ]

	m_Textures.SetAt( index, iTexture );
	return S_OK;
}


//--------------------------------------------------------------------------------------
DXUTTextureNode* CDXUTDialog::GetTexture( UINT index )
{
	if( NULL == m_pManager )
		return NULL;
	if(m_Textures.GetSize() < 0)
	{	return NULL;}

	if(index >= (UINT)m_Textures.GetSize())
	{	
		SHUtil::MsgBox("CDXUTDialog::GetTexture( UINT index ), Fail!! ,텍스쳐인덱스가 잘못되었소");
		LOG_WRITE_A("CDXUTDialog::GetTexture( UINT index ), Fail!! ,텍스쳐인덱스가 잘못되었소");
		return nullptr;
	}
	return m_pManager->GetTextureNode( m_Textures.GetAt( index ) );
}



//--------------------------------------------------------------------------------------
bool CDXUTDialog::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	bool bHandled = false;

	// For invisible dialog, do not handle anything.
	if( !m_bVisible )
		return false;

	// If automation command-line switch is on, enable this dialog's keyboard input
	// upon any key press or mouse click.
	if( DXUTGetAutomation() &&
		( WM_LBUTTONDOWN == uMsg || WM_LBUTTONDBLCLK == uMsg || WM_KEYDOWN == uMsg ) )
	{
		m_pManager->EnableKeyboardInputForAllDialogs();
	}

	// If caption is enable, check for clicks in the caption area.
	if( m_bCaption )
	{
		if( uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK )
		{
			POINT mousePoint = { short( LOWORD( lParam ) ), short( HIWORD( lParam ) ) };

			if( mousePoint.x >= m_x && mousePoint.x < m_x + m_width &&
				mousePoint.y >= m_y && mousePoint.y < m_y + m_nCaptionHeight )
			{
				m_bDrag = true;
				SetCapture( DXUTGetHWND() );
				return true;
			}
		}
		else if( uMsg == WM_LBUTTONUP && m_bDrag )
		{
			POINT mousePoint = { short( LOWORD( lParam ) ), short( HIWORD( lParam ) ) };

			if( mousePoint.x >= m_x && mousePoint.x < m_x + m_width &&
				mousePoint.y >= m_y && mousePoint.y < m_y + m_nCaptionHeight )
			{
				ReleaseCapture();
				m_bDrag = false;
				m_bMinimized = !m_bMinimized;
				return true;
			}
		}
	}

	// If the dialog is minimized, don't send any messages to controls.
	if( m_bMinimized )
		return false;

	// If a control is in focus, it belongs to this dialog, and it's enabled, then give
	// it the first chance at handling the message.  [2014.11.2 ]
	if( s_pControlFocus && (s_pControlFocus->m_pDialog == this) && s_pControlFocus->GetEnabled() )
	{
		// If the control MsgProc handles it, then we don't.
		if( s_pControlFocus->MsgProc( uMsg, wParam, lParam ) )
			return true;
	}

	switch( uMsg )
	{
	case WM_SIZE:
	case WM_MOVE:
		{
			// Handle sizing and moving messages so that in case the mouse cursor is moved out
			// of an UI control because of the window adjustment, we can properly
			// unhighlight the highlighted control.
			POINT pt = { -1, -1 };
			OnMouseMove( pt );
			break;
		}

	case WM_ACTIVATEAPP:
		// Call OnFocusIn()/OnFocusOut() of the control that currently has the focus
		// as the application is activated/deactivated.  This matches the Windows
		// behavior.
		if( s_pControlFocus &&
			s_pControlFocus->m_pDialog == this &&
			s_pControlFocus->GetEnabled() )
		{
			if( wParam )
				s_pControlFocus->OnFocusIn();
			else
				s_pControlFocus->OnFocusOut();
		}
		break;

		// Keyboard messages
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		{
			// If a control is in focus, it belongs to this dialog, and it's enabled, then give
			// it the first chance at handling the message.
			if( s_pControlFocus &&	(s_pControlFocus->m_pDialog == this ) && s_pControlFocus->GetEnabled() )
			{
				if( s_pControlFocus->HandleKeyboard( uMsg, wParam, lParam ) )
					return true;
			}

			// Not yet handled, see if this matches a control's hotkey
			// Activate the hotkey if the focus doesn't belong to an
			// edit box.
			if( uMsg == WM_KEYDOWN && ( !s_pControlFocus ||
				( s_pControlFocus->GetType() != DXUT_CONTROL_EDITBOX
				&& s_pControlFocus->GetType() != DXUT_CONTROL_IMEEDITBOX ) ) )
			{
				for( int i = 0; i < m_Controls.GetSize(); i++ )
				{
					GUI_Base* pControl = m_Controls.GetAt( i );
					if( pControl->GetHotkey() == wParam )
					{
						pControl->OnHotkey();
						return true;
					}
				}
			}

			// Not yet handled, check for focus messages
			if( uMsg == WM_KEYDOWN )
			{
				// If keyboard input is not enabled, this message should be ignored
				if( !m_bKeyboardInput )
					return false;

				switch( wParam )
				{
				case VK_RIGHT:
				case VK_DOWN:
					if( s_pControlFocus != NULL )
					{
						return OnCycleFocus( true );
					}
					break;

				case VK_LEFT:
				case VK_UP:
					if( s_pControlFocus != NULL )
					{
						return OnCycleFocus( false );
					}
					break;

				case VK_TAB:
					{
						bool bShiftDown = ( ( GetKeyState( VK_SHIFT ) & 0x8000 ) != 0 );
						return OnCycleFocus( !bShiftDown );
					}
				}
			}

			break;
		}


		// Mouse messages
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_XBUTTONDBLCLK:
	case WM_MOUSEWHEEL:
		{
			// If not accepting mouse input, return false to indicate the message should still 
			// be handled by the application (usually to move the camera).
			if( !m_bMouseInput )
				return false;

			POINT mousePoint = { short( LOWORD( lParam ) ), short( HIWORD( lParam ) ) };
			mousePoint.x -= m_x;
			mousePoint.y -= m_y;

			// If caption is enabled, offset the Y coordinate by the negative of its height.
			if( m_bCaption )
				mousePoint.y -= m_nCaptionHeight;

			// If a control is in focus, it belongs to this dialog, and it's enabled, then give
			// it the first chance at handling the message.
			if( s_pControlFocus &&
				s_pControlFocus->m_pDialog == this &&
				s_pControlFocus->GetEnabled() )
			{
				if( s_pControlFocus->HandleMouse( uMsg, mousePoint, wParam, lParam ) )
					return true;
			}

			// Not yet handled, see if the mouse is over any controls
			GUI_Base* pControl = GetControlAtPoint( mousePoint );
			if( pControl != NULL && pControl->GetEnabled() )
			{
				bHandled = pControl->HandleMouse( uMsg, mousePoint, wParam, lParam );
				if( bHandled )
					return true;
			}
			else
			{
				// Mouse not over any controls in this dialog, if there was a control
				// which had focus it just lost it
				if( uMsg == WM_LBUTTONDOWN &&
					s_pControlFocus &&
					s_pControlFocus->m_pDialog == this )
				{
					s_pControlFocus->OnFocusOut();
					s_pControlFocus = NULL;
				}
			}

			// Still not handled, hand this off to the dialog. Return false to indicate the
			// message should still be handled by the application (usually to move the camera).
			switch( uMsg )
			{
			case WM_MOUSEMOVE:
				OnMouseMove( mousePoint );
				return false;
			}

			break;
		}

	case WM_CAPTURECHANGED:
		{
			// The application has lost mouse capture.
			// The dialog object may not have received
			// a WM_MOUSEUP when capture changed. Reset
			// m_bDrag so that the dialog does not mistakenly
			// think the mouse button is still held down.
			if( ( HWND )lParam != hWnd )
				m_bDrag = false;
		}
	}

	return false;
}

//--------------------------------------------------------------------------------------
GUI_Base* CDXUTDialog::GetControlAtPoint( POINT pt )
{
	// Search through all child controls for the first one which
	// contains the mouse point
	for( int i = 0; i < m_Controls.GetSize(); i++ )
	{
		GUI_Base* pControl = m_Controls.GetAt( i );

		if( pControl == NULL )
		{
			continue;
		}

		// We only return the current control if it is visible
		// and enabled.  Because GetControlAtPoint() is used to do mouse
		// hittest, it makes sense to perform this filtering.
		if( pControl->ContainsPoint( pt ) && pControl->GetEnabled() && pControl->GetVisible() )
		{
			return pControl;
		}
	}

	return NULL;
}


//--------------------------------------------------------------------------------------
bool CDXUTDialog::GetControlEnabled( int ID )
{
	GUI_Base* pControl = GetControl( ID );
	if( pControl == NULL )
		return false;

	return pControl->GetEnabled();
}



//--------------------------------------------------------------------------------------
void CDXUTDialog::SetControlEnabled( int ID, bool bEnabled )
{
	GUI_Base* pControl = GetControl( ID );
	if( pControl == NULL )
		return;

	pControl->SetEnabled( bEnabled );
}


//--------------------------------------------------------------------------------------
void CDXUTDialog::OnMouseUp( POINT pt )
{
	s_pControlPressed = NULL;
	m_pControlMouseOver = NULL;
}


//--------------------------------------------------------------------------------------
void CDXUTDialog::OnMouseMove( POINT pt )
{
	// Figure out which control the mouse is over now
	GUI_Base* pControl = GetControlAtPoint( pt );

	// If the mouse is still over the same control, nothing needs to be done
	if( pControl == m_pControlMouseOver )
		return;

	// Handle mouse leaving the old control
	if( m_pControlMouseOver )
		m_pControlMouseOver->OnMouseLeave();

	// Handle mouse entering the new control
	m_pControlMouseOver = pControl;
	if( pControl != NULL )
		m_pControlMouseOver->OnMouseEnter();
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTDialog::SetDefaultElement( UINT nControlType, UINT iElement, CDXUTElement* pElement )
{
	// If this Element type already exist in the list, simply update the stored Element
	for( int i = 0; i < m_DefaultElements.GetSize(); i++ )
	{
		DXUTElementHolder* pElementHolder = m_DefaultElements.GetAt( i );

		if( (pElementHolder->nControlType == nControlType)  && 	(pElementHolder->iElement == iElement) )
		{
			pElementHolder->Element = *pElement;
			return S_OK;
		}
	}

	// Otherwise, add a new entry
	DXUTElementHolder* pNewHolder;
	pNewHolder = new DXUTElementHolder;
	if( pNewHolder == NULL )
		return E_OUTOFMEMORY;

	pNewHolder->nControlType = nControlType;
	pNewHolder->iElement = iElement;
	pNewHolder->Element = *pElement;

	HRESULT hr = m_DefaultElements.Add( pNewHolder );
	if( FAILED( hr ) )
	{
		delete pNewHolder;
	}
	return hr;
}


//--------------------------------------------------------------------------------------
CDXUTElement* CDXUTDialog::GetDefaultElement( UINT nControlType, UINT iElement )
{
	for( int i = 0; i < m_DefaultElements.GetSize(); i++ )
	{
		DXUTElementHolder* pElementHolder = m_DefaultElements.GetAt( i );

		if( pElementHolder->nControlType == nControlType &&
			pElementHolder->iElement == iElement )
		{
			return &pElementHolder->Element;
		}
	}

	return NULL;
}



//--------------------------------------------------------------------------------------
HRESULT CDXUTDialog::AddStatic( int ID, LPCWSTR strText, int x, int y, int width, int height,
								bool bIsDefault,CDXUTStatic** ppCreated )
{
	HRESULT hr = S_OK;

	CDXUTStatic* pStatic = new CDXUTStatic( this );  // [2014.11.1 ]

	if( ppCreated != NULL )
		*ppCreated = pStatic;

	if( pStatic == NULL )
		return E_OUTOFMEMORY;

	hr = AddControl( pStatic );
	if( FAILED( hr ) )
		return hr;

	// Set the ID and list index
	pStatic->SetID( ID );
	pStatic->SetText( strText );
	pStatic->SetLocation( x, y );
	pStatic->SetSize( width, height );
	pStatic->m_bIsDefault = bIsDefault;

	return S_OK;
}

//-----------------------------------------------------------------------------
// Arg    : 
// Return : 
// Desc   : //[2014.12.4 ]
//-----------------------------------------------------------------------------
HRESULT		CDXUTDialog::AddImage( int ID, LPCWSTR strText, int x, int y, int width, int height, int _Ele_Idx_in, int _Ele_Idx_Out,
								   bool bIsDefault/*=false*/,   CDXUTStatic** ppCreated/*=NULL*/ )
{
	HRESULT hr = S_OK;
	CDXUTImage* pImg = new CDXUTImage( this );
	if( ppCreated != NULL )
		*ppCreated = pImg;

	if( pImg == NULL )
		return E_OUTOFMEMORY;

	hr = AddControl( pImg );
	if( FAILED( hr ) )
		return hr;
	
	// Set the ID and list index
	pImg->m_Ele_Idx_in  = _Ele_Idx_in;
	pImg->m_Ele_Idx_Out  = _Ele_Idx_Out;
	pImg->SetID( ID );
	pImg->SetText( strText );
	pImg->SetLocation( x, y );
	pImg->SetSize( width, height );

	pImg->m_bIsDefault = bIsDefault;

	return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTDialog::AddButton( int ID, LPCWSTR strText, int x, int y, int width, int height, int _Ele_Idx_in, int _Ele_Idx_Out,
								UINT nHotkey, bool bIsDefault, CDXUTButton** ppCreated )
{
	HRESULT hr = S_OK;

	CDXUTButton* pButton = new CDXUTButton( this );

	if( ppCreated != NULL )
		*ppCreated = pButton;

	if( pButton == NULL )
		return E_OUTOFMEMORY;

	hr = AddControl( pButton );
	if( FAILED( hr ) )
		return hr;

	// Set the ID and list index
	pButton->m_Ele_Idx_in  = _Ele_Idx_in;
	pButton->m_Ele_Idx_Out  = _Ele_Idx_Out;
	pButton->SetID( ID );
	pButton->SetText( strText );
	pButton->SetLocation( x, y );
	pButton->SetSize( width, height );
	pButton->SetHotkey( nHotkey );
	pButton->m_bIsDefault = bIsDefault;

	return S_OK;
}


//--------------------------------------------------------------------------------------
// HRESULT CDXUTDialog::AddCheckBox( int ID, LPCWSTR strText, int x, int y, int width, int height, bool bChecked,
// 	UINT nHotkey, bool bIsDefault, CDXUTCheckBox** ppCreated )
// {
// 	HRESULT hr = S_OK;
// 
// 	CDXUTCheckBox* pCheckBox = new CDXUTCheckBox( this );
// 
// 	if( ppCreated != NULL )
// 		*ppCreated = pCheckBox;
// 
// 	if( pCheckBox == NULL )
// 		return E_OUTOFMEMORY;
// 
// 	hr = AddControl( pCheckBox );
// 	if( FAILED( hr ) )
// 		return hr;
// 
// 	// Set the ID and list index
// 	pCheckBox->SetID( ID );
// 	pCheckBox->SetText( strText );
// 	pCheckBox->SetLocation( x, y );
// 	pCheckBox->SetSize( width, height );
// 	pCheckBox->SetHotkey( nHotkey );
// 	pCheckBox->m_bIsDefault = bIsDefault;
// 	pCheckBox->SetChecked( bChecked );
// 
// 	return S_OK;
// }



//--------------------------------------------------------------------------------------
// HRESULT CDXUTDialog::AddRadioButton( int ID, UINT nButtonGroup, LPCWSTR strText, int x, int y, int width, int height,
// 	bool bChecked, UINT nHotkey, bool bIsDefault, CDXUTRadioButton** ppCreated )
// {
// 	HRESULT hr = S_OK;
// 
// 	CDXUTRadioButton* pRadioButton = new CDXUTRadioButton( this );
// 
// 	if( ppCreated != NULL )
// 		*ppCreated = pRadioButton;
// 
// 	if( pRadioButton == NULL )
// 		return E_OUTOFMEMORY;
// 
// 	hr = AddControl( pRadioButton );
// 	if( FAILED( hr ) )
// 		return hr;
// 
// 	// Set the ID and list index
// 	pRadioButton->SetID( ID );
// 	pRadioButton->SetText( strText );
// 	pRadioButton->SetButtonGroup( nButtonGroup );
// 	pRadioButton->SetLocation( x, y );
// 	pRadioButton->SetSize( width, height );
// 	pRadioButton->SetHotkey( nHotkey );
// 	pRadioButton->SetChecked( bChecked );
// 	pRadioButton->m_bIsDefault = bIsDefault;
// 	pRadioButton->SetChecked( bChecked );
// 
// 	return S_OK;
// }




//--------------------------------------------------------------------------------------
// HRESULT CDXUTDialog::AddComboBox( int ID, int x, int y, int width, int height, UINT nHotkey, bool bIsDefault,
// 	CDXUTComboBox** ppCreated )
// {
// 	HRESULT hr = S_OK;
// 
// 	CDXUTComboBox* pComboBox = new CDXUTComboBox( this );
// 
// 	if( ppCreated != NULL )
// 		*ppCreated = pComboBox;
// 
// 	if( pComboBox == NULL )
// 		return E_OUTOFMEMORY;
// 
// 	hr = AddControl( pComboBox );
// 	if( FAILED( hr ) )
// 		return hr;
// 
// 	// Set the ID and list index
// 	pComboBox->SetID( ID );
// 	pComboBox->SetLocation( x, y );
// 	pComboBox->SetSize( width, height );
// 	pComboBox->SetHotkey( nHotkey );
// 	pComboBox->m_bIsDefault = bIsDefault;
// 
// 	return S_OK;
// }
// 


//--------------------------------------------------------------------------------------
HRESULT CDXUTDialog::AddSlider( int ID, int x, int y, int width, int height, int Ele_Stick, int Ele_Btn,
								int min, int max, int value, bool bIsDefault, CDXUTSlider** ppCreated )
{
	HRESULT hr = S_OK;

	CDXUTSlider* pSlider = new CDXUTSlider( this );

	if( ppCreated != NULL )
		*ppCreated = pSlider;

	if( pSlider == NULL )
		return E_OUTOFMEMORY;

	hr = AddControl( pSlider );
	if( FAILED( hr ) )
		return hr;

	pSlider->m_Ele_Stick = Ele_Stick;
	pSlider->m_Ele_Btn = Ele_Btn; 

	// Set the ID and list index
	pSlider->SetID( ID );
	pSlider->SetLocation( x, y );
	pSlider->SetSize( width, height );
	pSlider->m_bIsDefault = bIsDefault;
	pSlider->SetRange( min, max );
	pSlider->SetValue( value );
	pSlider->UpdateRects();

	return S_OK;
}
// 


//--------------------------------------------------------------------------------------
// HRESULT CDXUTDialog::AddEditBox( int ID, LPCWSTR strText, int x, int y, int width, int height, bool bIsDefault,
// 	CDXUTEditBox** ppCreated )
// {
// 	HRESULT hr = S_OK;
// 
// 	CDXUTEditBox* pEditBox = new CDXUTEditBox( this );
// 
// 	if( ppCreated != NULL )
// 		*ppCreated = pEditBox;
// 
// 	if( pEditBox == NULL )
// 		return E_OUTOFMEMORY;
// 
// 	hr = AddControl( pEditBox );
// 	if( FAILED( hr ) )
// 		return hr;
// 
// 	// Set the ID and position
// 	pEditBox->SetID( ID );
// 	pEditBox->SetLocation( x, y );
// 	pEditBox->SetSize( width, height );
// 	pEditBox->m_bIsDefault = bIsDefault;
// 
// 	if( strText )
// 		pEditBox->SetText( strText );
// 
// 	return S_OK;
// }


//--------------------------------------------------------------------------------------
// HRESULT CDXUTDialog::AddListBox( int ID, int x, int y, int width, int height, DWORD dwStyle, CDXUTListBox** ppCreated )
// {
// 	HRESULT hr = S_OK;
// 	CDXUTListBox* pListBox = new CDXUTListBox( this );
// 
// 	if( ppCreated != NULL )
// 		*ppCreated = pListBox;
// 
// 	if( pListBox == NULL )
// 		return E_OUTOFMEMORY;
// 
// 	hr = AddControl( pListBox );
// 	if( FAILED( hr ) )
// 		return hr;
// 
// 	// Set the ID and position
// 	pListBox->SetID( ID );
// 	pListBox->SetLocation( x, y );
// 	pListBox->SetSize( width, height );
// 	pListBox->SetStyle( dwStyle );
// 
// 	return S_OK;
// }



//-----------------------------------------------------------------------------
// Arg    : int ID( 고유아이디), int x, int y,(화면상의 좌표) int width, int height(UI 의 가로세로 크기),
//			bool bIsDefault/*=false*/(bIsDefault 이게 true면 다른 UI가 선택되면 포커스를 잃는다.),
//			CDXUTButton** ppCreated( 이미 만들어진 UI의 포인터를 전달해서 다이얼로그에 등록하고 싶을때 사용)
// Return : HRESULT (성공 = S_OK) (실패 = S_FALSE)
// Desc   : 프로그레스바를 다이얼로그에 등록한다. 
//-----------------------------------------------------------------------------// [2014.11.5 ]
HRESULT	CDXUTDialog::AddProgressBar(int ID, int x, int y, int width, int height, int Idx_In, int Idx_Out ,bool bIsDefault/*=false*/,
									CDXUTProgressBar** ppCreated/*=NULL */ )
{
	HRESULT hr = S_OK;


	CDXUTProgressBar* pProgBar = new CDXUTProgressBar( this );
	pProgBar->Set_EleIdx_In(Idx_In);
	pProgBar->Set_EleIdx_Out(Idx_Out);

	if( ppCreated != NULL )
		*ppCreated = pProgBar;

	if( pProgBar == NULL )
		return E_OUTOFMEMORY;

	hr = AddControl( pProgBar );
	if( FAILED( hr ) )
		return hr;

	// Set the ID and list index
	hr = pProgBar->SetID( ID );
	pProgBar->SetLocation( x, y );
	pProgBar->SetSize( width, height );
	pProgBar->m_bIsDefault = bIsDefault;



	return hr;
}

//-----------------------------------------------------------------------------
// Arg    : 
// Return : 
// Desc   : 컨트롤을 초기화한다.
//-----------------------------------------------------------------------------
HRESULT CDXUTDialog::InitControl( GUI_Base* pControl )
{
	HRESULT hr;

	if( pControl == NULL )
		return E_INVALIDARG;

	pControl->m_Index = m_Controls.GetSize();

	// Look for a default Element entries
	for( int i = 0; i < m_DefaultElements.GetSize(); i++ )
	{
		DXUTElementHolder* pElementHolder = m_DefaultElements.GetAt( i );
		if( pElementHolder->nControlType == pControl->GetType() )
			pControl->SetElement( pElementHolder->iElement, &pElementHolder->Element );
	}

	V_RETURN( pControl->OnInit() );

	return S_OK;
}



//--------------------------------------------------------------------------------------
HRESULT CDXUTDialog::AddControl( GUI_Base* pControl )
{
	HRESULT hr = S_OK;

	hr = InitControl( pControl );
	if( FAILED( hr ) )
		return DXTRACE_ERR( L"CDXUTDialog::InitControl", hr );

	// Add to the list
	hr = m_Controls.Add( pControl );
	if( FAILED( hr ) )
	{
		return DXTRACE_ERR( L"CGrowableArray::Add", hr );
	}

	return S_OK;
}


//--------------------------------------------------------------------------------------
GUI_Base* CDXUTDialog::GetControl( int ID )
{
	// Try to find the control with the given ID
	for( int i = 0; i < m_Controls.GetSize(); i++ )
	{
		GUI_Base* pControl = m_Controls.GetAt( i );

		if( pControl->GetID() == ID )
		{
			return pControl;
		}
	}

	// Not found
	return NULL;
}



//--------------------------------------------------------------------------------------
GUI_Base* CDXUTDialog::GetControl( int ID, UINT nControlType )
{
	// Try to find the control with the given ID
	for( int i = 0; i < m_Controls.GetSize(); i++ )
	{
		GUI_Base* pControl = m_Controls.GetAt( i );

		if( pControl->GetID() == ID && pControl->GetType() == nControlType )
		{
			return pControl;
		}
	}

	// Not found
	return NULL;
}



//--------------------------------------------------------------------------------------
GUI_Base* CDXUTDialog::GetNextControl( GUI_Base* pControl )
{
	int index = pControl->m_Index + 1;

	CDXUTDialog* pDialog = pControl->m_pDialog;

	// Cycle through dialogs in the loop to find the next control. Note
	// that if only one control exists in all looped dialogs it will
	// be the returned 'next' control.
	while( index >= ( int )pDialog->m_Controls.GetSize() )
	{
		pDialog = pDialog->m_pNextDialog;
		index = 0;
	}

	return pDialog->m_Controls.GetAt( index );
}

//--------------------------------------------------------------------------------------
GUI_Base* CDXUTDialog::GetPrevControl( GUI_Base* pControl )
{
	int index = pControl->m_Index - 1;

	CDXUTDialog* pDialog = pControl->m_pDialog;

	// Cycle through dialogs in the loop to find the next control. Note
	// that if only one control exists in all looped dialogs it will
	// be the returned 'previous' control.
	while( index < 0 )
	{
		pDialog = pDialog->m_pPrevDialog;
		if( pDialog == NULL )
			pDialog = pControl->m_pDialog;

		index = pDialog->m_Controls.GetSize() - 1;
	}

	return pDialog->m_Controls.GetAt( index );
}


//--------------------------------------------------------------------------------------
// void CDXUTDialog::ClearRadioButtonGroup( UINT nButtonGroup )
// {
// 	// Find all radio buttons with the given group number
// 	for( int i = 0; i < m_Controls.GetSize(); i++ )
// 	{
// 		GUI_Base* pControl = m_Controls.GetAt( i );
// 
// 		if( pControl->GetType() == DXUT_CONTROL_RADIOBUTTON )
// 		{
// 			CDXUTRadioButton* pRadioButton = ( CDXUTRadioButton* )pControl;
// 
// 			if( pRadioButton->GetButtonGroup() == nButtonGroup )
// 				pRadioButton->SetChecked( false, false );
// 		}
// 	}
// }



//--------------------------------------------------------------------------------------
// void CDXUTDialog::ClearComboBox( int ID )
// {
// 	CDXUTComboBox* pComboBox = GetComboBox( ID );
// 	if( pComboBox == NULL )
// 		return;
// 
// 	pComboBox->RemoveAllItems();
// }
// 
// 


//--------------------------------------------------------------------------------------
void CDXUTDialog::RequestFocus( GUI_Base* pControl )
{
	if( s_pControlFocus == pControl )
		return;

	if( !pControl->CanHaveFocus() )
		return;

	if( s_pControlFocus )
		s_pControlFocus->OnFocusOut();

	pControl->OnFocusIn();
	s_pControlFocus = pControl;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTDialog::DrawRect( RECT* pRect, D3DCOLOR color )
{
	if( m_pManager->GetD3D9Device() )
		return DrawRect9( pRect, color );
	else
		return DrawRect10( pRect, color );
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTDialog::DrawRect9( RECT* pRect, D3DCOLOR color )
{
	RECT rcScreen = *pRect;
	OffsetRect( &rcScreen, m_x, m_y );

	// If caption is enabled, offset the Y position by its height.
	if( m_bCaption )
		OffsetRect( &rcScreen, 0, m_nCaptionHeight );

	DXUT_SCREEN_VERTEX vertices[4] =
	{
		( float )rcScreen.left - 0.5f,  ( float )rcScreen.top - 0.5f,    0.5f, 1.0f, color, 0, 0,
		( float )rcScreen.right - 0.5f, ( float )rcScreen.top - 0.5f,    0.5f, 1.0f, color, 0, 0,
		( float )rcScreen.right - 0.5f, ( float )rcScreen.bottom - 0.5f, 0.5f, 1.0f, color, 0, 0,
		( float )rcScreen.left - 0.5f,  ( float )rcScreen.bottom - 0.5f, 0.5f, 1.0f, color, 0, 0,
	};

	IDirect3DDevice9* pd3dDevice = m_pManager->GetD3D9Device();

	// Since we're doing our own drawing here we need to flush the sprites
	m_pManager->m_pSprite->Flush();
	IDirect3DVertexDeclaration9* pDecl = NULL;
	pd3dDevice->GetVertexDeclaration( &pDecl );  // Preserve the sprite's current vertex decl
	pd3dDevice->SetFVF( DXUT_SCREEN_VERTEX::FVF );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );

	pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, vertices, sizeof( DXUT_SCREEN_VERTEX ) );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

	// Restore the vertex decl
	pd3dDevice->SetVertexDeclaration( pDecl );
	pDecl->Release();

	return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTDialog::DrawRect10( RECT* pRect, D3DCOLOR color )
{
	RECT rcScreen = *pRect;
	OffsetRect( &rcScreen, m_x, m_y );

	// If caption is enabled, offset the Y position by its height.
	if( m_bCaption )
		OffsetRect( &rcScreen, 0, m_nCaptionHeight );

	ID3D10Device* pd3dDevice = m_pManager->GetD3D10Device();

	// Convert the rect from screen coordinates to clip space coordinates.
	float Left, Right, Top, Bottom;
	Left = rcScreen.left * 2.0f / m_pManager->m_nBackBufferWidth - 1.0f;
	Right = rcScreen.right * 2.0f / m_pManager->m_nBackBufferWidth - 1.0f;
	Top = 1.0f - rcScreen.top * 2.0f / m_pManager->m_nBackBufferHeight;
	Bottom = 1.0f - rcScreen.bottom * 2.0f / m_pManager->m_nBackBufferHeight;
	DXUT_SCREEN_VERTEX_10 vertices[4] =
	{
		{ Left,     Top, 0.5f, D3DCOLOR_TO_D3DCOLORVALUE( color ), 0.0f, 0.0f },
		{ Right,    Top, 0.5f, D3DCOLOR_TO_D3DCOLORVALUE( color ), 1.0f, 0.0f },
		{ Left,  Bottom, 0.5f, D3DCOLOR_TO_D3DCOLORVALUE( color ), 0.0f, 1.0f },
		{ Right, Bottom, 0.5f, D3DCOLOR_TO_D3DCOLORVALUE( color ), 1.0f, 1.0f },
	};
	DXUT_SCREEN_VERTEX_10* pVB;
	if( SUCCEEDED( m_pManager->m_pVBScreenQuad10->Map( D3D10_MAP_WRITE_DISCARD, 0, ( LPVOID* )&pVB ) ) )
	{
		CopyMemory( pVB, vertices, sizeof( vertices ) );
		m_pManager->m_pVBScreenQuad10->Unmap();
	}

	// Since we're doing our own drawing here we need to flush the sprites
	m_pManager->m_pSprite10->Flush();

	ID3D10InputLayout* pOldLayout;
	D3D10_PRIMITIVE_TOPOLOGY OldTopology;

	pd3dDevice->IAGetInputLayout( &pOldLayout );
	pd3dDevice->IAGetPrimitiveTopology( &OldTopology );
	pd3dDevice->IASetInputLayout( m_pManager->m_pInputLayout10 );
	pd3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	D3D10_TECHNIQUE_DESC techDesc;
	m_pManager->m_pTechRenderUI10->GetDesc( &techDesc );
	for( UINT p = 0; p < techDesc.Passes; ++p )
	{
		m_pManager->m_pTechRenderUI10->GetPassByIndex( p )->Apply( 0 );
		pd3dDevice->Draw( 4, 0 );
	}

	pd3dDevice->IASetInputLayout( pOldLayout );
	pd3dDevice->IASetPrimitiveTopology( OldTopology );
	SAFE_RELEASE( pOldLayout );

	return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTDialog::DrawPolyLine( POINT* apPoints, UINT nNumPoints, D3DCOLOR color )
{
	DXUT_SCREEN_VERTEX* vertices = new DXUT_SCREEN_VERTEX[ nNumPoints ];
	if( vertices == NULL )
		return E_OUTOFMEMORY;

	DXUT_SCREEN_VERTEX* pVertex = vertices;
	POINT* pt = apPoints;
	for( UINT i = 0; i < nNumPoints; i++ )
	{
		pVertex->x = m_x + ( float )pt->x;
		pVertex->y = m_y + ( float )pt->y;
		pVertex->z = 0.5f;
		pVertex->h = 1.0f;
		pVertex->color = color;
		pVertex->tu = 0.0f;
		pVertex->tv = 0.0f;

		pVertex++;
		pt++;
	}

	IDirect3DDevice9* pd3dDevice = m_pManager->GetD3D9Device();

	// Since we're doing our own drawing here we need to flush the sprites
	m_pManager->m_pSprite->Flush();
	IDirect3DVertexDeclaration9* pDecl = NULL;
	pd3dDevice->GetVertexDeclaration( &pDecl );  // Preserve the sprite's current vertex decl
	pd3dDevice->SetFVF( DXUT_SCREEN_VERTEX::FVF );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );

	pd3dDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, nNumPoints - 1, vertices, sizeof( DXUT_SCREEN_VERTEX ) );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

	// Restore the vertex decl
	pd3dDevice->SetVertexDeclaration( pDecl );
	pDecl->Release();

	SAFE_DELETE_ARRAY( vertices );
	return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTDialog::DrawSprite( CDXUTElement* pElement, RECT* prcDest, float fDepth )
{
	if( m_pManager->GetD3D9Device() )
		return DrawSprite9( pElement, prcDest );
	else
		return DrawSprite10( pElement, prcDest, fDepth );
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTDialog::DrawSprite9( CDXUTElement* pElement, RECT* prcDest )
{
	// No need to draw fully transparent layers
	if( pElement->TextureColor.Current.a == 0 )
		return S_OK;

	RECT rcTexture = pElement->rcTexture;

	RECT rcScreen = *prcDest;
	OffsetRect( &rcScreen, m_x, m_y );

	// If caption is enabled, offset the Y position by its height.
	if( m_bCaption )
		OffsetRect( &rcScreen, 0, m_nCaptionHeight );

	DXUTTextureNode* pTextureNode = GetTexture( pElement->iTexture );
	if( pTextureNode == NULL )
		return E_FAIL;

	float fScaleX = ( float )RectWidth( rcScreen ) / RectWidth( rcTexture );
	float fScaleY = ( float )RectHeight( rcScreen ) / RectHeight( rcTexture );

	D3DXMATRIXA16 matTransform;
	D3DXMatrixScaling( &matTransform, fScaleX, fScaleY, 1.0f );

	m_pManager->m_pSprite->SetTransform( &matTransform );

	D3DXVECTOR3 vPos( ( float )rcScreen.left, ( float )rcScreen.top, 0.0f );

	vPos.x /= fScaleX;
	vPos.y /= fScaleY;

	return m_pManager->m_pSprite->Draw( pTextureNode->pTexture9, &rcTexture, NULL, &vPos,
										pElement->TextureColor.Current );
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTDialog::DrawSprite10( CDXUTElement* pElement, RECT* prcDest, float fDepth )
{
	// No need to draw fully transparent layers
	if( pElement->TextureColor.Current.a == 0 )
		return S_OK;

	RECT rcTexture = pElement->rcTexture;

	RECT rcScreen = *prcDest;
	OffsetRect( &rcScreen, m_x, m_y );
	OffsetRect( &rcScreen, RectWidth( rcScreen ) / 2, RectHeight( rcScreen ) / 2 );

	// If caption is enabled, offset the Y position by its height.
	if( m_bCaption )
		OffsetRect( &rcScreen, 0, m_nCaptionHeight );

	DXUTTextureNode* pTextureNode = GetTexture( pElement->iTexture );
	if( pTextureNode == NULL )
		return E_FAIL;

	float fBBWidth = ( float )m_pManager->m_nBackBufferWidth;
	float fBBHeight = ( float )m_pManager->m_nBackBufferHeight;
	float fScaleX = ( float )RectWidth( rcScreen );
	float fScaleY = ( float )RectHeight( rcScreen );

	D3DXVECTOR3 vPos( ( float )rcScreen.left, ( float )rcScreen.top, fDepth );

	D3DXMATRIXA16 matScaling;
	D3DXMATRIXA16 matTranslation;
	D3DXMatrixScaling( &matScaling, fScaleX, fScaleY, 1.0f );
	D3DXMatrixTranslation( &matTranslation, vPos.x, fBBHeight - vPos.y, vPos.z );

	D3DXMATRIXA16 matProjection;
	D3DXMatrixOrthoOffCenterLH( &matProjection, 0.0f, fBBWidth, 0.0f, fBBHeight, 0.1f, 10 );
	m_pManager->m_pSprite10->SetProjectionTransform( &matProjection );

	D3DX10_SPRITE Sprite;

	Sprite.matWorld = matScaling * matTranslation;
	Sprite.pTexture = pTextureNode->pTexResView;
	float fTexWidth = ( float )pTextureNode->dwWidth;
	float fTexHeight = ( float )pTextureNode->dwHeight;
	Sprite.TexCoord.x = ( float )( rcTexture.left ) / fTexWidth;
	Sprite.TexCoord.y = ( float )( rcTexture.top ) / fTexHeight;
	Sprite.TexSize.x = ( float )( rcTexture.right - rcTexture.left ) / fTexWidth;
	Sprite.TexSize.y = ( float )( rcTexture.bottom - rcTexture.top ) / fTexHeight;
	Sprite.TextureIndex = 0;
	Sprite.ColorModulate = pElement->TextureColor.Current;

	return m_pManager->m_pSprite10->DrawSpritesBuffered( &Sprite, 1 );
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTDialog::CalcTextRect( LPCWSTR strText, CDXUTElement* pElement, RECT* prcDest, int nCount )
{
	HRESULT hr = S_OK;

	DXUTFontNode* pFontNode = GetFont( pElement->iFont );
	if( pFontNode == NULL )
		return E_FAIL;

	DWORD dwTextFormat = pElement->dwTextFormat | DT_CALCRECT;
	// Since we are only computing the rectangle, we don't need a sprite.
	if( pFontNode->pFont10 )
	{
		hr = pFontNode->pFont10->DrawText( NULL, strText, nCount, prcDest, dwTextFormat, pElement->FontColor.Current );
		if( FAILED( hr ) )
			return hr;
	}
	else if( pFontNode->pFont9 )
	{
		hr = pFontNode->pFont9->DrawText( NULL, strText, nCount, prcDest, dwTextFormat, pElement->FontColor.Current );
		if( FAILED( hr ) )
			return hr;
	}

	return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTDialog::DrawText( LPCWSTR strText, CDXUTElement* pElement, RECT* prcDest, bool bShadow, int nCount )
{
	if( m_pManager->GetD3D9Device() )
		return DrawText9( strText, pElement, prcDest, bShadow, nCount );
	else
		return DrawText10( strText, pElement, prcDest, bShadow, nCount );
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTDialog::DrawText9( LPCWSTR strText, CDXUTElement* pElement, RECT* prcDest, bool bShadow, int nCount )
{
	HRESULT hr = S_OK;

	// No need to draw fully transparent layers
	if( pElement->FontColor.Current.a == 0 )
		return S_OK;

	RECT rcScreen = *prcDest;
	OffsetRect( &rcScreen, m_x, m_y );

	// If caption is enabled, offset the Y position by its height.
	if( m_bCaption )
		OffsetRect( &rcScreen, 0, m_nCaptionHeight );

	D3DXMATRIX matTransform;
	D3DXMatrixIdentity( &matTransform );
	m_pManager->m_pSprite->SetTransform( &matTransform );

	DXUTFontNode* pFontNode = GetFont( pElement->iFont );

	if( bShadow )
	{
		RECT rcShadow = rcScreen;
		OffsetRect( &rcShadow, 1, 1 );
		hr = pFontNode->pFont9->DrawText( m_pManager->m_pSprite, strText, nCount, &rcShadow, pElement->dwTextFormat,
										  D3DCOLOR_ARGB( DWORD( pElement->FontColor.Current.a * 255 ), 0, 0, 0 ) );
		if( FAILED( hr ) )
			return hr;
	}

	hr = pFontNode->pFont9->DrawText( m_pManager->m_pSprite, strText, nCount, &rcScreen, pElement->dwTextFormat,
									  pElement->FontColor.Current );
	if( FAILED( hr ) )
		return hr;

	return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTDialog::DrawText10( LPCWSTR strText, CDXUTElement* pElement, RECT* prcDest, bool bShadow, int nCount )
{
	HRESULT hr = S_OK;

	// No need to draw fully transparent layers
	if( pElement->FontColor.Current.a == 0 )
		return S_OK;

	RECT rcScreen = *prcDest;
	OffsetRect( &rcScreen, m_x, m_y );

	// If caption is enabled, offset the Y position by its height.
	if( m_bCaption )
		OffsetRect( &rcScreen, 0, m_nCaptionHeight );

	float fBBWidth = ( float )m_pManager->m_nBackBufferWidth;
	float fBBHeight = ( float )m_pManager->m_nBackBufferHeight;

	D3DXMATRIX matProjection;
	D3DXMatrixOrthoOffCenterLH( &matProjection, 0.0f, fBBWidth, 0.0f, fBBHeight, 0.1f, 10 );
	m_pManager->m_pSprite10->SetProjectionTransform( &matProjection );

	DXUTFontNode* pFontNode = GetFont( pElement->iFont );

	if( bShadow )
	{
		RECT rcShadow = rcScreen;
		OffsetRect( &rcShadow, 1, 1 );
		hr = pFontNode->pFont10->DrawText( m_pManager->m_pSprite10, strText, nCount, &rcShadow, pElement->dwTextFormat,
			D3DCOLOR_ARGB( DWORD( pElement->FontColor.Current.a * 255 ), 0, 0, 0 ) );
		if( FAILED( hr ) )
			return hr;
	}

	hr = pFontNode->pFont10->DrawText( m_pManager->m_pSprite10, strText, nCount, &rcScreen, pElement->dwTextFormat,
		pElement->FontColor.Current );
	if( FAILED( hr ) )
		return hr;

	return S_OK;
}


//--------------------------------------------------------------------------------------
void CDXUTDialog::SetBackgroundColors( D3DCOLOR colorTopLeft, D3DCOLOR colorTopRight, D3DCOLOR colorBottomLeft,
	D3DCOLOR colorBottomRight )
{
	m_colorTopLeft = colorTopLeft;
	m_colorTopRight = colorTopRight;
	m_colorBottomLeft = colorBottomLeft;
	m_colorBottomRight = colorBottomRight;
}


//--------------------------------------------------------------------------------------
void CDXUTDialog::SetNextDialog( CDXUTDialog* pNextDialog )
{
	if( pNextDialog == NULL )
		pNextDialog = this;

	m_pNextDialog = pNextDialog;
	if( pNextDialog )
		m_pNextDialog->m_pPrevDialog = this;
}


//--------------------------------------------------------------------------------------
void CDXUTDialog::ClearFocus()
{
	if( s_pControlFocus )
	{
		s_pControlFocus->OnFocusOut();
		s_pControlFocus = NULL;
	}

	ReleaseCapture();
}


//--------------------------------------------------------------------------------------
void CDXUTDialog::FocusDefaultControl()
{
	// Check for default control in this dialog
	for( int i = 0; i < m_Controls.GetSize(); i++ )
	{
		GUI_Base* pControl = m_Controls.GetAt( i );
		if( pControl->m_bIsDefault )
		{
			// Remove focus from the current control
			ClearFocus();

			// Give focus to the default control
			s_pControlFocus = pControl;
			s_pControlFocus->OnFocusIn();
			return;
		}
	}
}


//--------------------------------------------------------------------------------------
bool CDXUTDialog::OnCycleFocus( bool bForward )
{
	GUI_Base* pControl = NULL;
	CDXUTDialog* pDialog = NULL; // pDialog and pLastDialog are used to track wrapping of
	CDXUTDialog* pLastDialog;    // focus from first control to last or vice versa.

	if( s_pControlFocus == NULL )
	{
		// If s_pControlFocus is NULL, we focus the first control of first dialog in
		// the case that bForward is true, and focus the last control of last dialog when
		// bForward is false.
		//
		if( bForward )
		{
			// Search for the first control from the start of the dialog
			// array.
			for( int d = 0; d < m_pManager->m_Dialogs.GetSize(); ++d )
			{
				pDialog = pLastDialog = m_pManager->m_Dialogs.GetAt( d );
				if( pDialog && pDialog->m_Controls.GetSize() > 0 )
				{
					pControl = pDialog->m_Controls.GetAt( 0 );
					break;
				}
			}

			if( !pDialog || !pControl )
			{
				// No dialog has been registered yet or no controls have been
				// added to the dialogs. Cannot proceed.
				return true;
			}
		}
		else
		{
			// Search for the first control from the end of the dialog
			// array.
			for( int d = m_pManager->m_Dialogs.GetSize() - 1; d >= 0; --d )
			{
				pDialog = pLastDialog = m_pManager->m_Dialogs.GetAt( d );
				if( pDialog && pDialog->m_Controls.GetSize() > 0 )
				{
					pControl = pDialog->m_Controls.GetAt( pDialog->m_Controls.GetSize() - 1 );
					break;
				}
			}

			if( !pDialog || !pControl )
			{
				// No dialog has been registered yet or no controls have been
				// added to the dialogs. Cannot proceed.
				return true;
			}
		}
	}
	else if( s_pControlFocus->m_pDialog != this )
	{
		// If a control belonging to another dialog has focus, let that other
		// dialog handle this event by returning false.
		//
		return false;
	}
	else
	{
		// Focused control belongs to this dialog. Cycle to the
		// next/previous control.
		pLastDialog = s_pControlFocus->m_pDialog;
		pControl = ( bForward ) ? GetNextControl( s_pControlFocus ) : GetPrevControl( s_pControlFocus );
		pDialog = pControl->m_pDialog;
	}

	for( int i = 0; i < 0xffff; i++ )
	{
		// If we just wrapped from last control to first or vice versa,
		// set the focused control to NULL. This state, where no control
		// has focus, allows the camera to work.
		int nLastDialogIndex = m_pManager->m_Dialogs.IndexOf( pLastDialog );
		int nDialogIndex = m_pManager->m_Dialogs.IndexOf( pDialog );
		if( ( !bForward && nLastDialogIndex < nDialogIndex ) ||
			( bForward && nDialogIndex < nLastDialogIndex ) )
		{
			if( s_pControlFocus )
				s_pControlFocus->OnFocusOut();
			s_pControlFocus = NULL;
			return true;
		}

		// If we've gone in a full circle then focus doesn't change
		if( pControl == s_pControlFocus )
			return true;

		// If the dialog accepts keybord input and the control can have focus then
		// move focus
		if( pControl->m_pDialog->m_bKeyboardInput && pControl->CanHaveFocus() )
		{
			if( s_pControlFocus )
				s_pControlFocus->OnFocusOut();
			s_pControlFocus = pControl;
			s_pControlFocus->OnFocusIn();
			return true;
		}

		pLastDialog = pDialog;
		pControl = ( bForward ) ? GetNextControl( pControl ) : GetPrevControl( pControl );
		pDialog = pControl->m_pDialog;
	}

	// If we reached this point, the chain of dialogs didn't form a complete loop
	DXTRACE_ERR( L"CDXUTDialog: Multiple dialogs are improperly chained together", E_FAIL );
	return false;
}




//--------------------------------------------------------------------------------------
void CDXUTDialog::InitDefaultElements()
{
	SetFont( 0, L"Arial", 24, FW_NORMAL );

	CDXUTElement Element;
	RECT rcTexture;

	//-------------------------------------
	// Element for the caption
	//-------------------------------------
	m_CapElement.SetFont( 0 );
	SetRect( &rcTexture, 17, 269, 241, 287 );
	m_CapElement.SetTexture( 0, &rcTexture );
	m_CapElement.TextureColor.States[ DXUT_STATE_NORMAL ] = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	m_CapElement.FontColor.States[ DXUT_STATE_NORMAL ] = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	m_CapElement.SetFont( 0, D3DCOLOR_ARGB( 255, 255, 255, 255 ), DT_LEFT | DT_VCENTER );
	// Pre-blend as we don't need to transition the state
	m_CapElement.TextureColor.Blend( DXUT_STATE_NORMAL, 10.0f );
	m_CapElement.FontColor.Blend( DXUT_STATE_NORMAL, 10.0f );


	//-------------------------------------
	// CDXUTStatic  [2014.11.1 ]
	//-------------------------------------
	Element.SetFont( 0 );
	Element.FontColor.States[ DXUT_STATE_DISABLED ] = D3DCOLOR_ARGB( 200, 200, 200, 200 );

	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_STATIC, 0, &Element );


	//-------------------------------------
	// CDXUTButton - Button
	//-------------------------------------
	SetRect( &rcTexture, 0, 0, 136, 54 );
	Element.SetTexture( 0, &rcTexture );
	Element.SetFont( 0 );
	Element.TextureColor.States[ DXUT_STATE_NORMAL ] = D3DCOLOR_ARGB( 150, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_PRESSED ] = D3DCOLOR_ARGB( 200, 255, 255, 255 );
	Element.FontColor.States[ DXUT_STATE_MOUSEOVER ] = D3DCOLOR_ARGB( 255, 0, 0, 0 );


	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_BUTTON, 0, &Element );


	//-------------------------------------
	// CDXUTButton - Fill layer
	//-------------------------------------
	SetRect( &rcTexture, 136, 0, 252, 54 );
	Element.SetTexture( 0, &rcTexture, D3DCOLOR_ARGB( 0, 255, 255, 255 ) );
	Element.TextureColor.States[ DXUT_STATE_MOUSEOVER ] = D3DCOLOR_ARGB( 160, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_PRESSED ] = D3DCOLOR_ARGB( 60, 0, 0, 0 );
	Element.TextureColor.States[ DXUT_STATE_FOCUS ] = D3DCOLOR_ARGB( 30, 255, 255, 255 );


	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_BUTTON, 1, &Element );


	//-------------------------------------
	// CDXUTCheckBox - Box
	//-------------------------------------
	SetRect( &rcTexture, 0, 54, 27, 81 );
	Element.SetTexture( 0, &rcTexture );
	Element.SetFont( 0, D3DCOLOR_ARGB( 255, 255, 255, 255 ), DT_LEFT | DT_VCENTER );
	Element.FontColor.States[ DXUT_STATE_DISABLED ] = D3DCOLOR_ARGB( 200, 200, 200, 200 );
	Element.TextureColor.States[ DXUT_STATE_NORMAL ] = D3DCOLOR_ARGB( 150, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_FOCUS ] = D3DCOLOR_ARGB( 200, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_PRESSED ] = D3DCOLOR_ARGB( 255, 255, 255, 255 );

	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_CHECKBOX, 0, &Element );


	//-------------------------------------
	// CDXUTCheckBox - Check
	//-------------------------------------
	SetRect( &rcTexture, 27, 54, 54, 81 );
	Element.SetTexture( 0, &rcTexture );

	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_CHECKBOX, 1, &Element );


	//-------------------------------------
	// CDXUTRadioButton - Box
	//-------------------------------------
	SetRect( &rcTexture, 54, 54, 81, 81 );
	Element.SetTexture( 0, &rcTexture );
	Element.SetFont( 0, D3DCOLOR_ARGB( 255, 255, 255, 255 ), DT_LEFT | DT_VCENTER );
	Element.FontColor.States[ DXUT_STATE_DISABLED ] = D3DCOLOR_ARGB( 200, 200, 200, 200 );
	Element.TextureColor.States[ DXUT_STATE_NORMAL ] = D3DCOLOR_ARGB( 150, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_FOCUS ] = D3DCOLOR_ARGB( 200, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_PRESSED ] = D3DCOLOR_ARGB( 255, 255, 255, 255 );

	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_RADIOBUTTON, 0, &Element );


	//-------------------------------------
	// CDXUTRadioButton - Check
	//-------------------------------------
	SetRect( &rcTexture, 81, 54, 108, 81 );
	Element.SetTexture( 0, &rcTexture );

	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_RADIOBUTTON, 1, &Element );


	//-------------------------------------
	// CDXUTComboBox - Main
	//-------------------------------------
	SetRect( &rcTexture, 7, 81, 247, 123 );
	Element.SetTexture( 0, &rcTexture );
	Element.SetFont( 0 );
	Element.TextureColor.States[ DXUT_STATE_NORMAL ] = D3DCOLOR_ARGB( 150, 200, 200, 200 );
	Element.TextureColor.States[ DXUT_STATE_FOCUS ] = D3DCOLOR_ARGB( 170, 230, 230, 230 );
	Element.TextureColor.States[ DXUT_STATE_DISABLED ] = D3DCOLOR_ARGB( 70, 200, 200, 200 );
	Element.FontColor.States[ DXUT_STATE_MOUSEOVER ] = D3DCOLOR_ARGB( 255, 0, 0, 0 );
	Element.FontColor.States[ DXUT_STATE_PRESSED ] = D3DCOLOR_ARGB( 255, 0, 0, 0 );
	Element.FontColor.States[ DXUT_STATE_DISABLED ] = D3DCOLOR_ARGB( 200, 200, 200, 200 );


	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_COMBOBOX, 0, &Element );


	//-------------------------------------
	// CDXUTComboBox - Button
	//-------------------------------------
	SetRect( &rcTexture, 98, 189, 151, 238 );
	Element.SetTexture( 0, &rcTexture );
	Element.TextureColor.States[ DXUT_STATE_NORMAL ] = D3DCOLOR_ARGB( 150, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_PRESSED ] = D3DCOLOR_ARGB( 255, 150, 150, 150 );
	Element.TextureColor.States[ DXUT_STATE_FOCUS ] = D3DCOLOR_ARGB( 200, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_DISABLED ] = D3DCOLOR_ARGB( 70, 255, 255, 255 );

	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_COMBOBOX, 1, &Element );


	//-------------------------------------
	// CDXUTComboBox - Dropdown
	//-------------------------------------
	SetRect( &rcTexture, 13, 123, 241, 160 );
	Element.SetTexture( 0, &rcTexture );
	Element.SetFont( 0, D3DCOLOR_ARGB( 255, 0, 0, 0 ), DT_LEFT | DT_TOP );

	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_COMBOBOX, 2, &Element );


	//-------------------------------------
	// CDXUTComboBox - Selection
	//-------------------------------------
	SetRect( &rcTexture, 12, 163, 239, 183 );
	Element.SetTexture( 0, &rcTexture );
	Element.SetFont( 0, D3DCOLOR_ARGB( 255, 255, 255, 255 ), DT_LEFT | DT_TOP );

	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_COMBOBOX, 3, &Element );


	//-------------------------------------
	// CDXUTSlider - Track
	//-------------------------------------
	SetRect( &rcTexture, 1, 187, 93, 228 );
	Element.SetTexture( 0, &rcTexture );
	Element.TextureColor.States[ DXUT_STATE_NORMAL ] = D3DCOLOR_ARGB( 150, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_FOCUS ] = D3DCOLOR_ARGB( 200, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_DISABLED ] = D3DCOLOR_ARGB( 70, 255, 255, 255 );

	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_SLIDER, 0, &Element );

	//-------------------------------------
	// CDXUTSlider - Button
	//-------------------------------------
	SetRect( &rcTexture, 151, 193, 192, 234 );
	Element.SetTexture( 0, &rcTexture );

	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_SLIDER, 1, &Element );

	//-------------------------------------
	// CDXUTScrollBar - Track
	//-------------------------------------
	int nScrollBarStartX = 196;
	int nScrollBarStartY = 191;
	SetRect( &rcTexture, nScrollBarStartX + 0, nScrollBarStartY + 21, nScrollBarStartX + 22, nScrollBarStartY + 32 );
	Element.SetTexture( 0, &rcTexture );
	Element.TextureColor.States[ DXUT_STATE_DISABLED ] = D3DCOLOR_ARGB( 255, 200, 200, 200 );

	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_SCROLLBAR, 0, &Element );

	//-------------------------------------
	// CDXUTScrollBar - Up Arrow
	//-------------------------------------
	SetRect( &rcTexture, nScrollBarStartX + 0, nScrollBarStartY + 1, nScrollBarStartX + 22, nScrollBarStartY + 21 );
	Element.SetTexture( 0, &rcTexture );
	Element.TextureColor.States[ DXUT_STATE_DISABLED ] = D3DCOLOR_ARGB( 255, 200, 200, 200 );


	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_SCROLLBAR, 1, &Element );

	//-------------------------------------
	// CDXUTScrollBar - Down Arrow
	//-------------------------------------
	SetRect( &rcTexture, nScrollBarStartX + 0, nScrollBarStartY + 32, nScrollBarStartX + 22, nScrollBarStartY + 53 );
	Element.SetTexture( 0, &rcTexture );
	Element.TextureColor.States[ DXUT_STATE_DISABLED ] = D3DCOLOR_ARGB( 255, 200, 200, 200 );


	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_SCROLLBAR, 2, &Element );

	//-------------------------------------
	// CDXUTScrollBar - Button
	//-------------------------------------
	SetRect( &rcTexture, 220, 192, 238, 234 );
	Element.SetTexture( 0, &rcTexture );

	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_SCROLLBAR, 3, &Element );


	//-------------------------------------
	// CDXUTEditBox
	//-------------------------------------
	// Element assignment:
	//   0 - text area
	//   1 - top left border
	//   2 - top border
	//   3 - top right border
	//   4 - left border
	//   5 - right border
	//   6 - lower left border
	//   7 - lower border
	//   8 - lower right border

	Element.SetFont( 0, D3DCOLOR_ARGB( 255, 0, 0, 0 ), DT_LEFT | DT_TOP );

	// Assign the style
	SetRect( &rcTexture, 14, 90, 241, 113 );
	Element.SetTexture( 0, &rcTexture );
	SetDefaultElement( DXUT_CONTROL_EDITBOX, 0, &Element );
	SetRect( &rcTexture, 8, 82, 14, 90 );
	Element.SetTexture( 0, &rcTexture );
	SetDefaultElement( DXUT_CONTROL_EDITBOX, 1, &Element );
	SetRect( &rcTexture, 14, 82, 241, 90 );
	Element.SetTexture( 0, &rcTexture );
	SetDefaultElement( DXUT_CONTROL_EDITBOX, 2, &Element );
	SetRect( &rcTexture, 241, 82, 246, 90 );
	Element.SetTexture( 0, &rcTexture );
	SetDefaultElement( DXUT_CONTROL_EDITBOX, 3, &Element );
	SetRect( &rcTexture, 8, 90, 14, 113 );
	Element.SetTexture( 0, &rcTexture );
	SetDefaultElement( DXUT_CONTROL_EDITBOX, 4, &Element );
	SetRect( &rcTexture, 241, 90, 246, 113 );
	Element.SetTexture( 0, &rcTexture );
	SetDefaultElement( DXUT_CONTROL_EDITBOX, 5, &Element );
	SetRect( &rcTexture, 8, 113, 14, 121 );
	Element.SetTexture( 0, &rcTexture );
	SetDefaultElement( DXUT_CONTROL_EDITBOX, 6, &Element );
	SetRect( &rcTexture, 14, 113, 241, 121 );
	Element.SetTexture( 0, &rcTexture );
	SetDefaultElement( DXUT_CONTROL_EDITBOX, 7, &Element );
	SetRect( &rcTexture, 241, 113, 246, 121 );
	Element.SetTexture( 0, &rcTexture );
	SetDefaultElement( DXUT_CONTROL_EDITBOX, 8, &Element );

	//-------------------------------------
	// CDXUTListBox - Main
	//-------------------------------------
	SetRect( &rcTexture, 13, 123, 241, 160 );
	Element.SetTexture( 0, &rcTexture );
	Element.SetFont( 0, D3DCOLOR_ARGB( 255, 0, 0, 0 ), DT_LEFT | DT_TOP );

	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_LISTBOX, 0, &Element );

	//-------------------------------------
	// CDXUTListBox - Selection
	//-------------------------------------

	SetRect( &rcTexture, 16, 166, 240, 183 );
	Element.SetTexture( 0, &rcTexture );
	Element.SetFont( 0, D3DCOLOR_ARGB( 255, 255, 255, 255 ), DT_LEFT | DT_TOP );

	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_LISTBOX, 1, &Element );


	//////////////////////////////////////////////////////////////////////////
// 	// 여기서부터 내가만든 유아이 엘리먼트 초기화 [2014.11.5 ]
   // 프로그래스바(스킬 포인트바)의 파란영역(내부)
	SetRect( &rcTexture, 724, 452, 981, 488  );
	Element.SetTexture( eTEX_ID_SHUI, &rcTexture, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	Element.TextureColor.Current = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_PROGRESSBAR, 0, &Element );
	
	// 프로그레스바(스킬 포인트바)의 테두리(외부)
	SetRect( &rcTexture, 723, 500, 982, 533);
	Element.SetTexture( eTEX_ID_SHUI, &rcTexture, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	Element.TextureColor.Current = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_PROGRESSBAR, 1, &Element );


	// 프로그레스바(HP바)의 테두리(내부)
	SetRect( &rcTexture, 0, 97, 829, 198 );
	Element.SetTexture( eTEX_ID_SHUI, &rcTexture, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	Element.TextureColor.Current = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_PROGRESSBAR, 2, &Element ); // [2014.12.2 ]

	// 프로그레스바(HP바)의 테두리(외부)
	SetRect( &rcTexture,  0, 0, 829, 98  );
	Element.SetTexture( eTEX_ID_SHUI, &rcTexture, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	Element.TextureColor.Current = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_PROGRESSBAR, 3, &Element );  // [2014.12.2 ]

	// 타이머바 (슬라이더바) 의  막대
	SetRect( &rcTexture, 13, 191, 494, 250 );
	Element.SetTexture( eTEX_ID_SHUI, &rcTexture, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	Element.TextureColor.Current = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_NORMAL ] = D3DCOLOR_ARGB( 150, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_FOCUS ] = D3DCOLOR_ARGB( 200, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_DISABLED ] = D3DCOLOR_ARGB( 70, 255, 255, 255 );
	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_SLIDER, eEle_Timer_Stick, &Element ); // [2014.12.2 ]

	// 타이머바 의 버튼 
	SetRect( &rcTexture,  16, 271, 83, 341  );
	Element.SetTexture( eTEX_ID_SHUI, &rcTexture, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	Element.TextureColor.Current = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_NORMAL ] = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_FOCUS ] = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_DISABLED ] = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_SLIDER, eEle_Timer_Button, &Element );  // [2014.12.2 ]

	//---------------------- 스킬1이미지 테두리 -----------------------------------------ㅎㅎ
	SetRect( &rcTexture,  735, 541, 814, 607  );
	Element.SetTexture( eTEX_ID_SHUI, &rcTexture, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	Element.TextureColor.Current = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_DISABLED ] = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_IMAGE, eEle_Skill1_Image1, &Element );  // [2014.12.2 ]
	
	//---------------------- 스킬1이미지 원본 -----------------------------------------
	SetRect( &rcTexture,  735, 635, 813, 705  );
	Element.SetTexture( eTEX_ID_SHUI, &rcTexture, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	Element.TextureColor.Current = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_DISABLED ] = D3DCOLOR_ARGB( 250, 50, 50, 50 );
	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_IMAGE, eEle_Skill1_Image2, &Element );  // [2014.12.2 ]

	//---------------------- 스킬2이미지 테두리 -----------------------------------------
	SetRect( &rcTexture, 817, 541, 896, 606 );
	Element.SetTexture( eTEX_ID_SHUI, &rcTexture, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	Element.TextureColor.Current = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_DISABLED ] = D3DCOLOR_ARGB(  255, 255, 255, 255);
	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_IMAGE, eEle_Skill2_Image1, &Element );  // [2014.12.4 ]

	//---------------------- 스킬2이미지 원본 -----------------------------------------
	SetRect( &rcTexture,  820, 636, 899, 707  );
	Element.SetTexture( eTEX_ID_SHUI, &rcTexture, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	Element.TextureColor.Current = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_DISABLED ] = D3DCOLOR_ARGB(  250, 50, 50, 50 );
	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_IMAGE, eEle_Skill2_Image2, &Element );  // [2014.12.4 ]

	//---------------------- 스킬3이미지 테두리 -----------------------------------------00
	SetRect( &rcTexture, 898, 541, 980, 606 );
	Element.SetTexture( eTEX_ID_SHUI, &rcTexture, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	Element.TextureColor.Current = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_DISABLED ] = D3DCOLOR_ARGB( 255, 255, 255, 255);
	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_IMAGE, eEle_Skill3_Image1, &Element );  // [2014.12.4 ]

	//---------------------- 스킬3이미지 원본 -----------------------------------------
	SetRect( &rcTexture,  899, 636, 981, 707  );
	Element.SetTexture( eTEX_ID_SHUI, &rcTexture, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	Element.TextureColor.Current = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_DISABLED ] = D3DCOLOR_ARGB(  250, 50, 50, 50  );
	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_IMAGE, eEle_Skill3_Image2, &Element );  // [2014.12.4 ]

	//---------------------- 모자  -----------------------------------------
	SetRect( &rcTexture, 607, 208, 976, 401 );
	Element.SetTexture( eTEX_ID_SHUI, &rcTexture, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	Element.TextureColor.Current = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_DISABLED ] = D3DCOLOR_ARGB( 255, 255, 255, 255);
	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_IMAGE, eEle_Hat, &Element );  // [2014.12.4 ]


	

	//---------------------- 실패했을때 뜨는 이미지 Fail !!  [2014.12.4 ] -----------------------------------------
	SetRect( &rcTexture,  217, 686, 421, 787  );
	Element.SetTexture( eTEX_ID_SHUI, &rcTexture, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	Element.TextureColor.Current = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	//Element.TextureColor.States[ DXUT_STATE_DISABLED ] = D3DCOLOR_ARGB(  250, 50, 50, 50  );
	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_IMAGE, eEle_Fail_Image, &Element );  // [2014.12.4 ]

	//---------------------- 성공했을때 뜨는 이미지 Success !!  [2014.12.4 ] -----------------------------------------
	SetRect( &rcTexture,  30, 888, 413, 987  );
	Element.SetTexture( eTEX_ID_SHUI, &rcTexture, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	Element.TextureColor.Current = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	//Element.TextureColor.States[ DXUT_STATE_DISABLED ] = D3DCOLOR_ARGB(  250, 50, 50, 50  );
	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_IMAGE, eEle_Success_Image, &Element );  // [2014.12.4 ]

	//---------------------- Tedy Vs Night !!!!!!!!  [2014.12.4 ] -----------------------------------------
	SetRect( &rcTexture,  429, 740, 1022, 1022  );
	Element.SetTexture( eTEX_ID_SHUI, &rcTexture, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	Element.TextureColor.Current = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	//Element.TextureColor.States[ DXUT_STATE_DISABLED ] = D3DCOLOR_ARGB(  250, 50, 50, 50  );
	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_IMAGE, eEle_TeddyVSNight, &Element );  // [2014.12.4 ]

	//////////////////////////////////////////////////////////////////////////
	//-------------------------------------
	// StartButton 1
	//-------------------------------------
	SetRect( &rcTexture, 27, 488, 238, 575 );
	Element.SetTexture( 0, &rcTexture );
	Element.SetFont( 0 );
	Element.TextureColor.States[ DXUT_STATE_NORMAL ] = D3DCOLOR_ARGB( 150, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_PRESSED ] = D3DCOLOR_ARGB( 200, 255, 255, 255 );
	Element.FontColor.States[ DXUT_STATE_MOUSEOVER ] = D3DCOLOR_ARGB( 255, 0, 0, 0 );

	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_BUTTON, eEle_StartButton1, &Element );


	//-------------------------------------
	// StartButton 2
	//-------------------------------------
	SetRect( &rcTexture, 271, 489, 480, 573 );
	Element.SetTexture( 0, &rcTexture, D3DCOLOR_ARGB( 0, 255, 255, 255 ) );
	Element.TextureColor.States[ DXUT_STATE_MOUSEOVER ] = D3DCOLOR_ARGB( 160, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_PRESSED ] = D3DCOLOR_ARGB( 60, 0, 0, 0 );
	Element.TextureColor.States[ DXUT_STATE_FOCUS ] = D3DCOLOR_ARGB( 30, 255, 255, 255 );

	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_BUTTON, eEle_StartButton2, &Element );

	//-------------------------------------
	// CreditButton 1
	//-------------------------------------
	SetRect( &rcTexture, 26, 585, 254, 670 );
	Element.SetTexture( 0, &rcTexture );
	Element.SetFont( 0 );
	Element.TextureColor.States[ DXUT_STATE_NORMAL ] = D3DCOLOR_ARGB( 150, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_PRESSED ] = D3DCOLOR_ARGB( 200, 255, 255, 255 );
	Element.FontColor.States[ DXUT_STATE_MOUSEOVER ] = D3DCOLOR_ARGB( 255, 0, 0, 0 );

	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_BUTTON, eEle_CreditButton1, &Element );


	//-------------------------------------
	// CreditButton 2
	//-------------------------------------
	SetRect( &rcTexture, 270, 586, 498, 670 );
	Element.SetTexture( 0, &rcTexture, D3DCOLOR_ARGB( 0, 255, 255, 255 ) );
	Element.TextureColor.States[ DXUT_STATE_MOUSEOVER ] = D3DCOLOR_ARGB( 160, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_PRESSED ] = D3DCOLOR_ARGB( 60, 0, 0, 0 );
	Element.TextureColor.States[ DXUT_STATE_FOCUS ] = D3DCOLOR_ARGB( 30, 255, 255, 255 );

	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_BUTTON, eEle_CreditButton2, &Element );

	//-------------------------------------
	// ExitButton 1
	//-------------------------------------
	SetRect( &rcTexture, 31, 684, 198, 763 );
	Element.SetTexture( 0, &rcTexture );
	Element.SetFont( 0 );
	Element.TextureColor.States[ DXUT_STATE_NORMAL ] = D3DCOLOR_ARGB( 150, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_PRESSED ] = D3DCOLOR_ARGB( 200, 255, 255, 255 );
	Element.FontColor.States[ DXUT_STATE_MOUSEOVER ] = D3DCOLOR_ARGB( 255, 0, 0, 0 );

	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_BUTTON, eEle_ExitButton1, &Element );


	//-------------------------------------
	// ExitButton 2
	//-------------------------------------
	SetRect( &rcTexture, 29, 777, 196, 857 );
	Element.SetTexture( 0, &rcTexture, D3DCOLOR_ARGB( 0, 255, 255, 255 ) );
	Element.TextureColor.States[ DXUT_STATE_MOUSEOVER ] = D3DCOLOR_ARGB( 160, 255, 255, 255 );
	Element.TextureColor.States[ DXUT_STATE_PRESSED ] = D3DCOLOR_ARGB( 60, 0, 0, 0 );
	Element.TextureColor.States[ DXUT_STATE_FOCUS ] = D3DCOLOR_ARGB( 30, 255, 255, 255 );

	// Assign the Element
	SetDefaultElement( DXUT_CONTROL_BUTTON, eEle_ExitButton2, &Element );
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
// 여기서부터 class CDialogRscMgr 함수정의
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


//-------------------------------------------------------------------------
// 싱글톤 객체 반환 [2014.11.5 ]
//-------------------------------------------------------------------------
CDialogRscMgr* CDialogRscMgr::GetInstance()
{
	if( nullptr == pInstance  )  pInstance = new CDialogRscMgr();
	
	return pInstance;
}


//--------------------------------------------------------------------------------------
CDialogRscMgr::CDialogRscMgr()
{
	// Begin D3D9-specific
	m_pd3d9Device = NULL;
	m_pStateBlock = NULL;
	m_pSprite = NULL;
	// End D3D9-specific

	// Begin D3D10-specific
	m_pd3d10Device = NULL;
	m_pEffect10 = NULL;
	m_pTechRenderUI10 = NULL;
	m_pTechRenderUIUntex10 = NULL;
	m_pFxTexture10 = NULL;
	m_pInputLayout10 = NULL;
	m_pVBScreenQuad10 = NULL;
	m_pStateBlock10 = NULL;
	m_pSprite10 = NULL;
	m_nBackBufferWidth = m_nBackBufferHeight = 0;

	// End D3D10-specific
}


//--------------------------------------------------------------------------------------
CDialogRscMgr::~CDialogRscMgr()
{
	int i;
	for( i = 0; i < m_FontCache.GetSize(); i++ )
	{
		DXUTFontNode* pFontNode = m_FontCache.GetAt( i );
		SAFE_DELETE( pFontNode );
	}
	m_FontCache.RemoveAll();

	for( i = 0; i < m_TextureCache.GetSize(); i++ )
	{
		DXUTTextureNode* pTextureNode = m_TextureCache.GetAt( i );
		SAFE_DELETE( pTextureNode );
	}
	m_TextureCache.RemoveAll();

	//CUniBuffer::Uninitialize();  에디트박스 를 위한 코드 [2014.11.2 ]
}


//--------------------------------------------------------------------------------------
HRESULT CDialogRscMgr::OnD3D9CreateDevice( LPDIRECT3DDEVICE9 pd3dDevice )
{
	HRESULT hr = S_OK;
	int i = 0;

	m_pd3d9Device = pd3dDevice;

	for( i = 0; i < m_FontCache.GetSize(); i++ )
	{
		hr = CreateFont9( i );
		if( FAILED( hr ) )
			return hr;
	}

	for( i = 0; i < m_TextureCache.GetSize(); i++ )
	{
		hr = CreateTexture9( i );
		if( FAILED( hr ) )
			return hr;
	}

	hr = D3DXCreateSprite( pd3dDevice, &m_pSprite );
	if( FAILED( hr ) )
		return DXUT_ERR( L"D3DXCreateSprite", hr );

	return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDialogRscMgr::OnD3D9ResetDevice()
{
	HRESULT hr = S_OK;

	for( int i = 0; i < m_FontCache.GetSize(); i++ )
	{
		DXUTFontNode* pFontNode = m_FontCache.GetAt( i );

		if( pFontNode->pFont9 )
			pFontNode->pFont9->OnResetDevice();
	}

	if( m_pSprite )
		m_pSprite->OnResetDevice();

	V_RETURN( m_pd3d9Device->CreateStateBlock( D3DSBT_ALL, &m_pStateBlock ) );

	return S_OK;
}

//--------------------------------------------------------------------------------------
bool CDialogRscMgr::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return false;
}


//--------------------------------------------------------------------------------------
void CDialogRscMgr::OnD3D9LostDevice()
{
	for( int i = 0; i < m_FontCache.GetSize(); i++ )
	{
		DXUTFontNode* pFontNode = m_FontCache.GetAt( i );

		if( pFontNode->pFont9 )
			pFontNode->pFont9->OnLostDevice();
	}

	if( m_pSprite )
		m_pSprite->OnLostDevice();

	SAFE_RELEASE( m_pStateBlock );
}


//--------------------------------------------------------------------------------------
void CDialogRscMgr::OnD3D9DestroyDevice()
{
	int i = 0;

	m_pd3d9Device = NULL;

	// Release the resources but don't clear the cache, as these will need to be
	// recreated if the device is recreated
	for( i = 0; i < m_FontCache.GetSize(); i++ )
	{
		DXUTFontNode* pFontNode = m_FontCache.GetAt( i );
		SAFE_RELEASE( pFontNode->pFont9 );
	}

	for( i = 0; i < m_TextureCache.GetSize(); i++ )
	{
		DXUTTextureNode* pTextureNode = m_TextureCache.GetAt( i );
		SAFE_RELEASE( pTextureNode->pTexture9 );
	}

	SAFE_RELEASE( m_pSprite );
}

// 
// //--------------------------------------------------------------------------------------
// HRESULT CDialogRscMgr::OnD3D10CreateDevice( ID3D10Device* pd3dDevice )
// {
// 	m_pd3d10Device = pd3dDevice;
// 
// 	HRESULT hr = S_OK;
// 
// 	// Create the UI effect object
// 	V_RETURN( D3DX10CreateEffectFromMemory( g_strUIEffectFile, g_uUIEffectFileSize, NULL, NULL,
// 		NULL, "fx_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
// 		pd3dDevice, NULL, NULL, &m_pEffect10, NULL, NULL ) );
// 
// 	m_pTechRenderUI10 = m_pEffect10->GetTechniqueByName( "RenderUI" );
// 	m_pTechRenderUIUntex10 = m_pEffect10->GetTechniqueByName( "RenderUIUntex" );
// 	m_pFxTexture10 = m_pEffect10->GetVariableByName( "g_Texture" )->AsShaderResource();
// 
// 	// Create the font and texture objects in the cache arrays.
// 	int i = 0;
// 	for( i = 0; i < m_FontCache.GetSize(); i++ )
// 	{
// 		hr = CreateFont10( i );
// 		if( FAILED( hr ) )
// 			return hr;
// 	}
// 
// 	for( i = 0; i < m_TextureCache.GetSize(); i++ )
// 	{
// 		hr = CreateTexture10( i );
// 		if( FAILED( hr ) )
// 			return hr;
// 	}
// 
// 	// Create input layout
// 	const D3D10_INPUT_ELEMENT_DESC layout[] =
// 	{
// 		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0 },
// 		{ "COLOR",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
// 		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,       0, 28, D3D10_INPUT_PER_VERTEX_DATA, 0 },
// 	};
// 
// 	D3D10_PASS_DESC PassDesc;
// 	V_RETURN( m_pTechRenderUI10->GetPassByIndex( 0 )->GetDesc( &PassDesc ) );
// 	V_RETURN( pd3dDevice->CreateInputLayout( layout, 3, PassDesc.pIAInputSignature,
// 		PassDesc.IAInputSignatureSize, &m_pInputLayout10 ) );
// 	DXUT_SetDebugName( m_pInputLayout10, "CDialogRscMgr" );
// 
// 	// Create a vertex buffer quad for rendering later
// 	D3D10_BUFFER_DESC BufDesc;
// 	BufDesc.ByteWidth = sizeof( DXUT_SCREEN_VERTEX_10 ) * 4;
// 	BufDesc.Usage = D3D10_USAGE_DYNAMIC;
// 	BufDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
// 	BufDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
// 	BufDesc.MiscFlags = 0;
// 	V_RETURN( pd3dDevice->CreateBuffer( &BufDesc, NULL, &m_pVBScreenQuad10 ) );
// 	DXUT_SetDebugName( m_pVBScreenQuad10, "CDialogRscMgr" );
// 
// 	return S_OK;
// }


//--------------------------------------------------------------------------------------
HRESULT CDialogRscMgr::OnD3D10ResizedSwapChain( ID3D10Device* pd3dDevice,
	const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc )
{
	HRESULT hr = S_OK;

	m_nBackBufferWidth = pBackBufferSurfaceDesc->Width;
	m_nBackBufferHeight = pBackBufferSurfaceDesc->Height;

	hr = D3DX10CreateSprite( pd3dDevice, DXUT_MAX_GUI_SPRITES, &m_pSprite10 );
	if( FAILED( hr ) )
		return DXUT_ERR( L"D3DX10CreateSprite", hr );

	D3D10_STATE_BLOCK_MASK StateBlockMask;
	DXUT_Dynamic_D3D10StateBlockMaskEnableAll( &StateBlockMask );
	DXUT_Dynamic_D3D10StateBlockMaskDisableCapture( &StateBlockMask, D3D10_DST_OM_RENDER_TARGETS, 0, 1 );
	V_RETURN( DXUT_Dynamic_D3D10CreateStateBlock( pd3dDevice, &StateBlockMask, &m_pStateBlock10 ) );

	return hr;
}


//--------------------------------------------------------------------------------------
void CDialogRscMgr::OnD3D10ReleasingSwapChain()
{
	SAFE_RELEASE( m_pSprite10 );
	SAFE_RELEASE( m_pStateBlock10 );
}


//--------------------------------------------------------------------------------------
void CDialogRscMgr::OnD3D10DestroyDevice()
{
	int i;

	m_pd3d10Device = NULL;

	// Release the resources but don't clear the cache, as these will need to be
	// recreated if the device is recreated
	for( i = 0; i < m_FontCache.GetSize(); i++ )
	{
		DXUTFontNode* pFontNode = m_FontCache.GetAt( i );
		SAFE_RELEASE( pFontNode->pFont10 );
	}

	for( i = 0; i < m_TextureCache.GetSize(); i++ )
	{
		DXUTTextureNode* pTextureNode = m_TextureCache.GetAt( i );
		SAFE_RELEASE( pTextureNode->pTexResView );
		SAFE_RELEASE( pTextureNode->pTexture10 );
	}

	SAFE_RELEASE( m_pVBScreenQuad10 );
	SAFE_RELEASE( m_pStateBlock10 );
	SAFE_RELEASE( m_pSprite10 );
	SAFE_RELEASE( m_pInputLayout10 );
	SAFE_RELEASE( m_pEffect10 );
}


//--------------------------------------------------------------------------------------
bool CDialogRscMgr::RegisterDialog( CDXUTDialog* pDialog )
{
	// Check that the dialog isn't already registered.
	for( int i = 0; i < m_Dialogs.GetSize(); ++i )
		if( m_Dialogs.GetAt( i ) == pDialog )
			return true;

	// Add to the list.
	if( FAILED( m_Dialogs.Add( pDialog ) ) )
		return false;

	// Set up next and prev pointers.
	if( m_Dialogs.GetSize() > 1 )
		m_Dialogs[m_Dialogs.GetSize() - 2]->SetNextDialog( pDialog );
	m_Dialogs[m_Dialogs.GetSize() - 1]->SetNextDialog( m_Dialogs[0] );

	return true;
}


//--------------------------------------------------------------------------------------
void CDialogRscMgr::UnregisterDialog( CDXUTDialog* pDialog )
{
	// Search for the dialog in the list.
	for( int i = 0; i < m_Dialogs.GetSize(); ++i )
		if( m_Dialogs.GetAt( i ) == pDialog )
		{
			m_Dialogs.Remove( i );
			if( m_Dialogs.GetSize() > 0 )
			{
				int l, r;

				if( 0 == i )
					l = m_Dialogs.GetSize() - 1;
				else
					l = i - 1;

				if( m_Dialogs.GetSize() == i )
					r = 0;
				else
					r = i;

				m_Dialogs[l]->SetNextDialog( m_Dialogs[r] );
			}
			return;
		}
}


//--------------------------------------------------------------------------------------
void CDialogRscMgr::EnableKeyboardInputForAllDialogs()
{
	// Enable keyboard input for all registered dialogs
	for( int i = 0; i < m_Dialogs.GetSize(); ++i )
		m_Dialogs[i]->EnableKeyboardInput( true );
}


//--------------------------------------------------------------------------------------
int CDialogRscMgr::AddFont( LPCWSTR strFaceName, LONG height, LONG weight )
{
	// See if this font already exists
	for( int i = 0; i < m_FontCache.GetSize(); i++ )
	{
		DXUTFontNode* pFontNode = m_FontCache.GetAt( i );
		size_t nLen = 0;
		nLen = wcslen( strFaceName );
		if( 0 == _wcsnicmp( pFontNode->strFace, strFaceName, nLen ) &&
			pFontNode->nHeight == height &&
			pFontNode->nWeight == weight )
		{
			return i;
		}
	}

	// Add a new font and try to create it
	DXUTFontNode* pNewFontNode = new DXUTFontNode;
	if( pNewFontNode == NULL )
		return -1;

	ZeroMemory( pNewFontNode, sizeof( DXUTFontNode ) );
	wcscpy_s( pNewFontNode->strFace, MAX_PATH, strFaceName );
	pNewFontNode->nHeight = height;
	pNewFontNode->nWeight = weight;
	m_FontCache.Add( pNewFontNode );

	int iFont = m_FontCache.GetSize() - 1;

	// If a device is available, try to create immediately
	if( m_pd3d9Device )
		CreateFont9( iFont );
	if( m_pd3d10Device )
		CreateFont10( iFont );

	return iFont;
}


//--------------------------------------------------------------------------------------
int CDialogRscMgr::AddTexture( LPCWSTR strFilename )
{
	// See if this texture already exists
	for( int i = 0; i < m_TextureCache.GetSize(); i++ )
	{
		DXUTTextureNode* pTextureNode = m_TextureCache.GetAt( i );
		size_t nLen = 0;
		nLen = wcslen( strFilename );
		if( pTextureNode->bFileSource &&  // Sources must match
			0 == _wcsnicmp( pTextureNode->strFilename, strFilename, nLen ) )
		{
			return i;
		}
	}

	// Add a new texture and try to create it
	DXUTTextureNode* pNewTextureNode = new DXUTTextureNode;
	if( pNewTextureNode == NULL )
		return -1;

	ZeroMemory( pNewTextureNode, sizeof( DXUTTextureNode ) );
	pNewTextureNode->bFileSource = true;
	wcscpy_s( pNewTextureNode->strFilename, MAX_PATH, strFilename );

	m_TextureCache.Add( pNewTextureNode );

	int iTexture = m_TextureCache.GetSize() - 1;

	// If a device is available, try to create immediately
	if( m_pd3d9Device )
		CreateTexture9( iTexture );

	return iTexture;
}


//--------------------------------------------------------------------------------------
int CDialogRscMgr::AddTexture( LPCWSTR strResourceName, HMODULE hResourceModule )
{
	// See if this texture already exists   이미 존재하는 텍스쳐인지 확인 [2014.11.3 ]
	for( int i = 0; i < m_TextureCache.GetSize();  i++ )
	{
		DXUTTextureNode* pTextureNode = m_TextureCache.GetAt( i );
		if( (!pTextureNode->bFileSource) &&  /*Sources must match*/ ( pTextureNode->hResourceModule == hResourceModule) /*Module handles must match*/ )  
		{
			if( IS_INTRESOURCE( strResourceName ) )
			{
				// Integer-based ID
				if( ( INT_PTR )strResourceName == pTextureNode->nResourceID )
					return i;
			}
			else
			{
				// String-based ID
				size_t nLen = 0;
				nLen = wcslen( strResourceName );
				if( 0 == _wcsnicmp( pTextureNode->strFilename, strResourceName, nLen ) )
					return i;
			}
		}
	}

	// Add a new texture and try to create it
	DXUTTextureNode* pNewTextureNode = new DXUTTextureNode;
	if( pNewTextureNode == NULL )
		return -1;

	ZeroMemory( pNewTextureNode, sizeof( DXUTTextureNode ) );
	pNewTextureNode->hResourceModule = hResourceModule;
	if( IS_INTRESOURCE( strResourceName ) )
	{
		pNewTextureNode->nResourceID = ( int )( size_t )strResourceName;
	}
	else
	{
		pNewTextureNode->nResourceID = 0;
		wcscpy_s( pNewTextureNode->strFilename, MAX_PATH, strResourceName );   // int 아이디가 아니라면 스트링아이디이고 
																			   // 이럴경우 텍스쳐 경로가 입력되야함 [2014.11.3 ]
	}

	m_TextureCache.Add( pNewTextureNode );     //방금만든 텍스쳐 노드를 텍스쳐 캐시에 저장한다. [2014.11.3 ]

	int iTexture = m_TextureCache.GetSize() - 1;

	// If a device is available, try to create immediately
	if( m_pd3d9Device )
		CreateTexture9( iTexture ); //텍스쳐 생성해주는 함수,, 텍스쳐 아이디를 인자로 넘겨준다.  [2014.11.3 ]

	return iTexture;
}


//--------------------------------------------------------------------------------------
HRESULT CDialogRscMgr::CreateFont9( UINT iFont )
{
	HRESULT hr = S_OK;

	DXUTFontNode* pFontNode = m_FontCache.GetAt( iFont );

	SAFE_RELEASE( pFontNode->pFont9 );

	V_RETURN( D3DXCreateFont( m_pd3d9Device, pFontNode->nHeight, 0, pFontNode->nWeight, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		pFontNode->strFace, &pFontNode->pFont9 ) );

	return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDialogRscMgr::CreateFont10( UINT iFont )
{
	HRESULT hr = S_OK;

	DXUTFontNode* pFontNode = m_FontCache.GetAt( iFont );

	SAFE_RELEASE( pFontNode->pFont10 );

	V_RETURN( D3DX10CreateFont( m_pd3d10Device, pFontNode->nHeight, 0, pFontNode->nWeight, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		pFontNode->strFace, &pFontNode->pFont10 ) );

	return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDialogRscMgr::CreateTexture9( UINT iTexture )
{
	HRESULT hr = S_OK;

	DXUTTextureNode* pTextureNode = m_TextureCache.GetAt( iTexture );


	D3DXIMAGE_INFO info;

	if( !pTextureNode->bFileSource )
	{
		if( pTextureNode->nResourceID == 0xFFFF && pTextureNode->hResourceModule == ( HMODULE )0xFFFF )
		{
			hr = DXUTCreateGUITextureFromInternalArray9( m_pd3d9Device, &pTextureNode->pTexture9, &info );
			if( FAILED( hr ) )
				return DXTRACE_ERR( L"D3DXCreateTextureFromFileInMemoryEx", hr );
			DXUT_SetDebugName( pTextureNode->pTexture9, "DXUT GUI Texture" );
		}
		else
		{
			LPCWSTR pID = pTextureNode->nResourceID ? ( LPCWSTR )( size_t )pTextureNode->nResourceID : pTextureNode->strFilename;

		// Create texture from resource
		hr = D3DXCreateTextureFromResourceEx( m_pd3d9Device, pTextureNode->hResourceModule, pID, D3DX_DEFAULT,
												D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
												D3DX_DEFAULT, D3DX_DEFAULT, 0, &info, NULL, &pTextureNode->pTexture9 );

		if( FAILED( hr ) )
			return DXTRACE_ERR( L"D3DXCreateTextureFromResourceEx", hr );
		}
	}
	else
	{
		// Make sure there's a texture to create
		if( pTextureNode->strFilename[0] == 0 )
			return S_OK;

		// Create texture from file
		hr = D3DXCreateTextureFromFileEx( m_pd3d9Device, pTextureNode->strFilename, D3DX_DEFAULT, D3DX_DEFAULT,
											1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,	D3DX_DEFAULT, D3DX_DEFAULT, 0,
											&info, NULL, &pTextureNode->pTexture9 );
		if( FAILED( hr ) )
		{	return DXTRACE_ERR( L"D3DXCreateTextureFromFileEx", hr );	}
	}

	// Store dimensions
	pTextureNode->dwWidth = info.Width;
	pTextureNode->dwHeight = info.Height;

	return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDialogRscMgr::CreateTexture10( UINT iTexture )
{
	HRESULT hr = S_OK;

	DXUTTextureNode* pTextureNode = m_TextureCache.GetAt( iTexture );

	if( !pTextureNode->bFileSource )
	{
		if( pTextureNode->nResourceID == 0xFFFF && pTextureNode->hResourceModule == ( HMODULE )0xFFFF )
		{
			hr = DXUTCreateGUITextureFromInternalArray10( m_pd3d10Device, &pTextureNode->pTexture10, NULL );
			if( FAILED( hr ) )
				return DXTRACE_ERR( L"D3DX10CreateResourceFromFileInMemory", hr );
			DXUT_SetDebugName( pTextureNode->pTexture10, "DXUT GUI Texture" );
		}
		else
		{
			LPCWSTR pID = pTextureNode->nResourceID ? ( LPCWSTR )( size_t )pTextureNode->nResourceID :
				pTextureNode->strFilename;

		D3DX10_IMAGE_INFO SrcInfo;
		D3DX10GetImageInfoFromResource( NULL, pID, NULL, &SrcInfo, NULL );

		// Create texture from resource
		ID3D10Resource* pRes;
		D3DX10_IMAGE_LOAD_INFO loadInfo;
		loadInfo.Width = D3DX10_DEFAULT;
		loadInfo.Height = D3DX10_DEFAULT;
		loadInfo.Depth = D3DX10_DEFAULT;
		loadInfo.FirstMipLevel = 0;
		loadInfo.MipLevels = 1;
		loadInfo.Usage = D3D10_USAGE_DEFAULT;
		loadInfo.BindFlags = D3D10_BIND_SHADER_RESOURCE;
		loadInfo.CpuAccessFlags = 0;
		loadInfo.MiscFlags = 0;
		loadInfo.Format = MAKE_TYPELESS( SrcInfo.Format );
		loadInfo.Filter = D3DX10_FILTER_NONE;
		loadInfo.MipFilter = D3DX10_FILTER_NONE;
		loadInfo.pSrcInfo = &SrcInfo;

		hr = D3DX10CreateTextureFromResource( m_pd3d10Device, pTextureNode->hResourceModule, pID, &loadInfo,
			NULL, &pRes, NULL );
		if( FAILED( hr ) )
			return DXTRACE_ERR( L"D3DX10CreateResourceFromResource", hr );
		DXUT_SetDebugName( pRes, "DXUT GUI Texture" );
		hr = pRes->QueryInterface( __uuidof( ID3D10Texture2D ), ( LPVOID* )&pTextureNode->pTexture10 );
		SAFE_RELEASE( pRes );
		if( FAILED( hr ) )
			return hr;
		}
	}
	else
	{
		// Make sure there's a texture to create
		if( pTextureNode->strFilename[0] == 0 )
			return S_OK;

		D3DX10_IMAGE_INFO SrcInfo;
		D3DX10GetImageInfoFromFile( pTextureNode->strFilename, NULL, &SrcInfo, NULL );

		// Create texture from file
		ID3D10Resource* pRes;
		D3DX10_IMAGE_LOAD_INFO loadInfo;
		loadInfo.Width = D3DX10_DEFAULT;
		loadInfo.Height = D3DX10_DEFAULT;
		loadInfo.Depth = D3DX10_DEFAULT;
		loadInfo.FirstMipLevel = 0;
		loadInfo.MipLevels = 1;
		loadInfo.Usage = D3D10_USAGE_DEFAULT;
		loadInfo.BindFlags = D3D10_BIND_SHADER_RESOURCE;
		loadInfo.CpuAccessFlags = 0;
		loadInfo.MiscFlags = 0;
		loadInfo.Format = MAKE_TYPELESS( SrcInfo.Format );
		loadInfo.Filter = D3DX10_FILTER_NONE;
		loadInfo.MipFilter = D3DX10_FILTER_NONE;
		loadInfo.pSrcInfo = &SrcInfo;
		hr = D3DX10CreateTextureFromFile( m_pd3d10Device, pTextureNode->strFilename, &loadInfo, NULL, &pRes, NULL );
		if( FAILED( hr ) )
		{
			return DXTRACE_ERR( L"D3DX10CreateResourceFromFileEx", hr );
		}
		DXUT_SetDebugName( pRes, "DXUT GUI Texture" );
		hr = pRes->QueryInterface( __uuidof( ID3D10Texture2D ), ( LPVOID* )&pTextureNode->pTexture10 );
		SAFE_RELEASE( pRes );
		if( FAILED( hr ) )
			return hr;
	}

	// Store dimensions
	D3D10_TEXTURE2D_DESC desc;
	pTextureNode->pTexture10->GetDesc( &desc );
	pTextureNode->dwWidth = desc.Width;
	pTextureNode->dwHeight = desc.Height;

	// Create resource view
	D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Format = MAKE_SRGB( desc.Format );
	SRVDesc.Texture2D.MipLevels = 1;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	hr = m_pd3d10Device->CreateShaderResourceView( pTextureNode->pTexture10, &SRVDesc, &pTextureNode->pTexResView );
	DXUT_SetDebugName( pTextureNode->pTexResView, "DXUT GUI Texture" );

	return hr;
}

//-----------------------------------------------------------------------------
// Arg    : int _UI_ID
// Return : GUI_Base*	
// Desc   : UI고유아이디로 해당컨트롤을 찾아준다.				 [2014.11.5 ]
//-----------------------------------------------------------------------------
GUI_Base*	CDialogRscMgr::GetControl(int _UI_ID) //
{
	
	int GA_DlgsSize = m_Dialogs.GetSize(); // GrowableArray size 를 구한다.
	int controlSize;

	// 같은아이디가 하나라도 있는지 검사 [2014.11.5 ]
	for(int i = 0; i < GA_DlgsSize ; ++i)
	{
		CDXUTDialog* pDlg  = m_Dialogs.GetAt(i);
		controlSize  = pDlg->m_Controls.GetSize();
		for(int ControlNum = 0 ; ControlNum < controlSize ; ControlNum++)
		{
			GUI_Base* pGB = pDlg->m_Controls.GetAt(ControlNum);

			if(_UI_ID == pGB->GetID())
			{
				return pGB;
			}
		}
	}

	//같은 아이가 없다면 nullptr을 반환 [2014.11.5 ]
	return nullptr;
}

/****************************************************** End *****************************************************/