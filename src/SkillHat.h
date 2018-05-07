
/***********************************************************************
                  SkillHat.h
			  // [2014.11.14 ]
************************************************************************/


#pragma  once

/************************************************************************/
/*						//[2014.11.28 ]
				struct HAT_DATA :  Monster�� ���� �ܺ� ����(��ũ��Ʈ ��)
				�� �����Ѵ�.
*/
/************************************************************************/
struct	HAT_DATA
{
	int			    Size; // �� ����ü�� ������

	float			Range; // ���� ��Ÿ� //[2014.11.27 ]
	float			CoolTime; // ��ų ��Ÿ�� //[2014.11.28 ]
	HAT_DATA(){}
};


/************************************************************************/
/*					//[2014.11.28 ]
					class CHatSkill 
				CEntity Ŭ������ ��ӹ޴´�.
*/
/************************************************************************/
class CHatSkill 	: public CEntity, //Base Class
					  virtual public IProgressable,
				      virtual public IRenderable,
				      virtual public ICollidable

{
	
public:
	friend class CHatSkill_Creator;

	CRscPack_Character				m_RscPack;// ���ҽ� ������ҵ�
	D3DXHANDLE						m_hTech;// ���̴�, � ��ũ���� ���ų�?  [2014.11.28 ]
	
	//////////////////////////////////////////////////////////////////////////
	// transform�� ���� �������
	D3DXVECTOR3						m_StartPos; // ��ų ���� ��ġ/   [2014.11.28 ]
	D3DXMATRIX						m_CharacterWorldTM ; //ĳ���Ϳ���TM  [2014.11.28 ]


	//////////////////////////////////////////////////////////////////////////
	// �ܺκ���
	HAT_DATA						m_HatData;
	HAT_DATA						m_HatDataBk;// �����
	static CString					m_EntityType; // ��ƼƼ�� Ÿ�� �̸�

	//////////////////////////////////////////////////////////////////////////
	// �ǽð����� ���Ǵ� ����
	bool							m_b_is_TimerOn; // ��ų ������ Ÿ�̸� �� ������?  [2014.11.28 ]
	bool							m_bAlive;	// ī�彺ų�� ����ֳ� //  [2014.11.28 ]
	float							m_CoolTime; // ��Ÿ���� ���� ������ų���� / [2014.11.28 ]
	float							m_DelayTime; // ��ų ���� �ߵ��Ҷ� ���� ���� �ð� //  [2014.11.28 ]
	CBoundSphere*					m_pBoundSphere_Body; // ī���� �浹��
	float							m_AccuTime; // ������ �ð�  [2014.11.28 ]
	SHR_Mesh*						m_pHatMesh;  // �ٿ�彺�Ǿ� �޽� �� �����͸� �̸� �����س��� �ǽð����� ��� ã�ƾ��ϱ⶧���� // [2014.11.28 ]
		
public:

	CHatSkill();
	virtual ~CHatSkill();

public: //���� ������ �Լ�

	// from IRenderable
	virtual void					Draw();
	// from IProgressable
	virtual void					Update(float _dTime);		
	// from IMessagable
	virtual bool					MsgProc(  SH_MSG* _pMessage );

	// from ICollidable
	virtual HRESULT					InitCollider(); //[2014.11.11 ]

	//�ʱ�ȭ�Ҷ� �ʿ��� �Լ�
	int								Init(LPDIRECT3DDEVICE9 _pDevice , D3DXVECTOR3 _SPos = D3DXVECTOR3(0,0,200),
										 HAT_DATA* pBData= nullptr );

public:// �Ϲ��Լ�
	// ī�彺ų �ߵ�!! int DelayFrame ��ų�ߵ� ���� �ð�(�������� ������ų ���ΰ�) // [2014.11.27 ]
	void							SetDelayTime_for_Begin(D3DXVECTOR3 _StartPos,D3DXVECTOR3 _SeeVec,
														   D3DXMATRIX _CharacterWorldTM ,  int DelayFrame, const SHR_Mesh* _pRefMesh = nullptr); // [2014.11.27 ]

	// ī��Ӽ������� ��ƽ�ũ��Ʈ���� �о���δ�. 
	int								Load_AttrInfo_from_Lua(const CHAR* _pPath);

public: //Get,, set �Լ�

	static CString					GetCreatorName(){return m_EntityType;}
	CRscPack_Character*				GetRscPack(){return &m_RscPack;}

	void							SetAngle_Pitch(float _val){ m_Angle_Pitch = _val;}
	void							SetAngle_Roll(float _val){ m_Angle_Roll = _val;}
	void							SetAngle_Yaw(float _val){ m_Angle_Yaw = _val;}
	void							SetDead(){m_bAlive = false; } //[2014.11.26 ]
	bool							isReady();//[2014.11.28 ]

private: // �����Լ�

	void							_OnDrawStart();
	void							_OnUpdateStart(float _dTime);

};


////////////////////////////////////////////////////////////////////////////
// �������� ����
extern CHatSkill*				G_pHat; // [2014.11.27 ]

/****************************************************** End *****************************************************/