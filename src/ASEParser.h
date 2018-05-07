
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
	//데이터.. 잠시 저장했다 메쉬 만들때 사라질 정보
	BONE_LIST					*pTempBoneList;
	int							m_BoneCnt;

public:

	ASEParser();
	virtual ~ASEParser();

public:

	HRESULT						load(wchar_t* path, ASEMesh* _pAseMesh , CCamera* _PCamera = NULL);
	void						Make_Hierarchy(ASEMesh* _pAseMesh);       //Geoobject를 계층구조화 시키기
	void    					MakeBoneList(ASEMesh* _pAseMesh); // 스키닝에 적용될 본리스트(메트리스팔레트) 만들기
	void    					MakeEmptyKeyFrame(ASEMesh* _pAseMesh);		// 0번키프레임이 없을경우 채워넣어준다.
	int     					Cal_GeoObjs_LocalTM(ASEMesh* _pAseMesh);  //GeoObject들의 로컬티엠 구하기 

	//여기서 부터 파싱 함수
	//메테리얼 파싱	
	bool 						Parsing_Material_List(ASEMesh* _pAseMesh);
	bool 						Parsing_Material(MATERIAL& _Mtrl);
	bool 						Parsing_DiffuseMap(Mtrl_MAP& r_DMap);

	//오브젝트 파싱
	bool 						Parsing_GeoObject(GEOMOBJECT* pGeoObjs);
	bool 						Parsing_ShapeObject(GEOMOBJECT* pGeoObjs);
	bool 						Parsing_HerperObject(GEOMOBJECT* pGeoObjs);
	bool 						Parsing_CameraObject(ASECameraData* _pCData);

	bool 						Parsing_NodeTM(GEOMOBJECT* pGeoObjs);
	bool 						Parsing_NodeTM_Camera(ASECameraData* _pCData);
	bool 						Parsing_Mesh(GEOMOBJECT* pGeoObjs);
	bool 						Parsing_ShapeLine(SHAPE_LINE& _ShapeLine);
	bool 						Parsing_CameraSetting(ASECameraData* _pCData);

	//애니메이션 정보 파싱
	bool 						Parsing_Scene(ASEMesh* _pAseMesh);
	bool 						Parsing_TMAnimation(GEOMOBJECT* pGeoObjs);
	bool 						Parsing_TMAnimation_Camera(ASECameraData* _pCData); //카메라 전용
	
};
