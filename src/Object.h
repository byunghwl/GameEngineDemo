#pragma once

//////////////////////////////////////////////////////////////////////////
// 전방선언 	


/************************************************************************


************************************************************************/
class IRenderable
{
protected:

public:

	virtual void				Draw() =0;
	virtual ~IRenderable(){}
};




/************************************************************************/
/*					class IAlphaBlendable  
		알파블랜딩 되는 메쉬나 플레인은 알파소팅을 해야하기 때문에
	반드시 이 인터페이스를 상속받아야 한다. (IRenderable을 상속하면 안됨!!!) 

					[2014.12.8 ]
*/	
/************************************************************************/
class IAlphaBlendable  
{
	
	friend struct  CompareForAlphaSort;
protected:
	float						 m_DistToCamera;  // 카메라와의 거리	

public:// 가상함수

	virtual void				 DrawAlpha() = 0;

	// _CamPos : 카메라 위치
	virtual float				 CalDistToCam(D3DXVECTOR3 _CamPos) PURE;

	virtual						 ~IAlphaBlendable(){}

public: 

	void						Set_m_DistToCam(float _Val){m_DistToCamera = _Val;} // [2014.12.8 ]
};

/************************************************************************/
/*			알파소팅을 위한  함수객체 //[2014.12.8 ]
*/
/************************************************************************/

struct  CompareForAlphaSort
{
	bool operator() (IAlphaBlendable* _pLeft, IAlphaBlendable* _pRight)
	{
		return _pLeft->m_DistToCamera > _pRight->m_DistToCamera; // [2014.12.8 ]
	}
};


/************************************************************************/
/*                                                                
*/
/************************************************************************/
class IMessagable
{
protected:

public:

	virtual bool				MsgProc(SH_MSG* _pMessage ) = 0;
	virtual ~IMessagable(){}
};

/************************************************************************/
/*                                                                
*/
/************************************************************************/
class IProgressable
{
protected:

public:

	virtual void				Update(float _dTime) =0;
	virtual ~IProgressable(){}
	
};

/************************************************************************/
/*  인터페이스  class IThinkable
*/
/************************************************************************/
class IThinkable
{
protected:

public:

	virtual void				AIUpdate(float _dTime) = 0;
	virtual ~IThinkable(){}
};

/************************************************************************/
/*     인터페이스   class IPickable                                                        
*/
/************************************************************************/
class IPickable
{
protected:

	bool							isPickable;

public://생성자
	
	IPickable(){ isPickable = true;}		

public: //가상함수

	virtual IPickable*				Picking() =0;
	virtual ~IPickable(){}
};