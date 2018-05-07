//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: pSystem.h
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Represents a general particle system.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////
#pragma  once

#include <list>

	const float INFINITY = FLT_MAX;
	const float EPSILON  = 0.001f;
	const D3DXCOLOR      WHITE( D3DCOLOR_XRGB(255, 255, 255) );
	const D3DXCOLOR      BLACK( D3DCOLOR_XRGB(  0,   0,   0) );
	const D3DXCOLOR        RED( D3DCOLOR_XRGB(255,   0,   0) );
	const D3DXCOLOR      GREEN( D3DCOLOR_XRGB(  0, 255,   0) );
	const D3DXCOLOR       BLUE( D3DCOLOR_XRGB(  0,   0, 255) );
	const D3DXCOLOR     YELLOW( D3DCOLOR_XRGB(255, 255,   0) );
	const D3DXCOLOR       CYAN( D3DCOLOR_XRGB(  0, 255, 255) );
	const D3DXCOLOR    MAGENTA( D3DCOLOR_XRGB(255,   0, 255) );

	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
	{
		D3DMATERIAL9 mtrl;
		mtrl.Ambient  = a;
		mtrl.Diffuse  = d;
		mtrl.Specular = s;
		mtrl.Emissive = e;
		mtrl.Power    = p;
		return mtrl;
	}
// Colors

	const D3DMATERIAL9 WHITE_MTRL  = InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);
	const D3DMATERIAL9 RED_MTRL    = InitMtrl(RED, RED, RED, BLACK, 2.0f);
	const D3DMATERIAL9 GREEN_MTRL  = InitMtrl(GREEN, GREEN, GREEN, BLACK, 2.0f);
	const D3DMATERIAL9 BLUE_MTRL   = InitMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f);
	const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 2.0f);

	// Desc: Return random float in [lowBound, highBound] interval.
	float GetRandomFloat(float lowBound, float highBound);
	

	// Desc: Returns a random vector in the bounds specified by min and max.
	void GetRandomVector(
		D3DXVECTOR3* out,
		D3DXVECTOR3* min,
		D3DXVECTOR3* max);

	//
	// Conversion
	//
	DWORD FtoDw(float f);

	struct BoundingBox
	{
		BoundingBox();

		bool isPointInside(D3DXVECTOR3& p);

		D3DXVECTOR3 _min;
		D3DXVECTOR3 _max;
	};

	struct Particle
	{
		D3DXVECTOR3 _position;
		D3DCOLOR    _color;
		static const DWORD FVF;
	};
	
	struct Attribute
	{
		Attribute()
		{
			_lifeTime = 0.0f;
			_age      = 0.0f;
			_isAlive  = true;
		}

		D3DXVECTOR3 _position;     
		D3DXVECTOR3 _velocity;     
		D3DXVECTOR3 _acceleration; 
		float       _lifeTime;     // how long the particle lives for before dying  
		float       _age;          // current age of the particle  
		D3DXCOLOR   _color;        // current color of the particle   
		D3DXCOLOR   _colorFade;    // how the color fades with respect to time
		bool        _isAlive;    
	};


	class PSystem
	{
	public:
		PSystem();
		virtual ~PSystem();

		virtual bool init(IDirect3DDevice9* device, LPCWSTR texFileName);
		virtual void reset();
		
		// sometimes we don't want to free the memory of a dead particle,
		// but rather respawn it instead.
		virtual void resetParticle(Attribute* attribute,int i=0) = 0;
		virtual void addParticle(int j=0);

		virtual void update(float timeDelta) = 0;

		virtual void preRender();
		virtual void render();
		virtual void postRender();
		void Lost();
		bool isEmpty();
		bool isDead();
		D3DXVECTOR3             _origin;
	protected:
		virtual void removeDeadParticles();

	protected:
		IDirect3DDevice9*       _device;
	
		BoundingBox        _boundingBox;
		float                   _emitRate;   // rate new particles are added to system
		float                   _size;       // size of particles
		IDirect3DTexture9*      _tex;
		IDirect3DVertexBuffer9* _vb;
		std::list<Attribute>    _particles;
		int						_numparticle;
		int                     _maxParticles; // max allowed particles system can have

		// Following three data elements used for rendering the p-system efficiently


		DWORD _vbSize;      // size of vb
		DWORD _vbOffset;    // offset in vb to lock   
		DWORD _vbBatchSize; // number of vertices to lock starting at _vbOffset
	};


	class Snow : public PSystem
	{
	public:
		Snow(BoundingBox* boundingBox, int numParticles);
		void resetParticle(Attribute* attribute,int i=0);
		void update(float timeDelta);
	};

	class Explosion : public PSystem
	{
	public:
		Explosion(D3DXVECTOR3* origin, int numParticles);
		void resetParticle(Attribute* attribute,int i=0);
		void update(float timeDelta);
		void preRender();
		void postRender();
	};

	class fog : public PSystem
	{
	public:
		fog(D3DXVECTOR3* origin, int numParticles);
		void resetParticle(Attribute* attribute,int i=0);
		void update(float timeDelta);
		void preRender();
		void postRender();

		
		D3DXMATRIX r,r1,r2,r3;//연기의 방향을 현재 자주포의 cross 처럼 정해주기 위한 회전행렬
	};

	class Flash : public PSystem
	{
	public:
		Flash(D3DXVECTOR3* origin, int numParticles);
		void resetParticle(Attribute* attribute,int i=0);
		void update(float timeDelta);
		void preRender();
		void postRender();
	};


/****************************************************** End *****************************************************/