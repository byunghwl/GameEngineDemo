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
//
//    c26~c256 = ��Ű�������� ����Ʈ����
//  Input Registers:
//
//    v0    = per-vertex position
//    v1    = per-vertex blendWeight
//    v2    = per-vertex belndIndices
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


//�ϴ� 3��° Weight�� ���Ѵ�.(weight�� 3���� �Ѿ���Ƿ�)
dp4	r1.z,	c0.zzzy,	v1;	

//blend�ε����� ��ġ�� ����س���.
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
mova	a0.x,   r5.z;   			//a0 : ��������� �迭�� �ε��������� a0.x �� ����� �� �ִ�.
mad	r2,	r1.z,	c[a0.x+26], r2;		//����ص״� 4��° weight
mad	r3, 	r1.z,	c[a0.x+27], r3;
mad	r4,     r1.z,	c[a0.x+28], r4;


//���������� ������ TM�� viewPorjTM�� �����ش�.
m4x3	r0.xyz,	   v0,	   r2;			// r2 : weight�������� blend�� TM
mov	r0.w,	   c0.y;                        // c0.y���� 1.0�� ����ִ�.
m4x4 	oPos,	   r0,	   c4;			// c4 : viewPorjTM


// Normal������Ѵ�. (r5 : transform�� normal , lighting��꿡 �ΰ� �ΰ� ���δ�.)

m3x3  r5.xyz,   v3,     r2;


//  �븻������ �Ѵ�.
nrm   r1,	r5; 

// Ambient��Diffuse Term������Ѵ�.
dp3   r3, 	r1,	c13;  //r3 : Dot (N, L)

max   r4,       r3,     c0.xxxx;//MAX(Dot (N, L), 0) (r4 : saturation�� Dot (N, L)) 

mov   r7,	c14; 		//Light Ambient

mad   oD0,	r4,     c12,   r7; // Light Ambient + MAX(Dot (N, L), 0)* Light_DiffColor => 

mov   oT0,	v4;













