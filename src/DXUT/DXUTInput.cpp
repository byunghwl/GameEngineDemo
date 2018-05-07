
#include "dxut.h"



//////////////////////////////////////////////////////////////////////////
// 전역변수 선언
CInput		SHG_Input;
//////////////////////////////////////////////////////////////////////////
//------------------------------------
CInput::CInput() : mInput(0), mKey(0), mMou(0), mMouseSmooth(5)
{
	memset(mCstk, 0, sizeof(BYTE)*256);
	memset(mPstk, 0, sizeof(BYTE)*256);
	memset(&mCstm, 0, sizeof(DIMOUSESTATE));
	memset(&mPstm, 0, sizeof(DIMOUSESTATE));
}

//-----------------------------------------------------------------------------
// 소멸자
//-----------------------------------------------------------------------------
CInput::~CInput() 
{
	Release();
}
//-----------------------------------------------------------------------------
//초기화 함수
//-----------------------------------------------------------------------------
HRESULT CInput::Initialize(HINSTANCE hInst, HWND hWnd)
{	

	m_CurMouPos.x = 0; m_CurMouPos.y = 0;
	m_LastMouPos.x = 0; m_LastMouPos.y = 0;

	//DirectInput 객체 생성.
	if(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&mInput, NULL)!=DI_OK)
		return E_FAIL;

	//키보드 입력 디바이스 생성.
	if(mInput->CreateDevice(GUID_SysKeyboard, &mKey, NULL)!=DI_OK)					return E_FAIL;
	//키보드 디바이스의 협력 레벨을 설정한다.
	if(mKey->SetCooperativeLevel(hWnd, DISCL_BACKGROUND|DISCL_NONEXCLUSIVE)!=DI_OK)	return E_FAIL;
	//디바이스의 데이터 포맷 설정.
	if(mKey->SetDataFormat(&c_dfDIKeyboard)!=DI_OK)									return E_FAIL;
	//디바이스 접근 제어권 얻기.
	if(mKey->Acquire()!=DI_OK)														return E_FAIL;

	//마우스 입력 디바이스 생성.
	if(mInput->CreateDevice(GUID_SysMouse, &mMou, NULL)!=DI_OK)	
	{
		MessageBox( NULL, TEXT( "Mouse not found. The sample will now exit." ),
					TEXT( "DirectInput Sample" ),
					MB_ICONERROR | MB_OK );//2014.07.21
		
		return E_FAIL;
	}
	//마우스 디바이스의 협력 레벨 설정.
	if(mMou->SetCooperativeLevel(hWnd, DISCL_BACKGROUND|DISCL_NONEXCLUSIVE)!=DI_OK)
	{return E_FAIL;}
	//디바이스의 데이터 포맷 설정.
	if(mMou->SetDataFormat(&c_dfDIMouse)!=DI_OK)
	{return E_FAIL;}
	//디바이스 접근 제어권 얻기.
	if(mMou->Acquire()!=DI_OK)
	{return E_FAIL;}

	return S_OK;
}
//-----------------------------------
void CInput::Release()
{
	if(mKey)	{	mKey->Unacquire();	mKey->Release();	}	mKey=NULL;
	if(mMou)	{	mMou->Unacquire();	mMou->Release();	}	mMou=NULL;
	if(mInput)		mInput->Release();	mInput=NULL;
}
//-----------------------------------
void CInput::OnFrameMove(float _dTime)
{
	memcpy(mPstk, mCstk, sizeof(BYTE)*256);
	if(mKey && mKey->GetDeviceState(256,(LPVOID)mCstk))
	{
		memset(mCstk, 0, sizeof(BYTE)*256);
		mKey->Acquire();
	}
	memcpy(&mPstm, &mCstm, sizeof(DIMOUSESTATE));
	if(mMou && mMou->GetDeviceState(sizeof(DIMOUSESTATE),(LPVOID)&mCstm))
	{
		memset(&mCstm, 0, sizeof(DIMOUSESTATE));
		mMou->Acquire();
	}
	//마우스 이동량 얻기.
	D3DXVECTOR3 CurMouseDelta =D3DXVECTOR3((float)mCstm.lX, (float)mCstm.lY, (float)mCstm.lZ/120.f);

	
	float fPercentOfNew =  1.0f / mMouseSmooth;

	if (MouseDown(0) || MouseDown(1) || MouseDown(2))	mMouseDelta = mMouseDelta*(1.0f - fPercentOfNew) + CurMouseDelta*fPercentOfNew;
	else							mMouseDelta = CurMouseDelta;

	if (D3DXVec3Length(&mMouseDelta)<=0.001f) mMouseDelta*=0.f;

	////////////////////////////////////////////////////////////////////////////////////////
	// 마우스 이동량 계산하기
	//LOG_WRITE_A("mouse x %f" , (float)mCstm.lX ); //[2014.11.24 ]

	if(G_CurSceneType == eSceneType_MainGame)
	{
		SetCursor( NULL );	// 마우스를 나타나지 않게 않다.
		GetCursorPos(&m_CurMouPos);
		mMouseDelta.x = m_CurMouPos.x - m_LastMouPos.x; //[2014.11.24 ]
		mMouseDelta.y =m_LastMouPos.y - m_CurMouPos.y  ;
		//LOG_WRITE_A("cur mouse x %f" , (float)m_CurMouPos.x );
		//LOG_WRITE_A("last mouse x %f" , (float)m_LastMouPos.x );
		m_LastMouPos = m_CurMouPos;
		RECT	rc;
		POINT	pt;
		POINT   CurMPos;	
		GetClientRect( SHGlobal_GetHwnd, &rc );
		pt.x = (rc.right - rc.left) / 2;
		pt.y = (rc.bottom - rc.top) / 2;
		ClientToScreen( SHGlobal_GetHwnd, &pt );

		GetCursorPos(&CurMPos);//[2014.11.24 ]

		SetCursorPos( pt.x, pt.y );

		mMouseDelta.x= (float)(CurMPos.x - pt.x); //[2014.11.24 ]

		SHGlobal_GetCurScene_Camera->Get_m_AngleYaw() += mMouseDelta.x * _dTime * 0.1f; //카메라 회전각도 미리 업데이트 
	}
	//////////////////////////////////////////////////////////////////////////
}