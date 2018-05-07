/************************************************************************/
/*					�ϴ� ���̷�Ʈ ���� ��
					//[2014.12.6 ]
*/
/************************************************************************/

#pragma  once
#pragma comment(lib, "./common/dsound.lib")

class CSManager;
//////////////////////////////////////////////////////////////////////////
// enum 

enum eSoundName
{	
	
	eSound_Enemy_Hit = 0,   // ���ǰ�    �ݵ�� 0�������� ����!!
	eSound_Enemy_Atk, //  ���� ����   [2014.12.9 ]
	eSound_Tedy_Atk, // ������ �Ϲݰ���

	eSound_Skill_Card,
	eSound_Skill_Rock,
	eSound_Skill_Hat,


	eSound_BGM_Play,  // �����÷��� �Ҷ�..
	eSound_BGM_Intro, // ���۹������Ҷ�..
	eSound_BGM_Credit, // ũ���� BGM
	eSound_BGM_Title, // ����Ÿ��Ʋ BGM
	eSound_BGM_Fail, //���о� BGM
	eSound_BGM_Clear, //������ BGM
	eSound_Max
};



/************************************************************************/
/*                     class SHSound
*/
/************************************************************************/
class SHSound
{
	friend class CSManager;

public:
	LPDIRECTSOUND                m_lpDirectSound;
	LPDIRECTSOUNDBUFFER			 m_SoundBuff;

	MMCKINFO					parent;
	MMCKINFO					child;
	WAVEFORMATEX				wavefmt;
	HMMIO						wavefile;

public:
	
	SHSound();
	~SHSound();
	void						Lost();
	
	bool						LoadWave (WCHAR *szFileName);
	// _bLoop : false�� �ѹ��� //[2014.12.6 ]
	void						PlayWave (DWORD _bLoop);

	void						StopWave ();

public:// get.. set

	void						Set_m_LpDirectSound(LPDIRECTSOUND _LPD3dSound){m_lpDirectSound = _LPD3dSound;};						

};



/************************************************************************/
/*                     class CSManager
					//[2014.12.7 ]
*/
/************************************************************************/
class CSManager
{
private:
	SHSound						 m_SoundArr[eSound_Max]; //[2014.12.6 ]
	LPDIRECTSOUND                m_lpDirectSound;
	

public:
	CSManager();
	~CSManager();
	// �ݵ�� ���Լ��� �����ʱ⿡ �ѹ� ȣ���ؾ��Ѵ�. 
	bool						Init (HWND hwnd);


public: // �Ϲ��Լ� 

	// SoundName : enum eSoundName , _bLoop  : �ݺ��ǰ� �Ұ��ΰ�? true or false [2014.12.6 ]
	HRESULT						PlayWave(int SoundName , bool _bLoop );
    // Stop Wav [2014.12.7 ]
	HRESULT						StopWave(int SoundName);
};


//////////////////////////////////////////////////////////////////////////
// ����޴��� ��������
extern CSManager						G_SHSoundMgr; // [2014.12.6 ]

/****************************************************** End *****************************************************/