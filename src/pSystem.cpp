//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: pSystem.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Represents a general particle system.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "DirectX.h"
#include "pSystem.h"

const DWORD Particle::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
int once=0;
float GetRandomFloat(float lowBound, float highBound)
{
	if( lowBound >= highBound ) // bad input
		return lowBound;

	// get random float in [0, 1] interval
	float f = (rand() % 10000) * 0.0001f; 

	// return float in [lowBound, highBound] interval. 
	return (f * (highBound - lowBound)) + lowBound; 
}

void GetRandomVector(
	  D3DXVECTOR3* out,
	  D3DXVECTOR3* min,
	  D3DXVECTOR3* max)
{
	out->x = GetRandomFloat(min->x, max->x);
	out->y = GetRandomFloat(min->y, max->y);
	out->z = GetRandomFloat(min->z, max->z);
}

DWORD FtoDw(float f)
{
	return *((DWORD*)&f);
}
BoundingBox::BoundingBox()
{
	// infinite small 
	_min.x = INFINITY;
	_min.y = INFINITY;
	_min.z = INFINITY;

	_max.x = -INFINITY;
	_max.y = -INFINITY;
	_max.z = -INFINITY;
}

bool BoundingBox::isPointInside(D3DXVECTOR3& p)
{
	if( p.x >= _min.x && p.y >= _min.y && p.z >= _min.z &&
		p.x <= _max.x && p.y <= _max.y && p.z <= _max.z )
	{
		return true;
	}
	else
	{
		return false;
	}
}
PSystem::PSystem()
{
	_device = 0;
	_vb     = 0;
	_tex    = 0;
}

PSystem::~PSystem()
{
	
}
void PSystem::Lost()
{
	SAFE_RELEASE(_vb);
	SAFE_RELEASE(_tex);
}


bool PSystem::init(IDirect3DDevice9* device,  LPCWSTR texFileName)
{
	// vertex buffer's size does not equal the number of particles in our system.  We
	// use the vertex buffer to draw a portion of our particles at a time.  The arbitrary
	// size we choose for the vertex buffer is specified by the _vbSize variable.

	_device = device; // save a ptr to the device

	HRESULT hr = 0;

	hr = device->CreateVertexBuffer(
		_vbSize * sizeof(Particle),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		Particle::FVF,
		D3DPOOL_DEFAULT, // D3DPOOL_MANAGED can't be used with D3DUSAGE_DYNAMIC 
		&_vb,
		0);
	
	if(FAILED(hr))
	{
		::MessageBox(0, L"파티클버택스버퍼만들기 - FAILED", L"PSystem", 0);
		return false;
	}

	hr = D3DXCreateTextureFromFile(
		device,
		texFileName,
		&_tex);

	if(FAILED(hr))
	{
		::MessageBox(0, L"파티클텍스쳐- 실패", L"PSystem", 0);
		return false;
	}

	return true;
}

void PSystem::reset()
{
	std::list<Attribute>::iterator i;
	int a=0;
	for(i = _particles.begin(); i != _particles.end(); i++)
	{
		resetParticle( &(*i) ,a);
		a++;
	}
}

void PSystem::addParticle(int j)
{
	Attribute attribute;

	resetParticle(&attribute,j);

	_particles.push_back(attribute);
}

void PSystem::preRender()
{
	_device->SetRenderState(D3DRS_LIGHTING, false);
	_device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	_device->SetRenderState(D3DRS_POINTSCALEENABLE, true); 
	_device->SetRenderState(D3DRS_POINTSIZE, FtoDw(_size));
	_device->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));

	// control the size of the particle relative to distance
	_device->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	_device->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	_device->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));
		
	// use alpha from texture
	_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    _device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	_device->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);
	_device->SetRenderState( D3DRS_ALPHAREF,  0x08);
    _device->SetRenderState( D3DRS_ALPHAFUNC,  D3DCMP_GREATEREQUAL);
}

void PSystem::postRender()
{
	_device->SetRenderState(D3DRS_LIGHTING,          true);
	_device->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	_device->SetRenderState(D3DRS_POINTSCALEENABLE,  false);
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE,  false);
}

void PSystem::render()
{
	//
	// Remarks:  The render method works by filling a section of the vertex buffer with data,
	//           then we render that section.  While that section is rendering we lock a new
	//           section and begin to fill that section.  Once that sections filled we render it.
	//           This process continues until all the particles have been drawn.  The benifit
	//           of this method is that we keep the video card and the CPU busy.  

	if( !_particles.empty() )
	{
		//
		// set render states
		//

		preRender();
		
		_device->SetTexture(0, _tex);
		_device->SetFVF(Particle::FVF);
		_device->SetStreamSource(0, _vb, 0, sizeof(Particle));

		//
		// render batches one by one
		//

		// start at beginning if we're at the end of the vb
		if(_vbOffset >= _vbSize)
			_vbOffset = 0;

		Particle* v = 0;

		_vb->Lock(
			_vbOffset    * sizeof( Particle ),
			_vbBatchSize * sizeof( Particle ),
			(void**)&v,
			_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		DWORD numParticlesInBatch = 0;

		//
		// Until all particles have been rendered.
		//
		std::list<Attribute>::iterator i;
		for(i = _particles.begin(); i != _particles.end(); i++)
		{
			if( i->_isAlive )
			{
				//
				// Copy a batch of the living particles to the
				// next vertex buffer segment
				//
				v->_position = i->_position;
				v->_color    = (D3DCOLOR)i->_color;
				v++; // next element;

				numParticlesInBatch++; //increase batch counter

				// if this batch full?
				if(numParticlesInBatch == _vbBatchSize) 
				{
					//
					// Draw the last batch of particles that was
					// copied to the vertex buffer. 
					//
					_vb->Unlock();

					_device->DrawPrimitive(
						D3DPT_POINTLIST,
						_vbOffset,
						_vbBatchSize);

					//
					// While that batch is drawing, start filling the
					// next batch with particles.
					//

					// move the offset to the start of the next batch
					_vbOffset += _vbBatchSize; 

					// don't offset into memory thats outside the vb's range.
					// If we're at the end, start at the beginning.
					if(_vbOffset >= _vbSize) 
						_vbOffset = 0;       

					_vb->Lock(
						_vbOffset    * sizeof( Particle ),
						_vbBatchSize * sizeof( Particle ),
						(void**)&v,
						_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

					numParticlesInBatch = 0; // reset for new batch
				}	
			}
		}

		_vb->Unlock();

		// its possible that the LAST batch being filled never 
		// got rendered because the condition 
		// (numParticlesInBatch == _vbBatchSize) would not have
		// been satisfied.  We draw the last partially filled batch now.
		
		if( numParticlesInBatch )
		{
			_device->DrawPrimitive(
				D3DPT_POINTLIST,
				_vbOffset,
				numParticlesInBatch);
		}

		// next block
		_vbOffset += _vbBatchSize; 

		//
		// reset render states
		//

		postRender();
	}
}

bool PSystem::isEmpty()
{
	return _particles.empty();
}

bool PSystem::isDead()
{
	std::list<Attribute>::iterator i;
	for(i = _particles.begin(); i != _particles.end(); i++)
	{
		// is there at least one living particle?  If yes,
		// the system is not dead.
		if( i->_isAlive )
			return false;
	}
	// no living particles found, the system must be dead.
	return true;
}

void PSystem::removeDeadParticles()
{
	std::list<Attribute>::iterator i;

	i = _particles.begin();

	while( i != _particles.end() )
	{
		if( i->_isAlive == false )
		{
			// erase returns the next iterator, so no need to
		    // incrememnt to the next one ourselves.
			i = _particles.erase(i); 
		}
		else
		{
			i++; // next in list
		}
	}
}

//*****************************************************************************
// Snow System
//***************

Snow::Snow(BoundingBox* boundingBox, int numParticles)
{
	_boundingBox   = *boundingBox;
	_size          = 0.25f;
	_vbSize        = 2048;
	_vbOffset      = 0; 
	_vbBatchSize   = 512; 
	
	for(int i = 0; i < numParticles; i++)
		addParticle();
}

void Snow::resetParticle(Attribute* attribute,int i)
{
	attribute->_isAlive  = true;

	// get random x, z coordinate for the position of the snow flake.
	GetRandomVector(
		&attribute->_position,
		&_boundingBox._min,
		&_boundingBox._max);

	// no randomness for height (y-coordinate).  Snow flake
	// always starts at the top of bounding box.
	attribute->_position.y = _boundingBox._max.y; 

	// snow flakes fall downwards and slightly to the left
	attribute->_velocity.x = GetRandomFloat(0.0f, 1.0f) * -3.0f;
	attribute->_velocity.y = GetRandomFloat(0.0f, 1.0f) * -10.0f;
	attribute->_velocity.z = 0.0f;

	// white snow flake
	attribute->_color = WHITE;
}

void Snow::update(float timeDelta)
{
	std::list<Attribute>::iterator i;
	for(i = _particles.begin(); i != _particles.end(); i++)
	{
		i->_position += i->_velocity * timeDelta;

		// is the point outside bounds?
		if( _boundingBox.isPointInside( i->_position ) == false ) 
		{
			// nope so kill it, but we want to recycle dead 
			// particles, so respawn it instead.
			resetParticle( &(*i) );
		}
	}
}

//*****************************************************************************
// mAseInfo System
//********************

Explosion::Explosion(D3DXVECTOR3* origin, int numParticles)
{
	_origin        = *origin;
	_size          = 14.0f;
	_vbSize        = 2048;
	_vbOffset      = 0;   
	_vbBatchSize   = 512; 

	for(int i = 0; i < numParticles; i++)
		addParticle();
}

void Explosion::resetParticle(Attribute* attribute,int i)
{
	attribute->_isAlive  = true;
	attribute->_position = _origin;
	float C=0.0f;
	D3DXVECTOR3 min = D3DXVECTOR3(-1.0f, 0.0f, -1.0f);
	D3DXVECTOR3 max = D3DXVECTOR3( 1.0f,  1.0f,  1.0f);

	GetRandomVector(
		&attribute->_velocity,
		&min,
		&max);

	// normalize to make spherical
	D3DXVec3Normalize(
		&attribute->_velocity,
		&attribute->_velocity);

	attribute->_velocity *= 90.0f;

	/*attribute->_color = D3DXCOLOR(
		d3d::GetRandomFloat(0.0f, 1.0f),
		d3d::GetRandomFloat(0.0f, 1.0f),
		d3d::GetRandomFloat(0.0f, 1.0f),
		1.0f);*/
	C=GetRandomFloat(0.3f, 1.0f);
	attribute->_color = D3DXCOLOR(C,C,C,1.0f);

	attribute->_age      = 0.0f;
	attribute->_lifeTime = 3.6f; // lives for 2 seconds
}

void Explosion::update(float timeDelta)
{
	std::list<Attribute>::iterator i;

	for(i = _particles.begin(); i != _particles.end(); i++)
	{
		// only update living particles
		if( i->_isAlive )
		{
			
			if(i->_age>0.04f)
			{	i->_position += i->_velocity*0.01f * timeDelta;	}
			else
			{i->_position += i->_velocity * timeDelta;}
			
			i->_age += timeDelta;

			if(i->_age > i->_lifeTime) // kill 
				i->_isAlive = false;
		}
	}
}

void Explosion::preRender()
{
	PSystem::preRender();

	//_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
   // _device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// read, but don't write particles to z-buffer
	_device->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void Explosion::postRender()
{
	PSystem::postRender();

	_device->SetRenderState(D3DRS_ZWRITEENABLE, true);
}
// Explosion flash System
//********************

Flash::Flash(D3DXVECTOR3* origin, int numParticles)
{
	_origin        = *origin;
	_size          = 20.9f;
	_vbSize        = 2048;
	_vbOffset      = 0;   
	_vbBatchSize   = 512; 

	for(int i = 0; i < numParticles; i++)
		addParticle();
}

void Flash::resetParticle(Attribute* attribute,int i)
{
	attribute->_isAlive  = true;
	attribute->_position = _origin;

	D3DXVECTOR3 min = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	D3DXVECTOR3 max = D3DXVECTOR3( 1.0f,  1.0f,  1.0f);

	GetRandomVector(
		&attribute->_velocity,
		&min,
		&max);

	// normalize to make spherical
	D3DXVec3Normalize(
		&attribute->_velocity,
		&attribute->_velocity);

	attribute->_velocity *= 3.0f;

	attribute->_color = D3DXCOLOR(
		GetRandomFloat(0.0f, 1.0f),
		GetRandomFloat(0.0f, 1.0f),
		GetRandomFloat(0.0f, 1.0f),
		1.0f);

	attribute->_age      = 0.0f;
	attribute->_lifeTime = 0.3f; // lives for 2 seconds
}

void Flash::update(float timeDelta)
{
	std::list<Attribute>::iterator i;

	for(i = _particles.begin(); i != _particles.end(); i++)
	{
		// only update living particles
		if( i->_isAlive )
		{
			i->_position += i->_velocity * timeDelta;

			i->_age += timeDelta;

			if(i->_age > i->_lifeTime) // kill 
				i->_isAlive = false;
		}
	}
}

void Flash::preRender()
{
	PSystem::preRender();

	_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    _device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// read, but don't write particles to z-buffer
	_device->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void Flash::postRender()
{
	PSystem::postRender();

	_device->SetRenderState(D3DRS_ZWRITEENABLE, true);
}
//*****************************************************************************

fog::fog(D3DXVECTOR3* origin, int numParticles)
{
	_origin        = *origin;
	_size          = 1.8f;
	_vbSize        = 2048;
	_vbOffset      = 0;   
	_vbBatchSize   = 512; 
	_numparticle	=numParticles;

	for(int i = 0; i < numParticles; i++)
		addParticle(i);
}


void fog::resetParticle(Attribute* attribute,int i)
{
	attribute->_isAlive  = true;
	attribute->_position = _origin;
	float C=0.0f;
	D3DXVECTOR3 min;
	D3DXVECTOR3 max;
	
	
	if(i>(_numparticle/2))
	{
	 min = D3DXVECTOR3(-1.3f, -1.3f, 2.0f);
	 max = D3DXVECTOR3( 1.3f,  1.3f, 5.0f);
	}
	else
	{    min = D3DXVECTOR3(-1.3f, -1.3f, -2.0f);
		 max = D3DXVECTOR3( 1.3f,  1.3f, -5.0f);
	}
	r=r1*r2*r3;
	D3DXVec3TransformNormal(&min,&min,&r);
	D3DXVec3TransformNormal(&max,&max,&r);
	GetRandomVector(
		&attribute->_velocity,
		&min,
		&max);

	// normalize to make spherical
	D3DXVec3Normalize(
		&attribute->_velocity,
		&attribute->_velocity);

	attribute->_velocity *= GetRandomFloat(1.5f, 80.0f);//80.0f;

	/*attribute->_color = D3DXCOLOR(
		d3d::GetRandomFloat(0.0f, 1.0f),
		d3d::GetRandomFloat(0.0f, 1.0f),
		d3d::GetRandomFloat(0.0f, 1.0f),
		1.0f);*/
	C=GetRandomFloat(0.4f, 0.9f);
	attribute->_color = D3DXCOLOR(C,C,C,1.0f);

	attribute->_age      = 0.0f;
	attribute->_lifeTime = 1.6f; // lives for 2 seconds
}



void fog::update(float timeDelta)
{
	std::list<Attribute>::iterator i;

	for(i = _particles.begin(); i != _particles.end(); i++)
	{
		// only update living particles
		if( i->_isAlive )
		{
			
			if(i->_age>0.08f)
			{	i->_position += i->_velocity*0.008f * timeDelta;}
			else
			{i->_position += i->_velocity * timeDelta;}
			
			i->_age += timeDelta;

			if(i->_age > i->_lifeTime) // kill 
				i->_isAlive = false;
		}
	}
}

void fog::preRender()
{
	PSystem::preRender();

	//_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
   // _device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// read, but don't write particles to z-buffer
	_device->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void fog::postRender()
{
	PSystem::postRender();

	_device->SetRenderState(D3DRS_ZWRITEENABLE, true);
}
// Explosion flash System