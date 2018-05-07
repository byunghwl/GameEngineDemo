/************************************************************************/
/*							[2014.7.24 배성환]                          */
//							    bullet 클래스
//						슈팅게임에 쓰이는 각종 총. 화포류나 로켓, 미사일이 이클래스를 
//								상속받는다.
//					
/************************************************************************/

#pragma  once

//[2014.7.24 배성환]
//////////////////////////////////////////////////////////////////////////
// 전방선언
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
struct Missile_DATA
{ 	 
	//외부에서 들어올 (스크립터 등을통해..) 데이터가 있을경우
	//이곳에 추가합니다.... 

	//이 구조체의 사이즈
	int		Size;
	bool	is_right ; //오른쪽 날개에 장착되어있는가?
	float	Dist_to_Smoke;// 미사일 중점에서 부터 연기 분출 지점까지의 거리
	Missile_DATA();	

};

/************************************************************************/
/*						 [2014.7.24 배성환]                                    
					 class cBullet : 최상위클래스					    
*/
/************************************************************************/

class cMissile : public cBullet 
{
	friend  class AirCraft;

protected:
	// 멤버변수(외부에서 설정하지않는 내부 데이터들)
	static int					m_MisslieCnt; //미사일 객체가 게임내에서 생성된 총 개수이기 때문에 static 변수로 선언 
	static ASEMesh				*m_pMesh;	  //메쉬 데이타  모든 미사일이 같은 메쉬 공유
	AirCraft					*m_pAirCraft; // 이 미사일이 장착된 곳의 포인터,,
											  //(이때부터는 같이 다닐필요가 없기 때문에 비행기가 터져도 미사일은 존재해야함)
	
	cSmoke						m_Smoke;	  //연기 분출 파티클 시스템 
	D3DXVECTOR3					m_BezierPoint[4];  //베지어 적용할 점 4개
	float						m_BezPercent;  // 베지어 곡선 진행률

	//외부 설정 데이터
	Missile_DATA				m_MData; 
	Missile_DATA				m_MDbk;			//총알 리셋시 백업용 [2014.7.24 배성환]

public:
	//생성자 소멸자 
	cMissile();
	cMissile( LPDIRECT3DDEVICE9 _Device, Missile_DATA* pMData = nullptr, Bullet_DATA* pBData = nullptr );					 
	virtual ~cMissile();


public:
	//가상함수 재정의
	virtual int					Init( Missile_DATA* pMD , Bullet_DATA* pBD, LPDIRECT3DDEVICE9 _Device);	
	virtual	void				Reset();
	virtual void				Release();
	virtual int					Update(float timeDelta);
	virtual int					Draw(float dTime, float alpha = 1.0f);
	virtual void			    SetVtxShader(LPDIRECT3DVERTEXSHADER9 _pShader){ m_pMesh->Set_VtxShader(_pShader);		}
	virtual void				SetPixShader(LPDIRECT3DPIXELSHADER9 _pShader){m_pMesh->Set_PixShader(_pShader);		}
public:
	//상태 체크 함수


public:
	// get.. set
	ASEMesh*					GetASEData() const{ return m_pMesh;} //외부에서 포인터를이용해 내부를 변경해야하므로
																	 //const로 반환하면 안됨


};




/****************************************************** End *****************************************************/