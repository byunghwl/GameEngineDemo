
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
	//Ű���� & ���콺 Ŭ��
	//Ű���� ����������
	bool	key(DWORD key)	{	return ( mCstk[key] & 0x80)	? true : false; }	
	//Ű���� ��
	bool	keyUp(DWORD key){	return ( mCstk[key] & 0x80)	? false: true;	}	
	
	//Ű���� �ѹ� ���� //[2014.7.22 ]
	bool	KEY(DWORD key)	{	return ((mCstk[key] & 0x80) && !(mPstk[key] & 0x80))? true : false; }	
	//���콺 Ŭ��
	bool	MouseDown(int mou)	{	return ( mCstm.rgbButtons[mou] & 0x80) ? true : false; }	
	//���콺 ��			
	bool	MouseUp(int mou)	{	return ( mCstm.rgbButtons[mou] & 0x80) ? false: true; }
	//���콺 �ѹ� Ŭ��					
	bool	MouseOneClick(int mou)	{	return ((mCstm.rgbButtons[mou] & 0x80) && !(mPstm.rgbButtons[mou] & 0x80)) ? true : false;	}

	void	SetMouseSmooth(float sm)			{ mMouseSmooth = sm;			}
	void	SetMousePos(float x, float y)		{ mMousePos.x=x; mMousePos.y=y; }
	//���콺 �̵���
	D3DXVECTOR3 GetMouseDelta()	const		{ return mMouseDelta;			}
	D3DXVECTOR2 GetMousePos()	const		{ return mMousePos;			}

public:
	LPDIRECTINPUT8			mInput;
	LPDIRECTINPUTDEVICE8	mKey;
	BYTE					mCstk[256],mPstk[256];  // Ű���� ���°�(����,����)

	LPDIRECTINPUTDEVICE8	mMou;
	DIMOUSESTATE			mCstm, mPstm;			// ���콺 ���°�(����,����)

	POINT					m_CurMouPos; //	 ���� ���콺 ������ //[2014.11.24 ]
	POINT					m_LastMouPos; //	���� �������� ���콺 ������ //[2014.11.24 ]

	D3DXVECTOR2				mMousePos;				// ���콺 ������ ��ġ
	D3DXVECTOR3				mMouseDelta;			// ���콺 �̵���.
	D3DXVECTOR3				mKeyDelta;				// Ű���� �̵���.
	float					mMouseSmooth;
};


//////////////////////////////////////////////////////////////////////////
// �������� ����
extern CInput		SHG_Input;