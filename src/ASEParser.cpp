//*****************************************************************************
// 2014. 06. 23
// ASE �ļ� Ŭ���� 
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
//�Ϲ��Լ�
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------
// ������ �迭�� ������ �� �������ش�.
//-------------------------------------------------------------
void pstrcpy(wchar_t** s, wchar_t* d) {
	int len = (int)wcslen(d);
	if (len) { 
		(*s) = new wchar_t[sizeof(wchar_t) * len];
				wcscpy_s((*s),sizeof(wchar_t) * len ,d);
	}
}

//-------------------------------------------------------------
// �յ� ����ǥ�� �����ش�.
//-------------------------------------------------------------
void removeQmarks(wchar_t *str)
{
	size_t size   = wcslen(str);
	wchar_t* temp = str;
	// "��" ����ǥ ���� (�տ� ���� �������� ������츦 ���� ������ ��°�� �˻縦 ���Ѵ�.)
	while (size-1 > 0) {
		if ((*temp)==L'"') {
			wcscpy_s(str, sizeof(wchar_t) *size ,temp+(wcslen(str)-size));
			break; 
		}
		temp++;
		size--;
	}
	// "��" ����ǥ ���� (���ڿ��� ������ ��°�� �˻�)
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
// ���⼭���� �ļ��Լ� ���� ����
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
// �ļ� ������, �Ҹ���
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
// �ε��Լ�, ase ���ϰ�ο� aseData�� ���ڷ� �޴´�.
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
		
			case	TOKERN_SCENE:		//���׸��� ����Ʈ �Ľ�	
					{	
						Parsing_Scene(_pAseMesh);
					}
			break;

			case	TOKERN_MATERIAL_LIST:		//���׸��� ����Ʈ �Ľ�	
					{	
						Parsing_Material_List(_pAseMesh);
					}
				break;

			case	TOKERN_GEOMOBJECT:			
					{	
						gObjs=NULL;
						gObjs = new GEOMOBJECT(); //aseData�� m_pGeoObjs�� �޸� �Ҵ����ش�.
						ASSERT_IF_FAIL_MemAlloc(gObjs)  //�޸� �Ҵ������ ��� ���Ʈ

						gObjs->m_TMAni.FrameCnt  =  _pAseMesh->m_Scene.LastFrame  -  _pAseMesh->m_Scene.FirstFrame;
						gObjs->pAseData  =  _pAseMesh;
						Parsing_GeoObject(gObjs); //������ �о�鿩 ������ ��..

						_pAseMesh->m_GA_GeoObject.Add(gObjs); //GrowableArray�� ����ش�.					
					}
				break;

			case    TOKERN_HELPEROBJECT:
					{
						gObjs=NULL;
						gObjs = new HELPEROBJECT(); //aseData�� m_pGeoObjs�� �޸� �Ҵ����ش�.
						ASSERT_IF_FAIL_MemAlloc(gObjs)  //�޸� �Ҵ������ ��� ���Ʈ

						gObjs->m_TMAni.FrameCnt = _pAseMesh->m_Scene.LastFrame - _pAseMesh->m_Scene.FirstFrame;
						gObjs->pAseData = _pAseMesh;
						Parsing_HerperObject(gObjs);

						_pAseMesh->m_GA_GeoObject.Add(gObjs); //GrowableArray�� ����ش�.	
					}
				break;

			case    TOKERN_SHAPEOBJECT:
				{
					gObjs=NULL;
					gObjs = new SHAPEOBJECT(); //aseData�� m_pGeoObjs�� �޸� �Ҵ����ش�.
					ASSERT_IF_FAIL_MemAlloc(gObjs)  //�޸� �Ҵ������ ��� ���Ʈ
											
					gObjs->m_TMAni.FrameCnt = _pAseMesh->m_Scene.LastFrame - _pAseMesh->m_Scene.FirstFrame;
					gObjs->pAseData = _pAseMesh;
					Parsing_ShapeObject(gObjs);

					_pAseMesh->m_GA_GeoObject.Add(gObjs); //GrowableArray�� ����ش�.	
				}
				break;

			case TOKERN_CAMERAOBJECT:
				{
					if(_PCamera !=NULL )
					{	
						_PCamera->GetAseCData()->m_Scene = _pAseMesh->m_Scene;
						Parsing_CameraObject(_PCamera->GetAseCData()); //ī�޶� ���� �б�
					}
					
				}
				break;

		};

		token = m_pLexer->GetToken(TokenString);
	}

	//��������ȭ
	Make_Hierarchy(_pAseMesh);
	Cal_GeoObjs_LocalTM(_pAseMesh);//����TM ���ϱ�
	MakeEmptyKeyFrame(_pAseMesh);//  0��Ű�������� ������� ä���־��ش�.

	if(pTempBoneList != NULL)
	{	
		MakeBoneList(_pAseMesh); //��Ʈ�����ȷ�Ʈ�� ������ٶ� ����� ������Ʈ ����
	}

	return	0;
}

//-----------------------------------------------------------------------------
// ��Ʈ�����ȷ�Ʈ�� ������ٶ� ����� ������Ʈ ����
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
					LOG_WRITE_A("  [BoneList %d��] -> %s" , i , pCurObj->NodeTM.Name);
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
					
				_pAseMesh->m_SkinnedMesh->DeepCopy(*pCurObj);  //��Ű�������� �ִ� GeoObject�� ���� ���ش�. ��������
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
		{	//��Ʈ�����ȷ�Ʈ
			GEOMOBJECT* pCurObj = _pAseMesh->m_GA_GeoObject.GetAt(i);
			pCurObj->m_BoneIniMat = pCurObj->NodeTM.NodeTM * SkinMeshInv;// ���̴ϼ�Mat = ���� NodeTM * skinMesh�� NodeTM�� �����
			D3DXMatrixInverse(&pCurObj->m_BoneIniMat , NULL, &pCurObj->m_BoneIniMat);

		}
		m_BoneCnt = 0;

		SAFE_DELETE_ARR(pTempBoneList)
	}
	
}

//---------------------------------------------------------------------------------------------
//   Geoobject�� ��������ȭ ��Ű��
//---------------------------------------------------------------------------------------------

void ASEParser::Make_Hierarchy(ASEMesh* _pAseMesh)
{
	
	int logidx=0;

/*	char Cname[256] , Pname[256];*/
	GEOMOBJECT *pChild, *pParent;
	//����ó��
	if(_pAseMesh->m_GA_GeoObject.GetSize() == 0 )
	{
		MessageBox(NULL,TEXT("geoObject�� �ϳ��� �����ϴ�."), TEXT("Failed"), MB_OK);   assert(false);
	}
	

	for(int i= 0; i <_pAseMesh->m_GA_GeoObject.GetSize()  ; i++)
	{	
		pChild=_pAseMesh->m_GA_GeoObject.GetAt(i);
		TMAnimation &_TMANI = pChild->m_TMAni;

		CHAR* parentName = pChild->NodeTM.ParentName;
		
		if(strlen(parentName) == 0) //�θ� ���ٸ�
		{	
			LOG_WRITE_A( "   %d��GeoObject : %s , �θ����̸� : %s ",i,pChild->NodeTM.Name, parentName);
			pChild->m_pParent = NULL;	

			continue;
		}

		for(int j=0 ; j<_pAseMesh->m_GA_GeoObject.GetSize() ; j++)
		{
			pParent = _pAseMesh->m_GA_GeoObject.GetAt(j);
			CHAR* Name = pParent->NodeTM.Name;

			if(strlen(Name) == 0)
			{
				write_log_file("ASEParser::Make_Hierarchy(ASEMesh* _pAseMesh) , Name �� NULL�Դϴ�.");
				assert(false);			
			}

			if( strcmp(Name, pChild->NodeTM.ParentName) == 0 )//�θ� ã�Ҵٸ�!!
			{					
				LOG_WRITE_A( "   %d��GeoObject : %s , �θ����̸� : %s ",i,pChild->NodeTM.Name, parentName);
				pChild->m_pParent =  pParent;
				break;
			}
		}
	}

	


}

//-----------------------------------------------------------------------------
// �� GeoObject���� ����TM���ϱ�,, 
// �θ��������� �ʿ��ϱ� ������ �θ��带 ������ �� �Լ��� �����ؾ� �Ѵ�.
//-----------------------------------------------------------------------------

int ASEParser::Cal_GeoObjs_LocalTM(ASEMesh* _pAseMesh)
{

	GEOMOBJECT *pChild, *pParent;

	for(int i= 0; i <_pAseMesh->m_GA_GeoObject.GetSize()  ; i++)
	{
		pChild=_pAseMesh->m_GA_GeoObject.GetAt(i);
		TMAnimation &_TMANI = pChild->m_TMAni;
		CHAR* parentName = pChild->NodeTM.ParentName;

		if(strlen(parentName) == 0) //�θ� ���ٸ�
		{
			//����TM ���ϱ�
			pChild->m_MLocalTM = pChild->NodeTM.NodeTM;			
			//���⼭���� Decompose

			D3DXQUATERNION QRot;
			D3DXVECTOR3 scaleVec, transVec;
			D3DXMatrixDecompose(&scaleVec, &QRot,&transVec, &pChild->m_MLocalTM );

			D3DXMatrixTranslation(&pChild->m_LTM_Pos,transVec.x, transVec.y, transVec.z);
			D3DXMatrixRotationQuaternion(&pChild->m_LTM_Rot,&QRot);
			D3DXMatrixScaling(&pChild->m_LTM_scale, scaleVec.x, scaleVec.y, scaleVec.z );

			LOG_WRITE_A( "����̸� %s , m_MLocalTM:11   <%f,  %f,  %f> ", pChild->NodeTM.Name, pChild->m_MLocalTM._11, pChild->m_MLocalTM._12, pChild->m_MLocalTM._13 );
			LOG_WRITE_A( "����̸� %s , m_MLocalTM:21   <%f,  %f,  %f> ", pChild->NodeTM.Name, pChild->m_MLocalTM._21,  pChild->m_MLocalTM._22,  pChild->m_MLocalTM._23);
			LOG_WRITE_A( "����̸� %s , m_MLocalTM:31   <%f,  %f,  %f> ", pChild->NodeTM.Name, pChild->m_MLocalTM._31,  pChild->m_MLocalTM._32,  pChild->m_MLocalTM._33);
			LOG_WRITE_A( "����̸� %s , m_MLocalTM:41   <%f,  %f,  %f> ", pChild->NodeTM.Name, pChild->m_MLocalTM._41,  pChild->m_MLocalTM._42,  pChild->m_MLocalTM._43);
			
			continue;
		}
		//��������
		for(int j=0 ; j<_pAseMesh->m_GA_GeoObject.GetSize() ; j++)
		{
			pParent = _pAseMesh->m_GA_GeoObject.GetAt(j);
			CHAR* Name = pParent->NodeTM.Name;

			if(strlen(Name) == 0)
			{
				write_log_file("ASEParser::Make_Hierarchy(ASEMesh* _pAseMesh) , Name �� NULL�Դϴ�.");
				assert(false);			
			}

			if( strcmp(Name, pChild->NodeTM.ParentName) == 0 )//�θ� ã�Ҵٸ�!!
			{					
				D3DXMATRIX InvP;//�θ��� ������� ����� ����
				D3DXMatrixInverse(&InvP,NULL, &pChild->m_pParent->NodeTM.NodeTM); //�θ����� ���TM�� ������� ���Ѵ�.
				pChild->m_MLocalTM = pChild->NodeTM.NodeTM * InvP; //�ڽĳ�� * �θ����� ����� = �ڽ��� ����TM

				//���⼭���� Decompose
				D3DXQUATERNION QRot;
				D3DXVECTOR3 scaleVec, transVec;
				D3DXMatrixDecompose(&scaleVec, &QRot,&transVec, &pChild->m_MLocalTM );//����TM�� decompose�Ѵ�.
				//decompose�� ������ ���� �ش��ϴ� ������ķ� ������ش�.
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
//  0��Ű�������� ������� ä���־��ش�.
//---------------------------------------------------------------------------------------------

void ASEParser::MakeEmptyKeyFrame(ASEMesh* _pAseMesh)
{
	GEOMOBJECT *pCurGeoObject;
	for(int i= 0; i <_pAseMesh->m_GA_GeoObject.GetSize()  ; i++)
	{
		pCurGeoObject=_pAseMesh->m_GA_GeoObject.GetAt(i);
		// ù��° Ű���� 0�� �ƴҶ� 0���� ������ش�.
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

		//Rotate �ִϸ��̼� ���������ֱ�
		if(pCurGeoObject->m_TMAni.pRot != NULL)
		{
			for(int i=1; i< pCurGeoObject->m_TMAni.RotSize ; i++)
			{
				D3DXQuaternionMultiply(&pCurGeoObject->m_TMAni.pRot[i].quat,  &pCurGeoObject->m_TMAni.pRot[i-1].quat,  &pCurGeoObject->m_TMAni.pRot[i].quat );	
			}
		}
		//scale axis �ִϸ��̼� ���������ֱ�
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
// Scene������ �Ľ��Ѵ�.
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
					 m_pLexer->GetToken(TokenString); mCnt = atoi(TokenString);//���׸��� �� ����

					if(mCnt <=0)
					{		return false;					}// �Ѱ����� 0���������� ���׸����� ���°��̹Ƿ� ����
				}
				break;

			case TOKERN_MATERIAL:
				{
					 if(mCnt>0)
					 {	
						 ptempM =NULL;
						 ptempM = new MATERIAL(); //�Ͻ����� ���׸���ü�� ����
						 ASSERT_IF_FAIL_MemAlloc(ptempM)  //�޸� ���� ����
							
						m_pLexer->GetToken(TokenString);   ptempM->_mtrl_ID = atoi(TokenString);
						 Parsing_Material(*ptempM);	//������ �о���� ��,,

						 _pAseMesh->m_GA_Mtrl.Insert(ptempM->_mtrl_ID, ptempM);  //CGrowableArray�� ����ش�.
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
						mbstowcs_s(&tSize,wTokenString,strlen(TokenString)+1, TokenString, _TRUNCATE); // mbcs���ڿ��� ������ ����Ʈ ����ŭ wbcs���ڿ��� ��ȯ
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

				case TOKERN_MAP_DIFFUSE://��ǻ��� �Ľ�
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

				case TOKERN_NUMSUBMTLS://������׸��� ����
					{
							m_pLexer->GetToken(TokenString);	_Mtrl._SubMtrlCnt = atoi(TokenString);
					}	
					break;

				case TOKERN_SUBMATERIAL://������׸��� �Ľ�
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
					mbstowcs_s(&tSize,wTokenString,strlen(TokenString)+1, TokenString, _TRUNCATE); // mbcs���ڿ��� ������ ����Ʈ ����ŭ wbcs���ڿ��� ��ȯ
					pstrcpy(&(r_DMap.pClass),wTokenString);
				}
				break;

			case TOKERN_BITMAP:
				{
					m_pLexer->GetToken(TokenString); //
					mbstowcs_s(&tSize,wTokenString,strlen(TokenString)+1, TokenString, _TRUNCATE); // mbcs���ڿ��� ������ ����Ʈ ����ŭ wbcs���ڿ��� ��ȯ
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

			case TOKERN_NODE_PARENT: //�θ� �̸�

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
// *SHAPEOBJECT �Ľ�
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

			case TOKERN_NODE_PARENT: //�θ� �̸�

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
						_pShapeObjs->pSLine = new SHAPE_LINE[_pShapeObjs->m_LineCnt]; //���ΰ�����ŭ ��������
						ASSERT_IF_FAIL_MemAlloc(_pShapeObjs->pSLine)
					}
				}
				break;

			case TOKERN_SHAPE_LINE:
				{	int idx;
					m_pLexer->GetToken(TokenString);  idx = atoi(TokenString);
					Parsing_ShapeLine(_pShapeObjs->pSLine[idx]);
					//��� ���ؽ���ǥ�� �븻�� NodeTM�� ������� �����־� Local��ǥ�� �����.
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
//  *HELPEROBJECT �Ľ�
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

			case TOKERN_NODE_PARENT: //�θ� �̸�

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
// CameraObject �Ľ�
//---------------------------------------------------------------------------------------------

bool ASEParser::Parsing_CameraObject(ASECameraData* _pCData)
{
	size_t tSize=0;
	CHAR	TokenString[256];
//	TCHAR	wTokenString[256];	
	//targetnode�� �Ľ����� �ʱ����� �÷���
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

			case TOKERN_NODE_PARENT: //�θ� �̸�

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
	//����TM ���ϱ�
	_pCData->m_MLocalTM = _pCData->NodeTM.NodeTM;
	//���⼭���� Decompose

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
// GeomObject�� ��ƮTm�� �о���δ�.
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
			case TOKERN_NODE_NAME://��� �̸�

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
	/* //�װ�Ƽ�꽺���� ���Ե� ���TM
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

	//�װ�Ƽ�꽺������ ���Ծȵ� ���TM�� ����� ���� �ڵ�
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
// ī�޶�� ���TM �б�
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
			case TOKERN_NODE_NAME://��� �̸�

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

	D3DXMatrixRotationX(&RotTM,D3DX_PI/2.0f); //x������ 90�� ȸ��
	_pCData->NodeTM.NodeTM = RotTM* _pCData->NodeTM.NodeTM ;
	
	return	TRUE;
}

//////////////////////////////////////////////////////////////////////////
//

bool ASEParser::Parsing_Mesh(GEOMOBJECT* pGeoObjs)
{
	
	int Norm_Cnt = 0; //�븻�� ������ ����� index
	int Vert_idx = 0; //���ؽ� ���ؽ�

	// �о���ΰ��� ��� ������ ���� ����
	
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
// 							GEOMOBJECT* preGObjs= pGeoObjs; //����  pGeoObjs�� �����͸� ��� ����(�޸𸮸� �ȳ���)
// 
// 							pGeoObjs = NULL;
// 							pGeoObjs = new SKINNED_OBJECT(); //pGeoObjs�� SKINNED_OBJECT��ü�� �ٽ� �����Ѵ�.
// 							ASSERT_IF_FAIL_MemAlloc(pGeoObjs)  //�޸� �Ҵ������ ��� ���Ʈ
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
				
				case TOKERN_MESH_NUMVERTEX://���ؽ� ����

					m_pLexer->GetToken(TokenString);
					pGeoObjs->num_vertex = atoi(TokenString); //���ؼ� ���� ���
			
					break;

				case TOKERN_MESH_NUMTVERTEX:
					{
						int TvertexCnt;
						m_pLexer->GetToken(TokenString);  TvertexCnt = atoi(TokenString);
						if(TvertexCnt>0)
						{
						  pGeoObjs->ptVertex = new D3DXVECTOR3[TvertexCnt];//ptVertex[]�� ���߿� createmesh �Լ����� �޸�����
						  ASSERT_IF_FAIL_MemAlloc(pGeoObjs->ptVertex)
						}
					

					}
					break;

				case TOKERN_MESH_NUMFACES: //�ﰢ�� ���� 
					{		
						m_pLexer->GetToken(TokenString);
						pGeoObjs->num_faces = atoi(TokenString);
						// ���⼭ �ϴ� �־��� ��츦 ����� �ε������� * 3�� ������ŭ ���ؽ��� ���� ������ �����.
						pGeoObjs->pVertex= new D3DXVECTOR3[pGeoObjs->num_faces*3];
						ASSERT_IF_FAIL_MemAlloc(pGeoObjs->pVertex)
						// ���⼭ �ϴ� �־��� ��츦 ����� �ε������� * 3�� ������ŭ �븻�� ���� ���͹迭�� �����.
						pGeoObjs->num_pNormalCnt = pGeoObjs->num_faces*3;
						pGeoObjs->pNormal = new D3DXVECTOR3[pGeoObjs->num_pNormalCnt];
						ASSERT_IF_FAIL_MemAlloc(pGeoObjs->pNormal)
						memset(pGeoObjs->pNormal,0.0f,sizeof(D3DXVECTOR3)*pGeoObjs->num_faces*3);

						pGeoObjs->pUV = new UV_COORD[pGeoObjs->num_faces*3];//createmesh �Լ����� �޸� ����
						ASSERT_IF_FAIL_MemAlloc(pGeoObjs->pUV)
						memset(pGeoObjs->pUV,0,sizeof(UV_COORD)*pGeoObjs->num_faces*3);//ó���� �� 0���� �ʱ�ȭ

						pGeoObjs->p_I_N_MtID = new Temp_idx_Norm_mtID[pGeoObjs->num_faces*3];
						ASSERT_IF_FAIL_MemAlloc(pGeoObjs->p_I_N_MtID)

						pGeoObjs->pFace_List = new FACE_LIST[pGeoObjs->num_faces];
						ASSERT_IF_FAIL_MemAlloc(pGeoObjs->pFace_List)

						if(pGeoObjs->m_isSkinned == TRUE) //��Ű�� �޽����?
						{
							pGeoObjs->pVertWeightList = new VWeight_List[pGeoObjs->num_faces*3];//����Ʈ�������� �޸��Ҵ�
							ASSERT_IF_FAIL_MemAlloc(pGeoObjs->pVertWeightList)
							memset(pGeoObjs->pVertWeightList ,0 , sizeof(VWeight_List)*pGeoObjs->num_faces*3);//ó���� �� 0���� �ʱ�ȭ

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
									case TOKERN_MESH_VERTEX://���ؽ����� �о���̱�

										m_pLexer->GetToken(TokenString);
										Vert_idx = atoi(TokenString);

										m_pLexer->GetToken(TokenString);
										pGeoObjs->pVertex[Vert_idx].x = (float)atof(TokenString);//���ؽ� x�� ���
										m_pLexer->GetToken(TokenString);
										pGeoObjs->pVertex[Vert_idx].z = (float)atof(TokenString);//���ؽ� z�� ���
										m_pLexer->GetToken(TokenString);
										pGeoObjs->pVertex[Vert_idx].y = (float)atof(TokenString);//���ؽ� y�� ���

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
						
																				  // �޽� ����� ���� �޸�����
						ASSERT_IF_FAIL_MemAlloc(pGeoObjs->ptVertex) 
						//if(pGeoObjs->ptVertex == NULL){write_log_file("�޸� �Ҵ� ���� : pGeoObjs->ptVertex ");assert(true);}
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
						pGeoObjs->ptFace = new int[pGeoObjs->num_faces*3]; //createmesh �Լ����� �޸� ����
						if(pGeoObjs->ptFace == NULL){write_log_file("�޸� �Ҵ� ���� : pGeoObjs->ptFace ");}

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

									m_pLexer->GetToken(TokenString);//�ε����б�
									pGeoObjs->p_I_N_MtID[Norm_Cnt].idx	   = atoi(TokenString);
									m_pLexer->GetToken(TokenString);
									pGeoObjs->p_I_N_MtID[Norm_Cnt].Norm.x = (float)atof(TokenString);// x �� ���
									m_pLexer->GetToken(TokenString);
									pGeoObjs->p_I_N_MtID[Norm_Cnt].Norm.z = (float)atof(TokenString);// z �� ���
									m_pLexer->GetToken(TokenString);
									pGeoObjs->p_I_N_MtID[Norm_Cnt].Norm.y = (float)atof(TokenString);// y �� ���

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

												pGeoObjs->pVertWeightList[idx].insert(tempVW); //���Ϳ� �־��ش�.
											
											}
											break;
										}
										token = m_pLexer->GetToken(TokenString);
									}

						/*%%*/	pGeoObjs->pVertWeightList[idx].Sort_By_Weight(); //����Ʈ�������� �˾Ƽ� ���������� �Ǵ� �Լ�
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
	//�ڡڡ� ���ؽ��� ���� idx�ε� �븻�� �ٸ� ��� ���ؽ� ������ �ֱ� , Norm_Cnt = face*3 �ڡڡ�
	// pGeoObjs->p_I_N_MtID[] ���� *MESH_VERTEXNORMAL[...] ������ ����ִ�.(*MESH_FACENORMAL �� ����) 
	int vidx=0; //���ؽ� �ε���

	for(int i=0; i<Norm_Cnt ; i++)
	{
		vidx = pGeoObjs->p_I_N_MtID[i].idx;

		if(pGeoObjs->pNormal[vidx] == g_ZeroVec3 )// �븻�� 0, 0, 0 �̶�� ����̹Ƿ� �־��ش�.
		{	
			pGeoObjs->pNormal[vidx] = pGeoObjs->p_I_N_MtID[i].Norm;
			//�̶� U,V���� ���� �־��ش�. 
			if(pGeoObjs->ptFace !=NULL) //uv���� �ִٸ�??
			{
				pGeoObjs->pUV[vidx]._u = pGeoObjs->ptVertex[pGeoObjs->ptFace[i]].x;//U
				pGeoObjs->pUV[vidx]._v = pGeoObjs->ptVertex[pGeoObjs->ptFace[i]].y;//V
			}
		
		}
		else if(( pGeoObjs->pNormal[vidx] == pGeoObjs->p_I_N_MtID[i].Norm) && (pGeoObjs->ptFace != NULL))  
		{		/*������ �ڡڡڳ븻�� ���ؽ���ǥ�� ������ UV��ǥ�� �ٸ� ���ڡڡڰ� �����Ƿ�(multi-sub ���׸����ǰ��!!) 	
				       �׷���� �� ���ؽ��� ���� �߰��� ��� �Ѵ�.  */			
		
				if( (pGeoObjs->ptVertex[pGeoObjs->ptFace[i]].x != pGeoObjs->pUV[vidx]._u) ||
					(pGeoObjs->ptVertex[pGeoObjs->ptFace[i]].y != pGeoObjs->pUV[vidx]._v)
				  )
				{
					pGeoObjs->pVertex[pGeoObjs->num_vertex] = pGeoObjs->pVertex[vidx];//���ؽ� ���� �������� ���ؽ� ����(�߰�)
					pGeoObjs->pNormal[pGeoObjs->num_vertex] = pGeoObjs->p_I_N_MtID[i].Norm;// �븻�迭�������� �߰�
					
					if(pGeoObjs->m_isSkinned == true)
					pGeoObjs->pVertWeightList[pGeoObjs->num_vertex] = pGeoObjs->pVertWeightList[vidx]; //���ؽ�����Ʈ�� �������� ����(�߰�)

					pGeoObjs->pUV[pGeoObjs->num_vertex]._u = pGeoObjs->ptVertex[pGeoObjs->ptFace[i]].x;//U	//U,V�� �־��ֱ�
					pGeoObjs->pUV[pGeoObjs->num_vertex]._v = pGeoObjs->ptVertex[pGeoObjs->ptFace[i]].y;//V	//U,V�� �־��ֱ�
					
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
		else if(pGeoObjs->pNormal[vidx] != pGeoObjs->p_I_N_MtID[i].Norm) //���ؽ� idx�� ������ �븻�� �ٸ����
		{	
			pGeoObjs->pVertex[pGeoObjs->num_vertex] = pGeoObjs->pVertex[vidx];//���ؽ� ���� �������� ���ؽ� ����(�߰�)
			
			if(pGeoObjs->m_isSkinned == true)//��Ų�� �޽����
			pGeoObjs->pVertWeightList[pGeoObjs->num_vertex] = pGeoObjs->pVertWeightList[vidx]; //���ؽ�����Ʈ�� �������� ����(�߰�)
			
			pGeoObjs->pNormal[pGeoObjs->num_vertex] = pGeoObjs->p_I_N_MtID[i].Norm;// �븻�迭�������� �߰�
			//U,V�� �־��ֱ�
			if(pGeoObjs->ptFace !=NULL) //uv���� �ִٸ�??
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
	//�ε��� ���� �����
	 pGeoObjs->num_pIndice= pGeoObjs->num_faces*3;
	 pGeoObjs->p_indices = new WORD[	pGeoObjs->num_pIndice];
	
	 ASSERT_IF_FAIL_MemAlloc(pGeoObjs->p_indices )
	 for(int i=0; i<pGeoObjs->num_faces*3; i++)
	 {
	    pGeoObjs->p_indices[i] = pGeoObjs->p_I_N_MtID[i].idx; //�ε��� ����
	 }

	//face_list �����
	 
	for(int i=0; i<pGeoObjs->num_faces; i++)
	{
		pGeoObjs->pFace_List[i].idx_1 = pGeoObjs->p_I_N_MtID[i*3].idx; //�ε��� ����
		pGeoObjs->pFace_List[i].idx_2 = pGeoObjs->p_I_N_MtID[i*3+1].idx; //�ε��� ����
		pGeoObjs->pFace_List[i].idx_3 = pGeoObjs->p_I_N_MtID[i*3+2].idx; //�ε��� ����
	}

	//////////////////////////////////////////////////////////////////////////
	//�ӽ� ���� �޸� ����
	//if(pGeoObjs->p_I_N_MtID!= NULL){delete[] pGeoObjs->p_I_N_MtID; pGeoObjs->p_I_N_MtID=NULL;}

	if(pTempVec3!= NULL){delete[] pTempVec3; pTempVec3=NULL;}

	//��� ���ؽ���ǥ�� �븻�� NodeTM�� ������� �����־� Local��ǥ�� �����.
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
// *SHAPE_LINE �б�
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
			case TOKERN_SHAPE_CLOSED://���� �����ΰ�?
					isClosed = true;
				break;

			case TOKERN_SHAPE_VERTEXCOUNT: //�� ������ �����ϴ� ���� ����
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

	if(isClosed == TRUE) //���� �����̸� ������������ ó���Ͱ� ���� ���ش�.
	{
		 _ShapeLine.pVert_List[_ShapeLine.Vert_Cnt-1] =  _ShapeLine.pVert_List[0];
	}

	
	return true;
}


//---------------------------------------------------------------------------------------------
// TMani ���� �б� : GEOMOBJECT ����
//---------------------------------------------------------------------------------------------

bool ASEParser::Parsing_TMAnimation(GEOMOBJECT* pGeoObjs)
{
	SCENE&	_S=pGeoObjs->pAseData->m_Scene;
	pGeoObjs->m_TMAni.FirstTick = _S.FirstFrame * _S.Ticks_Per_Frame;
	pGeoObjs->m_TMAni.LastTick  = _S.LastFrame  * _S.Ticks_Per_Frame;
	pGeoObjs->m_TMAni.LastTime  = (float)(_S.LastFrame * _S.FrameSpeed) ;
	pGeoObjs->m_TMAni.m_pScene = &pGeoObjs->pAseData->m_Scene;
			
	TMAnimation& _TMAni =  pGeoObjs->m_TMAni;
	int		TickPerSample = pGeoObjs->pAseData->m_Scene.Ticks_Per_Frame *5; //�ѻ��ô� ƽ��,ex)192*5=960, �ݵ�� 3dmax���� ������ 5�����Ӵ� �ѹ��� ���ø� �ؾ��Ѵ�. 
	_TMAni.TickPerFrame = pGeoObjs->pAseData->m_Scene.Ticks_Per_Frame 	;
	

	D3DXVECTOR3* TempRotvec = NULL;
	float		 TempAngle = 0;
	TempRotvec = new D3DXVECTOR3[_TMAni.FrameCnt]; //�Ͻ������� CONTROL_ROT_SAMPLE�� �о�鿩 ������ ����
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
								if((CurTick != 0 ) && (idx == 0))//�̰��� ù��° Ű�������� 0�� �ƴ� ����̴�. 0��������Pos���� ���߿� ���Ƿ� ������ش�.
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
					LOG_WRITE_A("����̸� : %s",pGeoObjs->NodeTM.Name);
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
					LOG_WRITE_A("����̸� : %s",pGeoObjs->NodeTM.Name);

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
					TempScaleAxis = new D3DXVECTOR3[_TMAni.FrameCnt]; //�Ͻ������� CONTROL_ROT_SAMPLE�� �о�鿩 ������ ����
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

					if(pTempScale[idx-1].Curtick !=  _TMAni.LastTick) //�������������� ���ٸ� ������ ������ �߰�
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
					LOG_WRITE_A("����̸� : %s",pGeoObjs->NodeTM.Name);
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
// ASECameraData ���� �ִϸ��̼� �Ľ��Լ�
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
		_pCData->m_TMAni.m_pScene = &_pCData->m_Scene;  //������ �ݵ�� TMAnimation �Լ��� �־����

		TMAnimation& _TMAni =  _pCData->m_TMAni;
		int		TickPerSample = _pCData->m_Scene.Ticks_Per_Frame *5; //�ѻ��ô� ƽ��,ex)192*5=960, �ݵ�� 3dmax���� ������ 5�����Ӵ� �ѹ��� ���ø� �ؾ��Ѵ�. 
		_TMAni.TickPerFrame = _pCData->m_Scene.Ticks_Per_Frame 	;
		_TMAni.FrameCnt =  _S.LastFrame - _S.FirstFrame;

		D3DXVECTOR3* TempRotvec = NULL;
		float		 TempAngle = 0;
		TempRotvec = new D3DXVECTOR3[_TMAni.FrameCnt]; //�Ͻ������� CONTROL_ROT_SAMPLE�� �о�鿩 ������ ����
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
									if((CurTick != 0 ) && (idx == 0))//�̰��� ù��° Ű�������� 0�� �ƴ� ����̴�. 0��������Pos���� ���߿� ���Ƿ� ������ش�.
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
						LOG_WRITE_A("����̸� : %s",_pCData->NodeTM.Name);
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
						LOG_WRITE_A("����̸� : %s", _pCData->NodeTM.Name);

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
						TempScaleAxis = new D3DXVECTOR3[_TMAni.FrameCnt]; //�Ͻ������� CONTROL_ROT_SAMPLE�� �о�鿩 ������ ����
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
	{	LOG_WRITE_A("Parsing_CameraSetting �Լ��� ���ڰ��� NULL��!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
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