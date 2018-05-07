/************************************************************************/
/*							
							Unit 클래스 
					캐릭터 , 몬스터 , npc, 주인공 등의 base class
*/
/************************************************************************/
#pragma  once

//////////////////////////////////////////////////////////////////////////
// 전방선언
class CGameObject;


class IBuildable
{
protected:

	
public:
	
	IBuildable();
	virtual ~IBuildable();

public:

	virtual int								Build(const WCHAR* _Path) PURE;

};


/************************************************************************/
/*			
					struct UNIT_DATA
			유닛이 가지고 있는 기본 속성들을 외부에서 입력받은 정보
*/
/************************************************************************/



/************************************************************************/
/*						   [2014.7.26 ]
						  class CEntity
*/
/************************************************************************/
class CEntity : public CGameObject,//base class
				virtual public IMessagable
{
	
protected:

	
	// transform을 위한 멤버변수
	D3DXVECTOR3						m_Vec_StartSee; // 화면에 맨처음 생성되었을때 바라보고있는 방향
	D3DXVECTOR3						m_VecRight, m_VecUp, m_VecSee;
	D3DXVECTOR3						m_Vec_DestVector; //향하려고 하는 목표벡터
	D3DXMATRIX						m_MWorld;		// 최종 월드 행렬  =  회전 *  이동 $
	D3DXVECTOR3						m_Pos;          //현재 월드 상의  위치
	D3DXVECTOR3						m_PrePos;       // 미리 충돌체크해서 못가게할떄 검사용으로 사용하는 위치

	D3DXMATRIX						m_MTrans;       //이동행렬
	D3DXMATRIX						m_MRot, m_MRotPitch,  m_MRotRoll,  m_MRotYaw;		   //회전행렬 '최종', '우향기준', '전방기준', '상향기준' 회전행렬
	 
	float							m_Angle_Pitch; // 우측벡터를 기준으로 회전한 각도
	float							m_Angle_Roll;  // 시선 벡터를 기준으로 회전한 각도
	float							m_Angle_Yaw;   //  up 벡터를 기준으로 회전한 각도
	
	bool							m_isRotating; //현재 회전하고 있나?? (키값이 눌린상태인가?)
	//외부데이타

public:

	CEntity(){};
	CEntity(LPDIRECT3DDEVICE9 _pDevice);
	virtual ~CEntity();

public: //가상함수
	
	virtual void					Draw(){};
	virtual void					Update(float _dTime){}

	virtual bool					MsgProc( SH_MSG* _pMessage ){return true;}

	HRESULT							Init(LPDIRECT3DDEVICE9 _pDevice);

public://get..set

	D3DXVECTOR3						GetPos() const { return m_Pos;}
	D3DXVECTOR3						GetSeeVec() const {return m_VecSee;}
	D3DXVECTOR3						GetUpVec() const {return m_VecUp;}
	D3DXVECTOR3						GetRightVec() const {return m_VecRight;}
	D3DXMATRIX						GetWorldTM() const {return m_MWorld;}
	float							GetAngle_Pitch(){return m_Angle_Pitch;}
	float							GetAngle_Roll(){return m_Angle_Roll;}
	float							GetAngle_Yaw(){return m_Angle_Yaw;}
	D3DXMATRIX						GetMRot(){return m_MRot;}
	const bool&						GetIsRotating() const { return m_isRotating;}

	void							SetAngle_Pitch(float _val){ m_Angle_Pitch = _val;}
	void							SetAngle_Roll(float _val){ m_Angle_Roll = _val;}
	void							SetAngle_Yaw(float _val){ m_Angle_Yaw = _val;}
	void							SetPosition(const D3DXVECTOR3& _Val){m_Pos = _Val;  } 
	
};



/****************************************************** End *****************************************************/