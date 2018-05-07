//**************************************************************//
//  Effect File exported by RenderMonkey 1.6
//
//  - Although many improvements were made to RenderMonkey FX  
//    file export, there are still situations that may cause   
//    compilation problems once the file is exported, such as  
//    occasional naming conflicts for methods, since FX format 
//    does not support any notions of name spaces. You need to 
//    try to create workspaces in such a way as to minimize    
//    potential naming conflicts on export.                    
//    
//  - Note that to minimize resulting name collisions in the FX 
//    file, RenderMonkey will mangle names for passes, shaders  
//    and function names as necessary to reduce name conflicts. 
//**************************************************************//

//--------------------------------------------------------------//
// Skinning_DirLight
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string Skinning_DirLight_Pass_0_Model : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Sphere.3ds";

const float4   gTempVector = float4( 0.0f , 1.0f, -1.0f, 3.0f ) ; 
extern float4   gLightDiff : register(c12);
extern float4   gLightDir  : register(c13);
extern float4   gLightAmbi : register(c14);
extern float4   gViewPos   : register(c15);   // tT| pos
extern float4   gMaterial_Specular_Power : register(c16);

float4x4 gViewProjTM : ViewProjection;
float4x4 gWorldTM : World;
float3x4 gMatPal[55] : register(c26) ; //MATRIX pallete


struct VS_INPUT 
{
   float3 Position    : POSITION0;
   float4 BlendWeight : BLENDWEIGHT;
   float4 BlendIndex  : BLENDINDICES;
   float3 Norm        : NORMAL;
   float2 UV          : TEXCOORD0;
   
   
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float3 LightDiff: COLOR0;
   
   float2 UV       : TEXCOORD0;
   float3 WorldPos : TEXCOORD6; // ppl D t wolrd???ìÀX
   float3 Norm     : TEXCOORD7; // ppl D t World???Normal
   
};



VS_OUTPUT Skinning_DirLight_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;
   float4 weightW = dot(gTempVector.zzzy, Input.BlendWeight);  

   float3x4 MatRes;
   float3x4 MatTemp;
   
   MatTemp = mul( gMatPal[Input.BlendIndex.x], int( Input.BlendWeight.x) );
   MatRes = MatTemp;

   MatTemp = mul( gMatPal[Input.BlendIndex.y], int( Input.BlendWeight.y) );
   MatRes += MatTemp;
   
   MatTemp = mul( gMatPal[Input.BlendIndex.z], int( Input.BlendWeight.z) );
   MatRes += MatTemp;
   
   MatTemp = mul( gMatPal[Input.BlendIndex.w], int(weightW.x) );
   MatRes += MatTemp;
   
////////////////////////////////////////////////////////////////////////////
   Output.Position.x = dot(float4(Input.Position, 1)  , float4(MatRes[0]));
   Output.Position.y = dot(float4(Input.Position, 1)  , float4(MatRes[1]));
   Output.Position.z = dot(float4(Input.Position, 1)  , float4(MatRes[2]));
   Output.Position.w = 1.0f;
   
   // ppl D t wolrd???ìÀX
   Output.WorldPos = Output.Position;
   
   // ppl D t wolrd???x?transform normal , lightingÄ°?P?P???)
   Output.Norm.x = dot(float4(Input.Position,0) , float4(MatRes[0]));
   Output.Norm.y = dot(float4(Input.Position,0) , float4(MatRes[1]));
   Output.Norm.z = dot(float4(Input.Position,0) , float4(MatRes[2]));
   
   // x?t?\?
   Output.Norm = normalize(Output.Norm);
   
   // =@ptT?? ? Ä°
   float3 DotNL = max(dot(Output.Norm ,gLightDir.xyz ), 0);
   Output.LightDiff = mul(gLightDiff.xyz , DotNL);
   Output.LightDiff += gLightAmbi;   
   Output.Position = mul( float4(Input.Position,1), gViewProjTM );
   
   Output.UV = Input.UV;
   
   
   
   return( Output );
   
}





struct PS_INPUT
{
   float4 p : POSITION0;
   float3 LightDiff: COLOR0;
   float2 UV       : TEXCOORD0;
   float3 WorldPos : TEXCOORD6; // ppl D t wolrd???ìÀX
   float3 Norm     : TEXCOORD7; // ppl D t World???Normal

};

sampler2D DiffuseSampler;
sampler2D SpecularSampler;

float4 Skinning_DirLight_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR0
{   
   
   float3 TempNorm = normalize(Input.Norm);
  
   float3 EyeVector = gViewPos - Input.WorldPos;
  
   float3 HalfVector = gLightDir + EyeVector;
   HalfVector = normalize(HalfVector);
      

   float3 Specular = pow(saturate(dot(HalfVector , TempNorm)) ,gMaterial_Specular_Power);
      
   Specular *= gLightDiff;
  
   float3 Ambient = gLightAmbi * tex2D(DiffuseSampler, Input.UV );
   
   
   //specular TermD Ä°X0
   
   

   return float4(Ambient + Input.LightDiff + Specular ,1);
   
}




//--------------------------------------------------------------//
// Technique Section for Skinning_DirLight
//--------------------------------------------------------------//
technique Skinning_DirLight
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 Skinning_DirLight_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 Skinning_DirLight_Pass_0_Pixel_Shader_ps_main();
   }

}

