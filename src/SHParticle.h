/************************************************************************/
/*							[2014.7.23 ]                          */
//							  파티클 시스템
//						본격 용책 예제 가져다 쓰기
/************************************************************************/


#pragma once


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
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/************************************************************************/
/*                                                
		파티클 입자의 속성 구조체
*/
//[2014.7.23 ]
/************************************************************************/

struct Attribute
{
	Attribute()
	{
		_lifeTime = 0.0f;
		_age      = 0.0f;
		_isAlive  = true;
	}

	D3DXVECTOR3		_position;     
	D3DXVECTOR3		_velocity;     
	D3DXVECTOR3		_acceleration; // 가속도
	float			_lifeTime;     // how long the particle lives for before dying  
	float			_age;          // current age of the particle  
	D3DXCOLOR		_color;        // current color of the particle   
	D3DXCOLOR		_colorFade;    // how the color fades with respect to time
	bool			_isAlive;    
};



/************************************************************************/
/*						 파티클 시스템  class PSyste
								최상위 클래스 
*/
//[2014.7.23 ]
/************************************************************************/
class PSystem
{
protected:
	//멤버변수
	IDirect3DDevice9*					m_pDevice;	//디바이스
	IDirect3DTexture9*					m_pTex;		//텍스쳐
	IDirect3DVertexBuffer9*				m_pVtxBuff;  //버텍스 버퍼
	float								m_emitRate;   // rate new particles are added to system
	float								m_size;       // size of particles
	D3DXVECTOR3							m_origin;    // 파티클에 적용될 시작위치

	D3DXMATRIX							m_MatIdentity;	
	DWORD								m_VtxBuf_Size;      // size of vb
	DWORD								m_VtxBuf_Offset;    // offset in vb to lock   
	DWORD								m_VtxBuf_BatchSize; // number of vertices to lock starting at _vbOffset
	std::list<Attribute>				stlList_Attributes; // stl리스트 파티클의 속성담기용  [2014.7.23 ]
public:
	//생성자 , 소멸자
	PSystem();
	virtual	 ~PSystem();
		
public: 
	//가상 함수
	// sometimes we don't want to free the memory of a dead particle,
	// but rather respawn it instead.
	virtual bool			 init(IDirect3DDevice9* _device,  LPCWSTR _texFileName);
	// 모든 파티클 입자들의 속성을 처음으로 되돌린다.
	virtual void			 Reset_All();
	// 파티클 입자의 속성을 리셋한다.
	virtual	void			 Reset_Attribute(Attribute* attribute,int i=0) = 0;
	// 파티클 속성을 리셋한후 stlList_Attributes 에 추가한다.
	virtual void			 addParticle(int j=0);

	virtual void			 update(float timeDelta) = 0;

	virtual void			 preRender();
	virtual void			 render();
	virtual void			 postRender();
	//
	virtual void			 removeDeadParticles();

public:
	// 상태 체크 함수
	bool					isEmpty();
	bool					isDead();

public:
	//get ..set
	void					SetOrigne(D3DXVECTOR3 _Val){m_origin = _Val;}
};


#include "SHSmoke.h"

/****************************************************** End *****************************************************/