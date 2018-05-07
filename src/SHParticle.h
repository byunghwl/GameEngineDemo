/************************************************************************/
/*							[2014.7.23 ]                          */
//							  ��ƼŬ �ý���
//						���� ��å ���� ������ ����
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
		��ƼŬ ������ �Ӽ� ����ü
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
	D3DXVECTOR3		_acceleration; // ���ӵ�
	float			_lifeTime;     // how long the particle lives for before dying  
	float			_age;          // current age of the particle  
	D3DXCOLOR		_color;        // current color of the particle   
	D3DXCOLOR		_colorFade;    // how the color fades with respect to time
	bool			_isAlive;    
};



/************************************************************************/
/*						 ��ƼŬ �ý���  class PSyste
								�ֻ��� Ŭ���� 
*/
//[2014.7.23 ]
/************************************************************************/
class PSystem
{
protected:
	//�������
	IDirect3DDevice9*					m_pDevice;	//����̽�
	IDirect3DTexture9*					m_pTex;		//�ؽ���
	IDirect3DVertexBuffer9*				m_pVtxBuff;  //���ؽ� ����
	float								m_emitRate;   // rate new particles are added to system
	float								m_size;       // size of particles
	D3DXVECTOR3							m_origin;    // ��ƼŬ�� ����� ������ġ

	D3DXMATRIX							m_MatIdentity;	
	DWORD								m_VtxBuf_Size;      // size of vb
	DWORD								m_VtxBuf_Offset;    // offset in vb to lock   
	DWORD								m_VtxBuf_BatchSize; // number of vertices to lock starting at _vbOffset
	std::list<Attribute>				stlList_Attributes; // stl����Ʈ ��ƼŬ�� �Ӽ�����  [2014.7.23 ]
public:
	//������ , �Ҹ���
	PSystem();
	virtual	 ~PSystem();
		
public: 
	//���� �Լ�
	// sometimes we don't want to free the memory of a dead particle,
	// but rather respawn it instead.
	virtual bool			 init(IDirect3DDevice9* _device,  LPCWSTR _texFileName);
	// ��� ��ƼŬ ���ڵ��� �Ӽ��� ó������ �ǵ�����.
	virtual void			 Reset_All();
	// ��ƼŬ ������ �Ӽ��� �����Ѵ�.
	virtual	void			 Reset_Attribute(Attribute* attribute,int i=0) = 0;
	// ��ƼŬ �Ӽ��� �������� stlList_Attributes �� �߰��Ѵ�.
	virtual void			 addParticle(int j=0);

	virtual void			 update(float timeDelta) = 0;

	virtual void			 preRender();
	virtual void			 render();
	virtual void			 postRender();
	//
	virtual void			 removeDeadParticles();

public:
	// ���� üũ �Լ�
	bool					isEmpty();
	bool					isDead();

public:
	//get ..set
	void					SetOrigne(D3DXVECTOR3 _Val){m_origin = _Val;}
};


#include "SHSmoke.h"

/****************************************************** End *****************************************************/