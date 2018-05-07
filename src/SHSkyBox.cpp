/************************************************************************/
/*							[2014.8.2 ]
							��ī�̹ڽ�.cpp

*/
/************************************************************************/


#include "DXUT.h"
#include "Camera.h"


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
// ���⼭���� SkyBox�Լ�����
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
// ������ �Ҹ���
//-----------------------------------------------------------------------------

cSkyBox::cSkyBox() : m_pVtxBuf(nullptr), m_pIdxBuf(nullptr), m_pDevice(nullptr), m_pCubeTex(nullptr)
{

}

//-----------------------------------------------------------------------------
//
//
//-----------------------------------------------------------------------------
cSkyBox::~cSkyBox()
{
	SAFE_RELEASE(m_pVtxBuf)
	SAFE_RELEASE(m_pIdxBuf)
	SAFE_RELEASE(m_pCubeTex)   //ť���ؽ��� 
	
}

//-----------------------------------------------------------------------------
// [2014.8.2 ]
// ��ī�̹ڽ� �ʱ�ȭ �Լ�
//-----------------------------------------------------------------------------
HRESULT	cSkyBox::Init(LPDIRECT3DDEVICE9 _pDevice, wchar_t* _pPath)
{
	
	HRESULT hr;

	if(_pDevice == nullptr)
	{ 
		return S_FALSE;
	}
	m_pDevice = _pDevice;

	CUSTOM_VERTEX_Cube_Tex  tempVtx[24];
	//�ո�
	tempVtx[0].vtx = D3DXVECTOR3(-100, 100, -100);   tempVtx[0].texcoord  = D3DXVECTOR3(-1,1,-1);
	tempVtx[1].vtx = D3DXVECTOR3(-100,-100, -100);   tempVtx[1].texcoord = D3DXVECTOR3(-1,-1,-1) ;
	tempVtx[2].vtx = D3DXVECTOR3( 100, 100, -100);   tempVtx[2].texcoord =  D3DXVECTOR3(1,1,-1);
	tempVtx[3].vtx = D3DXVECTOR3(100,-100,-100);     tempVtx[3].texcoord =  D3DXVECTOR3(1,-1,-1);
	//�����ʸ�
	tempVtx[4].vtx  = D3DXVECTOR3( 100, 100, 100);  tempVtx[4].texcoord = D3DXVECTOR3(1,1,1);
	tempVtx[5].vtx  = D3DXVECTOR3(100,-100,100);    tempVtx[5].texcoord = D3DXVECTOR3(1,-1,1); 
	tempVtx[6].vtx  = D3DXVECTOR3(100,100,-100);    tempVtx[6].texcoord = D3DXVECTOR3(1,1,-1) ;
	tempVtx[7].vtx  = D3DXVECTOR3(100,-100,-100);    tempVtx[7].texcoord = D3DXVECTOR3(1,-1,-1) ;
	//�޸�
	tempVtx[8].vtx  =  D3DXVECTOR3(-100, 100, 100); tempVtx[8].texcoord=  D3DXVECTOR3(-1,1,1);
	tempVtx[9].vtx  = D3DXVECTOR3(-100,-100,100);   tempVtx[9].texcoord=  D3DXVECTOR3(-1,-1,1);
	tempVtx[10].vtx = D3DXVECTOR3(100,100,100);     tempVtx[10].texcoord= D3DXVECTOR3(1,1,1);
	tempVtx[11].vtx = D3DXVECTOR3(100,-100,100);    tempVtx[11].texcoord= D3DXVECTOR3(1,-1,1);
	//���ʸ�
	tempVtx[12].vtx = D3DXVECTOR3(-100, 100, -100); tempVtx[12].texcoord= D3DXVECTOR3(-1,1,-1);
	tempVtx[13].vtx = D3DXVECTOR3(-100,-100,-100);  tempVtx[13].texcoord= D3DXVECTOR3(-1,-1,-1);
	tempVtx[14].vtx = D3DXVECTOR3(-100,100,100);    tempVtx[14].texcoord= D3DXVECTOR3(-1,1,1);
	tempVtx[15].vtx = D3DXVECTOR3(-100,-100,100);   tempVtx[15].texcoord= D3DXVECTOR3(-1,-1,1);
	//����
	tempVtx[16].vtx =D3DXVECTOR3(-100, 100,-100);   tempVtx[16].texcoord= D3DXVECTOR3(-1,1,-1);
	tempVtx[17].vtx =D3DXVECTOR3(-100,100,100);     tempVtx[17].texcoord= D3DXVECTOR3(-1,1,1) ; 
	tempVtx[18].vtx =D3DXVECTOR3(100,100,-100);     tempVtx[18].texcoord= D3DXVECTOR3(1,1,-1);
	tempVtx[19].vtx =D3DXVECTOR3(100,100,100);      tempVtx[19].texcoord= D3DXVECTOR3(1,1,1);
	//�Ʒ���
	tempVtx[20].vtx =D3DXVECTOR3(-100,-100,100);     tempVtx[20].texcoord= D3DXVECTOR3(-1,-1,1); 
	tempVtx[21].vtx =D3DXVECTOR3(-100,-100,-100);    tempVtx[21].texcoord= D3DXVECTOR3(-1,-1,-1);  
	tempVtx[22].vtx =D3DXVECTOR3(100,-100,100);      tempVtx[22].texcoord= D3DXVECTOR3(1,-1,1);
	tempVtx[23].vtx =D3DXVECTOR3(100,-100,-100);     tempVtx[23].texcoord= D3DXVECTOR3(1,-1,-1);

	//�ε���
	WORD tempidx[36] = {
		0,2,1,
		1,2,3,
		4,5,6,
		6,5,7,
		8,9,10,
		9,11,10,
		12,13,14,
		13,15,14,
		16,17,18,
		17,19,18,
		20,21,22,
		21,23,22

	};
	//////////////////////////////////////////////////////////////////////////
	// Create vertex buffer   [2014.8.2 ]
	VOID* pVertices;
	//���ؽ����� ����
	if( FAILED( m_pDevice->CreateVertexBuffer( 24 * sizeof(CUSTOM_VERTEX_Cube_Tex), 0, MY_FVF,D3DPOOL_DEFAULT, &m_pVtxBuf, NULL ) ) )
	{	write_log_file("���ؽ� ���� ���� ����"); assert(false);	}
	//���ؽ����� ��
	if( FAILED( m_pVtxBuf->Lock( 0, sizeof( CUSTOM_VERTEX_Cube_Tex ) *24, ( void** )&pVertices, 0 ) ) )
	{	write_log_file("���ؽ� ���� �� ����"); assert(false);}

	memcpy( pVertices, tempVtx, sizeof( CUSTOM_VERTEX_Cube_Tex )*24 );
	//���
	m_pVtxBuf->Unlock();

	//////////////////////////////////////////////////////////////////////////
	// Create Index Buffer  [2014.8.2 ]
	
	if( FAILED( _pDevice->CreateIndexBuffer(36 * sizeof(WORD), D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,
											D3DPOOL_MANAGED, &m_pIdxBuf,NULL	)
			  ) 
	  )
	{ write_log_file("�ε��� ���� ���� ����");    assert(false);		  }
	//�ε������� ��
	void* pIndices;
	if( FAILED(m_pIdxBuf->Lock( 0, sizeof( WORD )*36 , ( void** )&pIndices, 0 ) ) )
	{	write_log_file("���ؽ� ���� �� ����"); assert(false);}

	memcpy( pIndices, tempidx, sizeof( WORD )* 36  );

	m_pIdxBuf->Unlock();
	
	//////////////////////////////////////////////////////////////////////////
	// Create CubeTexture   [2014.8.2 ]

	V_RETURN(D3DXCreateCubeTextureFromFile(m_pDevice,_pPath, &G_pCubeTex))
	 m_pCubeTex= G_pCubeTex;
	return S_OK;
}

//-----------------------------------------------------------------------------
// ������Ʈ �Լ�  [2014.8.2 ]
//-----------------------------------------------------------------------------
void	cSkyBox::OnFrameMove(float _dTime)
{

}

//-----------------------------------------------------------------------------
// [2014.8.2 ]
// �׸��� �Լ�
//-----------------------------------------------------------------------------
void cSkyBox::OnFrameRender(D3DXVECTOR3 _CameraPos)
{
	if ((m_pIdxBuf == nullptr) ||( m_pVtxBuf == nullptr))
	{  MessageBox(NULL, TEXT("��ī�̹ڽ� �׸������"), TEXT("Failed"), MB_OK);  LOG_WRITE_A("��ī�̹ڽ� �׸��� ����!!!@@@@@@@@@@@@");
	   return;
	}
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);


	//��ī�̹ڽ��� �׸����� z���ۿ� light�� ���ش�.
	m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE );			//Z���� ����
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	
	// �ؽ��ļ���
	m_pDevice->SetTexture(0, m_pCubeTex);

	D3DXMATRIX WorldTM ;
	//��ī�̹ڽ��� �׻� ī�޶� ����ٳ���Ѵ�.
	D3DXMatrixTranslation(&WorldTM, _CameraPos.x, _CameraPos.y, _CameraPos.z); //	[2014.8.2 ]

	m_pDevice->SetTransform(D3DTS_WORLD, &WorldTM);
	m_pDevice->SetStreamSource(0, m_pVtxBuf, 0, sizeof(CUSTOM_VERTEX_Cube_Tex));
	m_pDevice->SetFVF(CubeTexFVF);
	m_pDevice->SetIndices(m_pIdxBuf);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, true);            //����Ʈ �ٽ� Ű��
	m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE );			//Z���� �ѱ�
	
	
}