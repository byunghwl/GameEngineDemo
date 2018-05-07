// 2014. 05
// 
// �浹üũ �Լ� ����
//
//
//

#pragma once

#define   POSITION D3DXVECTOR3
#define   VECTOR3  D3DXVECTOR3
//////////////////////////////////////////////////////////////////////////
//
// �浹üũ�� ��ȯ���� �ڼ��� Ÿ�ԡ��϶��� ���� ����
enum LINE_TYPE
{
	LT_NONE,
	LT_LEFT,
	LT_RIGHT,
	LT_TOP,
	LT_BOTTOM
};


//////////////////////////////////////////////////////////////////////////
//
// �ﰢ���� �浹üũ�� ��ȯ���� �ڼ��� Ÿ�ԡ��϶��� ���� ����
enum LINE_TYPE_TRI
{
	LTT_NONE,
	LTT_TOP,
	LTT_LEFT,
	LTT_RIGHT
	
};
typedef struct DIST_NUM
{
	float Dist; // ���� ������ �Ÿ��� ��� ������ ����
	int num; // ���й�ȣ�� ������ ����

}DIST_NUM;


/***********************************************************************
						struct	SH_CollideInfo

					 �浹�������� �浹���� ������ �Ѱ��ֱ� ���� 
						����ü  [2014.11.10 ]
************************************************************************/
struct SH_CollideInfo
{
	float						Velo; // �ӵ�
	D3DXVECTOR3					Direction;//���⺤��
	bool						X,Z; //�浹������ �簢���� �ε��� �� ??  [2014.11.25 ]

	SH_CollideInfo(){Velo = 0.0f; Direction = D3DXVECTOR3(0.0f, 0.0f, 0.0f);}
};

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
// ���⼭���� ...�Լ�����
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




//////////////////////////////////////////////////////////////////////////
// ������ ���� �Ÿ� ���ϱ�
//
// ���ϰ� : ���� ������ �ִܰŸ� 2D ��!!!!!!!!!!!!!!!!!!�ڡڡ�
// _p : ��
// _Norm : ax + bx + c = 0 ������ �����Ŀ��� (a,b)�� �ش��ϴ� ��( 2�����̹Ƿ� z���� 0.0f) ,, ������ ����������
// Spos : ������ ������

float CalDist_Vert_Line(POSITION _p , VECTOR3 _Norm , POSITION _Spos );



//////////////////////////////////////////////////////////////////////////
// �ݻ纤�� ���ϴ� �Լ�
// _DirIn : �Ի� ����
// _Norm  : �ݻ��Ұ��� �븻����
//
VECTOR3 Cal_Reflect_Vec(VECTOR3 _DirIn, VECTOR3 _Norm);


//////////////////////////////////////////////////////////////////////////
// ���� ������ ���ʿ� �ִ��� �Ǻ��ϴ� �Լ� 2D ��!!!!!!!!!!!!!!!!!!�ڡڡ�
// _S : ������ ������
// _V : ����
// _p : ������ ��

bool IsLeft_Vert_Vec( POSITION _S, D3DXVECTOR3 _V1, POSITION _p );

//////////////////////////////////////////////////////////////////////////
// ���� ������ ���ʿ� �ִ��� �Ǻ��ϴ� �Լ� 2D ��!!!!!!!!!!!!!!!!!!�ڡڡ�
// _S : ������ ������
// _V : ����1
//_V2 : ����2

bool IsLeft_Vec_Vec(  D3DXVECTOR3 _V1, D3DXVECTOR3 _V2 );

//////////////////////////////////////////////////////////////////////////
// ���Ͱ� ���� ������ ���ʿ� �ִ��� �Ǻ��ϴ� �Լ�
// _V1: ���غ���(x��)
// _v2: ������������������ �Ǻ��� ����(z��) �ڡڡڡ� 3D ��!!!!!!!!!!!!!!!!!!�ڡڡ�

bool IsLeft_Vec_Vec_3D(  D3DXVECTOR3 _V1, D3DXVECTOR3 _V2 );
//-----------------------------------------------------------------------------
// 3D�󿡼� ���� ������ �浹
//-----------------------------------------------------------------------------

bool Collision_Sphere_Line(const D3DXVECTOR3& _SphereCenter,
							const float& _Raius,
							const D3DXVECTOR3& _LineBegine, 
							const D3DXVECTOR3& _LineDirection );


//-------------------------------------------------------------------------
// Arg	  : const D3DXVECTOR3& Sp1_Center , const float& Sp1_Rad  //��1�� ������ ������
//			const D3DXVECTOR3& Sp2_Center , const float& Sp2_Rad  // ��2�� ������ ������
// Return :
// Desc   : 3D���� �� ���� �浹�׽�Ʈ �ϴ� �Լ�[2014.11.11 ]
//-------------------------------------------------------------------------
bool CollCheck_Sphere_vs_Shpere(const D3DXVECTOR3& Sp1_Center , const float& Sp1_Rad , const D3DXVECTOR3& Sp2_Center , const float& Sp2_Rad  );


//-------------------------------------------------------------------------
// Arg	  : const D3DXVECTOR3& Sp1_Center , const float& Sp1_Rad  //��1�� ������ ������
//			const D3DXVECTOR3& Box_Center ,  const D3DXVECTOR3& Box_Max, const D3DXVECTOR3& Box_Min //aabb �ڽ���  ����, �ִ���, �˼���
// Return : bool 
// Desc   : 3D���� ���� aabb�ڽ��� �浹�׽�Ʈ �ϴ� �Լ�   [2014.11.11 ]
//-------------------------------------------------------------------------
bool CollCheck_Sphere_vs_BoxAABB(const D3DXVECTOR3& Sp1_Center , const float& Sp1_Rad ,
								 const D3DXVECTOR3& Box_Center , const D3DXVECTOR3& Box_Max, const D3DXVECTOR3& Box_Min );