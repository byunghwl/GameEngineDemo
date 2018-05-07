#include "DXUT.h"
#include "Animator.h"


// ���⼭���� CAnimator class �Լ�����
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// ������ �Ҹ���
//-----------------------------------------------------------------------------
CAnimator::CAnimator()
{ 
	m_Velo = 1.0f; 

	m_AniTime = 0;
	m_CurClipName = "NONE";
	m_bBlendStart = false;
	m_BlendingTime = 0.0f;
	m_BlendMaxTime = 0.16f; //���� ����� �� �ð�
	m_pCurClip = nullptr;

}


CAnimator::~CAnimator()
{};

//-----------------------------------------------------------------------------
// Arg    : 
// Return : 
// Desc   :  ���Կ����� �����ε�
//-----------------------------------------------------------------------------
// CAnimator&		CAnimator::operator=(const CAnimator& _rhs)
// {
// 	(*this) = _
// 
// 	return *this;
// }

//-----------------------------------------------------------------------------
// Desc   : ������Ʈ�Լ�, �������Ӹ��� Anitime�� ������Ų��.
//-----------------------------------------------------------------------------
void	CAnimator::AniTimeUpdate()
{
	m_AniTime += SHGlobal_GetEngineTime * 1000.0f * m_Velo; 

	if(m_bBlendStart == true)
	{
		m_BlendingTime += SHGlobal_GetEngineTime;
	}
	
	if(m_BlendingTime > 0.16f)
	{
		m_BlendingTime = 0.0f;
		m_bBlendStart = false;
	}
}



//-------------------------------------------------------------------------
// Arg	  : CString& _AniName (�ִϸ��̼� ���� �̸�), int start(���� Ű), int end(�� Ű)
// Desc   : std::map ������ ä���ش�. 
//-------------------------------------------------------------------------
void	CAnimator::AddAniClip(CString& _AniName , int start, int end ,bool _isLoop)
{
	assert(start >= 0);
	assert(end >= start);

	if(_AniName.GetLength() <= 0)
	{
		SHUtil::MsgBox("CAnimator::AddAniClip() , _AniName�� �߸��Ǿ����ϴ�. ");
		assert(false);
	}

	CLIP_INFO tempClip(start , end, _isLoop);
	m_stdMap_AniState.insert(std::pair<CString, CLIP_INFO>(_AniName, tempClip) );
}

//-----------------------------------------------------------------------------
// Arg    :  const CHAR* _pName (�ִϸ��̼�Ŭ�� �̸�)
// Return : enum eAni_State  , play or pause
// Desc   : _pName �� �ش��ϴ� �ִϸ��̼��� ���¸� ��ȯ�Ѵ�. 
//-----------------------------------------------------------------------------
int		CAnimator::GetAniState_By_Name(const CHAR* _pName)
{
	if(_pName == nullptr)
	{	SHUtil::MsgBox("CAnimator::Change_Ani_Clip(), _pName�� �߸��Ǿ����ϴ�. ");	assert(false);	}

	CString Name = _pName ;

	std::map<CString, CLIP_INFO>::iterator Iter_Pos;

	Iter_Pos = m_stdMap_AniState.find(Name);

	// �����̸��� �ִ�Ŭ���� ã�� ������.. FAIL ����
	if(Iter_Pos == m_stdMap_AniState.end())
	{	SHUtil::MsgBox("%s �ش��ϴ� Ŭ���̸��� �����ϴ�. !!",_pName );
		return -1; 
	}

	return Iter_Pos->second.eAniState;
	
}


//-----------------------------------------------------------------------------
// Arg    : 
// Return : int
// Desc   : ���� ����ǰ��ִ� �ִϸ��̼�Ŭ�������� <����Ʈ �ߵ������� �����ð�> �� �����´�. 
//-----------------------------------------------------------------------------
int		CAnimator::GetCurClip_EffectDelayTime()
{
	if(m_pCurClip == nullptr){SHUtil::MsgBox("CAnimator::GetCurClip_EffectDelayTime(), Fail , m_pCurClip �� nullptr�Դϴ�.") ; return -1;}
	return m_pCurClip->DelayFrm_for_Eff;
}


//-------------------------------------------------------------------------
// Arg	  :  const CHAR* _pName (�ִϸ��̼�Ŭ�� �̸�)
// Return :  ����(0), ����(1) , 
// Desc   :  �ִ� Ŭ�� ������  �ٲ۴�.
//-------------------------------------------------------------------------
int  	CAnimator::Change_Ani_Clip(const CHAR* _pName)
{
	if(_pName == nullptr)
	{	SHUtil::MsgBox("CAnimator::Change_Ani_Clip(), _pName�� �߸��Ǿ����ϴ�. ");	assert(false);	}

	CString Name = _pName ;

	std::map<CString, CLIP_INFO>::iterator Iter_Pos;

	Iter_Pos = m_stdMap_AniState.find(Name);
	//////////////////////////////////////////////////////////////////////////
	// ���⼭���� ���ϸ��̼� ���� ������ �ֱ�  [2014.11.25 ]

	if((m_AniTime <= m_EndTime) && (m_AniTime>= m_StartTime))
	{	Iter_Pos->second.eAniState = eAniState_Play; 	}
	else
	{
		Iter_Pos->second.eAniState = eAniState_Stop; 
	}


	//////////////////////////////////////////////////////////////////////////
	
	// �����̸��� �ִ�Ŭ���� ã�� ������.. FAIL ����
	if(Iter_Pos == m_stdMap_AniState.end())
	{	return RVT_SEARCH_FAIL; 	}

	// ���� �Ϸ��� Ŭ���� �̹� ����Ǿ��ִٸ�
	if(m_CurClipName.Compare(Iter_Pos->first) == 0) 
	{	return RVT_SAME_CLIP;		}

	// �����̸��� Ŭ���� ã�Ҵٸ�
	m_StartTime = Iter_Pos->second.Start;
	m_EndTime = Iter_Pos->second.End;
	m_CurClipName = Iter_Pos->first;

	m_bBlendStart = true; // �ִϸ��̼� ���� ����

	m_pCurClip = &Iter_Pos->second;

	m_AniTime = m_StartTime; //[2014.11.30 ]

	return RVT_OK;

}

//-----------------------------------------------------------------------------
// Arg    : 
// Return :
// Desc   : 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Desc   :  ��� ���Ͽ��� Ŭ�������� �ҷ��´�. [2014.10.20 ]
//-----------------------------------------------------------------------------
int			CAnimator::Load_ClipInfo_from_Lua(const CHAR* _pPath)
{


	SH_Lua TempLua(_pPath);
	int tableSize; 

	try
	{
		TempLua.GetGlobal_int("Clip_Info_Size" , tableSize);

		lua_getglobal(TempLua.GetLuaState(), "Clip_Info");
	
		for(int i = 1 ; i<= tableSize ; i++)
		{
			char clipName[Max_Name_Len];						// Ŭ���̸�
			int STime, ETime;// Ŭ�� ���۽ð�, ���ð�
			bool IsLoop;
			int DelayFrame;

			lua_rawgeti(TempLua.GetLuaState(), 1, i);
			
			TempLua.Getfield_String("Name", clipName );
			TempLua.Getfield_int("Start", STime);
			TempLua.Getfield_int("End", ETime);
			TempLua.Getfield_bool("IsLoop", IsLoop);
			TempLua.Getfield_int("DelayFrame_Eff", DelayFrame);
			STime*= 30; 
			ETime*= 30;
			CLIP_INFO tempClip(STime, ETime, IsLoop, DelayFrame); 
			m_stdMap_AniState.insert(std::pair<CString, CLIP_INFO>(clipName, tempClip));
		}
	}
	catch(...)
	{
		SHUtil::MsgBox("CAnimator::Load_ClipInfo_from_Lua() , ��ƿ���ó��!!");
		return S_FALSE;
	}

	return S_OK;
}