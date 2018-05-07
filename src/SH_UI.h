/***********************************************************************
                          SH_UI.h
			    // [2014.10.31 ]
		이곳에 각종 GUI_Base 를 상속받은 각종 UI가 선언되어있다.
************************************************************************/
#pragma  once

//사용할 dds 텍스쳐 번호
enum eTextureID
{
	eTEX_ID_CustomUI = 0,
	eTEX_ID_SHUI ,           //내가만든 .dds  [2014.11.3 ]
	eTEX_ID_Title  // [2014.12.2 ]
};

//--------------------------------------------------------------------------------------
// UI control Unique IDs
//--------------------------------------------------------------------------------------
enum eUI_ID
{
	eUI_ID_StartButton=1,// 0은 초기화값이므로 아이디로 사용 불가 반드시 1이상부터 시작 [2014.11.5 ]
	eUI_ID_CreditButton,
	eUI_ID_ExitButton,
	eUI_ID_Logo,
	eUI_ID_STATIC,
	eUI_ID_SkillPointBar,
	eUI_ID_HPBar,
	eUI_ID_SkillImg1,
	eUI_ID_SkillImg2,
	eUI_ID_SkillImg3,
	
	eUI_ID_HatImg,

	eUI_ID_TimerSlider,
	eUI_ID_MAX                   //DWORD의 크기( 4294967285)를 넘어가면 안됨!! [2014.11.6 ]

};


/***********************************************************************
//						Static control  // [2014.10.31 ]
//						 글자에 해당하는 UI
// 
************************************************************************/

class CDXUTStatic : public GUI_Base
{
protected:

	WCHAR					m_strText[MAX_PATH];      // Window text  

public: // 생성자,. 소멸자

	CDXUTStatic( CDXUTDialog* pDialog = NULL );
	virtual ~CDXUTStatic(){};

public:  // 일반함수

	virtual void			Render( float fElapsedTime );
	virtual BOOL			ContainsPoint( POINT pt ){	return false;	}
	// strDest로 m_strText를 복사한다. 
	HRESULT					GetTextCopy(LPWSTR strDest,	UINT bufferCount );

public:// get.. set

	LPCWSTR					GetText() const{return m_strText;	}

	HRESULT					SetText( LPCWSTR strText );



};


/***********************************************************************
//						Static Image  // [2014.10.31 ]
//					그냥 사각 이미지 띄워야 할때
// 
************************************************************************/
class CDXUTImage : public CDXUTStatic
{
protected:
	friend class CDXUTDialog;
	int							m_Ele_Idx_in; // [2014.12.3 ]
	int							m_Ele_Idx_Out;

public: // 생성자,. 소멸자

	CDXUTImage( CDXUTDialog* pDialog = NULL );
	virtual ~CDXUTImage(){};

public:  // 일반함수

	virtual void			Render( float fElapsedTime );
	virtual BOOL			ContainsPoint( POINT pt ){	return false;	}
	// strDest로 m_strText를 복사한다. 
	HRESULT					GetTextCopy(LPWSTR strDest,	UINT bufferCount );

public:// get.. set

	LPCWSTR					GetText() const{return m_strText;	}

	HRESULT					SetText( LPCWSTR strText );



};



/***********************************************************************
                 class SHGUI_Button 
//         단순한 버튼  // [2014.10.31 ]
				
************************************************************************/
class CDXUTButton : public CDXUTStatic
{
	friend class CDXUTDialog;

protected:

	bool						m_bPressed;
	int							m_Ele_Idx_in; // [2014.12.3 ]
	int							m_Ele_Idx_Out;
public:// 생성자 소멸자

	CDXUTButton( CDXUTDialog* pDialog = NULL );

public: // 일반함수

	virtual bool				HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool				HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );
	virtual void				OnHotkey()
	{
		if( m_pDialog->IsKeyboardInputEnabled() ) m_pDialog->RequestFocus( this );
		m_pDialog->SendEvent( EVENT_BUTTON_CLICKED, true, this );
	}

	virtual BOOL    ContainsPoint( POINT pt )
	{
		return PtInRect( &m_rcBoundingBox, pt );
	}
	virtual bool    CanHaveFocus()
	{
		return ( m_bVisible && m_bEnabled );
	}

	virtual void    Render( float fElapsedTime );


};



/***********************************************************************
                 class CDXUTProgressBar 
//         프로그레스바  made by sungwhan // [2014.11.4 ]
				
************************************************************************/

class CDXUTProgressBar : public CDXUTStatic
{
protected:

	bool							m_bPressed;
	float							m_Percentage; // 프로그래스 진행 바의 진행률 [2014.11.5 ]
	int								m_EleIdx_In; // 안쪽 프로그래스바 의 엘리먼트 아이디  [2014.12.2 ]
	int								m_EleIdx_Out; // 안쪽 프로그래스바 의 엘리먼트 아이디  [2014.12.2 ]
public:// 생성자 소멸자

	CDXUTProgressBar( CDXUTDialog* pDialog = NULL );

public: // 일반함수

	virtual bool					HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool					HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );
	virtual void					OnHotkey()
	{
		if( m_pDialog->IsKeyboardInputEnabled() ) m_pDialog->RequestFocus( this );
		m_pDialog->SendEvent( EVENT_BUTTON_CLICKED, true, this );
	}

	virtual BOOL					ContainsPoint( POINT pt ){	return PtInRect( &m_rcBoundingBox, pt );}
	virtual bool					CanHaveFocus()	{	return ( m_bVisible && m_bEnabled ); }

	virtual void					Render( float fElapsedTime );

public: // get .. set

	virtual void					SetPercentage(float _val);// [2014.11.5 ]

	void							Set_EleIdx_In(int _val){m_EleIdx_In = _val;}// [2014.12.2 ]
	void							Set_EleIdx_Out(int _val){m_EleIdx_Out = _val;}// [2014.12.2 ]
};


//-----------------------------------------------------------------------------
// Slider control // [2014.12.4 ]
//-----------------------------------------------------------------------------
class CDXUTSlider : public GUI_Base
{
	friend class CDXUTDialog;
	
public:

	CDXUTSlider( CDXUTDialog* pDialog = NULL );

	virtual BOOL    ContainsPoint( POINT pt );
	virtual bool    CanHaveFocus()
	{
		return ( m_bVisible && m_bEnabled );
	}
	virtual bool    HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool    HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );

	virtual void    UpdateRects();

	virtual void    Render( float fElapsedTime );

	void            SetValue( int nValue )
	{
		SetValueInternal( nValue, false );
	}
	int             GetValue() const
	{
		return m_nValue;
	};

	void            GetRange( int& nMin, int& nMax ) const
	{
		nMin = m_nMin; nMax = m_nMax;
	}
	void            SetRange( int nMin, int nMax );

protected:
	
	void            SetValueInternal( int nValue, bool bFromInput );
	int             ValueFromPos( int x );

	int				m_nValue;

	int				m_Ele_Stick, m_Ele_Btn; 

	int				m_nMin;
	int				m_nMax;

	int				m_nDragX;      // Mouse position at start of drag
	int				m_nDragOffset; // Drag offset from the center of the button
	int				m_nButtonX;

	bool			m_bPressed;
	RECT			m_rcButton;
};

//////////////////////////////////////////////////////////////////////////
// 콜백 함수 선언  [2014.12.3 ]
void CALLBACK OnGUIEvent_Title( UINT nEvent, int nControlID, GUI_Base* pControl, void* pUserContext );

/****************************************************** end of this file *******************************************************************/