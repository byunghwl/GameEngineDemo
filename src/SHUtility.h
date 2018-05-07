//////////////////////////////////////////////////////////////////////////
// 2014. 06. 19. 
// 
// d3d����  �Լ� ����
//
//////////////////////////////////////////////////////////////////////////


#pragma once


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
// ���⼭���� namespace SHUtil ����
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
namespace SHUtil
{
	//////////////////////////////////////////////////////////////////////////
	// ���� �ѻ����� �ʱ�ȭ!!
	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);

	D3DMATERIAL9 InitMaterial(D3DXCOLOR diffuse, D3DXCOLOR amb, D3DXCOLOR specular, D3DXCOLOR emissive,D3DXCOLOR power);
	
	float getFPS(float _dTime);

	//-------------------------------------------------------------------------
	//  [2014.7.27 ]
	//	������ ��� �������� �������� �����ִ� �Լ� 
	//	_Bezier[] : �������� �� �迭
	//  _Start    : ���� ù��° ������(������)
	//  _End	  : ���� ������ ������(������)
	//  _size	  : ������ �� ����
	//  _Second   : �ι�° ������,����ڰ�  �����ֱ� (�߻��ϰ� �ٷβ��̸� �̻��ϴϱ�)
	//-------------------------------------------------------------------------
	void Rand_BezierPos(D3DXVECTOR3 _Bezier[],D3DXVECTOR3 _Start, D3DXVECTOR3 _End  , int _size,D3DXVECTOR3 _Second);
	//-------------------------------------------------------------------------
	// ������ � �Լ� [2014.7.27 ]
	// Bezier[] ������ �������� �ִ� �迭
	// idx �� Bezier[]�迭�� ���� ������ �ε����� �ֱ� (size -1)
	// float a �� ������( 0.0f ~ 1.0f ������ ��)
	//-------------------------------------------------------------------------
	D3DXVECTOR3  BezierCurb( D3DXVECTOR3 Bezier[], int idx ,float a );//�� ���� �

	void    MsgBoxW(LPCWSTR format, ...);
	void	MsgBox(LPCSTR format, ...);
}

//-----------------------------------------------------------------------------
/*float f=1.5f; ��� �����մϴ�.	f�� ��Ʈ�� ǥ���ϸ� �̷����ϴ�.	0011 1111 1100 0000 0000 0000 0000 0000
�Ǽ��� �Ϲ����� 2���� ��� ����� �ٸ��ϴ�.	�׸���	int d; d=f; ��� �ϸ� d����  1�̶�� ���� ���Ե˴ϴ�. �Ҽ����� ��޾�����...
�̶� d�� ��Ʈ�� ǥ���ϸ� 	0000 0000 0000 0000 0000 0000 0000 0001 	�Ϲ����� 2������ ���� ���� 1����...
�Ǽ��� ���� ������ �����ϸ� �޸�(��Ʈ)�� �״�� �������� �ʰ�	"�Ҽ��� ���ϸ� �� ��"���μ� ������ �մϴ�...
������ FtoDW()�� ����d=*(int*)&f; �� �����ϸ� (���� ǥ���ϸ� f�� ����Ǿ��ִ� �޸�(��Ʈ)�� �״�� ��������  
int�� ó�� ����Ѵ� ��� �����ϴ°̴ϴ�.)	�̶� d�� ��Ʈ�� ǥ���ϸ�	0011 1111 1100 0000 0000 0000 0000 0000
�Ǽ����� ���� ��Ʈ �״�� ��������...	�޸�����(��Ʈ)�� �״�� �������� ���� �Լ��׿�,,[2014.7.23 ]*/ 
//-----------------------------------------------------------------------------
DWORD FtoDw(float f);
// lowBound �� highBound ������ float���� �������� �����س���.
float GetRandomFloat(float lowBound, float highBound);

// ������ �� ����(x,y,z) �� min��  max ������ ������ ���ο� �������͸� ������.
void GetRandomVector(D3DXVECTOR3* pOut,	const D3DXVECTOR3* pMin, const D3DXVECTOR3* pMax);

//-------------------------------------------------------------
// ���� ��ζ�� �����ϰ� '\'�� '/'�ڿ��ִ� ���ڸ� ���⵵�� �����
//-------------------------------------------------------------
void cutSlash(wchar_t* str);
void cutSlash(const WCHAR* str, WCHAR* _name);


/************************************************************************/
/*					class SH_Timer  // [2014.12.2 ]
				�پ��Ѱ��� ���̱����� Ÿ�̸� 
				
*/
/************************************************************************/
class SH_Timer
{
private:
	float				m_Time;// �����ð�
	bool				m_bisTimerOn;

public:// ������.. �Ҹ���

	SH_Timer(){m_Time =0.0f; m_bisTimerOn = false;}
	~SH_Timer(){};

public:// �Ϲ��Լ� // [2014.12.2 ]
	// _Val  �� Ÿ�̸Ӹ� �����.
	void				SetTimer(float _Val){m_Time = _Val; m_bisTimerOn = true;}//
	// ������ �ð��� �� �Ǿ���??  [2014.12.2 ]
	bool				isTimeUp();
	// Ÿ�̸Ӳ������� 
	void				SetOff();
};


/************************************************************************/
/*       �������� �ø��� �ϱ����� Ŭ����                                */
/************************************************************************/
/// �������ҿ� ��Ȯ�ϰ� ���Ե��� �ʴ���, �ణ�� ������ �־ �������ҿ� ���Խ�Ű�� ���� ��
#define PLANE_EPSILON	5.0f


class ZFrustum
{
	D3DXVECTOR3		m_vtx[8];	/// ���������� ������ ���� 8��
	D3DXVECTOR3		m_vPos;		/// ���� ī�޶��� ������ǥ
	D3DXPLANE		m_plane[6];	/// ���������� �����ϴ� 6���� ���
	static  int    m_innerObjCnt; ///�������Ҿȿ� �ִ� ������Ʈ�� �� ���� ,, �������Ӹ��� 0���� �ʱ�ȭ �ȴ�. 
public:
	
	/// ������
	ZFrustum();
	
	/// ī�޶�(view) * ��������(projection)����� �Է¹޾� 6���� ����� �����.
	bool	Make( D3DXMATRIX* pmatViewProj );

	/** �߽�(v)�� ������(radius)�� ���� ��豸(bounding sphere)�� �������Ҿȿ� ������
	 *  TRUE�� ��ȯ, �ƴϸ� FALSE�� ��ȯ�Ѵ�.
	 */
	bool	IsSphereIN( D3DXVECTOR3* pv, float radius );

	/// ���������� ȭ�鿡 �׷��ش�.
	bool	Draw( LPDIRECT3DDEVICE9 pDev );

public://get,,set
	/// ���� ī�޶��� ������ǥ�� ����ش�.
	D3DXVECTOR3*	GetPos() { return &m_vPos; }
	const int&		Get_InnerObjCnt() const {return m_innerObjCnt;}
};



/************************************************************************/
/*   �浹ü ��豸 Ŭ����                                               */
/************************************************************************/

class BoundingSphere
{
private:

	float				m_Radius; //������ 
	D3DXVECTOR3			m_Pos , m_PosOrigine;    // ������� ��ġ
	bool				m_HaveMesh; // ID3DXMesh�� ������ �ֳ�?
	ID3DXMesh*			m_Mesh; //ȭ��� �׷��ֱ����� �޽�
	D3DXMATRIX			matIdentity;

public:
	//������
	BoundingSphere();
	//�Ҹ���
	virtual ~BoundingSphere();

	HRESULT					init(float _radius,  bool _HaveMesh  ,D3DXVECTOR3 _pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

public:
	//operator
	BoundingSphere&			operator=(const BoundingSphere& rhs){ DeepCopy(rhs); return *this; }
public:

	void					Draw( const D3DXMATRIX* _WorldTM);
	HRESULT					DeepCopy(const BoundingSphere& rhs);

public:
	//get..set
	const float&			get_Radius() const{return m_Radius;} 
	const D3DXVECTOR3&		get_Pos() const{return m_Pos;}


};


//��� ��ŷ���� �Լ�
HRESULT Pick(float _dTime);


/***********************************************************************
        Manages the insertion point when drawing text                              
************************************************************************/
class SHEngine_TextHelper
{
public:
	SHEngine_TextHelper( ID3DXFont* pFont9 = NULL, ID3DXSprite* pSprite9 = NULL, int nLineHeight = 15 );
	~SHEngine_TextHelper();

	void Init( ID3DXFont* pFont9 = NULL, ID3DXSprite* pSprite9 = NULL, int nLineHeight = 15 );

	void SetInsertionPos( int x, int y ) { m_pt.x = x; m_pt.y = y; }
	void SetForegroundColor( D3DXCOLOR clr ) { m_clr = clr; }

	void Begin();
	HRESULT DrawFormattedTextLine( const WCHAR* strMsg, ... );
	HRESULT DrawTextLine( const WCHAR* strMsg );
	HRESULT DrawFormattedTextLine( RECT &rc, DWORD dwFlags, const WCHAR* strMsg, ... );
	HRESULT DrawTextLine( RECT &rc, DWORD dwFlags, const WCHAR* strMsg );
	void End();

protected:
	ID3DXFont*   m_pFont9;
	ID3DXSprite* m_pSprite9;
	D3DXCOLOR    m_clr;
	POINT        m_pt;
	int          m_nLineHeight;
};



//////////////////////////////////////////////////////////////////////////
// SH������ ���ռ��̴� ����Ҷ� ...��ũ�оȿ��� ����� �� �ִ� �н��� �ִ밳�� [2014.11.8 ]
#define MaxRenderPass 7  

/***********************************************************************
                       class CFullScreenQuad
   ����Ʈ ���μ��̿� ����� ȭ��Ŭ���̾�Ʈ���� ũ���� ��� �簢��
						[2014.11.7 ]
************************************************************************/

class CFullScreenQuad
{
public: 
	// ��� ����Ÿ�� �ؽ��� ,, �ϴ� 7��������������
	static LPDIRECT3DTEXTURE9				g_pRenderTargetTexResult; //���� ȭ�鿡 �׸� ����Ÿ�� [2014.11.8 ]
	static LPDIRECT3DSURFACE9				g_pHWBackBuffer;		  // ���� �ϵ���� ����� [2014.11.8 ]
	static LPDIRECT3DTEXTURE9				g_pRenderTargetTexPass[MaxRenderPass]; // [2014.11.8 ]
	static LPDIRECT3DSURFACE9				g_pSceneSurface[MaxRenderPass];

private:
	// ȭ���� ���� ä��� �簢��  [2014.11.7 ]
	LPDIRECT3DVERTEXDECLARATION9			m_pFullscreenQuadDecl;  
	LPDIRECT3DVERTEXBUFFER9					m_pFullscreenQuadVB;
	LPDIRECT3DINDEXBUFFER9					m_pFullscreenQuadIB;
	LPDIRECT3DDEVICE9						m_pD3DDevice;// D3D ��ġ


public:

	CFullScreenQuad(); // ������
	~CFullScreenQuad(); // �Ҹ���

public:
	// ȭ����üũ�⿡ �´� �簢���� ���� ..post effect�� ���� [2014.11.7 ]
	int										Init();
	void									Draw();
	HRESULT									ClearAllSurface();// ��� �����̽��� �׷����� 

};

//////////////////////////////////////////////////////////////////////////

extern CFullScreenQuad			G_ScreenQuad; // [2014.11.8 ]


//----------------------------------------------------------------------------
// ��ũ�� �Լ� �����ʸ��콺�� �巡�� �ؼ� ȸ����Ų��.
// D3DXMATRIX* _pOut  :  ��ȯ��
//-----------------------------------------------------------------------------
//void ArcBall(GEOMOBJECT* _pBone );



/****************************************************** End *****************************************************/

