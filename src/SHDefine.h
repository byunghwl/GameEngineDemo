#pragma once

//#define SH_DEBUG_
//�޽� Ÿ��
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

#define Max_Bone 100  // �� ��Ų�޽��� ����ɼ��ִ� ���� �ִ� ����
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
// ���ؽ�+�븻+�ؽ���
struct CUSTOM_VERTEX_NORM_TEX
{
	FLOAT x, y, z;      // The untransformed, 3D position for the vertex
	float nx, ny, nz;  //Normal
	float u,v;  //�ؽ��� ��ǥ
	
};
#define D3DFVF_MYVERTEX_NORM_TEX1    (D3DFVF_XYZ | D3DFVF_NORMAL |D3DFVF_TEX1  )

//////////////////////////////////////////////////////////////////////////
// �׳� ���ؽ� 
struct CUSTOM_VERTEX_
{
	FLOAT x, y, z;      // The untransformed, 3D position for the vertex
};
#define D3DFVF_MYVERTEX			  (D3DFVF_XYZ  )

//////////////////////////////////////////////////////////////////////////
// ���ؽ� + �븻
struct CUSTOM_VERTEX_NORM_
{
	FLOAT x, y, z;      // The untransformed, 3D position for the vertex
	float nx, ny, nz;  //Normal
};
#define D3DFVF_MYVERTEX_NORM     (D3DFVF_XYZ | D3DFVF_NORMAL   )

//////////////////////////////////////////////////////////////////////////
// ���ؽ� + �ؽ���
struct CUSTOM_VTX_XYZ_TEX1_
{
	FLOAT x, y, z;      // The untransformed, 3D position for the vertex
	float u,v;  //�ؽ��� ��ǥ
};
#define D3DFVF_XYZ_TEX1		  (D3DFVF_XYZ | D3DFVF_TEX1)

//////////////////////////////////////////////////////////////////////////
// ���ؽ� + Į��
struct CUSTOM_VTX_XYZ_COLOR_
{
	D3DXVECTOR3 _position;
	D3DCOLOR    _color;
};
#define D3DFVF_XYZ_COLOR		  (D3DFVF_XYZ | D3DFVF_DIFFUSE)

//-----------------------------------------------------------------------------
// ��Ʈ�����ȷ�Ʈ ����ϱ� ����  ���ؽ� ���� ���� 
//-----------------------------------------------------------------------------
struct CUSTOM_SKINNED_VERTEX
{
	D3DXVECTOR3	pos;
	float		bWeight[3];
	DWORD		Boneidx;
	D3DXVECTOR3	norm;
	D3DXVECTOR2	tex;

	// ���縦���� ������ �������̵�
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

// ť���ؽ��ĸ� ����Ҷ� �����ؾ��ϴ� fvf  [2014.8.3 ]
#define CubeTexFVF  ( D3DFVF_XYZ | D3DFVF_TEX1 |  D3DFVF_TEXCOORDSIZE3(0) )  

// ���ؽ�+�븻+�ؽ���  [2014.8.3 ]
struct CUSTOM_VERTEX_Cube_Tex
{
	D3DXVECTOR3  vtx;      // The untransformed, 3D position for the vertex
	D3DXVECTOR3  texcoord;  //�ؽ��� ��ǥ

};



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// ������ ����

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
//�޸� ����
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
// �Ʈ
#define ASSERT_IF_FAIL_MemAlloc(_val)  if(_val == NULL){write_log_file("�޸� �Ҵ� ���� : "#_val);assert(FALSE);} 
//�������ͷ� �˻��Ұ��
//#define ASSERT_MAllocFail_Nullptr(_val)  if(_val == nullptr){write_log_file("�޸� �Ҵ� ���� : "#_val);assert(FALSE);}  
#define ASSERT_IF_FAIL_D_CAST(_val)  if(_val == NULL){write_log_file("���̳��� ĳ��Ʈ ���� : "#_val);assert(FALSE);} 
#define ASSERT_IF_NULL(_val)  if(_val == NULL){write_log_file("�����ͺ����� NULL �Դϴ� : "#_val);assert(FALSE);} 
//SH_TRACE �޽����ڽ��� ���� ���Ʈ
#define SH_TRACE()   LOG_WRITE_A("<assert> File : %s, Line : %s",__FILE__, __LINE__ );  assert(false); 


//Ű�Է�
//0x0000: ������ ���� ���� ���� ȣ������� �� ���� ����
//0x8000: ������ ���� ���� ���� ȣ������� ���� ����
//0x8001: ������ ���� ���� �ְ� ȣ������� ���� ����
//0x0001: ������ ���� ���� �ְ� ȣ������� �� ���� ����
#define KEY_DOWN(vk_code)				 ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)					 ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
#define KEY_DOWN_CONTINUE(vk_code)		((GetAsyncKeyState(vk_code) & 0x8001) ? 1 : 0)
#define KEY_UP_CONTINUE(vk_code)        ((GetAsyncKeyState(vk_code) & 0x8001) ? 0 : 1)

#define FLAG_ON(_FLAG, _VAL)		_FLAG |= _VAL ;
#define FLAG_OF(_FLAG, _VAL)		_FLAG &= ~(_VAL) ;


//����ó��
#define  SH_RETURN(_val) { if(_val == S_FALSE){SHUtil::MsgBox("File : %s\nLine :%s", "������", "g��������"); LOG_WRITE_A(":File : %s, Line : %s", __FILE__, __LINE__ );} 	return _val; }
					