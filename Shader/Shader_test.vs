vs_2_0

//------------------------------------------------------------------------------
// Vertex Shader test  ( 배성환 2014.7.27) 
// GEOMOBJECT::Draw() 에서 상수 설정
//
//  Constants Registers:
//
//    c0    =   tempVector(0.0f, 1.0f, 2.0f, 10.0f)
//    c4-c7 =   m_WorldTM ;
//    c8-c11=   m_WorldTM *viewTM * ProjTM;
//    c12   =   Light Diffuse
//    c13   =	Light Direction
//    c14   =   Light Ambient
//
//    c20   =   EyePosition 
//    c21   =   Material Diffuse
//    c22   =   Material Specular
//    c23   =   Material Ambient
//    c24   =   Material Emissive
//
//    c26~c256 = 스키닝을위한 본매트릭스
//  Input Registers:
//
//    v0    = per-vertex position
//    v1    = per-vertex Normal
//    v2    = per-vertex Tex
//
//  Output Registers:
//
//    oPos  = homogeneous position
//    oD0   = diffuse color
//
//------------------------------------------------------------------------------
dcl_position    v0
dcl_normal      v1
dcl_texcoord0   v2

// 1) Position을World/view/projection matrix에의해변환한다.

mul     r0,     v0,    c4;         //v0 * WorldTM = r0
m4x4    oPos,	v0, 	c8;


// 2) Normal을계산한다.(Diffuse와Ambient를이용함.)

m3x3  r2.xyz,   v1,     c8;
//노말라이즈 한다.
nrm   r1,	r2; 

// 3) Ambient와Diffuse Term을계산한다.
dp3   r3, 	r1,	c13; //Dot(light_Direction, Normal)

max   r4,       r3,     c0.xxxx;// saturation(Dot(light_Direction, Normal))

mul   r7,	r4,      c12; 	//Light diffuse x r4

mov   r6,       c23;		//r6 : material_Ambient

mad   r7,	r6,     c14,   r7;  // material_Ambient * Light_Ambient + MAX(Dot (N, L), 0) x light_diffuse x Material_diffuse

add   oD0,	r7,     c24;   // material_Ambient * Light_Ambient + MAX(Dot (N, L), 0) x light_diffuse x Material_diffuse +M_Emissive




// EyeDirection을 구하기
add   r2,	-r0, 	c20;  // EyePos - vtxPos
nrm   r6,       r2;	      // Normalize( EyePos - vtxPos ) = EyeVec : r6

// specular Term을 계산하기
add   r5,       r6,     c13;  //EyeVec + LightVec
nrm   r2,       r5;           //Normalize(EyeVec + LightVec ) = HarfVec
dp3   r4.y,     r1,     r2;   //Dot(N,H) = r4.y
mov   r4.w,     c22.w;        // c22.w : specular power
lit   r4,       r4;           //lit값을 최종 계산한다.
mul   r8.xyz,   r4.z,   c12.xyz;  // cos(@)^power * Light_Diffuse
mul   oD1,      r8,     c22;    //cos(@)^power * Light_Diffuse * Material_Specular;


//environment mapping용 vector계산
dp3 	r7,       r6,       r1;   //Dot(Norm, EyeVec) = r7
add     r7.w,     r7.w,     r7.w; // 2*Dot(Norm, EyeVec) =r7.w;
mad     oT0.xyz,  r1,       r7.w,   -r6;   
//mov    oT0,     v2;
