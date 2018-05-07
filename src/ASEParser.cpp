//*****************************************************************************
// 2014. 06. 23
// ASE 파서 클래스 
//*****************************************************************************

#pragma once

#include "DXUT.H"
#include "Camera.h"
#include "ASEData.h"
#include "ASELexer.h"
#include "ASEParser.h"

D3DXVECTOR3 g_ZeroVec3 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//일반함수
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------
// 포인터 배열을 생성한 후 복사해준다.
//-------------------------------------------------------------
void pstrcpy(wchar_t** s, wchar_t* d) {
	int len = (int)wcslen(d);
	if (len) { 
		(*s) = new wchar_t[sizeof(wchar_t) * len];
				wcscpy_s((*s),sizeof(wchar_t) * len ,d);
	}
}

//-------------------------------------------------------------
// 앞뒤 따옴표를 없애준다.
//-------------------------------------------------------------
void removeQmarks(wchar_t *str)
{
	size_t size   = wcslen(str);
	wchar_t* temp = str;
	// "앞" 따옴표 삭제 (앞에 없고 마지막에 있을경우를 위해 마지막 번째는 검사를 안한다.)
	while (size-1 > 0) {
		if ((*temp)==L'"') {
			wcscpy_s(str, sizeof(wchar_t) *size ,temp+(wcslen(str)-size));
			break; 
		}
		temp++;
		size--;
	}
	// "뒤" 따옴표 삭제 (문자열의 마지막 번째만 검사)
	size = wcslen(str);
	if (str[size-1]==L'"')
		str[size-1]=L'\0';
}




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
// 여기서부터 파서함수 정의 시작
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

//-----------------------------------------------------------------------------
// 파서 생성자, 소멸자
//-----------------------------------------------------------------------------
ASEParser::ASEParser()
	:m_pLexer(NULL) , pTempBoneList(NULL)
{

}

ASEParser::~ASEParser()
{
	if(m_pLexer !=NULL) delete m_pLexer;
	SAFE_DELETE_ARR(pTempBoneList)
}

//-----------------------------------------------------------------------------
// 로드함수, ase 파일경로와 aseData를 인자로 받는다.
//-----------------------------------------------------------------------------

HRESULT ASEParser::load(wchar_t* path, ASEMesh* _pAseMesh, CCamera* _PCamera/* = NULL*/)
{	
	m_path = path;
	m_pLexer = new CLexer(path);
	GEOMOBJECT* gObjs = NULL;

	CHAR	TokenString[256];
	//WCHAR   WTokenString[256];

	LONG	token = m_pLexer->GetToken(TokenString);
	while(token != TOKEND_END)
	{
		switch(token)
		{
		
			case	TOKERN_SCENE:		//메테리얼 리스트 파싱	
					{	
						Parsing_Scene(_pAseMesh);
					}
			break;

			case	TOKERN_MATERIAL_LIST:		//메테리얼 리스트 파싱	
					{	
						Parsing_Material_List(_pAseMesh);
					}
				break;

			case	TOKERN_GEOMOBJECT:			
					{	
						gObjs=NULL;
						gObjs = new GEOMOBJECT(); //aseData의 m_pGeoObjs를 메모리 할당해준다.
						ASSERT_IF_FAIL_MemAlloc(gObjs)  //메모리 할당실패할 경우 어썰트

						gObjs->m_TMAni.FrameCnt  =  _pAseMesh->m_Scene.LastFrame  -  _pAseMesh->m_Scene.FirstFrame;
						gObjs->pAseData  =  _pAseMesh;
						Parsing_GeoObject(gObjs); //정보를 읽어들여 저장한 후..

						_pAseMesh->m_GA_GeoObject.Add(gObjs); //GrowableArray에 담아준다.					
					}
				break;

			case    TOKERN_HELPEROBJECT:
					{
						gObjs=NULL;
						gObjs = new HELPEROBJECT(); //aseData의 m_pGeoObjs를 메모리 할당해준다.
						ASSERT_IF_FAIL_MemAlloc(gObjs)  //메모리 할당실패할 경우 어썰트

						gObjs->m_TMAni.FrameCnt = _pAseMesh->m_Scene.LastFrame - _pAseMesh->m_Scene.FirstFrame;
						gObjs->pAseData = _pAseMesh;
						Parsing_HerperObject(gObjs);

						_pAseMesh->m_GA_GeoObject.Add(gObjs); //GrowableArray에 담아준다.	
					}
				break;

			case    TOKERN_SHAPEOBJECT:
				{
					gObjs=NULL;
					gObjs = new SHAPEOBJECT(); //aseData의 m_pGeoObjs를 메모리 할당해준다.
					ASSERT_IF_FAIL_MemAlloc(gObjs)  //메모리 할당실패할 경우 어썰트
											
					gObjs->m_TMAni.FrameCnt = _pAseMesh->m_Scene.LastFrame - _pAseMesh->m_Scene.FirstFrame;
					gObjs->pAseData = _pAseMesh;
					Parsing_ShapeObject(gObjs);

					_pAseMesh->m_GA_GeoObject.Add(gObjs); //GrowableArray에 담아준다.	
				}
				break;

			case TOKERN_CAMERAOBJECT:
				{
					if(_PCamera !=NULL )
					{	
						_PCamera->GetAseCData()->m_Scene = _pAseMesh->m_Scene;
						Parsing_CameraObject(_PCamera->GetAseCData()); //카메라 정보 읽기
					}
					
				}
				break;

		};

		token = m_pLexer->GetToken(TokenString);
	}

	//계층구조화
	Make_Hierarchy(_pAseMesh);
	Cal_GeoObjs_LocalTM(_pAseMesh);//로컬TM 구하기
	MakeEmptyKeyFrame(_pAseMesh);//  0번키프레임이 없을경우 채워넣어준다.

	if(pTempBoneList != NULL)
	{	
		MakeBoneList(_pAseMesh); //메트릭스팔레트를 만들어줄때 사용할 본리스트 셋팅
	}

	return	0;
}

//-----------------------------------------------------------------------------
// 메트릭스팔레트를 만들어줄때 사용할 본리스트 셋팅
//-----------------------------------------------------------------------------

void ASEParser::MakeBoneList(ASEMesh* _pAseMesh)
{	
	int GeoObjCnt = _pAseMesh->m_GA_GeoObject.GetSize();

	if(m_BoneCnt > 0)
	{
		for(int i=0; i< m_BoneCnt ; i++ )
		{
			for(int j = 0; j <  GeoObjCnt ; j++)
			{
				GEOMOBJECT* pCurObj = _pAseMesh->m_GA_GeoObject.GetAt(j);

				if( strcmp(pTempBoneList[i].Name, pCurObj->NodeTM.Name ) == 0)
				{	
					_pAseMesh->m_GA_BonesList.Add(pCurObj);
					LOG_WRITE_A("  [BoneList %d번] -> %s" , i , pCurObj->NodeTM.Name);
				}		
			
			}			
		}		
		
		for(int j = 0; j <  GeoObjCnt ; j++)			
		{	
			GEOMOBJECT* pCurObj = _pAseMesh->m_GA_GeoObject.GetAt(j);
			if(pCurObj->m_isSkinned == true)
			{
				_pAseMesh->m_SkinnedMesh = new SKINNED_OBJECT();
				ASSERT_IF_FAIL_MemAlloc(_pAseMesh->m_SkinnedMesh )
					
				_pAseMesh->m_SkinnedMesh->DeepCopy(*pCurObj);  //스키닝정보가 있는 GeoObject만 따로 빼준다. 깊은복사
				SAFE_DELETE_(pCurObj)
				_pAseMesh->m_GA_GeoObject.Remove(j);  

				_pAseMesh->m_SkinnedMesh->m_pMatPalette  = new D3DXMATRIX[m_BoneCnt];
				ASSERT_IF_FAIL_MemAlloc(_pAseMesh->m_SkinnedMesh->m_pMatPalette  )				

				break;
			}
		}
		
		D3DXMATRIX SkinMeshInv;
		D3DXMatrixInverse(&SkinMeshInv, NULL, &_pAseMesh->m_SkinnedMesh->NodeTM.NodeTM);

		for(int i=0; i<_pAseMesh->m_GA_GeoObject.GetSize() ; i++)
		{	//메트릭스팔레트
			GEOMOBJECT* pCurObj = _pAseMesh->m_GA_GeoObject.GetAt(i);
			pCurObj->m_BoneIniMat = pCurObj->NodeTM.NodeTM * SkinMeshInv;// 본이니셜Mat = 본의 NodeTM * skinMesh의 NodeTM의 역행렬
			D3DXMatrixInverse(&pCurObj->m_BoneIniMat , NULL, &pCurObj->m_BoneIniMat);

		}
		m_BoneCnt = 0;

		SAFE_DELETE_ARR(pTempBoneList)
	}
	
}

//---------------------------------------------------------------------------------------------
//   Geoobject를 계층구조화 시키기
//---------------------------------------------------------------------------------------------

void ASEParser::Make_Hierarchy(ASEMesh* _pAseMesh)
{
	
	int logidx=0;

/*	char Cname[256] , Pname[256];*/
	GEOMOBJECT *pChild, *pParent;
	//예외처리
	if(_pAseMesh->m_GA_GeoObject.GetSize() == 0 )
	{
		MessageBox(NULL,TEXT("geoObject가 하나도 없습니다."), TEXT("Failed"), MB_OK);   assert(false);
	}
	

	for(int i= 0; i <_pAseMesh->m_GA_GeoObject.GetSize()  ; i++)
	{	
		pChild=_pAseMesh->m_GA_GeoObject.GetAt(i);
		TMAnimation &_TMANI = pChild->m_TMAni;

		CHAR* parentName = pChild->NodeTM.ParentName;
		
		if(strlen(parentName) == 0) //부모가 없다면
		{	
			LOG_WRITE_A( "   %d번GeoObject : %s , 부모노드이름 : %s ",i,pChild->NodeTM.Name, parentName);
			pChild->m_pParent = NULL;	

			continue;
		}

		for(int j=0 ; j<_pAseMesh->m_GA_GeoObject.GetSize() ; j++)
		{
			pParent = _pAseMesh->m_GA_GeoObject.GetAt(j);
			CHAR* Name = pParent->NodeTM.Name;

			if(strlen(Name) == 0)
			{
				write_log_file("ASEParser::Make_Hierarchy(ASEMesh* _pAseMesh) , Name 이 NULL입니다.");
				assert(false);			
			}

			if( strcmp(Name, pChild->NodeTM.ParentName) == 0 )//부모를 찾았다면!!
			{					
				LOG_WRITE_A( "   %d번GeoObject : %s , 부모노드이름 : %s ",i,pChild->NodeTM.Name, parentName);
				pChild->m_pParent =  pParent;
				break;
			}
		}
	}

	


}

//-----------------------------------------------------------------------------
// 각 GeoObject들의 로컬TM구하기,, 
// 부모노드정보가 필요하기 때문에 부모노드를 구한후 이 함수를 실행해야 한다.
//-----------------------------------------------------------------------------

int ASEParser::Cal_GeoObjs_LocalTM(ASEMesh* _pAseMesh)
{

	GEOMOBJECT *pChild, *pParent;

	for(int i= 0; i <_pAseMesh->m_GA_GeoObject.GetSize()  ; i++)
	{
		pChild=_pAseMesh->m_GA_GeoObject.GetAt(i);
		TMAnimation &_TMANI = pChild->m_TMAni;
		CHAR* parentName = pChild->NodeTM.ParentName;

		if(strlen(parentName) == 0) //부모가 없다면
		{
			//로컬TM 구하기
			pChild->m_MLocalTM = pChild->NodeTM.NodeTM;			
			//여기서부터 Decompose

			D3DXQUATERNION QRot;
			D3DXVECTOR3 scaleVec, transVec;
			D3DXMatrixDecompose(&scaleVec, &QRot,&transVec, &pChild->m_MLocalTM );

			D3DXMatrixTranslation(&pChild->m_LTM_Pos,transVec.x, transVec.y, transVec.z);
			D3DXMatrixRotationQuaternion(&pChild->m_LTM_Rot,&QRot);
			D3DXMatrixScaling(&pChild->m_LTM_scale, scaleVec.x, scaleVec.y, scaleVec.z );

			LOG_WRITE_A( "노드이름 %s , m_MLocalTM:11   <%f,  %f,  %f> ", pChild->NodeTM.Name, pChild->m_MLocalTM._11, pChild->m_MLocalTM._12, pChild->m_MLocalTM._13 );
			LOG_WRITE_A( "노드이름 %s , m_MLocalTM:21   <%f,  %f,  %f> ", pChild->NodeTM.Name, pChild->m_MLocalTM._21,  pChild->m_MLocalTM._22,  pChild->m_MLocalTM._23);
			LOG_WRITE_A( "노드이름 %s , m_MLocalTM:31   <%f,  %f,  %f> ", pChild->NodeTM.Name, pChild->m_MLocalTM._31,  pChild->m_MLocalTM._32,  pChild->m_MLocalTM._33);
			LOG_WRITE_A( "노드이름 %s , m_MLocalTM:41   <%f,  %f,  %f> ", pChild->NodeTM.Name, pChild->m_MLocalTM._41,  pChild->m_MLocalTM._42,  pChild->m_MLocalTM._43);
			
			continue;
		}
		//이중포문
		for(int j=0 ; j<_pAseMesh->m_GA_GeoObject.GetSize() ; j++)
		{
			pParent = _pAseMesh->m_GA_GeoObject.GetAt(j);
			CHAR* Name = pParent->NodeTM.Name;

			if(strlen(Name) == 0)
			{
				write_log_file("ASEParser::Make_Hierarchy(ASEMesh* _pAseMesh) , Name 이 NULL입니다.");
				assert(false);			
			}

			if( strcmp(Name, pChild->NodeTM.ParentName) == 0 )//부모를 찾았다면!!
			{					
				D3DXMATRIX InvP;//부모의 역행렬이 저장될 변수
				D3DXMatrixInverse(&InvP,NULL, &pChild->m_pParent->NodeTM.NodeTM); //부모노드의 노드TM의 역행렬을 구한다.
				pChild->m_MLocalTM = pChild->NodeTM.NodeTM * InvP; //자식노드 * 부모노드의 역행렬 = 자식의 로컬TM

				//여기서부터 Decompose
				D3DXQUATERNION QRot;
				D3DXVECTOR3 scaleVec, transVec;
				D3DXMatrixDecompose(&scaleVec, &QRot,&transVec, &pChild->m_MLocalTM );//로컬TM을 decompose한다.
				//decompose한 성분을 각각 해당하는 로컬행렬로 만들어준다.
				D3DXMatrixTranslation(&pChild->m_LTM_Pos,transVec.x, transVec.y, transVec.z);
				D3DXMatrixRotationQuaternion(&pChild->m_LTM_Rot,&QRot);
				D3DXMatrixScaling(&pChild->m_LTM_scale, scaleVec.x, scaleVec.y, scaleVec.z );
							
				break;
			}
		}
	}

	return 0;
}
//---------------------------------------------------------------------------------------------
//  0번키프레임이 없을경우 채워넣어준다.
//---------------------------------------------------------------------------------------------

void ASEParser::MakeEmptyKeyFrame(ASEMesh* _pAseMesh)
{
	GEOMOBJECT *pCurGeoObject;
	for(int i= 0; i <_pAseMesh->m_GA_GeoObject.GetSize()  ; i++)
	{
		pCurGeoObject=_pAseMesh->m_GA_GeoObject.GetAt(i);
		// 첫번째 키값이 0이 아닐때 0값을 만들어준다.
		// 1) Pos
		if( (pCurGeoObject->m_TMAni.isFirstPosKeyNZero == true)  )
		{
			D3DXMATRIX Inv, MLocal;
			D3DXQUATERNION tempQ;
			D3DXVECTOR3 tempScale, TempTrans;
			if(pCurGeoObject->m_pParent != NULL)
			{
				D3DXMatrixInverse(&Inv,NULL, &pCurGeoObject->m_pParent->NodeTM.NodeTM );
				MLocal = pCurGeoObject->NodeTM.NodeTM * Inv;
			}
			else
			{
				MLocal = pCurGeoObject->NodeTM.NodeTM ;
			}

			D3DXMatrixDecompose(&tempScale, &tempQ,&TempTrans ,&MLocal);
			pCurGeoObject->m_TMAni.pPos[0].pos = TempTrans;
		}
		//2) Rot
		if((pCurGeoObject->m_TMAni.isFirstRotKeyNZero == true) )
		{
			D3DXMATRIX Inv, MLocal ;
			D3DXQUATERNION tempQ;
			D3DXVECTOR3 tempScale, TempTrans;
			if(pCurGeoObject->m_pParent != NULL)
			{
				D3DXMatrixInverse(&Inv,NULL, &pCurGeoObject->m_pParent->NodeTM.NodeTM );
				MLocal = pCurGeoObject->NodeTM.NodeTM * Inv;
			}
			else
			{
				MLocal = pCurGeoObject->NodeTM.NodeTM ;
			}

			D3DXMatrixDecompose(&tempScale, &pCurGeoObject->m_TMAni.pRot[0].quat, &TempTrans ,&MLocal);

		}
		//3) scale
		if(pCurGeoObject->m_TMAni.isFirstScaleKeyNZero == true)
		{
			D3DXMATRIX Inv, MLocal ;
			D3DXQUATERNION tempQ;
			D3DXVECTOR3 tempScale, TempTrans;
			if(pCurGeoObject->m_pParent != NULL)
			{
				D3DXMatrixInverse(&Inv,NULL, &pCurGeoObject->m_pParent->NodeTM.NodeTM );
				MLocal = pCurGeoObject->NodeTM.NodeTM * Inv;
			}
			else
			{
				MLocal = pCurGeoObject->NodeTM.NodeTM ;
			}
			

			D3DXMatrixDecompose(&tempScale, &tempQ,&TempTrans ,&MLocal);
			pCurGeoObject->m_TMAni.pScale[0].scale = tempScale;
			pCurGeoObject->m_TMAni.pScale[0].quat = tempQ;
		}

		//Rotate 애니메이션 누적시켜주기
		if(pCurGeoObject->m_TMAni.pRot != NULL)
		{
			for(int i=1; i< pCurGeoObject->m_TMAni.RotSize ; i++)
			{
				D3DXQuaternionMultiply(&pCurGeoObject->m_TMAni.pRot[i].quat,  &pCurGeoObject->m_TMAni.pRot[i-1].quat,  &pCurGeoObject->m_TMAni.pRot[i].quat );	
			}
		}
		//scale axis 애니메이션 누적시켜주기
		if(pCurGeoObject->m_TMAni.pScale != NULL)
		{
			for(int k=1; k< pCurGeoObject->m_TMAni.FrameCnt ; k++)
			{
				D3DXQuaternionMultiply(&pCurGeoObject->m_TMAni.pScale[i].quat, &pCurGeoObject->m_TMAni.pScale[i-1].quat, &pCurGeoObject->m_TMAni.pScale[i].quat);
			}
		}
		
	}
}
//---------------------------------------------------------------------------------------------
// Scene정보를 파싱한다.
//---------------------------------------------------------------------------------------------

bool ASEParser::Parsing_Scene(ASEMesh* _pAseMesh)
{
	
	size_t tSize=0;
	int mCnt=0;
	CHAR	TokenString[256];
	//TCHAR	wTokenString[256];

	LONG	token = m_pLexer->GetToken(TokenString);

	if(token == TOKEND_BLOCK_START)
	{
		token = m_pLexer->GetToken(TokenString);
		while(token != TOKEND_BLOCK_END)
		{
			switch(token)
			{
				case TOKERN_SCENE_FIRSTFRAME:
					{
						m_pLexer->GetToken(TokenString);   
						_pAseMesh->m_Scene.FirstFrame = atoi(TokenString);
					}
					break;

				case TOKERN_SCENE_TICKSPERFRAME:
					{
						m_pLexer->GetToken(TokenString);   
						_pAseMesh->m_Scene.Ticks_Per_Frame = atoi(TokenString);
					}
					break;
				case TOKERN_SCENE_LASTFRAME:
					{
						m_pLexer->GetToken(TokenString);   
						_pAseMesh->m_Scene.LastFrame = atoi(TokenString);
					}
					break;
				case TOKERN_SCENE_FRAMESPEED:
					{
						m_pLexer->GetToken(TokenString);   
						_pAseMesh->m_Scene.FrameSpeed = atoi(TokenString);
					}
					break;
		

			};

			token = m_pLexer->GetToken(TokenString);

		}

	}
	


	return	TRUE;
}
//////////////////////////////////////////////////////////////////////////
//

bool  ASEParser::Parsing_Material_List(ASEMesh* _pAseMesh)
{
	MATERIAL* ptempM =NULL;
	size_t tSize=0;
	int mCnt=0;
	CHAR	TokenString[256];
	//TCHAR	wTokenString[256];

	LONG	token = m_pLexer->GetToken(TokenString);

	if(token == TOKEND_BLOCK_START)
	{

		token = m_pLexer->GetToken(TokenString);
		while(token != TOKEND_BLOCK_END)
		{
			switch(token)
			{
			case TOKERN_MATERIAL_COUNT:
				{
					 m_pLexer->GetToken(TokenString); mCnt = atoi(TokenString);//메테리얼 총 개수

					if(mCnt <=0)
					{		return false;					}// 총개수가 0보다작으면 메테리얼이 없는것이므로 리턴
				}
				break;

			case TOKERN_MATERIAL:
				{
					 if(mCnt>0)
					 {	
						 ptempM =NULL;
						 ptempM = new MATERIAL(); //일시적인 메테리얼객체를 생성
						 ASSERT_IF_FAIL_MemAlloc(ptempM)  //메모리 생성 실패
							
						m_pLexer->GetToken(TokenString);   ptempM->_mtrl_ID = atoi(TokenString);
						 Parsing_Material(*ptempM);	//정보를 읽어들인 후,,

						 _pAseMesh->m_GA_Mtrl.Insert(ptempM->_mtrl_ID, ptempM);  //CGrowableArray에 담아준다.
					 }							
				}
				break;

			default:

				break;
			};

			token = m_pLexer->GetToken(TokenString);

		}

	}

	return	TRUE;
}


//////////////////////////////////////////////////////////////////////////
//

bool ASEParser::Parsing_Material(MATERIAL& _Mtrl)
{
	size_t tSize=0;
	CHAR	TokenString[256];
	TCHAR	wTokenString[256];

	LONG	token = m_pLexer->GetToken(TokenString);

	if(token == TOKEND_BLOCK_START)
	{

		token = m_pLexer->GetToken(TokenString);
		while(token != TOKEND_BLOCK_END)
		{
			switch(token)
			{
		
				case TOKERN_MATERIAL_CLASS:
					{
						m_pLexer->GetToken(TokenString); //
						mbstowcs_s(&tSize,wTokenString,strlen(TokenString)+1, TokenString, _TRUNCATE); // mbcs문자열을 지정한 바이트 수만큼 wbcs문자열로 변환
						pstrcpy(&(_Mtrl.pClass),wTokenString);
					}
					break;
					
				case TOKERN_MATERIAL_AMBIENT:
					{
						m_pLexer->GetToken(TokenString); //
						_Mtrl._Ambient.r	= (float)atof(TokenString);
						m_pLexer->GetToken(TokenString); //
						_Mtrl._Ambient.g	= (float)atof(TokenString);
						m_pLexer->GetToken(TokenString); //
						_Mtrl._Ambient.b	= (float)atof(TokenString);
						_Mtrl._Ambient.a = 1.0f;
					}					
					break;

				case TOKERN_MATERIAL_DIFFUSE:
					{
						m_pLexer->GetToken(TokenString); //
						_Mtrl._Diffuse.r	= (float)atof(TokenString);
						m_pLexer->GetToken(TokenString); //
						_Mtrl._Diffuse.g	= (float)atof(TokenString);
						m_pLexer->GetToken(TokenString); //
						_Mtrl._Diffuse.b	= (float)atof(TokenString);
						_Mtrl._Diffuse.a    = 1.0f;
					}					
					break;

				case TOKERN_MATERIAL_SPECULAR:
					{
						m_pLexer->GetToken(TokenString); //
						_Mtrl._Specular.r	= (float)atof(TokenString);
						m_pLexer->GetToken(TokenString); //
						_Mtrl._Specular.g	= (float)atof(TokenString);
						m_pLexer->GetToken(TokenString); //
						_Mtrl._Specular.b	= (float)atof(TokenString);
						_Mtrl._Specular.a    = 1.0f;
										
					}					
					break;

				case TOKERN_MATERIAL_SHINE:
					{
						m_pLexer->GetToken(TokenString); 		_Mtrl._Shine= (float)atof(TokenString);
					}					
					break;

				case TOKERN_MATERIAL_SHINESTRENGTH:
					{
						m_pLexer->GetToken(TokenString); 		_Mtrl._Shine_Strength= (float)atof(TokenString);
					}		
					break;

				case TOKERN_MATERIAL_TRANSPARENCY:
					{
						m_pLexer->GetToken(TokenString); 		_Mtrl._Transparency= (float)atof(TokenString);
					}	
					break;

				case TOKERN_MATERIAL_SELFILLUM:
					{
						m_pLexer->GetToken(TokenString); 		_Mtrl._Self_illum= (float)atof(TokenString);
					}	
					break;

				case TOKERN_MAP_DIFFUSE://디퓨즈맵 파싱
					{
						Parsing_DiffuseMap(_Mtrl._Diffuse_Map);
					}	
					break;

				case TOKERN_MAP_GENERIC:
					{	Mtrl_MAP tempGeneric;
						Parsing_DiffuseMap(tempGeneric);
					}	
					break;

				case TOKERN_MAP_OPACITY:
					{	Mtrl_MAP tempMAP;
					Parsing_DiffuseMap(tempMAP);
					}	
					break;
				case TOKERN_MAP_REFLECT:
					{	Mtrl_MAP tempMAP;
					Parsing_DiffuseMap(tempMAP);
					}	
					break;
				case TOKERN_MAP_BUMP:
					{	Mtrl_MAP tempMAP;
					Parsing_DiffuseMap(tempMAP);
					}	
					break;
				case TOKERN_MAP_SELFILLUM:
					{	Mtrl_MAP tempMAP;
					Parsing_DiffuseMap(tempMAP);
					}	
					break;
				case TOKERN_MAP_AMBIENT:
					{	Mtrl_MAP tempMAP;
						Parsing_DiffuseMap(tempMAP);
					}	
					break;
				case TOKERN_MAP_SPECULAR:
					{	Mtrl_MAP tempMAP;
						Parsing_DiffuseMap(tempMAP);
					}	
					break;
				case TOKERN_MAP_SHINE:
					{	Mtrl_MAP tempMAP;
						Parsing_DiffuseMap(tempMAP);
					}	
					break;

				case TOKERN_NUMSUBMTLS://서브메테리얼 개수
					{
							m_pLexer->GetToken(TokenString);	_Mtrl._SubMtrlCnt = atoi(TokenString);
					}	
					break;

				case TOKERN_SUBMATERIAL://서브메테리얼 파싱
					{	
						if(_Mtrl._SubMtrlCnt>0)
						{
							MATERIAL* m = NULL;
							m = new MATERIAL();
							ASSERT_IF_FAIL_MemAlloc(m)
							
							m_pLexer->GetToken(TokenString); m->_mtrl_ID = atoi(TokenString);
							Parsing_Material(*m);
							_Mtrl._GA_SubMtrl.Insert(m->_mtrl_ID,m);
						}
					}	
					break;


			};

			token = m_pLexer->GetToken(TokenString);

		}

	}

	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
bool ASEParser::Parsing_DiffuseMap( Mtrl_MAP& r_DMap)
{
	//CGrowableArray<FACE_Set*> *pGAFaceSet = &_pAseMesh->m_GA_FaceSet;
	size_t tSize=0;
	CHAR	TokenString[256];
	TCHAR	wTokenString[256];

	LONG	token = m_pLexer->GetToken(TokenString);

	if(token == TOKEND_BLOCK_START)
	{

		token = m_pLexer->GetToken(TokenString);
		while(token != TOKEND_BLOCK_END)
		{
			switch(token)
			{

			case TOKERN_MAP_CLASS:
				{
					m_pLexer->GetToken(TokenString); //
					mbstowcs_s(&tSize,wTokenString,strlen(TokenString)+1, TokenString, _TRUNCATE); // mbcs문자열을 지정한 바이트 수만큼 wbcs문자열로 변환
					pstrcpy(&(r_DMap.pClass),wTokenString);
				}
				break;

			case TOKERN_BITMAP:
				{
					m_pLexer->GetToken(TokenString); //
					mbstowcs_s(&tSize,wTokenString,strlen(TokenString)+1, TokenString, _TRUNCATE); // mbcs문자열을 지정한 바이트 수만큼 wbcs문자열로 변환
					pstrcpy(&(r_DMap.pBitmapName),wTokenString);
					removeQmarks(r_DMap.pBitmapName);
					cutSlash(r_DMap.pBitmapName);
				}
				break;

			case TOKERN_UVW_U_OFFSET:
				{
					m_pLexer->GetToken(TokenString); 			r_DMap.u_Offset = (float)atof(TokenString);
				}
				break;

			case TOKERN_UVW_V_OFFSET:
				{
					m_pLexer->GetToken(TokenString); 			r_DMap.v_Offset = (float)atof(TokenString);
				}
			case TOKERN_UVW_U_TILING:
				{
					m_pLexer->GetToken(TokenString); 			r_DMap.u_Tiling = (float)atof(TokenString);
				}
			case TOKERN_UVW_V_TILING:
				{
					m_pLexer->GetToken(TokenString); 			r_DMap.v_Tiling = (float)atof(TokenString);
				}

			default:

				break;
			};

			token = m_pLexer->GetToken(TokenString);

		}

	}

	return	TRUE;
}


//////////////////////////////////////////////////////////////////////////
//

bool ASEParser::Parsing_GeoObject(GEOMOBJECT* _pGeoObjs)
{


	size_t tSize=0;
	CHAR	TokenString[256];
	//TCHAR	wTokenString[256];
	


	LONG	token = m_pLexer->GetToken(TokenString);
	if(token == TOKEND_BLOCK_START)
	{

		token = m_pLexer->GetToken(TokenString);
		while(token != TOKEND_BLOCK_END)
		{
			switch(token)
			{

			case TOKERN_NODE_PARENT: //부모 이름

				m_pLexer->GetToken(TokenString);
				
				strncpy_s(_pGeoObjs->NodeTM.ParentName,TokenString,strlen(TokenString)+1);
			
				break;

			case	TOKERN_NODE_TM:
					{
						Parsing_NodeTM(_pGeoObjs);
					}

				break;

			case	TOKERN_MESH:
					{
						_pGeoObjs->m_HaveMesh = true;
						Parsing_Mesh(_pGeoObjs);
					}
				break;

			case TOKERN_MATERIAL_REF:
				{
					m_pLexer->GetToken(TokenString);  _pGeoObjs->Mtrl_Ref = atoi(TokenString); 
				}
				break;

			case    TOKERN_TM_ANIMATION:
				{
					
					Parsing_TMAnimation(_pGeoObjs);
				}
				break;
	
			};

			token = m_pLexer->GetToken(TokenString);  
		
		}
		
	}

	

	return	TRUE;
}

//---------------------------------------------------------------------------------------------
// *SHAPEOBJECT 파싱
//---------------------------------------------------------------------------------------------
bool ASEParser::Parsing_ShapeObject(GEOMOBJECT* pGeoObjs)
{

	SHAPEOBJECT* _pShapeObjs = dynamic_cast<SHAPEOBJECT*>(pGeoObjs);
	ASSERT_IF_FAIL_D_CAST(_pShapeObjs)

	size_t tSize=0;
	CHAR	TokenString[256];
//	TCHAR	wTokenString[256];	
	
	LONG	token = m_pLexer->GetToken(TokenString);
	if(token == TOKEND_BLOCK_START)
	{
		token = m_pLexer->GetToken(TokenString);
		while(token != TOKEND_BLOCK_END)
		{
			switch(token)
			{

			case TOKERN_NODE_PARENT: //부모 이름

				m_pLexer->GetToken(TokenString);
				strncpy_s(pGeoObjs->NodeTM.ParentName,TokenString,strlen(TokenString)+1);

				break;

			case TOKERN_NODE_TM:
				{
					Parsing_NodeTM(pGeoObjs);
				}

				break;	

			case TOKERN_SHAPE_LINECOUNT:
				{	
					m_pLexer->GetToken(TokenString);  _pShapeObjs->SetLineCnt(atoi(TokenString));
					if(_pShapeObjs->m_LineCnt > 0 )
					{
						_pShapeObjs->pSLine = new SHAPE_LINE[_pShapeObjs->m_LineCnt]; //라인개수만큼 동적생성
						ASSERT_IF_FAIL_MemAlloc(_pShapeObjs->pSLine)
					}
				}
				break;

			case TOKERN_SHAPE_LINE:
				{	int idx;
					m_pLexer->GetToken(TokenString);  idx = atoi(TokenString);
					Parsing_ShapeLine(_pShapeObjs->pSLine[idx]);
					//모든 버텍스좌표와 노말에 NodeTM의 역행렬을 곱해주어 Local좌표로 만든다.
					D3DXMATRIX M_Inv;
					D3DXMatrixInverse(&M_Inv,0,&(_pShapeObjs->NodeTM.NodeTM));

					for(int i=0;i<_pShapeObjs->pSLine[idx].Vert_Cnt ; i++)
					{
						D3DXVec3TransformCoord(&_pShapeObjs->pSLine[idx].pVert_List[i], &_pShapeObjs->pSLine[idx].pVert_List[i], &M_Inv);
					}
				}
				break;

			case    TOKERN_TM_ANIMATION:
				{
					Parsing_TMAnimation(pGeoObjs);
				}
				break;
			};

			token = m_pLexer->GetToken(TokenString);  

		}

	}
	return	TRUE;
}

//---------------------------------------------------------------------------------------------
//  *HELPEROBJECT 파싱
//---------------------------------------------------------------------------------------------

bool ASEParser::Parsing_HerperObject(GEOMOBJECT* pGeoObjs)
{
	size_t tSize=0;
	CHAR	TokenString[256];
//	TCHAR	wTokenString[256];



	LONG	token = m_pLexer->GetToken(TokenString);
	if(token == TOKEND_BLOCK_START)
	{

		token = m_pLexer->GetToken(TokenString);
		while(token != TOKEND_BLOCK_END)
		{
			switch(token)
			{

			case TOKERN_NODE_PARENT: //부모 이름

				m_pLexer->GetToken(TokenString);
			
				strncpy_s(pGeoObjs->NodeTM.ParentName,TokenString,strlen(TokenString)+1);

				break;

			case TOKERN_NODE_TM:
				 {
					Parsing_NodeTM(pGeoObjs);
				 }

				break;	

			case    TOKERN_TM_ANIMATION:
				{
					Parsing_TMAnimation(pGeoObjs);
				}
				break;
			};

			token = m_pLexer->GetToken(TokenString);  

		}

	}
	return	TRUE;
}


//---------------------------------------------------------------------------------------------
// CameraObject 파싱
//---------------------------------------------------------------------------------------------

bool ASEParser::Parsing_CameraObject(ASECameraData* _pCData)
{
	size_t tSize=0;
	CHAR	TokenString[256];
//	TCHAR	wTokenString[256];	
	//targetnode를 파싱하지 않기위한 플레그
	int		nodeflag=0; 
	int		TmFlag =0;


	LONG	token = m_pLexer->GetToken(TokenString);
	if(token == TOKEND_BLOCK_START)
	{
		token = m_pLexer->GetToken(TokenString);
		while(token != TOKEND_BLOCK_END)
		{
			switch(token)
			{

			case TOKERN_NODE_PARENT: //부모 이름

				m_pLexer->GetToken(TokenString);
				
				break;

			case TOKERN_NODE_TM:
				{	
					if(nodeflag == 0)
					{
						Parsing_NodeTM_Camera(_pCData);  nodeflag++;
						break;
					}
					if(nodeflag > 0)
					{	
						while(1)
						{							
							if(m_pLexer->GetToken(TokenString) ==TOKEND_BLOCK_END ) break;
						}
						break;
					}
				}

				break;	
			case TOKERN_CAMERA_SETTINGS:
				{
					Parsing_CameraSetting(_pCData);
				}
				break;

			case TOKERN_TM_ANIMATION:
				{	
					if(TmFlag == 0 )
					{
						Parsing_TMAnimation_Camera(_pCData); TmFlag++;
						break;
					}
					if(TmFlag > 0)
					{	int cnt = 0;
						while(cnt<2)
						{
							if(m_pLexer->GetToken(TokenString) == TOKEND_BLOCK_END) 
							{
								cnt++;
							}
						}
						break;
					}
					
				}
				break;
			};

			token = m_pLexer->GetToken(TokenString);  
		}

	}
	//로컬TM 구하기
	_pCData->m_MLocalTM = _pCData->NodeTM.NodeTM;
	//여기서부터 Decompose

	D3DXQUATERNION QRot;
	D3DXVECTOR3 scaleVec, transVec;
	D3DXMatrixDecompose(&scaleVec, &QRot,&transVec, &_pCData->m_MLocalTM );

	D3DXMatrixTranslation(&_pCData->m_LTM_Pos,transVec.x, transVec.y, transVec.z);
	D3DXMatrixRotationQuaternion(&_pCData->m_LTM_Rot,&QRot);
	D3DXMatrixScaling(&_pCData->m_LTM_scale, scaleVec.x, scaleVec.y, scaleVec.z );

	if( (_pCData->m_TMAni.isFirstPosKeyNZero == true)  )
	{
		D3DXMATRIX Inv, MLocal;
		D3DXQUATERNION tempQ;
		D3DXVECTOR3 tempScale, TempTrans;
		MLocal = _pCData->NodeTM.NodeTM ;

		D3DXMatrixDecompose(&tempScale, &tempQ,&TempTrans ,&MLocal);
		_pCData->m_TMAni.pPos[0].pos = TempTrans;
	}
	//2) Rot
	if((_pCData->m_TMAni.isFirstRotKeyNZero == true) )
	{
		D3DXMATRIX Inv, MLocal ;
		D3DXQUATERNION tempQ;
		D3DXVECTOR3 tempScale, TempTrans;
		MLocal = _pCData->NodeTM.NodeTM ;
	
		D3DXMatrixDecompose(&tempScale, &_pCData->m_TMAni.pRot[0].quat, &TempTrans ,&MLocal);

	}
	//3) scale
	if(_pCData->m_TMAni.isFirstScaleKeyNZero == true)
	{
		D3DXMATRIX Inv, MLocal ;
		D3DXQUATERNION tempQ;
		D3DXVECTOR3 tempScale, TempTrans;

		MLocal = _pCData->NodeTM.NodeTM * Inv;

		D3DXMatrixDecompose(&tempScale, &tempQ,&TempTrans ,&MLocal);
		_pCData->m_TMAni.pScale[0].scale = tempScale;
	}

	for(int i=1; i< _pCData->m_TMAni.RotSize ; i++)
	{
		D3DXQuaternionMultiply(&_pCData->m_TMAni.pRot[i].quat,  &_pCData->m_TMAni.pRot[i-1].quat,  &_pCData->m_TMAni.pRot[i].quat );	
	}
	return	TRUE;
}

//---------------------------------------------------------------------------------------------
// Parsing_NodeTM
// GeomObject의 노트Tm을 읽어들인다.
//---------------------------------------------------------------------------------------------

bool ASEParser::Parsing_NodeTM(GEOMOBJECT* pGeoObjs)
{
	NODE_TM TempNodeTM;

	D3DXVECTOR3 RotAxis,  ScaleVec, ScaleAxis;
//	float		 ScaleAngle;


	size_t tSize=0;
	CHAR	TokenString[512];
/*	TCHAR	wTokenString[512];*/

	LONG	token = m_pLexer->GetToken(TokenString);

	if(token == TOKEND_BLOCK_START)
	{

		token = m_pLexer->GetToken(TokenString);
		while(token != TOKEND_BLOCK_END)
		{
			switch(token)
			{
			case TOKERN_NODE_NAME://노드 이름

				m_pLexer->GetToken(TokenString);

				strncpy_s(pGeoObjs->NodeTM.Name , TokenString, strlen(TokenString)+1);
			
				break;

			case	TOKERN_TM_ROW0:
				{
				    m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					TempNodeTM.NodeTM._11	 = (float)atof(TokenString);
					token = m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					TempNodeTM.NodeTM._13 = (float)atof(TokenString);
					token = m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					TempNodeTM.NodeTM._12 = (float)atof(TokenString);
					TempNodeTM.NodeTM._14		 = 0.0f;
				}
				break;

			case	TOKERN_TM_ROW1:
				{
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					TempNodeTM.NodeTM._21	 = (float)atof(TokenString);
					token = m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					TempNodeTM.NodeTM._23 = (float)atof(TokenString);
					token = m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					TempNodeTM.NodeTM._22 = (float)atof(TokenString);
					TempNodeTM.NodeTM._24		 = 0.0f;
				}
				break;

			case	TOKERN_TM_ROW2:
				{
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					TempNodeTM.NodeTM._31	 = (float)atof(TokenString);
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					TempNodeTM.NodeTM._33 = (float)atof(TokenString);
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					TempNodeTM.NodeTM._32 = (float)atof(TokenString);
					TempNodeTM.NodeTM._34		 = 0.0f;
				}
				break;

			case	TOKERN_TM_ROW3:
				{
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					TempNodeTM.NodeTM._41	 = (float)atof(TokenString);
					token = m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					TempNodeTM.NodeTM._43 = (float)atof(TokenString);
					token = m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					TempNodeTM.NodeTM._42 = (float)atof(TokenString);
					TempNodeTM.NodeTM._44		 = 1.0f;
				}
				break;
			case	TOKERN_TM_POS:
				{
					D3DXVECTOR3 trans;
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					pGeoObjs->NodeTM.Pos.x = (float)atof(TokenString);
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					pGeoObjs->NodeTM.Pos.z = (float)atof(TokenString);
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					pGeoObjs->NodeTM.Pos.y = (float)atof(TokenString);		
				
				}
				break;
			case	TOKERN_TM_ROTAXIS:
				{
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					pGeoObjs->NodeTM.RotAxis.x = (float)atof(TokenString);
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					pGeoObjs->NodeTM.RotAxis.z = (float)atof(TokenString);
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					pGeoObjs->NodeTM.RotAxis.y = (float)atof(TokenString);
				}
				break;

			case	TOKERN_TM_ROTANGLE:
				{
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					pGeoObjs->NodeTM.RotAngle= (float)atof(TokenString);
					
				}
				break;
			case	TOKERN_TM_SCALE:
				{
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					pGeoObjs->NodeTM.Scale.x= (float)atof(TokenString);
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					pGeoObjs->NodeTM.Scale.z= (float)atof(TokenString);
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					pGeoObjs->NodeTM.Scale.y= (float)atof(TokenString);
				}
				break;
			case	TOKERN_TM_SCALEAXIS:
				{
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					pGeoObjs->NodeTM.ScaleAxis.x= (float)atof(TokenString);
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					pGeoObjs->NodeTM.ScaleAxis.z= (float)atof(TokenString);
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					pGeoObjs->NodeTM.ScaleAxis.y= (float)atof(TokenString);


				}
				break;
			case	TOKERN_TM_SCALEAXISANG:
				{
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					pGeoObjs->NodeTM.ScaleAngle= (float)atof(TokenString);  
				}
				break;
			};

			token = m_pLexer->GetToken(TokenString);

		}

	}
	/* //네거티브스케일 포함된 노드TM
	pGeoObjs->NodeTM.NodeTM._11 = TempNodeTM.NodeTM._11; 
	pGeoObjs->NodeTM.NodeTM._12 = TempNodeTM.NodeTM._12;
	pGeoObjs->NodeTM.NodeTM._13 = TempNodeTM.NodeTM._13; 
	pGeoObjs->NodeTM.NodeTM._14 = TempNodeTM.NodeTM._14; 

	pGeoObjs->NodeTM.NodeTM._21 = TempNodeTM.NodeTM._31; 
	pGeoObjs->NodeTM.NodeTM._22 = TempNodeTM.NodeTM._32;
	pGeoObjs->NodeTM.NodeTM._23 = TempNodeTM.NodeTM._33; 
	pGeoObjs->NodeTM.NodeTM._24 = TempNodeTM.NodeTM._34; 

	pGeoObjs->NodeTM.NodeTM._31 = TempNodeTM.NodeTM._21; 
	pGeoObjs->NodeTM.NodeTM._32 = TempNodeTM.NodeTM._22;
	pGeoObjs->NodeTM.NodeTM._33 = TempNodeTM.NodeTM._23; 
	pGeoObjs->NodeTM.NodeTM._34 = TempNodeTM.NodeTM._24; 

	pGeoObjs->NodeTM.NodeTM._41 = TempNodeTM.NodeTM._41; 
	pGeoObjs->NodeTM.NodeTM._42 = TempNodeTM.NodeTM._42;
	pGeoObjs->NodeTM.NodeTM._43 = TempNodeTM.NodeTM._43; 
	pGeoObjs->NodeTM.NodeTM._44 = TempNodeTM.NodeTM._44; 
	*/

	//네거티브스케일이 포함안된 노드TM을 만들기 위한 코드
	{
		D3DXMATRIX PosTM, RotTM, ScaleTM, ScaleAxisTM, ScaleAxisTM_Inv;

		D3DXMatrixTranslation(&PosTM, pGeoObjs->NodeTM.Pos.x, pGeoObjs->NodeTM.Pos.y, pGeoObjs->NodeTM.Pos.z);
		D3DXMatrixRotationAxis(&RotTM, 	&pGeoObjs->NodeTM.RotAxis, 	pGeoObjs->NodeTM.RotAngle );
		D3DXMatrixScaling(&ScaleTM, pGeoObjs->NodeTM.Scale.x, pGeoObjs->NodeTM.Scale.y, pGeoObjs->NodeTM.Scale.z);
		D3DXMatrixRotationAxis(&ScaleAxisTM,  &pGeoObjs->NodeTM.ScaleAxis, pGeoObjs->NodeTM.ScaleAngle);
		D3DXMatrixInverse(&ScaleAxisTM_Inv, NULL, &ScaleAxisTM);

		pGeoObjs->NodeTM.NodeTM = ScaleAxisTM_Inv * ScaleTM * ScaleAxisTM * RotTM * PosTM;
	}

	return	TRUE;

}
//-----------------------------------------------------------------------------
// 카메라용 노드TM 읽기
//-----------------------------------------------------------------------------
bool ASEParser::Parsing_NodeTM_Camera(ASECameraData* _pCData)
{	ASECameraData tempC;
	if(_pCData == NULL)
	{
		_pCData = &tempC;
	}
	NODE_TM TempNodeTM;

	D3DXVECTOR3 RotAxis,  ScaleVec, ScaleAxis;
//	float		/*RotAngle,*/ ScaleAngle;


	size_t tSize=0;
	CHAR	TokenString[512];
	/*TCHAR	wTokenString[512];*/

	LONG	token = m_pLexer->GetToken(TokenString);

	if(token == TOKEND_BLOCK_START)
	{

		token = m_pLexer->GetToken(TokenString);
		while(token != TOKEND_BLOCK_END)
		{
			switch(token)
			{
			case TOKERN_NODE_NAME://노드 이름

				m_pLexer->GetToken(TokenString);

				strncpy_s(_pCData->NodeTM.Name , TokenString, strlen(TokenString)+1);

				break;

			case	TOKERN_TM_ROW0:
				{
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					TempNodeTM.NodeTM._11	 = (float)atof(TokenString);
					token = m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					TempNodeTM.NodeTM._13 = (float)atof(TokenString);
					token = m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					TempNodeTM.NodeTM._12 = (float)atof(TokenString);
					TempNodeTM.NodeTM._14		 = 0.0f;
				}
				break;

			case	TOKERN_TM_ROW1:
				{
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					TempNodeTM.NodeTM._21	 = (float)atof(TokenString);
					token = m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					TempNodeTM.NodeTM._23 = (float)atof(TokenString);
					token = m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					TempNodeTM.NodeTM._22 = (float)atof(TokenString);
					TempNodeTM.NodeTM._24		 = 0.0f;
				}
				break;

			case	TOKERN_TM_ROW2:
				{
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					TempNodeTM.NodeTM._31	 = (float)atof(TokenString);
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					TempNodeTM.NodeTM._33 = (float)atof(TokenString);
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					TempNodeTM.NodeTM._32 = (float)atof(TokenString);
					TempNodeTM.NodeTM._34		 = 0.0f;
				}
				break;

			case	TOKERN_TM_ROW3:
				{
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					TempNodeTM.NodeTM._41	 = (float)atof(TokenString);
					token = m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					TempNodeTM.NodeTM._43 = (float)atof(TokenString);
					token = m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					TempNodeTM.NodeTM._42 = (float)atof(TokenString);
					TempNodeTM.NodeTM._44		 = 1.0f;
				}
				break;
			case	TOKERN_TM_POS:
				{
					D3DXVECTOR3 trans;
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					_pCData->NodeTM.Pos.x = (float)atof(TokenString);
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					_pCData->NodeTM.Pos.z = (float)atof(TokenString);
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					_pCData->NodeTM.Pos.y = (float)atof(TokenString);		

				}
				break;
			case	TOKERN_TM_ROTAXIS:
				{
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					_pCData->NodeTM.RotAxis.x = (float)atof(TokenString);
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					_pCData->NodeTM.RotAxis.z = (float)atof(TokenString);
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					_pCData->NodeTM.RotAxis.y = (float)atof(TokenString);
				}
				break;

			case	TOKERN_TM_ROTANGLE:
				{
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					_pCData->NodeTM.RotAngle= (float)atof(TokenString);

				}
				break;
			case	TOKERN_TM_SCALE:
				{
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					_pCData->NodeTM.Scale.x= (float)atof(TokenString);
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					_pCData->NodeTM.Scale.z= (float)atof(TokenString);
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					_pCData->NodeTM.Scale.y= (float)atof(TokenString);
				}
				break;
			case	TOKERN_TM_SCALEAXIS:
				{
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					_pCData->NodeTM.ScaleAxis.x= (float)atof(TokenString);
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					_pCData->NodeTM.ScaleAxis.z= (float)atof(TokenString);
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					_pCData->NodeTM.ScaleAxis.y= (float)atof(TokenString);


				}
				break;
			case	TOKERN_TM_SCALEAXISANG:
				{
					m_pLexer->GetToken(TokenString);		// TOKEND_NUMBER
					_pCData->NodeTM.ScaleAngle= (float)atof(TokenString);  
				}
				break;
			};

			token = m_pLexer->GetToken(TokenString);

		}

	}
	_pCData->NodeTM.NodeTM._11 = TempNodeTM.NodeTM._11; 
	_pCData->NodeTM.NodeTM._12 = TempNodeTM.NodeTM._12;
	_pCData->NodeTM.NodeTM._13 = TempNodeTM.NodeTM._13; 
	_pCData->NodeTM.NodeTM._14 = TempNodeTM.NodeTM._14; 

	_pCData->NodeTM.NodeTM._21 = TempNodeTM.NodeTM._31; 
	_pCData->NodeTM.NodeTM._22 = TempNodeTM.NodeTM._32;
	_pCData->NodeTM.NodeTM._23 = TempNodeTM.NodeTM._33; 
	_pCData->NodeTM.NodeTM._24 = TempNodeTM.NodeTM._34; 

	_pCData->NodeTM.NodeTM._31 = TempNodeTM.NodeTM._21; 
	_pCData->NodeTM.NodeTM._32 = TempNodeTM.NodeTM._22;
	_pCData->NodeTM.NodeTM._33 = TempNodeTM.NodeTM._23; 
	_pCData->NodeTM.NodeTM._34 = TempNodeTM.NodeTM._24; 

	_pCData->NodeTM.NodeTM._41 = TempNodeTM.NodeTM._41; 
	_pCData->NodeTM.NodeTM._42 = TempNodeTM.NodeTM._42;
	_pCData->NodeTM.NodeTM._43 = TempNodeTM.NodeTM._43; 
	_pCData->NodeTM.NodeTM._44 = TempNodeTM.NodeTM._44; 

	D3DXMATRIX RotTM;

	D3DXMatrixRotationX(&RotTM,D3DX_PI/2.0f); //x축으로 90도 회전
	_pCData->NodeTM.NodeTM = RotTM* _pCData->NodeTM.NodeTM ;
	
	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////
//

bool ASEParser::Parsing_Mesh(GEOMOBJECT* pGeoObjs)
{
	
	int Norm_Cnt = 0; //노말을 담을떄 사용할 index
	int Vert_idx = 0; //버텍스 인텍스

	// 읽어들인값을 잠시 저장해 놓을 공간
	
	pGeoObjs->p_I_N_MtID = NULL;
	D3DXVECTOR3* pTempVec3 = NULL;

	size_t tSize=0;
	CHAR	TokenString[256];
//	TCHAR	wTokenString[256];

	LONG	token = m_pLexer->GetToken(TokenString);

	if(token == TOKEND_BLOCK_START)
	{

		token = m_pLexer->GetToken(TokenString);
		while(token != TOKEND_BLOCK_END)
		{
			switch(token)
			{
				case TOKERN_MESH_NUMBONE:
					{	
						m_pLexer->GetToken(TokenString);  m_BoneCnt = atoi(TokenString);
						if(m_BoneCnt > 0)
						{
// 							SKINNED_OBJECT* pSkinedObjs = NULL;
// 							GEOMOBJECT* preGObjs= pGeoObjs; //이전  pGeoObjs의 포인터를 잠시 저장(메모리릭 안나게)
// 
// 							pGeoObjs = NULL;
// 							pGeoObjs = new SKINNED_OBJECT(); //pGeoObjs를 SKINNED_OBJECT객체로 다시 생성한다.
// 							ASSERT_IF_FAIL_MemAlloc(pGeoObjs)  //메모리 할당실패할 경우 어썰트
// 							
// 							pGeoObjs->m_TMAni.FrameCnt  =  preGObjs->pAseData->m_Scene.LastFrame  -   preGObjs->pAseData->m_Scene.FirstFrame;
// 							pGeoObjs->pAseData  =   preGObjs->pAseData;

							pTempBoneList = new BONE_LIST[m_BoneCnt];
							ASSERT_IF_FAIL_MemAlloc(pTempBoneList)
							pGeoObjs->m_isSkinned = true;
							pGeoObjs->num_BoneCnt = m_BoneCnt;
						}
						
					}
					break;
				
				case TOKERN_MESH_NUMVERTEX://버텍스 개수

					m_pLexer->GetToken(TokenString);
					pGeoObjs->num_vertex = atoi(TokenString); //버텍수 개수 얻기
			
					break;

				case TOKERN_MESH_NUMTVERTEX:
					{
						int TvertexCnt;
						m_pLexer->GetToken(TokenString);  TvertexCnt = atoi(TokenString);
						if(TvertexCnt>0)
						{
						  pGeoObjs->ptVertex = new D3DXVECTOR3[TvertexCnt];//ptVertex[]는 나중에 createmesh 함수에서 메모리해제
						  ASSERT_IF_FAIL_MemAlloc(pGeoObjs->ptVertex)
						}
					

					}
					break;

				case TOKERN_MESH_NUMFACES: //삼각형 개수 
					{		
						m_pLexer->GetToken(TokenString);
						pGeoObjs->num_faces = atoi(TokenString);
						// 여기서 일단 최악의 경우를 대비해 인덱스개수 * 3의 개수만큼 버텍스를 담을 공간을 만든다.
						pGeoObjs->pVertex= new D3DXVECTOR3[pGeoObjs->num_faces*3];
						ASSERT_IF_FAIL_MemAlloc(pGeoObjs->pVertex)
						// 여기서 일단 최악의 경우를 대비해 인덱스개수 * 3의 개수만큼 노말을 담을 벡터배열을 만든다.
						pGeoObjs->num_pNormalCnt = pGeoObjs->num_faces*3;
						pGeoObjs->pNormal = new D3DXVECTOR3[pGeoObjs->num_pNormalCnt];
						ASSERT_IF_FAIL_MemAlloc(pGeoObjs->pNormal)
						memset(pGeoObjs->pNormal,0.0f,sizeof(D3DXVECTOR3)*pGeoObjs->num_faces*3);

						pGeoObjs->pUV = new UV_COORD[pGeoObjs->num_faces*3];//createmesh 함수에서 메모리 해제
						ASSERT_IF_FAIL_MemAlloc(pGeoObjs->pUV)
						memset(pGeoObjs->pUV,0,sizeof(UV_COORD)*pGeoObjs->num_faces*3);//처음에 다 0으로 초기화

						pGeoObjs->p_I_N_MtID = new Temp_idx_Norm_mtID[pGeoObjs->num_faces*3];
						ASSERT_IF_FAIL_MemAlloc(pGeoObjs->p_I_N_MtID)

						pGeoObjs->pFace_List = new FACE_LIST[pGeoObjs->num_faces];
						ASSERT_IF_FAIL_MemAlloc(pGeoObjs->pFace_List)

						if(pGeoObjs->m_isSkinned == TRUE) //스키닝 메쉬라면?
						{
							pGeoObjs->pVertWeightList = new VWeight_List[pGeoObjs->num_faces*3];//웨이트값을위한 메모리할당
							ASSERT_IF_FAIL_MemAlloc(pGeoObjs->pVertWeightList)
							memset(pGeoObjs->pVertWeightList ,0 , sizeof(VWeight_List)*pGeoObjs->num_faces*3);//처음에 다 0으로 초기화

						}
					}
					break;

				case TOKERN_MESH_VERTEX_LIST:
					{
						LONG	token = m_pLexer->GetToken(TokenString);
						if(token == TOKEND_BLOCK_START)
						{
							token = m_pLexer->GetToken(TokenString);
							while(token != TOKEND_BLOCK_END)
							{	switch(token)
								{
									case TOKERN_MESH_VERTEX://버텍스정보 읽어들이기

										m_pLexer->GetToken(TokenString);
										Vert_idx = atoi(TokenString);

										m_pLexer->GetToken(TokenString);
										pGeoObjs->pVertex[Vert_idx].x = (float)atof(TokenString);//버텍스 x값 얻기
										m_pLexer->GetToken(TokenString);
										pGeoObjs->pVertex[Vert_idx].z = (float)atof(TokenString);//버텍스 z값 얻기
										m_pLexer->GetToken(TokenString);
										pGeoObjs->pVertex[Vert_idx].y = (float)atof(TokenString);//버텍스 y값 얻기

										break;
								}
								token = m_pLexer->GetToken(TokenString);
							}
						}
					}

					break;
		
				case TOKERN_MESH_FACE_LIST:
					{ int idx=0;
						LONG	token = m_pLexer->GetToken(TokenString);
						if(token == TOKEND_BLOCK_START)
						{
							token = m_pLexer->GetToken(TokenString);
							while(token != TOKEND_BLOCK_END)
							{
								switch(token)
								{
									case TOKERN_MESH_MTLID:
										m_pLexer->GetToken(TokenString);
										pGeoObjs->pFace_List[idx].mtrl_ID = atoi(TokenString);
										idx++;
									break;

								}
								token = m_pLexer->GetToken(TokenString);
							}
						}
					}

					break;
				case TOKERN_MESH_TVERTLIST:
					{	
						int idx=0;
						
																				  // 메쉬 만들고 나면 메모리해제
						ASSERT_IF_FAIL_MemAlloc(pGeoObjs->ptVertex) 
						//if(pGeoObjs->ptVertex == NULL){write_log_file("메모리 할당 실패 : pGeoObjs->ptVertex ");assert(true);}
						token = m_pLexer->GetToken(TokenString);

						while(token != TOKEND_BLOCK_END)
						{	
							switch(token)
							{
								case TOKERN_MESH_TVERT :
									{
										m_pLexer->GetToken(TokenString);
										idx = atoi(TokenString);
										m_pLexer->GetToken(TokenString); //U
										pGeoObjs->ptVertex[idx].x = (float)atof(TokenString);
										m_pLexer->GetToken(TokenString); //V
										pGeoObjs->ptVertex[idx].y = 1-(float)atof(TokenString);
										m_pLexer->GetToken(TokenString);
										pGeoObjs->ptVertex[idx].z =0.0f;

									}
									break;


							}
							token = m_pLexer->GetToken(TokenString);
						}
					}

					break;

				case TOKERN_MESH_TFACELIST:
					{	
						int idx=0;
						pGeoObjs->ptFace = new int[pGeoObjs->num_faces*3]; //createmesh 함수에서 메모리 해제
						if(pGeoObjs->ptFace == NULL){write_log_file("메모리 할당 실패 : pGeoObjs->ptFace ");}

						token = m_pLexer->GetToken(TokenString);
						while(token != TOKEND_BLOCK_END)
						{	switch(token)
							{
								case TOKERN_MESH_TFACE :
									{
										m_pLexer->GetToken(TokenString);
										idx = atoi(TokenString);
										m_pLexer->GetToken(TokenString); 
										pGeoObjs->ptFace[idx*3]   = atoi(TokenString);//tface 
										m_pLexer->GetToken(TokenString); 
										pGeoObjs->ptFace[idx*3+1] = atoi(TokenString);
										m_pLexer->GetToken(TokenString);
										pGeoObjs->ptFace[idx*3+2] = atoi(TokenString);

									}
									break;


							}
							token = m_pLexer->GetToken(TokenString);
						}
					}

					break;

				case TOKERN_MESH_NORMALS:
					{	
						int idx=0;				

						token = m_pLexer->GetToken(TokenString);
						while(token != TOKEND_BLOCK_END)
						{	
							switch(token)
							{
								case TOKERN_MESH_VERTEXNORMAL:

									m_pLexer->GetToken(TokenString);//인덱스읽기
									pGeoObjs->p_I_N_MtID[Norm_Cnt].idx	   = atoi(TokenString);
									m_pLexer->GetToken(TokenString);
									pGeoObjs->p_I_N_MtID[Norm_Cnt].Norm.x = (float)atof(TokenString);// x 값 얻기
									m_pLexer->GetToken(TokenString);
									pGeoObjs->p_I_N_MtID[Norm_Cnt].Norm.z = (float)atof(TokenString);// z 값 얻기
									m_pLexer->GetToken(TokenString);
									pGeoObjs->p_I_N_MtID[Norm_Cnt].Norm.y = (float)atof(TokenString);// y 값 얻기

									pGeoObjs->p_I_N_MtID[Norm_Cnt].mtrl_ID = pGeoObjs->pFace_List[Norm_Cnt/3].mtrl_ID;

									Norm_Cnt+=1;

									break;
							}
							token = m_pLexer->GetToken(TokenString);
						}
					}

					break;

				case TOKERN_BONE_LIST:
					{	
						token = m_pLexer->GetToken(TokenString);
						while(token != TOKEND_BLOCK_END)
						{	
							switch(token)
							{
							case TOKERN_BONE:
								{
									int idx;
									token = m_pLexer->GetToken(TokenString); idx =  atoi(TokenString);

									while(token != TOKEND_BLOCK_END)
									{	
										switch(token)
										{
										case TOKERN_BONE_NAME:
											{	
												token = m_pLexer->GetToken(TokenString);
												pTempBoneList[idx].idx = idx;
												strcpy_s(pTempBoneList[idx].Name, TokenString);
											}
											break;
										}
										token = m_pLexer->GetToken(TokenString);
									}
								}
							
								break;
							}
							token = m_pLexer->GetToken(TokenString);
						}
					}

					break;

				case TOKERN_MESH_WVERTEXS:
					{	
						token = m_pLexer->GetToken(TokenString);
						while(token != TOKEND_BLOCK_END)
						{	
							switch(token)
							{
							case TOKERN_MESH_WEIGHT:
								{
									int idx; 
									token = m_pLexer->GetToken(TokenString); idx =  atoi(TokenString);
									Vertex_Weight tempVW;
									while(token != TOKEND_BLOCK_END)
									{	
										switch(token)
										{
										case TOKERN_BONE_BLENGING_WEIGHT:
											{	
												
												m_pLexer->GetToken(TokenString);
												tempVW.Boneidx = atoi(TokenString);
												m_pLexer->GetToken(TokenString);
												tempVW.Weight = (float)atof(TokenString);

												pGeoObjs->pVertWeightList[idx].insert(tempVW); //벡터에 넣어준다.
											
											}
											break;
										}
										token = m_pLexer->GetToken(TokenString);
									}

						/*%%*/	pGeoObjs->pVertWeightList[idx].Sort_By_Weight(); //웨이트값에따라 알아서 내림차순이 되는 함수
								}

								break;
							}
							token = m_pLexer->GetToken(TokenString);
						}
					}
					break;

				case TOKERN_SKIN_INITTM:
					{	D3DXMATRIX  tempTM;
						token = m_pLexer->GetToken(TokenString);
						while(token != TOKEND_BLOCK_END)
						{	
				
							
							token = m_pLexer->GetToken(TokenString);
						}

					
					}
					break;
			
			};

			token = m_pLexer->GetToken(TokenString);

		}

	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//★★★ 버텍스가 같은 idx인데 노말이 다른 경우 버텍스 나누어 주기 , Norm_Cnt = face*3 ★★★
	// pGeoObjs->p_I_N_MtID[] 에는 *MESH_VERTEXNORMAL[...] 정보가 들어있다.(*MESH_FACENORMAL 은 제외) 
	int vidx=0; //버텍스 인덱스

	for(int i=0; i<Norm_Cnt ; i++)
	{
		vidx = pGeoObjs->p_I_N_MtID[i].idx;

		if(pGeoObjs->pNormal[vidx] == g_ZeroVec3 )// 노말이 0, 0, 0 이라면 빈것이므로 넣어준다.
		{	
			pGeoObjs->pNormal[vidx] = pGeoObjs->p_I_N_MtID[i].Norm;
			//이때 U,V값도 같이 넣어준다. 
			if(pGeoObjs->ptFace !=NULL) //uv값이 있다면??
			{
				pGeoObjs->pUV[vidx]._u = pGeoObjs->ptVertex[pGeoObjs->ptFace[i]].x;//U
				pGeoObjs->pUV[vidx]._v = pGeoObjs->ptVertex[pGeoObjs->ptFace[i]].y;//V
			}
		
		}
		else if(( pGeoObjs->pNormal[vidx] == pGeoObjs->p_I_N_MtID[i].Norm) && (pGeoObjs->ptFace != NULL))  
		{		/*하지만 ★★★노말과 버텍스좌표는 같은데 UV좌표가 다를 경우★★★가 있으므로(multi-sub 메테리얼의경우!!) 	
				       그럴경우 또 버텍스를 새로 추가해 줘야 한다.  */			
		
				if( (pGeoObjs->ptVertex[pGeoObjs->ptFace[i]].x != pGeoObjs->pUV[vidx]._u) ||
					(pGeoObjs->ptVertex[pGeoObjs->ptFace[i]].y != pGeoObjs->pUV[vidx]._v)
				  )
				{
					pGeoObjs->pVertex[pGeoObjs->num_vertex] = pGeoObjs->pVertex[vidx];//버텍스 버퍼 마지막에 버텍스 복사(추가)
					pGeoObjs->pNormal[pGeoObjs->num_vertex] = pGeoObjs->p_I_N_MtID[i].Norm;// 노말배열마지막에 추가
					
					if(pGeoObjs->m_isSkinned == true)
					pGeoObjs->pVertWeightList[pGeoObjs->num_vertex] = pGeoObjs->pVertWeightList[vidx]; //버텍스웨이트도 마지막에 복사(추가)

					pGeoObjs->pUV[pGeoObjs->num_vertex]._u = pGeoObjs->ptVertex[pGeoObjs->ptFace[i]].x;//U	//U,V값 넣어주기
					pGeoObjs->pUV[pGeoObjs->num_vertex]._v = pGeoObjs->ptVertex[pGeoObjs->ptFace[i]].y;//V	//U,V값 넣어주기
					
					for(int j=i ; j<Norm_Cnt ; j++ )
					{
						if( (pGeoObjs->p_I_N_MtID[j].idx == vidx)   &&   (pGeoObjs->p_I_N_MtID[j].Norm == pGeoObjs->p_I_N_MtID[i].Norm ) &&
							(pGeoObjs->ptFace[j] == pGeoObjs->ptFace[i])
						  )
						{
							pGeoObjs->p_I_N_MtID[j].idx  =  pGeoObjs->num_vertex;
						}
					}
					pGeoObjs->num_vertex+=1;
				}
		}
		else if(pGeoObjs->pNormal[vidx] != pGeoObjs->p_I_N_MtID[i].Norm) //버텍스 idx는 같은데 노말이 다른경우
		{	
			pGeoObjs->pVertex[pGeoObjs->num_vertex] = pGeoObjs->pVertex[vidx];//버텍스 버퍼 마지막에 버텍스 복사(추가)
			
			if(pGeoObjs->m_isSkinned == true)//스킨드 메쉬라면
			pGeoObjs->pVertWeightList[pGeoObjs->num_vertex] = pGeoObjs->pVertWeightList[vidx]; //버텍스웨이트도 마지막에 복사(추가)
			
			pGeoObjs->pNormal[pGeoObjs->num_vertex] = pGeoObjs->p_I_N_MtID[i].Norm;// 노말배열마지막에 추가
			//U,V값 넣어주기
			if(pGeoObjs->ptFace !=NULL) //uv값이 있다면??
			{
				pGeoObjs->pUV[pGeoObjs->num_vertex]._u = pGeoObjs->ptVertex[pGeoObjs->ptFace[i]].x;//U
				pGeoObjs->pUV[pGeoObjs->num_vertex]._v = pGeoObjs->ptVertex[pGeoObjs->ptFace[i]].y;//V
			}

			for(int j=i ; j<Norm_Cnt ; j++ )
			{
				if( (pGeoObjs->p_I_N_MtID[j].idx == vidx)   &&   (pGeoObjs->p_I_N_MtID[j].Norm == pGeoObjs->p_I_N_MtID[i].Norm )   )
				{
					pGeoObjs->p_I_N_MtID[j].idx  =  pGeoObjs->num_vertex;
				}
			}

			pGeoObjs->num_vertex+=1;			
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//인덱스 버퍼 만들기
	 pGeoObjs->num_pIndice= pGeoObjs->num_faces*3;
	 pGeoObjs->p_indices = new WORD[	pGeoObjs->num_pIndice];
	
	 ASSERT_IF_FAIL_MemAlloc(pGeoObjs->p_indices )
	 for(int i=0; i<pGeoObjs->num_faces*3; i++)
	 {
	    pGeoObjs->p_indices[i] = pGeoObjs->p_I_N_MtID[i].idx; //인덱스 복사
	 }

	//face_list 만들기
	 
	for(int i=0; i<pGeoObjs->num_faces; i++)
	{
		pGeoObjs->pFace_List[i].idx_1 = pGeoObjs->p_I_N_MtID[i*3].idx; //인덱스 복사
		pGeoObjs->pFace_List[i].idx_2 = pGeoObjs->p_I_N_MtID[i*3+1].idx; //인덱스 복사
		pGeoObjs->pFace_List[i].idx_3 = pGeoObjs->p_I_N_MtID[i*3+2].idx; //인덱스 복사
	}

	//////////////////////////////////////////////////////////////////////////
	//임시 저장 메모리 해제
	//if(pGeoObjs->p_I_N_MtID!= NULL){delete[] pGeoObjs->p_I_N_MtID; pGeoObjs->p_I_N_MtID=NULL;}

	if(pTempVec3!= NULL){delete[] pTempVec3; pTempVec3=NULL;}

	//모든 버텍스좌표와 노말에 NodeTM의 역행렬을 곱해주어 Local좌표로 만든다.
	D3DXMATRIX M_Inv;
	D3DXMatrixInverse(&M_Inv,0,&(pGeoObjs->NodeTM.NodeTM));

	for(int i=0;i<pGeoObjs->num_vertex ; i++)
	{
		D3DXVec3TransformCoord(&pGeoObjs->pVertex[i], &pGeoObjs->pVertex[i], &M_Inv);
		D3DXVec3TransformNormal(&pGeoObjs->pNormal[i] , &pGeoObjs->pNormal[i] , &M_Inv);

	}


	return	TRUE;
	
}

//-----------------------------------------------------------------------------
// *SHAPE_LINE 읽기
//-----------------------------------------------------------------------------

bool ASEParser::Parsing_ShapeLine(SHAPE_LINE& _ShapeLine)
{
	bool isClosed = FALSE;

	size_t tSize=0;
	CHAR	TokenString[256];
//	TCHAR	wTokenString[256];

	LONG	token = m_pLexer->GetToken(TokenString);

	if(token == TOKEND_BLOCK_START)
	{

		token = m_pLexer->GetToken(TokenString);
		while(token != TOKEND_BLOCK_END)
		{
			switch(token)
			{
			case TOKERN_SHAPE_CLOSED://닫힌 라인인가?
					isClosed = true;
				break;

			case TOKERN_SHAPE_VERTEXCOUNT: //한 라인을 구성하는 정점 개수
				{
					m_pLexer->GetToken(TokenString);  _ShapeLine.Vert_Cnt = atoi(TokenString);
					if(isClosed == true)
					{
						_ShapeLine.Vert_Cnt+=1;
					}
					_ShapeLine.pVert_List = new D3DXVECTOR3[_ShapeLine.Vert_Cnt];
					ASSERT_IF_FAIL_MemAlloc(_ShapeLine.Vert_Cnt )
				}
				break;

			case TOKERN_SHAPE_VERTEX_KNOT:
				{	
					int idx;
					m_pLexer->GetToken(TokenString);  idx = atoi(TokenString);
					m_pLexer->GetToken(TokenString);  _ShapeLine.pVert_List[idx].x = (float)atof(TokenString);
					m_pLexer->GetToken(TokenString);  _ShapeLine.pVert_List[idx].z = (float)atof(TokenString);
					m_pLexer->GetToken(TokenString);  _ShapeLine.pVert_List[idx].y = (float)atof(TokenString);			

				}
				break;

			case TOKERN_SHAPE_VERTEX_INTERP:
				{
					int idx;
					m_pLexer->GetToken(TokenString);  idx = atoi(TokenString);
					m_pLexer->GetToken(TokenString);  _ShapeLine.pVert_List[idx].x = (float)atof(TokenString);
					m_pLexer->GetToken(TokenString);  _ShapeLine.pVert_List[idx].z = (float)atof(TokenString);
					m_pLexer->GetToken(TokenString);  _ShapeLine.pVert_List[idx].y = (float)atof(TokenString);

				}
				break;

			};

			token = m_pLexer->GetToken(TokenString);

		}

	}

	if(isClosed == TRUE) //닫힌 선분이면 마지막정점을 처음것과 같게 해준다.
	{
		 _ShapeLine.pVert_List[_ShapeLine.Vert_Cnt-1] =  _ShapeLine.pVert_List[0];
	}

	
	return true;
}


//---------------------------------------------------------------------------------------------
// TMani 정보 읽기 : GEOMOBJECT 전용
//---------------------------------------------------------------------------------------------

bool ASEParser::Parsing_TMAnimation(GEOMOBJECT* pGeoObjs)
{
	SCENE&	_S=pGeoObjs->pAseData->m_Scene;
	pGeoObjs->m_TMAni.FirstTick = _S.FirstFrame * _S.Ticks_Per_Frame;
	pGeoObjs->m_TMAni.LastTick  = _S.LastFrame  * _S.Ticks_Per_Frame;
	pGeoObjs->m_TMAni.LastTime  = (float)(_S.LastFrame * _S.FrameSpeed) ;
	pGeoObjs->m_TMAni.m_pScene = &pGeoObjs->pAseData->m_Scene;
			
	TMAnimation& _TMAni =  pGeoObjs->m_TMAni;
	int		TickPerSample = pGeoObjs->pAseData->m_Scene.Ticks_Per_Frame *5; //한샘플당 틱수,ex)192*5=960, 반드시 3dmax에서 뽑을때 5프레임당 한번씩 샘플링 해야한다. 
	_TMAni.TickPerFrame = pGeoObjs->pAseData->m_Scene.Ticks_Per_Frame 	;
	

	D3DXVECTOR3* TempRotvec = NULL;
	float		 TempAngle = 0;
	TempRotvec = new D3DXVECTOR3[_TMAni.FrameCnt]; //일시적으로 CONTROL_ROT_SAMPLE을 읽어들여 저장할 변수
	ASSERT_IF_FAIL_MemAlloc(TempRotvec)



	size_t tSize=0;
	CHAR	TokenString[256];
	//TCHAR	wTokenString[256];
	
	LONG	token = m_pLexer->GetToken(TokenString);

	if(token == TOKEND_BLOCK_START)
	{
		token = m_pLexer->GetToken(TokenString);
		while(token != TOKEND_BLOCK_END)
		{
			switch(token)
			{
			case TOKERN_CONTROL_POS_TRACK:
				{
					int CurTick=0;
					CONTROL_POS* pTempPos = NULL;
					pTempPos = new CONTROL_POS[_TMAni.FrameCnt];
					ASSERT_IF_FAIL_MemAlloc(pTempPos)
					int idx = 0; int idx2=0;

					token = m_pLexer->GetToken(TokenString);
					while(token != TOKEND_BLOCK_END)
					{	
						switch(token)
						{
							case TOKERN_CONTROL_POS_SAMPLE :
							{
								m_pLexer->GetToken(TokenString);	CurTick= atoi(TokenString);	
								if((CurTick != 0 ) && (idx == 0))//이경우는 첫번째 키프레임이 0이 아닌 경우이다. 0번프레임Pos값을 나중에 임의로 만들어준다.
								{

									_TMAni.isFirstPosKeyNZero = true;
									pTempPos[idx].Curtick = 0;
									pTempPos[idx].pos = D3DXVECTOR3(0,0,0);
									idx++;
									

								}
								else if(CurTick != (idx2*TickPerSample))
								{											
										pTempPos[idx].Curtick = CurTick-TickPerSample; pTempPos[idx].pos = pTempPos[idx-1].pos; 
										idx2 = CurTick/TickPerSample;
										idx++;							
								}
							
									pTempPos[idx].Curtick = CurTick;
									m_pLexer->GetToken(TokenString); 	pTempPos[idx].pos.x = (float)atof(TokenString);
									m_pLexer->GetToken(TokenString);	pTempPos[idx].pos.z = (float)atof(TokenString);
									m_pLexer->GetToken(TokenString); 	pTempPos[idx].pos.y = (float)atof(TokenString);
									
								idx2++;
								idx++;
							}
							break;
						}
						token = m_pLexer->GetToken(TokenString);
					}
					if(pTempPos[idx-1].Curtick !=  _TMAni.LastTick) 
					{
						pTempPos[idx].Curtick = _TMAni.LastTick; pTempPos[idx].pos = pTempPos[idx-1].pos;
						idx++;
						
					}

					_TMAni.pPos = new CONTROL_POS[idx];  _TMAni.PosSize = idx;
					ASSERT_IF_FAIL_MemAlloc(_TMAni.pPos )
					memcpy(_TMAni.pPos, pTempPos, sizeof(CONTROL_POS)*idx);
					SAFE_DELETE_ARR(pTempPos)
					LOG_WRITE_A("노드이름 : %s",pGeoObjs->NodeTM.Name);
					for(int i=0; i<idx ; i++)
					{
							  _TMAni.pPos[i].Curtick =   (_TMAni.pPos[i].Curtick/_TMAni.TickPerFrame * _S.FrameSpeed);
 							LOG_WRITE_A("pTempPos[%d] :(%f) <%f,  %f,  %f>", i,  _TMAni.pPos[i].Curtick ,  _TMAni.pPos[i].pos.x,  _TMAni.pPos[i].pos.y,  _TMAni.pPos[i].pos.z );
					}
				}
				break;

			case TOKERN_CONTROL_ROT_TRACK:
				{   
					int CurTick=0;
					CONTROL_ROT* pTempRot=NULL;
					pTempRot = new CONTROL_ROT[_TMAni.FrameCnt]; 
					ASSERT_IF_FAIL_MemAlloc(pTempRot);

					int idx = 0; int idx2=0;
					token = m_pLexer->GetToken(TokenString);
					while(token != TOKEND_BLOCK_END)
					{	switch(token)
						{
						case TOKERN_CONTROL_ROT_SAMPLE :
							{	

								m_pLexer->GetToken(TokenString);	CurTick= atoi(TokenString);
								if((CurTick != 0) && (idx == 0))
								{
									_TMAni.isFirstRotKeyNZero = true;							
									pTempRot[idx].Curtick = 0;
									pTempRot[idx].quat = D3DXQUATERNION(0,0,0,0);
									idx++;
									
								}
								else if(CurTick != (idx2*TickPerSample))
								{
										pTempRot[idx].Curtick = CurTick-TickPerSample;  TempRotvec[idx] =  TempRotvec[idx-1];	D3DXQuaternionIdentity(&pTempRot[idx].quat);
										idx2 = CurTick/TickPerSample;
										idx++;													
								}
								pTempRot[idx].Curtick =  CurTick;
								m_pLexer->GetToken(TokenString); 	TempRotvec[idx].x	= (float)atof(TokenString);
								m_pLexer->GetToken(TokenString); 	TempRotvec[idx].z	= (float)atof(TokenString);
								m_pLexer->GetToken(TokenString); 	TempRotvec[idx].y	= (float)atof(TokenString);
								m_pLexer->GetToken(TokenString); 	TempAngle = (float)atof(TokenString);

								D3DXQuaternionRotationAxis(&pTempRot[idx].quat , &TempRotvec[idx], TempAngle );
								
								idx2++;
								idx++;
							}
							break;
						}
						token = m_pLexer->GetToken(TokenString);
					}

					if(pTempRot[idx-1].Curtick !=  _TMAni.LastTick) 
					{
						pTempRot[idx].Curtick = _TMAni.LastTick; D3DXQuaternionIdentity(&pTempRot[idx].quat);
						
						idx++;
					}

					

					_TMAni.pRot = new CONTROL_ROT[idx];      _TMAni.RotSize = idx;
					ASSERT_IF_FAIL_MemAlloc(_TMAni.pRot )
					memcpy(_TMAni.pRot, pTempRot,  sizeof(CONTROL_ROT)*idx);

					SAFE_DELETE_ARR(pTempRot)
					LOG_WRITE_A("노드이름 : %s",pGeoObjs->NodeTM.Name);

					for(int i=0; i<idx ; i++)
					{		 
						_TMAni.pRot[i].Curtick =   (_TMAni.pRot[i].Curtick/_TMAni.TickPerFrame * _S.FrameSpeed);
						 LOG_WRITE_A("_TMAni.pRot[%d] :(%f) <%f,   %f,   %f,   %f>", i,  _TMAni.pRot[i].Curtick ,  _TMAni.pRot[i].quat.x,   _TMAni.pRot[i].quat.y,  _TMAni.pRot[i].quat.z,  _TMAni.pRot[i].quat.w);
					}
				}
				break;

			case TOKERN_CONTROL_SCALE_TRACK:
				{

					D3DXVECTOR3* TempScaleAxis = NULL;
					float		 TempSAngle = 0;
					TempScaleAxis = new D3DXVECTOR3[_TMAni.FrameCnt]; //일시적으로 CONTROL_ROT_SAMPLE을 읽어들여 저장할 변수
					ASSERT_IF_FAIL_MemAlloc(TempScaleAxis)

					int CurTick=0;
					CONTROL_SCALE* pTempScale = NULL;
					pTempScale = new CONTROL_SCALE[_TMAni.FrameCnt];

					int idx = 0; int idx2=0;
					token = m_pLexer->GetToken(TokenString);

					while(token != TOKEND_BLOCK_END)
					{	switch(token)
						{
						case TOKERN_CONTROL_SCALE_SAMPLE :
							{
								m_pLexer->GetToken(TokenString);	CurTick= atoi(TokenString);	
								if((CurTick != 0) && (idx == 0))
								{
									_TMAni.isFirstScaleKeyNZero = true;							
									pTempScale[idx].Curtick = 0;
									pTempScale[idx].scale = D3DXVECTOR3(1,1,1);
									pTempScale[idx].scale_Axis = D3DXVECTOR3(1,1,1);
									pTempScale[idx].angle = 0.0f;
									idx++;

								}
								else if(CurTick != (idx2*TickPerSample))
								{
									pTempScale[idx].Curtick = CurTick-TickPerSample;  pTempScale[idx] =  pTempScale[idx-1];	D3DXQuaternionIdentity(&pTempScale[idx].quat);
									idx2 = CurTick/TickPerSample;
									idx++;													
								}
								pTempScale[idx].Curtick	= CurTick	;
								m_pLexer->GetToken(TokenString); 			pTempScale[idx].scale.x	= (float)atof(TokenString);
								m_pLexer->GetToken(TokenString); 			pTempScale[idx].scale.z	= (float)atof(TokenString);
								m_pLexer->GetToken(TokenString); 			pTempScale[idx].scale.y	= (float)atof(TokenString);
								m_pLexer->GetToken(TokenString); 			TempScaleAxis[idx].x	= (float)atof(TokenString);
								m_pLexer->GetToken(TokenString); 			TempScaleAxis[idx].z	= (float)atof(TokenString);
								m_pLexer->GetToken(TokenString); 			TempScaleAxis[idx].y	= (float)atof(TokenString);
								m_pLexer->GetToken(TokenString); 			TempSAngle	= (float)atof(TokenString);

								D3DXQuaternionRotationAxis(&pTempScale[idx].quat , &TempScaleAxis[idx], TempSAngle );
								
								idx2++;
								idx++;
							}
							break;
						}
						token = m_pLexer->GetToken(TokenString);
					}

					if(pTempScale[idx-1].Curtick !=  _TMAni.LastTick) //마지막프레임이 없다면 마지막 프레임 추가
					{
						pTempScale[idx].Curtick = _TMAni.LastTick; pTempScale[idx].scale = pTempScale[idx-1].scale;  D3DXQuaternionIdentity(&pTempScale[idx].quat);
						idx++;
					}
						_TMAni.pScale = new CONTROL_SCALE[idx]; 
						_TMAni.ScaleSize = idx;
						ASSERT_IF_FAIL_MemAlloc(_TMAni.pScale )
						memcpy(_TMAni.pScale, pTempScale, sizeof(CONTROL_SCALE)*idx);

					SAFE_DELETE_ARR(TempScaleAxis)
					SAFE_DELETE_ARR(pTempScale)
					LOG_WRITE_A("노드이름 : %s",pGeoObjs->NodeTM.Name);
					for(int i=0; i<_TMAni.ScaleSize ; i++)
					{		 
						_TMAni.pScale[i].Curtick =   (_TMAni.pScale[i].Curtick/_TMAni.TickPerFrame * _S.FrameSpeed);
						LOG_WRITE_A("_TMAni.pScale[%d] :(%f) <%f,   %f,   %f,   %f>", i,  _TMAni.pScale[i].Curtick ,  _TMAni.pScale[i].quat.x,   _TMAni.pScale[i].quat.y,  _TMAni.pScale[i].quat.z,  _TMAni.pScale[i].quat.w);
					}
				}
				break;
		

			};

			token = m_pLexer->GetToken(TokenString);

		}
	}
	
	SAFE_DELETE_ARR(TempRotvec)
	return	TRUE;

}
//-----------------------------------------------------------------------------
// ASECameraData 전용 애니메이션 파싱함수
//-----------------------------------------------------------------------------

bool ASEParser::Parsing_TMAnimation_Camera(ASECameraData* _pCData)
{		ASECameraData tempC;
		if(_pCData == NULL)
		{
			_pCData = &tempC;
		}

		SCENE&	_S=_pCData->m_Scene;
		_pCData->m_TMAni.FirstTick = _S.FirstFrame * _S.Ticks_Per_Frame;
		_pCData->m_TMAni.LastTick  = _S.LastFrame  * _S.Ticks_Per_Frame;
		_pCData->m_TMAni.LastTime  = _S.LastFrame * _S.FrameSpeed ;
		_pCData->m_TMAni.m_pScene = &_pCData->m_Scene;  //이줄은 반드시 TMAnimation 함수에 있어야함

		TMAnimation& _TMAni =  _pCData->m_TMAni;
		int		TickPerSample = _pCData->m_Scene.Ticks_Per_Frame *5; //한샘플당 틱수,ex)192*5=960, 반드시 3dmax에서 뽑을때 5프레임당 한번씩 샘플링 해야한다. 
		_TMAni.TickPerFrame = _pCData->m_Scene.Ticks_Per_Frame 	;
		_TMAni.FrameCnt =  _S.LastFrame - _S.FirstFrame;

		D3DXVECTOR3* TempRotvec = NULL;
		float		 TempAngle = 0;
		TempRotvec = new D3DXVECTOR3[_TMAni.FrameCnt]; //일시적으로 CONTROL_ROT_SAMPLE을 읽어들여 저장할 변수
		ASSERT_IF_FAIL_MemAlloc(TempRotvec)

		size_t tSize=0;
		CHAR	TokenString[256];
		//TCHAR	wTokenString[256];

		LONG	token = m_pLexer->GetToken(TokenString);

		if(token == TOKEND_BLOCK_START)
		{
			token = m_pLexer->GetToken(TokenString);
			while(token != TOKEND_BLOCK_END)
			{
				switch(token)
				{
				case TOKERN_CONTROL_POS_TRACK:
					{
						int CurTick=0;
						CONTROL_POS* pTempPos = NULL;
						pTempPos = new CONTROL_POS[_TMAni.FrameCnt];
						ASSERT_IF_FAIL_MemAlloc(pTempPos)
							int idx = 0; int idx2=0;

						token = m_pLexer->GetToken(TokenString);
						while(token != TOKEND_BLOCK_END)
						{	
							switch(token)
							{
							case TOKERN_CONTROL_POS_SAMPLE :
								{
									m_pLexer->GetToken(TokenString);	CurTick= atoi(TokenString);	
									if((CurTick != 0 ) && (idx == 0))//이경우는 첫번째 키프레임이 0이 아닌 경우이다. 0번프레임Pos값을 나중에 임의로 만들어준다.
									{

										_TMAni.isFirstPosKeyNZero = true;
										pTempPos[idx].Curtick = 0;
										pTempPos[idx].pos = D3DXVECTOR3(0,0,0);
										idx++;

									}
									else if(CurTick != (idx2*TickPerSample))
									{											
										pTempPos[idx].Curtick = CurTick-TickPerSample; pTempPos[idx].pos = pTempPos[idx-1].pos; 
										idx2 = CurTick/TickPerSample;
										idx++;							
									}

									pTempPos[idx].Curtick = CurTick;
									m_pLexer->GetToken(TokenString); 	pTempPos[idx].pos.x = (float)atof(TokenString);
									m_pLexer->GetToken(TokenString);	pTempPos[idx].pos.z = (float)atof(TokenString);
									m_pLexer->GetToken(TokenString); 	pTempPos[idx].pos.y = (float)atof(TokenString);

									idx2++;
									idx++;
								}
								break;
							}
							token = m_pLexer->GetToken(TokenString);
						}
						if(pTempPos[idx-1].Curtick !=  _TMAni.LastTick) 
						{
							pTempPos[idx].Curtick = _TMAni.LastTick; pTempPos[idx].pos = pTempPos[idx-1].pos;
							idx++;

						}

						_TMAni.pPos = new CONTROL_POS[idx]; 
						ASSERT_IF_FAIL_MemAlloc(_TMAni.pPos )
						_TMAni.PosSize = idx;
						memcpy(_TMAni.pPos, pTempPos, sizeof(CONTROL_POS)*idx);
						SAFE_DELETE_ARR(pTempPos)
						LOG_WRITE_A("노드이름 : %s",_pCData->NodeTM.Name);
							for(int i=0; i<idx ; i++)
							{
								_TMAni.pPos[i].Curtick =   (_TMAni.pPos[i].Curtick/_TMAni.TickPerFrame * _S.FrameSpeed);
								LOG_WRITE_A("pTempPos[%d] :(%f) <%f,  %f,  %f>", i,  _TMAni.pPos[i].Curtick ,  _TMAni.pPos[i].pos.x,  _TMAni.pPos[i].pos.y,  _TMAni.pPos[i].pos.z );
							}
					}
					break;

				case TOKERN_CONTROL_ROT_TRACK:
					{   
						int CurTick=0;
						CONTROL_ROT* pTempRot=NULL;
						pTempRot = new CONTROL_ROT[_TMAni.FrameCnt]; 
						ASSERT_IF_FAIL_MemAlloc(pTempRot);

						int idx = 0; int idx2=0;
						token = m_pLexer->GetToken(TokenString);
						while(token != TOKEND_BLOCK_END)
						{	switch(token)
						{
						case TOKERN_CONTROL_ROT_SAMPLE :
							{	

								m_pLexer->GetToken(TokenString);	CurTick= atoi(TokenString);
								if((CurTick != 0) && (idx == 0))
								{
									_TMAni.isFirstRotKeyNZero = true;							
									pTempRot[idx].Curtick = 0;
									pTempRot[idx].quat = D3DXQUATERNION(0,0,0,0);
									idx++;

								}
								else if(CurTick != (idx2*TickPerSample))
								{
									pTempRot[idx].Curtick = CurTick-TickPerSample;  TempRotvec[idx] =  TempRotvec[idx-1];	D3DXQuaternionIdentity(&pTempRot[idx].quat);
									idx2 = CurTick/TickPerSample;
									idx++;													
								}
								pTempRot[idx].Curtick =  CurTick;
								m_pLexer->GetToken(TokenString); 	TempRotvec[idx].x	= (float)atof(TokenString);
								m_pLexer->GetToken(TokenString); 	TempRotvec[idx].z	= (float)atof(TokenString);
								m_pLexer->GetToken(TokenString); 	TempRotvec[idx].y	= (float)atof(TokenString);
								m_pLexer->GetToken(TokenString); 	TempAngle = (float)atof(TokenString);

								D3DXQuaternionRotationAxis(&pTempRot[idx].quat , &TempRotvec[idx], TempAngle );

								idx2++;
								idx++;
							}
							break;
						}
						token = m_pLexer->GetToken(TokenString);
						}

						if(pTempRot[idx-1].Curtick !=  _TMAni.LastTick) 
						{
							pTempRot[idx].Curtick = _TMAni.LastTick; D3DXQuaternionIdentity(&pTempRot[idx].quat);

							idx++;
						}



						_TMAni.pRot = new CONTROL_ROT[idx];      _TMAni.RotSize = idx;
						ASSERT_IF_FAIL_MemAlloc(_TMAni.pRot )
							memcpy(_TMAni.pRot, pTempRot,  sizeof(CONTROL_ROT)*idx);

						SAFE_DELETE_ARR(pTempRot)
						LOG_WRITE_A("노드이름 : %s", _pCData->NodeTM.Name);

						for(int i=0; i<idx ; i++)
						{		 
							_TMAni.pRot[i].Curtick =   (_TMAni.pRot[i].Curtick/_TMAni.TickPerFrame * _S.FrameSpeed);
							LOG_WRITE_A("_TMAni.pRot[%d] :(%f) <%f,   %f,   %f,   %f>", i,  _TMAni.pRot[i].Curtick ,  _TMAni.pRot[i].quat.x,   _TMAni.pRot[i].quat.y,  _TMAni.pRot[i].quat.z,  _TMAni.pRot[i].quat.w);
						}
					}
					break;

				case TOKERN_CONTROL_SCALE_TRACK:
					{

						D3DXVECTOR3* TempScaleAxis = NULL;
						float		 TempSAngle = 0;
						TempScaleAxis = new D3DXVECTOR3[_TMAni.FrameCnt]; //일시적으로 CONTROL_ROT_SAMPLE을 읽어들여 저장할 변수
						ASSERT_IF_FAIL_MemAlloc(TempScaleAxis)

							int CurTick=0;
						CONTROL_SCALE* pTempScale = NULL;
						pTempScale = new CONTROL_SCALE[_TMAni.FrameCnt];

						int idx = 0; int idx2=0;
						token = m_pLexer->GetToken(TokenString);

						while(token != TOKEND_BLOCK_END)
						{	switch(token)
						{
						case TOKERN_CONTROL_SCALE_SAMPLE :
							{
								// 								m_pLexer->GetToken(TokenString);	CurTick= atoi(TokenString);	
								// 								if(CurTick != (idx2*TickPerSample))
								// 								{
								// 									pTempScale[idx].Curtick = CurTick-TickPerSample; pTempScale[idx].scale = pTempScale[idx-1].scale; 
								// 									D3DXQuaternionIdentity(&pTempScale[idx].quat); 
								// 									LOG_WRITE_A("pTempScale[%d] :(%f) <%f,  %f,  %f>", idx,  pTempScale[idx].Curtick , pTempScale[idx].scale.x, pTempScale[idx].scale.y , pTempScale[idx].scale.z );
								// 									idx2 = CurTick/TickPerSample;
								// 									idx++;
								// 								}
								// 								pTempScale[idx].Curtick	= CurTick	;
								// 								m_pLexer->GetToken(TokenString); 			pTempScale[idx].scale.x	= (float)atof(TokenString);
								// 								m_pLexer->GetToken(TokenString); 			pTempScale[idx].scale.z	= (float)atof(TokenString);
								// 								m_pLexer->GetToken(TokenString); 			pTempScale[idx].scale.y	= (float)atof(TokenString);
								// 								m_pLexer->GetToken(TokenString); 			TempScaleAxis[idx].x	= (float)atof(TokenString);
								// 								m_pLexer->GetToken(TokenString); 			TempScaleAxis[idx].z	= (float)atof(TokenString);
								// 								m_pLexer->GetToken(TokenString); 			TempScaleAxis[idx].y	= (float)atof(TokenString);
								// 								m_pLexer->GetToken(TokenString); 			TempSAngle	= (float)atof(TokenString);
								// 
								// 								D3DXQuaternionRotationAxis(&pTempScale[idx].quat , &TempScaleAxis[idx], TempSAngle );
								// 								LOG_WRITE_A("pTempScale[%d] :(%f) <%f,  %f,  %f>", idx,  pTempScale[idx].Curtick , pTempScale[idx].scale.x, pTempScale[idx].scale.y , pTempScale[idx].scale.z );
								// 								idx2++;
								// 								idx++;
							}
							break;
						}
						token = m_pLexer->GetToken(TokenString);
						}
						if(pTempScale[idx].Curtick !=  _TMAni.LastTick) 
						{
							pTempScale[idx].Curtick = _TMAni.LastTick; pTempScale[idx].scale = pTempScale[idx-1].scale;  D3DXQuaternionIdentity(&pTempScale[idx].quat);
							LOG_WRITE_A("pTempScale[%d] :(%f) <%f,  %f,  %f>", idx,   pTempScale[idx].Curtick , pTempScale[idx].scale.x,  pTempScale[idx].scale.y,  pTempScale[idx].scale.z );
						}

						for(int i=1; i< _TMAni.FrameCnt ; i++)
						{
							D3DXQuaternionMultiply(&pTempScale[i].quat, &pTempScale[i].quat, &pTempScale[i-1].quat);
						}

						_TMAni.pScale = new CONTROL_SCALE[idx]; 
						_TMAni.ScaleSize = idx;
						ASSERT_IF_FAIL_MemAlloc(_TMAni.pScale )
							memcpy(_TMAni.pScale, pTempScale, sizeof(CONTROL_SCALE)*idx);

						SAFE_DELETE_ARR(TempScaleAxis)
					}
					break;


				};

				token = m_pLexer->GetToken(TokenString);

			}
		}

		SAFE_DELETE_ARR(TempRotvec)
			return	TRUE;
}

//---------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------

bool ASEParser::Parsing_CameraSetting(ASECameraData* _pCData)
{
	if(_pCData == NULL)
	{	LOG_WRITE_A("Parsing_CameraSetting 함수의 인자값이 NULL임!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		return FALSE;
	}
	size_t tSize=0;
	CHAR	TokenString[256];
	//TCHAR	wTokenString[256];

	LONG	token = m_pLexer->GetToken(TokenString);

	if(token == TOKEND_BLOCK_START)
	{
		token = m_pLexer->GetToken(TokenString);
		while(token != TOKEND_BLOCK_END)
		{
			switch(token)
			{
			case TOKERN_CAMERA_FOV:
				{   
					token = m_pLexer->GetToken(TokenString);  _pCData->m_FOV = (float)atof(TokenString);
				}
				break;	


			};

			token = m_pLexer->GetToken(TokenString);

		}
	}


		return	TRUE;
}

/****************************************************** End *****************************************************/