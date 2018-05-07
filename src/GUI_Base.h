/***********************************************************************
                   // [2014.10.31 ]  
				      "GUI_Base.h

************************************************************************/

#pragma  once

//--------------------------------------------------------------------------------------
// Defines and macros 
//--------------------------------------------------------------------------------------
#define EVENT_BUTTON_CLICKED                0x0101
#define EVENT_COMBOBOX_SELECTION_CHANGED    0x0201
#define EVENT_RADIOBUTTON_CHANGED           0x0301
#define EVENT_CHECKBOX_CHANGED              0x0401
#define EVENT_SLIDER_VALUE_CHANGED          0x0501
#define EVENT_EDITBOX_STRING                0x0601
// EVENT_EDITBOX_CHANGE is sent when the listbox content changes
// due to user input.
#define EVENT_EDITBOX_CHANGE                0x0602
#define EVENT_LISTBOX_ITEM_DBLCLK           0x0701
// EVENT_LISTBOX_SELECTION is fired off when the selection changes in
// a single selection list box.
#define EVENT_LISTBOX_SELECTION             0x0702
#define EVENT_LISTBOX_SELECTION_END         0x0703

// Delay and repeat period when clicking on the scroll bar arrows
#define SCROLLBAR_ARROWCLICK_DELAY  0.33
#define SCROLLBAR_ARROWCLICK_REPEAT 0.05

#define DXUT_NEAR_BUTTON_DEPTH 0.6f
#define DXUT_FAR_BUTTON_DEPTH 0.8f

#define DXUT_MAX_GUI_SPRITES 500


//////////////////////////////////////////////////////////////////////////
// 전방선언
class INotifiable;
class GUI_Base;
class CDXUTStatic;
class CDXUTSlider;
class CDXUTImage;
class CDXUTButton;
class CDXUTDialog;
class CDXUTElement;
class CDialogRscMgr;
class CDXUTProgressBar;
// 함수포인터
typedef VOID ( CALLBACK*PCALLBACKDXUTGUIEVENT )( UINT nEvent, int nControlID, GUI_Base* pControl,void* pUserContext );


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Enums for pre-defined control types
// 
// 컨트롤 타입
enum DXUT_CONTROL_TYPE
{
	DXUT_CONTROL_BUTTON,
	DXUT_CONTROL_STATIC,
	DXUT_CONTROL_IMAGE,
	DXUT_CONTROL_CHECKBOX,
	DXUT_CONTROL_RADIOBUTTON,
	DXUT_CONTROL_COMBOBOX,
	DXUT_CONTROL_SLIDER,
	DXUT_CONTROL_EDITBOX,
	DXUT_CONTROL_IMEEDITBOX,
	DXUT_CONTROL_LISTBOX,
	DXUT_CONTROL_SCROLLBAR,
	DXUT_CONTROL_PROGRESSBAR,
	DXUT_CONTROL_TPYE_MAX
};

// 컨트롤들의 상태  
enum DXUT_CONTROL_STATE
{
	DXUT_STATE_NORMAL = 0,  // 일반
	DXUT_STATE_DISABLED,    // 사용불가
	DXUT_STATE_HIDDEN,      // 안보임
	DXUT_STATE_FOCUS,		 // 포커스받은상태
	DXUT_STATE_MOUSEOVER,   // 마우스 위에있는 상태
	DXUT_STATE_PRESSED,     // 눌림
	DXUT_CONTROL_STATE_MAX
};


enum SH_UI_ElementIdxBTN
{
	eEle_StartButton1 = 0,
	eEle_StartButton2,
	eEle_CreditButton1,
	eEle_CreditButton2,
	eEle_ExitButton1,
	eEle_ExitButton2

};

enum SH_UI_ElementIdxImage
{
	eEle_Skill1_Image1 = 0, //스킬1 테두리
	eEle_Skill1_Image2,  // 스킬1 이미지
	eEle_Skill2_Image1, //스킬2 테두리
	eEle_Skill2_Image2, // 스킬2 이미지
	eEle_Skill3_Image1, //스킬2 테두리
	eEle_Skill3_Image2, // 스킬2 이미지
	eEle_Fail_Image, // 실패시 뜨는 이미지  [2014.12.4 ]
	eEle_TeddyVSNight,
	eEle_Hat,
	eEle_Success_Image, // 성공시 뜨는 이미지  [2014.12.4 ]

};

enum SH_UI_ElementIdxSlider
{
	eEle_Timer_Stick,
	eEle_Timer_Button, //[2014.12.5 ]
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


/***********************************************************************
             class    INotifiable       // [2014.10.31 ]                                          
************************************************************************/
class INotifiable
{

public:
	virtual  DWORD			Notify( SH_MSG* _pMessage ) PURE;

public:
	INotifiable(){};
	virtual ~INotifiable(){};


};



/***********************************************************************
                   // [2014.10.31 ]  
				     class GUI_Base
	static , button, CheckBox.  등등 의 부모클래스가 되는 클래스 
************************************************************************/
class GUI_Base
{
protected:

	

	RECT							m_rcBoundingBox;  // Rectangle defining the active region of the control [2014.10.31 ]
	int								m_ID;             // 이 UI의 고유 아이디
	DXUT_CONTROL_TYPE				m_Type;			  // 이 UI의 Control type, set once in constructor  
	UINT							m_nHotkey;        // Virtual key code for this control's hotkey
	void*							m_pUserData;      // Data associated with this control that is set by user.
	bool							m_bEnabled;       // Enabled/disabled flag
	DXUT_CONTROL_STATE				m_State;		 // DXUT_CONTROL_STATE [2014.12.4 ]

public:

	bool							m_bVisible;		  // Shown/hidden flag
	bool							m_bMouseOver;     // Mouse pointer is above control
	bool							m_bHasFocus;      // Control has input focus
	bool							m_bIsDefault;     // Is the default control
	// Size, scale, and positioning members
	int								m_x, m_y;
	int								m_width, m_height;
	// These members are set by the container
	CDXUTDialog*					m_pDialog;        // Parent container
	UINT							m_Index;          // Index within the control list
	CGrowableArray<CDXUTElement*>	m_Elements;       // All display elements

public:// 생성자
	
	GUI_Base(CDXUTDialog* pDialog = NULL );
	virtual ~GUI_Base();

protected : // 내부함수

	virtual void					UpdateRects();

public: //일반함수

	//virtual void					Control(MSG& _Message);
		


	virtual HRESULT					OnInit(){return S_OK;}

	virtual void					Refresh();
	virtual void					Render( float fElapsedTime ){};

	// Windows message handler
	virtual bool					MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam ){	return false;}

	virtual bool					HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam ){	return false;}
	virtual bool					HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam ){	return false;}

	virtual bool					CanHaveFocus()		{	return false;}
	virtual void					OnFocusIn()			{m_bHasFocus = true;	}
	virtual void					OnFocusOut()		{m_bHasFocus = false;}
	virtual void					OnMouseEnter()		{	m_bMouseOver = true;	}
	virtual void					OnMouseLeave()		{	m_bMouseOver = false;	}
	virtual void					OnHotkey()			{}

	virtual BOOL					ContainsPoint( POINT pt ){	return PtInRect( &m_rcBoundingBox, pt );	}


public:  // get.. set

	virtual void					SetEnabled( bool bEnabled ){m_bEnabled = bEnabled;	}
	virtual void					SetVisible( bool bVisible ){	m_bVisible = bVisible;	}
	virtual void					SetPercentage(float _val){}// for ProgressBar [2014.11.5 ]  프로그레스바 클래스에서 재정의 되어있음

	HRESULT							SetID( int ID );
	void							SetLocation( int x, int y )	{m_x = x; m_y = y; UpdateRects();	}
	void							SetSize( int width, int height ){m_width = width; m_height = height; UpdateRects();	}
	void							SetHotkey( UINT nHotkey ){	m_nHotkey = nHotkey;}
	void							SetUserData( void* pUserData ){	m_pUserData = pUserData;	}
	virtual void					SetTextColor( D3DCOLOR Color );
	HRESULT							SetElement( UINT iElement, CDXUTElement* pElement );

	virtual bool					GetVisible()		{   return m_bVisible;}
	virtual bool					GetEnabled()		{	return m_bEnabled;	}
	UINT							GetType() const		{	return m_Type;	}
	int								GetID() const		{	return m_ID;	}
	UINT							GetHotkey()			{	return m_nHotkey;	}	
	void*							GetUserData() const	{	return m_pUserData;	}	
	CDXUTElement*					GetElement( UINT iElement ){return m_Elements.GetAt( iElement );	}// [2014.11.1 ]
	

};








/************************************************************************/
/*				    버튼 등 UI에 보여질 color 구조체 선언
                       [2014.11.1 ]
*/
/************************************************************************/
struct DXUTBlendColor
{
	void        Init( D3DCOLOR defaultColor, D3DCOLOR disabledColor = D3DCOLOR_ARGB( 200, 128, 128, 128 ),
					  D3DCOLOR hiddenColor = 0 );

	void        Blend( UINT iState, float fElapsedTime, float fRate = 0.7f );

	D3DCOLOR    States[ DXUT_CONTROL_STATE_MAX ]; // 상태에 따라 섞이게 될 색이 다르다.  [2014.11.1 ]

	D3DXCOLOR Current;
};



/************************************************************************
    Contains all the display tweakables for a sub-control 
				[2014.11.1 ]

/************************************************************************/

class CDXUTElement
{
public:
	void    SetTexture( UINT iTexture, RECT* prcTexture, D3DCOLOR defaultTextureColor = D3DCOLOR_ARGB( 255, 255, 255,255 ) );
	
	void    SetFont( UINT iFont, D3DCOLOR defaultFontColor = D3DCOLOR_ARGB( 255, 255, 255,255 ),  // [2014.11.1 ]
				     DWORD dwTextFormat = DT_CENTER | DT_VCENTER );

	void    Refresh();

	UINT iTexture;          // Index of the texture for this Element 
	UINT iFont;             // Index of the font for this Element
	DWORD dwTextFormat;     // The format argument to DrawText 

	RECT rcTexture;         // Bounding rect of this element on the composite texture

	DXUTBlendColor TextureColor;
	DXUTBlendColor FontColor;
};


/************************************************************************/
/*						Structs for shared resources
						[2014.11.1 ]
*/
/************************************************************************/
struct DXUTTextureNode
{
	bool bFileSource;  // True if this texture is loaded from a file. False if from resource.
	HMODULE hResourceModule;
	int nResourceID;   // Resource ID. If 0, string-based ID is used and stored in strFilename.
	WCHAR strFilename[MAX_PATH];
	DWORD dwWidth;
	DWORD dwHeight;
	IDirect3DTexture9* pTexture9;
	ID3D10Texture2D* pTexture10;
	ID3D10ShaderResourceView* pTexResView;
};

struct DXUTFontNode
{
	WCHAR strFace[MAX_PATH];
	LONG nHeight;
	LONG nWeight;
	ID3DXFont* pFont9;
	ID3DX10Font* pFont10;
};


// Contains all the display information for a given control type
struct DXUTElementHolder
{
	UINT nControlType;
	UINT iElement;

	CDXUTElement Element;
};


/***********************************************************************
                     class    SH_CDialog             
					 [2014.11.1 ]     
	  All controls must be assigned to a dialog, which handles
	         input and rendering for the controls.
************************************************************************/
class CDXUTDialog
{
	friend class CDialogRscMgr;
	friend class GUI_Base;

public:
	CDXUTDialog();
	~CDXUTDialog();

	// Need to call this now
	void						Init( CDialogRscMgr* pManager, bool bRegisterDialog = true );
	void						Init( CDialogRscMgr* pManager, bool bRegisterDialog,
									  LPCWSTR pszControlTextureFilename , LPCSTR pszDialogName ); // [2014.11.5 ]
	void						Init( CDialogRscMgr* pManager, bool bRegisterDialog,
									  LPCWSTR szControlTextureResourceName, HMODULE hControlTextureResourceModule );

	// Windows message handler
	bool						MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// Control creation
	HRESULT						AddStatic( int ID, LPCWSTR strText, int x, int y, int width, int height, bool bIsDefault=false,
								           CDXUTStatic** ppCreated=NULL );
	HRESULT						AddImage( int ID, LPCWSTR strText, int x, int y, int width, int height,int _Ele_Idx_in, int _Ele_Idx_Out, bool bIsDefault=false,
										   CDXUTStatic** ppCreated=NULL );
	HRESULT						AddButton( int ID, LPCWSTR strText, int x, int y, int width, int height, int _Ele_Idx_in, int _Ele_Idx_Out, UINT nHotkey=0,
										   bool bIsDefault=false, CDXUTButton** ppCreated=NULL );
// 	HRESULT             AddCheckBox( int ID, LPCWSTR strText, int x, int y, int width, int height, bool bChecked=false,
// 		UINT nHotkey=0, bool bIsDefault=false, CDXUTCheckBox** ppCreated=NULL );
// 	HRESULT             AddRadioButton( int ID, UINT nButtonGroup, LPCWSTR strText, int x, int y, int width,
// 		int height, bool bChecked=false, UINT nHotkey=0, bool bIsDefault=false,
// 		CDXUTRadioButton** ppCreated=NULL );
// 	HRESULT             AddComboBox( int ID, int x, int y, int width, int height, UINT nHotKey=0, bool bIsDefault=
// 		false, CDXUTComboBox** ppCreated=NULL );
 	HRESULT						AddSlider( int ID, int x, int y, int width, int height, int Ele_Stick, int Ele_Btn,
										   int min=0, int max=100, int value=50, bool bIsDefault=false, CDXUTSlider** ppCreated=NULL );
	//      AddIMEEditBox has been renamed into DXUTguiIME.cpp as CDXUTIMEEditBox::CreateIMEEditBox
// 	HRESULT             AddEditBox( int ID, LPCWSTR strText, int x, int y, int width, int height, bool bIsDefault=
// 		false, CDXUTEditBox** ppCreated=NULL );
// 	HRESULT             AddListBox( int ID, int x, int y, int width, int height, DWORD dwStyle=0,
// 		CDXUTListBox** ppCreated=NULL );

	HRESULT						AddProgressBar(int ID, int x, int y, int width, int height, int Idx_In, int Idx_Out ,bool bIsDefault=false,
											   CDXUTProgressBar** ppCreated=NULL  );// [2014.11.5 ]
	HRESULT						AddControl( GUI_Base* pControl ); // [2014.11.1 ]
	HRESULT						InitControl( GUI_Base* pControl );

	// Control retrieval
	CDXUTStatic*				GetStatic( int ID )	{	return ( CDXUTStatic* )GetControl( ID, DXUT_CONTROL_STATIC );	}
	CDXUTImage*					GetImage( int ID )	{	return ( CDXUTImage* )GetControl( ID, DXUT_CONTROL_IMAGE );	}//[2014.12.4 ]
	CDXUTButton*				GetButton( int ID )	{	return ( CDXUTButton* )GetControl( ID, DXUT_CONTROL_BUTTON );	}
// 	CDXUTCheckBox* GetCheckBox( int ID )
// 	{
// 		return ( CDXUTCheckBox* )GetControl( ID, DXUT_CONTROL_CHECKBOX );
// 	}
// 	CDXUTRadioButton* GetRadioButton( int ID )
// 	{
// 		return ( CDXUTRadioButton* )GetControl( ID, DXUT_CONTROL_RADIOBUTTON );
// 	}
// 	CDXUTComboBox* GetComboBox( int ID )
// 	{
// 		return ( CDXUTComboBox* )GetControl( ID, DXUT_CONTROL_COMBOBOX );
// 	}
	CDXUTSlider* GetSlider( int ID )
	{
		return ( CDXUTSlider* )GetControl( ID, DXUT_CONTROL_SLIDER );
	}
// 	CDXUTEditBox* GetEditBox( int ID )
// 	{
// 		return ( CDXUTEditBox* )GetControl( ID, DXUT_CONTROL_EDITBOX );
// 	}
// 	CDXUTListBox* GetListBox( int ID )
// 	{
// 		return ( CDXUTListBox* )GetControl( ID, DXUT_CONTROL_LISTBOX );
// 	}
// 
	GUI_Base*					GetControl( int ID );
	GUI_Base*					GetControl( int ID, UINT nControlType );
	GUI_Base*					GetControlAtPoint( POINT pt );

	bool						GetControlEnabled( int ID );
	void						SetControlEnabled( int ID, bool bEnabled );
// 
// 	void						ClearRadioButtonGroup( UINT nGroup );
// 	void						ClearComboBox( int ID );

	// Access the default display Elements used when adding new controls
	HRESULT						SetDefaultElement( UINT nControlType, UINT iElement, CDXUTElement* pElement );
	CDXUTElement*				GetDefaultElement( UINT nControlType, UINT iElement );

	// Methods called by controls
	void						SendEvent( UINT nEvent, bool bTriggeredByUser, GUI_Base* pControl );
	void						RequestFocus( GUI_Base* pControl );

	// Render helpers
	HRESULT						DrawRect( RECT* pRect, D3DCOLOR color );
	HRESULT						DrawRect9( RECT* pRect, D3DCOLOR color );
	HRESULT						DrawRect10( RECT* pRect, D3DCOLOR color );
	HRESULT						DrawPolyLine( POINT* apPoints, UINT nNumPoints, D3DCOLOR color );
	HRESULT						DrawSprite( CDXUTElement* pElement, RECT* prcDest, float fDepth );
	HRESULT						DrawSprite9( CDXUTElement* pElement, RECT* prcDest );
	HRESULT						DrawSprite10( CDXUTElement* pElement, RECT* prcDest, float fDepth );
	HRESULT						CalcTextRect( LPCWSTR strText, CDXUTElement* pElement, RECT* prcDest, int nCount = -1 );
	HRESULT						DrawText( LPCWSTR strText, CDXUTElement* pElement, RECT* prcDest, bool bShadow = false,
										  int nCount = -1 );
	HRESULT						DrawText9( LPCWSTR strText, CDXUTElement* pElement, RECT* prcDest, bool bShadow = false,
										   int nCount = -1 );
	HRESULT						DrawText10( LPCWSTR strText, CDXUTElement* pElement, RECT* prcDest, bool bShadow = false,
											int nCount = -1 );
public:  // get,, set
	// Attributes
	CString						GetName() const{ return m_DlgName;}
	bool						GetVisible(){	return m_bVisible;	}
	void						SetVisible( bool bVisible ){	m_bVisible = bVisible;}
	bool						GetMinimized()	{	return m_bMinimized;	}
	void						SetMinimized( bool bMinimized ){	m_bMinimized = bMinimized;	}
	void						SetBackgroundColors( D3DCOLOR colorAllCorners )	{	SetBackgroundColors( colorAllCorners, colorAllCorners, colorAllCorners, colorAllCorners );	}
	void						SetBackgroundColors( D3DCOLOR colorTopLeft, D3DCOLOR colorTopRight, D3DCOLOR colorBottomLeft,
													 D3DCOLOR colorBottomRight );
	void						EnableCaption( bool bEnable ){	m_bCaption = bEnable;}
	int							GetCaptionHeight() const	{	return m_nCaptionHeight;	}
	void						SetCaptionHeight( int nHeight ){m_nCaptionHeight = nHeight;	}
	void						SetCaptionText( const WCHAR* pwszText )	{	wcscpy_s( m_wszCaption, sizeof( m_wszCaption ) / sizeof( m_wszCaption[0] ), pwszText );	}
	void						GetLocation( POINT& Pt ) const{	Pt.x = m_x; Pt.y = m_y;	}
	void						SetLocation( int x, int y ){	m_x = x; m_y = y;	}
	void						SetSize( int width, int height ){	m_width = width; m_height = height;	}
	int							GetWidth(){	return m_width;	}
	int							GetHeight(){	return m_height;}
	static void WINAPI			SetRefreshTime( float fTime ){	s_fTimeRefresh = fTime;	}

	static GUI_Base* WINAPI		GetNextControl( GUI_Base* pControl );
	static GUI_Base* WINAPI		GetPrevControl( GUI_Base* pControl );

	void						RemoveControl( int ID );
	void						RemoveAllControls();

	// Sets the callback used to notify the app of control events
	void						SetCallback( PCALLBACKDXUTGUIEVENT pCallback, void* pUserContext = NULL );
	void						EnableNonUserEvents( bool bEnable ){m_bNonUserEvents = bEnable;	}
	void						EnableKeyboardInput( bool bEnable ){	m_bKeyboardInput = bEnable;	}
	void						EnableMouseInput( bool bEnable ){	m_bMouseInput = bEnable;	}
	bool						IsKeyboardInputEnabled() const	{	return m_bKeyboardInput;	}

	// Device state notification
	void						Refresh();
	HRESULT						OnRender( float fElapsedTime );

	// Shared resource access. Indexed fonts and textures are shared among
	// all the controls.
	HRESULT						SetFont( UINT index, LPCWSTR strFaceName, LONG height, LONG weight );
	DXUTFontNode*				GetFont( UINT index );

	HRESULT						SetTexture( UINT index, LPCWSTR strFilename );
	HRESULT						SetTexture( UINT index, LPCWSTR strResourceName, HMODULE hResourceModule );
	DXUTTextureNode*			GetTexture( UINT index );
	// [2014.11.3 ]
	void						SetMediator( INotifiable* _pMediator){m_pMediator = _pMediator;}


	CDialogRscMgr*				GetManager(){	return m_pManager;	}

	static void WINAPI			ClearFocus();
	void						FocusDefaultControl();

	bool						m_bNonUserEvents;
	bool						m_bKeyboardInput;
	bool						m_bMouseInput;
	INotifiable*				m_pMediator;      // 미디에이터 패턴 // [2014.10.31 ]


private:
	int m_nDefaultControlID;

	HRESULT						OnRender9( float fElapsedTime );
	HRESULT						OnRender10( float fElapsedTime );

	static double				s_fTimeRefresh;
	double						m_fTimeLastRefresh;

	// Initialize default Elements
	void						InitDefaultElements();

	// Windows message handlers
	void						OnMouseMove( POINT pt );
	void						OnMouseUp( POINT pt );

	void						SetNextDialog( CDXUTDialog* pNextDialog );

	// Control events
	bool						OnCycleFocus( bool bForward );

	static GUI_Base*			s_pControlFocus;        // The control which has focus
	static GUI_Base*			s_pControlPressed;      // The control currently pressed

	GUI_Base*					m_pControlMouseOver;           // The control which is hovered over

	bool						m_bVisible;
	bool						m_bCaption;
	bool						m_bMinimized;
	bool						m_bDrag;
	WCHAR						m_wszCaption[256];

	int							m_x;
	int							m_y;
	int							m_width;
	int							m_height;
	int							m_nCaptionHeight;

	D3DCOLOR					m_colorTopLeft;
	D3DCOLOR					m_colorTopRight;
	D3DCOLOR					m_colorBottomLeft;
	D3DCOLOR					m_colorBottomRight;

	CDialogRscMgr* m_pManager;
	PCALLBACKDXUTGUIEVENT		m_pCallbackEvent;
	void*						m_pCallbackEventUserContext;

	CGrowableArray <int>		m_Textures;   // Index into m_TextureCache;
	CGrowableArray <int>		m_Fonts;      // Index into m_FontCache;

	CGrowableArray <GUI_Base*>  m_Controls;
	CGrowableArray <DXUTElementHolder*> m_DefaultElements;

	CDXUTElement				m_CapElement;  // Element for the caption

	CDXUTDialog*				m_pNextDialog;
	CDXUTDialog*				m_pPrevDialog;
	CString						m_DlgName; // 다이얼로그 이름 [2014.11.5 ]
	
};


/************************************************************************/
/*					class CDialogRscMgr
			Manages shared resources of dialogs
*/
/************************************************************************/

//-----------------------------------------------------------------------------
// Manages shared resources of dialogs
//-----------------------------------------------------------------------------
class CDialogRscMgr
{
public:
	CDialogRscMgr();
	~CDialogRscMgr();
	//싱글톤 객체 리턴
	static CDialogRscMgr*			GetInstance();

public:
	bool							MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// D3D9 specific
	HRESULT							OnD3D9CreateDevice( LPDIRECT3DDEVICE9 pd3dDevice );
	HRESULT							OnD3D9ResetDevice();
	void							OnD3D9LostDevice();
	void							OnD3D9DestroyDevice();
	IDirect3DDevice9*				GetD3D9Device()	{	return m_pd3d9Device;	}

	// D3D10 specific
	//HRESULT OnD3D10CreateDevice( ID3D10Device* pd3dDevice );
	HRESULT							OnD3D10ResizedSwapChain( ID3D10Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc );
	void							OnD3D10ReleasingSwapChain();
	void							OnD3D10DestroyDevice();
	ID3D10Device*					GetD3D10Device(){	return m_pd3d10Device;}

	DXUTFontNode*					GetFontNode( int iIndex )	{	return m_FontCache.GetAt( iIndex );	};
	DXUTTextureNode*				GetTextureNode( int iIndex ){	return m_TextureCache.GetAt( iIndex );	};
	GUI_Base*						GetControl(int _UI_ID); // UI고유아이디로 해당컨트롤을 찾아준다. 

	int								AddFont( LPCWSTR strFaceName, LONG height, LONG weight );
	int								AddTexture( LPCWSTR strFilename );
	int								AddTexture( LPCWSTR strResourceName, HMODULE hResourceModule );

	bool							RegisterDialog( CDXUTDialog* pDialog );
	void							UnregisterDialog( CDXUTDialog* pDialog );
	void							EnableKeyboardInputForAllDialogs();

	
public:// Shared between all dialogs
	// D3D9
	IDirect3DStateBlock9*			m_pStateBlock;
	ID3DXSprite*					m_pSprite;          // Sprite used for drawing

	// D3D10
	ID3D10Effect*					m_pEffect10;        // Effect used to render UI with D3D10
	ID3D10EffectTechnique*			m_pTechRenderUI10;  // Technique: RenderUI
	ID3D10EffectTechnique*			m_pTechRenderUIUntex10;  // Technique: RenderUI without texture
	ID3D10EffectShaderResourceVariable* m_pFxTexture10;
	ID3D10InputLayout*				m_pInputLayout10;
	ID3D10Buffer*					m_pVBScreenQuad10;
	ID3D10StateBlock*				m_pStateBlock10;
	ID3DX10Sprite*					m_pSprite10;
	UINT							m_nBackBufferWidth;
	UINT							m_nBackBufferHeight;

	CGrowableArray <CDXUTDialog*>	m_Dialogs;            // Dialogs registered

protected:
	// D3D9 specific
	IDirect3DDevice9*				m_pd3d9Device;
	HRESULT							CreateFont9( UINT index );
	HRESULT							CreateTexture9( UINT index );

	// D3D10 specific
	ID3D10Device*					m_pd3d10Device;
	HRESULT							CreateFont10( UINT index );
	HRESULT							CreateTexture10( UINT index );

	CGrowableArray <DXUTTextureNode*> m_TextureCache;   // Shared textures
	CGrowableArray <DXUTFontNode*> m_FontCache;         // Shared fonts

	static CDialogRscMgr*		    pInstance; // for single ton
};
