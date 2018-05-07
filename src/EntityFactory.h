#pragma  once


/***********************************************************************
                                                                   
************************************************************************/
class EntityFactory_Arg
{
public:

	D3DXVECTOR3		Pos;
	float			starttick;
	CHAR			KeyInfoPath[Max_Path_Len];
	CHAR			AttrInfoPath[Max_Path_Len]; //엔티티 속성정보가 들어있는 루아스크립트 파일의 경로 
};


/***********************************************************************
							IEntityCreator
			엔티티를 생성해주기 위한 크리에이터들의 Base 클래스                       
************************************************************************/
class IEntityCreator
{
protected:
public:
	IEntityCreator(){};
	virtual ~IEntityCreator(){};

	virtual CGameObject*				CreateEntity(const WCHAR* _pRscPackPath, EntityFactory_Arg &_EFArg) PURE;
};


/***********************************************************************
                        CBearCreator

				 CBear만 전용으로 생성해주는 객체
************************************************************************/
class CBearCreator : public IEntityCreator
{

private:
public:
	CBearCreator();
	~CBearCreator();
	
public:
	virtual	CGameObject*				CreateEntity(const WCHAR* _pRscPackName, EntityFactory_Arg &_EFArg);
};


/***********************************************************************
                        CRoom_Creator

				 CRoom만 전용으로 생성해주는 객체  
************************************************************************/
class CRoom_Creator : public IEntityCreator
{

private:
public:
	CRoom_Creator();
	~CRoom_Creator();
	
public:
	virtual	CGameObject*				CreateEntity(const WCHAR* _pRscPackName, EntityFactory_Arg &_EFArg);
};

/***********************************************************************
                        CGhost_Creator  

		 CGhost만 전용으로 생성해주는 객체  [2014.11.15 ]
************************************************************************/
class CGhost_Creator : public IEntityCreator
{

private:
public:
	CGhost_Creator();
	~CGhost_Creator();
	
public:
	virtual	CGameObject*				CreateEntity(const WCHAR* _pRscPackName, EntityFactory_Arg &_EFArg);
};


/***********************************************************************
                        CCardSkill_Creator  

		 CCardSkill만 전용으로 생성해주는 객체  [2014.11.27 ]
************************************************************************/
class CCardSkill_Creator : public IEntityCreator
{

private:
public:
	CCardSkill_Creator();
	~CCardSkill_Creator();
	
public:
	virtual	CGameObject*				CreateEntity(const WCHAR* _pRscPackName, EntityFactory_Arg &_EFArg);
};

/***********************************************************************
                        CRockPunchSkill_Creator  

		 CRockPunchSkill만 전용으로 생성해주는 객체 [2014.11.29 ]
************************************************************************/
class CRockPunchSkill_Creator : public IEntityCreator
{

private:
public:
	CRockPunchSkill_Creator();
	~CRockPunchSkill_Creator();
	
public:
	virtual	CGameObject*				CreateEntity(const WCHAR* _pRscPackName, EntityFactory_Arg &_EFArg);
};
/***********************************************************************
                        CHatSkill_Creator  

		  CHatSkill만 전용으로 생성해주는 객체 //[2014.11.28 ]
************************************************************************/
class CHatSkill_Creator : public IEntityCreator
{

private:
public:
	CHatSkill_Creator();
	~CHatSkill_Creator();
	
public:
	virtual	CGameObject*				CreateEntity(const WCHAR* _pRscPackName, EntityFactory_Arg &_EFArg);
};



/***********************************************************************

                            CEntityFactory


		이 클래스로 모든 엔티티를 생성한다.  반드시 해당하는 엔티티전용
			Creator 클래스를 미리 Map에 등록해놔야 한다. 


************************************************************************/
class CEntityFactory
{

private:

	std::map<CString  , IEntityCreator*>	m_Map;
	static CEntityFactory*				    pInstance; // 싱글톤

	CEntityFactory();
	~CEntityFactory();

public:
	// CString _pCreatorName	 : IEntityCreator 를 상속받은 클래스 종류를 구별하기 위한 문자열 키값
	// const WCHAR* _RscPackName : .rpi(resource pack info) 확장자를 가지고 있는 파일 이름
	// EntityFactory_Arg &_EFArg :  엔진사용자가 넘겨주고 싶은 값들을 모아놓은 구조체
	CGameObject*							CreateEntity(CString _pCreatorName, const WCHAR* _RscPackName ,
														 EntityFactory_Arg &_EFArg);
	// IEntityCreator 를 상속받은 Creator를 등록한다.
	// 실패시 S_FALSE  , 성공시 S_OK
	int										Register(CString _CreatorType, IEntityCreator* _pNewCreator);
	
	//싱글톤 객체 리턴
	static CEntityFactory*					GetInstance();
};


#include "EntityFactoryArg.h"
/*************************************************** end of this file **************************************************************/