/************************************************************************/
/*							[2014.7.23 ]                          */
//							  연기 효과 
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
// 여기서부터 cSmoke 클래스 맴버 함수정의
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
// 생성자 소멸자 [2014.7.23 ]
//-----------------------------------------------------------------------------
cSmoke::cSmoke(D3DXVECTOR3* origin, int numParticles, float Maxsize)
{
	m_origin		= *origin;
	m_size			= 1.0f;// sprite 사이즈
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
// 각 파티클의 속성을 초기화 해주는 함수
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
// 분출가스 초기화 함수  [2014.7.26 ]
//_EmitPower : 연기 분출 강도 , 
//_texFileName: 사용할 텍스쳐 이름
// numParticles 파티클 최대 개수 
// sprMaxsize : 스프라이트 max사이즈
//-----------------------------------------------------------------------------
int	cSmoke::Init(  LPDIRECT3DDEVICE9 _pDevice , LPCWSTR _texFileName, float _EmitPower/* = 10.0f*/,
				   int numParticles/* = 256*/, float sprMaxsize /*= 100.0f*/)	
{
	m_origin		= D3DXVECTOR3(0,0,0);
	m_size			= 1.0f;			  // sprite 처음 시작 사이즈
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
// smoke 업데이트 함수
//-----------------------------------------------------------------------------
void cSmoke::update(float timeDelta)
{
	if(timeDelta >1.0f)
		timeDelta = 0.0f;
	float X,Y,Z = 0.0f;
	std::list<Attribute>::iterator i;

	for(i = stlList_Attributes.begin(); i != stlList_Attributes.end(); i++)
	{	//죽어있는 파티클을 발견할때까지 돌면서 하나만 살리고 루프 빠져나오기
		if( i->_isAlive == false )
		{			
			i->_isAlive = true;
			i->_position  = m_origin; //이좌표는 외부에서 매번 업데이트 되어야 한다.
			X=GetRandomFloat(-5.0f, 5.0f);//랜덤으로 방향 정하기
			Y=GetRandomFloat(-5.0f, 5.0f);
			Z=GetRandomFloat(-5.0f, 5.0f);

			i->_velocity.x = (m_EmissDir.x * m_EmitPower) - X;
			i->_velocity.y = (m_EmissDir.y * m_EmitPower) - Y;
			i->_velocity.z = (m_EmissDir.z * m_EmitPower) - Z;
			i->_age      = 0.0f;
			break;		
		}
	}

	//루프를 돌면서 살아있는것만 업데이트 해준다.
	for(i = stlList_Attributes.begin(); i != stlList_Attributes.end(); i++)
	{	// only update living particles
		if( i->_isAlive )
		{			
			//현재포지션을 업데이트 해준다.
			i->_position += i->_velocity/**0.01f*/ * timeDelta;
			//매프레임마다 age를 더해준다.
			i->_age += timeDelta; 
			//라이프타임이 다된것은 죽인다.
			if(i->_age > i->_lifeTime) // kill 
				i->_isAlive = false;

			//매프레임마다. 사이즈를 점점 증가시킨다. [2014.7.24 ]
			if(m_size< m_SprMaxSize)
			{
				m_size+= timeDelta;
			}

		}
	}
}

//---------------------------------------------------------------------------------------------
// 렌더그리기 전에 미리 설정하는 부분 [2014.7.24 ]
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
// 렌더그리고 나서 그후에 설정하는 부분 [2014.7.24 ]
//---------------------------------------------------------------------------------------------
void cSmoke::postRender()
{
	PSystem::postRender();

	//_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	// _device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// read, but don't write particles to z-buffer
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}
