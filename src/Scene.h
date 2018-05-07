/************************************************************************/
/*								Scene.h
		CScene Class 를 상속받은 각종 자식Scene 선언한 헤더파일
						[2014.8.30 ]
*/
/************************************************************************/


#pragma  once

/************************************************************************/
/*                       Scene for Play
*/
/************************************************************************/
class CScene_MainGame : public CScene
						
{
protected:

	

public:

	CScene_MainGame();

	virtual ~CScene_MainGame();

public:// 재정의 함수
	
	// override
	virtual int								InitObjects();
	// override
	virtual void							OnRenderStart();
	// override
	virtual void							OnRenderEnd();
	// override [2014.11.16 ]
	virtual void							OnUpdateStart(float _dTime);
	// override  [2014.12.2 ]
	virtual void							OnUpdateEnd(float _dTime);
	// override
	virtual DWORD							Notify(SH_MSG* _pMessage );

	// 텍스트 호출하는 함수 // [2014.12.1 ]
	virtual void							RenderText( float DTime );

	// UI그려주는 함수 재정의  // [2014.12.1 ]
	virtual void							RenderUI( float DTime );

public:

	
};



/************************************************************************/
/*                        씬 타이틀 화면  [2014.12.2 ]
*/
/************************************************************************/
class CScene_Title : public CScene

{
protected:



public:

	CScene_Title();

	virtual ~CScene_Title();

public:// 재정의 함수

	// override
	virtual int								InitObjects();
	// override
	virtual void							OnRenderStart();
	// override
	virtual void							OnRenderEnd();
	// override [2014.11.16 ]
	virtual void							OnUpdateStart(float _dTime);
	// override  [2014.12.2 ]
	virtual void							OnUpdateEnd(float _dTime);
	// override
	virtual DWORD							Notify(SH_MSG* _pMessage );

	// 텍스트 호출하는 함수 // [2014.12.1 ]
	virtual void							RenderText( float DTime );

	// UI그려주는 함수 재정의  // [2014.12.1 ]
	virtual void							RenderUI( float DTime );

public:


};

/************************************************************************/
/*                        씬 오프닝 화면  [2014.12.2 ]
*/
/************************************************************************/
class CScene_Openning : public CScene

{
protected:

	CSprite									m_LoadImage; //[2014.12.3 ]


public:

	CScene_Openning();

	virtual ~CScene_Openning();

public:// 재정의 함수

	// override
	virtual int								InitObjects();
	// override
	virtual void							OnRenderStart();
	// override
	virtual void							OnRenderEnd();
	// override [2014.11.16 ]
	virtual void							OnUpdateStart(float _dTime);
	// override  [2014.12.2 ]
	virtual void							OnUpdateEnd(float _dTime);
	// override
	virtual DWORD							Notify(SH_MSG* _pMessage );

	// 텍스트 호출하는 함수 // [2014.12.1 ]
	virtual void							RenderText( float DTime );

	// UI그려주는 함수 재정의  // [2014.12.1 ]
	virtual void							RenderUI( float DTime );

public:


};

/************************************************************************/
/*                        씬 실패 화면  [2014.12.5 ]
*/
/************************************************************************/
class CScene_Fail : public CScene

{
protected:

	CSprite									m_LoadImage; //[2014.12.3 ]


public:

	CScene_Fail();

	virtual ~CScene_Fail();

public:// 재정의 함수

	// override
	virtual int								InitObjects();
	// override
	virtual void							OnRenderStart();
	// override
	virtual void							OnRenderEnd();
	// override [2014.11.16 ]
	virtual void							OnUpdateStart(float _dTime);
	// override  [2014.12.2 ]
	virtual void							OnUpdateEnd(float _dTime);
	// override
	virtual DWORD							Notify(SH_MSG* _pMessage );

	// 텍스트 호출하는 함수 // [2014.12.1 ]
	virtual void							RenderText( float DTime );

	// UI그려주는 함수 재정의  // [2014.12.1 ]
	virtual void							RenderUI( float DTime );

public:


};

/************************************************************************/
/*                        씬 성공 화면  [2014.12.5 ]
*/
/************************************************************************/
class CScene_Success : public CScene

{
protected:

	CSprite									m_LoadImage; //[2014.12.3 ]


public:

	CScene_Success();

	virtual ~CScene_Success();

public:// 재정의 함수

	// override
	virtual int								InitObjects();
	// override
	virtual void							OnRenderStart();
	// override
	virtual void							OnRenderEnd();
	// override [2014.11.16 ]
	virtual void							OnUpdateStart(float _dTime);
	// override  [2014.12.2 ]
	virtual void							OnUpdateEnd(float _dTime);
	// override
	virtual DWORD							Notify(SH_MSG* _pMessage );

	// 텍스트 호출하는 함수 // [2014.12.1 ]
	virtual void							RenderText( float DTime );

	// UI그려주는 함수 재정의  // [2014.12.1 ]
	virtual void							RenderUI( float DTime );

public:


};

/************************************************************************/
/*                        씬 성공 화면  [2014.12.5 ]
*/
/************************************************************************/
class CScene_Credit : public CScene

{
protected:

	CSprite									m_LoadImage; //[2014.12.3 ]


public:

	CScene_Credit();

	virtual ~CScene_Credit();

public:// 재정의 함수

	// override
	virtual int								InitObjects();
	// override
	virtual void							OnRenderStart();
	// override
	virtual void							OnRenderEnd();
	// override [2014.11.16 ]
	virtual void							OnUpdateStart(float _dTime);
	// override  [2014.12.2 ]
	virtual void							OnUpdateEnd(float _dTime);
	// override
	virtual DWORD							Notify(SH_MSG* _pMessage );

	// 텍스트 호출하는 함수 // [2014.12.1 ]
	virtual void							RenderText( float DTime );

	// UI그려주는 함수 재정의  // [2014.12.1 ]
	virtual void							RenderUI( float DTime );

public:


};