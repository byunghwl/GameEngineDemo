/************************************************************************/
/*							
							Unit Ŭ���� 
					ĳ���� , ���� , npc, ���ΰ� ���� base class
*/
/************************************************************************/
#pragma  once

//////////////////////////////////////////////////////////////////////////
// ���漱��
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
			������ ������ �ִ� �⺻ �Ӽ����� �ܺο��� �Է¹��� ����
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

	
	// transform�� ���� �������
	D3DXVECTOR3						m_Vec_StartSee; // ȭ�鿡 ��ó�� �����Ǿ����� �ٶ󺸰��ִ� ����
	D3DXVECTOR3						m_VecRight, m_VecUp, m_VecSee;
	D3DXVECTOR3						m_Vec_DestVector; //���Ϸ��� �ϴ� ��ǥ����
	D3DXMATRIX						m_MWorld;		// ���� ���� ���  =  ȸ�� *  �̵� $
	D3DXVECTOR3						m_Pos;          //���� ���� ����  ��ġ
	D3DXVECTOR3						m_PrePos;       // �̸� �浹üũ�ؼ� �������ҋ� �˻������ ����ϴ� ��ġ

	D3DXMATRIX						m_MTrans;       //�̵����
	D3DXMATRIX						m_MRot, m_MRotPitch,  m_MRotRoll,  m_MRotYaw;		   //ȸ����� '����', '�������', '�������', '�������' ȸ�����
	 
	float							m_Angle_Pitch; // �������͸� �������� ȸ���� ����
	float							m_Angle_Roll;  // �ü� ���͸� �������� ȸ���� ����
	float							m_Angle_Yaw;   //  up ���͸� �������� ȸ���� ����
	
	bool							m_isRotating; //���� ȸ���ϰ� �ֳ�?? (Ű���� ���������ΰ�?)
	//�ܺε���Ÿ

public:

	CEntity(){};
	CEntity(LPDIRECT3DDEVICE9 _pDevice);
	virtual ~CEntity();

public: //�����Լ�
	
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