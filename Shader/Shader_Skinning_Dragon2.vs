vs_2_0

//------------------------------------------------------------------------------
// Vertex Shader test  ( �輺ȯ 2014.7.29) 
// SKINNED_OBJECT::Draw() ���� ��� ����
//
//  Constants Registers:
//
//    c0    =   tempVector(0.0f, 1.0f, -1.0f, 3.0f);  
//    c4-c7 =   view* proj;
//    c8-c11=   �ƹ��͵�����
//    c12   =   Light Diffuse
//    c13   =	Light Direction
//    c14   =   Light Ambient
//    c15   =
//    c16   =
//    c17   = 
//    c18   = 
//    c19   =
//    c20   =   EyePosition
//    c21   =   Material Diffuse
//    c22   =   Material Specular   c22.w���� specular power�� �־��ش�.
//    c23   =   Material Ambient
//    c24   =   Material Emissive
//
//    c26  ~ c246 = ��Ű�������� ����Ʈ���� ���밳�� �ִ� 55�� 
//    c247 ~ c250 = 
//    c251 ~ c254 = 
//    c255  =
//    c256  =
//  Input Registers:
//
//    v0    = per-vertex position
//    v1    = per-vertex blendWeight
//    v2    = per-vertex blendIndices
//    v3    = per-vertex Normal
//    v4    = per-vertex Tex
//
//  Output Registers:
//
//    oPos  = homogeneous position
//    oD0   = diffuse color
//
//------------------------------------------------------------------------------
dcl_position      v0
dcl_blendweight   v1
dcl_blendindices  v2
dcl_normal        v3
dcl_texcoord0     v4


//�ϴ� 4��° Weight�� ���Ѵ�.(weight�� 3���� �Ѿ���Ƿ�)
dp4	r1.z,	c0.zzzy,	v1;	

//blend�ε����� ��ġ�� ����س���.(���̷�Ʈ�׽����� float4�� 3�����Ѱ��ִϱ� �ε����� 3�� ����)
mul 	r5, 	v2, 	c0.w;

//ù��° ����Ʈ(r5.x)�� �ִ� Blend index�� �ش��ϴ� matirx�� ���ؼ� ���Ѵ�.
mova	a0.x,   r5.x;   			//a0 : ��������� �迭�� �ε��������� a0.x �� ����� �� �ִ�.
mul	r2,	v1.x,	c[a0.x+26];		
mul     r3, 	v1.x,	c[a0.x+27];
mul	r4,     v1.x,	c[a0.x+28];


//�ι�° ����Ʈ(r5.y)�� �ִ� Blend index�� �ش��ϴ� matirx�� ���ؼ� ���Ѵ�.
mova	a0.x,   r5.y;   			//a0 : ��������� �迭�� �ε��������� a0.x �� ����� �� �ִ�.
mad	r2,	v1.y,	c[a0.x+26], r2;		
mad	r3, 	v1.y,	c[a0.x+27], r3;
mad	r4,     v1.y,	c[a0.x+28], r4;


//����° ����Ʈ(r5.z)�� �ִ� Blend index�� �ش��ϴ� matirx�� ���ؼ� ���Ѵ�.
mova	a0.x,   r5.z;   			//a0 : ��������� �迭�� �ε��������� a0.x �� ����� �� �ִ�.
mad	r2,	v1.z,	c[a0.x+26], r2;		
mad	r3, 	v1.z,	c[a0.x+27], r3;
mad	r4,     v1.z,	c[a0.x+28], r4;

//�׹�° ����Ʈ(r5.w)�� �ִ� Blend index�� �ش��ϴ� matirx�� ���ؼ� ���Ѵ�.
mova	a0.x,   r5.w;   			//a0 : ��������� �迭�� �ε��������� a0.x �� ����� �� �ִ�.
mad	r2,	r1.z,	c[a0.x+26], r2;		//����ص״� 4��° weight
mad	r3, 	r1.z,	c[a0.x+27], r3;
mad	r4,     r1.z,	c[a0.x+28], r4;


//���������� ������ TM�� viewPorjTM�� �����ش�.
m4x3	r0.xyz,	   v0,	   r2;			// r2 : weight�������� blend�� TM
mov	r0.w,	   c0.y;                        // c0.y���� 1.0�� ����ִ�.
m4x4 	oPos,	   r0,	   c4;			// c4 : viewPorjTM

//PPL�� ���� ���� �������� T6�� �Ѱ��ش�.
mov   oT6,    r0;

// Normal������Ѵ�. (r5 : transform�� normal , lighting��꿡 �ΰ� �ΰ� ���δ�.)
m3x3  r5.xyz,   v3,     r2;

//ppl�� ���� �븻�� �Ѱ��ش�. ������ �ȼ����̴����� �븻������������ϱ⶧���� ���⼭ �븻������ ����..
mov   oT7,     r5;

//  �븻������ �Ѵ�.
nrm   r1,	r5; 

// Ambient��Diffuse Term������Ѵ�.
dp3   r3, 	r1,	c13;  //r3 : Dot (N, L)

max   r4,       r3,     c0.xxxx;//MAX(Dot (N, L), 0) (r4 : saturation�� Dot (N, L)) 

mul   r7,	r4,      c12; 	//Light diffuse x r4

mul   r7,       r7,      c21;   //MAX(Dot (N, L), 0) x light_diffuse x Material_diffuse

mov   r6,       c23;		//r6 : material_Ambient

mad   r7,	r6,      c14,   r7;  // material_Ambient * Light_Ambient + MAX(Dot (N, L), 0) x light_diffuse x Material_diffuse

add   oD0,	r7,     c24; //material_Ambient * Light_Ambient + MAX(Dot (N, L), 0) x light_diffuse x Material_diffuse + Material_Emissive

// EyeDirection�� ���ϱ�
//add   r2,	-r0, 	c20;  // EyePos - vtxPos
//nrm   r6,       r2;	      //Normalize( EyePos - vtxPos ) = EyeVec

// specular Term�� ����ϱ�
//add   r5,       r6,     c13;  //EyeVec + LightVec
//nrm   r2,       r5;           //Normalize(EyeVec + LightVec ) = HarfVec
//dp3   r4.y,     r1,     r2;   //Dot(N,H) = r4.y
//mov   r4.w,     c22.w;        //c22.w : specular power
//lit   r4,       r4;           //lit���� ���� ����Ѵ�.
//mul   r8.xyz,   r4.z,   c12.xyz;  // cos(@)^power * Light_Diffuse
//mul   oD1,      r8,     c22;    //cos(@)^power * Light_Diffuse * Material_Specular;

mov   oT0,	v4;

     











