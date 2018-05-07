
/***********************************************************************
                  SkillCard.h
			  // [2014.11.14 ]
************************************************************************/


#pragma  once

//////////////////////////////////////////////////////////////////////////
#define  MAXCARD_NUM  5

//////////////////////////////////////////////////////////////////////////

/************************************************************************/
/*						// [2014.11.27 ]
				struct SCATTER_DATA :  Monster의 각종 외부 정보(스크립트 등)
				을 보관한다.
*/
/************************************************************************/
struct	SCATTER_DATA
{
	int			    Size; // 이 구조체의 사이즈
	float		    MoveSpeed;// 카드의 이동속도 // [2014.11.27 ]
	float			Range; // 카드 사거리 //[2014.11.27 ]
	float			CoolTime; // 스킬 쿨타임 //[2014.11.28 ]
	float			EachAngle; // 발사될 카드들 사이의 흩뿌려지는 각도 //[2014.11.30 ]
	SCATTER_DATA(){MoveSpeed = 500.0f; }
};


/************************************************************************/
/*					// [2014.11.27 ]
					class CScatterSkill 
				CEntity 클래스를 상속받는다.
*/
/************************************************************************/
class CScatterSkill 	: public CEntity //Base Class
{
protected:


	
	D3DXHANDLE						m_hTech;// 셰이더, 어떤 테크닉을 쓸거냐? [2014.10.30 ]
	
	//////////////////////////////////////////////////////////////////////////
	// transform을 위한 멤버변수
	D3DXVECTOR3						m_StartPos; // 스킬 시작 위치/  [2014.11.27 ]
	D3DXMATRIX						m_CharacterWorldTM ; //캐릭터월드TM  [2014.11.27 ]

	CCardSkill						m_Card[MAXCARD_NUM];// 다섯개까지 발사가능하게 하기위해..  //[2014.12.1 ]
	//////////////////////////////////////////////////////////////////////////
	// 외부변수
	SCATTER_DATA					m_ScatterData;
	SCATTER_DATA					m_ScatterDataBk;// 백업용
	static CString					m_EntityType; // 엔티티별 타입 이름

	//////////////////////////////////////////////////////////////////////////
	// 실시간으로 계산되는 변수
	bool							m_b_is_TimerOn; // 스킬 딜레이 타이머 가 켜졌나? [2014.11.27 ]	
	bool							m_bAlive;	// 카드스킬이 살아있나 // [2014.11.27 ]
	float							m_CoolTime; // 쿨타임을 위해 누적시킬변수 //[2014.11.28 ]
	float							m_DelayTime; // 스킬 지연 발동할때 계산될 지연 시간 // [2014.11.27 ]
	
	float							m_AccuTime; // 누적될 시간  [2014.11.28 ]


public:

	CScatterSkill();
	virtual ~CScatterSkill();

public: //가상 재정의 함수

	// from IRenderable
	virtual void					Draw();
	// from IProgressable
	virtual void					Update(float _dTime);		
	// from IMessagable
	virtual bool					MsgProc(  SH_MSG* _pMessage );

	// from ICollidable
	virtual HRESULT					InitCollider(); //[2014.11.11 ]

	//초기화할때 필요한 함수
	int								Init(LPDIRECT3DDEVICE9 _pDevice , D3DXVECTOR3 _SPos = D3DXVECTOR3(0,0,0),
										 SCATTER_DATA* pScatData= nullptr );

public:// 일반함수
	// 카드스킬 발동!! int DelayFrame 스킬발동 지연 시간(몇프레임 지연시킬 것인가) // [2014.11.27 ]
	// _pRefMesh = nullptr : 지연시간 이 다되고나서 참조할 레퍼런스 포인트의 위치를 위해 .. [2014.11.29 ]
	void							SetDelayTime_for_Begin(D3DXVECTOR3 _StartPos,D3DXVECTOR3 _SeeVec, D3DXMATRIX _CharacterWorldTM ,
														   int DelayFrame, const SHR_Mesh* _pRefMesh = nullptr); // [2014.11.27 ]

	// 카드속성정보를 루아스크립트에서 읽어들인다. 
	int								Load_AttrInfo_from_Lua(const CHAR* _pPath);

public: //Get,, set 함수

	static CString					GetCreatorName(){return m_EntityType;}


	void							SetAngle_Pitch(float _val){ m_Angle_Pitch = _val;}
	void							SetAngle_Roll(float _val){ m_Angle_Roll = _val;}
	void							SetAngle_Yaw(float _val){ m_Angle_Yaw = _val;}
	void							SetDead(){m_bAlive = false; } //[2014.11.26 ]
	bool							isReady();//[2014.11.28 ]

private: // 내부함수

	void							_OnDrawStart();
	void							_OnUpdateStart(float _dTime);

};


///////////////////////////////////////////////////////////////////////////
// 전역변수 선언
extern CScatterSkill*				G_pScatter; // [2014.11.30 ]

/************************************************************************ end of this file *******************************************************************************/