
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
				struct SCATTER_DATA :  Monster�� ���� �ܺ� ����(��ũ��Ʈ ��)
				�� �����Ѵ�.
*/
/************************************************************************/
struct	SCATTER_DATA
{
	int			    Size; // �� ����ü�� ������
	float		    MoveSpeed;// ī���� �̵��ӵ� // [2014.11.27 ]
	float			Range; // ī�� ��Ÿ� //[2014.11.27 ]
	float			CoolTime; // ��ų ��Ÿ�� //[2014.11.28 ]
	float			EachAngle; // �߻�� ī��� ������ ��ѷ����� ���� //[2014.11.30 ]
	SCATTER_DATA(){MoveSpeed = 500.0f; }
};


/************************************************************************/
/*					// [2014.11.27 ]
					class CScatterSkill 
				CEntity Ŭ������ ��ӹ޴´�.
*/
/************************************************************************/
class CScatterSkill 	: public CEntity //Base Class
{
protected:


	
	D3DXHANDLE						m_hTech;// ���̴�, � ��ũ���� ���ų�? [2014.10.30 ]
	
	//////////////////////////////////////////////////////////////////////////
	// transform�� ���� �������
	D3DXVECTOR3						m_StartPos; // ��ų ���� ��ġ/  [2014.11.27 ]
	D3DXMATRIX						m_CharacterWorldTM ; //ĳ���Ϳ���TM  [2014.11.27 ]

	CCardSkill						m_Card[MAXCARD_NUM];// �ټ������� �߻簡���ϰ� �ϱ�����..  //[2014.12.1 ]
	//////////////////////////////////////////////////////////////////////////
	// �ܺκ���
	SCATTER_DATA					m_ScatterData;
	SCATTER_DATA					m_ScatterDataBk;// �����
	static CString					m_EntityType; // ��ƼƼ�� Ÿ�� �̸�

	//////////////////////////////////////////////////////////////////////////
	// �ǽð����� ���Ǵ� ����
	bool							m_b_is_TimerOn; // ��ų ������ Ÿ�̸� �� ������? [2014.11.27 ]	
	bool							m_bAlive;	// ī�彺ų�� ����ֳ� // [2014.11.27 ]
	float							m_CoolTime; // ��Ÿ���� ���� ������ų���� //[2014.11.28 ]
	float							m_DelayTime; // ��ų ���� �ߵ��Ҷ� ���� ���� �ð� // [2014.11.27 ]
	
	float							m_AccuTime; // ������ �ð�  [2014.11.28 ]


public:

	CScatterSkill();
	virtual ~CScatterSkill();

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
	int								Init(LPDIRECT3DDEVICE9 _pDevice , D3DXVECTOR3 _SPos = D3DXVECTOR3(0,0,0),
										 SCATTER_DATA* pScatData= nullptr );

public:// �Ϲ��Լ�
	// ī�彺ų �ߵ�!! int DelayFrame ��ų�ߵ� ���� �ð�(�������� ������ų ���ΰ�) // [2014.11.27 ]
	// _pRefMesh = nullptr : �����ð� �� �ٵǰ��� ������ ���۷��� ����Ʈ�� ��ġ�� ���� .. [2014.11.29 ]
	void							SetDelayTime_for_Begin(D3DXVECTOR3 _StartPos,D3DXVECTOR3 _SeeVec, D3DXMATRIX _CharacterWorldTM ,
														   int DelayFrame, const SHR_Mesh* _pRefMesh = nullptr); // [2014.11.27 ]

	// ī��Ӽ������� ��ƽ�ũ��Ʈ���� �о���δ�. 
	int								Load_AttrInfo_from_Lua(const CHAR* _pPath);

public: //Get,, set �Լ�

	static CString					GetCreatorName(){return m_EntityType;}


	void							SetAngle_Pitch(float _val){ m_Angle_Pitch = _val;}
	void							SetAngle_Roll(float _val){ m_Angle_Roll = _val;}
	void							SetAngle_Yaw(float _val){ m_Angle_Yaw = _val;}
	void							SetDead(){m_bAlive = false; } //[2014.11.26 ]
	bool							isReady();//[2014.11.28 ]

private: // �����Լ�

	void							_OnDrawStart();
	void							_OnUpdateStart(float _dTime);

};


///////////////////////////////////////////////////////////////////////////
// �������� ����
extern CScatterSkill*				G_pScatter; // [2014.11.30 ]

/************************************************************************ end of this file *******************************************************************************/