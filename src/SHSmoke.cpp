/************************************************************************/
/*							[2014.7.23 ]                          */
//							  ���� ȿ�� 
//					
/************************************************************************/

#include "DXUT.h"
#include "SHSmoke.h"


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
// ���⼭���� cSmoke Ŭ���� �ɹ� �Լ�����
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
// ������ �Ҹ��� [2014.7.23 ]
//-----------------------------------------------------------------------------
cSmoke::cSmoke(D3DXVECTOR3* origin, int numParticles, float Maxsize)
{
	m_origin		= *origin;
	m_size			= 1.0f;// sprite ������
	m_SprMaxSize		= Maxsize; //[2014.7.24 ]
	m_VtxBuf_Size	= 512;
	m_VtxBuf_Offset	= 0;   
	m_VtxBuf_BatchSize = 512; 

	for(int i = 0; i < numParticles; i++)
		addParticle();
}

cSmoke::~cSmoke()
{
	
}



//-----------------------------------------------------------------------------
// [2014.7.23 ]
// �� ��ƼŬ�� �Ӽ��� �ʱ�ȭ ���ִ� �Լ�
//-----------------------------------------------------------------------------
void cSmoke::Reset_Attribute(Attribute* attribute,int i)
{
	attribute->_isAlive  = false;
	attribute->_position = m_origin;
	float X,Y,Z = 0.0f;
	float C=0.0f;
	X=GetRandomFloat(-5.0f, 5.0f);
	Y=GetRandomFloat(-5.0f, 5.0f);
	Z=GetRandomFloat(-5.0f, 5.0f);

	attribute->_velocity.x = m_EmissDir.x -X;
	attribute->_velocity.y = m_EmissDir.x -Y;
	attribute->_velocity.z = m_EmissDir.x -Z;



	//GetRandomVector(&attribute->_velocity,	&min,	&max);
	// normalize to make spherical
//	D3DXVec3Normalize(	&attribute->_velocity,	&attribute->_velocity);

	//attribute->_velocity *= 90.0f;

	/*attribute->_color = D3DXCOLOR(
		d3d::GetRandomFloat(0.0f, 1.0f),
		d3d::GetRandomFloat(0.0f, 1.0f),
		d3d::GetRandomFloat(0.0f, 1.0f),
		1.0f);*/
	C=GetRandomFloat(0.7f, 1.0f);
	attribute->_color = D3DXCOLOR(C,C,C,1.0f);

	attribute->_age      = 0.0f;
	attribute->_lifeTime = 3.6f; // live for 3.6 second

}

//-----------------------------------------------------------------------------
// ���Ⱑ�� �ʱ�ȭ �Լ�  [2014.7.26 ]
//_EmitPower : ���� ���� ���� , 
//_texFileName: ����� �ؽ��� �̸�
// numParticles ��ƼŬ �ִ� ���� 
// sprMaxsize : ��������Ʈ max������
//-----------------------------------------------------------------------------
int	cSmoke::Init(  LPDIRECT3DDEVICE9 _pDevice , LPCWSTR _texFileName, float _EmitPower/* = 10.0f*/,
				   int numParticles/* = 256*/, float sprMaxsize /*= 100.0f*/)	
{
	m_origin		= D3DXVECTOR3(0,0,0);
	m_size			= 1.0f;			  // sprite ó�� ���� ������
	m_SprMaxSize	= sprMaxsize;	  //[2014.7.24 ]
	m_VtxBuf_Size	= 1024;
	m_VtxBuf_Offset	= 0;   
	m_VtxBuf_BatchSize = 512; 

	for(int i = 0; i < numParticles; i++)
	{	addParticle();		}

	 PSystem::init(_pDevice , _texFileName);

	 m_EmitPower = _EmitPower;

	 return 1;
}


//-----------------------------------------------------------------------------
// [2014.7.23 ]
// smoke ������Ʈ �Լ�
//-----------------------------------------------------------------------------
void cSmoke::update(float timeDelta)
{
	if(timeDelta >1.0f)
		timeDelta = 0.0f;
	float X,Y,Z = 0.0f;
	std::list<Attribute>::iterator i;

	for(i = stlList_Attributes.begin(); i != stlList_Attributes.end(); i++)
	{	//�׾��ִ� ��ƼŬ�� �߰��Ҷ����� ���鼭 �ϳ��� �츮�� ���� ����������
		if( i->_isAlive == false )
		{			
			i->_isAlive = true;
			i->_position  = m_origin; //����ǥ�� �ܺο��� �Ź� ������Ʈ �Ǿ�� �Ѵ�.
			X=GetRandomFloat(-5.0f, 5.0f);//�������� ���� ���ϱ�
			Y=GetRandomFloat(-5.0f, 5.0f);
			Z=GetRandomFloat(-5.0f, 5.0f);

			i->_velocity.x = (m_EmissDir.x * m_EmitPower) - X;
			i->_velocity.y = (m_EmissDir.y * m_EmitPower) - Y;
			i->_velocity.z = (m_EmissDir.z * m_EmitPower) - Z;
			i->_age      = 0.0f;
			break;		
		}
	}

	//������ ���鼭 ����ִ°͸� ������Ʈ ���ش�.
	for(i = stlList_Attributes.begin(); i != stlList_Attributes.end(); i++)
	{	// only update living particles
		if( i->_isAlive )
		{			
			//������������ ������Ʈ ���ش�.
			i->_position += i->_velocity/**0.01f*/ * timeDelta;
			//�������Ӹ��� age�� �����ش�.
			i->_age += timeDelta; 
			//������Ÿ���� �ٵȰ��� ���δ�.
			if(i->_age > i->_lifeTime) // kill 
				i->_isAlive = false;

			//�������Ӹ���. ����� ���� ������Ų��. [2014.7.24 ]
			if(m_size< m_SprMaxSize)
			{
				m_size+= timeDelta;
			}

		}
	}
}

//---------------------------------------------------------------------------------------------
// �����׸��� ���� �̸� �����ϴ� �κ� [2014.7.24 ]
//---------------------------------------------------------------------------------------------

void cSmoke::preRender()
{
	PSystem::preRender();

	//_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	// _device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// read, but don't write particles to z-buffer
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
}


//---------------------------------------------------------------------------------------------
// �����׸��� ���� ���Ŀ� �����ϴ� �κ� [2014.7.24 ]
//---------------------------------------------------------------------------------------------
void cSmoke::postRender()
{
	PSystem::postRender();

	//_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	// _device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// read, but don't write particles to z-buffer
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}
