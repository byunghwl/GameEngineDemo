//////////////////////////////////////////////////////////////////////////
// 2014. 06. 19. 
// 
// d3d관련  함수 모음
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
// 여기서부터 namespace SHUtil 선언
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
	// 빛을 한색으로 초기화!!
	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);

	D3DMATERIAL9 InitMaterial(D3DXCOLOR diffuse, D3DXCOLOR amb, D3DXCOLOR specular, D3DXCOLOR emissive,D3DXCOLOR power);
	
	float getFPS(float _dTime);

	//-------------------------------------------------------------------------
	//  [2014.7.27 ]
	//	베지어 곡선의 변곡점을 랜덤으로 정해주는 함수 
	//	_Bezier[] : 변곡점이 들어갈 배열
	//  _Start    : 가장 첫번째 변곡점(시작점)
	//  _End	  : 가장 마지막 변곡점(도착점)
	//  _size	  : 변곡점 총 개수
	//  _Second   : 두번째 변곡점,사용자가  정해주기 (발사하고 바로꺽이면 이상하니까)
	//-------------------------------------------------------------------------
	void Rand_BezierPos(D3DXVECTOR3 _Bezier[],D3DXVECTOR3 _Start, D3DXVECTOR3 _End  , int _size,D3DXVECTOR3 _Second);
	//-------------------------------------------------------------------------
	// 베지어 곡선 함수 [2014.7.27 ]
	// Bezier[] 배지어 변곡점이 있는 배열
	// idx 에 Bezier[]배열의 가장 마지막 인덱스값 넣기 (size -1)
	// float a 는 진행율( 0.0f ~ 1.0f 사이의 값)
	//-------------------------------------------------------------------------
	D3DXVECTOR3  BezierCurb( D3DXVECTOR3 Bezier[], int idx ,float a );//베 지어 곡선

	void    MsgBoxW(LPCWSTR format, ...);
	void	MsgBox(LPCSTR format, ...);
}

//-----------------------------------------------------------------------------
/*float f=1.5f; 라고 선언합니다.	f를 비트로 표시하면 이렇습니다.	0011 1111 1100 0000 0000 0000 0000 0000
실수는 일반적인 2진수 계산 방법과 다릅니다.	그리고	int d; d=f; 라고 하면 d에는  1이라는 값이 대입됩니다. 소수점은 취급안하죠...
이때 d를 비트로 표시하면 	0000 0000 0000 0000 0000 0000 0000 0001 	일반적인 2진수로 보면 값은 1이죠...
실수를 정수 변수에 대입하면 메모리(비트)값 그대로 대입하지 않고	"소수점 이하를 뺀 값"으로서 대입을 합니다...
하지만 FtoDW()와 같이d=*(int*)&f; 로 대입하면 (말로 표현하면 f가 저장되어있는 메모리(비트)값 그대로 이제부터  
int형 처럼 취급한다 라고 선언하는겁니다.)	이때 d를 비트로 표시하면	0011 1111 1100 0000 0000 0000 0000 0000
실수에서 쓰인 비트 그대로 가져오죠...	메모리정보(비트)를 그대로 가져오기 위한 함수네요,,[2014.7.23 ]*/ 
//-----------------------------------------------------------------------------
DWORD FtoDw(float f);
// lowBound 와 highBound 사이의 float값을 랜덤으로 생성해낸다.
float GetRandomFloat(float lowBound, float highBound);

// 벡터의 각 성분(x,y,z) 의 min과  max 사이의 값으로 새로운 랜덤백터를 만들어낸다.
void GetRandomVector(D3DXVECTOR3* pOut,	const D3DXVECTOR3* pMin, const D3DXVECTOR3* pMax);

//-------------------------------------------------------------
// 파일 경로라고 가정하고 '\'나 '/'뒤에있는 문자만 남기도록 지운다
//-------------------------------------------------------------
void cutSlash(wchar_t* str);
void cutSlash(const WCHAR* str, WCHAR* _name);


/************************************************************************/
/*					class SH_Timer  // [2014.12.2 ]
				다양한곳에 쓰이기위한 타이머 
				
*/
/************************************************************************/
class SH_Timer
{
private:
	float				m_Time;// 누적시간
	bool				m_bisTimerOn;

public:// 생성자.. 소멸자

	SH_Timer(){m_Time =0.0f; m_bisTimerOn = false;}
	~SH_Timer(){};

public:// 일반함수 // [2014.12.2 ]
	// _Val  로 타이머를 맞춘다.
	void				SetTimer(float _Val){m_Time = _Val; m_bisTimerOn = true;}//
	// 맞춰진 시간이 다 되었나??  [2014.12.2 ]
	bool				isTimeUp();
	// 타이머꺼버리기 
	void				SetOff();
};


/************************************************************************/
/*       프러스텀 컬링을 하기위한 클래스                                */
/************************************************************************/
/// 프러스텀에 정확하게 포함되지 않더라도, 약간의 여분을 주어서 프러스텀에 포함시키기 위한 값
#define PLANE_EPSILON	5.0f


class ZFrustum
{
	D3DXVECTOR3		m_vtx[8];	/// 프러스텀을 구성할 정점 8개
	D3DXVECTOR3		m_vPos;		/// 현재 카메라의 월드좌표
	D3DXPLANE		m_plane[6];	/// 프러스텀을 구성하는 6개의 평면
	static  int    m_innerObjCnt; ///프러스텀안에 있는 오프젝트의 총 개수 ,, 매프레임마다 0으로 초기화 된다. 
public:
	
	/// 생성자
	ZFrustum();
	
	/// 카메라(view) * 프로젝션(projection)행렬을 입력받아 6개의 평면을 만든다.
	bool	Make( D3DXMATRIX* pmatViewProj );

	/** 중심(v)와 반지름(radius)를 갖는 경계구(bounding sphere)가 프러스텀안에 있으면
	 *  TRUE를 반환, 아니면 FALSE를 반환한다.
	 */
	bool	IsSphereIN( D3DXVECTOR3* pv, float radius );

	/// 프러스텀을 화면에 그려준다.
	bool	Draw( LPDIRECT3DDEVICE9 pDev );

public://get,,set
	/// 현재 카메라의 월드좌표를 얻어준다.
	D3DXVECTOR3*	GetPos() { return &m_vPos; }
	const int&		Get_InnerObjCnt() const {return m_innerObjCnt;}
};



/************************************************************************/
/*   충돌체 경계구 클래스                                               */
/************************************************************************/

class BoundingSphere
{
private:

	float				m_Radius; //반지름 
	D3DXVECTOR3			m_Pos , m_PosOrigine;    // 월드상의 위치
	bool				m_HaveMesh; // ID3DXMesh를 가지고 있나?
	ID3DXMesh*			m_Mesh; //화면상에 그려주기위한 메쉬
	D3DXMATRIX			matIdentity;

public:
	//생성자
	BoundingSphere();
	//소멸자
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


//모든 픽킹해줄 함수
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
// SH엔진의 통합셰이더 사용할때 ...테크닉안에서 사용할 수 있는 패스의 최대개수 [2014.11.8 ]
#define MaxRenderPass 7  

/***********************************************************************
                       class CFullScreenQuad
   포스트 프로세싱에 사용할 화면클라이언트영역 크기의 평면 사각형
						[2014.11.7 ]
************************************************************************/

class CFullScreenQuad
{
public: 
	// 장면 렌더타깃 텍스쳐 ,, 일단 7개까지만들어놓기
	static LPDIRECT3DTEXTURE9				g_pRenderTargetTexResult; //최종 화면에 그릴 렌더타겟 [2014.11.8 ]
	static LPDIRECT3DSURFACE9				g_pHWBackBuffer;		  // 현재 하드웨어 백버퍼 [2014.11.8 ]
	static LPDIRECT3DTEXTURE9				g_pRenderTargetTexPass[MaxRenderPass]; // [2014.11.8 ]
	static LPDIRECT3DSURFACE9				g_pSceneSurface[MaxRenderPass];

private:
	// 화면을 가득 채우는 사각형  [2014.11.7 ]
	LPDIRECT3DVERTEXDECLARATION9			m_pFullscreenQuadDecl;  
	LPDIRECT3DVERTEXBUFFER9					m_pFullscreenQuadVB;
	LPDIRECT3DINDEXBUFFER9					m_pFullscreenQuadIB;
	LPDIRECT3DDEVICE9						m_pD3DDevice;// D3D 장치


public:

	CFullScreenQuad(); // 생성자
	~CFullScreenQuad(); // 소멸자

public:
	// 화면전체크기에 맞는 사각형을 정의 ..post effect를 위해 [2014.11.7 ]
	int										Init();
	void									Draw();
	HRESULT									ClearAllSurface();// 모든 서페이스에 그려졌던 

};

//////////////////////////////////////////////////////////////////////////

extern CFullScreenQuad			G_ScreenQuad; // [2014.11.8 ]


//----------------------------------------------------------------------------
// 아크볼 함수 오른쪽마우스를 드래그 해서 회전시킨다.
// D3DXMATRIX* _pOut  :  반환값
//-----------------------------------------------------------------------------
//void ArcBall(GEOMOBJECT* _pBone );



/****************************************************** End *****************************************************/

