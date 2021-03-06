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
string Skinning_DirLight_Pass_0_Model : ModelData = "C:\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Sphere.3ds";

float4   gTempVector = float4( 0.0f , 1.0f, -1.0f, 3.0f); // �� ��
float4   gLightDiff ;
float4   gLightDir  ;
float4   gLightAmbi ;
float4   gViewPos;   // tT| pos
float4   gMaterial_Specular_Power ;


float4x4 gViewProjTM;
float4x4 gWorldTM;
float3x4 gMatPal[55] ; //MATRIX pallete


struct VS_INPUT 
{
   float3 Position    : POSITION0;
   float3 BlendWeight : BLENDWEIGHT;
   float4 BlendIndex  : BLENDINDICES;
   float3 Norm        : NORMAL;
   float2 UV          : TEXCOORD0;
   
   
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float3 LightDiff: COLOR0;
   
   float2 UV       : TEXCOORD0;
   float3 WorldPos : TEXCOORD6; // ppl D t wolrd� �t� ��X
   float3 Norm     : TEXCOORD7; // ppl D t World� �t� Normal
   
};



VS_OUTPUT Skinning_DirLight_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;
///////////////////////////////////////////////////////////////////////////
// �0 �0 ��� İ
   // |� 4�� Weight| l\�.(weight  3� �$�\)
   float weightW = dot(TempVector.zzzy, Input.BlendWeight);  
   
   // blendxq�X X| İt��.
   // float4 matIdx = mul(Input.BlendIndex , gTempVector.w);
   
   // �� � T��� 3x4  �
   float3x4 MatRes;
   float3x4 MatTemp;
   
   // ��� xq��  t�X� matirx| lt �t� D �\�.
   MatTemp = mul( gMatPal[Input.BlendIndex.x+26], int( Input.BlendWeight.x) );
   MatRes = MatTemp;
   
   // P�� xq��  t�X� matirx| lt �t� D �\�.
   MatTemp = mul( gMatPal[Input.BlendIndex.y+26], int( Input.BlendWeight.y) );
   MatRes += MatTemp;
   
   // 8�� xq��  t�X� matirx| lt �t� D �\�.
   MatTemp = mul( gMatPal[Input.BlendIndex.z+26], int( Input.BlendWeight.z) );
   MatRes += MatTemp;
   
   //$�� xq��  t�X� matirx| lt lt�X 4�� �t�| �\�.
   MatTemp = mul( gMatPal[Input.BlendIndex.w+26], weightW );
   MatRes += MatTemp;
   
////////////////////////////////////////////////////////////////////////////

   // ��  ���� �4..0| \�) �Xt ���� H��� �,D �X� �� @ ��t�.
   Output.Position.x = dot(float4(Input.Position, 1)  , float4(MatRes._1));
   Output.Position.y = dot(float4(Input.Position, 1)  , float4(MatRes._2));
   Output.Position.z = dot(float4(Input.Position, 1)  , float4(MatRes._3));
   Output.Position.w = 1.0f;
   
   // ppl D t wolrd� �t� ��X
   Output.WorldPos = Output.Position;
   
   // ppl D t wolrd� �t� x� transform normal , lightingİ� P� P� �x�.)
   Output.Norm.x = dot(float4(Input.Position,0) , float4(MatRes._1));
   Output.Norm.y = dot(float4(Input.Position,0) , float4(MatRes._2));
   Output.Norm.z = dot(float4(Input.Position,0) , float4(MatRes._3));
   
   // x�|t� \�.
   Output.Norm = normalize(Output.Norm);
   
   // =@ptT� � � İ
   float3 DotNL = max(dot(Output.Norm ,gLightDir.xyz ), 0); // [t �� �
   Output.LightDiff = mul(gLightDiff.xyz , DotNL);
   Output.LightDiff += gLightAmbi;   //X�(ambient) Tt�0 
   //t� �� ���  �t� ���0 �8� �\X �,� �Xt �.
   Output.Position = mul( Input.Position, matViewProjection );
   
   Output.UV = Input.UV;
   
   
   
   return( Output );
   
}





struct PS_INPUT
{
   float3 LightDiff: COLOR0;
   float2 UV       : TEXCOORD0;
   float3 WorldPos : TEXCOORD6; // ppl D t wolrd� �t� ��X
   float3 Norm     : TEXCOORD7; // ppl D t World� �t� Normal

};

sampler2D DiffuseSampler;
sampler2D SpecularSampler;

float4 Skinning_DirLight_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR0
{   
   //�( �D x�|t� t�0
   float3 TempNorm = normalize(Input.Norm);
   // � )� �0lX0
   float3 EyeVector = gViewPos - Input.WorldPos;
   // HalfVector lX0
   float3 HalfVector = gLightDir + EyeVector;
   HalfVector = normalize(HalfVector);
      
   //� lX0
   float3 Specular = pow(saturate(dot(HalfVector , TempNorm)) ,gMaterial_Specular_Power);
      
   Specular *= gLightDiff;
  
   float3 Ambient = gLightAmbi * tex2D(DiffuseSampler, Input.UV );
   
   
   //specular TermD İX0
   
   

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

