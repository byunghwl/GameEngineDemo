//*****************************************************************************
// 2014. 06. 23
//*****************************************************************************


#include "DXUT.H"

#include "ASELexer.h"
#include "ASEMesh.h"
#include "Camera.h"

#define FRAME_PER_SAMPLE 5
#define SUB_MATERIAL_MAX 512


//---------------------------------------------------------------------------------------------
// 전역변수 또는 전역 함수
//---------------------------------------------------------------------------------------------



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
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
// 여기서부터 ASEMesh 클래스 함수 정의
//
/////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// 생성자

ASEMesh::ASEMesh()  : m_SkinnedMesh(NULL), m_pVtxShader(nullptr), m_pPixShader(nullptr)

{

}


//////////////////////////////////////////////////////////////////////////
// 소멸자

ASEMesh::~ASEMesh()
{
	Release();

}


//---------------------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------------------
void  ASEMesh::Update(float _dTime,	 D3DXMATRIX MoveTM)
{
	for(int i=0; i<m_GA_GeoObject.GetSize() ; i++)
	{
		m_GA_GeoObject.GetAt(i)->Update(_dTime, MoveTM);
	}

	if(m_SkinnedMesh !=NULL) //skinned 메쉬가 있다면??
		m_SkinnedMesh->Update(_dTime, MoveTM); //따로 업데이트 해준다.

}



//////////////////////////////////////////////////////////////////////////
//
void ASEMesh::Draw(IDirect3DDevice9* _device)
{

	for(int i=0; i<m_GA_GeoObject.GetSize() ; i++)
	{
		m_GA_GeoObject.GetAt(i)->Draw(_device, m_pVtxShader, m_pPixShader);
	}

	if(m_SkinnedMesh !=NULL) //skinned 메쉬가 있다면??
	{
		SKINNED_OBJECT *pSO = dynamic_cast<SKINNED_OBJECT*>(m_SkinnedMesh);
		if(pSO == nullptr)
		{
			MessageBox(NULL,TEXT("젠장!!!!!!!!!!!!!!. "), TEXT("Failed"), MB_OK);  assert(false);
		}
		m_SkinnedMesh->SKINNED_OBJECT::Draw(_device, m_pVtxShader, m_pPixShader);
	}

}



//////////////////////////////////////////////////////////////////////////
// 버텍스 버퍼와 인덱스 버퍼 만들어주기 

bool ASEMesh::createMesh(LPDIRECT3DDEVICE9 _pDevice)
{
	
	for(int i=0; i<m_GA_GeoObject.GetSize(); i++)
	{
		MATERIAL* m = nullptr;

		GEOMOBJECT* gObjs = m_GA_GeoObject.GetAt(i);
		if(gObjs->Mtrl_Ref == -1)
		{ 	write_log_file("메테리얼이 없습니다.");		}
		else
		{		m = m_GA_Mtrl.GetAt(gObjs->Mtrl_Ref);	}//해당하는 메테리얼을 가져온다.
	
		gObjs->Create_FaceSet(m, _pDevice);
	}
	
	if(m_SkinnedMesh != NULL)
	{	
		MATERIAL* m = nullptr;
		if(m_SkinnedMesh->Mtrl_Ref == -1)
		{ 	write_log_file("메테리얼이 없습니다.");		}
		else
		{		m = m_GA_Mtrl.GetAt(m_SkinnedMesh->Mtrl_Ref);	}//해당하는 메테리얼을 가져온다.

		m_SkinnedMesh->Create_FaceSet(m, _pDevice);
	}
	

	return true;
}

//////////////////////////////////////////////////////////////////////////
// 내부데이터 메모리 해제

void ASEMesh::Release()
{
	write_log_file("메모리 해제 : ASEMesh::Release()");
	//CGrowableArray 안에 담은 객체들을 메모리 해제한다.
	for(int i=0 ; i<m_GA_Mtrl.GetSize() ; i++)	   {	MATERIAL*& m = m_GA_Mtrl.GetAt(i);        SAFE_DELETE_(m)  	}
	for(int i=0 ; i<m_GA_GeoObject.GetSize() ; i++){	GEOMOBJECT*& m= m_GA_GeoObject.GetAt(i);  SAFE_DELETE_(m)    }
	//m_GA_BonesList 는 단순한 참조이므로 메모리 해제할필요없다.
	/*for(int i=0 ; i<m_GA_BonesList.GetSize() ; i++){	GEOMOBJECT*& m= m_GA_BonesList.GetAt(i);  SAFE_DELETE_(m)    }*/

	SAFE_DELETE_(m_SkinnedMesh)
	//로그쓰기
	
}

//-----------------------------------------------------------------------------
// 메쉬내부의 GeoObject들을 전부 픽킹해서 그 포인터를 반환하는 함수
//-----------------------------------------------------------------------------

GEOMOBJECT*  ASEMesh::Piking(const D3DXVECTOR3& _vPickRayOrig, const D3DXVECTOR3& _vPickRayDir )
{
	//함수내에서 사용될 변수선언
	GEOMOBJECT*		pRetGeoObj = nullptr;//반환할 GeoObject
	D3DXVECTOR3		CameraPos = SHGlobal_GetCurScene_Camera->GetPos(); //카메라의 현재위치 얻어오기
	
	D3DXVECTOR3		tempvec ; //카메라와의 거리를 구하기 위해 잠시저장해놓은
	float			pRetLength, CurBoneLength;
	// 여기서부터 캐릭터내의 경계구들에 대한 픽킹검사
		
		GEOMOBJECT* pBone;
		for(int i = 0 ; i<m_GA_GeoObject.GetSize(); i++  )
		{
			//현재 본의 포인터를 얻어온다.
			pBone = m_GA_GeoObject.GetAt(i);
			
			const BoundingSphere* pBS = &pBone->m_BoundSphere;
			//충돌 했다면 
			if(Collision_Sphere_Line(pBS->get_Pos(), pBS->get_Radius(), _vPickRayOrig, _vPickRayDir ) == true)
			{
				if(pRetGeoObj == nullptr )
				{	pRetGeoObj = pBone;				}
				else
				{
					tempvec = pRetGeoObj->m_BoundSphere.get_Pos() - CameraPos;
					pRetLength = D3DXVec3Length(&tempvec);
					tempvec = pBone->m_BoundSphere.get_Pos() - CameraPos;
					CurBoneLength = D3DXVec3Length(&tempvec);

					if(CurBoneLength < pRetLength)
					{
						pRetGeoObj = pBone;
					}					
				}

			}
		}


	return pRetGeoObj;
}



//-----------------------------------------------------------------------------
// _Name으로 핼퍼오브젝트를 찾아서 그포인터를 반환해주는 함수  [2014.7.26 ]
// 못찾았을경우 nullptr을 리턴한다.
//-----------------------------------------------------------------------------
const HELPEROBJECT*		ASEMesh::Get_HelperObject(CHAR* _Name) const
{
	GEOMOBJECT* pG= nullptr;
	HELPEROBJECT* ph = nullptr;

	for(int i = 0 ; i <m_GA_GeoObject.GetSize(); ++i)
	{
		pG	= m_GA_GeoObject.GetAt(i);
		if(StrCmpA(_Name, pG->NodeTM.Name ) == 0)
		{
			ph = dynamic_cast<HELPEROBJECT*>(pG);
			ASSERT_IF_FAIL_D_CAST(ph)
			return ph;
		}
	}
	return ph;
}






/************************************************ End *****************************************************/