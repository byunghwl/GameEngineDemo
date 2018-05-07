#pragma once

//#define SH_DEBUG_
//메쉬 타입
enum eMesh_Type
{
	eMeshType_Normal,
	eMeshType_Skin,
	eMeshType_Helper,
	eMeshType_Shape,
	eMeshType_Bone,
	eMeshType_BoundSphere,//[2014.11.25 ]
	eMeshType_BoundBox
};

#define Max_Bone 100  // 한 스킨메쉬에 적용될수있는 본의 최대 개수
#define Max_Name_Len 512
#define Max_Path_Len 1024
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 버텍스+노말+텍스쳐
struct CUSTOM_VERTEX_NORM_TEX
{
	FLOAT x, y, z;      // The untransformed, 3D position for the vertex
	float nx, ny, nz;  //Normal
	float u,v;  //텍스쳐 좌표
	
};
#define D3DFVF_MYVERTEX_NORM_TEX1    (D3DFVF_XYZ | D3DFVF_NORMAL |D3DFVF_TEX1  )

//////////////////////////////////////////////////////////////////////////
// 그냥 버텍스 
struct CUSTOM_VERTEX_
{
	FLOAT x, y, z;      // The untransformed, 3D position for the vertex
};
#define D3DFVF_MYVERTEX			  (D3DFVF_XYZ  )

//////////////////////////////////////////////////////////////////////////
// 버텍스 + 노말
struct CUSTOM_VERTEX_NORM_
{
	FLOAT x, y, z;      // The untransformed, 3D position for the vertex
	float nx, ny, nz;  //Normal
};
#define D3DFVF_MYVERTEX_NORM     (D3DFVF_XYZ | D3DFVF_NORMAL   )

//////////////////////////////////////////////////////////////////////////
// 버텍스 + 텍스쳐
struct CUSTOM_VTX_XYZ_TEX1_
{
	FLOAT x, y, z;      // The untransformed, 3D position for the vertex
	float u,v;  //텍스쳐 좌표
};
#define D3DFVF_XYZ_TEX1		  (D3DFVF_XYZ | D3DFVF_TEX1)

//////////////////////////////////////////////////////////////////////////
// 버텍스 + 칼라
struct CUSTOM_VTX_XYZ_COLOR_
{
	D3DXVECTOR3 _position;
	D3DCOLOR    _color;
};
#define D3DFVF_XYZ_COLOR		  (D3DFVF_XYZ | D3DFVF_DIFFUSE)

//-----------------------------------------------------------------------------
// 메트릭스팔레트 사용하기 위한  버텍스 형식 정의 
//-----------------------------------------------------------------------------
struct CUSTOM_SKINNED_VERTEX
{
	D3DXVECTOR3	pos;
	float		bWeight[3];
	DWORD		Boneidx;
	D3DXVECTOR3	norm;
	D3DXVECTOR2	tex;

	// 복사를위한 연산자 오버라이딩
	CUSTOM_SKINNED_VERTEX& operator=(const CUSTOM_SKINNED_VERTEX& rhs)
	{
		pos = rhs.pos; Boneidx = rhs.Boneidx; norm = rhs.norm ;  tex = rhs.tex;
		memcpy_s(bWeight,  3*sizeof(float) , rhs.bWeight,  3*sizeof(float)	);

		return *this;
	}

};

#define   D3DFVF_MYVERTEX_SKINNED_VTX  (D3DFVF_XYZB4 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_TEX1)

#define BLENDINDEX( i4, i3, i2, i1 )		((i4)<<24|(i3)<<16|(i2)<<8|(i1))
//////////////////////////////////////////////////////////////////////////

#define MY_VERTEX			 CUSTOM_VERTEX_NORM_TEX
#define MY_FVF				 (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1  )

#define MY_VERTEX_SKINED	 CUSTOM_SKINNED_VERTEX
#define MY_FVF_SKINED		 (D3DFVF_XYZB4 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_TEX1)

// 큐브텍스쳐를 사용할때 설정해야하는 fvf  [2014.8.3 ]
#define CubeTexFVF  ( D3DFVF_XYZ | D3DFVF_TEX1 |  D3DFVF_TEXCOORDSIZE3(0) )  

// 버텍스+노말+텍스쳐  [2014.8.3 ]
struct CUSTOM_VERTEX_Cube_Tex
{
	D3DXVECTOR3  vtx;      // The untransformed, 3D position for the vertex
	D3DXVECTOR3  texcoord;  //텍스쳐 좌표

};



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 색정보 정의

#define COLOR_WHITE   D3DCOLOR_ARGB(0,255, 255, 255)
#define COLOR_BLACK   D3DCOLOR_XRGB(  0,   0,   0) 
#define COLOR_RED     D3DCOLOR_XRGB(255,   0,   0) 
#define COLOR_GREEN   D3DCOLOR_XRGB(  0, 255,   0) 
#define COLOR_BLUE    D3DCOLOR_XRGB(  0,   0, 255)
#define COLOR_YELLOW  D3DCOLOR_XRGB(255, 255,   0) 
#define COLOR_CYAN    D3DCOLOR_XRGB(  0, 255, 255) 
#define COLOR_MAGENTA D3DCOLOR_XRGB(255,   0, 255) 
#define DEFAULT_MATERIAL SHUtil::InitMaterial(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE,  COLOR_WHITE, 0.4f)


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//메모리 해제
#define SAFE_DELETE_ARR(val) if(val!=NULL) {delete[] val; val = NULL;}

#define SAFE_DELETE_(val) if(val!=NULL) {delete val; val = NULL;} 
	
/*#define SAFE_RELEASE(val) if(val != NULL) { val->Release(); val = NULL;}*/
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 어설트
#define ASSERT_IF_FAIL_MemAlloc(_val)  if(_val == NULL){write_log_file("메모리 할당 실패 : "#_val);assert(FALSE);} 
//널포인터로 검사할경우
//#define ASSERT_MAllocFail_Nullptr(_val)  if(_val == nullptr){write_log_file("메모리 할당 실패 : "#_val);assert(FALSE);}  
#define ASSERT_IF_FAIL_D_CAST(_val)  if(_val == NULL){write_log_file("다이나믹 캐스트 실패 : "#_val);assert(FALSE);} 
#define ASSERT_IF_NULL(_val)  if(_val == NULL){write_log_file("포인터변수가 NULL 입니다 : "#_val);assert(FALSE);} 
//SH_TRACE 메시지박스를 띄우고 어썰트
#define SH_TRACE()   LOG_WRITE_A("<assert> File : %s, Line : %s",__FILE__, __LINE__ );  assert(false); 


//키입력
//0x0000: 이전에 누른 적이 없고 호출시점에 안 눌린 상태
//0x8000: 이전에 누른 적이 없고 호출시점에 눌린 상태
//0x8001: 이전에 누른 적이 있고 호출시점에 눌린 상태
//0x0001: 이전에 누른 적이 있고 호출시점에 안 눌린 상태
#define KEY_DOWN(vk_code)				 ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)					 ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
#define KEY_DOWN_CONTINUE(vk_code)		((GetAsyncKeyState(vk_code) & 0x8001) ? 1 : 0)
#define KEY_UP_CONTINUE(vk_code)        ((GetAsyncKeyState(vk_code) & 0x8001) ? 0 : 1)

#define FLAG_ON(_FLAG, _VAL)		_FLAG |= _VAL ;
#define FLAG_OF(_FLAG, _VAL)		_FLAG &= ~(_VAL) ;


//에러처리
#define  SH_RETURN(_val) { if(_val == S_FALSE){SHUtil::MsgBox("File : %s\nLine :%s", "하하하", "gㅎㅎㅎ하"); LOG_WRITE_A(":File : %s, Line : %s", __FILE__, __LINE__ );} 	return _val; }
					