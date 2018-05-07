/************************************************************************/
/*							[2014.7.24 �輺ȯ]                          */
//							    bullet Ŭ����
//						���ð��ӿ� ���̴� ���� ȭ������ ����, �̻����� ��Ŭ������ 
//								��ӹ޴´�.
//					
/************************************************************************/

#pragma  once

//[2014.7.24 �輺ȯ]
//////////////////////////////////////////////////////////////////////////
// ���漱��
class AirCraft;
class ASEMesh;
class AirCraft;
class cBullet;
class cSmoke;



//////////////////////////////////////////////////////////////////////////



/************************************************************************/
/* 
						[2014.7.24 �輺ȯ]
						cBullet�� �ܺο��� ���� ���� 
						��������� ��� ���� ����ü
*/
/************************************************************************/
struct Bullet_DATA
{ 	 
	//�ܺο��� ���� (��ũ���� ��������..) �����Ͱ� �������
	//�̰��� �߰��մϴ�.... 

	//�� ����ü�� ������
	int					Size;  
	//������..
	int					Dmg;			
	
	float				Velo; // �̵� �ӵ�
	
	//� �̵��� ���� �߰� ������.
	float				Angle;			
	float				Scale; 
	//���밡���Ѱ�?
	bool				IsPiercing;
	// �Ѿ��� �����ǰ� ���� �浹�� ��� �����ð��� ������ ������� �ϱ� ���� �ð� [2014.7.24 �輺ȯ]
	float				lifetime;
	Bullet_DATA();	

};


/************************************************************************/
/*						 [2014.7.24 �輺ȯ]                                    
			 class cBullet : �Ѿ��̳� �̻��������� �ֻ���Ŭ����					    
*/
/************************************************************************/

class cBullet 
{
protected:
	// �������(�ܺο��� ���������ʴ� ���� �����͵�)
	LPDIRECT3DDEVICE9       m_pDevice;  // Our rendering device
	bool					m_IsFire;	// �߻� �Ǿ���?
	bool					m_IsBoom;	// ���� �Ǿ���? �̻��Ϸ�...

	D3DXMATRIXA16			m_WorldTM;	// �Ѿ��� ���� TM 
	D3DXVECTOR3				m_LookVec;	// ���� �Ѿ��� �ٶ󺸰� �ִ� ����		[2014.7.25 �輺ȯ]
	D3DXVECTOR3				m_RightVec, m_UpVec;	// ������, ������ 
	D3DXVECTOR3				m_Pos;		// ���� �Ѿ��� ������� ��ġ			[2014.7.25 �輺ȯ]
	float					m_CurLife;  // �Ѿ��� ���� (�ð��� �������� ���� �����Ѵ�. ������Ʈ�Լ����� ������Ŵ )
	
	//�ܺ� ���� ������
	Bullet_DATA				m_BData; 
	Bullet_DATA				m_BDbk;		 // �Ѿ� ���½� ����� [2014.7.24 �輺ȯ]

public:
	//������ ó��������ǥ��, ��ƼŬ ����
	cBullet();
	cBullet(Bullet_DATA* pBData ,  LPDIRECT3DDEVICE9 _pDevice);					 
	virtual ~cBullet();


public:
	//�����Լ� 
	virtual int				Init( Bullet_DATA* pBD, LPDIRECT3DDEVICE9 _pDevice );	
	virtual	void			Reset();
	virtual void			Release();
	virtual int				update(float timeDelta);
	virtual int				Draw(float dTime, float alpha = 1.0f) = 0;
	virtual void			Cal_WorldTM();
	virtual void			SetVtxShader(LPDIRECT3DVERTEXSHADER9 _pShader)=0;
	virtual void			SetPixShader(LPDIRECT3DPIXELSHADER9 _pShader)=0;
public:
	//���� üũ �Լ�
	bool					IsFire(){ return m_IsFire; }
	
public:
	// get.. set
	void					SetFire(bool v = true) { m_IsFire = v; }



};

//////////////////////////////////////////////////////////////////////////
#include "Missile.h"

/****************************************************** End *****************************************************/