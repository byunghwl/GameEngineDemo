// 2014. 05
// 
//
//
//
//
#include "DXUT.h"

#include "SHCollision.h"
#include "math.h"




//////////////////////////////////////////////////////////////////////////
// ���� ������ ���ʿ� �ִ��� �Ǻ��ϴ� �Լ�  �ڡڡ�2D ��!!!!!!!!!!!!!!!!!!�ڡڡ�
// _S : ������ ������
// _V : ����
// _p : ������ ��

bool IsLeft_Vert_Vec( POSITION _S, D3DXVECTOR3 _V1, POSITION _p )
{
	D3DXVECTOR3 _V2 = _p - _S;
	D3DXVECTOR3 tempCross;

	D3DXVec3Normalize(&_V1, &_V1);
	D3DXVec3Normalize(&_V2, &_V2);

	D3DXVec3Cross(&tempCross,&_V1,&_V2);


	if(tempCross.z < 0.0f)
	{
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// ������ ���� �Ÿ� ���ϱ� �ڡڡ�2D ��!!!!!!!!!!!!!!!!!!�ڡڡ�
//
// ���ϰ� : ���� ������ �ִܰŸ�
// _p : ��
// _Norm : ax + bx + c = 0 ������ �����Ŀ��� (a,b)�� �ش��ϴ� ��( 2�����̹Ƿ� z���� 0.0f) ,, ������ ����������
// Spos : ���� ������ ������

float CalDist_Vert_Line(POSITION _p , VECTOR3 _Norm , POSITION _Spos )
{
	float C; // ax + bx + c = 0 ���� C�� �ش��ϴ� ��
	float R; // ���� ������ �ִܰŸ�
	float a, b , m, n;

	a= _Norm.x;   b = _Norm.y;   m = _p.x;    n = _p.y;

	C = -1.0f * (a*_Spos.x + b*_Spos.y);

	float temp = a*m + b*n + C;

	if(temp<0) // ���밪 �����ֱ�
		temp*=-1.0f;

	R =  temp/(sqrt(a*a + b*b));


	return R;
}


//////////////////////////////////////////////////////////////////////////
// ���Ͱ� ���� ������ ���ʿ� �ִ��� �Ǻ��ϴ� �Լ�
// _V1: ���غ���
// _v2: ������������������ �Ǻ��� ���� ,2D������ ��밡�ɡڡڡڡ� 2D ��!!!!!!!!!!!!!!!!!!�ڡڡ�

bool IsLeft_Vec_Vec(  D3DXVECTOR3 _V1, D3DXVECTOR3 _V2 )
{
	
	D3DXVECTOR3 tempCross;

	D3DXVec3Normalize(&_V1, &_V1);
	D3DXVec3Normalize(&_V2, &_V2);

	D3DXVec3Cross(&tempCross,&_V1,&_V2);


	if(tempCross.z < 0.0f)
	{
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// ���Ͱ� ���� ������ ���ʿ� �ִ��� �Ǻ��ϴ� �Լ�
// _V1: ���غ���(x��)
// _v2: ������������������ �Ǻ��� ����(z��) �ڡڡڡ� 3D ��!!!!!!!!!!!!!!!!!!�ڡڡ�

bool IsLeft_Vec_Vec_3D(  D3DXVECTOR3 _V1, D3DXVECTOR3 _V2 )
{

	D3DXVECTOR3 tempCross;

	D3DXVec3Normalize(&_V1, &_V1);
	D3DXVec3Normalize(&_V2, &_V2);

	D3DXVec3Cross(&tempCross,&_V1, &_V2);


	if(tempCross.y < 0.0f)
	{
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// 3D�󿡼� ���� ������ �浹
//-----------------------------------------------------------------------------

bool Collision_Sphere_Line(const D3DXVECTOR3& _SphereCenter, const float& _Raius,
	const D3DXVECTOR3& _LineBegine,  const D3DXVECTOR3& _LineDirection )
{
	D3DXVECTOR3 LineNormalized; //���̸� 1�θ���
	D3DXVECTOR3 toSphereCent = _SphereCenter - _LineBegine; //�������������� ���� �߽ɱ����� �Ÿ�
	float projLength, toSphLength;  // 

	D3DXVec3Normalize(&LineNormalized,&_LineDirection);

	projLength = D3DXVec3Dot(&LineNormalized, &toSphereCent );

	if(projLength <=0) //toSphereCent �� LineNormalized �� ���̰��� 90�� �̻��� 
		return false;

	toSphLength = D3DXVec3Length(&toSphereCent);

	if(toSphLength < _Raius)// ������ �������� ���ȿ��ִٸ� ������ �浹��
		return true;


	if(toSphLength*toSphLength - projLength*projLength  < _Raius*_Raius)
		return true;


	return false;

}


//-------------------------------------------------------------------------
// Arg	  : const D3DXVECTOR3& Sp1_Center , const float& Sp1_Rad  //��1�� ������ ������
//			const D3DXVECTOR3& Sp2_Center , const float& Sp2_Rad  // ��2�� ������ ������
// Return :
// Desc   : 3D���� �� ���� �浹�׽�Ʈ �ϴ� �Լ�   [2014.11.11 ]
//-------------------------------------------------------------------------
bool CollCheck_Sphere_vs_Shpere(const D3DXVECTOR3& Sp1_Center , const float& Sp1_Rad , const D3DXVECTOR3& Sp2_Center , const float& Sp2_Rad  )
{
	float DistCenters; // �� ������ �Ÿ�
	float DistSumRadius = Sp1_Rad + Sp2_Rad ; // �ι������� ��

	D3DXVECTOR3 tempVec = Sp2_Center - Sp1_Center;

	DistCenters = D3DXVec3Length(&tempVec);// �� ������ �Ÿ� [2014.11.11 ]

	if(DistCenters < DistSumRadius)
	{
		return true;
	}

	return false;
}


//-------------------------------------------------------------------------
// Arg	  : const D3DXVECTOR3& Sp1_Center , const float& Sp1_Rad  //��1�� ������ ������
//			const D3DXVECTOR3& Box_Center ,  const D3DXVECTOR3& Box_Max, const D3DXVECTOR3& Box_Min //aabb �ڽ���  ����, �ִ���, �˼���
// Return : bool 
// Desc   : 3D���� ���� aabb�ڽ��� �浹�׽�Ʈ �ϴ� �Լ�   [2014.11.11 ]
//-------------------------------------------------------------------------
bool CollCheck_Sphere_vs_BoxAABB(const D3DXVECTOR3& Sp1_Center , const float& Sp1_Rad ,
								 const D3DXVECTOR3& Box_Center , const D3DXVECTOR3& Box_Max, const D3DXVECTOR3& Box_Min )
{
	D3DXVECTOR3		VectoCenter ; // ���� �������� �ڽ��� �������� ���ϴ� ����
	D3DXVECTOR3		MovedPoint ; // ���� �������� �ڽ������������� ���������̹�ŭ �̵��� ��

	VectoCenter = Box_Center - Sp1_Center;
	D3DXVec3Normalize(&VectoCenter, &VectoCenter);

	MovedPoint = Sp1_Center + (VectoCenter * Sp1_Rad);
	// x��˻�
	if( (MovedPoint.x < Box_Max.x )	&& 	(MovedPoint.x > Box_Min.x ) 	)
	{	// y��˻�
		if( (MovedPoint.y < Box_Max.y )	&& 	(MovedPoint.y > Box_Min.y )  )
		{    // z��˻�
			if( (MovedPoint.z < Box_Max.z )	&& 	(MovedPoint.z > Box_Min.z )  )
			{
				return true;
			}
		}
	}

	return false;
}


