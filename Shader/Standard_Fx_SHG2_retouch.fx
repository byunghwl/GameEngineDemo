//**************************************************************//
// BSH 2014.11.13 Version
//**************************************************************//

//--------------------------------------------------------------//
// Skinning_DirLight
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Global Variable

extern float4   gLightDiff ;
extern float4   gLightDir  ;
extern float4   gLightAmbi ;
extern float4   gViewPos   ;  
extern float    gMaterial_Specular_Power  ;
extern float	gUVAni_Val = 0.0f; // for UV Animation  2014.12.03 BSH

float4x4 gViewProjTM : ViewProjection;
float4x4 gWorldTM;
float4x4 gMatPal[50] ; //MATRIX pallete


texture    gDiffuseMap;
texture    gOpacityMap;
texture	   gSpecularMap;
texture	   gNormalMap;
texture    gShadowMap;

//For RimLight
float4 gLimLightColor;
float  gLimOffset;

//Color For Outline
float4     gLineColor;
float	   gLineOffset;

//Variable for PointLight
float4      gLightPos; 
float		gLightRange;

//Value for Fog
float4 		gFogValue;
float4 		gFogColor;

//PostEffect 
float      gBlurOffset;

// last Texture for Post Effect
texture    gTexTargetResult;
texture    gTexPass0; //For Default
texture    gTexPass1; //For Blur PostEffect
texture    gTexPass2;
texture    gTexPass3;
texture    gTexPass4;
texture    gTexPass5;
texture    gTexPass6;

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sampler


sampler DiffuseSampler = sampler_state
{
	texture = (gDiffuseMap);
	MinFilter = LINEAR;
	MagFilter = LINEAR;

	AddressU = WRAP;
	AddressV = WRAp;
};

sampler OpacitySampler = sampler_state
{
	texture = (gOpacityMap);
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;

	AddressU = WRAP;
	AddressV = WRAp;
};


sampler SpecularSampler = sampler_state
{
	texture = (gSpecularMap);
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

sampler NormalSampler = sampler_state
{
	texture = (gNormalMap);
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;

	AddressU = WRAP;
	AddressV = WRAp;
};



sampler ShadowSampler = sampler_state
{
	texture = (gShadowMap);
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;

	AddressU = WRAP;
	AddressV = WRAp;
};

sampler2D SamplerPass0 = sampler_state
{
	Texture = (gTexPass0);
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;

	AddressU = CLAMP;
	AddressV = CLAMP;
   
};

sampler2D SamplerPass1 = sampler_state
{
	Texture = (gTexPass1);
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;

	AddressU = CLAMP;
	AddressV = CLAMP;
   
};

sampler2D SamplerPass2 = sampler_state
{
	Texture = (gTexPass2);
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;

	AddressU = CLAMP;
	AddressV = CLAMP;
   
};
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
 //structs 

struct VS_IN_PNT 
{
   float3 Position    : POSITION0;
   float3 Norm        : NORMAL;
   float2 UV          : TEXCOORD0;
};


struct VS_IN_SKIN 
{
   float3 Position    : POSITION0;
   float4 BlendWeight : BLENDWEIGHT;
   float4 BlendIndex  : BLENDINDICES;
   float3 Norm        : NORMAL;
   float2 UV          : TEXCOORD0;
     
};

struct VS_OUT_PPL
{
   float4 Position : POSITION0;
   float3 LightDiff: COLOR0;
   float2 UV       : TEXCOORD0;
   float4 WorldPos : TEXCOORD6;
   float4 Norm     : TEXCOORD7; 
};


struct VS_OUT_PPL_FOG
{
   float4 Position : POSITION0;
   float3 LightDiff: COLOR0;
   float2 UV       : TEXCOORD0;
   float4 WorldPos : TEXCOORD6;
   float4 Norm     : TEXCOORD7; 
   float4 mFogValue : COLOR1;
};


struct PS_INPUT
{  
 
   float4 LightDiff: COLOR0;
   float2 UV       : TEXCOORD0;
   float4 WorldPos : TEXCOORD6;
   float4 Norm     : TEXCOORD7; 

};

struct PS_INPUT_FOG
{  
 
   float4 LightDiff: COLOR0;
   float2 UV       : TEXCOORD0;
   float4 WorldPos : TEXCOORD6;
   float4 Norm     : TEXCOORD7; 
   float4 mFogValue : COLOR1;

};

// for no Effect
struct VS_Pos_Uv 
{
   float4 mPosition: POSITION;
   float2 mUV : TEXCOORD0;
};

// for no Effect
struct PS_Uv 
{
   float2 mUV : TEXCOORD0;
};

// for no Effect withFog
struct VS_Pos_Uv_Fog 
{
   float4 mPosition: POSITION;
   float4 mFogValue : COLOR1;
   float2 mUV : TEXCOORD0;
};

// for no Effect with Fog
struct PS_Uv_Fog 
{
   float2 mUV : TEXCOORD0;
   float4 mFogValue : COLOR1;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
// main Functions 
// vs
//--------------------------------------------------------------------------------------------------------------------//
VS_OUT_PPL Skinning_DirLight_vs_main( VS_IN_SKIN Input )
{
   VS_OUT_PPL Output;
   
/////////////////////////////////////////////////////////////////////////////
//  this block is for skinning


   float weightW = 1.0f - ( Input.BlendWeight.x + Input.BlendWeight.y + Input.BlendWeight.z ) ; 
   
   float4x4 TM = gMatPal[Input.BlendIndex.x] * Input.BlendWeight.x; 
   TM += mul( gMatPal[Input.BlendIndex.y] , Input.BlendWeight.y);
   TM += mul( gMatPal[Input.BlendIndex.z] , Input.BlendWeight.z);
   TM += mul( gMatPal[Input.BlendIndex.w] , weightW );

   
////////////////////////////////////////////////////////////////////////////
  // float4 TempP = float4(Input.Position , 1.0f) ;
   Output.WorldPos = mul(float4(Input.Position , 1.0f) , TM);
   Output.Norm  = mul(float4(Input.Norm , 0.0f) , TM);
   // for ppl(pixel per lighting), pass caculated Normal to Pixel Shader 
   // normalize 
   Output.Norm = normalize(Output.Norm );
   
   // Calculate Light Diffuse
   
   float4 LightDir = gLightPos - Output.WorldPos; 
   float4 DotNL = saturate(dot(Output.Norm , LightDir ) );
   	
   Output.LightDiff = gLightDiff.xyz * DotNL.xyz + gLightAmbi.xyz; 

   Output.Position = mul( Output.WorldPos, gViewProjTM );

   Output.UV = Input.UV;
   
   return( Output );
   
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// main Functions 
// vs withFog
//--------------------------------------------------------------------------------------------------------------------//
VS_OUT_PPL_FOG Skinning_DirLight_FOG_vs_main( VS_IN_SKIN Input )
{
   VS_OUT_PPL_FOG Output;
   
/////////////////////////////////////////////////////////////////////////////
//  this block is for skinning


   float weightW = 1.0f - ( Input.BlendWeight.x + Input.BlendWeight.y + Input.BlendWeight.z ) ; 
   
   float4x4 TM = gMatPal[Input.BlendIndex.x] * Input.BlendWeight.x; 
   TM += mul( gMatPal[Input.BlendIndex.y] , Input.BlendWeight.y);
   TM += mul( gMatPal[Input.BlendIndex.z] , Input.BlendWeight.z);
   TM += mul( gMatPal[Input.BlendIndex.w] , weightW );

   
////////////////////////////////////////////////////////////////////////////
  // float4 TempP = float4(Input.Position , 1.0f) ;
   Output.WorldPos = mul(float4(Input.Position , 1.0f) , TM);
   Output.Norm  = mul(float4(Input.Norm , 0.0f) , TM);
   // for ppl(pixel per lighting), pass caculated Normal to Pixel Shader 
   // normalize 
   Output.Norm = normalize(Output.Norm );
   
   // Calculate Light Diffuse
   
   float4 LightDir = gLightPos - Output.WorldPos; 
   float4 DotNL = saturate(dot(Output.Norm , LightDir ) );
   	
   Output.LightDiff = gLightDiff.xyz * DotNL.xyz + gLightAmbi.xyz; 

   Output.Position = mul( Output.WorldPos, gViewProjTM );

   //float fDensity = gFogValue.x;
   float fDensity = gFogValue.z;
   float fFogEnd  = gFogValue.w;
   
   float fDist = Output.Position.z / fFogEnd * 4;
   float f = exp(-fDist * fDensity);
   Output.mFogValue = float4(0,0,0,0);
   Output.mFogValue.x = f;
   
   Output.UV = Input.UV;
   
   return( Output );
   
}


//--------------------------------------------------------------------------------------------------------------------//
// Vertex Shader Function  
// PointLight + Tex + ppl  [BSH 2014.11.12]
VS_OUT_PPL PointLight_Tex_ppl_vs_main(VS_IN_PNT input)
 {  
	VS_OUT_PPL output;  // "Multiplication will be done in the pre-shader - so no cost per vertex"

	float4 posWorld = mul(float4(input.Position, 1.0f), gWorldTM); 
	output.WorldPos = posWorld; 
	output.Norm = mul(float4(input.Norm , 0.0f), gWorldTM); 
	float4 DotNL = saturate(dot(output.Norm.xyz , gLightDir.xyz ) );
	output.LightDiff = gLightDiff.xyz * DotNL.xyz + gLightAmbi.xyz; 
	
	output.Position = mul(posWorld, gViewProjTM);
	output.UV = input.UV;     // Passing information on to do both specular AND diffuse calculation in pixel shader  
	

	return output; 
 }   
 
// vs
//--------------------------------------------------------------------------------------------------------------------// [**2014.11.11 BH**]  In pass0, extract the outline 
VS_OUT_PPL Outline_PNT_Contour_vs_main( VS_IN_PNT  Input ) 
{
   VS_OUT_PPL Output;
   //calculate nothing, just what we need is only Position and Texcoord;

   Output.WorldPos = mul(float4(Input.Position,1.0f) , gWorldTM); 
   Output.Norm  = mul(float4(Input.Norm , 0.0f) , gWorldTM);
   Output.Norm = normalize(Output.Norm );
    
   //Scale up in order make a contour line.
   float3 manuNorm = (float3)Output.Norm * gLineOffset;
  
   float4 manuPosition = float4( ((float3)Output.WorldPos + manuNorm) , 1.0f);
   Output.Position = mul( manuPosition , gViewProjTM );
   Output.LightDiff = gLightDiff.xyz + gLightAmbi.xyz; 
   Output.UV = Input.UV;
   
   return( Output );
   
}
 
// vs
//--------------------------------------------------------------------------------------------------------------------
// [**2014.11.24 BH**]  In pass0, extract the outline 
VS_OUT_PPL Outline_SPNT_Contour_vs_main( VS_IN_SKIN  Input ) 
{
   VS_OUT_PPL Output;
   //calculate nothing, just what we need is only Position and Texcoord;

   /////////////////////////////////////////////////////////////////////////////
//  this block is for skinning

   float weightW = 1.0f - ( Input.BlendWeight.x + Input.BlendWeight.y + Input.BlendWeight.z ) ; 
   
   float4x4 TM = gMatPal[Input.BlendIndex.x] * Input.BlendWeight.x; 
   TM += mul( gMatPal[Input.BlendIndex.y] , Input.BlendWeight.y);
   TM += mul( gMatPal[Input.BlendIndex.z] , Input.BlendWeight.z);
   TM += mul( gMatPal[Input.BlendIndex.w] , weightW );

   
////////////////////////////////////////////////////////////////////////////
  // float4 TempP = float4(Input.Position , 1.0f) ;
   Output.WorldPos = mul(float4(Input.Position , 1.0f) , TM);
   Output.Norm  = mul(float4(Input.Norm , 0.0f) , TM);
   // for ppl(pixel per lighting), pass caculated Normal to Pixel Shader 
   // normalize 
   Output.Norm = normalize(Output.Norm );
   
   
   float3 manuNorm = (float3)Output.Norm * gLineOffset;
  
   float4 manuPosition = float4( ((float3)Output.WorldPos + manuNorm) , 1.0f);
   Output.Position = mul( manuPosition , gViewProjTM );
 
   // Calculate Light Diffuse
   float4 DotNL = saturate(dot(Output.Norm.xyz , gLightDir.xyz ) );
   Output.LightDiff = gLightDiff.xyz * DotNL.xyz + gLightAmbi.xyz; 
   Output.UV = Input.UV;
   
   return( Output );   
}
// vs
//--------------------------------------------------------------------------------------------------------------------// [**2014.11.11 Bh**] default PNT PPL Shader

VS_OUT_PPL PNT_Default_vs_main( VS_IN_PNT  Input ) 
{
   VS_OUT_PPL Output;

   Output.WorldPos = mul(float4(Input.Position , 1.0f) , gWorldTM);
   Output.Norm  = mul(float4(Input.Norm , 0.0f) , gWorldTM);

   // for ppl(pixel per lighting), pass caculated Normal to Pixel Shader 
   // normalize 
   Output.Norm = normalize(Output.Norm );
   
   // Calculate Light Diffuse
   float4 DotNL = saturate(dot(Output.Norm.xyz , gLightDir.xyz ) );
   	
   Output.LightDiff = gLightDiff.xyz * DotNL.xyz + gLightAmbi.xyz; 

   Output.Position = mul( Output.WorldPos, gViewProjTM );

   Output.UV = Input.UV;
   
   return( Output );
}

//--------------------------------------------------------------------------------------------------------------------//
// ps
float4 Skinning_DirLight_ps_main(VS_OUT_PPL Input) : COLOR0
{   
   
   float3 TempNorm = normalize(Input.Norm.xyz);
   
   float3 EyeVector = normalize(gViewPos.xyz - Input.WorldPos.xyz);
   float3 LightDir = normalize(gLightDir.xyz);

   float3 HalfVector = LightDir + EyeVector ;

   HalfVector = normalize(HalfVector);
      

  float3 Specular ; 

  float Dot = dot(HalfVector.xyz , TempNorm.xyz);

  Specular =  pow(saturate(Dot), gMaterial_Specular_Power  );
 

   Specular *= gLightDiff.xyz;	
   return float4(Specular , 0.0f) + (float4(Input.LightDiff, 1.0f) * tex2D(DiffuseSampler , Input.UV));
   //return tex2D(DiffuseSampler , Input.UV);//float4(Specular , 0.0f);
}
//--------------------------------------------------------------------------------------------------------------------//
// ps [2014.11.13 Byung Hwan] PointLight
float4 Skinning_PointLight_ps_main(VS_OUT_PPL Input) : COLOR0
{   
   //float3      gLightPos; 
   //float		gLightRange;

   float3 TempNorm = normalize(Input.Norm.xyz);
   
   float3 EyeVector = normalize(gViewPos.xyz - Input.WorldPos.xyz);
   float3 LightDir = normalize( gLightPos.xyz - Input.WorldPos.xyz); 

   float dist = distance( gLightPos, Input.WorldPos);
   float attenu = 1.0f - saturate( dist / gLightRange);  // Basic attenuation
   
   if(dist >= (gLightRange + 10.0f ))
   {
	attenu  =  0.04f ;
   }
 
   float3 HalfVector = LightDir + EyeVector ;

   HalfVector = normalize(HalfVector);
      

  float3 Specular ; 

  float Dot = dot(HalfVector.xyz , TempNorm.xyz);

  Specular =  pow(saturate(Dot), 70.0f ) * attenu;
  float4 texColor = tex2D(DiffuseSampler , Input.UV);
 

   Specular *= gLightDiff.xyz;	

  return float4(Specular , 0.0f) + (float4(Input.LightDiff.xyz * attenu , 1.0f) *texColor );
//return tex2D(DiffuseSampler , Input.UV);//float4(Specular , 0.0f);
}



//--------------------------------------------------------------------------------------------------------------------//
// ps [**2014.11.11 bh**] cartoon Shader
float4 Cartoon_DirLight_ps_main(VS_OUT_PPL Input) : COLOR0
{   
   
   float3 TempNorm = normalize(Input.Norm.xyz);
   float4 DotNL = saturate(dot(Input.Norm.xyz , gLightDir.xyz ) );
   
   float3 EyeVector = normalize(gViewPos.xyz - Input.WorldPos.xyz);
   float3 LightDir = normalize(gLightDir.xyz);

   float3 HalfVector = LightDir + EyeVector ;

   HalfVector = normalize(HalfVector);
      

  float3 Specular ; 

  float Dot = dot(HalfVector.xyz , TempNorm.xyz) ;

  Specular =  pow(saturate(Dot), 60  );
 
   Specular *= gLightDiff.xyz;	

 //For cartoon Effect 
 float4 FinalDiffuse =  float4(DotNL.xyz, 1.0f) + gLightAmbi;
 float4 texturecolor =  tex2D(DiffuseSampler , Input.UV) ;

 FinalDiffuse  = ceil(FinalDiffuse  * 2) / 2.0f;
 float4 FinalOut = FinalDiffuse * texturecolor;
 
 return ( float4(Specular , 0.0f) +  float4(FinalOut.xyz , 1.0f) );

}
//--------------------------------------------------------------------------------------------------------------------//
// ps [**2014.11.26 bh**] cartoon Shader
float4 Cartoon_PointLight_ps_main(VS_OUT_PPL Input) : COLOR0
{   
   float3 TempNorm = normalize(Input.Norm.xyz);
   float3 EyeVector = normalize(gViewPos.xyz - Input.WorldPos.xyz);
   float3 LightDir = normalize( gLightPos.xyz - Input.WorldPos.xyz); 
   
   float4 DotNL = saturate(dot(Input.Norm.xyz , LightDir.xyz ) );
  
   float dist = distance( gLightPos, Input.WorldPos);
   float attenu = 1.0f - saturate( dist / gLightRange);  // Basic attenuation
  
   if(dist >= (gLightRange + 10.0f ))
   {
	attenu  =  0.04f ;
   }
  
   float3 HalfVector = LightDir + EyeVector ;
   HalfVector = normalize(HalfVector);
      

  float3 Specular ; 

  float Dot = dot(HalfVector.xyz , TempNorm.xyz) ;

  Specular =  pow(saturate(Dot), 60  );
 
   Specular *= gLightDiff.xyz;	

 //For cartoon Effect 
 float4 FinalDiffuse =  (DotNL + gLightAmbi) * attenu;
 FinalDiffuse = float4(FinalDiffuse.xyz, 1.0f);
 float4 texturecolor =  tex2D(DiffuseSampler , Input.UV) ;

 FinalDiffuse  = ceil(FinalDiffuse  * 2) / 2.0f;
 float4 FinalOut = FinalDiffuse * texturecolor;
 
 return ( float4(Specular , 0.0f) +  float4(FinalOut.xyz , 1.0f) );

}

//--------------------------------------------------------------------------------------------------------------------//
// ps [**2014.12.01 bh**] cartoon Shader No Specular
float4 Cartoon_PointLight_NOSPECULAR_ps_main(VS_OUT_PPL Input) : COLOR0
{   
   float3 TempNorm = normalize(Input.Norm.xyz);
   float3 EyeVector = normalize(gViewPos.xyz - Input.WorldPos.xyz);
   float3 LightDir = normalize( gLightPos.xyz - Input.WorldPos.xyz); 
   
   float4 DotNL = saturate(dot(Input.Norm.xyz , LightDir.xyz ) );
  
   float dist = distance( gLightPos, Input.WorldPos);
   float attenu = 1.0f - saturate( dist / gLightRange);  // Basic attenuation
  
   if(dist >= (gLightRange + 10.0f ))
   {
	attenu  =  0.04f ;
   }
  
   float3 HalfVector = LightDir + EyeVector ;
   HalfVector = normalize(HalfVector);
      

  float3 Specular ; 

  float Dot = dot(HalfVector.xyz , TempNorm.xyz) ;

  Specular =  pow(saturate(Dot), 60  );
 
   Specular *= gLightDiff.xyz;	

 //For cartoon Effect 
 float4 FinalDiffuse =  (DotNL + gLightAmbi) * attenu;
 FinalDiffuse = float4(FinalDiffuse.xyz, 1.0f);
 float4 texturecolor =  tex2D(DiffuseSampler , Input.UV) ;

 FinalDiffuse  = ceil(FinalDiffuse  * 2) / 2.0f;
 float4 FinalOut = FinalDiffuse * texturecolor;
 
 return (  float4(FinalOut.xyz , 1.0f) );

}

//--------------------------------------------------------------------------------------------------------------------//
// ps [**2014.12.01 bh**] cartoon Shader No Specular with LimLight
float4 Cartoon_PointLight_NOSPECULAR_withLimLight_ps_main(VS_OUT_PPL Input) : COLOR0
{   
   float3 TempNorm = normalize(Input.Norm.xyz);
   float3 EyeVector = normalize(gViewPos.xyz - Input.WorldPos.xyz);
   float3 LightDir = normalize( gLightPos.xyz - Input.WorldPos.xyz); 
   
   float4 DotNL = saturate(dot(Input.Norm.xyz , LightDir.xyz ) );
  
   float dist = distance( gLightPos, Input.WorldPos);
   float attenu = 1.0f - saturate( dist / gLightRange);  // Basic attenuation
  
   if(dist >= (gLightRange + 10.0f ))
   {
	attenu  =  0.04f ;
   }
  
   float3 HalfVector = LightDir + EyeVector ;
   HalfVector = normalize(HalfVector);
      
  float3 rimValue =1 - dot(EyeVector, Input.Norm.xyz);
  rimValue = pow( rimValue, gLimOffset);
  rimValue = rimValue * gLimLightColor;
  float Dot = dot(HalfVector.xyz , TempNorm.xyz) ;

  
 //For cartoon Effect 
 float4 FinalDiffuse =  (DotNL + gLightAmbi) * attenu;
 FinalDiffuse = float4(FinalDiffuse.xyz, 1.0f);
 float4 texturecolor =  tex2D(DiffuseSampler , Input.UV) ;

 FinalDiffuse  = ceil(FinalDiffuse  * 2) / 2.0f;
 float4 FinalOut = FinalDiffuse * texturecolor + float4(rimValue, 0.0f);
 
 return (  float4( FinalOut.xyz , 1.0f)  );

}
//--------------------------------------------------------------------------------------------------------------------//
// ps [**2014.12.04 bh**] cartoon Shader No Specular with LimLight + FOG
float4 Cartoon_PointLight_NOSPECULAR_withLimLight_FOG_ps_main(VS_OUT_PPL_FOG Input) : COLOR0
{   
   float3 TempNorm = normalize(Input.Norm.xyz);
   float3 EyeVector = normalize(gViewPos.xyz - Input.WorldPos.xyz);
   float3 LightDir = normalize( gLightPos.xyz - Input.WorldPos.xyz); 
   
   float4 DotNL = saturate(dot(Input.Norm.xyz , LightDir.xyz ) );
  
   float dist = distance( gLightPos, Input.WorldPos);
   float attenu = 1.0f - saturate( dist / gLightRange);  // Basic attenuation
  
   if(dist >= (gLightRange + 10.0f ))
   {
	attenu  =  0.04f ;
   }
  
   float3 HalfVector = LightDir + EyeVector ;
   HalfVector = normalize(HalfVector);
      
  float3 rimValue =1 - dot(EyeVector, Input.Norm.xyz);
  rimValue = pow( rimValue, gLimOffset);
  rimValue = rimValue * gLimLightColor;
  float Dot = dot(HalfVector.xyz , TempNorm.xyz) ;

  
  float fog = Input.mFogValue.x;
	
 //For cartoon Effect 
 float4 FinalDiffuse =  (DotNL + gLightAmbi) * attenu;
 FinalDiffuse = float4(FinalDiffuse.xyz, 1.0f);
 float4 texturecolor =  tex2D(DiffuseSampler , Input.UV) ;

 FinalDiffuse  = ceil(FinalDiffuse  * 2) / 2.0f;
 float4 FinalOut = FinalDiffuse * texturecolor + float4(rimValue, 0.0f);
 FinalOut =  float4( FinalOut.xyz , 1.0f);
 
 return lerp(gFogColor, FinalOut, fog);

}
//--------------------------------------------------------------------------------------------------------------------//
// ps
float4 Skinning_DirLight_SpecularMap_ps_main(VS_OUT_PPL Input) : COLOR0
{   
   
   float3 TempNorm = normalize(Input.Norm.xyz);
   
   float3 EyeVector = normalize(gViewPos.xyz - Input.WorldPos.xyz);
   float3 LightDir = normalize(gLightDir.xyz);

   float3 HalfVector = LightDir + EyeVector ;

   HalfVector = normalize(HalfVector);
      
  float3 Specular ; 

  float Dot = dot(HalfVector.xyz , TempNorm.xyz);

  Specular =  pow(saturate(Dot), gMaterial_Specular_Power  );
 
  float4 SpecIntensity = tex2D(SpecularSampler , Input.UV);

   Specular *= SpecIntensity.xyz* gLightDiff.xyz;	
   return float4(Specular , 0.0f) + (float4(Input.LightDiff, 1.0f) * tex2D(DiffuseSampler , Input.UV));
//return tex2D(DiffuseSampler , Input.UV);//float4(Specular , 0.0f);
//return float4(255 , 0,0,0);
}

//--------------------------------------------------------------------------------------------------------------------//
// no effect vs_Main 2014.11.20 BSH
// vs
VS_Pos_Uv NoEffect_vs_main(VS_Pos_Uv Input)
{
   VS_Pos_Uv Output;


   Output.mPosition = Input.mPosition;
   Output.mUV = Input.mUV;
   
   return Output;
}

//--------------------------------------------------------------------------------------------------------------------//
// no effect
// ps
float4 NoEffect_ps_main(PS_Uv Input) : COLOR0
{
	float4 tex = tex2D(SamplerPass0, Input.mUV);
	return tex;
//return float4(1.0f, 0,0,1.0f);
}
//--------------------------------------------------------------------------------------------------------------------//

// Post effect Blur vs_Main 2014.11.20 BSH
//--------------------------------------------------------------------------------------------------------------------//
// Blur effect
// ps
float4 BlurEffect_vertical_ps_main(PS_Uv Input) : COLOR0
{
	float4 tex = tex2D(SamplerPass1, Input.mUV);
	
	 float4 sum = float4(0,0,0,0);
   
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x, Input.mUV.y - 4.0*gBlurOffset)) * 0.05;
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x, Input.mUV.y - 3.0*gBlurOffset)) * 0.09;
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x, Input.mUV.y - 2.0*gBlurOffset)) * 0.12;
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x, Input.mUV.y - gBlurOffset)) * 0.15;
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x, Input.mUV.y)) * 0.16;
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x, Input.mUV.y + gBlurOffset)) * 0.15;
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x, Input.mUV.y + 2.0*gBlurOffset)) * 0.12;
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x, Input.mUV.y + 3.0*gBlurOffset)) * 0.09;
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x, Input.mUV.y + 4.0*gBlurOffset)) * 0.05;
	
	return float4(sum.xyz,1.0f);
}
//------------
float4 BlurEffect_Horizontal_ps_main(PS_Uv Input) : COLOR0
{
	 float4 sum = float4(0,0,0,0);
   
	   sum += tex2D(SamplerPass2, float2(Input.mUV.x - 4.0*gBlurOffset, Input.mUV.y)) * 0.05;
	   sum += tex2D(SamplerPass2, float2(Input.mUV.x - 3.0*gBlurOffset, Input.mUV.y)) * 0.09;
	   sum += tex2D(SamplerPass2, float2(Input.mUV.x - 2.0*gBlurOffset, Input.mUV.y)) * 0.12;
	   sum += tex2D(SamplerPass2, float2(Input.mUV.x - gBlurOffset, Input.mUV.y)) * 0.15;
	   sum += tex2D(SamplerPass2, float2(Input.mUV.x, Input.mUV.y)) * 0.16;
	   sum += tex2D(SamplerPass2, float2(Input.mUV.x + gBlurOffset, Input.mUV.y)) * 0.15;
	   sum += tex2D(SamplerPass2, float2(Input.mUV.x + 2.0*gBlurOffset, Input.mUV.y)) * 0.12;
	   sum += tex2D(SamplerPass2, float2(Input.mUV.x + 3.0*gBlurOffset, Input.mUV.y)) * 0.09;
	   sum += tex2D(SamplerPass2, float2(Input.mUV.x + 4.0*gBlurOffset, Input.mUV.y)) * 0.05;
  
  
	return float4(sum.xyz,1.0f);
}

float4 GlowEffect_ps_main(PS_Uv Input) : COLOR0
{
	 float4 sum = float4(0,0,0,0);
   
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x - 4.0*gBlurOffset, Input.mUV.y)) * 0.05;
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x - 3.0*gBlurOffset, Input.mUV.y)) * 0.09;
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x - 2.0*gBlurOffset, Input.mUV.y)) * 0.12;
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x - gBlurOffset, Input.mUV.y)) * 0.15;
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x, Input.mUV.y)) * 0.16;
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x + gBlurOffset, Input.mUV.y)) * 0.15;
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x + 2.0*gBlurOffset, Input.mUV.y)) * 0.12;
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x + 3.0*gBlurOffset, Input.mUV.y)) * 0.09;
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x + 4.0*gBlurOffset, Input.mUV.y)) * 0.05;
	   
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x, Input.mUV.y - 4.0*gBlurOffset)) * 0.05;
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x, Input.mUV.y - 3.0*gBlurOffset)) * 0.09;
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x, Input.mUV.y - 2.0*gBlurOffset)) * 0.12;
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x, Input.mUV.y - gBlurOffset)) * 0.15;
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x, Input.mUV.y)) * 0.16;
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x, Input.mUV.y + gBlurOffset)) * 0.15;
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x, Input.mUV.y + 2.0*gBlurOffset)) * 0.12;
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x, Input.mUV.y + 3.0*gBlurOffset)) * 0.09;
	   sum += tex2D(SamplerPass1, float2(Input.mUV.x, Input.mUV.y + 4.0*gBlurOffset)) * 0.05;
  
  
	return float4(sum.xyz,1.0f);
}
//--------------------------------------------------------------------------------------------------------------------//
// MergeLayer  SceneSurface + EntitySurface
// ps
 float4 MergeNoEffect_ps_main(PS_Uv Input) : COLOR0
{
	float3 SceneLayer  = tex2D(SamplerPass0, Input.mUV);
	float3 EntityLayer = tex2D(SamplerPass1, Input.mUV);
  
   float3 Final = SceneLayer + EntityLayer;
   return float4( Final, 1.0f);
}

//--------------------------------------------------------------------------------------------------------------------//
// MergeLayer  SceneSurface + EntitySurface
// ps
 float4 MergeBlur_ps_main(PS_Uv Input) : COLOR0
{
	float4 EntityLayer  = tex2D(SamplerPass1, Input.mUV);
	float4 BlurLayer    = tex2D(SamplerPass2, Input.mUV);
  
   float4 Final = BlurLayer  + EntityLayer  ;
   return float4( Final);
}

//--------------------------------------------------------------------------------------------------------------------//
// Contour Line
// ps
float4 Contour_ps_main() : COLOR0
{   
   return(  gLineColor );
   
}

//--------------------------------------------------------------------------------------------------------------------//
// no effect just  Texture to pixel shader  2014.11.20 BSH
// vs
VS_Pos_Uv Tex_vs_main(VS_IN_PNT Input)
{
   VS_Pos_Uv Output;

   Output.mPosition = mul(float4(Input.Position , 1.0f ) , gWorldTM) ;
   Output.mPosition = mul(Output.mPosition ,  gViewProjTM); 
   //Output.mPosition = Input.Position;
   Input.UV.x += gUVAni_Val;
   Output.mUV = Input.UV;
   
   return Output;
}

//--------------------------------------------------------------------------------------------------------------------//
// no effect   Just apply Texture using DiffuseMap
// ps
float4 Tex_ps_main(PS_Uv Input) : COLOR0
{
	float4 tex = tex2D(DiffuseSampler, Input.mUV);
	tex =  tex * 0.65f;
	return tex;
	//return float4(255,0,0,0);
}



//--------------------------------------------------------------------------------------------------------------------//
// no effect just  Texture to pixel shader  2014.11.20 BSH
// vs
VS_Pos_Uv_Fog Tex_vs_withFog_main(VS_IN_PNT Input)
{
   VS_Pos_Uv_Fog Output;

   Output.mPosition = mul(float4(Input.Position , 1.0f ) , gWorldTM) ;
   Output.mPosition = mul(Output.mPosition ,  gViewProjTM); 
   
   //float4 		gFogValue;
   //float4 		gFogColor;
   float fDensity = gFogValue.x;
   float fFogEnd  = gFogValue.y;
   
   float fDist = Output.mPosition.z / fFogEnd * 4;
   float f = exp(-fDist * fDensity);
   Output.mFogValue = float4(0,0,0,0);
   Output.mFogValue.x = f;
   
   //Output.mPosition = Input.Position;
   Input.UV.x += gUVAni_Val;
   Output.mUV = Input.UV;
   
   return Output;
}

//--------------------------------------------------------------------------------------------------------------------//
// no effect   Just apply Texture using DiffuseMap
// ps
float4 Tex_ps_withFog_main(PS_Uv_Fog Input) : COLOR0
{
	float4 tex = tex2D(DiffuseSampler, Input.mUV);
	tex =  tex * 0.65f;
	
	float fog = Input.mFogValue.x;
	
	
	return lerp(gFogColor, tex, fog);
	//return float4(255,0,0,0);
}
//--------------------------------------------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
// these are technique 


//--------------------------------------------------------------//
// Technique Section for Skinning_DirLight
//--------------------------------------------------------------//
technique Skinning_DirLight
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 Skinning_DirLight_vs_main();
      PixelShader = compile ps_2_0 Skinning_DirLight_ps_main();
   }
}

//--------------------------------------------------------------//
// Technique Section for PointLight_tex_ppl  [2014.11.13 BSH]
//--------------------------------------------------------------//
technique PointLight_tex_ppl
{
   pass Pass_0
   {
	  //CULLMODE = CW;
      VertexShader = compile vs_2_0 PointLight_Tex_ppl_vs_main();
      PixelShader = compile ps_2_0 Skinning_PointLight_ps_main();
   }
}



//--------------------------------------------------------------//
// Technique Section for Skinning_PointLight
//--------------------------------------------------------------//
technique Skinning_PointLight
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 Skinning_DirLight_vs_main();
      PixelShader = compile ps_2_0 Skinning_PointLight_ps_main();
   }
}

//--------------------------------------------------------------//
// Technique Section for Skinning_DirLight+SpecularMap
//--------------------------------------------------------------//
technique Skinning_DirLight_SpecularMap
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 Skinning_DirLight_vs_main();
      PixelShader = compile ps_2_0 Skinning_DirLight_SpecularMap_ps_main();
   }
}


//--------------------------------------------------------------//
// Technique Section for Outline Direction Light
//--------------------------------------------------------------//
technique Outline_PNT_DIR
{

  pass Pass_0
   {
      CULLMODE = CCW;

      VertexShader = compile vs_2_0 Outline_PNT_Contour_vs_main();
      PixelShader  = compile ps_2_0 Contour_ps_main();
   }
pass Pass_1
   {
      CULLMODE = NONE;

      VertexShader = compile vs_2_0 PNT_Default_vs_main();
      PixelShader = compile ps_2_0 Skinning_DirLight_ps_main();
   }
}
//--------------------------------------------------------------//
// Technique Section for Outline Direction Light
//--------------------------------------------------------------//
technique Outline_SPNT_DIR
{

  pass Pass_0
   {
      CULLMODE = CCW;

      VertexShader = compile vs_2_0 Outline_SPNT_Contour_vs_main();
      PixelShader  = compile ps_2_0 Contour_ps_main();
	  
   }
   
   pass Pass_1
   {
      CULLMODE = CW;

      VertexShader = compile vs_2_0 Skinning_DirLight_vs_main();
	  PixelShader = compile ps_2_0 Cartoon_DirLight_ps_main();
   }
}

//--------------------------------------------------------------//
// Technique Section for Outline PointLight
//--------------------------------------------------------------//
technique Outline_PNT_POINT
{

  pass Pass_0
   {
      CULLMODE = CCW;

      VertexShader = compile vs_2_0 Outline_PNT_Contour_vs_main();
      PixelShader  = compile ps_2_0 Contour_ps_main();
   }
pass Pass_1
   {
      CULLMODE = CW;

      VertexShader = compile vs_2_0 PNT_Default_vs_main();
      PixelShader = compile ps_2_0 Cartoon_PointLight_ps_main();
   }
}
//--------------------------------------------------------------//
// Technique Section for Outline PointLight
//--------------------------------------------------------------//
technique Outline_SPNT_POINT
{

  pass Pass_0
   {
      CULLMODE = CCW;

      VertexShader = compile vs_2_0 Outline_SPNT_Contour_vs_main();
      PixelShader  = compile ps_2_0 Contour_ps_main();
	  
   }
   
   pass Pass_1
   {
      CULLMODE = CW;

      VertexShader = compile vs_2_0 Skinning_DirLight_vs_main();
      PixelShader = compile ps_2_0 Cartoon_PointLight_ps_main();
	
   }
}

//--------------------------------------------------------------//
// Technique Section for Outline PointLight_withoutSpecular
//--------------------------------------------------------------//
technique Outline_PNT_POINT_NOSPECULAR
{

  pass Pass_0
   {
      CULLMODE = CCW;

      VertexShader = compile vs_2_0 Outline_PNT_Contour_vs_main();
      PixelShader  = compile ps_2_0 Contour_ps_main();
   }
pass Pass_1
   {
      CULLMODE = CW;

      VertexShader = compile vs_2_0 PNT_Default_vs_main();
      PixelShader = compile ps_2_0 Cartoon_PointLight_NOSPECULAR_ps_main();
   }
}
//--------------------------------------------------------------//
// Technique Section for Outline PointLight_withoutSpecular
//--------------------------------------------------------------//
technique Outline_SPNT_POINT_NOSPECULAR
{

  pass Pass_0
   {
      CULLMODE = CCW;

      VertexShader = compile vs_2_0 Outline_SPNT_Contour_vs_main();
      PixelShader  = compile ps_2_0 Contour_ps_main();
	  
   }
   
   pass Pass_1
   {
      CULLMODE = CW;

      VertexShader = compile vs_2_0 Skinning_DirLight_vs_main();
      PixelShader = compile ps_2_0 Cartoon_PointLight_NOSPECULAR_ps_main();
	
   }
}
//--------------------------------------------------------------//
// Technique Section for Outline PointLight_withoutSpecular+LimLight 
//--------------------------------------------------------------//
technique Outline_PNT_POINT_NOSPECULAR_withLim
{

  pass Pass_0
   {
      CULLMODE = CCW;

      VertexShader = compile vs_2_0 Outline_PNT_Contour_vs_main();
      PixelShader  = compile ps_2_0 Contour_ps_main();
   }
pass Pass_1
   {
      CULLMODE = CW;

      VertexShader = compile vs_2_0 PNT_Default_vs_main();
      PixelShader = compile ps_2_0 Cartoon_PointLight_NOSPECULAR_withLimLight_ps_main();
   }
}

//--------------------------------------------------------------//
// Technique Section for Outline PointLight_withoutSpecular+LimLight 
//--------------------------------------------------------------//
technique Outline_SPNT_POINT_NOSPECULAR_withLim
{

  pass Pass_0
   {
      CULLMODE = CCW;

      VertexShader = compile vs_2_0 Outline_SPNT_Contour_vs_main();
      PixelShader  = compile ps_2_0 Contour_ps_main();
	  
   }
   
   pass Pass_1
   {
      CULLMODE = CW;

      VertexShader = compile vs_2_0 Skinning_DirLight_vs_main();
      PixelShader = compile ps_2_0 Cartoon_PointLight_NOSPECULAR_withLimLight_ps_main();
	
   }
}

//--------------------------------------------------------------//
// Technique Section for Outline PointLight_withoutSpecular+LimLight+FOG 
//--------------------------------------------------------------//
technique SPNT_POINT_NOSPECULAR_withLim_FOG
{
   pass Pass_1
   {
      CULLMODE = CW;

      VertexShader = compile vs_2_0 Skinning_DirLight_FOG_vs_main();
      PixelShader  = compile ps_2_0 Cartoon_PointLight_NOSPECULAR_withLimLight_FOG_ps_main();
	
   }
}


//--------------------------------------------------------------//
// Technique Section for ColorConversion
//--------------------------------------------------------------//
technique Rigid_Tex
{
   pass Pass_0
   {
     // CULLMODE = NONE;

      VertexShader = compile vs_2_0 Tex_vs_main();
      PixelShader = compile ps_2_0 Tex_ps_main();
   }
}

//--------------------------------------------------------------//
// Technique Section for ColorConversion
//--------------------------------------------------------------//
technique Rigid_Tex_WithFOG
{
   pass Pass_0
   {
     // CULLMODE = NONE;

      VertexShader = compile vs_2_0 Tex_vs_withFog_main();
      PixelShader = compile ps_2_0 Tex_ps_withFog_main();
   }
}
//--------------------------------------------------------------//
// Technique Section for PostEffect None
//--------------------------------------------------------------//
technique NoEffect
{
   pass Pass_0
   {
      CULLMODE = NONE;
	
      VertexShader = compile vs_2_0 NoEffect_vs_main();
      PixelShader = compile ps_2_0 NoEffect_ps_main();
   }
}
//--------------------------------------------------------------//
// Technique Section for PostEffect Blur
//--------------------------------------------------------------//
technique BlurFx
{
   pass Pass_0
   {
   CULLMODE = NONE;
      VertexShader = compile vs_2_0 NoEffect_vs_main();
      PixelShader  = compile ps_2_0 BlurEffect_vertical_ps_main();
   }
   
    pass Pass_1
   {
   CULLMODE = NONE;
      VertexShader = compile vs_2_0 NoEffect_vs_main();
      PixelShader = compile ps_2_0 BlurEffect_Horizontal_ps_main();
   }
}

//--------------------------------------------------------------//
// Technique Section for MergeSurface MergeNoEffect
//--------------------------------------------------------------//
technique MergeNoEffect
{
   pass Pass_0
   {
	CULLMODE = NONE;
		
          VertexShader = compile vs_2_0 NoEffect_vs_main();
	  PixelShader = compile ps_2_0  MergeNoEffect_ps_main();
	 
   }
}
//--------------------------------------------------------------//
// Technique Section for MergeSurface MergeNoEffect
//--------------------------------------------------------------//
technique MergeBlur
{
   pass Pass_0
   {
	CULLMODE = NONE;
		
      	  VertexShader = compile vs_2_0 NoEffect_vs_main();
	  PixelShader = compile ps_2_0  MergeBlur_ps_main();
	  //PixelShader = compile ps_2_0  GlowEffect_ps_main();
   }
}