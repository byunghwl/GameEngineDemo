vs_2_0

//------------------------------------------------------------------------------
// Vertex Shader test  ( 배성환 2014.7.29) 
// SKINNED_OBJECT::Draw() 에서 상수 설정
//
//  Constants Registers:
//
//    c0    =   tempVector(0.0f, 1.0f, -1.0f, 3.0f);  
//    c4-c7 =   view* proj;
//    c8-c11=   아무것도없음
//    c12   =   Light Diffuse
//    c13   =	Light Direction
//    c14   =   Light Ambient
//
//    c26~c256 = 스키닝을위한 본매트릭스
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


//일단 3번째 Weight를 구한다.(weight가 3개만 넘어오므로)
dp4	r1.z,	c0.zzzy,	v1;	

//blend인덱스의 위치를 계산해낸다.
mul 	r5, 	v2, 	c0.w;

//첫번째 바이트(r5.x)에 있는 Blend index에 해당하는 matirx를 구해서 곱한다.
mova	a0.x,   r5.x;   			//a0 : 어셈블리에서 배열의 인덱스값으로 a0.x 만 사용할 수 있다.
mul	r2,	v1.x,	c[a0.x+26];		
mul     r3, 	v1.x,	c[a0.x+27];
mul	r4,     v1.x,	c[a0.x+28];


//두번째 바이트(r5.y)에 있는 Blend index에 해당하는 matirx를 구해서 곱한다.
mova	a0.x,   r5.y;   			//a0 : 어셈블리에서 배열의 인덱스값으로 a0.x 만 사용할 수 있다.
mad	r2,	v1.y,	c[a0.x+26], r2;		
mad	r3, 	v1.y,	c[a0.x+27], r3;
mad	r4,     v1.y,	c[a0.x+28], r4;


//세번째 바이트(r5.z)에 있는 Blend index에 해당하는 matirx를 구해서 곱한다.
mova	a0.x,   r5.z;   			//a0 : 어셈블리에서 배열의 인덱스값으로 a0.x 만 사용할 수 있다.
mad	r2,	v1.z,	c[a0.x+26], r2;		
mad	r3, 	v1.z,	c[a0.x+27], r3;
mad	r4,     v1.z,	c[a0.x+28], r4;

//네번째 바이트(r5.w)에 있는 Blend index에 해당하는 matirx를 구해서 곱한다.
mova	a0.x,   r5.z;   			//a0 : 어셈블리에서 배열의 인덱스값으로 a0.x 만 사용할 수 있다.
mad	r2,	r1.z,	c[a0.x+26], r2;		//계산해뒀던 4번째 weight
mad	r3, 	r1.z,	c[a0.x+27], r3;
mad	r4,     r1.z,	c[a0.x+28], r4;


//최종적으로 구해진 TM과 viewPorjTM을 곱해준다.
m4x3	r0.xyz,	   v0,	   r2;			// r2 : weight값에의해 blend된 TM
mov	r0.w,	   c0.y;                        // c0.y에는 1.0이 들어있다.
m4x4 	oPos,	   r0,	   c4;			// c4 : viewPorjTM


// Normal을계산한다. (r5 : transform된 normal , lighting계산에 두고 두고 쓰인다.)

m3x3  r5.xyz,   v3,     r2;


//  노말라이즈 한다.
nrm   r1,	r5; 

// Ambient와Diffuse Term을계산한다.
dp3   r3, 	r1,	c13;  //r3 : Dot (N, L)

max   r4,       r3,     c0.xxxx;//MAX(Dot (N, L), 0) (r4 : saturation된 Dot (N, L)) 

mov   r7,	c14; 		//Light Ambient

mad   oD0,	r4,     c12,   r7; // Light Ambient + MAX(Dot (N, L), 0)* Light_DiffColor => 

mov   oT0,	v4;













