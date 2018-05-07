#pragma once

//////////////////////////////////////////////////////////////////////////
// ���漱�� 	


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
		���ĺ��� �Ǵ� �޽��� �÷����� ���ļ����� �ؾ��ϱ� ������
	�ݵ�� �� �������̽��� ��ӹ޾ƾ� �Ѵ�. (IRenderable�� ����ϸ� �ȵ�!!!) 

					[2014.12.8 ]
*/	
/************************************************************************/
class IAlphaBlendable  
{
	
	friend struct  CompareForAlphaSort;
protected:
	float						 m_DistToCamera;  // ī�޶���� �Ÿ�	

public:// �����Լ�

	virtual void				 DrawAlpha() = 0;

	// _CamPos : ī�޶� ��ġ
	virtual float				 CalDistToCam(D3DXVECTOR3 _CamPos) PURE;

	virtual						 ~IAlphaBlendable(){}

public: 

	void						Set_m_DistToCam(float _Val){m_DistToCamera = _Val;} // [2014.12.8 ]
};

/************************************************************************/
/*			���ļ����� ����  �Լ���ü //[2014.12.8 ]
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
/*  �������̽�  class IThinkable
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
/*     �������̽�   class IPickable                                                        
*/
/************************************************************************/
class IPickable
{
protected:

	bool							isPickable;

public://������
	
	IPickable(){ isPickable = true;}		

public: //�����Լ�

	virtual IPickable*				Picking() =0;
	virtual ~IPickable(){}
};