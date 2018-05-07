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
// 충돌체 타입, 모든 충돌체는 반드시 초기화할때 타입을 지정해준다. [2014.11.10 ]
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
				충돌을 관리하는 메니져
		 테그를 이용해  충돌체를 등록하고 원하는 물체끼리 충돌할수 있다.
		사라진 충돌체를 삭제하여 충돌체크를 안한다. 
					[2014.11.11 ]
*/
/************************************************************************/
class CCollideMgr 
{
	friend class ICollidable;

private:
	static CCollideMgr*							m_pCollMgr;//싱글톤	
	CCollideMgr();
	virtual ~CCollideMgr();

public:
	//싱글톤 인스턴스 반환
	static CCollideMgr*							getInstance(); 


	
public:

	std::list<ICollider*>						m_Collider_List[eCollide_Tag_Max];					


};


/***********************************************************************
                          class   ICollidable

		충돌가능한 속성을 가지는 객체가 이클래스를 상속하여 충돌함수를 호출한다.
							[2014.11.10 ]
************************************************************************/

class ICollidable
{
protected:

	ICollider*						m_pCollidObj; 
	SH_CollideInfo					m_Colli_Info;// 상대방과 부딪힌 충돌정보

public:
	ICollidable();
	virtual ~ICollidable();

public://가상함수

	virtual HRESULT					InitCollider() PURE; // 반드시 이함수를 재정의 해줘야함

public://일반함수
	// , SH_CollideInfo& _Collide_Info 반환될 충돌정보
	bool							Collide_Check(ICollidable* pIColliObj, SH_CollideInfo* _Collide_Info = nullptr);		

	
};



/***********************************************************************
                         class ICollider

	충돌할때 필요한 충돌구나 충돌박스가 상속받을 최상위 부모 클래스 
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

				충돌구 클래스 [2014.11.10 ]
*/					
/************************************************************************/
class  CBoundSphere : public ICollider
{
private:

	float							m_Radius ; // 반지름
	D3DXVECTOR3						m_Center;     // 매프레임마다 변경되는 포지션 [2014.11.11 ]

public:// 생성자 소멸자

	CBoundSphere(); // 반드시 생성자에서 m_ColliType 을 지정해줘야함 [2014.11.11 ]
	virtual ~CBoundSphere();

public:// 가상함수 재정의 
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

				충돌구 클래스 [2014.11.10 ]
*/					
/************************************************************************/
class  CBoundBox_AABB : public ICollider
{
private:

	D3DXVECTOR3						m_Max, m_Min ; // 바운딩박스의 두꼭지점
	D3DXVECTOR3						m_Center;     // 매프레임마다 변경되는 포지션 [2014.11.11 ]

public:// 생성자 소멸자

	CBoundBox_AABB(); // 반드시 생성자에서 m_ColliType 을 지정해줘야함 [2014.11.11 ]
	virtual ~CBoundBox_AABB();

public:// 가상함수 재정의 
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