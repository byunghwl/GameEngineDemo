#include "DXUT.h"
#include "Animator.h"


// 여기서부터 CAnimator class 함수정의
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
// 생성자 소멸자
//-----------------------------------------------------------------------------
CAnimator::CAnimator()
{ 
	m_Velo = 1.0f; 

	m_AniTime = 0;
	m_CurClipName = "NONE";
	m_bBlendStart = false;
	m_BlendingTime = 0.0f;
	m_BlendMaxTime = 0.16f; //블랜딩 진행될 총 시간
	m_pCurClip = nullptr;

}


CAnimator::~CAnimator()
{};

//-----------------------------------------------------------------------------
// Arg    : 
// Return : 
// Desc   :  대입연산자 오버로딩
//-----------------------------------------------------------------------------
// CAnimator&		CAnimator::operator=(const CAnimator& _rhs)
// {
// 	(*this) = _
// 
// 	return *this;
// }

//-----------------------------------------------------------------------------
// Desc   : 업데이트함수, 매프레임마다 Anitime을 누적시킨다.
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
// Arg	  : CString& _AniName (애니메이션 종류 이름), int start(시작 키), int end(끝 키)
// Desc   : std::map 정보를 채워준다. 
//-------------------------------------------------------------------------
void	CAnimator::AddAniClip(CString& _AniName , int start, int end ,bool _isLoop)
{
	assert(start >= 0);
	assert(end >= start);

	if(_AniName.GetLength() <= 0)
	{
		SHUtil::MsgBox("CAnimator::AddAniClip() , _AniName이 잘못되었습니다. ");
		assert(false);
	}

	CLIP_INFO tempClip(start , end, _isLoop);
	m_stdMap_AniState.insert(std::pair<CString, CLIP_INFO>(_AniName, tempClip) );
}

//-----------------------------------------------------------------------------
// Arg    :  const CHAR* _pName (애니메이션클립 이름)
// Return : enum eAni_State  , play or pause
// Desc   : _pName 에 해당하는 애니메이션의 상태를 반환한다. 
//-----------------------------------------------------------------------------
int		CAnimator::GetAniState_By_Name(const CHAR* _pName)
{
	if(_pName == nullptr)
	{	SHUtil::MsgBox("CAnimator::Change_Ani_Clip(), _pName이 잘못되었습니다. ");	assert(false);	}

	CString Name = _pName ;

	std::map<CString, CLIP_INFO>::iterator Iter_Pos;

	Iter_Pos = m_stdMap_AniState.find(Name);

	// 같은이름의 애니클립을 찾지 못했음.. FAIL 리턴
	if(Iter_Pos == m_stdMap_AniState.end())
	{	SHUtil::MsgBox("%s 해당하는 클립이름이 없습니다. !!",_pName );
		return -1; 
	}

	return Iter_Pos->second.eAniState;
	
}


//-----------------------------------------------------------------------------
// Arg    : 
// Return : int
// Desc   : 현재 진행되고있는 애니메이션클립정보의 <이펙트 발동까지의 지연시간> 을 가져온다. 
//-----------------------------------------------------------------------------
int		CAnimator::GetCurClip_EffectDelayTime()
{
	if(m_pCurClip == nullptr){SHUtil::MsgBox("CAnimator::GetCurClip_EffectDelayTime(), Fail , m_pCurClip 이 nullptr입니다.") ; return -1;}
	return m_pCurClip->DelayFrm_for_Eff;
}


//-------------------------------------------------------------------------
// Arg	  :  const CHAR* _pName (애니메이션클립 이름)
// Return :  성공(0), 실패(1) , 
// Desc   :  애니 클립 종류를  바꾼다.
//-------------------------------------------------------------------------
int  	CAnimator::Change_Ani_Clip(const CHAR* _pName)
{
	if(_pName == nullptr)
	{	SHUtil::MsgBox("CAnimator::Change_Ani_Clip(), _pName이 잘못되었습니다. ");	assert(false);	}

	CString Name = _pName ;

	std::map<CString, CLIP_INFO>::iterator Iter_Pos;

	Iter_Pos = m_stdMap_AniState.find(Name);
	//////////////////////////////////////////////////////////////////////////
	// 여기서부터 에니메이션 상태 설정해 주기  [2014.11.25 ]

	if((m_AniTime <= m_EndTime) && (m_AniTime>= m_StartTime))
	{	Iter_Pos->second.eAniState = eAniState_Play; 	}
	else
	{
		Iter_Pos->second.eAniState = eAniState_Stop; 
	}


	//////////////////////////////////////////////////////////////////////////
	
	// 같은이름의 애니클립을 찾지 못했음.. FAIL 리턴
	if(Iter_Pos == m_stdMap_AniState.end())
	{	return RVT_SEARCH_FAIL; 	}

	// 현재 하려는 클립이 이미 실행되어있다면
	if(m_CurClipName.Compare(Iter_Pos->first) == 0) 
	{	return RVT_SAME_CLIP;		}

	// 같은이름의 클립을 찾았다면
	m_StartTime = Iter_Pos->second.Start;
	m_EndTime = Iter_Pos->second.End;
	m_CurClipName = Iter_Pos->first;

	m_bBlendStart = true; // 애니메이션 블랜딩 시작

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
// Desc   :  루아 파일에서 클립정보를 불러온다. [2014.10.20 ]
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
			char clipName[Max_Name_Len];						// 클립이름
			int STime, ETime;// 클립 시작시간, 끝시간
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
		SHUtil::MsgBox("CAnimator::Load_ClipInfo_from_Lua() , 루아예외처리!!");
		return S_FALSE;
	}

	return S_OK;
}