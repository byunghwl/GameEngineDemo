/************************************************************************/
/*   2014. 07. 09														*/
/*	 ASEData class											            */
/************************************************************************/

#include "DXUT.H"
#include "ASEData.h"


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
// ���⼭���� FACE_Set �Լ�����
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
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
// FACE_Set �Ҹ���
//-----------------------------------------------------------------------------

FACE_Set::~FACE_Set()
{	
	write_log_file("�޸� ���� : ~FACE_Set(), �Ҹ��� ȣ�� ");

	if(vertexbuffer !=NULL) 
	{
		vertexbuffer->Release(); vertexbuffer = NULL; write_log_file("�޸����� : vertexbuffer->Release()  ");
	}

	if(indexbuffer !=NULL) 
	{ 
		indexbuffer->Release();  indexbuffer = NULL; write_log_file("�޸����� : indexbuffer->Release();  ");
	}


	SAFE_RELEASE(m_pTex) write_log_file("�޸����� : m_pTex->Release();  ");
	SAFE_DELETE_ARR(ptemp_Vtx)

	SAFE_DELETE_ARR(ptemp_Vtx_Skin)
	SAFE_DELETE_ARR(ptemp_Idx)
}

//-----------------------------------------------------------------------------
//  FACE_Set �׷��ֱ�
//-----------------------------------------------------------------------------

void FACE_Set::Draw(LPDIRECT3DDEVICE9 _device)
{	
	if(m_pTex!=NULL)
	{		_device->SetTexture(0,m_pTex);	}

	// assert(false);

	_device->SetMaterial(&m_Material);	

	_device->SetStreamSource( 0, vertexbuffer, 0, sizeof( MY_VERTEX ) );
	_device->SetIndices( indexbuffer);
	_device->SetFVF( MY_FVF );

	_device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, numvertex, 0 , numfaces );
}


//-------------------------------------------------------------------------
// ���̴� ���� �׸��� �Լ� 
//-------------------------------------------------------------------------
void  FACE_Set::Draw(LPDIRECT3DDEVICE9 _device, LPDIRECT3DVERTEXSHADER9  _pVShader ,  LPDIRECT3DPIXELSHADER9  _pPShader)
{

	//c21�� material diffuse �־��ֱ�
	_device->SetVertexShaderConstantF( 21, (float*)(&m_Material.Diffuse), 1 );	
	//c22�� material Specular �־��ֱ�
	m_Material.Specular.a = m_Material.Power;  //c22.w �� ����ŧ�� �Ŀ��� �־��ش�.
	_device->SetVertexShaderConstantF( 22, (float*)(&m_Material.Specular), 1 );
	//c23�� material Ambient �־��ֱ�
	_device->SetVertexShaderConstantF( 23, (float*)(&m_Material.Ambient), 1 );
	//c24�� material Emissive �־��ֱ�
	_device->SetVertexShaderConstantF( 24, (float*)(&m_Material.Emissive), 1 ); 
	//���̴�����
	_device->SetVertexShader(_pVShader);
	_device->SetPixelShader( _pPShader );

	//if(m_pTex!=NULL)
 	//{		_device->SetTexture(0,m_pTex);	}
	/*if(G_pCubeTex!=NULL)*/
	_device->SetTexture(0,G_pCubeTex);	
	// assert(false);

	//_device->SetMaterial(&m_Material);	

	_device->SetStreamSource( 0, vertexbuffer, 0, sizeof( MY_VERTEX ) );
	_device->SetIndices( indexbuffer);
	//_device->SetFVF( MY_FVF );
	_device->SetFVF( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0) );
	_device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, numvertex, 0 , numfaces );

	//���̴� ����
	_device->SetPixelShader( NULL );
	_device->SetVertexShader(NULL);
}



//-----------------------------------------------------------------------------
// FACE_Set �׷��ֱ� ,,��Ų�� �޽��϶�!!!
// direct9.0c���� �������ִ� ��Ʈ���� �ȷ�Ʈ�� �̿��� ���
//-----------------------------------------------------------------------------

void FACE_Set::DrawSkinning(LPDIRECT3DDEVICE9 _device)
{
	if(m_pTex!=NULL)
	{		_device->SetTexture(0,m_pTex);	}

	_device->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE );
	// blend weight�� 4��(��Ÿ �ƴ�! 4��!)
	_device->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_3WEIGHTS );

	_device->SetStreamSource( 0, vertexbuffer, 0, sizeof(MY_VERTEX_SKINED) );
	_device->SetFVF( MY_FVF_SKINED );
	_device->SetIndices( indexbuffer );
	_device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, numvertex, 0, numfaces );
	_device->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE );
	_device->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE );
 	if(m_pTex!=NULL)
 	{		_device->SetTexture(0,NULL);	}
}

//-------------------------------------------------------------------------
// FACE_Set �׷��ֱ� ,,��Ų�� �޽��϶�!!!			
// ��Ʈ�����ȷ�Ʈ�� ���ؽ� ���̴��� �Ѱܼ� ����ϱ�,  
//-------------------------------------------------------------------------

void FACE_Set::DrawSkinning(LPDIRECT3DDEVICE9 _device , LPDIRECT3DVERTEXSHADER9  _pVShader,  LPDIRECT3DPIXELSHADER9  _pPShader)
{
	
	D3DXVECTOR4 CameraPos = SHGlobal_GetCurScene_Camera->GetPos();
	CameraPos.w = 0.0f;
	//c20�� ī�޶� ��ġ �־��ֱ�
	_device->SetVertexShaderConstantF( 20, (float*)CameraPos, 1 );
	_device->SetPixelShaderConstantF( 20, (float*)CameraPos, 1 );
	//c21�� material diffuse �־��ֱ�
	_device->SetVertexShaderConstantF( 21, (float*)(&m_Material.Diffuse), 1 );	
	//c22�� material Specular �־��ֱ�
	m_Material.Specular.a = m_Material.Power;   //c22.w �� ����ŧ�� �Ŀ��� �־��ش�.
	_device->SetVertexShaderConstantF( 22, (float*)(&m_Material.Specular), 1 );
	_device->SetPixelShaderConstantF( 22, (float*)(&m_Material.Specular), 1 );
	//c23�� material Ambient �־��ֱ�
	_device->SetVertexShaderConstantF( 23, (float*)(&m_Material.Ambient), 1 );
	//c24�� material Emissive �־��ֱ�
	_device->SetVertexShaderConstantF( 24, (float*)(&m_Material.Emissive), 1 );

	//���̴�����
	_device->SetVertexShader(_pVShader);
	_device->SetPixelShader( _pPShader );

	if(m_pTex!=NULL)
	{		_device->SetTexture(0,m_pTex);	}

	// assert(false);

	//_device->SetMaterial(&m_Material);	

	_device->SetStreamSource( 0, vertexbuffer, 0, sizeof( MY_VERTEX_SKINED ) );
	_device->SetIndices( indexbuffer);
	_device->SetFVF( MY_FVF_SKINED );

	_device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, numvertex, 0 , numfaces );

	//���̴� ����
	_device->SetPixelShader( NULL );
	_device->SetVertexShader(NULL);
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
// ���⼭���� GEOMOBJECT �Լ�����
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


//---------------------------------------------------------------------------------------------
// GEOMOBJECT ������
//---------------------------------------------------------------------------------------------

GEOMOBJECT::GEOMOBJECT() :  num_vertex(0),  num_faces(0), num_tvertex(0), num_tvfaces(0), num_cvertex(0),m_HaveMesh(false),
	num_cvfaces(0), Mtrl_Ref(-1), m_FaceSetCnt(0), m_CurAniTime(0.0f), m_isSkinned(false),num_pIndice(0),num_pNormalCnt(0),
	num_BoneCnt(0),
	pVertex(NULL), ptVertex(NULL), pcVertex(NULL), pNormal(NULL), pFace_List(NULL), pVertWeightList(NULL),
	p_indices(NULL),ptFace(NULL),pUV(NULL),m_pFaceSet(NULL),m_pParent(NULL),pAseData(NULL),m_pMatPalette(NULL),
	p_I_N_MtID(NULL)
{
	D3DXMatrixIdentity(&m_MLocalTM); // ������ķ� �ʱ�ȭ
	D3DXMatrixIdentity(&m_MWorldTM);
	D3DXMatrixIdentity(&m_LTM_Pos); // ������ķ� �ʱ�ȭ
	D3DXMatrixIdentity(&m_LTM_Rot);
	D3DXMatrixIdentity(&m_LTM_scale); // ������ķ� �ʱ�ȭ
	D3DXMatrixIdentity(&m_ArcBallTM); // ������ķ� �ʱ�ȭ	
	MeshType = eMeshType_Normal;
}


//-----------------------------------------------------------------------------
// GEOMOBJECT �Ҹ���
//-----------------------------------------------------------------------------

GEOMOBJECT::~GEOMOBJECT()
{   // �޸� ����
	write_log_file("�޸� ���� : ~GEOMOBJECT() ȣ��");
	SAFE_DELETE_ARR(pVertex) 	
		SAFE_DELETE_ARR(ptVertex) 
		SAFE_DELETE_ARR(pcVertex)
		SAFE_DELETE_ARR(pNormal) 
		SAFE_DELETE_ARR(p_indices)
		SAFE_DELETE_ARR(ptFace)
		SAFE_DELETE_ARR(pUV) 
		SAFE_DELETE_ARR(m_pFaceSet)
		SAFE_DELETE_ARR(pFace_List)
		SAFE_DELETE_ARR(pVertWeightList)
		SAFE_DELETE_ARR(m_pMatPalette)
		SAFE_DELETE_ARR(p_I_N_MtID)
		

}


//---------------------------------------------------------------------------------------------
//  GEOMOBJECT ������Ʈ �Լ�
//---------------------------------------------------------------------------------------------

void GEOMOBJECT:: Update(float _dTime, D3DXMATRIX MoveTM)
{
//	if(m_isSkinned == FALSE)
	//{
		m_AniTm_Pos = m_LTM_Pos;
		m_AniTm_Rot = m_LTM_Rot;
		m_AniTM_Scale = m_LTM_scale;
		Animation(_dTime);

		if(m_pParent !=NULL) //�ڽ��ϋ�
		{//m_MWorldTM = m_MLocalTM *  m_pParent->m_MWorldTM ;
			m_MWorldTM = m_AniTM_Scale * m_AniTm_Rot *m_ArcBallTM *m_AniTm_Pos *  m_pParent->m_MWorldTM;

		}
		if(m_pParent ==NULL) //�ڱⰡ�θ��϶�
		{	//m_MWorldTM = m_MLocalTM * MoveTM;
			m_MWorldTM = (m_AniTM_Scale  * m_AniTm_Rot *m_ArcBallTM *m_AniTm_Pos) * MoveTM;
		}


}


//-----------------------------------------------------------------------------
//  GEOMOBJECT �׸��� �Լ�
//-----------------------------------------------------------------------------

void GEOMOBJECT::Draw(LPDIRECT3DDEVICE9 _device)
{
	if(m_HaveMesh == FALSE)
		return;

		_device->SetTransform(D3DTS_WORLD,&m_MWorldTM);

		if(m_FaceSetCnt >0)
		{
			for(int i=0; i<m_FaceSetCnt ; i++)
			{
				m_pFaceSet[i].Draw(_device);
			}
		}
		else
		{
			m_pFaceSet[0].Draw(_device);
		}

		m_BoundSphere.Draw(&m_MWorldTM);

}


//-------------------------------------------------------------------------
// ���̴� �� ����ϴ� �׸��� �Լ�
// _pVShader : ���ؽ����̴�
// _pPShader : �ȼ� ���̴�
//-------------------------------------------------------------------------
void	GEOMOBJECT::Draw( LPDIRECT3DDEVICE9 _device, LPDIRECT3DVERTEXSHADER9  _pVShader,  LPDIRECT3DPIXELSHADER9  _pPShader)
{
	if(m_HaveMesh == FALSE)
		return;

	D3DXMATRIX matTranspose , tempTM;
	D3DXMATRIX mattemp = SHGlobal_GetCurScene_Camera->Get_ViewProjMat();
	tempTM = m_MWorldTM * mattemp;

	D3DXMatrixTranspose( &matTranspose, &m_MWorldTM );
	// Load the combined model- mWorld matrix in registers c[4]-c[7] at vertex shader
	_device->SetVertexShaderConstantF( 4, (float*)matTranspose, 4 );	

	D3DXMatrixTranspose( &matTranspose, &tempTM );
	// Load the combined model- mWorld matrix * SHGlobal_GetCurScene_Camera->Get_ViewProjMat() in registers c[8]-c[11] at vertex shader
	_device->SetVertexShaderConstantF( 8, (float*)matTranspose, 4 );	


	D3DXVECTOR4 CameraPos = SHGlobal_GetCurScene_Camera->GetPos();
	CameraPos.w = 0.0f;

	//c20�� ī�޶� ��ġ �־��ֱ�
	_device->SetVertexShaderConstantF( 20, (float*)CameraPos, 1 );
	SHGlobal_GetD3DDevice->SetVertexShaderConstantF( 13, (float*)(G_vecDirLight*-1.0f), 1 );


	if(m_FaceSetCnt >0)
	{
		for(int i=0; i<m_FaceSetCnt ; i++)
		{
			m_pFaceSet[i].Draw(_device, _pVShader, _pPShader);
		}
	}
	else
	{
		m_pFaceSet[0].Draw(_device, _pVShader, _pPShader);
	}

	m_BoundSphere.Draw(&m_MWorldTM);
}




//-----------------------------------------------------------------------------
// d3d ������ ���׸����� ������ش�
// id = -1 �̸� ������׸����� ���°��� (class : standard)
// id = 0�̻��̸� (multi-sub Material)
//-----------------------------------------------------------------------------

D3DMATERIAL9 GEOMOBJECT::Make_Material(MATERIAL* m, int id /*=-1*/)
{

	D3DMATERIAL9 Res_Mtrl;

	

	if(m == nullptr)//������ �׳� �⺻ ���׸����
	{
		Res_Mtrl = SHUtil::InitMaterial(COLOR_WHITE,COLOR_WHITE,COLOR_WHITE,COLOR_WHITE,0.3f);
		return Res_Mtrl;
	}
	//������׸��� ������
	if(id== -1)
	{	
		Res_Mtrl.Ambient  = m->_Ambient; // �ֺ���
		Res_Mtrl.Diffuse  = m->_Diffuse;// ��ü ����
		Res_Mtrl.Specular = m->_Specular * m->_Shine_Strength;    //
		Res_Mtrl.Emissive = m->_Self_illum * m->_Diffuse; //��ü �߱�
		Res_Mtrl.Power    = m->_Shine * 127.9f; //����ŧ�� �Ŀ�
	}
	//������׸��� ������
	if(id>=0)
	{
		Res_Mtrl.Ambient  = m->_GA_SubMtrl.GetAt(id)->_Ambient; // �ֺ���
		Res_Mtrl.Diffuse  = m->_GA_SubMtrl.GetAt(id)->_Diffuse;// ��ü ����
		Res_Mtrl.Specular = m->_GA_SubMtrl.GetAt(id)->_Specular * m->_GA_SubMtrl.GetAt(id)->_Shine_Strength;    //����ŧ��
		Res_Mtrl.Emissive = m->_GA_SubMtrl.GetAt(id)->_Self_illum * m->_GA_SubMtrl.GetAt(id)->_Diffuse; //��ü �߱�
		Res_Mtrl.Power    = m->_GA_SubMtrl.GetAt(id)->_Shine * 127.9f; //����ŧ�� �Ŀ�
	}
	
	return Res_Mtrl;
}


//-----------------------------------------------------------------------------
// faceset�� ������ش�.
//-----------------------------------------------------------------------------
void GEOMOBJECT::Create_FaceSet(MATERIAL* m, LPDIRECT3DDEVICE9 _pDevice)
{

	m_BoundSphere.init(10.0f,  false );

	if(m_HaveMesh == FALSE)
	{
		LOG_WRITE_A(" %s : �޽��� �� �Ȱ������־�!!", NodeTM.Name);
		return;
	}

	if(m != NULL)
		m_FaceSetCnt = m->_SubMtrlCnt; //������׸��󰳼���ŭ FaceSet�� ����� �ֱ� ����..
	//m_FaceSetCnt= 0;//bshsos ������׸��� �ϼ��Ǹ� �����ֱ�

	MY_VERTEX* tempVertex = NULL;
	tempVertex = new MY_VERTEX[num_vertex];//�Ͻ��� ���ؽ��迭����
	ASSERT_IF_FAIL_MemAlloc(tempVertex)

		for(int i=0; i<num_vertex ;  i++)
		{
			tempVertex[i].x  = pVertex[i].x; 
			tempVertex[i].y  = pVertex[i].y;
			tempVertex[i].z  = pVertex[i].z;
			tempVertex[i].nx = pNormal[i].x;
			tempVertex[i].ny = pNormal[i].y;
			tempVertex[i].nz = pNormal[i].z;
			tempVertex[i].u  = pUV[i]._u;
			tempVertex[i].v  = pUV[i]._v;
		}

		//�ε��� ����
		WORD* tempIndices = NULL;

		tempIndices = new WORD[ num_faces*3 ];
		ASSERT_IF_FAIL_MemAlloc(tempIndices)
			memcpy(tempIndices, p_indices, sizeof(WORD)*num_faces*3);

		//////////////////////////////////////////////////////////////////////////
		//subMaterial�� ���ٸ�
		if(m_FaceSetCnt <=0) 
		{
			m_FaceSetCnt = 1;
			m_pFaceSet = new FACE_Set[1];
			ASSERT_IF_FAIL_MemAlloc(m_pFaceSet)
			m_pFaceSet[0].numvertex = num_vertex;
			m_pFaceSet[0].numfaces = num_faces;
			m_pFaceSet[0].Mtrl_ID = Mtrl_Ref;

			 
			//���ؽ����� ����
			if( FAILED( _pDevice->CreateVertexBuffer( m_pFaceSet[0].numvertex * sizeof(MY_VERTEX), 0, MY_FVF,D3DPOOL_DEFAULT, &m_pFaceSet[0].vertexbuffer, NULL ) ) )
			{	write_log_file("���ؽ� ���� ���� ����"); assert(false);	}
			//���ؽ����� ��
			if( FAILED( m_pFaceSet[0].vertexbuffer->Lock( 0, sizeof( MY_VERTEX )*	m_pFaceSet[0].numvertex, ( void** )&m_pFaceSet[0].m_pVtxBufPointer, 0 ) ) )
			{	write_log_file("���ؽ� ���� �� ����"); assert(false);}

			memcpy( m_pFaceSet[0].m_pVtxBufPointer, tempVertex, sizeof( MY_VERTEX )*	m_pFaceSet[0].numvertex );
			//���
			m_pFaceSet[0].vertexbuffer->Unlock();

			//�ε������� ����
			if( FAILED( _pDevice->CreateIndexBuffer( m_pFaceSet[0].numfaces*3 * sizeof(WORD), D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,
				D3DPOOL_MANAGED, &m_pFaceSet[0].indexbuffer,NULL
				)
				) 
				)
			{ write_log_file("�ε��� ���� ���� ����");    assert(false);		  }
			//�ε������� ��
			
			if( FAILED( m_pFaceSet[0].indexbuffer->Lock( 0, sizeof( WORD )*m_pFaceSet[0].numfaces*3 , ( void** )&m_pFaceSet[0].m_pIdxBufPointer, 0 ) ) )
			{	write_log_file("���ؽ� ���� �� ����"); assert(false);}

			memcpy( m_pFaceSet[0].m_pIdxBufPointer, tempIndices, sizeof( WORD )* m_pFaceSet[0].numfaces*3  );

			m_pFaceSet[0].indexbuffer->Unlock();

			//���׸���� �ؽ��� ����

			m_pFaceSet[0].m_Material = Make_Material(m);
		
			if(m!=nullptr)
			{
				if(m->_Diffuse_Map.pBitmapName == NULL)
				{
					write_log_file("GEOMOBJECT::Create_FaceSet(), m->_Diffuse_Map.pBitmapName �� NULL �Դϴ�.");
				}
				else
				{
					TCHAR Path[512]=L"./image/";
					wcsncat_s(Path, m->_Diffuse_Map.pBitmapName ,wcslen( m->_Diffuse_Map.pBitmapName));
					wcscpy_s(m_pFaceSet[0].pTexPath, Path);
					wcscpy_s(m_pFaceSet[0].pTexName, m->_Diffuse_Map.pBitmapName);
					D3DXCreateTextureFromFile(_pDevice, Path, &m_pFaceSet[0].m_pTex);
					ASSERT_IF_FAIL_MemAlloc(m_pFaceSet[0].m_pTex);
				}
			}

		}
		else //subMaterial ������ 1�̻��̶�� 
		{
			//���� ���׸��� ������ŭ Face_set�� ������ش�.
			m_pFaceSet = new FACE_Set[m_FaceSetCnt];
			ASSERT_IF_FAIL_MemAlloc(m_pFaceSet)

			for(int i=0; i<m_FaceSetCnt ;  i++)
			{
				m_pFaceSet[i].Mtrl_ID = Mtrl_Ref;
				m_pFaceSet[i].ptemp_INMID = new Temp_idx_Norm_mtID[num_pIndice];
				ASSERT_IF_FAIL_MemAlloc(m_pFaceSet[i].ptemp_INMID)
				m_pFaceSet[i].ptemp_Vtx = new MY_VERTEX[num_pIndice];
				ASSERT_IF_FAIL_MemAlloc(m_pFaceSet[i].ptemp_Vtx)
				m_pFaceSet[i].ptemp_Idx = new WORD[num_pIndice];
				ASSERT_IF_FAIL_MemAlloc(m_pFaceSet[i].ptemp_Idx)
			}

			//p_I_N_MtID �� ����(�ε��� ����)��ŭ ���鼭 ���׸��� ���̵𺰷� ���ؽ��� �����������Ѵ�. 
			for(int j=0; j<num_pIndice ; j++ )
			{
				int& cnt = m_pFaceSet[p_I_N_MtID[j].mtrl_ID].TempINMIDCnt;
				m_pFaceSet[p_I_N_MtID[j].mtrl_ID].ptemp_INMID[cnt++] = p_I_N_MtID[j];
			}

			//���� �������� �� faceset�� p_I_N_MtID�� ptemp_Vtx�� ptemp_Idx�� �����. 
			for(int j=0; j<m_FaceSetCnt ; j++)
			{
				LOG_WRITE_A("face_set[%d] \n",j);
				int& inmcnt = m_pFaceSet[j].TempINMIDCnt;
				m_pFaceSet[j].numvertex = 0;
				int& VCnt = m_pFaceSet[j].numvertex;
				m_pFaceSet[j].numfaces = inmcnt/3;

				for(int inmIdx=0; inmIdx < inmcnt ; inmIdx++)
				{
					int V_idx = m_pFaceSet[j].ptemp_INMID[inmIdx].idx;
					
					if(m_pFaceSet[j].ptemp_INMID[inmIdx].check == false)// //�ѹ��̶�  ���簡 �ȵȰ��̶��
					{	
						m_pFaceSet[j].ptemp_Vtx[VCnt] = tempVertex[V_idx];//ptemp_Vtx�����

						for(int k = inmIdx ; k< inmcnt ; k++)
						{
							if((m_pFaceSet[j].ptemp_INMID[k].idx == V_idx) && (m_pFaceSet[j].ptemp_INMID[k].check == false))
							{
								m_pFaceSet[j].ptemp_INMID[k].idx = VCnt;
								m_pFaceSet[j].ptemp_INMID[k].check = true;//�̹� ����� ������ ǥ��
							}
						}
						
						m_pFaceSet[j].ptemp_Idx[inmIdx] = m_pFaceSet[j].ptemp_INMID[inmIdx].idx;//ptemp_Idx�����
						VCnt++;
					}
					else 
					{	
						m_pFaceSet[j].ptemp_Idx[inmIdx] = m_pFaceSet[j].ptemp_INMID[inmIdx].idx;
					
					}
					//LOG_WRITE_A("ptemp_Vtx[%d] : %f, %f, %f  ",VCnt-1,m_pFaceSet[j].ptemp_Vtx[VCnt-1].x, m_pFaceSet[j].ptemp_Vtx[VCnt-1].y, m_pFaceSet[j].ptemp_Vtx[VCnt-1].z);
				//LOG_WRITE_A("ptemp_Idx[%d] : %d  ",inmIdx,(int)m_pFaceSet[j].ptemp_Idx[inmIdx]);
				}
				
			}

			// ���� �� face_set���� ���ؽ� ���ۿ� �ε��� ���۸� ���ɾ��ش�. 
			// ���� ���׸���� �ؽ��ĸ� ������ �ش�.
			for(int fsidx=0; fsidx<m_FaceSetCnt ;fsidx++) //fsidx : face_set_index
			{	
				//////////////////////////////////////////////////////////////////////////
				//���ؽ� ���ɱ�
				
				//���ؽ����� ����
				if( FAILED( _pDevice->CreateVertexBuffer( m_pFaceSet[fsidx].numvertex * sizeof(MY_VERTEX), 0, MY_FVF,D3DPOOL_DEFAULT, &m_pFaceSet[fsidx].vertexbuffer, NULL ) ) )
				{	write_log_file("���ؽ� ���� ���� ����"); assert(false);	}
				//���ؽ����� ��
				if( FAILED( m_pFaceSet[fsidx].vertexbuffer->Lock( 0, sizeof( MY_VERTEX )*m_pFaceSet[fsidx].numvertex, ( void** )&m_pFaceSet[fsidx].m_pVtxBufPointer, 0 ) ) )
				{	write_log_file("���ؽ� ���� �� ����"); assert(false);}

				memcpy( m_pFaceSet[fsidx].m_pVtxBufPointer, m_pFaceSet[fsidx].ptemp_Vtx, sizeof( MY_VERTEX )*m_pFaceSet[fsidx].numvertex );
				//���
				m_pFaceSet[fsidx].vertexbuffer->Unlock();

				//////////////////////////////////////////////////////////////////////////
				// �ε��� ��ɱ�
				// �ε������� ����
				if( FAILED( _pDevice->CreateIndexBuffer( m_pFaceSet[fsidx].TempINMIDCnt* sizeof(WORD), D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,
														D3DPOOL_MANAGED, &m_pFaceSet[fsidx].indexbuffer,NULL)
						  ) 
				  )
				{ write_log_file("�ε��� ���� ���� ����");    assert(false);		  }
				//�ε������� ��
				
				if( FAILED( m_pFaceSet[fsidx].indexbuffer->Lock( 0, sizeof( WORD )*m_pFaceSet[fsidx].TempINMIDCnt , ( void** )&m_pFaceSet[fsidx].m_pIdxBufPointer, 0 ) ) )
				{	write_log_file("���ؽ� ���� �� ����"); assert(false);}

				memcpy( m_pFaceSet[fsidx].m_pIdxBufPointer, m_pFaceSet[fsidx].ptemp_Idx, sizeof( WORD )* m_pFaceSet[fsidx].TempINMIDCnt );

				m_pFaceSet[fsidx].indexbuffer->Unlock();

				//////////////////////////////////////////////////////////////////////////
				// ���׸��� ����
				m_pFaceSet[fsidx].m_Material = Make_Material(m,fsidx);
				// �ؽ��� ����
				if(m!=nullptr)
				{
					if(m->_GA_SubMtrl.GetAt(fsidx)->_Diffuse_Map.pBitmapName == NULL)
					{
						write_log_file("GEOMOBJECT::Create_FaceSet(), m->_Diffuse_Map.pBitmapName �� NULL �Դϴ�.");
					}
					else
					{
						TCHAR Path[512]=L"./image/";
						wcsncat_s(Path,  m->_GA_SubMtrl.GetAt(fsidx)->_Diffuse_Map.pBitmapName,wcslen( m->_GA_SubMtrl.GetAt(fsidx)->_Diffuse_Map.pBitmapName));
						wcscpy_s(m_pFaceSet[fsidx].pTexPath, Path);
						wcscpy_s(m_pFaceSet[fsidx].pTexName, m->_GA_SubMtrl.GetAt(fsidx)->_Diffuse_Map.pBitmapName);
						D3DXCreateTextureFromFile(_pDevice, Path, &m_pFaceSet[fsidx].m_pTex);
						ASSERT_IF_FAIL_MemAlloc(m_pFaceSet[fsidx].m_pTex);
					}
				}
			}


			//���ٰɾ����� �ӽ����� �޸� ����
			for(int i=0; i<m_FaceSetCnt ;  i++)
			{	
					SAFE_DELETE_ARR(m_pFaceSet[i].ptemp_Vtx)
					SAFE_DELETE_ARR(m_pFaceSet[i].ptemp_Vtx_Skin)
					SAFE_DELETE_ARR(m_pFaceSet[i].ptemp_INMID)
					SAFE_DELETE_ARR(m_pFaceSet[i].ptemp_Idx)
			}

		}
		//////////////////////////////////////////////////////////////////////////
		// �޽� �ٸ������ aseData���� �������� �ߴ� �޸𸮵��� ������ �ش�.	

			SAFE_DELETE_ARR(tempVertex)
			SAFE_DELETE_ARR(tempIndices)

			SAFE_DELETE_ARR(pVertex)
			SAFE_DELETE_ARR(ptVertex)
			SAFE_DELETE_ARR(pcVertex)
			SAFE_DELETE_ARR(pNormal)
			SAFE_DELETE_ARR(ptFace)
			SAFE_DELETE_ARR(pFace_List)
}

//---------------------------------------------------------------------------------------------
// GeoObject�� �ִϸ��̼������������� �ִϸ��̼��� �����ִ� �Լ�
//---------------------------------------------------------------------------------------------

void GEOMOBJECT::Animation(float _dTime)
{
	if(m_TMAni.m_pScene ==NULL)//TMAni�� ���ٸ� �ִϸ��̼��� ���� �ʴ´�;;
		return;
		
	m_CurAniTime +=  (_dTime*1000.0f); 

	if(m_TMAni.LastTime < m_CurAniTime)
		m_CurAniTime = 0.0f;
		
	m_AniTm_Pos = GetCurr_PosTM(m_CurAniTime); //CurrTick�� �ش��ϴ� PositionTM�� ����ؼ� ��ȯ�Ѵ�.
	m_AniTm_Rot = GetCurr_RotTM(m_CurAniTime);
	m_AniTM_Scale = GetCurr_ScaleTM(m_CurAniTime);

}

//-----------------------------------------------------------------------------
// ���� ƽ�� �´� PosTM�� �����´�.
//-----------------------------------------------------------------------------

D3DXMATRIX GEOMOBJECT::GetCurr_PosTM(float _CurTick)
{
	D3DXVECTOR3 V;
	float ProgRate = 0.0f ;//���� ����� 
	D3DXMATRIX retM ,Inv; //  ���� ��ȯ ��Ʈ���� 

	retM = m_LTM_Pos;
	if(m_TMAni.pPos != NULL)
	{
		int len = m_TMAni.PosSize;

		//int startIdx = ((m_TMAni.FrameCnt/FRAME_PER_SAMPLE * _CurTick)/m_TMAni.LastTime) +1;
		int startIdx = (_CurTick* (float)m_TMAni.PosSize /m_TMAni.LastTime)+10;

		if(startIdx>=len) startIdx = len-1;


		for(int i=startIdx; i>=1 ; i--)
		{
			if(m_TMAni.pPos[i-1].Curtick < _CurTick)
			{
				ProgRate = (_CurTick - m_TMAni.pPos[i-1].Curtick) / (m_TMAni.pPos[i].Curtick - m_TMAni.pPos[i-1].Curtick);
				D3DXVec3Lerp(&V, &m_TMAni.pPos[i-1].pos ,  &m_TMAni.pPos[i].pos,  ProgRate);

				D3DXMatrixTranslation(&retM, V.x, V.y, V.z);

				break;
			}
			else if(m_TMAni.pPos[i-1].Curtick == _CurTick)
			{
				D3DXMatrixTranslation(&retM, m_TMAni.pPos[i-1].pos.x, m_TMAni.pPos[i-1].pos.y , m_TMAni.pPos[i-1].pos.z);

				break;
			}
		}
	}


	return retM;

}

//-----------------------------------------------------------------------------
// ���� ƽ�� �´� RotTM�� �����´�.
//-----------------------------------------------------------------------------

D3DXMATRIX  GEOMOBJECT::GetCurr_RotTM(float _CurTick)
{

	D3DXQUATERNION Q;
	float ProgRate = 0.0f ;//���� ����� 
	D3DXMATRIX retM ,Inv; //  ���� ��ȯ ��Ʈ���� 

	retM = m_LTM_Rot;
	if(m_TMAni.pRot != NULL)
	{
		int len = m_TMAni.RotSize;

		//int startIdx = ((m_TMAni.FrameCnt/FRAME_PER_SAMPLE * _CurTick)/m_TMAni.LastTick) +1;
		int startIdx = (_CurTick* (float)m_TMAni.RotSize /m_TMAni.LastTime)+10;
		if(startIdx>=len) startIdx = len-1;

		for(int i=startIdx; i>=1 ; i--)
		{
			if(m_TMAni.pRot[i-1].Curtick < _CurTick)
			{
				ProgRate = (_CurTick - m_TMAni.pRot[i-1].Curtick) / (m_TMAni.pRot[i].Curtick - m_TMAni.pRot[i-1].Curtick);
				D3DXQuaternionSlerp(&Q,   &m_TMAni.pRot[i-1].quat, &m_TMAni.pRot[i].quat,  ProgRate);
				D3DXMatrixRotationQuaternion(&retM, &Q);

				break;
			}
			else if(m_TMAni.pRot[i-1].Curtick == _CurTick)
			{
				D3DXMatrixRotationQuaternion(&retM, &m_TMAni.pRot[i-1].quat);

				break;
			}
		}
	}
	return retM;
}

//-----------------------------------------------------------------------------
// ���� ƽ�� �´� ScaleTM�� �����´�.
//-----------------------------------------------------------------------------

D3DXMATRIX  GEOMOBJECT::GetCurr_ScaleTM(float _CurTick)
{
	D3DXVECTOR3 V;
	D3DXQUATERNION Q;
	float ProgRate = 0.0f ;//���� ����� 
	D3DXMATRIX retM ,SM, SAxisM, Inv, SAxisInv; //  ���� ��ȯ ��Ʈ���� 

	retM = m_LTM_scale;
	if(m_TMAni.pScale != NULL)
	{
		int len = m_TMAni.ScaleSize;
		int startIdx = (_CurTick* (float)m_TMAni.ScaleSize /m_TMAni.LastTime)+10;
		if(startIdx>=len) startIdx = len-1;

		for(int i=startIdx; i>=1 ; i--)
		{
			if(m_TMAni.pScale[i-1].Curtick < _CurTick)
			{
				ProgRate = (_CurTick - m_TMAni.pScale[i-1].Curtick) / (m_TMAni.pScale[i].Curtick - m_TMAni.pScale[i-1].Curtick);

				D3DXVec3Lerp(&V, &m_TMAni.pScale[i-1].scale ,  &m_TMAni.pScale[i].scale,  ProgRate);
				D3DXMatrixScaling(&SM, V.x, V.y, V.z);

				D3DXQuaternionSlerp(&Q,   &m_TMAni.pScale[i-1].quat, &m_TMAni.pScale[i].quat,  ProgRate);
				D3DXMatrixRotationQuaternion(&SAxisM, &Q);

				break;
			}
			else if(m_TMAni.pScale[i-1].Curtick == _CurTick)
			{	
				D3DXMatrixScaling(&SM, m_TMAni.pScale[i-1].scale.x,  m_TMAni.pScale[i-1].scale.y,  m_TMAni.pScale[i-1].scale.z);
				D3DXMatrixRotationQuaternion(&SAxisM, &m_TMAni.pScale[i-1].quat);

				break;
			}
		}
		D3DXMatrixInverse(&SAxisInv, NULL, &SAxisM);

		retM = SAxisInv * SM * SAxisM;
	}
	return retM;
}

GEOMOBJECT&		GEOMOBJECT::DeepCopy(const GEOMOBJECT& rhs)
{
	pAseData = rhs.pAseData; //�̰� �������� ���ص� �ȴ�. �ܼ��� ����Ű�� ��������..
	//////////////////////////////////////////////////////////////////////////
	// �о������ ������� ����

	Mtrl_Ref = rhs.Mtrl_Ref;    // ���׸��� ���۷��� ,, *MATERIAL_REF,  �ʱⰪ�� -1
	num_vertex  = rhs.num_vertex;
	num_faces  = rhs.num_faces;
	num_tvertex = rhs.num_tvertex;
	num_tvfaces = rhs.num_tvfaces;
	num_cvertex = rhs.num_cvertex;
	num_cvfaces =  rhs.num_cvfaces;
	num_pIndice = rhs.num_pIndice;
	num_pNormalCnt = rhs.num_pNormalCnt;

	if(rhs.p_indices !=NULL)
	{	int size = num_pIndice ;
	p_indices = new WORD[size]; 
	ASSERT_IF_FAIL_MemAlloc(p_indices)
		memcpy(p_indices , rhs.p_indices , size*sizeof(WORD));
	}

	if((rhs.pVertex !=NULL) && (num_vertex>0))
	{	
		int size = num_vertex;
		pVertex = new D3DXVECTOR3[size];
		ASSERT_IF_FAIL_MemAlloc(pVertex)
			memcpy(pVertex , rhs.pVertex , size*sizeof(D3DXVECTOR3));
		int a = 0;
	}

	if((rhs.ptVertex !=NULL) && (num_tvertex >0))
	{	int size = num_tvertex;
	ptVertex = new D3DXVECTOR3[size];
	ASSERT_IF_FAIL_MemAlloc(ptVertex)
		memcpy(ptVertex , rhs.ptVertex , size*sizeof(D3DXVECTOR3));
	}

	if((rhs.pcVertex !=NULL) && (num_cvertex>0))
	{		int size = num_cvertex;
	pcVertex = new D3DXVECTOR3[size];
	ASSERT_IF_FAIL_MemAlloc(pcVertex)
		memcpy(pcVertex , rhs.pcVertex , size*sizeof(D3DXVECTOR3));
	}

	if((rhs.pNormal !=NULL) && (num_pNormalCnt >0))
	{	int size = num_pNormalCnt;
	pNormal = new D3DXVECTOR3[num_pNormalCnt];
	ASSERT_IF_FAIL_MemAlloc(pNormal)
		memcpy(pNormal , rhs.pNormal , size*sizeof(D3DXVECTOR3));
	}

	if((rhs.pVertWeightList !=NULL  ) && (num_pIndice > 0))
	{	int size = num_pIndice;
	pVertWeightList = new VWeight_List[size];
	ASSERT_IF_FAIL_MemAlloc(pVertWeightList)
		memcpy(pVertWeightList , rhs.pVertWeightList , size*sizeof(VWeight_List));
	}

	if((rhs.ptFace !=NULL) && (num_pIndice > 0))
	{
		int size = num_pIndice;
		ptFace = new int[size];
		ASSERT_IF_FAIL_MemAlloc(ptFace)
			memcpy(ptFace , rhs.ptFace , size*sizeof(int));
		int a = 0;
	}

	if((rhs.pUV !=NULL) && (num_pIndice > 0))
	{
		int size = num_pIndice;
		pUV = new UV_COORD[size];
		ASSERT_IF_FAIL_MemAlloc(pUV)
			memcpy(pUV , rhs.pUV , size*sizeof(UV_COORD));
	}

	if((rhs.pFace_List !=NULL) && (num_faces >0))
	{
		int size = num_faces;
		pFace_List = new FACE_LIST[size];
		ASSERT_IF_FAIL_MemAlloc(pFace_List)
			memcpy(pFace_List , rhs.pFace_List , size*sizeof(FACE_LIST));

	}

	NodeTM = rhs.NodeTM;   
	m_isSkinned = rhs.m_isSkinned;
	m_HaveMesh = rhs.m_HaveMesh;
	m_BoneIniMat = rhs.m_BoneIniMat;

	if((rhs.m_pMatPalette !=NULL) && (num_BoneCnt > 0))
	{
		int size = sizeof(rhs.m_pMatPalette)/sizeof(D3DXMATRIX);
		m_pMatPalette = new D3DXMATRIX[sizeof(rhs.m_pMatPalette)/sizeof(D3DXMATRIX)];
		ASSERT_IF_FAIL_MemAlloc(m_pMatPalette)
			memcpy(m_pMatPalette , rhs.m_pMatPalette , size*sizeof(D3DXMATRIX));

	}

	m_CurAniTime = rhs.m_CurAniTime;
	m_FaceSetCnt = rhs.m_FaceSetCnt;

	if((rhs.m_pFaceSet !=NULL)&& (m_FaceSetCnt>0))
	{
		int size = m_FaceSetCnt;
		m_pFaceSet = new FACE_Set[size];
		ASSERT_IF_FAIL_MemAlloc(m_FaceSetCnt)
			memcpy(m_pFaceSet , rhs.m_pFaceSet , size*sizeof(FACE_Set));

	}

	if((rhs.p_I_N_MtID !=NULL)&& (num_pIndice>0))
	{
		int size = num_pIndice;
		p_I_N_MtID = new Temp_idx_Norm_mtID[size];
		ASSERT_IF_FAIL_MemAlloc(p_I_N_MtID)
			memcpy(p_I_N_MtID , rhs.p_I_N_MtID , size*sizeof(Temp_idx_Norm_mtID));

	}

	m_MLocalTM =		 rhs.m_MLocalTM; 
	m_MWorldTM =		 rhs.m_MWorldTM;
	m_LTM_scale =		rhs.m_LTM_scale;
	m_LTM_Rot =			rhs.m_LTM_Rot;
	m_LTM_Pos =			 rhs.m_LTM_Pos;
	m_AniTM_Scale =		rhs.m_AniTM_Scale;
	m_AniTm_Rot  =      rhs.m_AniTm_Rot;
	m_AniTm_Pos =       rhs.m_AniTm_Pos;
	m_TMAni		=		rhs.m_TMAni;
	m_pParent	=       rhs.m_pParent; // �������� �ʿ����

	return *this;
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
// ���⼭���� SKINNED_OBJECT class �Լ�����
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
// Create_FaceSet :  GEOMOBJECT�� �����Լ� Create_FaceSet�� �������ϴ� �Լ� 
//-----------------------------------------------------------------------------

void  SKINNED_OBJECT::Create_FaceSet(MATERIAL* m, LPDIRECT3DDEVICE9 _pDevice)
{
	if(m_HaveMesh == FALSE)
	{
		LOG_WRITE_A(" %s : �޽��� �� �Ȱ������־�!!", NodeTM.Name);
		return;
	}

	if(m != NULL)
		m_FaceSetCnt = m->_SubMtrlCnt; //������׸��󰳼���ŭ FaceSet�� ����� �ֱ� ����..


	MY_VERTEX_SKINED* tempVertex = NULL;
	tempVertex = new MY_VERTEX_SKINED[num_vertex];//�Ͻ��� ���ؽ��迭����
	ASSERT_IF_FAIL_MemAlloc(tempVertex)

		for(int i=0; i<num_vertex ;  i++)
		{
			tempVertex[i].pos.x   = pVertex[i].x; 
			tempVertex[i].pos.y   = pVertex[i].y;
			tempVertex[i].pos.z   = pVertex[i].z;
			tempVertex[i].bWeight[0] = pVertWeightList[i].m_VertexW[0].Weight;
			tempVertex[i].bWeight[1] = pVertWeightList[i].m_VertexW[1].Weight;
			tempVertex[i].bWeight[2] = pVertWeightList[i].m_VertexW[2].Weight;
			tempVertex[i].Boneidx = BLENDINDEX(pVertWeightList[i].m_VertexW[3].Boneidx , pVertWeightList[i].m_VertexW[2].Boneidx , pVertWeightList[i].m_VertexW[1].Boneidx , pVertWeightList[i].m_VertexW[0].Boneidx);
			tempVertex[i].norm.x = pNormal[i].x;
			tempVertex[i].norm.y = pNormal[i].y;
			tempVertex[i].norm.z = pNormal[i].z;
			tempVertex[i].tex.x  = pUV[i]._u;
			tempVertex[i].tex.y  = pUV[i]._v;
			LOG_WRITE_A("    { %f, %f, %f : %d ,%d ,%d , %d}",tempVertex[i].bWeight[0] ,tempVertex[i].bWeight[1] ,tempVertex[i].bWeight[2], pVertWeightList[i].m_VertexW[3].Boneidx , pVertWeightList[i].m_VertexW[2].Boneidx , pVertWeightList[i].m_VertexW[1].Boneidx , pVertWeightList[i].m_VertexW[0].Boneidx);
		}

		//�ε��� ����
		WORD* tempIndices = NULL;

		tempIndices = new WORD[ num_faces*3 ];
		ASSERT_IF_FAIL_MemAlloc(tempIndices)
		memcpy(tempIndices, p_indices, sizeof(WORD)*num_faces*3);

		//////////////////////////////////////////////////////////////////////////
		//subMaterial�� ���ٸ�
		if(m_FaceSetCnt <=0) 
		{
			m_pFaceSet = new FACE_Set[1];
			ASSERT_IF_FAIL_MemAlloc(m_pFaceSet)
			m_pFaceSet[0].numvertex = num_vertex;
			m_pFaceSet[0].numfaces = num_faces;
			m_pFaceSet[0].Mtrl_ID = Mtrl_Ref;

			
			//���ؽ����� ����
			if( FAILED( _pDevice->CreateVertexBuffer( m_pFaceSet[0].numvertex * sizeof(MY_VERTEX_SKINED), 0, MY_FVF_SKINED , D3DPOOL_DEFAULT, &m_pFaceSet[0].vertexbuffer,  NULL ) ) )
			{	write_log_file("���ؽ� ���� ���� ����"); assert(false);	}
			//���ؽ����� ��
			if( FAILED( m_pFaceSet[0].vertexbuffer->Lock( 0, sizeof( MY_VERTEX_SKINED )*	m_pFaceSet[0].numvertex, ( void** )&m_pFaceSet[0].m_pVtxBufPointer, 0 ) ) )
			{	write_log_file("���ؽ� ���� �� ����"); assert(false);}

			memcpy( m_pFaceSet[0].m_pVtxBufPointer, tempVertex, sizeof( MY_VERTEX_SKINED )*	m_pFaceSet[0].numvertex );
			//���
			m_pFaceSet[0].vertexbuffer->Unlock();

			//�ε������� ����
			if( FAILED( _pDevice->CreateIndexBuffer( m_pFaceSet[0].numfaces*3 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
				D3DPOOL_MANAGED, &m_pFaceSet[0].indexbuffer,NULL
				)
				) 
				)
			{ write_log_file("�ε��� ���� ���� ����");    assert(false);		  }

			//�ε������� ��
			
			if( FAILED( m_pFaceSet[0].indexbuffer->Lock( 0, sizeof( WORD )*m_pFaceSet[0].numfaces*3 , ( void** )&m_pFaceSet[0].m_pIdxBufPointer, 0 ) ) )
			{	write_log_file("���ؽ� ���� �� ����"); assert(false);}

			memcpy( m_pFaceSet[0].m_pIdxBufPointer, tempIndices, sizeof( WORD )* m_pFaceSet[0].numfaces*3  );

			m_pFaceSet[0].indexbuffer->Unlock();

			//���׸���� �ؽ��� ����

			m_pFaceSet[0].m_Material = Make_Material(m);
			//bshsos
			//m = NULL;
			if(m!=NULL)//
			{
				if(m->_Diffuse_Map.pBitmapName == NULL)
				{
					write_log_file("GEOMOBJECT::Create_FaceSet(), m->_Diffuse_Map.pBitmapName �� NULL �Դϴ�.");
				}
				else
				{
					TCHAR Path[512]=L"./image/";
					wcsncat_s(Path, m->_Diffuse_Map.pBitmapName ,wcslen( m->_Diffuse_Map.pBitmapName));
					wcscpy_s(m_pFaceSet[0].pTexPath, Path);
					wcscpy_s(m_pFaceSet[0].pTexName, m->_Diffuse_Map.pBitmapName);
					D3DXCreateTextureFromFile(_pDevice, Path, &m_pFaceSet[0].m_pTex);
					ASSERT_IF_FAIL_MemAlloc(m_pFaceSet[0].m_pTex);
				}
			}

		}
		else //subMaterial ������ 1�̻��̶�� 
		{
			//���� ���׸��� ������ŭ Face_set�� ������ش�.
			m_pFaceSet = new FACE_Set[m_FaceSetCnt];
			ASSERT_IF_FAIL_MemAlloc(m_pFaceSet)

				for(int i=0; i<m_FaceSetCnt ;  i++)
				{
					m_pFaceSet[i].Mtrl_ID = Mtrl_Ref;
					m_pFaceSet[i].ptemp_INMID = new Temp_idx_Norm_mtID[num_pIndice];
					ASSERT_IF_FAIL_MemAlloc(m_pFaceSet[i].ptemp_INMID)
						m_pFaceSet[i].ptemp_Vtx_Skin = new MY_VERTEX_SKINED[num_pIndice];
					ASSERT_IF_FAIL_MemAlloc(m_pFaceSet[i].ptemp_Vtx_Skin)
						m_pFaceSet[i].ptemp_Idx = new WORD[num_pIndice];
					ASSERT_IF_FAIL_MemAlloc(m_pFaceSet[i].ptemp_Idx)
				}

				//p_I_N_MtID �� ����(�ε��� ����)��ŭ ���鼭 ���׸��� ���̵𺰷� ���ؽ��� �����������Ѵ�. 
				for(int j=0; j<num_pIndice ; j++ )
				{
					int& cnt = m_pFaceSet[p_I_N_MtID[j].mtrl_ID].TempINMIDCnt;
					m_pFaceSet[p_I_N_MtID[j].mtrl_ID].ptemp_INMID[cnt++] = p_I_N_MtID[j];
				}

				//���� �������� �� faceset�� p_I_N_MtID�� ptemp_Vtx_Skin�� ptemp_Idx�� �����. 
				for(int j=0; j<m_FaceSetCnt ; j++)
				{
					LOG_WRITE_A("face_set[%d] \n",j);
					int& inmcnt = m_pFaceSet[j].TempINMIDCnt;
					m_pFaceSet[j].numvertex = 0;
					int& VCnt = m_pFaceSet[j].numvertex;
					m_pFaceSet[j].numfaces = inmcnt/3;

					for(int inmIdx=0; inmIdx < inmcnt ; inmIdx++)
					{
						int V_idx = m_pFaceSet[j].ptemp_INMID[inmIdx].idx;

						if(m_pFaceSet[j].ptemp_INMID[inmIdx].check == false)// //�ѹ��̶�  ���簡 �ȵȰ��̶��
						{	
							m_pFaceSet[j].ptemp_Vtx_Skin[VCnt] = tempVertex[V_idx];//ptemp_Vtx_Skin�����

							for(int k = inmIdx ; k< inmcnt ; k++)
							{
								if((m_pFaceSet[j].ptemp_INMID[k].idx == V_idx) && (m_pFaceSet[j].ptemp_INMID[k].check == false))
								{
									m_pFaceSet[j].ptemp_INMID[k].idx = VCnt;
									m_pFaceSet[j].ptemp_INMID[k].check = true;//�̹� ����� ������ ǥ��
								}
							}

							m_pFaceSet[j].ptemp_Idx[inmIdx] = m_pFaceSet[j].ptemp_INMID[inmIdx].idx;//ptemp_Idx�����
							VCnt++;
						}
						else 
						{	
							m_pFaceSet[j].ptemp_Idx[inmIdx] = m_pFaceSet[j].ptemp_INMID[inmIdx].idx;

						}
						LOG_WRITE_A("ptemp_Vtx_Skin[%d] : %f, %f, %f  ",VCnt-1,m_pFaceSet[j].ptemp_Vtx_Skin[VCnt-1].pos.x, m_pFaceSet[j].ptemp_Vtx_Skin[VCnt-1].pos.y, m_pFaceSet[j].ptemp_Vtx_Skin[VCnt-1].pos.z);
						LOG_WRITE_A("ptemp_Idx[%d] : %d  ",inmIdx,(int)m_pFaceSet[j].ptemp_Idx[inmIdx]);
					}

				}

				// ���� �� face_set���� ���ؽ� ���ۿ� �ε��� ���۸� ���ɾ��ش�. 
				// ���� ���׸���� �ؽ��ĸ� ������ �ش�.
				for(int fsidx=0; fsidx<m_FaceSetCnt ;fsidx++) //fsidx : face_set_index
				{	
					//////////////////////////////////////////////////////////////////////////
					//���ؽ� ���ɱ�
					
					//���ؽ����� ����
					if( FAILED( _pDevice->CreateVertexBuffer( m_pFaceSet[fsidx].numvertex * sizeof(MY_VERTEX_SKINED), 0, MY_FVF_SKINED,D3DPOOL_DEFAULT, &m_pFaceSet[fsidx].vertexbuffer, NULL ) ) )
					{	write_log_file("���ؽ� ���� ���� ����"); assert(false);	}
					//���ؽ����� ��
					if( FAILED( m_pFaceSet[fsidx].vertexbuffer->Lock( 0, sizeof( MY_VERTEX_SKINED )*m_pFaceSet[fsidx].numvertex, ( void** )&m_pFaceSet[fsidx].m_pVtxBufPointer, 0 ) ) )
					{	write_log_file("���ؽ� ���� �� ����"); assert(false);}

					memcpy( m_pFaceSet[fsidx].m_pVtxBufPointer, m_pFaceSet[fsidx].ptemp_Vtx_Skin, sizeof( MY_VERTEX_SKINED )*m_pFaceSet[fsidx].numvertex );
					//���
					m_pFaceSet[fsidx].vertexbuffer->Unlock();

					//////////////////////////////////////////////////////////////////////////
					// �ε��� ��ɱ�
					// �ε������� ����
					if( FAILED( _pDevice->CreateIndexBuffer( m_pFaceSet[fsidx].TempINMIDCnt* sizeof(WORD), D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,
						D3DPOOL_MANAGED, &m_pFaceSet[fsidx].indexbuffer,NULL)
						) 
						)
					{ write_log_file("�ε��� ���� ���� ����");    assert(false);		  }
					//�ε������� ��
					
					if( FAILED( m_pFaceSet[fsidx].indexbuffer->Lock( 0, sizeof( WORD )*m_pFaceSet[fsidx].TempINMIDCnt , ( void** )&m_pFaceSet[fsidx].m_pIdxBufPointer, 0 ) ) )
					{	write_log_file("���ؽ� ���� �� ����"); assert(false);}

					memcpy( m_pFaceSet[fsidx].m_pIdxBufPointer, m_pFaceSet[fsidx].ptemp_Idx, sizeof( WORD )* m_pFaceSet[fsidx].TempINMIDCnt );

					m_pFaceSet[fsidx].indexbuffer->Unlock();

					//////////////////////////////////////////////////////////////////////////
					// ���׸��� ����
					m_pFaceSet[fsidx].m_Material = Make_Material(m,fsidx);
					// �ؽ��� ����
					if(m!=nullptr)
					{
						if(m->_GA_SubMtrl.GetAt(fsidx)->_Diffuse_Map.pBitmapName == NULL)
						{	LOG_WRITE_A("FILE : %s ,   LINE : %d",__FILE__ , __LINE__);
							write_log_file("GEOMOBJECT::Create_FaceSet_skin(), m->_Diffuse_Map.pBitmapName �� NULL �Դϴ�.");
						}
						else
						{
							TCHAR Path[512]=L"./image/";
							wcsncat_s(Path,  m->_GA_SubMtrl.GetAt(fsidx)->_Diffuse_Map.pBitmapName,wcslen( m->_GA_SubMtrl.GetAt(fsidx)->_Diffuse_Map.pBitmapName));
							wcscpy_s(m_pFaceSet[fsidx].pTexPath, Path);
							wcscpy_s(m_pFaceSet[fsidx].pTexName,m->_GA_SubMtrl.GetAt(fsidx)->_Diffuse_Map.pBitmapName);
							D3DXCreateTextureFromFile(_pDevice, Path, &m_pFaceSet[fsidx].m_pTex);
							ASSERT_IF_FAIL_MemAlloc(m_pFaceSet[fsidx].m_pTex);
						}
					}
				}


				//���ٰɾ����� �ӽ����� �޸� ����
				for(int i=0; i<m_FaceSetCnt ;  i++)
				{	
						SAFE_DELETE_ARR(m_pFaceSet[i].ptemp_Vtx_Skin)
						SAFE_DELETE_ARR(m_pFaceSet[i].ptemp_Vtx_Skin)
						SAFE_DELETE_ARR(m_pFaceSet[i].ptemp_INMID)
						SAFE_DELETE_ARR(m_pFaceSet[i].ptemp_Idx)
				}

		}
		//////////////////////////////////////////////////////////////////////////
		// �޽� �ٸ������ aseData���� �������� �ߴ� �޸𸮵��� ������ �ش�.

		SAFE_DELETE_ARR(tempVertex)
			SAFE_DELETE_ARR(tempIndices)

			SAFE_DELETE_ARR(pVertex)
			SAFE_DELETE_ARR(ptVertex)
			SAFE_DELETE_ARR(pcVertex)
			SAFE_DELETE_ARR(pNormal)
			SAFE_DELETE_ARR(ptFace)
			SAFE_DELETE_ARR(pFace_List)
}

//-----------------------------------------------------------------------------
//  SKINNED_OBJECT �׸��� �Լ�
//  direct9.0c���� �������ִ� ��Ʈ���� �ȷ�Ʈ�� �̿��� �׸���
//-----------------------------------------------------------------------------

void SKINNED_OBJECT::Draw(LPDIRECT3DDEVICE9 _device)
{
	if(m_HaveMesh == FALSE)
		return;

		for( int i = 0 ; i < pAseData->m_GA_BonesList.GetSize()  ; i++ )
		{	
			_device->SetTransform( D3DTS_WORLDMATRIX(i), &m_pMatPalette[i] );//��Ʈ���� �ȷ�Ʈ ���� 
		}

		if(m_FaceSetCnt >0)
		{
			for(int i=0; i<m_FaceSetCnt ; i++)
			{
				m_pFaceSet[i].DrawSkinning(_device); //direct9.0c���� �������ִ� ��Ʈ���� �ȷ�Ʈ�� �̿��� �׸���
			}
		}
		else
		{
			m_pFaceSet[0].DrawSkinning(_device);
		}

}

//-------------------------------------------------------------------------
// SKINNED_OBJECT::Draw()  
// ��Ű���� ���̴����� ���ִ� �Լ�
//
//-------------------------------------------------------------------------

void	SKINNED_OBJECT::Draw( LPDIRECT3DDEVICE9 _device, LPDIRECT3DVERTEXSHADER9  _pVShader,  LPDIRECT3DPIXELSHADER9  _pPShader)
{
	if(m_HaveMesh == FALSE)
		return;

	D3DXMATRIX matTranspose , tempTM;
	tempTM =  SHGlobal_GetCurScene_Camera->Get_ViewProjMat();
	D3DXMatrixTranspose( &matTranspose, &tempTM );
	// Load the combined model- mWorld matrix in registers c[4]-c[7] at vertex shader
	_device->SetVertexShaderConstantF( 4, (float*)matTranspose, 4 );	

	D3DXVECTOR4 CameraPos = SHGlobal_GetCurScene_Camera->GetPos();
	CameraPos.w = 0.0f;
	//c20�� ī�޶� ��ġ �־��ֱ�
	//_device->SetVertexShaderConstantF( 20, (float*)CameraPos, 1 ); 
	_device->SetPixelShaderConstantF( 20, (float*)CameraPos, 1 ); 
	for(int i= 0 ; i < pAseData->m_GA_BonesList.GetSize() ; i++)
	{
		D3DXMatrixTranspose( &matTranspose, &m_pMatPalette[i] );
		_device->SetVertexShaderConstantF( i*3+26, (float*)matTranspose, 3 );//��Ʈ�����ȷ�Ʈ�� 	
	}


	if(m_FaceSetCnt >0)
	{
		for(int i=0; i<m_FaceSetCnt ; i++)
		{
			m_pFaceSet[i].DrawSkinning(_device, _pVShader, _pPShader); //direct9.0c���� �������ִ� ��Ʈ���� �ȷ�Ʈ�� �̿��� �׸���
		}
	}
	else
	{
		m_pFaceSet[0].DrawSkinning(_device, _pVShader, _pPShader);
	}
}

//---------------------------------------------------------------------------------------------
//  Skinned Object ������Ʈ �Լ�
//---------------------------------------------------------------------------------------------

void SKINNED_OBJECT:: Update(float _dTime, D3DXMATRIX MoveTM)
{
	//��Ʈ���� �ȷ�Ʈ ������ֱ�
		for(int i= 0 ; i < pAseData->m_GA_BonesList.GetSize() ; i++)
		{
			m_pMatPalette[i] = pAseData->m_GA_BonesList.GetAt(i)->m_BoneIniMat * pAseData->m_GA_BonesList.GetAt(i)->m_MWorldTM ;
		}

}

//-----------------------------------------------------------------------------
// �θ�Ŭ���������� �ڽĿ��� �����Ҷ� �������縦 ���� ���������
//-----------------------------------------------------------------------------
GEOMOBJECT&	 SKINNED_OBJECT::DeepCopy(const GEOMOBJECT& rhs)
{
	GEOMOBJECT::DeepCopy(rhs);
	
		
	return *this;
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
// ���⼭���� HelperObject Ŭ���� �Լ�����
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


//---------------------------------------------------------------------------------------------
// HELPEROBJECT ������Ʈ �Լ�
//---------------------------------------------------------------------------------------------

void HELPEROBJECT::Update(float _dTime, D3DXMATRIX MoveTM)
{	
	m_AniTm_Pos = m_LTM_Pos;
	m_AniTm_Rot = m_LTM_Rot;
	m_AniTM_Scale = m_LTM_scale;
	//m_MWorldTM = NodeTM.NodeTM * MoveTM; //�ڽ��� ���� ���
	Animation(_dTime);

	if(m_pParent !=NULL)
	{//m_MWorldTM = m_MLocalTM *  m_pParent->m_MWorldTM ;
		m_MWorldTM = m_AniTM_Scale *m_ArcBallTM * m_AniTm_Rot* m_AniTm_Pos *  m_pParent->m_MWorldTM;
	}
	if(m_pParent ==NULL) //�ڱⰡ�θ��϶�
	{	//m_MWorldTM = m_MLocalTM * MoveTM;
		m_MWorldTM =  m_AniTM_Scale*m_ArcBallTM * m_AniTm_Rot* m_AniTm_Pos * MoveTM;
	}

	m_AniTm_Pos = m_LTM_Pos;
	m_AniTm_Rot = m_LTM_Rot;
	m_AniTM_Scale = m_LTM_scale;
	Animation(_dTime);


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
// ���⼭���� ShapeObject Ŭ���� �Լ�����
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


//---------------------------------------------------------------------------------------------
// ShapeObject ������Ʈ �Լ�
//---------------------------------------------------------------------------------------------

void SHAPEOBJECT::Update(float _dTime, D3DXMATRIX MoveTM)
{

	m_ViewTM = SHGlobal_GetCurScene_Camera->Get_ViewMat(); //falgonce = false;
	//}

	m_ProjTM = SHGlobal_GetCurScene_Camera->Get_ProjMat();
	m_AniTm_Pos = m_LTM_Pos;
	m_AniTm_Rot = m_LTM_Rot;
	m_AniTM_Scale = m_LTM_scale;
	//m_MWorldTM = NodeTM.NodeTM * MoveTM; //�ڽ��� ���� ���
	Animation(_dTime);

	if(m_pParent !=NULL)
	{//m_MWorldTM = m_MLocalTM *  m_pParent->m_MWorldTM ;
		m_MWorldTM = m_AniTM_Scale *m_ArcBallTM * m_AniTm_Rot * m_AniTm_Pos *  m_pParent->m_MWorldTM  ;
	}
	if(m_pParent ==NULL) //�ڱⰡ�θ��϶�
	{	//m_MWorldTM = m_MLocalTM * MoveTM;
		m_MWorldTM =  m_AniTM_Scale *m_ArcBallTM * m_AniTm_Rot * m_AniTm_Pos * MoveTM  ;

	}
}

//-----------------------------------------------------------------------------
// �̰����� Create_Line �Լ� �� �ҷ��´�.
//-----------------------------------------------------------------------------

void SHAPEOBJECT::Create_FaceSet(MATERIAL* m, LPDIRECT3DDEVICE9 _pDevice)
{
	Create_Line(_pDevice);
}

//-----------------------------------------------------------------------------
// �̰����� ���ΰ�ü�� ������ �ش�.
//-----------------------------------------------------------------------------

void SHAPEOBJECT::Create_Line(LPDIRECT3DDEVICE9 _pDevice)
{
	if(m_LineCnt > 0)
	{
		D3DXCreateLine(_pDevice, &m_pShape); // Line ����
	}

}

//-----------------------------------------------------------------------------
// ���� �׷��ֱ�
//-----------------------------------------------------------------------------

void SHAPEOBJECT::Draw( LPDIRECT3DDEVICE9 _device)
{	

	m_ShapeWorldTM = m_MWorldTM * m_ViewTM * m_ProjTM;
	for(int i=0; i<m_LineCnt ; i++)
	{
		m_pShape->SetWidth(1);
		m_pShape->Begin();

		m_pShape->DrawTransform(pSLine[i].pVert_List,  pSLine[i].Vert_Cnt,  &m_ShapeWorldTM,  D3DCOLOR_XRGB(128, 0, 0));
		m_pShape->End();
	}
} 


///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//
//  ���⼭����VWeight_List Ŭ���� �Լ�����
//
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------
// m_VertexW�� Weight������ ������������ �������ش�.
//---------------------------------------------------------------------------------------------

void VWeight_List::Sort_By_Weight()
{
	if(m_size>0)
	{
		for(int i=1 ; i < m_size ; i++)
		{
			for( int j=1 ; j< m_size ;  j++)
			{
				if(  m_VertexW[j-1] < m_VertexW[j] )
				{
					Vertex_Weight TempVW = m_VertexW[j];
					m_VertexW[j] = m_VertexW[j-1];
					m_VertexW[j-1] = TempVW;
				}
			}
		}
	}

	//ù��° ����(����ũ��_���������ϱ�!!)

	if(m_size > 4)
	{
		for(int i= m_size-1 ;  i > 3 ; i--)
		{
			m_VertexW[0].Weight += m_VertexW[i].Weight;//���� weight�� ū ù��°���� weight�� �����ֱ�
			m_size--;
		}
	}

	if(m_size > 4)
	{
		MessageBox(NULL,TEXT("void VWeight_List::Sort_By_Weight() : Sorting ���� : stlVec_VW �� ����� 4����Ů�ϴ�. "), TEXT("Failed"), MB_OK);  assert(false);
	}
}

//---------------------------------------------------------------------------------------------
//  �����Լ�
//---------------------------------------------------------------------------------------------

bool	VWeight_List::insert(const Vertex_Weight& _Val )
{
	if(m_size>MAX_VERTEX_WEIGHT )
	{
		MessageBox(NULL,TEXT("Sm_VertexW�� ����� MAX_VERTEX_WEIGHT�� �ʰ��մϴ�. "), TEXT("Failed"), MB_OK);  assert(false);
	}

	m_VertexW[m_size] = _Val;
	m_size++;

	return true;
}