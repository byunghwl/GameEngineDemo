/************************************************************************/
/*						Resource.h

*/
/************************************************************************/

#pragma  once


/************************************************************************/
/*					class CFX_SHADER  
				다이렉트 엑스 이펙트 프레임 워크 [2014.10.22 ]
*/
/************************************************************************/
class CFX_SHADER  : public IResource
{
public:

	LPD3DXEFFECT						m_pShader; // 세이더 객체
	int									m_MaxPass;

public:

	CFX_SHADER();
	virtual ~CFX_SHADER();
	int									Create(const WCHAR* _pName , const WCHAR* _pPath  );

public:// get.. set
	void								SetMaxPass(int val){m_MaxPass = val;}

	LPD3DXEFFECT						GetPointer(){return m_pShader;}
	int							        GetMaxPass(){return m_MaxPass;}
};




/***********************************************************************
           // 내 엔진에 공통적으로 적용될 수 있는 이펙트 프레임 
		                 워크 상수 테이블         // [2014.10.30 ]                                               
************************************************************************/

class C_ConstantTable
{

public:
	//////////////////////////////////////////////////////////////////////////
	//C_ConstantTable은 시스템 전역에서 접근할 수 있도록 퍼블릭으로 ...
	D3DXHANDLE			h_LightDiff;  // 빛의 색
	D3DXHANDLE			h_LightDir;   // 빛의 방향
	D3DXHANDLE 			h_LightAmbi;  // 빛의 주변광
	D3DXHANDLE 			h_ViewPos;   // 카메라 위치
	D3DXHANDLE 			h_ViewProjTM; //view * proj TM
	D3DXHANDLE 			h_WorldTM;  // World TM
	D3DXHANDLE 			h_MatPallete;// 스키닝을 위한 메트릭스 팔레트
	D3DXHANDLE 			h_Mtrl_SpecularPower; // 재질의 스페큘러 파워
	D3DXHANDLE			h_PointLightPos;  //포인트라이트의 포지션   [2014.11.12 ]
	D3DXHANDLE			h_PointLightRadius;  //포인트라이트의 범위  [2014.11.12 ]

	D3DXHANDLE			h_LineColor;  //외각선 칼라  [2014.11.13 ]
	D3DXHANDLE			h_LineOffset;  //외각선 두꺠 범위  [2014.11.13 ]
	D3DXHANDLE			h_LimLightColor;// 림라이트칼라
	D3DXHANDLE			h_LimOffset; // 림라이트 강도
	D3DXHANDLE			h_UVAni_Val ; // 유븨 애니메이션 할때 필요한 값 // [2014.12.3 ]
	//////////////////////////////////////////////////////////////////////////
	// 여기서부터 텍스쳐 핸들
	D3DXHANDLE 			h_TexDiffMap ;  //  디퓨즈 텍스쳐의 핸들
	D3DXHANDLE 			h_TexSpecularMap ;  //  스페큘러맵 텍스쳐의 핸들
	D3DXHANDLE 			h_TexOpacityMap ;  // 투명맵 텍스쳐의 핸들
	D3DXHANDLE 			h_TexNormMap ;  //  노말맵 텍스쳐의 핸들
	D3DXHANDLE 			h_TexShadowMap; // 그림자맵 텍스쳐
	//포스트 이펙트
	D3DXHANDLE			h_TexTargetResult; // 포스트이펙트 할때 최종적으로 합쳐져서 그려질 텍스쳐 [2014.11.8 ]
	D3DXHANDLE			h_Tex_Pass0;       // 패스가 여러개일때 패스별로 텍스쳐저장이 필요할떄 [2014.11.8 ]
	D3DXHANDLE			h_Tex_Pass1;
	D3DXHANDLE			h_Tex_Pass2;
	D3DXHANDLE			h_Tex_Pass3;
	D3DXHANDLE			h_Tex_Pass4;
	D3DXHANDLE			h_Tex_Pass5;
	D3DXHANDLE			h_Tex_Pass6;
	//////////////////////////////////////////////////////////////////////////
	// 여기서부터 테크니큐 핸들
	D3DXHANDLE 			h_Tech_Skin_DirLight; // 스키닝 + ppl + directional Light
	D3DXHANDLE 			h_Tech_Skin_SpecularMap_DirLight; // 스키닝 + ppl + 스페큘러맵 적용 +  directional Light [2014.11.6 ]
	D3DXHANDLE			h_Tech_NoEffect;

public:
	C_ConstantTable();
	~C_ConstantTable();

	int			        Initialize();


};


//////////////////////////////////////////////////////////////////////////
// 외부 전역변수 선언

extern CFX_SHADER            G_FXShader;
extern C_ConstantTable       G_ConstantTable;
//////////////////////////////////////////////////////////////////////////
// 외부 전역변수 선언


/****************************************************** End *****************************************************/
