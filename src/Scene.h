/************************************************************************/
/*								Scene.h
		CScene Class �� ��ӹ��� ���� �ڽ�Scene ������ �������
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

public:// ������ �Լ�
	
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

	// �ؽ�Ʈ ȣ���ϴ� �Լ� // [2014.12.1 ]
	virtual void							RenderText( float DTime );

	// UI�׷��ִ� �Լ� ������  // [2014.12.1 ]
	virtual void							RenderUI( float DTime );

public:

	
};



/************************************************************************/
/*                        �� Ÿ��Ʋ ȭ��  [2014.12.2 ]
*/
/************************************************************************/
class CScene_Title : public CScene

{
protected:



public:

	CScene_Title();

	virtual ~CScene_Title();

public:// ������ �Լ�

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

	// �ؽ�Ʈ ȣ���ϴ� �Լ� // [2014.12.1 ]
	virtual void							RenderText( float DTime );

	// UI�׷��ִ� �Լ� ������  // [2014.12.1 ]
	virtual void							RenderUI( float DTime );

public:


};

/************************************************************************/
/*                        �� ������ ȭ��  [2014.12.2 ]
*/
/************************************************************************/
class CScene_Openning : public CScene

{
protected:

	CSprite									m_LoadImage; //[2014.12.3 ]


public:

	CScene_Openning();

	virtual ~CScene_Openning();

public:// ������ �Լ�

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

	// �ؽ�Ʈ ȣ���ϴ� �Լ� // [2014.12.1 ]
	virtual void							RenderText( float DTime );

	// UI�׷��ִ� �Լ� ������  // [2014.12.1 ]
	virtual void							RenderUI( float DTime );

public:


};

/************************************************************************/
/*                        �� ���� ȭ��  [2014.12.5 ]
*/
/************************************************************************/
class CScene_Fail : public CScene

{
protected:

	CSprite									m_LoadImage; //[2014.12.3 ]


public:

	CScene_Fail();

	virtual ~CScene_Fail();

public:// ������ �Լ�

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

	// �ؽ�Ʈ ȣ���ϴ� �Լ� // [2014.12.1 ]
	virtual void							RenderText( float DTime );

	// UI�׷��ִ� �Լ� ������  // [2014.12.1 ]
	virtual void							RenderUI( float DTime );

public:


};

/************************************************************************/
/*                        �� ���� ȭ��  [2014.12.5 ]
*/
/************************************************************************/
class CScene_Success : public CScene

{
protected:

	CSprite									m_LoadImage; //[2014.12.3 ]


public:

	CScene_Success();

	virtual ~CScene_Success();

public:// ������ �Լ�

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

	// �ؽ�Ʈ ȣ���ϴ� �Լ� // [2014.12.1 ]
	virtual void							RenderText( float DTime );

	// UI�׷��ִ� �Լ� ������  // [2014.12.1 ]
	virtual void							RenderUI( float DTime );

public:


};

/************************************************************************/
/*                        �� ���� ȭ��  [2014.12.5 ]
*/
/************************************************************************/
class CScene_Credit : public CScene

{
protected:

	CSprite									m_LoadImage; //[2014.12.3 ]


public:

	CScene_Credit();

	virtual ~CScene_Credit();

public:// ������ �Լ�

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

	// �ؽ�Ʈ ȣ���ϴ� �Լ� // [2014.12.1 ]
	virtual void							RenderText( float DTime );

	// UI�׷��ִ� �Լ� ������  // [2014.12.1 ]
	virtual void							RenderUI( float DTime );

public:


};