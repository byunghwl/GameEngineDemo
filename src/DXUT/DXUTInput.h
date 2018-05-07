
#pragma once
//-----------------------------------
// DirectX Input
//-----------------------------------

#define DIRECTINPUT_VERSION 0x0800
#define DIM_LBUTTON	0
#define DIM_RBUTTON	1
#define DIM_MBUTTON	2


#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib,"dxguid.lib")

class CInput
{
public:
	CInput();
	~CInput();
	HRESULT Initialize(HINSTANCE hInst, HWND hWnd);
	void	OnFrameMove(float _dTime);
	void	Release();
	//키보드 & 마우스 클릭
	//키보드 누르고있음
	bool	key(DWORD key)	{	return ( mCstk[key] & 0x80)	? true : false; }	
	//키보드 업
	bool	keyUp(DWORD key){	return ( mCstk[key] & 0x80)	? false: true;	}	
	
	//키보드 한번 누름 //[2014.7.22 ]
	bool	KEY(DWORD key)	{	return ((mCstk[key] & 0x80) && !(mPstk[key] & 0x80))? true : false; }	
	//마우스 클릭
	bool	MouseDown(int mou)	{	return ( mCstm.rgbButtons[mou] & 0x80) ? true : false; }	
	//마우스 업			
	bool	MouseUp(int mou)	{	return ( mCstm.rgbButtons[mou] & 0x80) ? false: true; }
	//마우스 한번 클릭					
	bool	MouseOneClick(int mou)	{	return ((mCstm.rgbButtons[mou] & 0x80) && !(mPstm.rgbButtons[mou] & 0x80)) ? true : false;	}

	void	SetMouseSmooth(float sm)			{ mMouseSmooth = sm;			}
	void	SetMousePos(float x, float y)		{ mMousePos.x=x; mMousePos.y=y; }
	//마우스 이동량
	D3DXVECTOR3 GetMouseDelta()	const		{ return mMouseDelta;			}
	D3DXVECTOR2 GetMousePos()	const		{ return mMousePos;			}

public:
	LPDIRECTINPUT8			mInput;
	LPDIRECTINPUTDEVICE8	mKey;
	BYTE					mCstk[256],mPstk[256];  // 키보드 상태값(현재,이전)

	LPDIRECTINPUTDEVICE8	mMou;
	DIMOUSESTATE			mCstm, mPstm;			// 마우스 상태값(현재,이전)

	POINT					m_CurMouPos; //	 현재 마우스 포지션 //[2014.11.24 ]
	POINT					m_LastMouPos; //	이전 프레임의 마우스 포지션 //[2014.11.24 ]

	D3DXVECTOR2				mMousePos;				// 마우스 포인터 위치
	D3DXVECTOR3				mMouseDelta;			// 마우스 이동량.
	D3DXVECTOR3				mKeyDelta;				// 키보드 이동량.
	float					mMouseSmooth;
};


//////////////////////////////////////////////////////////////////////////
// 전역변수 선언
extern CInput		SHG_Input;