/************************************************************************/
/*					Resource.cpp			
*/
/************************************************************************/

#include "DXUT.h"
#include "Shader.h"

//////////////////////////////////////////////////////////////////////////
// 전역변수 선언

C_ConstantTable    G_ConstantTable;
CFX_SHADER            G_FXShader; // [2014.10.29 ]  전역변수로 통합셰이더 접근가능하게 하기



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
//
// 여기서부터 class C_ConstantTable 의 함수정의
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
//-------------------------------------------------------------------------
// 생성자 
//-------------------------------------------------------------------------
C_ConstantTable::C_ConstantTable()
{
    h_LightDiff = NULL;  // 빛의 색
	h_LightDir= NULL;   // 빛의 방향
	h_LightAmbi= NULL;  // 빛의 주변광
	h_ViewPos= NULL;   // 카메라 위치
	h_ViewProjTM= NULL; //view * proj TM
	h_WorldTM= NULL;  // World TM
	h_MatPallete= NULL;// 스키닝을 위한 메트릭스 팔레트
	h_Mtrl_SpecularPower= NULL; // 재질의 스페큘러 파워

	h_TexDiffMap= NULL ;  //  디퓨즈 텍스쳐의 핸들
	h_TexSpecularMap= NULL ;  //  스페큘러맵 텍스쳐의 핸들
	h_TexOpacityMap= NULL ;  // 투명맵 텍스쳐의 핸들
	h_TexNormMap= NULL ;  //  노말맵 텍스쳐의 핸들
	h_TexShadowMap= NULL; // 그림자맵 텍스쳐

}
//-------------------------------------------------------------------------
// 소멸자
//-------------------------------------------------------------------------
C_ConstantTable::~C_ConstantTable()
{

}

//-------------------------------------------------------------------------
// 초기화 함수
//-------------------------------------------------------------------------
int		C_ConstantTable::Initialize()
{
	LPD3DXEFFECT tempEffect =  G_FXShader.GetPointer();
	// 핸들 얻어서 상수테이블에 해당하는 핸들 저장하기 	[2014.10.29 ]
	G_ConstantTable.h_LightDiff = tempEffect->GetParameterByName(0, "gLightDiff");
	if(G_ConstantTable.h_LightDiff == nullptr){ return S_FALSE;  assert(false);}

	G_ConstantTable.h_LightDir = tempEffect->GetParameterByName(0, "gLightDir");
	if(G_ConstantTable.h_LightDir == nullptr) { return S_FALSE;  assert(false);}

	G_ConstantTable.h_LightAmbi = tempEffect->GetParameterByName(0, "gLightAmbi");
	if(G_ConstantTable.h_LightAmbi == nullptr) { return S_FALSE;  assert(false);}

	G_ConstantTable.h_ViewPos = tempEffect->GetParameterByName(0, "gViewPos");
	if(G_ConstantTable.h_ViewPos == nullptr) { return S_FALSE;  assert(false);}

	G_ConstantTable.h_Mtrl_SpecularPower = tempEffect->GetParameterByName(0, "gMaterial_Specular_Power");
	if(G_ConstantTable.h_Mtrl_SpecularPower == nullptr) { return S_FALSE;  assert(false);}

	G_ConstantTable.h_ViewProjTM = tempEffect->GetParameterByName(0, "gViewProjTM"); 
	if(G_ConstantTable.h_ViewProjTM == nullptr) { return S_FALSE;  assert(false);}

	G_ConstantTable.h_WorldTM = tempEffect->GetParameterByName(0, "gWorldTM"); 
	if(G_ConstantTable.h_WorldTM == nullptr) { return S_FALSE;  assert(false);}

	G_ConstantTable.h_MatPallete = tempEffect->GetParameterByName(0, "gMatPal"); 
	if(G_ConstantTable.h_MatPallete == nullptr) { return S_FALSE;  assert(false);}

	G_ConstantTable.h_PointLightPos = tempEffect->GetParameterByName(0, "gLightPos"); // [2014.11.12 ]
	if(G_ConstantTable.h_PointLightPos == nullptr) assert(false);

	G_ConstantTable.h_PointLightRadius = tempEffect->GetParameterByName(0, "gLightRange"); // [2014.11.12 ]
	if(G_ConstantTable.h_PointLightRadius == nullptr) assert(false);

	G_ConstantTable.h_LineColor = tempEffect->GetParameterByName(0, "gLineColor"); // [2014.11.13 ]
	if(G_ConstantTable.h_PointLightRadius == nullptr) assert(false);

	G_ConstantTable.h_LineOffset = tempEffect->GetParameterByName(0, "gLineOffset"); // [2014.11.13 ]
	if(G_ConstantTable.h_LineOffset == nullptr) assert(false);

	G_ConstantTable.h_LimLightColor = tempEffect->GetParameterByName(0, "gLimLightColor"); //  [2014.12.1 ]
	if(G_ConstantTable.h_LimLightColor == nullptr) assert(false);

	G_ConstantTable.h_LimOffset = tempEffect->GetParameterByName(0, "gLimOffset"); //  [2014.12.1 ]
	if(G_ConstantTable.h_LimOffset == nullptr) assert(false);

	//////////////////////////////////////////////////////////////////////////
	// 여기서부터 텍스쳐 핸들
	G_ConstantTable.h_TexDiffMap = tempEffect->GetParameterByName(0, "gDiffuseMap"); 
	if(G_ConstantTable.h_MatPallete == nullptr){ return S_FALSE;  assert(false);}

	G_ConstantTable.h_TexSpecularMap = tempEffect->GetParameterByName(0, "gSpecularMap"); 
	if(G_ConstantTable.h_TexSpecularMap == nullptr) { return S_FALSE;  assert(false);}

	G_ConstantTable.h_TexOpacityMap = tempEffect->GetParameterByName(0, "gOpacityMap"); 
	if(G_ConstantTable.h_TexOpacityMap == nullptr) { return S_FALSE;  assert(false);}

	G_ConstantTable.h_TexNormMap = tempEffect->GetParameterByName(0, "gNormalMap"); 
	if(G_ConstantTable.h_TexNormMap == nullptr) { return S_FALSE;  assert(false);}

	G_ConstantTable.h_TexShadowMap = tempEffect->GetParameterByName(0, "gShadowMap"); 
	if(G_ConstantTable.h_TexShadowMap == nullptr) { return S_FALSE;  assert(false);}

	G_ConstantTable.h_TexTargetResult = tempEffect->GetParameterByName(0, "gTexTargetResult");  // [2014.11.8 ]
	if(G_ConstantTable.h_TexTargetResult == nullptr) { return S_FALSE;  assert(false);}

	G_ConstantTable.h_Tex_Pass0 = tempEffect->GetParameterByName(0, "gTexPass0");  // [2014.11.8 ]
	if(G_ConstantTable.h_Tex_Pass0 == nullptr) { return S_FALSE;  assert(false);}
	
	G_ConstantTable.h_Tex_Pass1 = tempEffect->GetParameterByName(0, "gTexPass1");  // [2014.11.8 ]
	if(G_ConstantTable.h_Tex_Pass1 == nullptr) { return S_FALSE;  assert(false);}

	G_ConstantTable.h_Tex_Pass2 = tempEffect->GetParameterByName(0, "gTexPass2");  // [2014.11.8 ]
	if(G_ConstantTable.h_Tex_Pass2 == nullptr) { return S_FALSE;  assert(false);}
	
	G_ConstantTable.h_Tex_Pass3 = tempEffect->GetParameterByName(0, "gTexPass3");  // [2014.11.8 ]
	if(G_ConstantTable.h_Tex_Pass3 == nullptr) { return S_FALSE;  assert(false);}
	
	G_ConstantTable.h_Tex_Pass4 = tempEffect->GetParameterByName(0, "gTexPass4");  // [2014.11.8 ]
	if(G_ConstantTable.h_Tex_Pass4 == nullptr) { return S_FALSE;  assert(false);}

	G_ConstantTable.h_Tex_Pass5 = tempEffect->GetParameterByName(0, "gTexPass5");  // [2014.11.8 ]
	if(G_ConstantTable.h_Tex_Pass5 == nullptr) { return S_FALSE;  assert(false);}

	G_ConstantTable.h_Tex_Pass6 = tempEffect->GetParameterByName(0, "gTexPass6");  // [2014.11.8 ]
	if(G_ConstantTable.h_Tex_Pass6 == nullptr) { return S_FALSE;  assert(false);}

	G_ConstantTable.h_UVAni_Val = tempEffect->GetParameterByName(0, "gUVAni_Val"); // [2014.12.3 ]
	if(G_ConstantTable.h_UVAni_Val == nullptr) { return S_FALSE;  assert(false);}
	

	//////////////////////////////////////////////////////////////////////////
	// 여기서부터 테크닉 핸들
	G_ConstantTable.h_Tech_Skin_DirLight = tempEffect->GetTechniqueByName("Skinning_DirLight");
	if(G_ConstantTable.h_Tech_Skin_DirLight == nullptr){ return S_FALSE; assert(false);}

	G_ConstantTable.h_Tech_Skin_SpecularMap_DirLight = tempEffect->GetTechniqueByName("Skinning_DirLight_SpecularMap");
	if(G_ConstantTable.h_Tech_Skin_SpecularMap_DirLight == nullptr){ return S_FALSE; assert(false);}

	G_ConstantTable.h_Tech_NoEffect = tempEffect->GetTechniqueByName("NoEffect");
	if(G_ConstantTable.h_Tech_NoEffect == nullptr){ return S_FALSE; assert(false);}
	
	return S_OK;
}




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
//
// 여기서부터 class CFX_SHADER 의 함수정의
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
// 생성자
//-----------------------------------------------------------------------------
CFX_SHADER::CFX_SHADER()
{
	m_pShader = nullptr;
	eResourceType = eRT_FX_SHADER;
}


//-----------------------------------------------------------------------------
// 소멸자
//-----------------------------------------------------------------------------
CFX_SHADER::~CFX_SHADER()
{
	if(m_pShader != nullptr)
	{
		m_pShader->Release();
		m_pShader = nullptr;
	}
}


//-----------------------------------------------------------------------------
// Arg    : const WCHAR* _pName (리소스 이름) , const WCHAR* _pPath(리소스 경로 + 파일명)  
// Return : S_OK(성공) ,  S_FALSE(셰이더 만들기 실패)
// Desc   : dx 이펙트프레임워크 객체를 생성한다.
//-----------------------------------------------------------------------------
int		CFX_SHADER::Create(const WCHAR* _pName , const WCHAR* _pPath  )
{
	HRESULT res = S_OK;

	if(_pPath == nullptr)
	{
		SHUtil::MsgBox("Fail : CFX_SHADER::Create() ,_pPath 가 nullptr입니다.");assert(false);
		return S_FALSE;
	}
	if(_pName == nullptr)
	{
		SHUtil::MsgBox("Fail : CFX_SHADER::Create() ,_pName 가 nullptr입니다.");assert(false);
		return S_FALSE;
	}
	// 경로 복사
	wcscpy_s(m_Path , _pPath);
	m_PathSize =sizeof(WCHAR) * wcslen(m_Path);
	// 고유이름 복사
	wcscpy_s(m_Name , _pName);
	m_NameSize = sizeof(WCHAR) *wcslen(m_Name);

	if(m_pd3dDevice == nullptr) // 디바이스장치 생성되어있는지 검사 [2014.10.25 ]
	{	SHUtil::MsgBox("Fail : CFX_SHADER::Create(), m_pd3dDevice == nullptr "); 
		assert(m_pd3dDevice != nullptr); return S_FALSE;
	}


	LPD3DXBUFFER pError = NULL;
	DWORD dwShaderFlags = 0;

	dwShaderFlags |= D3DXSHADER_DEBUG;

	res = D3DXCreateEffectFromFile(m_pd3dDevice, m_Path,	NULL, NULL, dwShaderFlags, NULL, &m_pShader, &pError);

	if((res != D3D_OK) && pError)
	{
		int size	= pError->GetBufferSize();
		void *ack	= pError->GetBufferPointer();

		if(ack)
		{
			char* str = new char[size];
			sprintf_s(str, sizeof(char) *size ,(const char*)ack); //[2014.11.19 ]
			SHUtil::MsgBox(str);
			delete [] str;
			assert(false);

			return S_FALSE;
		}
	}

	return S_OK;
}

/****************************************************** End *****************************************************/