


#pragma  once

//////////////////////////////////////////////////////////////////////////
// 전방선언
class ASEMesh;
class cMissile;
class CEntity;
//////////////////////////////////////////////////////////////////////////
enum AIRCRAFT_TEXTURE_TYPE
{	//AIRCRAFT_TEXTURE_TYPE
	ATT_UP,
	ATT_DOWN,
	ATT_RIGHT,
	ATT_LEFT,
	ATT_FROTN,
	ATT_BACK,
//	ATT_SNIPER,
	ATT_MAX
};

/************************************************************************/
/*						[2014.7.26 배성환]
				struct A_DATA :  AirCraft의 각종 외부 정보(스크립트 등)
				을 보관한다.
*/
/************************************************************************/
struct	A_DATA
{
	int			Size; // 이 구조체의 사이즈
	int			MissileCnt; //가지고 있는 미사일 개수
};


/************************************************************************/
/*					[2014.7.26 배성환]
				class AirCraft  비행기 클래스
				CEntity 클래스를 상속받는다.
*/
/************************************************************************/
class AirCraft 
	: public CEntity, //Parent Class
	  public IControlable,
	  public IRenderable,
	  public IProgressable
{
	friend  class cMissile;
protected:

	ASEMesh*					m_pAseMesh; //ase파일을 읽어들여 만든 메쉬
	

	const HELPEROBJECT			*m_pHFirePos_R, *m_pHFirePos_L; // 미사일 발사위치 오른쪽, 왼쪽을 알려줄 헬퍼오브젝트.. 빌려쓰는 거기때문에 메모리 해제할 필요 없다.
	
	//////////////////////////////////////////////////////////////////////////
	// transform을 위한 멤버변수
	//D3DXVECTOR3				m_VecRight, m_VecUp, m_VecSee; //$ 유닛클래스로 이동 [2014.7.26 배성환]
	//D3DXVECTOR3				m_Pos;         //현재 월드 상의  위치 //$ 유닛클래스로 이동 [2014.7.26 배성환]


	//////////////////////////////////////////////////////////////////////////
	// 외부변수
	A_DATA						m_AData;
	A_DATA						m_ADatabk;// 백업용

public:
	AirCraft();
	virtual ~AirCraft();

public: //가상함수
	
	virtual void					Draw();
	virtual void					Update(float _dTime);	
	virtual bool					MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual void					SetVtxShader(LPDIRECT3DVERTEXSHADER9 _pShader){m_pAseMesh->Set_VtxShader(_pShader);		}
	virtual void					SetPixShader(LPDIRECT3DPIXELSHADER9 _pShader){m_pAseMesh->Set_PixShader(_pShader);		}
	//초기화할때 필요한 함수
	HRESULT							Init(LPDIRECT3DDEVICE9 _pDevice , A_DATA* pAData, UNIT_DATA* pUData );
	int								InitGameObject();
	void							InitMaterial(D3DXCOLOR _Ambi, D3DXCOLOR _Diffuse, D3DXCOLOR _Specu, D3DXCOLOR _Emissive, float _Specu_Power);

public:

	//이동을위한 함수
	void							MoveLeft(float& _dTime);
	void							MoveRight(float& _dTime);
	void							MoveForward(float& _dTime);
	void							MoveBack(float& _dTime);
	void							MoveDown(float& _dTime);
	void							MoveUp(float& _dTime);
	//회전을 위한 함수
	void							Rot_Pitch(float _Angle);// 우향벡터를 기준으로 회전
	void							Rot_Roll(float _Angle);//전방벡터 기준으로 회전
	void							Rot_Yaw(float _Angle);//상향벡터기준으로 회전


public: //Get,, set 함수


	ASEMesh*						GetASEData() const{ return m_pAseMesh;}
	





};


//////////////////////////////////////////////////////////////////////////
// 전역변수 선언부




/****************************************************** End *****************************************************/