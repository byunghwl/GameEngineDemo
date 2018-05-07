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
//  ASEMesh   Ŭ���� ����

class ASEMesh  : public ASEData
{
	friend class ASEParser;
	friend class GEOMOBJECT;
	friend class SKINNED_OBJECT;

public:
	//  �ܺο��� ������ ���ؽ� ���̴� (������ �ܺο���) 
	LPDIRECT3DVERTEXSHADER9				m_pVtxShader;  
	//  �ܺο��� ������ ���ؽ� ���̴� (������ �ܺο���)
	LPDIRECT3DPIXELSHADER9				m_pPixShader;  
	//�浹��
	BoundingSphere						m_BSphere;    // �浹��
	//GEOMOBJECT �� ��Ƴ��� �޸�Ǯ
	CGrowableArray<GEOMOBJECT*>			m_GA_GeoObject; // ��
	// ��Ų�� �Ǵ� GeoObject
	SKINNED_OBJECT*						m_SkinnedMesh; // �ٵ�
	// ���׸��� ��Ƴ��� �޸�Ǯ
	CGrowableArray<MATERIAL*>			m_GA_Mtrl;     
	// ������Ʈ
	CGrowableArray<GEOMOBJECT*>			m_GA_BonesList;  //�� �ε��� ����Ʈ �迭 (��Ʈ�����ȷ�Ʈ)

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
	const HELPEROBJECT*					 Get_HelperObject(CHAR* _Name) const; //CHAR* _Name���� ���ۿ�����Ʈ�� ã�´�.
	 
	void								 Set_VtxShader(const LPDIRECT3DVERTEXSHADER9 _pShder){m_pVtxShader = _pShder;}
	void								 Set_PixShader(const LPDIRECT3DPIXELSHADER9 _pShder){m_pPixShader = _pShder;} 




};


//*******************************************  ��  **********************************************//