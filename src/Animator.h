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
	bool isLoop; // �����ִϸ��̼��ΰ�? �ƴϸ� OneTimeAni�ΰ�  
	int DelayFrm_for_Eff; // ����Ʈ �ߵ����� �� �����ð� 

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
	float									m_AniTime;			// ����ü�� ���� �ð�
	float									m_Velo;				//������ �ӵ�

	CString									m_CurClipName;     // ���� ���õ� Ŭ�� �̸�
	int										m_StartTime;	   // ���� ���õ� �ִ� Ÿ���� ����
	int									    m_EndTime;        // ���� ���õ� �ִ� Ÿ���� ��
	// �ִϸ��̼� ���� �Ҷ� ����� ������
	bool									m_bBlendStart;	//�ִ� ���� ����
	float									m_BlendingTime; //������ ���۵ǰ� �������� ���� �ð�
	float									m_BlendMaxTime; // ������ ����Ǵ� �� �ð�
	CLIP_INFO*								m_pCurClip; // ���� Ŭ�� ������  [2014.11.25 ]

public:

	enum ReturnValueType
	{
		RVT_OK, // ���������� �Լ�����
		RVT_SEARCH_FAIL,// �̸��� ã�� ���ϰų� ���� ã�� ���� , �Լ����� ����
		RVT_SAME_CLIP //  �̹� ���� ������ Ŭ���� ���õǾ� ����
	};

public:// ������ , �Ҹ���

	CAnimator();
	virtual ~CAnimator();

public:// ������ �����ε�

	//CAnimator&								operator=(const CAnimator& _rhs);

public:// �Ϲ��Լ�

	void									AddAniClip(CString& _AniName , int start, int end ,bool _isLoop);//std::map ������ ä���ش�. 
	// m_AniTime �� �������Ӹ��� ������Ʈ �����ش�.
	void									AniTimeUpdate();
	// �������� ���� ����  Ÿ�̸Ӹ� ���������ٶ� ���
	void									Add_BlendTime(const float& _val ){m_BlendingTime += _val;}

	// _pName �� �ش��ϴ� �ִϸ��̼��� ���¸� ��ȯ�Ѵ�.   [2014.11.25 ]
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
	//  �ִ� Ŭ�� ������  �ٲ۴�.
	int										Change_Ani_Clip(const CHAR* _pName);
	// ��ƽ�ũ��Ʈ�κ��� Ŭ�������� �ҷ��´�.
	int										Load_ClipInfo_from_Lua(const CHAR* _pPath);

};