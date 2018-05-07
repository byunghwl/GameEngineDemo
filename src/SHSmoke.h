/************************************************************************/
/*							[2014.7.23 ]                          */
//							  연기 효과 
//					
/************************************************************************/

#pragma  once
class PSystem;



/************************************************************************/
/*						 [2014.7.23 ]                                    
						    class cSmoke
					     PSystem 클래스를 상속
*/
/************************************************************************/

class cSmoke : public PSystem
{
protected:
	//멤버변수
	D3DXVECTOR3				m_EmissDir;  //연기 방출 방향
	float					m_SprMaxSize; // sprite 맥스 사이즈
	float					m_EmitPower;//분출 파워
public:
	cSmoke(){};
	//생성자 처음시작좌표와, 파티클 개수
	cSmoke(D3DXVECTOR3* origin, int numParticles, float Maxsize);
	//소멸자
	virtual ~cSmoke();
	// 초기화 함수  _EmitPower : 연기 분출 강도 , _texFileName: 사용할 텍스쳐 이름
	int						Init(  LPDIRECT3DDEVICE9 _pDevice , LPCWSTR _texFileName, float _EmitPower = 10.0f,
								   int numParticles = 256, float sprMaxsize = 100.0f);	
	
public:
	//가상함수 재정의
	virtual void			Reset_Attribute(Attribute* attribute,int i=0);
	virtual void			update(float timeDelta);
	virtual void			preRender();
	virtual void			postRender();

public:
	// get.. set
	void					Set_EmissDir(D3DXVECTOR3 _Val){ m_EmissDir = _Val;}


};