#pragma once
/***********************************************************************
                  Skill.h
			  // [2014.11.14 배성환]
************************************************************************/


/************************************************************************/
/*							class  ISkill //[2014.12.1 배성환]
					스킬 인터페이스 ,, 모든스킬은 이 클래스를 상속받는다.
*/
/************************************************************************/
class ISkill
{
protected:

public:// 생성자,, 소멸자
	ISkill();
	virtual ~ISkill();

public:// 가상함수

	// 카드스킬 발동!! int DelayFrame 스킬발동 지연 시간(몇프레임 지연시킬 것인가) // [2014.11.27 배성환]
	// _pRefMesh = nullptr : 지연시간 이 다되고나서 참조할 레퍼런스 포인트의 위치를 위해 .. [2014.11.29 배성환]
	virtual void		SetDelayTime_for_Begin(D3DXVECTOR3 _StartPos,
											   D3DXVECTOR3 _SeeVec,
											   D3DXMATRIX _CharacterWorldTM ,
										       int DelayFrame, 
											   const SHR_Mesh* _pRefMesh = nullptr) PURE; // [2014.11.27 배성환]
};