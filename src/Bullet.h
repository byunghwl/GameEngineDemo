/************************************************************************/
/*							[2014.7.24 배성환]                          */
//							    bullet 클래스
//						슈팅게임에 쓰이는 각종 화포류나 로켓, 미사일이 이클래스를 
//								상속받는다.
//					
/************************************************************************/

#pragma  once

//[2014.7.24 배성환]
//////////////////////////////////////////////////////////////////////////
// 전방선언
class AirCraft;
class ASEMesh;
class AirCraft;
class cBullet;
class cSmoke;



//////////////////////////////////////////////////////////////////////////



/************************************************************************/
/* 
						[2014.7.24 배성환]
						cBullet의 외부에서 들어올 각종 
						멤버변수를 모아 놓은 구조체
*/
/************************************************************************/
struct Bullet_DATA
{ 	 
	//외부에서 들어올 (스크립터 등을통해..) 데이터가 있을경우
	//이곳에 추가합니다.... 

	//이 구조체의 사이즈
	int					Size;  
	//데미지..
	int					Dmg;			
	
	float				Velo; // 이동 속도
	
	//곡선 이동을 위한 추가 데이터.
	float				Angle;			
	float				Scale; 
	//관통가능한가?
	bool				IsPiercing;
	// 총알이 생성되고 나서 충돌이 없어도 일정시간이 지나면 사라지게 하기 위한 시간 [2014.7.24 배성환]
	float				lifetime;
	Bullet_DATA();	

};


/************************************************************************/
/*						 [2014.7.24 배성환]                                    
			 class cBullet : 총알이나 미사일종류의 최상위클래스					    
*/
/************************************************************************/

class cBullet 
{
protected:
	// 멤버변수(외부에서 설정하지않는 내부 데이터들)
	LPDIRECT3DDEVICE9       m_pDevice;  // Our rendering device
	bool					m_IsFire;	// 발사 되었냐?
	bool					m_IsBoom;	// 폭파 되었냐? 미사일류...

	D3DXMATRIXA16			m_WorldTM;	// 총알의 월드 TM 
	D3DXVECTOR3				m_LookVec;	// 현재 총알이 바라보고 있는 방향		[2014.7.25 배성환]
	D3DXVECTOR3				m_RightVec, m_UpVec;	// 오른쪽, 업벡터 
	D3DXVECTOR3				m_Pos;		// 현재 총알의 월드상의 위치			[2014.7.25 배성환]
	float					m_CurLife;  // 총알의 나이 (시간이 지날수록 값이 증가한다. 업데이트함수에서 증가시킴 )
	
	//외부 설정 데이터
	Bullet_DATA				m_BData; 
	Bullet_DATA				m_BDbk;		 // 총알 리셋시 백업용 [2014.7.24 배성환]

public:
	//생성자 처음시작좌표와, 파티클 개수
	cBullet();
	cBullet(Bullet_DATA* pBData ,  LPDIRECT3DDEVICE9 _pDevice);					 
	virtual ~cBullet();


public:
	//가상함수 
	virtual int				Init( Bullet_DATA* pBD, LPDIRECT3DDEVICE9 _pDevice );	
	virtual	void			Reset();
	virtual void			Release();
	virtual int				update(float timeDelta);
	virtual int				Draw(float dTime, float alpha = 1.0f) = 0;
	virtual void			Cal_WorldTM();
	virtual void			SetVtxShader(LPDIRECT3DVERTEXSHADER9 _pShader)=0;
	virtual void			SetPixShader(LPDIRECT3DPIXELSHADER9 _pShader)=0;
public:
	//상태 체크 함수
	bool					IsFire(){ return m_IsFire; }
	
public:
	// get.. set
	void					SetFire(bool v = true) { m_IsFire = v; }



};

//////////////////////////////////////////////////////////////////////////
#include "Missile.h"

/****************************************************** End *****************************************************/