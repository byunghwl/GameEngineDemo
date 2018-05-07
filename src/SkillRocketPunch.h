
/***********************************************************************
                  SkillCard.h
			  // [2014.11.14 �輺ȯ]
************************************************************************/


#pragma  once

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

/************************************************************************/
/*						// [2014.11.29 �輺ȯ]
				struct ROCKPUNCH_DATA :  Monster�� ���� �ܺ� ����(��ũ��Ʈ ��)
				�� �����Ѵ�.
*/
/************************************************************************/
struct	ROCKPUNCH_DATA
{
	int			    Size; // �� ����ü�� ������
	float		    MoveSpeed;// ī���� �̵��ӵ� // [2014.11.27 �輺ȯ]
	float			Range; // ī�� ��Ÿ� //[2014.11.27 �輺ȯ]
	float			CoolTime; // ��ų ��Ÿ�� //[2014.11.28 �輺ȯ]
	ROCKPUNCH_DATA(){MoveSpeed = 500.0f; }
};


/************************************************************************/
/*					// [2014.11.27 �輺ȯ]
					class CRockPunchSkill 
				CEntity Ŭ������ ��ӹ޴´�.
*/
/************************************************************************/
class CRockPunchSkill 	: public CEntity, //Base Class
					  virtual public IProgressable,
				      virtual public IRenderable,
				      virtual public ICollidable

{
	
public:
	friend class CGhost_Creator;

	CRscPack_Character				m_RscPack;// ���ҽ� ������ҵ�
	D3DXHANDLE						m_hTech;// ���̴�, � ��ũ���� ���ų�? [2014.10.30 �輺ȯ]
	
	//////////////////////////////////////////////////////////////////////////
	// transform�� ���� �������
	D3DXVECTOR3						m_StartPos; // ��ų ���� ��ġ/  [2014.11.27 �輺ȯ]
	D3DXMATRIX						m_CharacterWorldTM ; //ĳ���Ϳ���TM  [2014.11.27 �輺ȯ]


	//////////////////////////////////////////////////////////////////////////
	// �ܺκ���
	ROCKPUNCH_DATA					m_RockPunchData;
	ROCKPUNCH_DATA					m_RockPunchDataBk;// �����
	static CString					m_EntityType; // ��ƼƼ�� Ÿ�� �̸�

	//////////////////////////////////////////////////////////////////////////
	// �ǽð����� ���Ǵ� ����
	bool							m_b_is_TimerOn; // ��ų ������ Ÿ�̸� �� ������? [2014.11.27 �輺ȯ]	
	bool							m_bAlive;	// ī�彺ų�� ����ֳ� // [2014.11.27 �輺ȯ]
	float							m_CoolTime; // ��Ÿ���� ���� ������ų���� //[2014.11.28 �輺ȯ]
	float							m_DelayTime; // ��ų ���� �ߵ��Ҷ� ���� ���� �ð� // [2014.11.27 �輺ȯ]
	CBoundSphere*					m_pBoundSphere_Body; // ī���� �浹��
	float							m_AccuTime; // ������ �ð�  [2014.11.28 �輺ȯ]
	const SHR_Mesh*					m_BoneMesh_Hat; //[2014.11.29 �輺ȯ]


public:

	CRockPunchSkill();
	virtual ~CRockPunchSkill();

public: //���� ������ �Լ�

	// from IRenderable
	virtual void					Draw();
	// from IProgressable
	virtual void					Update(float _dTime);		
	// from IMessagable
	virtual bool					MsgProc(  SH_MSG* _pMessage );

	// from ICollidable
	virtual HRESULT					InitCollider(); //[2014.11.11 �輺ȯ]

	//�ʱ�ȭ�Ҷ� �ʿ��� �Լ�
	int								Init(LPDIRECT3DDEVICE9 _pDevice , D3DXVECTOR3 _SPos = D3DXVECTOR3(0,0,200),
										 ROCKPUNCH_DATA* pBData= nullptr );

public:// �Ϲ��Լ�
	// ī�彺ų �ߵ�!! int DelayFrame ��ų�ߵ� ���� �ð�(�������� ������ų ���ΰ�) // [2014.11.27 �輺ȯ]
	void							SetDelayTime_for_Begin(D3DXVECTOR3 _StartPos,D3DXVECTOR3 _SeeVec,
														   D3DXMATRIX _CharacterWorldTM ,  int DelayFrame,
														   const SHR_Mesh* _pRefMesh = nullptr); // [2014.11.27 �輺ȯ]

	// ī��Ӽ������� ��ƽ�ũ��Ʈ���� �о���δ�. 
	int								Load_AttrInfo_from_Lua(const CHAR* _pPath);

public: //Get,, set �Լ�

	static CString					GetCreatorName(){return m_EntityType;}
	CRscPack_Character*				GetRscPack(){return &m_RscPack;}

	void							SetAngle_Pitch(float _val){ m_Angle_Pitch = _val;}
	void							SetAngle_Roll(float _val){ m_Angle_Roll = _val;}
	void							SetAngle_Yaw(float _val){ m_Angle_Yaw = _val;}
	void							SetDead(){m_bAlive = false; } //[2014.11.26 �輺ȯ]
	bool							isReady();//[2014.11.28 �輺ȯ]

private: // �����Լ�

	void							_OnDrawStart();
	void							_OnUpdateStart(float _dTime);

};


///////////////////////////////////////////////////////////////////////////
// �������� ����
extern CRockPunchSkill*				G_pRockPunch; // [2014.11.27 �輺ȯ]

/************************************************************************ end of this file *******************************************************************************/