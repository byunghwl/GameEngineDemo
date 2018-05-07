/***********************************************************************
                  Ghost.h
			  // [2014.11.14]
************************************************************************/


#pragma  once

//////////////////////////////////////////////////////////////////////////
#define  GHOSTNUM_MAX  200 //[2014.11.16]
#define	 PASSPOINTNUM_MAX  30 //[2014.11.18]
//////////////////////////////////////////////////////////////////////////
// ���漱�� 


/************************************************************************/
/*						[2014.11.17 ]
				struct GHOST_DATA :  Monster�� ���� �ܺ� ����(��ũ��Ʈ ��)
				�� �����Ѵ�.
*/
/************************************************************************/
struct	GHOST_DATA
{
	int			    Size; // �� ����ü�� ������
	float		    MoveSpeed;// ������ �̵��ӵ� [2014.11.15 ]
	float		    RotSpeed; // ������ ȸ���ӵ� [2014.11.15 ]
	int			    HP;     //������ HP [2014.11.15 ]	
	int			    GhostType;  // ����Ÿ�� (1,2,3�ܰ�)
	int				Attack; // ���� ���ݷ� //[2014.12.5 ]
	D3DXVECTOR3     StartingPoint; // ���� �������� // [2014.11.15 ]
	// ������� �迭 [2014.11.18 ]
	D3DXVECTOR3		Vec_PassPointArr[PASSPOINTNUM_MAX];
	int				PassPointCnt; // ������� �� �� ���� [2014.11.18 ]


	GHOST_DATA();
};



/************************************************************************
                    Ghost DataBase    [2014.11.17]                                           
*************************************************************************/
class  CGhost_DataBase
{
	friend class CGhost;

private:
	// ��ƼƼ ���丮 �� �Ѱ��� ���ڰ�  �������� ���� [2014.11.16 ]
	GhostFactory_Arg		m_FactoryArg_Point1; // [2014.11.16 ]
	GhostFactory_Arg		m_FactoryArg_Point2;
	GhostFactory_Arg		m_FactoryArg_Point3;
	GhostFactory_Arg		m_FactoryArg_Point4;
	// ghost �Ӽ� ������//[2014.11.16 ]
	GHOST_DATA				m_GhostAttr_LV[3]; 	

	// ������� �迭 [2014.11.18 ]
	D3DXVECTOR3				m_Vec_PassPointArr1[PASSPOINTNUM_MAX];
	int						m_PassPointArr1_Size;   // ������ ������ 
	D3DXVECTOR3				m_Vec_PassPointArr2[PASSPOINTNUM_MAX];
	int						m_PassPointArr2_Size;
	D3DXVECTOR3				m_Vec_PassPointArr3[PASSPOINTNUM_MAX];
	int						m_PassPointArr3_Size;
	D3DXVECTOR3				m_Vec_PassPointArr4[PASSPOINTNUM_MAX];
	int						m_PassPointArr4_Size;

	// �ִϸ��̼� Ŭ������
	CAnimator				m_Animator;		

public:// ������  �Ҹ���

	CGhost_DataBase(){};
	~CGhost_DataBase(){};

public:
		
	int						Load_From_Lua(const CHAR* pPath_GhostAttr, const CHAR* pPath_GhostClipInfo,
										  const CHAR* pPath_Ghost_SpawnPoint); //[2014.11.16 ]

};

// ��Ʈ ����Ÿ ���̽� ��������  //[2014.11.16 ]
extern CGhost_DataBase		G_GhostDataBase;




//////////////////////////////////////////////////////////////////////////
// enum 
enum eGhost_Action_State
{
	eGAS_MOVING, // �̵�����
	eGAS_ATTACK,// ���ݻ���
	eGAS_HITED, // �ǰݻ���
	eGAS_DIE    // �״»��� 

};




/************************************************************************/
/*					[2014.7.29 ]
					class CGhost 
				CEntity Ŭ������ ��ӹ޴´�.
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

	CRscPack_Character				m_RscPack;//��Ʈ1 ���ҽ� ������ҵ�
	CRscPack_Character				m_RscPack2;//��Ʈ2 ���ҽ� ������ҵ�  
	CRscPack_Character				m_RscPack3;//��Ʈ3 ���ҽ� ������ҵ�  
	D3DXHANDLE						m_hTech;// ���̴�, � ��ũ���� ���ų�? 
	
	cSHBillBoard*					m_pEffDeath;// ������ ������ ����Ʈ
	cSHBillBoard*					m_pEffGoal;// �������� ������ ������ ����Ʈ 
	//////////////////////////////////////////////////////////////////////////
	// transform�� ���� �������


	//////////////////////////////////////////////////////////////////////////
	// �ܺκ���
	GHOST_DATA						m_GhostData;
	GHOST_DATA						m_GhostDatabk;// �����
	static CString					m_EntityType; // ��ƼƼ�� Ÿ�� �̸�

	//////////////////////////////////////////////////////////////////////////
	// �ǽð����� ���Ǵ� ����
	static int						m_CreatedGhostCnt; // ������ ��Ʈ �� ���� [2014.11.17 ]
	bool							m_bAlive , m_bVisible;	// ������ ����ֳ� [2014.11.17 ]
	int								m_CurPassPoint; // ���� �����ϰ� �ִ� �н� ����Ʈ  [2014.11.17 ]

	CBoundSphere*					m_pBoundSphere_Body; // ���� �浹��
	const SHR_Mesh*					m_pBSMesh_Ghost[3];  // �ٿ�彺�Ǿ� �޽� �� �����͸� �̸� �����س��� �ǽð����� ��� ã�ƾ��ϱ⶧���� [2014.11.26 ]
	
	float							m_DamageTimer;// �Ѵ� �¾����� �����ð����� �������� ���԰� �ϱ� ���� Ÿ�̸�//[2014.11.30 ]
	int								m_eActState; // ���� �ൿ ���� enum 

public:
	CGhost();
	virtual ~CGhost();

public: //�����Լ�

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

	//�ʱ�ȭ�Ҷ� �ʿ��� �Լ�
	int								Init(LPDIRECT3DDEVICE9 _pDevice , D3DXVECTOR3 _SPos = D3DXVECTOR3(0,0,200),
										 GHOST_DATA* pBData= nullptr );

	// �����ͺ��̽����ִ� ���������͸� ����� �ʱ�ȭ �ϴ� �Լ� [2014.11.17 ]
	// int TimeSection : �������������� �ð����� �������� ������ �ε��� [2014.11.18 ]
	int								InitUsingSpawnData(const GhostFactory_Arg& _SpwnInfo, int TimeSection , int PositionNum , bool Alive);

	//���� ������Ʈ �Լ����� ��Ʈ 
	static int						InitGhost_in_RealTime(const float& GameTimer , float _dTime);
public:
	//�̵������� �Լ�
	void							MoveLeft(float& _dTime);
	void							MoveRight(float& _dTime);
	void							MoveForward(float& _dTime);
	void							MoveBack(float& _dTime);
	void							MoveDown(float& _dTime);
	void							MoveUp(float& _dTime);
	//ȸ���� ���� �Լ�
	void							Rot_Pitch(float _Angle);// ���⺤�͸� �������� ȸ��
	void							Rot_Roll(float _Angle);//���溤�� �������� ȸ��
	void							Rot_Yaw(float _Angle);//���⺤�ͱ������� ȸ��

public:// �Ϲ��Լ�
	// ���� �Ӽ������� ��ƽ�ũ��Ʈ���� �о���δ�. //[2014.11.15 ]
	int								Load_AttrInfo_from_Lua(const CHAR* _pPath);
	// �Ѵ� �¾����� HP�� �ް��ϴ� �Լ�
	bool							DamagedFromHit(int _Damage); //[2014.11.30 ]
public: //Get,, set �Լ�

	static CString					GetCreatorName(){return m_EntityType;}
	CRscPack_Character*				GetRscPack(){return &m_RscPack;}

	void							SetAngle_Pitch(float _val){ m_Angle_Pitch = _val;}
	void							SetAngle_Roll(float _val){ m_Angle_Roll = _val;}
	void							SetAngle_Yaw(float _val){ m_Angle_Yaw = _val;}
	void							SetDead(){m_bAlive = false; } //[2014.11.26 ]


private: // �����Լ�

	void							_OnDrawStart();
	void							_OnUpdateStart(float _dTime);


};


//////////////////////////////////////////////////////////////////////////
// �������� ����
extern CGhost*				G_pGhost_Arr[GHOSTNUM_MAX]; //[2014.11.16 ]
extern CGhost*				G_pGhost_Origne; //����� �����̵Ǵ�  ��Ʈ [2014.12.7 ]	


/*************************************************** end of this file **************************************************************/