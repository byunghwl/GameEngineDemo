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
// �������� �Ǵ� ���� �Լ�
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
// ���⼭���� ASEMesh Ŭ���� �Լ� ����
//
/////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// ������

ASEMesh::ASEMesh()  : m_SkinnedMesh(NULL), m_pVtxShader(nullptr), m_pPixShader(nullptr)

{

}


//////////////////////////////////////////////////////////////////////////
// �Ҹ���

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

	if(m_SkinnedMesh !=NULL) //skinned �޽��� �ִٸ�??
		m_SkinnedMesh->Update(_dTime, MoveTM); //���� ������Ʈ ���ش�.

}



//////////////////////////////////////////////////////////////////////////
//
void ASEMesh::Draw(IDirect3DDevice9* _device)
{

	for(int i=0; i<m_GA_GeoObject.GetSize() ; i++)
	{
		m_GA_GeoObject.GetAt(i)->Draw(_device, m_pVtxShader, m_pPixShader);
	}

	if(m_SkinnedMesh !=NULL) //skinned �޽��� �ִٸ�??
	{
		SKINNED_OBJECT *pSO = dynamic_cast<SKINNED_OBJECT*>(m_SkinnedMesh);
		if(pSO == nullptr)
		{
			MessageBox(NULL,TEXT("����!!!!!!!!!!!!!!. "), TEXT("Failed"), MB_OK);  assert(false);
		}
		m_SkinnedMesh->SKINNED_OBJECT::Draw(_device, m_pVtxShader, m_pPixShader);
	}

}



//////////////////////////////////////////////////////////////////////////
// ���ؽ� ���ۿ� �ε��� ���� ������ֱ� 

bool ASEMesh::createMesh(LPDIRECT3DDEVICE9 _pDevice)
{
	
	for(int i=0; i<m_GA_GeoObject.GetSize(); i++)
	{
		MATERIAL* m = nullptr;

		GEOMOBJECT* gObjs = m_GA_GeoObject.GetAt(i);
		if(gObjs->Mtrl_Ref == -1)
		{ 	write_log_file("���׸����� �����ϴ�.");		}
		else
		{		m = m_GA_Mtrl.GetAt(gObjs->Mtrl_Ref);	}//�ش��ϴ� ���׸����� �����´�.
	
		gObjs->Create_FaceSet(m, _pDevice);
	}
	
	if(m_SkinnedMesh != NULL)
	{	
		MATERIAL* m = nullptr;
		if(m_SkinnedMesh->Mtrl_Ref == -1)
		{ 	write_log_file("���׸����� �����ϴ�.");		}
		else
		{		m = m_GA_Mtrl.GetAt(m_SkinnedMesh->Mtrl_Ref);	}//�ش��ϴ� ���׸����� �����´�.

		m_SkinnedMesh->Create_FaceSet(m, _pDevice);
	}
	

	return true;
}

//////////////////////////////////////////////////////////////////////////
// ���ε����� �޸� ����

void ASEMesh::Release()
{
	write_log_file("�޸� ���� : ASEMesh::Release()");
	//CGrowableArray �ȿ� ���� ��ü���� �޸� �����Ѵ�.
	for(int i=0 ; i<m_GA_Mtrl.GetSize() ; i++)	   {	MATERIAL*& m = m_GA_Mtrl.GetAt(i);        SAFE_DELETE_(m)  	}
	for(int i=0 ; i<m_GA_GeoObject.GetSize() ; i++){	GEOMOBJECT*& m= m_GA_GeoObject.GetAt(i);  SAFE_DELETE_(m)    }
	//m_GA_BonesList �� �ܼ��� �����̹Ƿ� �޸� �������ʿ����.
	/*for(int i=0 ; i<m_GA_BonesList.GetSize() ; i++){	GEOMOBJECT*& m= m_GA_BonesList.GetAt(i);  SAFE_DELETE_(m)    }*/

	SAFE_DELETE_(m_SkinnedMesh)
	//�α׾���
	
}

//-----------------------------------------------------------------------------
// �޽������� GeoObject���� ���� ��ŷ�ؼ� �� �����͸� ��ȯ�ϴ� �Լ�
//-----------------------------------------------------------------------------

GEOMOBJECT*  ASEMesh::Piking(const D3DXVECTOR3& _vPickRayOrig, const D3DXVECTOR3& _vPickRayDir )
{
	//�Լ������� ���� ��������
	GEOMOBJECT*		pRetGeoObj = nullptr;//��ȯ�� GeoObject
	D3DXVECTOR3		CameraPos = SHGlobal_GetCurScene_Camera->GetPos(); //ī�޶��� ������ġ ������
	
	D3DXVECTOR3		tempvec ; //ī�޶���� �Ÿ��� ���ϱ� ���� ��������س���
	float			pRetLength, CurBoneLength;
	// ���⼭���� ĳ���ͳ��� ��豸�鿡 ���� ��ŷ�˻�
		
		GEOMOBJECT* pBone;
		for(int i = 0 ; i<m_GA_GeoObject.GetSize(); i++  )
		{
			//���� ���� �����͸� ���´�.
			pBone = m_GA_GeoObject.GetAt(i);
			
			const BoundingSphere* pBS = &pBone->m_BoundSphere;
			//�浹 �ߴٸ� 
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
// _Name���� ���ۿ�����Ʈ�� ã�Ƽ� �������͸� ��ȯ���ִ� �Լ�  [2014.7.26 ]
// ��ã������� nullptr�� �����Ѵ�.
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