


#pragma  once

//////////////////////////////////////////////////////////////////////////
// ���漱��
class ASEMesh;
class CEntity;
class cSHBillBoard;


//////////////////////////////////////////////////////////////////////////
// ��Ʈ�÷��� ����
#define  BitFlag_Left   (1)  // ù��°��Ʈ
#define  BitFlag_Right  (2)  // �ι�°��Ʈ
#define  BitFlag_Up     (4)  // ��������Ʈ
#define  BitFlag_Down   (8)  // �׹�° ��Ʈ
#define  BitFlag_LClick (16) // �ټ���° ��Ʈ
#define  BitFlag_1      (32) // ������° ��Ʈ
#define  BitFlag_2      (64) // �ϰ���° ��Ʈ
#define  BitFlag_3      (128) // ������° ��Ʈ

// ��Ʈ�÷��� �˻� ��ũ��
#define  BitFlag_Move	(15)
#define  BitFlag_Skill  (240) // [2014.11.27]

#define  BitFlag_isMoving(X)		 ( X & 15) // ����Ű ��� �Ѱ��� ���ȴ��� �˻��ϱ� ���� �÷���	[2014.11.17 ]
#define  BitFlag_Using_AnySkill(X)	 ( X & 240)// ���ݽ�ų �� ��� �ϳ��� ����Ѵٸ�  // [2014.11.17 ]
#define  BitFlag_isLBtnClick(X)			 ( X & BitFlag_LClick) // LButton Ŭ���߳� �˻��ϴ� ��ũ�� [2014.11.26 ]



//////////////////////////////////////////////////////////////////////////
#define  S_POINT  8.33333f


/************************************************************************/
/*				[2014.7.29]
				struct BEAR_DATA :  Monster�� ���� �ܺ� ����(��ũ��Ʈ ��)
				�� �����Ѵ�.
*/
/************************************************************************/
struct	BEAR_DATA
{
	int			Size; // �� ����ü�� ������
	float		MoveSpeed;// ���� �̵��ӵ� [2014.11.13 ]
	float		RotSpeed; // ������ ȸ���ӵ� [2014.11.13 ]
	
	BEAR_DATA();
};


/************************************************************************/
/*				[2014.7.29]
				class CBear 
				CEntity Ŭ������ ��ӹ޴´�.
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
	friend	class CBearCreator; //���� ��ƼƼ���丮���� �����Ҽ� �ְ� ģ���߰� [2014.11.25 ]

	//////////////////////////////////////////////////////////////////////////
	// ȭ�鿡 ������ ���ҽ� ������� // 
	CRscPack_Character				m_RscPack;// ���ҽ� ������ҵ�
	cSHBillBoard*					m_pEffMagicStick;// ���������޷��ִ� ����Ʈ 

	//////////////////////////////////////////////////////////////////////////
	// ��ų���� 
	CScatterSkill					m_SkillCard;
	CRockPunchSkill					m_SkillRockPunch;
	CHatSkill						m_SkillHat;  // ���ڶ����� ��ų 


	//////////////////////////////////////////////////////////////////////////
	// ���ҽ��߿� �浹�� �ʿ��� �����͸� �̸� �޾Ƴ��´�. 
	CBoundSphere*					m_pBoundSphere_Stick; // �浹���� �����͸� �ʱ�ȭ �Ҷ� �ѹ��� �����س��´�. 				
	CBoundSphere*					m_pBoundSphere_Body; // �浹���� �����͸� �ʱ�ȭ �Ҷ� �ѹ��� �����س��´�. 		
	const SHR_Mesh*					m_BSMesh_Stick;// �浹���ǽð� ������Ʈ�� ���� �̸� ������ �޾Ƴ��� 
	const SHR_Mesh*					m_BoneMesh_Hat;//������ġ��ų ������ ��ġ �����ϱ� ���� �̸� ������ �޾Ƴ��� 
	D3DXVECTOR3						m_PosStick ; // ����� ���� �ǽð� ��ġ


	//////////////////////////////////////////////////////////////////////////
	// �ܺκ���
	BEAR_DATA						m_BearData;
	BEAR_DATA						m_BearDatabk;// �����
	static CString					m_EntityType; // ��ƼƼ�� Ÿ�� �̸�

	//////////////////////////////////////////////////////////////////////////
	// �Ϲݺ��� [2014.11.13 ]
	DWORD							m_SkillbitFlag; // ������������ ��Ʈ�� �����Ͽ� üũ	
	D3DXVECTOR3						m_Vec_Modified_See; // �浹�Ǿ����� ���� ���� ����
	float							m_SkillPoint; // ������ ��ų����Ʈ, �Ѵ� ���������� �����Ѵ�. 

	SH_Timer						m_NormalHit_Timer;// �Ϲݰ��ݿ� ���� Ÿ�̸�  

	// 8���⺤�� �̸� ����� ���� // [2014.11.13 ]
	D3DXVECTOR3						m_Vec_Right; // ������ //
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

public: //�����Լ�

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

	//�ʱ�ȭ�Ҷ� �ʿ��� �Լ�
	int								Init(LPDIRECT3DDEVICE9 _pDevice , D3DXVECTOR3 _SPos = D3DXVECTOR3(0,0,200),
										 BEAR_DATA* pBData= nullptr );
	int								InitGameObject();

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
	// �׵𺣾� �Ӽ������� ��ƽ�ũ��Ʈ���� �о���δ�. 
	int								Load_AttrInfo_from_Lua(const CHAR* _pPath);

	// �ݸ��� �ױ׸� ���ڷ� �浹üũ���ִ� �Լ� 
	bool							ColliChk_Using_Tag(int _tag);

public: //Get,, set �Լ�



	static CString					GetCreatorName(){return m_EntityType;}
	CRscPack_Character*				GetRscPack(){return &m_RscPack;}

	void							SetAngle_Pitch(float _val){ m_Angle_Pitch = _val;}
	void							SetAngle_Roll(float _val){ m_Angle_Roll = _val;}
	void							SetAngle_Yaw(float _val){ m_Angle_Yaw = _val;}


private: // �����Լ�

	void							_OnDrawStart();
	void							_OnUpdateStart(float _dTime);


};


//////////////////////////////////////////////////////////////////////////
// �������� �����

extern CBear*					     G_pTedyBear;


/****************************************************** End *****************************************************/