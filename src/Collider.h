/***********************************************************************
                          Collider.h
						 [2014.11.10]
************************************************************************/
#pragma once

//////////////////////////////////////////////////////////////////////////
// pre declaration

class ICollider;
class ICollidable;



//////////////////////////////////////////////////////////////////////////
// �浹ü Ÿ��, ��� �浹ü�� �ݵ�� �ʱ�ȭ�Ҷ� Ÿ���� �������ش�. [2014.11.10 ]
enum eCollider_Type
{
	eColli_Type_Sphere,
	eColli_Type_Box_AABB,
};


//////////////////////////////////////////////////////////////////////////
enum eCollide_Tag
{	
	eCollide_Tag_Ghost = 0,
	eCollide_Tag_BoxesInRoom,
	eCollide_Tag_Max 
};
/************************************************************************/
/*					class CCollideMgr 
				�浹�� �����ϴ� �޴���
		 �ױ׸� �̿���  �浹ü�� ����ϰ� ���ϴ� ��ü���� �浹�Ҽ� �ִ�.
		����� �浹ü�� �����Ͽ� �浹üũ�� ���Ѵ�. 
					[2014.11.11 ]
*/
/************************************************************************/
class CCollideMgr 
{
	friend class ICollidable;

private:
	static CCollideMgr*							m_pCollMgr;//�̱���	
	CCollideMgr();
	virtual ~CCollideMgr();

public:
	//�̱��� �ν��Ͻ� ��ȯ
	static CCollideMgr*							getInstance(); 


	
public:

	std::list<ICollider*>						m_Collider_List[eCollide_Tag_Max];					


};


/***********************************************************************
                          class   ICollidable

		�浹������ �Ӽ��� ������ ��ü�� ��Ŭ������ ����Ͽ� �浹�Լ��� ȣ���Ѵ�.
							[2014.11.10 ]
************************************************************************/

class ICollidable
{
protected:

	ICollider*						m_pCollidObj; 
	SH_CollideInfo					m_Colli_Info;// ����� �ε��� �浹����

public:
	ICollidable();
	virtual ~ICollidable();

public://�����Լ�

	virtual HRESULT					InitCollider() PURE; // �ݵ�� ���Լ��� ������ �������

public://�Ϲ��Լ�
	// , SH_CollideInfo& _Collide_Info ��ȯ�� �浹����
	bool							Collide_Check(ICollidable* pIColliObj, SH_CollideInfo* _Collide_Info = nullptr);		

	
};



/***********************************************************************
                         class ICollider

	�浹�Ҷ� �ʿ��� �浹���� �浹�ڽ��� ��ӹ��� �ֻ��� �θ� Ŭ���� 
						[2014.11.10 ]

************************************************************************/
class ICollider
{
	friend class ICollidable;
public:
	
	int							m_ColliType;

public:
	ICollider(){};
	virtual ~ICollider(){};

public:

	virtual bool				Colli_Check(ICollider* _pCollider, SH_CollideInfo* _Collide_Info = nullptr) PURE;
};

 


/************************************************************************/
/*					class  CBoundSphere

				�浹�� Ŭ���� [2014.11.10 ]
*/					
/************************************************************************/
class  CBoundSphere : public ICollider
{
private:

	float							m_Radius ; // ������
	D3DXVECTOR3						m_Center;     // �������Ӹ��� ����Ǵ� ������ [2014.11.11 ]

public:// ������ �Ҹ���

	CBoundSphere(); // �ݵ�� �����ڿ��� m_ColliType �� ����������� [2014.11.11 ]
	virtual ~CBoundSphere();

public:// �����Լ� ������ 
	//from 
	virtual bool					Colli_Check(ICollider* _pCollider, SH_CollideInfo* _Collide_Info = nullptr);

public:// get..set

	void							SetRadius(float _Rad){m_Radius = _Rad;}   
	void							SetCenter(D3DXVECTOR3 _Pos){m_Center = _Pos;}

	float							GetRadius() const {return m_Radius;}
	D3DXVECTOR3						GetCenter() const {return m_Center;}

};


/************************************************************************/
/*					class  CBoundBox_AABB

				�浹�� Ŭ���� [2014.11.10 ]
*/					
/************************************************************************/
class  CBoundBox_AABB : public ICollider
{
private:

	D3DXVECTOR3						m_Max, m_Min ; // �ٿ���ڽ��� �β�����
	D3DXVECTOR3						m_Center;     // �������Ӹ��� ����Ǵ� ������ [2014.11.11 ]

public:// ������ �Ҹ���

	CBoundBox_AABB(); // �ݵ�� �����ڿ��� m_ColliType �� ����������� [2014.11.11 ]
	virtual ~CBoundBox_AABB();

public:// �����Լ� ������ 
	//from ICollider
	virtual bool					Colli_Check(ICollider* _pCollider, SH_CollideInfo* _Collide_Info = nullptr);

public:// get..set

	void							SetMax(D3DXVECTOR3 _Val){m_Max = _Val;}   
	void							SetMin(D3DXVECTOR3 _Val){m_Min = _Val;}   
	void							SetCenter(D3DXVECTOR3 _Pos){m_Center = _Pos;}

	D3DXVECTOR3						GetMax() const {return m_Max;}
	D3DXVECTOR3						GetMin() const {return m_Min;}
	D3DXVECTOR3						GetCenter() const {return m_Center;}

};