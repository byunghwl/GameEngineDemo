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
// 점이 벡터의 왼쪽에 있는지 판별하는 함수  ★★★2D 용!!!!!!!!!!!!!!!!!!★★★
// _S : 벡터의 시작점
// _V : 벡터
// _p : 임의의 점

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
// 직선과 점의 거리 구하기 ★★★2D 용!!!!!!!!!!!!!!!!!!★★★
//
// 리턴값 : 점과 직선의 최단거리
// _p : 점
// _Norm : ax + bx + c = 0 직선의 방정식에서 (a,b)에 해당하는 값( 2차원이므로 z값은 0.0f) ,, 선분의 법선벡터임
// Spos : 법선 벡터의 시작점

float CalDist_Vert_Line(POSITION _p , VECTOR3 _Norm , POSITION _Spos )
{
	float C; // ax + bx + c = 0 에서 C에 해당하는 값
	float R; // 점과 직선의 최단거리
	float a, b , m, n;

	a= _Norm.x;   b = _Norm.y;   m = _p.x;    n = _p.y;

	C = -1.0f * (a*_Spos.x + b*_Spos.y);

	float temp = a*m + b*n + C;

	if(temp<0) // 절대값 취해주기
		temp*=-1.0f;

	R =  temp/(sqrt(a*a + b*b));


	return R;
}


//////////////////////////////////////////////////////////////////////////
// 벡터가 기준 벡터의 왼쪽에 있는지 판별하는 함수
// _V1: 기준벡터
// _v2: 왼쪽인지오른쪽인지 판별할 벡터 ,2D에서만 사용가능★★★★ 2D 용!!!!!!!!!!!!!!!!!!★★★

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
// 벡터가 기준 벡터의 왼쪽에 있는지 판별하는 함수
// _V1: 기준벡터(x축)
// _v2: 왼쪽인지오른쪽인지 판별할 벡터(z축) ★★★★ 3D 용!!!!!!!!!!!!!!!!!!★★★

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
// 3D상에서 구와 직선의 충돌
//-----------------------------------------------------------------------------

bool Collision_Sphere_Line(const D3DXVECTOR3& _SphereCenter, const float& _Raius,
	const D3DXVECTOR3& _LineBegine,  const D3DXVECTOR3& _LineDirection )
{
	D3DXVECTOR3 LineNormalized; //길이를 1로만듬
	D3DXVECTOR3 toSphereCent = _SphereCenter - _LineBegine; //직선시작점에서 구의 중심까지의 거리
	float projLength, toSphLength;  // 

	D3DXVec3Normalize(&LineNormalized,&_LineDirection);

	projLength = D3DXVec3Dot(&LineNormalized, &toSphereCent );

	if(projLength <=0) //toSphereCent 와 LineNormalized 의 사이각이 90도 이상임 
		return false;

	toSphLength = D3DXVec3Length(&toSphereCent);

	if(toSphLength < _Raius)// 직선의 시작점이 구안에있다면 무조건 충돌됨
		return true;


	if(toSphLength*toSphLength - projLength*projLength  < _Raius*_Raius)
		return true;


	return false;

}


//-------------------------------------------------------------------------
// Arg	  : const D3DXVECTOR3& Sp1_Center , const float& Sp1_Rad  //원1의 중점과 반지름
//			const D3DXVECTOR3& Sp2_Center , const float& Sp2_Rad  // 원2의 중점과 반지름
// Return :
// Desc   : 3D상의 두 구를 충돌테스트 하는 함수   [2014.11.11 ]
//-------------------------------------------------------------------------
bool CollCheck_Sphere_vs_Shpere(const D3DXVECTOR3& Sp1_Center , const float& Sp1_Rad , const D3DXVECTOR3& Sp2_Center , const float& Sp2_Rad  )
{
	float DistCenters; // 두 원점의 거리
	float DistSumRadius = Sp1_Rad + Sp2_Rad ; // 두반지름의 합

	D3DXVECTOR3 tempVec = Sp2_Center - Sp1_Center;

	DistCenters = D3DXVec3Length(&tempVec);// 두 원점의 거리 [2014.11.11 ]

	if(DistCenters < DistSumRadius)
	{
		return true;
	}

	return false;
}


//-------------------------------------------------------------------------
// Arg	  : const D3DXVECTOR3& Sp1_Center , const float& Sp1_Rad  //원1의 중점과 반지름
//			const D3DXVECTOR3& Box_Center ,  const D3DXVECTOR3& Box_Max, const D3DXVECTOR3& Box_Min //aabb 박스의  중점, 최대점, 죄소점
// Return : bool 
// Desc   : 3D상의 구와 aabb박스를 충돌테스트 하는 함수   [2014.11.11 ]
//-------------------------------------------------------------------------
bool CollCheck_Sphere_vs_BoxAABB(const D3DXVECTOR3& Sp1_Center , const float& Sp1_Rad ,
								 const D3DXVECTOR3& Box_Center , const D3DXVECTOR3& Box_Max, const D3DXVECTOR3& Box_Min )
{
	D3DXVECTOR3		VectoCenter ; // 원의 원점에서 박스의 원점으로 향하는 벡터
	D3DXVECTOR3		MovedPoint ; // 원의 원점에서 박스중점방향으로 반지름길이반큼 이동한 점

	VectoCenter = Box_Center - Sp1_Center;
	D3DXVec3Normalize(&VectoCenter, &VectoCenter);

	MovedPoint = Sp1_Center + (VectoCenter * Sp1_Rad);
	// x축검사
	if( (MovedPoint.x < Box_Max.x )	&& 	(MovedPoint.x > Box_Min.x ) 	)
	{	// y축검사
		if( (MovedPoint.y < Box_Max.y )	&& 	(MovedPoint.y > Box_Min.y )  )
		{    // z축검사
			if( (MovedPoint.z < Box_Max.z )	&& 	(MovedPoint.z > Box_Min.z )  )
			{
				return true;
			}
		}
	}

	return false;
}


