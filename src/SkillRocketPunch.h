
/***********************************************************************
                  SkillCard.h
			  // [2014.11.14 배성환]
************************************************************************/


#pragma  once

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

/************************************************************************/
/*						// [2014.11.29 배성환]
				struct ROCKPUNCH_DATA :  Monster의 각종 외부 정보(스크립트 등)
				을 보관한다.
*/
/************************************************************************/
struct	ROCKPUNCH_DATA
{
	int			    Size; // 이 구조체의 사이즈
	float		    MoveSpeed;// 카드의 이동속도 // [2014.11.27 배성환]
	float			Range; // 카드 사거리 //[2014.11.27 배성환]
	float			CoolTime; // 스킬 쿨타임 //[2014.11.28 배성환]
	ROCKPUNCH_DATA(){MoveSpeed = 500.0f; }
};


/************************************************************************/
/*					// [2014.11.27 배성환]
					class CRockPunchSkill 
				CEntity 클래스를 상속받는다.
*/
/************************************************************************/
class CRockPunchSkill 	: public CEntity, //Base Class
					  virtual public IProgressable,
				      virtual public IRenderable,
				      virtual public ICollidable

{
	
public:
	friend class CGhost_Creator;

	CRscPack_Character				m_RscPack;// 리소스 구성요소들
	D3DXHANDLE						m_hTech;// 셰이더, 어떤 테크닉을 쓸거냐? [2014.10.30 배성환]
	
	//////////////////////////////////////////////////////////////////////////
	// transform을 위한 멤버변수
	D3DXVECTOR3						m_StartPos; // 스킬 시작 위치/  [2014.11.27 배성환]
	D3DXMATRIX						m_CharacterWorldTM ; //캐릭터월드TM  [2014.11.27 배성환]


	//////////////////////////////////////////////////////////////////////////
	// 외부변수
	ROCKPUNCH_DATA					m_RockPunchData;
	ROCKPUNCH_DATA					m_RockPunchDataBk;// 백업용
	static CString					m_EntityType; // 엔티티별 타입 이름

	//////////////////////////////////////////////////////////////////////////
	// 실시간으로 계산되는 변수
	bool							m_b_is_TimerOn; // 스킬 딜레이 타이머 가 켜졌나? [2014.11.27 배성환]	
	bool							m_bAlive;	// 카드스킬이 살아있나 // [2014.11.27 배성환]
	float							m_CoolTime; // 쿨타임을 위해 누적시킬변수 //[2014.11.28 배성환]
	float							m_DelayTime; // 스킬 지연 발동할때 계산될 지연 시간 // [2014.11.27 배성환]
	CBoundSphere*					m_pBoundSphere_Body; // 카드의 충돌구
	float							m_AccuTime; // 누적될 시간  [2014.11.28 배성환]
	const SHR_Mesh*					m_BoneMesh_Hat; //[2014.11.29 배성환]


public:

	CRockPunchSkill();
	virtual ~CRockPunchSkill();

public: //가상 재정의 함수

	// from IRenderable
	virtual void					Draw();
	// from IProgressable
	virtual void					Update(float _dTime);		
	// from IMessagable
	virtual bool					MsgProc(  SH_MSG* _pMessage );

	// from ICollidable
	virtual HRESULT					InitCollider(); //[2014.11.11 배성환]

	//초기화할때 필요한 함수
	int								Init(LPDIRECT3DDEVICE9 _pDevice , D3DXVECTOR3 _SPos = D3DXVECTOR3(0,0,200),
										 ROCKPUNCH_DATA* pBData= nullptr );

public:// 일반함수
	// 카드스킬 발동!! int DelayFrame 스킬발동 지연 시간(몇프레임 지연시킬 것인가) // [2014.11.27 배성환]
	void							SetDelayTime_for_Begin(D3DXVECTOR3 _StartPos,D3DXVECTOR3 _SeeVec,
														   D3DXMATRIX _CharacterWorldTM ,  int DelayFrame,
														   const SHR_Mesh* _pRefMesh = nullptr); // [2014.11.27 배성환]

	// 카드속성정보를 루아스크립트에서 읽어들인다. 
	int								Load_AttrInfo_from_Lua(const CHAR* _pPath);

public: //Get,, set 함수

	static CString					GetCreatorName(){return m_EntityType;}
	CRscPack_Character*				GetRscPack(){return &m_RscPack;}

	void							SetAngle_Pitch(float _val){ m_Angle_Pitch = _val;}
	void							SetAngle_Roll(float _val){ m_Angle_Roll = _val;}
	void							SetAngle_Yaw(float _val){ m_Angle_Yaw = _val;}
	void							SetDead(){m_bAlive = false; } //[2014.11.26 배성환]
	bool							isReady();//[2014.11.28 배성환]

private: // 내부함수

	void							_OnDrawStart();
	void							_OnUpdateStart(float _dTime);

};


///////////////////////////////////////////////////////////////////////////
// 전역변수 선언
extern CRockPunchSkill*				G_pRockPunch; // [2014.11.27 배성환]

/************************************************************************ end of this file *******************************************************************************/