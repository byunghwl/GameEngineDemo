
#pragma once
#include "ASEData.h"

class CLexer;
class CCamera;

//////////////////////////////////////////////////////////////////////////
// CLASS Ase Parser

class ASEParser
{
	wchar_t*					m_path;
	CLexer*						m_pLexer;

public:
	//������.. ��� �����ߴ� �޽� ���鶧 ����� ����
	BONE_LIST					*pTempBoneList;
	int							m_BoneCnt;

public:

	ASEParser();
	virtual ~ASEParser();

public:

	HRESULT						load(wchar_t* path, ASEMesh* _pAseMesh , CCamera* _PCamera = NULL);
	void						Make_Hierarchy(ASEMesh* _pAseMesh);       //Geoobject�� ��������ȭ ��Ű��
	void    					MakeBoneList(ASEMesh* _pAseMesh); // ��Ű�׿� ����� ������Ʈ(��Ʈ�����ȷ�Ʈ) �����
	void    					MakeEmptyKeyFrame(ASEMesh* _pAseMesh);		// 0��Ű�������� ������� ä���־��ش�.
	int     					Cal_GeoObjs_LocalTM(ASEMesh* _pAseMesh);  //GeoObject���� ����Ƽ�� ���ϱ� 

	//���⼭ ���� �Ľ� �Լ�
	//���׸��� �Ľ�	
	bool 						Parsing_Material_List(ASEMesh* _pAseMesh);
	bool 						Parsing_Material(MATERIAL& _Mtrl);
	bool 						Parsing_DiffuseMap(Mtrl_MAP& r_DMap);

	//������Ʈ �Ľ�
	bool 						Parsing_GeoObject(GEOMOBJECT* pGeoObjs);
	bool 						Parsing_ShapeObject(GEOMOBJECT* pGeoObjs);
	bool 						Parsing_HerperObject(GEOMOBJECT* pGeoObjs);
	bool 						Parsing_CameraObject(ASECameraData* _pCData);

	bool 						Parsing_NodeTM(GEOMOBJECT* pGeoObjs);
	bool 						Parsing_NodeTM_Camera(ASECameraData* _pCData);
	bool 						Parsing_Mesh(GEOMOBJECT* pGeoObjs);
	bool 						Parsing_ShapeLine(SHAPE_LINE& _ShapeLine);
	bool 						Parsing_CameraSetting(ASECameraData* _pCData);

	//�ִϸ��̼� ���� �Ľ�
	bool 						Parsing_Scene(ASEMesh* _pAseMesh);
	bool 						Parsing_TMAnimation(GEOMOBJECT* pGeoObjs);
	bool 						Parsing_TMAnimation_Camera(ASECameraData* _pCData); //ī�޶� ����
	
};
