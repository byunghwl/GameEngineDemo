/***********************************************************************
                      SH_GUI.cpp
			      // [2014.10.31 ]
************************************************************************/


#include "DXUT.h"
#include "SH_UI.h"


//////////////////////////////////////////////////////////////////////////
//  [2014.12.4 ]
inline int RectWidth( RECT& rc )
{
	return ( ( rc ).right - ( rc ).left );
}
inline int RectHeight( RECT& rc )
{
	return ( ( rc ).bottom - ( rc ).top );
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
// ���⼭���� class GUI_Base �Լ� ����  // [2014.10.31 ]
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
// Arg	  :  CDXUTDialog* pDialog  (�� ����ƽ�� ���� ���̾�α�) �����ڿ��� ������̾�α׿� ������ �����Ѵ�.// [2014.10.31 ]
// Return :  
// Desc   : �����Ҷ� �����ڿ��� ������̾�α׿� ������ �����Ѵ�.  // [2014.10.31 ]
//-------------------------------------------------------------------------
CDXUTStatic::CDXUTStatic( CDXUTDialog* pDialog )
{
	m_Type = DXUT_CONTROL_STATIC;
	m_pDialog = pDialog;

	ZeroMemory( &m_strText, sizeof( m_strText ) );  // [2014.10.31 ]

}


//-------------------------------------------------------------------------
// Arg	  :  float fElapsedTime  
// Return :
// Desc   :  �׸��� �Լ�  // [2014.10.31 ]
//-------------------------------------------------------------------------
void CDXUTStatic::Render( float fElapsedTime )
{
	if( m_bVisible == false )
		return;

	DXUT_CONTROL_STATE iState = DXUT_STATE_NORMAL;

	if( m_bEnabled == false )
		iState = DXUT_STATE_DISABLED;
	
	 CDXUTElement* pElement = m_Elements.GetAt( 0 );

	pElement->FontColor.Blend( iState, fElapsedTime );

	m_pDialog->DrawText( m_strText, pElement, &m_rcBoundingBox, true );
}


//-------------------------------------------------------------------------
// <Out> strDest , �� �����  [2014.11.2 ]
// 
//-------------------------------------------------------------------------
HRESULT CDXUTStatic::GetTextCopy( __out_ecount(bufferCount) LPWSTR strDest, 
	UINT bufferCount )
{
	// Validate incoming parameters
	if( strDest == NULL || bufferCount == 0 )
	{
		return E_INVALIDARG;
	}

	// Copy the window text
	wcscpy_s( strDest, bufferCount, m_strText );

	return S_OK;
}


//-------------------------------------------------------------------------
// <In> LPCWSTR strText  [2014.11.2 ]
// 
//-------------------------------------------------------------------------
HRESULT CDXUTStatic::SetText( LPCWSTR strText )
{
	if( strText == NULL )
	{
		m_strText[0] = 0;
		return S_OK;
	}

	wcscpy_s( m_strText, MAX_PATH, strText );
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
// ���⼭���� class CDXUTImage �Լ�����  //[2014.12.4 ]
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
// Arg    : CDXUTDialog* pDialog
// Return : 
// Desc   : �����Ҷ� �ٿ��� ���̾�α׸� �ʿ����  [2014.11.1 ]
//-----------------------------------------------------------------------------
CDXUTImage::CDXUTImage( CDXUTDialog* pDialog )
{
	m_Type = DXUT_CONTROL_IMAGE;
	m_pDialog = pDialog;


	
	m_nHotkey = 0;
}

//-----------------------------------------------------------------------------
// Arg    : UINT uMsg, WPARAM wParam, LPARAM lParam  [2014.11.1 ]
// Return : bool
// Desc   : 
//-----------------------------------------------------------------------------
// bool CDXUTImage::HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam )
// {
// 	if( !m_bEnabled || !m_bVisible )
// 		return false;
// 
// 	switch( uMsg )
// 	{
// 	case WM_KEYDOWN:
// 		{
// 			switch( wParam )
// 			{
// 			case VK_SPACE:
// 				//m_bPressed = true;
// 				return true;
// 			}
// 		}
// 
// 	case WM_KEYUP:
// 		{
// 			switch( wParam )
// 			{
// 			case VK_SPACE:
// // 				if( m_bPressed == true )
// // 				{
// // 					m_bPressed = false;
// // 					m_pDialog->SendEvent( EVENT_BUTTON_CLICKED, true, this );
// /*				}*/
// 				return true;
// 			}
// 		}
// 	}
// 	return false;
// }


//-----------------------------------------------------------------------------
// Arg    : UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam  [2014.11.1 ]
// Return : 
// Desc   :
//-----------------------------------------------------------------------------
// bool CDXUTImage::HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam )
// {
// 	if( !m_bEnabled || !m_bVisible )
// 		return false;
// 
// // 	switch( uMsg )
// // 	{
// // 	case WM_LBUTTONDOWN:
// // 	case WM_LBUTTONDBLCLK:
// // 		{
// // 			if( ContainsPoint( pt ) )
// // 			{
// // 				// Pressed while inside the control
// // 				m_bPressed = true;
// // 				SetCapture( DXUTGetHWND() );
// // 
// // 				if( !m_bHasFocus )
// // 					m_pDialog->RequestFocus( this );
// // 
// // 				return true;
// // 			}
// // 
// // 			break;
// // 		}
// // 
// // 	case WM_LBUTTONUP:
// // 		{
// // 			if( m_bPressed )
// // 			{
// // 				m_bPressed = false;
// // 				ReleaseCapture();
// // 
// // 				if( !m_pDialog->m_bKeyboardInput )
// // 					m_pDialog->ClearFocus();
// // 
// // 				// Button click
// // 				if( ContainsPoint( pt ) )
// // 					m_pDialog->SendEvent( EVENT_BUTTON_CLICKED, true, this );
// // 
// // 				// 				SH_MSG_from_UI BtnMsg; 
// // 				// 				BtnMsg.m_UI_id = m_ID;
// // 				// 				BtnMsg.m_WinMsg = uMsg;
// // 				// 				BtnMsg.m_lParam = lParam;
// // 				// 				BtnMsg.m_wParam = wParam;
// // 				// 				m_pDialog->m_pMediator->Notify(&BtnMsg );
// // 				//�ӽ� 
// // 				//SHUtil::MsgBox("test �ϴ�");
// // 
// // 				return true;
// // 			}
// // 
// // 			break;
// // 		}
// // 	};
// 
// 	return false;
// }


//-----------------------------------------------------------------------------
// Arg    : float fElapsedTime    [2014.11.1 ]
// Return : 
// Desc   : 
//-----------------------------------------------------------------------------
void CDXUTImage::Render( float fElapsedTime )
{
	int nOffsetX = 0;
	int nOffsetY = 0;

	DXUT_CONTROL_STATE iState = DXUT_STATE_NORMAL;

	if( m_bVisible == false )
	{
		iState = DXUT_STATE_HIDDEN;
	}
	else if( m_bEnabled == false )
	{
		iState = DXUT_STATE_DISABLED;
	}
	else if( m_bMouseOver )
	{
		iState = DXUT_STATE_MOUSEOVER;

		nOffsetX = -1;
		nOffsetY = -2;
	}
	else if( m_bHasFocus )
	{
		iState = DXUT_STATE_FOCUS;
	}

	// Background fill layer
	CDXUTElement* pElement = m_Elements.GetAt( m_Ele_Idx_in );

	float fBlendRate = ( iState == DXUT_STATE_PRESSED ) ? 0.0f : 0.8f;
	fBlendRate = ( iState == DXUT_STATE_DISABLED ) ? 0.0f : 0.8f;


	RECT rcWindow = m_rcBoundingBox;
	OffsetRect( &rcWindow, nOffsetX, nOffsetY );


	// Blend current color
	pElement->TextureColor.Blend( iState , fElapsedTime, fBlendRate );
	pElement->FontColor.Blend( iState , fElapsedTime, fBlendRate );

	m_pDialog->DrawSprite( pElement, &rcWindow, DXUT_FAR_BUTTON_DEPTH );
	m_pDialog->DrawText( m_strText, pElement, &rcWindow );

	// Main button
	pElement = m_Elements.GetAt(m_Ele_Idx_Out  );

	// Blend current color
	pElement->TextureColor.Blend( iState, fElapsedTime, fBlendRate );
	pElement->FontColor.Blend( iState, fElapsedTime, fBlendRate );

	m_pDialog->DrawSprite( pElement, &rcWindow, DXUT_NEAR_BUTTON_DEPTH );
	m_pDialog->DrawText( m_strText, pElement, &rcWindow );
}

//-------------------------------------------------------------------------
// <In> LPCWSTR strText  [2014.11.2 ]
// 
//-------------------------------------------------------------------------
HRESULT CDXUTImage::SetText( LPCWSTR strText )
{
	if( strText == NULL )
	{
		m_strText[0] = 0;
		return S_OK;
	}

	wcscpy_s( m_strText, MAX_PATH, strText );
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
// ���⼭���� class CDXUTButton �Լ�����  [2014.11.1 ]
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
// Arg    : CDXUTDialog* pDialog
// Return : 
// Desc   : �����Ҷ� �ٿ��� ���̾�α׸� �ʿ����  [2014.11.1 ]
//-----------------------------------------------------------------------------
CDXUTButton::CDXUTButton( CDXUTDialog* pDialog )
{
	m_Type = DXUT_CONTROL_BUTTON;
	m_pDialog = pDialog;


	m_bPressed = false;
	m_nHotkey = 0;
}

//-----------------------------------------------------------------------------
// Arg    : UINT uMsg, WPARAM wParam, LPARAM lParam  [2014.11.1 ]
// Return : bool
// Desc   : 
//-----------------------------------------------------------------------------
bool CDXUTButton::HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( !m_bEnabled || !m_bVisible )
		return false;

	switch( uMsg )
	{
	case WM_KEYDOWN:
		{
			switch( wParam )
			{
			case VK_SPACE:
				m_bPressed = true;
				return true;
			}
		}

	case WM_KEYUP:
		{
			switch( wParam )
			{
			case VK_SPACE:
				if( m_bPressed == true )
				{
					m_bPressed = false;
					m_pDialog->SendEvent( EVENT_BUTTON_CLICKED, true, this );
				}
				return true;
			}
		}
	}
	return false;
}


//-----------------------------------------------------------------------------
// Arg    : UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam  [2014.11.1 ]
// Return : 
// Desc   :
//-----------------------------------------------------------------------------
bool CDXUTButton::HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam )
{
	if( !m_bEnabled || !m_bVisible )
		return false;

	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		{
			if( ContainsPoint( pt ) )
			{
				// Pressed while inside the control
				m_bPressed = true;
				SetCapture( DXUTGetHWND() );

				if( !m_bHasFocus )
					m_pDialog->RequestFocus( this );

				return true;
			}

			break;
		}

	case WM_LBUTTONUP:
		{
			if( m_bPressed )
			{
				m_bPressed = false;
				ReleaseCapture();

				if( !m_pDialog->m_bKeyboardInput )
					m_pDialog->ClearFocus();

				// Button click
				if( ContainsPoint( pt ) )
					m_pDialog->SendEvent( EVENT_BUTTON_CLICKED, true, this );

// 				SH_MSG_from_UI BtnMsg; 
// 				BtnMsg.m_UI_id = m_ID;
// 				BtnMsg.m_WinMsg = uMsg;
// 				BtnMsg.m_lParam = lParam;
// 				BtnMsg.m_wParam = wParam;
// 				m_pDialog->m_pMediator->Notify(&BtnMsg );
				//�ӽ� 
				//SHUtil::MsgBox("test �ϴ�");

				return true;
			}

			break;
		}
	};

	return false;
}


//-----------------------------------------------------------------------------
// Arg    : float fElapsedTime    [2014.11.1 ]
// Return : 
// Desc   : 
//-----------------------------------------------------------------------------
void CDXUTButton::Render( float fElapsedTime )
{
	int nOffsetX = 0;
	int nOffsetY = 0;

	DXUT_CONTROL_STATE iState = DXUT_STATE_NORMAL;

	if( m_bVisible == false )
	{
		iState = DXUT_STATE_HIDDEN;
	}
	else if( m_bEnabled == false )
	{
		iState = DXUT_STATE_DISABLED;
	}
	else if( m_bPressed )
	{
		iState = DXUT_STATE_PRESSED;

		nOffsetX = 1;
		nOffsetY = 2;
		
	}
	else if( m_bMouseOver )
	{
		iState = DXUT_STATE_MOUSEOVER;

		nOffsetX = -1;
		nOffsetY = -2;
	}
	else if( m_bHasFocus )
	{
		iState = DXUT_STATE_FOCUS;
	}

	// Background fill layer
	CDXUTElement* pElement = m_Elements.GetAt( m_Ele_Idx_in );

	float fBlendRate = ( iState == DXUT_STATE_PRESSED ) ? 0.0f : 0.8f;

	RECT rcWindow = m_rcBoundingBox;
	OffsetRect( &rcWindow, nOffsetX, nOffsetY );


	// Blend current color
	pElement->TextureColor.Blend( iState, fElapsedTime, fBlendRate );
	pElement->FontColor.Blend( iState, fElapsedTime, fBlendRate );

	m_pDialog->DrawSprite( pElement, &rcWindow, DXUT_FAR_BUTTON_DEPTH );
	m_pDialog->DrawText( m_strText, pElement, &rcWindow );

	// Main button
	pElement = m_Elements.GetAt(m_Ele_Idx_Out  );

	// Blend current color
	pElement->TextureColor.Blend( iState, fElapsedTime, fBlendRate );
	pElement->FontColor.Blend( iState, fElapsedTime, fBlendRate );
	
	m_pDialog->DrawSprite( pElement, &rcWindow, DXUT_NEAR_BUTTON_DEPTH );
	m_pDialog->DrawText( m_strText, pElement, &rcWindow );
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
// ���⼭���� class CDXUTProgressBar �Լ�����  [2014.11.4 ]
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
// Arg    : CDXUTDialog* pDialog
// Return : 
// Desc   : �����Ҷ� �ٿ��� ���̾�α׸� �ʿ����  [2014.11.1 ]
//-----------------------------------------------------------------------------
CDXUTProgressBar::CDXUTProgressBar( CDXUTDialog* pDialog )
{
	m_Type = DXUT_CONTROL_PROGRESSBAR;
	m_pDialog = pDialog;
	m_Percentage = 15;  // [2014.11.5 ]
	m_bPressed = false;
	m_nHotkey = 0;
	m_EleIdx_In = 0;
	m_EleIdx_Out = 0; // [2014.12.2 ]
}

//-----------------------------------------------------------------------------
// Arg    : UINT uMsg, WPARAM wParam, LPARAM lParam  [2014.11.1 ]
// Return : bool
// Desc   : 
//-----------------------------------------------------------------------------
bool CDXUTProgressBar::HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( !m_bEnabled || !m_bVisible )
		return false;

	switch( uMsg )
	{
	case WM_KEYDOWN:
		{
			switch( wParam )
			{
			case VK_SPACE:
				m_bPressed = true;
				return true;
			}
		}

	case WM_KEYUP:
		{
			switch( wParam )
			{
			case VK_SPACE:
				if( m_bPressed == true )
				{
					m_bPressed = false;
					m_pDialog->SendEvent( EVENT_BUTTON_CLICKED, true, this );
				}
				return true;
			}
		}
	}
	return false;
}


//-----------------------------------------------------------------------------
// Arg    : UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam  [2014.11.1 ]
// Return : 
// Desc   :
//-----------------------------------------------------------------------------
bool CDXUTProgressBar::HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam )
{
	if( !m_bEnabled || !m_bVisible )
		return false;

	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		{
// 			if( ContainsPoint( pt ) )
// 			{
// 				// Pressed while inside the control
// 				m_bPressed = true;
// 				SetCapture( DXUTGetHWND() );
// 
// 				if( !m_bHasFocus )
// 					m_pDialog->RequestFocus( this );
// 
// 				return true;
// 			}

			break;
		}

	case WM_LBUTTONUP:
		{
// 			if( m_bPressed )
// 			{
// 				m_bPressed = false;
// 				ReleaseCapture();
// 
// 				if( !m_pDialog->m_bKeyboardInput )
// 					m_pDialog->ClearFocus();
// 
// 				// Button click
// 				if( ContainsPoint( pt ) )
// 					m_pDialog->SendEvent( EVENT_BUTTON_CLICKED, true, this );
// 
// 				//m_pDialog->m_pMediator->Notify(this, uMsg, lParam, wParam );
// 				//�ӽ� 
// 
// 
// 				return true;
			//}

			break;
		}
	};

	return false;
}


//-----------------------------------------------------------------------------
// Arg    : float fElapsedTime    [2014.11.1 ]
// Return : 
// Desc   : 
//-----------------------------------------------------------------------------
void CDXUTProgressBar::Render( float fElapsedTime )
{
	int nOffsetX = 0;
	int nOffsetY = 0;

	DXUT_CONTROL_STATE iState = DXUT_STATE_NORMAL;

	if( m_bVisible == false )
	{
		iState = DXUT_STATE_HIDDEN;
	}
	else if( m_bEnabled == false )
	{
		iState = DXUT_STATE_DISABLED;
	}
	else if( m_bPressed )
	{
		iState = DXUT_STATE_PRESSED;

		nOffsetX = 1;
		nOffsetY = 2;
	}
	else if( m_bMouseOver )
	{
		iState = DXUT_STATE_MOUSEOVER;

		nOffsetX = -1;
		nOffsetY = -2;
	}
	else if( m_bHasFocus )
	{
		iState = DXUT_STATE_FOCUS;
	}

	// Background fill layer
	CDXUTElement* pElement = m_Elements.GetAt( m_EleIdx_In);

	float fBlendRate = ( iState == DXUT_STATE_PRESSED ) ? 0.0f : 0.8f;

	RECT rcWindow = m_rcBoundingBox;
	OffsetRect( &rcWindow, nOffsetX, nOffsetY );
	
	// Blend current color
	//pElement->TextureColor.Blend( iState, fElapsedTime, fBlendRate );
	//pElement->FontColor.Blend( iState, fElapsedTime, fBlendRate );

	float per = 0.0f;
	if(m_Percentage < 0.000001f)
	{	m_Percentage = 1.0f ;}
	else
	{ per = m_Percentage / 100.0f;}


// 	if(m_ID == eUI_ID_SkillPointBar)
// 	{		
// 		if(m_Percentage<33.4f)
// 		{
// 			rcWindow.right = m_rcBoundingBox.left + m_width * (per+0.09);
// 		}
// 		else
// 		{
// 			rcWindow.right = m_rcBoundingBox.left + m_width * per;
// 		}
// 		
// 	}
// 	else
// 	{
		rcWindow.right = m_rcBoundingBox.left + m_width * per;
/*	}*/
	

	m_pDialog->DrawSprite( pElement, &rcWindow, DXUT_FAR_BUTTON_DEPTH );
	//m_pDialog->DrawText( m_strText, pElement, &rcWindow );

	// Main button
	pElement = m_Elements.GetAt( m_EleIdx_Out );
	rcWindow = m_rcBoundingBox;
	OffsetRect( &rcWindow, nOffsetX, nOffsetY );
	// Blend current color
	//pElement->TextureColor.Blend( iState, fElapsedTime, fBlendRate );
	//pElement->FontColor.Blend( iState, fElapsedTime, fBlendRate );

	m_pDialog->DrawSprite( pElement, &rcWindow, DXUT_NEAR_BUTTON_DEPTH );
	//m_pDialog->DrawText( m_strText, pElement, &rcWindow );
}

//-------------------------------------------------------------------------
// Arg	  : int _val (0~100 )������ ��
// Return : 
// Desc   : �ܺο��� ���α׷������� �ۼ��������� ������ �� �ְ� ���ֱ� ���� �Լ�. 
//-------------------------------------------------------------------------
void	CDXUTProgressBar::SetPercentage(float _val)// [2014.11.5 ]
{


	if(  _val < 0.000001f)
	{	
		m_Percentage = 0.0001f;// LOG_WRITE_A(" CDXUTProgressBar::SetPercentage () , 0���� ���� ���� ���Դ�.");
		return ;
	}
	if( _val > 100.00001f)
	{	
		m_Percentage = 100.0f; //LOG_WRITE_A(" CDXUTProgressBar::SetPercentage () , 100���� ū���� ���Դ�");
		return ;
	}
	
		m_Percentage = _val;
	

	
	

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
// ���⼭���� CDXUTSlider Ŭ���� �Լ�����  [2014.12.4 ]
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



//--------------------------------------------------------------------------------------
CDXUTSlider::CDXUTSlider( CDXUTDialog* pDialog )
{
	m_Type = DXUT_CONTROL_SLIDER;
	m_pDialog = pDialog;

	m_nMin = 0;
	m_nMax = 100;
	m_nValue = 50;

	m_bPressed = false;
}


//--------------------------------------------------------------------------------------
BOOL CDXUTSlider::ContainsPoint( POINT pt )
{
	return ( PtInRect( &m_rcBoundingBox, pt ) ||
		PtInRect( &m_rcButton, pt ) );
}


//--------------------------------------------------------------------------------------
void CDXUTSlider::UpdateRects()
{
	GUI_Base::UpdateRects();

	m_rcButton = m_rcBoundingBox;
	m_rcButton.right = m_rcButton.left + RectHeight( m_rcButton );
	OffsetRect( &m_rcButton, -RectWidth( m_rcButton ) / 2, 0 );

	m_nButtonX = ( int )( ( m_nValue - m_nMin+5 ) * ( float )RectWidth( m_rcBoundingBox ) / ( m_nMax -5- m_nMin ) );
	OffsetRect( &m_rcButton, m_nButtonX, 0 );
}

int CDXUTSlider::ValueFromPos( int x )
{
	float fValuePerPixel = ( float )( m_nMax - m_nMin ) / RectWidth( m_rcBoundingBox );
	return ( int )( 0.5f + m_nMin + fValuePerPixel * ( x - m_rcBoundingBox.left ) );
}

//--------------------------------------------------------------------------------------
bool CDXUTSlider::HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( !m_bEnabled || !m_bVisible )
		return false;

	switch( uMsg )
	{
	case WM_KEYDOWN:
		{
			switch( wParam )
			{
			case VK_HOME:
				SetValueInternal( m_nMin, true );
				return true;

			case VK_END:
				SetValueInternal( m_nMax, true );
				return true;

			case VK_LEFT:
			case VK_DOWN:
				SetValueInternal( m_nValue - 1, true );
				return true;

			case VK_RIGHT:
			case VK_UP:
				SetValueInternal( m_nValue + 1, true );
				return true;

			case VK_NEXT:
				SetValueInternal( m_nValue - ( 10 > ( m_nMax - m_nMin ) / 10 ? 10 : ( m_nMax - m_nMin ) / 10 ),
					true );
				return true;

			case VK_PRIOR:
				SetValueInternal( m_nValue + ( 10 > ( m_nMax - m_nMin ) / 10 ? 10 : ( m_nMax - m_nMin ) / 10 ),
					true );
				return true;
			}
			break;
		}
	}


	return false;
}


//--------------------------------------------------------------------------------------
bool CDXUTSlider::HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam )
{
	if( !m_bEnabled || !m_bVisible )
		return false;

	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		{
			if( PtInRect( &m_rcButton, pt ) )
			{
				// Pressed while inside the control
				m_bPressed = true;
				SetCapture( DXUTGetHWND() );

				m_nDragX = pt.x;
				//m_nDragY = pt.y;
				m_nDragOffset = m_nButtonX - m_nDragX;

				//m_nDragValue = m_nValue;

				if( !m_bHasFocus )
					m_pDialog->RequestFocus( this );

				return true;
			}

			if( PtInRect( &m_rcBoundingBox, pt ) )
			{
				m_nDragX = pt.x;
				m_nDragOffset = 0;
				m_bPressed = true;

				if( !m_bHasFocus )
					m_pDialog->RequestFocus( this );

				if( pt.x > m_nButtonX + m_x )
				{
					SetValueInternal( m_nValue + 1, true );
					return true;
				}

				if( pt.x < m_nButtonX + m_x )
				{
					SetValueInternal( m_nValue - 1, true );
					return true;
				}
			}

			break;
		}

	case WM_LBUTTONUP:
		{
			if( m_bPressed )
			{
				m_bPressed = false;
				ReleaseCapture();
				m_pDialog->SendEvent( EVENT_SLIDER_VALUE_CHANGED, true, this );

				return true;
			}

			break;
		}

	case WM_MOUSEMOVE:
		{
			if( m_bPressed )
			{
				SetValueInternal( ValueFromPos( m_x + pt.x + m_nDragOffset ), true );
				return true;
			}

			break;
		}

	case WM_MOUSEWHEEL:
		{
			int nScrollAmount = int( ( short )HIWORD( wParam ) ) / WHEEL_DELTA;
			SetValueInternal( m_nValue - nScrollAmount, true );
			return true;
		}
	};

	return false;
}


//--------------------------------------------------------------------------------------
void CDXUTSlider::SetRange( int nMin, int nMax )
{
	m_nMin = nMin;
	m_nMax = nMax;

	SetValueInternal( m_nValue, false );
}


//--------------------------------------------------------------------------------------
void CDXUTSlider::SetValueInternal( int nValue, bool bFromInput )
{
	// Clamp to range
	nValue = __max( m_nMin, nValue );
	nValue = __min( m_nMax, nValue );

	if( nValue == m_nValue )
		return;

	m_nValue = nValue;
	UpdateRects();

	m_pDialog->SendEvent( EVENT_SLIDER_VALUE_CHANGED, bFromInput, this );
}


//--------------------------------------------------------------------------------------
void CDXUTSlider::Render( float fElapsedTime )
{
	int nOffsetX = 0;
	int nOffsetY = 0;

	DXUT_CONTROL_STATE iState = DXUT_STATE_NORMAL;

	if( m_bVisible == false )
	{
		iState = DXUT_STATE_HIDDEN;
	}
	else if( m_bEnabled == false )
	{
		iState = DXUT_STATE_DISABLED;
	}
	else if( m_bPressed )
	{
		iState = DXUT_STATE_PRESSED;

		nOffsetX = 1;
		nOffsetY = 2;
	}
	else if( m_bMouseOver )
	{
		iState = DXUT_STATE_MOUSEOVER;

		nOffsetX = -1;
		nOffsetY = -2;
	}
	else if( m_bHasFocus )
	{
		iState = DXUT_STATE_FOCUS;
	}

	//float fBlendRate = ( iState == DXUT_STATE_PRESSED ) ? 0.0f : 0.8f;

	CDXUTElement* pElement = m_Elements.GetAt( m_Ele_Stick );

	// Blend current color
	//pElement->TextureColor.Blend( iState, fElapsedTime, fBlendRate );
	m_pDialog->DrawSprite( pElement, &m_rcBoundingBox, DXUT_FAR_BUTTON_DEPTH );

	pElement = m_Elements.GetAt( m_Ele_Btn );

	// Blend current color
	//pElement->TextureColor.Blend( iState, fElapsedTime, fBlendRate );
	m_pDialog->DrawSprite( pElement, &m_rcButton, DXUT_NEAR_BUTTON_DEPTH );
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
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
/************************************************************************/
/*     �ӽù���
*/
/************************************************************************/

//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
void CALLBACK OnGUIEvent_Title( UINT nEvent, int nControlID, GUI_Base* pControl, void* pUserContext )
{
	//WCHAR wszOutput[1024]; [2014.11.19 ]

	switch( nControlID )
	{
	case eUI_ID_StartButton:
		{
			
		   CSceneMgr::getInstance()->OpenScene(L"Scene_Loading");    G_CurSceneType = eSceneType_Loading;
		   G_SHSoundMgr.StopWave(eSound_BGM_Title);
		   G_SHSoundMgr.PlayWave(eSound_BGM_Intro, true);
		   G_GameTimer =0.0f;
		   G_GirlHP = 100.0f; // ������ ü�� 
		}
		break;

	case eUI_ID_CreditButton:
		{
			CSceneMgr::getInstance()->OpenScene(L"Scene_Credit");    G_CurSceneType = eSceneType_Credit;
			 G_SHSoundMgr.StopWave(eSound_BGM_Title);
			 G_SHSoundMgr.PlayWave(eSound_BGM_Credit, true);
		}
		break;
	case eUI_ID_ExitButton:
		{
			PostQuitMessage(0);

		}
		break;
	}
}
/****************************************************** End *****************************************************/