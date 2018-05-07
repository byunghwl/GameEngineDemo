/***********************************************************************
                  Ghost.h
			  // [2014.11.14]
************************************************************************/


#pragma  once

//////////////////////////////////////////////////////////////////////////
#define  GHOSTNUM_MAX  200 //[2014.11.16]
#define	 PASSPOINTNUM_MAX  30 //[2014.11.18]
//////////////////////////////////////////////////////////////////////////
// 전방선언 


/************************************************************************/
/*						[2014.11.17 ]
				struct GHOST_DATA :  Monster의 각종 외부 정보(스크립트 등)
				을 보관한다.
*/
/************************************************************************/
struct	GHOST_DATA
{
	int			    Size; // 이 구조체의 사이즈
	float		    MoveSpeed;// 유령의 이동속도 [2014.11.15 ]
	float		    RotSpeed; // 유령의 회전속도 [2014.11.15 ]
	int			    HP;     //유령의 HP [2014.11.15 ]	
	int			    GhostType;  // 유령타입 (1,2,3단계)
	int				Attack; // 유령 공격력 //[2014.12.5 ]
	D3DXVECTOR3     StartingPoint; // 유령 생성지점 // [2014.11.15 ]
	// 통과지점 배열 [2014.11.18 ]
	D3DXVECTOR3		Vec_PassPointArr[PASSPOINTNUM_MAX];
	int				PassPointCnt; // 통과지점 의 총 개수 [2014.11.18 ]


	GHOST_DATA();
};



/************************************************************************
                    Ghost DataBase    [2014.11.17]                                           
*************************************************************************/
class  CGhost_DataBase
{
	friend class CGhost;

private:
	// 엔티티 팩토리 로 넘겨줄 인자값  스폰정보 포함 [2014.11.16 ]
	GhostFactory_Arg		m_FactoryArg_Point1; // [2014.11.16 ]
	GhostFactory_Arg		m_FactoryArg_Point2;
	GhostFactory_Arg		m_FactoryArg_Point3;
	GhostFactory_Arg		m_FactoryArg_Point4;
	// ghost 속성 레벨별//[2014.11.16 ]
	GHOST_DATA				m_GhostAttr_LV[3]; 	

	// 통과지점 배열 [2014.11.18 ]
	D3DXVECTOR3				m_Vec_PassPointArr1[PASSPOINTNUM_MAX];
	int						m_PassPointArr1_Size;   // 마지막 사이즈 
	D3DXVECTOR3				m_Vec_PassPointArr2[PASSPOINTNUM_MAX];
	int						m_PassPointArr2_Size;
	D3DXVECTOR3				m_Vec_PassPointArr3[PASSPOINTNUM_MAX];
	int						m_PassPointArr3_Size;
	D3DXVECTOR3				m_Vec_PassPointArr4[PASSPOINTNUM_MAX];
	int						m_PassPointArr4_Size;

	// 애니메이션 클립정보
	CAnimator				m_Animator;		

public:// 생성자  소멸자

	CGhost_DataBase(){};
	~CGhost_DataBase(){};

public:
		
	int						Load_From_Lua(const CHAR* pPath_GhostAttr, const CHAR* pPath_GhostClipInfo,
										  const CHAR* pPath_Ghost_SpawnPoint); //[2014.11.16 ]

};

// 고스트 데이타 베이스 전역변수  //[2014.11.16 ]
extern CGhost_DataBase		G_GhostDataBase;




//////////////////////////////////////////////////////////////////////////
// enum 
enum eGhost_Action_State
{
	eGAS_MOVING, // 이동상태
	eGAS_ATTACK,// 공격상태
	eGAS_HITED, // 피격상태
	eGAS_DIE    // 죽는상태 

};




/************************************************************************/
/*					[2014.7.29 ]
					class CGhost 
				CEntity 클래스를 상속받는다.
*/
/************************************************************************/
class CGhost 	: public CEntity, //Base Class
				  virtual public IProgressable,
				  virtual public IThinkable,
				  virtual public IRenderable,
				  virtual public ICollidable

{
	
public:
	friend class CGhost_Creator;

	CRscPack_Character				m_RscPack;//고스트1 리소스 구성요소들
	CRscPack_Character				m_RscPack2;//고스트2 리소스 구성요소들  
	CRscPack_Character				m_RscPack3;//고스트3 리소스 구성요소들  
	D3DXHANDLE						m_hTech;// 셰이더, 어떤 테크닉을 쓸거냐? 
	
	cSHBillBoard*					m_pEffDeath;// 죽을때 나오는 이펙트
	cSHBillBoard*					m_pEffGoal;// 아이한테 갔을때 나오는 이펙트 
	//////////////////////////////////////////////////////////////////////////
	// transform을 위한 멤버변수


	//////////////////////////////////////////////////////////////////////////
	// 외부변수
	GHOST_DATA						m_GhostData;
	GHOST_DATA						m_GhostDatabk;// 백업용
	static CString					m_EntityType; // 엔티티별 타입 이름

	//////////////////////////////////////////////////////////////////////////
	// 실시간으로 계산되는 변수
	static int						m_CreatedGhostCnt; // 생성된 고스트 총 개수 [2014.11.17 ]
	bool							m_bAlive , m_bVisible;	// 유령이 살아있나 [2014.11.17 ]
	int								m_CurPassPoint; // 현재 진행하고 있는 패스 포인트  [2014.11.17 ]

	CBoundSphere*					m_pBoundSphere_Body; // 몸의 충돌구
	const SHR_Mesh*					m_pBSMesh_Ghost[3];  // 바운드스피어 메쉬 의 포인터를 미리 저장해놓기 실시간으로 계속 찾아야하기때문에 [2014.11.26 ]
	
	float							m_DamageTimer;// 한대 맞았을때 일정시간동안 데미지를 안입게 하기 위한 타이머//[2014.11.30 ]
	int								m_eActState; // 현재 행동 상태 enum 

public:
	CGhost();
	virtual ~CGhost();

public: //가상함수

	// from IRenderable
	virtual void					Draw();
	// from IProgressable
	virtual void					Update(float _dTime);		
	// from IMessagable
	virtual bool					MsgProc(  SH_MSG* _pMessage );
	// from IPickable
	//virtual IPickable*				Picking(){ return this;};
	// from IThinkable
	virtual void					AIUpdate(float _dTime){};
	// from ICollidable
	virtual HRESULT					InitCollider(); //[2014.11.11 ]

	//초기화할때 필요한 함수
	int								Init(LPDIRECT3DDEVICE9 _pDevice , D3DXVECTOR3 _SPos = D3DXVECTOR3(0,0,200),
										 GHOST_DATA* pBData= nullptr );

	// 데이터베이스에있는 스폰데이터를 사용해 초기화 하는 함수 [2014.11.17 ]
	// int TimeSection : 각스폰지역마다 시간으로 나뉘어진 구간의 인덱스 [2014.11.18 ]
	int								InitUsingSpawnData(const GhostFactory_Arg& _SpwnInfo, int TimeSection , int PositionNum , bool Alive);

	//씬의 업데이트 함수에서 고스트 
	static int						InitGhost_in_RealTime(const float& GameTimer , float _dTime);
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
	// 유령 속성정보를 루아스크립트에서 읽어들인다. //[2014.11.15 ]
	int								Load_AttrInfo_from_Lua(const CHAR* _pPath);
	// 한대 맞았을때 HP를 달게하는 함수
	bool							DamagedFromHit(int _Damage); //[2014.11.30 ]
public: //Get,, set 함수

	static CString					GetCreatorName(){return m_EntityType;}
	CRscPack_Character*				GetRscPack(){return &m_RscPack;}

	void							SetAngle_Pitch(float _val){ m_Angle_Pitch = _val;}
	void							SetAngle_Roll(float _val){ m_Angle_Roll = _val;}
	void							SetAngle_Yaw(float _val){ m_Angle_Yaw = _val;}
	void							SetDead(){m_bAlive = false; } //[2014.11.26 ]


private: // 내부함수

	void							_OnDrawStart();
	void							_OnUpdateStart(float _dTime);


};


//////////////////////////////////////////////////////////////////////////
// 전역변수 선언
extern CGhost*				G_pGhost_Arr[GHOSTNUM_MAX]; //[2014.11.16 ]
extern CGhost*				G_pGhost_Origne; //복사될 원본이되는  고스트 [2014.12.7 ]	


/*************************************************** end of this file **************************************************************/