/**********************************************************************************************/
/* ���� ��ü����  ������ü�� ������ ���ְ� ������������ ������ ���� �������				  */
/*																							  */
/**********************************************************************************************/

#pragma  once

//////////////////////////////////////////////////////////////////////////
// ���༱��
class Camera;
class BoundingSphere;
class GEOMOBJECT;
class cSHBillBoard;
class cSmoke;
class AirCraft;
class cMissile;
class CBear;
class cSkyBox;
#define MAX_NUM_STRING 512


enum {
	// Ű���� ó���� ���� ����Ű ���� 
	eNumKey = 128, 
};

//-----------------------------------------------------------------------------
// ����,, �ܺ� ���� ����
//-----------------------------------------------------------------------------
extern   ZFrustum						G_Frustum;  //�������� �ø��Ҷ� ����� ��ü ,, 


		

//////////////////////////////////////////////////////////////////////////

extern   D3DMATERIAL9					G_DefaultMtrl; //�⺻ ���׸��� (white)
// ����Ʈ����Ʈ
extern D3DXVECTOR4						G_PointLightPosition;  // ����Ʈ����Ʈ�� ��ġ [2014.11.12 ]
extern float							G_PointLightRadius;    // ����Ʈ����Ʈ�� ��ġ [2014.11.12 ]
// ���� ���� (directional Light)
extern D3DXVECTOR3						G_vecDirLight;  
// ���� ��ǻ���
extern D3DXCOLOR						G_LightDiff;	
// ���� �����Ʈ
extern D3DXCOLOR						G_LightAmbi;	



//ȯ������Ҷ� ����� ��ī�̹ڽ��� ť����� ���������� ���´�.   [2014.8.3 ]
extern   LPDIRECT3DCUBETEXTURE9			G_pCubeTex;   
//----------------------------------------------------------------------------
// ���⼭���� ���̴� ��ü ����
//----------------------------------------------------------------------------
extern		LPDIRECT3DVERTEXSHADER9			G_pVtxShader ; //[2014.7.27 ]
extern		LPDIRECT3DPIXELSHADER9			G_pPStest;     // �ȼ� ���̴� �׽�Ʈ��  [2014.7.28 ]
extern      LPDIRECT3DVERTEXSHADER9			G_pVtxShader_Skin; //[2014.7.29 ]

//-----------------------------------------------------------------------------
// ���ӿ�����Ʈ�� ���������� ����
//-----------------------------------------------------------------------------
extern		AirCraft						G_AirCraft; //�����  
extern      cMissile						G_Missile;  //�̻���

extern		cSkyBox							G_SkyBox;  //��ī�̹ڽ�  [2014.8.2 ]		

//-----------------------------------------------------------------------------
// ���ӿ�����Ʈ���� ���Լ��ȿ��� �ʱ�ȭ�Ѵ�. [2014.7.25 ]
//-----------------------------------------------------------------------------
HRESULT Init_GameObjects(void);
//-----------------------------------------------------------------------------
// ���̴� �ʱ�ȭ �Լ� (main ���� ���ȴ�.)  [2014.7.27 ]
// ���̴� ��ü�� ������ �ִ� ���ӿ�����Ʈ����
// �̰����� GameObject::SetVtxShader()�Լ��� ����� �����͸� �����ؾ��Ѵ�.
//-----------------------------------------------------------------------------
HRESULT initShader( void );
//-----------------------------------------------------------------------------
// �Լ�����
//-----------------------------------------------------------------------------



/************************************************************************/
/* 
*/
/************************************************************************/
class SHGame : public CD3DFrameWork
{

private:
	SHGame();
	~SHGame();
protected:
	
	static SHGame*				    pInstance;

public: //
	
public:  // �ܺ��Լ�

	int 							Create(HINSTANCE _hInstance);

private:  //���� �Լ� and ������ �Լ�

	// �̰����� ������� , ����ϱ�, ���̾�ε� ����� �ʱ�ȭ ,, [2014.11.2 ]
	HRESULT							InitGame();
	//  UI �ʱ�ȭ [2014.11.3 ]
	HRESULT							Init_UI();
	virtual void					OnUpdateStart(float _dTime);
	virtual void				    OnUpdateEnd(float _dTime);

	virtual void					OnRenderStart();
	virtual void					OnRenderEnd();

	virtual void					OnCloseApp();
	// �ؽ�Ʈ ����Լ� ������
	//void							RenderText( float DTime );
	// UI�׷��ִ� �Լ� ������ 
	//void							RenderUI( float DTime );


public:  //���� Ȯ�� �Լ�

	

public: //������

	//�̱��� ��ü ����
	 static SHGame*					GetSHGame();

public:

	// SHGame �޼��� ó�� ���Լ�
	virtual void					OnGameMsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};



namespace __SH_GAME_FRAME__
{
#define SHGameFrame			SHGame::GetSHGame()

};



// [2014.11.2 ]
extern CDXUTDialog						g_MainPlayUI;             // dialog for sample specific controls
extern CDXUTDialog						g_MainTitle_UI;           // ����Ÿ��Ʋ�� ���� ���̾�α�
extern CDialogRscMgr      g_DialogResourceManager; // manager for shared resources of dialogs
/*************************************  end of "SHGame.h" ****************************************/