/***********************************************************************
				GameSysyem.h  
				[2014.11.17 ]
		   
		   ���� �ý��� ������ �Լ��� �ۼ��Ѵ�.                                                      
************************************************************************/
#pragma  once

extern float	  G_GameTimer ;

extern float	  G_GirlHP ; // ������ ü�� 

extern int		  G_CurSceneType; // ���� �� ���� [2014.12.3 ]

extern SH_Timer		G_ButtonAble;// Ÿ��Ʋ��ư������ Ÿ�̸�
//////////////////////////////////////////////////////////////////////////
// �� ����
enum eScene_Type //[2014.12.3 ]
{
	eSceneType_Loading,
	eSceneType_MainTitle,
	eSceneType_MainGame,
	eSceneType_Clear,
	eSceneType_Fail,
	eSceneType_Credit

};

/*************************************  end of "GameSysyem.h" ****************************************/