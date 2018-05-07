
#include "dxut.h"



//////////////////////////////////////////////////////////////////////////
// �������� ����
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
// �Ҹ���
//-----------------------------------------------------------------------------
CInput::~CInput() 
{
	Release();
}
//-----------------------------------------------------------------------------
//�ʱ�ȭ �Լ�
//-----------------------------------------------------------------------------
HRESULT CInput::Initialize(HINSTANCE hInst, HWND hWnd)
{	

	m_CurMouPos.x = 0; m_CurMouPos.y = 0;
	m_LastMouPos.x = 0; m_LastMouPos.y = 0;

	//DirectInput ��ü ����.
	if(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&mInput, NULL)!=DI_OK)
		return E_FAIL;

	//Ű���� �Է� ����̽� ����.
	if(mInput->CreateDevice(GUID_SysKeyboard, &mKey, NULL)!=DI_OK)					return E_FAIL;
	//Ű���� ����̽��� ���� ������ �����Ѵ�.
	if(mKey->SetCooperativeLevel(hWnd, DISCL_BACKGROUND|DISCL_NONEXCLUSIVE)!=DI_OK)	return E_FAIL;
	//����̽��� ������ ���� ����.
	if(mKey->SetDataFormat(&c_dfDIKeyboard)!=DI_OK)									return E_FAIL;
	//����̽� ���� ����� ���.
	if(mKey->Acquire()!=DI_OK)														return E_FAIL;

	//���콺 �Է� ����̽� ����.
	if(mInput->CreateDevice(GUID_SysMouse, &mMou, NULL)!=DI_OK)	
	{
		MessageBox( NULL, TEXT( "Mouse not found. The sample will now exit." ),
					TEXT( "DirectInput Sample" ),
					MB_ICONERROR | MB_OK );//2014.07.21
		
		return E_FAIL;
	}
	//���콺 ����̽��� ���� ���� ����.
	if(mMou->SetCooperativeLevel(hWnd, DISCL_BACKGROUND|DISCL_NONEXCLUSIVE)!=DI_OK)
	{return E_FAIL;}
	//����̽��� ������ ���� ����.
	if(mMou->SetDataFormat(&c_dfDIMouse)!=DI_OK)
	{return E_FAIL;}
	//����̽� ���� ����� ���.
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
	//���콺 �̵��� ���.
	D3DXVECTOR3 CurMouseDelta =D3DXVECTOR3((float)mCstm.lX, (float)mCstm.lY, (float)mCstm.lZ/120.f);

	
	float fPercentOfNew =  1.0f / mMouseSmooth;

	if (MouseDown(0) || MouseDown(1) || MouseDown(2))	mMouseDelta = mMouseDelta*(1.0f - fPercentOfNew) + CurMouseDelta*fPercentOfNew;
	else							mMouseDelta = CurMouseDelta;

	if (D3DXVec3Length(&mMouseDelta)<=0.001f) mMouseDelta*=0.f;

	////////////////////////////////////////////////////////////////////////////////////////
	// ���콺 �̵��� ����ϱ�
	//LOG_WRITE_A("mouse x %f" , (float)mCstm.lX ); //[2014.11.24 ]

	if(G_CurSceneType == eSceneType_MainGame)
	{
		SetCursor( NULL );	// ���콺�� ��Ÿ���� �ʰ� �ʴ�.
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

		SHGlobal_GetCurScene_Camera->Get_m_AngleYaw() += mMouseDelta.x * _dTime * 0.1f; //ī�޶� ȸ������ �̸� ������Ʈ 
	}
	//////////////////////////////////////////////////////////////////////////
}