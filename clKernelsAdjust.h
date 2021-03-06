
// NOTE: 05/12/13 Changed the sin(gamma) terms sign according to Erni - Optimization of Exit Waves...

const char* ThirdOrderCorrectionsource = 
"__kernel void clThirdOrderCorrection(__global float2* clRecon, __global float2* clCorrected, __global const float* CLxFrequencies, __global const float* CLyFrequencies, int sizeX, int sizeY, float wavel, float A1r, float A1i, float A2r, float A2i, float A3r, float A3i, float B2r, float B2i, float S3r, float S3i, float C1, float C3, float objap)	\n"
"{	\n"
"	//Get the work items ID \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	\n"
"	if(xid<sizeX&&yid<sizeY) \n"
"	{	\n"
"		int Index = xid + yid*sizeX; \n"
"		float frequency = sqrt(CLxFrequencies[xid]*CLxFrequencies[xid] + CLyFrequencies[yid]*CLyFrequencies[yid]);	\n"
"		float kx = CLxFrequencies[xid];	\n"
"		float ky = CLyFrequencies[yid];	\n"
"		float Pi = 3.141592f; \n"
"		float gamma = Pi*wavel*A1r*kx*kx + Pi*wavel*C1*kx*kx +2/3*Pi*wavel*wavel*A2r*kx*kx*kx + 2*Pi*wavel*wavel*B2r*kx*kx*kx + \n"
"		1/2*Pi*wavel*wavel*wavel*A3r*kx*kx*kx*kx + 2*Pi*wavel*wavel*wavel*S3r*kx*kx*kx*kx +1/2*Pi*wavel*wavel*wavel*C3*kx*kx*kx*kx \n"
"		+ 2*Pi*wavel*A1i*kx*ky + \n"
"		2*Pi*wavel*wavel*A2i*kx*kx*ky - 2 * Pi * wavel*wavel * B2i * kx*kx*ky + 2*Pi*wavel*wavel*wavel*A3i*kx*kx*kx*ky - 4*Pi*wavel*wavel*wavel*S3i*kx*kx*kx*ky \n"
"		- Pi * wavel * A1r*ky*ky + Pi*wavel*C1*ky*ky - 2 * Pi * wavel*wavel * A2r* kx * ky*ky + 2 * Pi * wavel*wavel * B2r*kx*ky*ky - 3 * Pi * wavel*wavel*wavel * A3r *kx*kx * ky*ky \n"
"		+ Pi * wavel*wavel*wavel*C3 * kx*kx * ky*ky \n"
"		- 2/3 * Pi * wavel*wavel*A2i * ky*ky*ky - 2 * Pi * wavel*wavel * B2i * ky*ky*ky - 2* Pi * wavel*wavel*wavel * A3i * kx * ky*ky*ky \n"
"		- 4 * Pi * wavel*wavel*wavel * S3i*kx*ky*ky*ky \n"
"		+1/2 * Pi * wavel*wavel*wavel * A3r*ky*ky*ky*ky - 2 * Pi * wavel*wavel*wavel *S3r * ky*ky*ky*ky + 1/2 * Pi * wavel*wavel*wavel*C3* ky*ky*ky*ky; \n"
"		clCorrected[Index].x = (frequency<=objap) *( cos(gamma)*clRecon[Index].x +sin(gamma)*clRecon[Index].y );	\n"
"		clCorrected[Index].y = (frequency<=objap) *( -clRecon[Index].x * sin(gamma) + clRecon[Index].y * cos(gamma) );	\n"
"	}	\n"
"}	\n"
;
