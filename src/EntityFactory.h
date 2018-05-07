#pragma  once


/***********************************************************************
                                                                   
************************************************************************/
class EntityFactory_Arg
{
public:

	D3DXVECTOR3		Pos;
	float			starttick;
	CHAR			KeyInfoPath[Max_Path_Len];
	CHAR			AttrInfoPath[Max_Path_Len]; //��ƼƼ �Ӽ������� ����ִ� ��ƽ�ũ��Ʈ ������ ��� 
};


/***********************************************************************
							IEntityCreator
			��ƼƼ�� �������ֱ� ���� ũ�������͵��� Base Ŭ����                       
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

				 CBear�� �������� �������ִ� ��ü
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

				 CRoom�� �������� �������ִ� ��ü  
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

		 CGhost�� �������� �������ִ� ��ü  [2014.11.15 ]
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

		 CCardSkill�� �������� �������ִ� ��ü  [2014.11.27 ]
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

		 CRockPunchSkill�� �������� �������ִ� ��ü [2014.11.29 ]
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

		  CHatSkill�� �������� �������ִ� ��ü //[2014.11.28 ]
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


		�� Ŭ������ ��� ��ƼƼ�� �����Ѵ�.  �ݵ�� �ش��ϴ� ��ƼƼ����
			Creator Ŭ������ �̸� Map�� ����س��� �Ѵ�. 


************************************************************************/
class CEntityFactory
{

private:

	std::map<CString  , IEntityCreator*>	m_Map;
	static CEntityFactory*				    pInstance; // �̱���

	CEntityFactory();
	~CEntityFactory();

public:
	// CString _pCreatorName	 : IEntityCreator �� ��ӹ��� Ŭ���� ������ �����ϱ� ���� ���ڿ� Ű��
	// const WCHAR* _RscPackName : .rpi(resource pack info) Ȯ���ڸ� ������ �ִ� ���� �̸�
	// EntityFactory_Arg &_EFArg :  ��������ڰ� �Ѱ��ְ� ���� ������ ��Ƴ��� ����ü
	CGameObject*							CreateEntity(CString _pCreatorName, const WCHAR* _RscPackName ,
														 EntityFactory_Arg &_EFArg);
	// IEntityCreator �� ��ӹ��� Creator�� ����Ѵ�.
	// ���н� S_FALSE  , ������ S_OK
	int										Register(CString _CreatorType, IEntityCreator* _pNewCreator);
	
	//�̱��� ��ü ����
	static CEntityFactory*					GetInstance();
};


#include "EntityFactoryArg.h"
/*************************************************** end of this file **************************************************************/