/************************************************************************/
/*							2014.7.23                             */
//							  ��ƼŬ Ŭ���� 
//
/************************************************************************/

#include "DXUT.h"
#include "SHParticle.h"

////////////////////////////////////////////////////////////////////////
// �Ϲ��Լ�


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
// ���⼭���� PSystem Ŭ���� ����Լ� ����
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
// ������ �Ҹ���
//---------------------------------------------------------------------------------------------
PSystem::PSystem()
{
	m_pDevice		= nullptr;
	m_pVtxBuff		= nullptr;
	m_pTex			= nullptr;

}


PSystem::~PSystem()
{
	SAFE_RELEASE(m_pVtxBuff)
	SAFE_RELEASE(m_pTex)
}


//---------------------------------------------------------------------------------------------
// �ʱ�ȭ �Լ�
// _texFileName : ����� �ؽ��� �̸� 
//---------------------------------------------------------------------------------------------

bool PSystem::init(IDirect3DDevice9* _device,  LPCWSTR _texFileName)
{
	// vertex buffer's size does not equal the number of particles in our system.  We
	// use the vertex buffer to draw a portion of our particles at a time.  The arbitrary
	// size we choose for the vertex buffer is specified by the m_VtxBuf_Size variable.

	m_pDevice = _device; // save a ptr to the device

	HRESULT hr = 0;

	hr = m_pDevice->CreateVertexBuffer(	m_VtxBuf_Size * sizeof(CUSTOM_VTX_XYZ_COLOR_),
										D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
										D3DFVF_XYZ_COLOR,
										D3DPOOL_DEFAULT, // D3DPOOL_MANAGED can't be used with D3DUSAGE_DYNAMIC 
										&m_pVtxBuff,
										0);

	if(FAILED(hr))
	{
		::MessageBox(0, L"��ƼŬ���ý����۸���� - FAILED", L"PSystem", 0);
		return false;
	}

	hr = D3DXCreateTextureFromFile(	m_pDevice,	_texFileName,	&m_pTex);

	if(FAILED(hr))
	{
		::MessageBox(0, L"��ƼŬ�ؽ���- ����", L"PSystem", 0);
		return false;
	}

	D3DXMatrixIdentity(&m_MatIdentity);

	

	return true;
}




//---------------------------------------------------------------------------------------------
//  ��� ��ƼŬ ���ڵ��� �Ӽ��� ó������ �����Ѵ�.
//---------------------------------------------------------------------------------------------
void	PSystem::Reset_All()
{
	std::list<Attribute>::iterator i;
	int a=0;
	for(i = stlList_Attributes.begin(); i != stlList_Attributes.end(); i++)
	{
		Reset_Attribute( &(*i) ,a);
		a++;
	}
}




//-----------------------------------------------------------------------------
// ��ƼŬ �Ӽ��� �������� stlList_Attributes �� �߰��Ѵ�.
//-----------------------------------------------------------------------------
void PSystem::addParticle(int j)
{
	Attribute attribute;

	Reset_Attribute(&attribute,j);	// �ڽ�Ŭ�������� override�� �����Լ�

	stlList_Attributes.push_back(attribute);
}




//-----------------------------------------------------------------------------
// render �����ϱ� ���� ����
//-----------------------------------------------------------------------------

void PSystem::preRender()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, false);
	m_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	m_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true); 
	m_pDevice->SetRenderState(D3DRS_POINTSIZE, FtoDw(m_size));
	m_pDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));

	// control the size of the particle relative to distance
	m_pDevice->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	m_pDevice->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	m_pDevice->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));

	// use alpha from texture
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState( D3DRS_ALPHAREF,  0x08);
	m_pDevice->SetRenderState( D3DRS_ALPHAFUNC,  D3DCMP_GREATEREQUAL);
}




//-----------------------------------------------------------------------------
// render �������� ����
//-----------------------------------------------------------------------------
void PSystem::postRender()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING,          true);
	m_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE,  false);
	m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, false);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,  false);
}



//-----------------------------------------------------------------------------
// �����Լ�
//-----------------------------------------------------------------------------
void PSystem::render()
{	
	// Remarks:  The render method works by filling a section of the vertex buffer with data,
	//           then we render that section.  While that section is rendering we lock a new
	//           section and begin to fill that section.  Once that sections filled we render it.
	//           This process continues until all the particles have been drawn.  The benifit
	//           of this method is that we keep the video card and the CPU busy.  

	if( !stlList_Attributes.empty() )
	{
		// set render states
		preRender();

		m_pDevice->SetTransform(D3DTS_WORLD, &m_MatIdentity);
		m_pDevice->SetTexture(0, m_pTex);
		m_pDevice->SetFVF(D3DFVF_XYZ_COLOR);
		m_pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(CUSTOM_VTX_XYZ_COLOR_));

		// render batches one by one
		// start at beginning if we're at the end of the vb
		if(m_VtxBuf_Offset >= m_VtxBuf_Size)
			m_VtxBuf_Offset = 0;

		CUSTOM_VTX_XYZ_COLOR_* v = nullptr;
		//////////////////////////////////////////////////////////////////////////
		//���⼭ ���� ���ؽ����� �� �ɾ��ֱ�
		m_pVtxBuff->Lock(m_VtxBuf_Offset* sizeof( CUSTOM_VTX_XYZ_COLOR_ ),m_VtxBuf_BatchSize * sizeof( CUSTOM_VTX_XYZ_COLOR_ ),
						(void**)&v,	m_VtxBuf_Offset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		DWORD numParticlesInBatch = 0;

		// Until all particles have been rendered.
		std::list<Attribute>::iterator i;
		for(i = stlList_Attributes.begin(); i != stlList_Attributes.end(); i++)
		{
			if( i->_isAlive )
			{
				// Copy a batch of the living particles to the
				// next vertex buffer segment
				v->_position = i->_position;
				v->_color    = (D3DCOLOR)i->_color;
				v++; // next element;

				numParticlesInBatch++; //increase batch counter

				// if this batch full?
				if(numParticlesInBatch == m_VtxBuf_BatchSize) 
				{
					// Draw the last batch of particles that was copied to the vertex buffer. 
					m_pVtxBuff->Unlock();
					m_pDevice->DrawPrimitive(D3DPT_POINTLIST,  m_VtxBuf_Offset,  m_VtxBuf_BatchSize);

					// While that batch is drawing, start filling the next batch with particles.
					// move the offset to the start of the next batch
					m_VtxBuf_Offset += m_VtxBuf_BatchSize; 

					// don't offset into memory thats outside the vb's range.
					// If we're at the end, start at the beginning.
					if(m_VtxBuf_Offset >= m_VtxBuf_Size) 
						m_VtxBuf_Offset = 0;       

					m_pVtxBuff->Lock(m_VtxBuf_Offset    * sizeof( CUSTOM_VTX_XYZ_COLOR_ ),
									 m_VtxBuf_BatchSize * sizeof( CUSTOM_VTX_XYZ_COLOR_ ),
									(void**)&v,	m_VtxBuf_Offset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

					numParticlesInBatch = 0; // reset for new batch
				}	
			}
		}
		m_pVtxBuff->Unlock();
		// its possible that the LAST batch being filled never 
		// got rendered because the condition 
		// (numParticlesInBatch == m_VtxBuf_BatchSize) would not have
		// been satisfied.  We draw the last partially filled batch now.
		if( numParticlesInBatch )
		{
			m_pDevice->DrawPrimitive(D3DPT_POINTLIST, m_VtxBuf_Offset,	numParticlesInBatch);
		}

		// next block
		m_VtxBuf_Offset += m_VtxBuf_BatchSize; 

		// reset render states
		postRender();
	}
}

//-----------------------------------------------------------------------------
// �������Ȯ���ϴ� �Լ� [2014.7.23 ]
//-----------------------------------------------------------------------------
bool PSystem::isEmpty()
{
	return stlList_Attributes.empty();
}


//-----------------------------------------------------------------------------
// ȭ�鿡 �׷���  ��� ��ƼŬ�� �����ֱ� ���ߴ��� Ȯ���ϴ� �Լ� [2014.7.23 ]
//-----------------------------------------------------------------------------

bool PSystem::isDead()
{
	std::list<Attribute>::iterator i;
	for(i = stlList_Attributes.begin(); i != stlList_Attributes.end(); i++)
	{
		// is there at least one living particle?  If yes,
		// the system is not dead.
		if( i->_isAlive )
			return false;
	}
	// no living particles found, the system must be dead.
	return true;
}

//-----------------------------------------------------------------------------
// [2014.7.23 ]
//  �ֱⰡ �ٵ� ��ƼŬ�� stlList����  �����Ѵ�.
//-----------------------------------------------------------------------------

void PSystem::removeDeadParticles()
{
	std::list<Attribute>::iterator i;

	i = stlList_Attributes.begin();

	while( i != stlList_Attributes.end() )
	{
		if( i->_isAlive == false )
		{
			// erase returns the next iterator, so no need to
			// incrememnt to the next one ourselves.
			i = stlList_Attributes.erase(i); 
		}
		else
		{
			i++; // next in list
		}
	}
}

/****************************************************** End *****************************************************/