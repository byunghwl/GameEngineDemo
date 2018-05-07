#pragma  once

enum eAni_State
{
	eAniState_Play,
	eAniState_Stop
};

struct CLIP_INFO 
{
	int Start, End;
	int eAniState;
	bool isLoop; // 루프애니메이션인가? 아니면 OneTimeAni인가  
	int DelayFrm_for_Eff; // 이펙트 발동까지 의 지연시간 

	CLIP_INFO(){eAniState = eAniState_Stop;}
	CLIP_INFO(int _start , int _end, bool _Isloop, int _DelayFrm_for_Eff = 0)
	{ 
		Start = _start; 
		End = _end;
		isLoop = _Isloop;
		eAniState = eAniState_Stop;
		DelayFrm_for_Eff = _DelayFrm_for_Eff;
	}
};
/***********************************************************************
						class CAnimator                                     
************************************************************************/
class CAnimator
{
protected:

	std::map<CString, CLIP_INFO>			m_stdMap_AniState;
	float									m_AniTime;			// 각개체의 고유 시간
	float									m_Velo;				//곱해질 속도

	CString									m_CurClipName;     // 현재 선택된 클립 이름
	int										m_StartTime;	   // 현재 선택된 애니 타임의 시작
	int									    m_EndTime;        // 현재 선택된 애니 타임의 끝
	// 애니메이션 블랜딩 할때 사용할 변수들
	bool									m_bBlendStart;	//애니 블랜딩 시작
	float									m_BlendingTime; //블랜딩이 시작되고 나서부터 지난 시간
	float									m_BlendMaxTime; // 블랜딩이 진행되는 총 시간
	CLIP_INFO*								m_pCurClip; // 현재 클립 포인터  [2014.11.25 ]

public:

	enum ReturnValueType
	{
		RVT_OK, // 정상적으로 함수실행
		RVT_SEARCH_FAIL,// 이름을 찾지 못하거나 값을 찾지 못함 , 함수역할 실패
		RVT_SAME_CLIP //  이미 같은 종류의 클립이 선택되어 있음
	};

public:// 생성자 , 소멸자

	CAnimator();
	virtual ~CAnimator();

public:// 연산자 오버로딩

	//CAnimator&								operator=(const CAnimator& _rhs);

public:// 일반함수

	void									AddAniClip(CString& _AniName , int start, int end ,bool _isLoop);//std::map 정보를 채워준다. 
	// m_AniTime 을 매프레임마다 업데이트 시켜준다.
	void									AniTimeUpdate();
	// 매프레임 마다 블랜딩  타이머를 증가시켜줄때 사용
	void									Add_BlendTime(const float& _val ){m_BlendingTime += _val;}

	// _pName 에 해당하는 애니메이션의 상태를 반환한다.   [2014.11.25 ]
	int										GetAniState_By_Name(const CHAR* _pName);

public: // get set

	float									SetVelo(const float &_Val){m_Velo = _Val;}
	void									SetAniTime(const float &_Val){m_AniTime = _Val;}
	void									SetAniTimeFirst(){m_AniTime = m_StartTime;}
	void									Set_BlendingMaxTime(const float& _val){m_BlendMaxTime  = _val;}
	void									Set_m_BlendStart(bool _Val){m_bBlendStart = _Val;}

	float									GetAniTime(){return m_AniTime ;}
	int										GetStartTime(){return m_StartTime;}
	int										GetEndTime(){return m_EndTime;}
	float									GetBlendingTime(){return m_BlendingTime;}
	float									GetBlendMaxTime(){return m_BlendMaxTime;}
	CLIP_INFO*								GetCurClipInfo() const{ return m_pCurClip;} 
	int										GetCurClip_EffectDelayTime(); 
	bool									is_BlendStart(){return m_bBlendStart;}

public:
	//  애니 클립 종류를  바꾼다.
	int										Change_Ani_Clip(const CHAR* _pName);
	// 루아스크립트로부터 클립정보를 불러온다.
	int										Load_ClipInfo_from_Lua(const CHAR* _pPath);

};