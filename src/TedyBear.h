


#pragma  once

//////////////////////////////////////////////////////////////////////////
// 전방선언
class ASEMesh;
class CEntity;
class cSHBillBoard;


//////////////////////////////////////////////////////////////////////////
// 비트플레그 선언
#define  BitFlag_Left   (1)  // 첫번째비트
#define  BitFlag_Right  (2)  // 두번째비트
#define  BitFlag_Up     (4)  // 세번쨰비트
#define  BitFlag_Down   (8)  // 네번째 비트
#define  BitFlag_LClick (16) // 다섯번째 비트
#define  BitFlag_1      (32) // 여섯번째 비트
#define  BitFlag_2      (64) // 일곱번째 비트
#define  BitFlag_3      (128) // 여덟번째 비트

// 비트플레그 검사 메크로
#define  BitFlag_Move	(15)
#define  BitFlag_Skill  (240) // [2014.11.27]

#define  BitFlag_isMoving(X)		 ( X & 15) // 방향키 어느 한개라도 눌렸는지 검사하기 위한 플레그	[2014.11.17 ]
#define  BitFlag_Using_AnySkill(X)	 ( X & 240)// 공격스킬 중 어느 하나라도 사용한다면  // [2014.11.17 ]
#define  BitFlag_isLBtnClick(X)			 ( X & BitFlag_LClick) // LButton 클릭했나 검사하는 메크로 [2014.11.26 ]



//////////////////////////////////////////////////////////////////////////
#define  S_POINT  8.33333f


/************************************************************************/
/*				[2014.7.29]
				struct BEAR_DATA :  Monster의 각종 외부 정보(스크립트 등)
				을 보관한다.
*/
/************************************************************************/
struct	BEAR_DATA
{
	int			Size; // 이 구조체의 사이즈
	float		MoveSpeed;// 곰의 이동속도 [2014.11.13 ]
	float		RotSpeed; // 곰돌이 회전속도 [2014.11.13 ]
	
	BEAR_DATA();
};


/************************************************************************/
/*				[2014.7.29]
				class CBear 
				CEntity 클래스를 상속받는다.
*/
/************************************************************************/
class CBear 	: public CEntity, //Base Class
				  virtual public IProgressable,
				  virtual public IThinkable,
				  virtual public IRenderable,
				  virtual public IPickable,
				  virtual public ICollidable

{
	
protected:
	friend	class CBearCreator; //곰의 엔티티팩토리에서 접근할수 있게 친구추가 [2014.11.25 ]

	//////////////////////////////////////////////////////////////////////////
	// 화면에 보여질 리소스 구성요소 // 
	CRscPack_Character				m_RscPack;// 리소스 구성요소들
	cSHBillBoard*					m_pEffMagicStick;// 마법봉에달려있는 이펙트 

	//////////////////////////////////////////////////////////////////////////
	// 스킬종류 
	CScatterSkill					m_SkillCard;
	CRockPunchSkill					m_SkillRockPunch;
	CHatSkill						m_SkillHat;  // 모자떨구기 스킬 


	//////////////////////////////////////////////////////////////////////////
	// 리소스중에 충돌에 필요한 포인터를 미리 받아놓는다. 
	CBoundSphere*					m_pBoundSphere_Stick; // 충돌구의 포인터를 초기화 할때 한번만 장착해놓는다. 				
	CBoundSphere*					m_pBoundSphere_Body; // 충돌구의 포인터를 초기화 할때 한번만 장착해놓는다. 		
	const SHR_Mesh*					m_BSMesh_Stick;// 충돌구실시간 업데이트를 위해 미리 포인터 받아놓음 
	const SHR_Mesh*					m_BoneMesh_Hat;//로켓펀치스킬 나갈때 위치 참조하기 위해 미리 포인터 받아놓기 
	D3DXVECTOR3						m_PosStick ; // 막대기 끝의 실시간 위치


	//////////////////////////////////////////////////////////////////////////
	// 외부변수
	BEAR_DATA						m_BearData;
	BEAR_DATA						m_BearDatabk;// 백업용
	static CString					m_EntityType; // 엔티티별 타입 이름

	//////////////////////////////////////////////////////////////////////////
	// 일반변수 [2014.11.13 ]
	DWORD							m_SkillbitFlag; // 눌렀을때마다 비트로 연산하여 체크	
	D3DXVECTOR3						m_Vec_Modified_See; // 충돌되었을때 사용될 변한 방향
	float							m_SkillPoint; // 누적될 스킬포인트, 한대 때릴때마다 증가한다. 

	SH_Timer						m_NormalHit_Timer;// 일반공격에 사용될 타이머  

	// 8방향벡터 미리 만들어 놓기 // [2014.11.13 ]
	D3DXVECTOR3						m_Vec_Right; // 오른쪽 //
	D3DXVECTOR3						m_Vec_Left;
	D3DXVECTOR3						m_Vec_Front;
	D3DXVECTOR3						m_Vec_Back ;
	D3DXVECTOR3						m_Vec_FR ;
	D3DXVECTOR3						m_Vec_FL ;
	D3DXVECTOR3						m_Vec_BR ;
	D3DXVECTOR3						m_Vec_BL ;

public:
	CBear();
	virtual ~CBear();

public: //가상함수

	// from IRenderable
	virtual void					Draw();
	// from IProgressable
	virtual void					Update(float _dTime);		
	// from IMessagable
	virtual bool					MsgProc(  SH_MSG* _pMessage );
	// from IPickable
	virtual IPickable*				Picking(){ return this;};
	// from IThinkable
	virtual void					AIUpdate(float _dTime){};
	// from ICollidable
	virtual HRESULT					InitCollider(); //[2014.11.11 ]

	//초기화할때 필요한 함수
	int								Init(LPDIRECT3DDEVICE9 _pDevice , D3DXVECTOR3 _SPos = D3DXVECTOR3(0,0,200),
										 BEAR_DATA* pBData= nullptr );
	int								InitGameObject();

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

public:// 일반함수
	// 테디베어 속성정보를 루아스크립트에서 읽어들인다. 
	int								Load_AttrInfo_from_Lua(const CHAR* _pPath);

	// 콜리드 테그를 인자로 충돌체크해주는 함수 
	bool							ColliChk_Using_Tag(int _tag);

public: //Get,, set 함수



	static CString					GetCreatorName(){return m_EntityType;}
	CRscPack_Character*				GetRscPack(){return &m_RscPack;}

	void							SetAngle_Pitch(float _val){ m_Angle_Pitch = _val;}
	void							SetAngle_Roll(float _val){ m_Angle_Roll = _val;}
	void							SetAngle_Yaw(float _val){ m_Angle_Yaw = _val;}


private: // 내부함수

	void							_OnDrawStart();
	void							_OnUpdateStart(float _dTime);


};


//////////////////////////////////////////////////////////////////////////
// 전역변수 선언부

extern CBear*					     G_pTedyBear;


/****************************************************** End *****************************************************/