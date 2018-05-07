// 2014. 06. 23

#pragma once

#include "Lexer.h"
#include "ASEData.h"


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//  ASEMesh   클래스 선언

class ASEMesh  : public ASEData
{
	friend class ASEParser;
	friend class GEOMOBJECT;
	friend class SKINNED_OBJECT;

public:
	//  외부에서 빌려온 버텍스 쉐이더 (해제도 외부에서) 
	LPDIRECT3DVERTEXSHADER9				m_pVtxShader;  
	//  외부에서 빌려온 버텍스 쉐이더 (해제도 외부에서)
	LPDIRECT3DPIXELSHADER9				m_pPixShader;  
	//충돌구
	BoundingSphere						m_BSphere;    // 충돌구
	//GEOMOBJECT 를 담아놓을 메모리풀
	CGrowableArray<GEOMOBJECT*>			m_GA_GeoObject; // 본
	// 스킨닝 되는 GeoObject
	SKINNED_OBJECT*						m_SkinnedMesh; // 바디
	// 메테리얼 담아놓을 메모리풀
	CGrowableArray<MATERIAL*>			m_GA_Mtrl;     
	// 본리스트
	CGrowableArray<GEOMOBJECT*>			m_GA_BonesList;  //본 인덱스 리스트 배열 (메트릭스팔레트)

public:
		
		ASEMesh();
		virtual ~ASEMesh();
		virtual 	void				Update(float _dTime,  D3DXMATRIX MoveTM);

public:
		bool							createMesh(LPDIRECT3DDEVICE9 _pDevice);		
		void							Draw(IDirect3DDevice9* device);		
		void							Release();
		GEOMOBJECT*						Piking(const D3DXVECTOR3& _vPickRayOrig, const D3DXVECTOR3& _vPickRayDir );

public://get,, set

	const CGrowableArray<GEOMOBJECT*>*   Get_GA_BoneList()	const {return &m_GA_BonesList;}	 
	const CGrowableArray<GEOMOBJECT*>*   Get_GA_GeoObjList()const {return &m_GA_GeoObject;}	
	const HELPEROBJECT*					 Get_HelperObject(CHAR* _Name) const; //CHAR* _Name으로 헬퍼오브젝트를 찾는다.
	 
	void								 Set_VtxShader(const LPDIRECT3DVERTEXSHADER9 _pShder){m_pVtxShader = _pShder;}
	void								 Set_PixShader(const LPDIRECT3DPIXELSHADER9 _pShder){m_pPixShader = _pShder;} 




};


//*******************************************  끝  **********************************************//