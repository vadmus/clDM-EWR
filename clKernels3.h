const char* pcpcfsource2 = 
"__kernel void clPCPCF(__global const float2* fft1, __global const float2* fft2, __global float2* fftresult, __global const float* CLxFrequencies, __global const float* CLyFrequencies, int sizeX, int sizeY, float focalstep, float wavelength, float pcpcfkmax) \n"
"{		\n"
"	//Get the work items ID \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	if(xid<sizeX&&yid<sizeY) \n"
"	{	\n"
"		int Index = xid + yid*sizeX; \n"
"		float frequency = sqrt(CLxFrequencies[xid]*CLxFrequencies[xid] + CLyFrequencies[yid]*CLyFrequencies[yid]); \n"
"		float compensation = cos(3.1415926534f * focalstep * wavelength * frequency * frequency); \n"
"		float c1r = fft1[Index].x; \n"
"		float c1i = fft1[Index].y; \n"
"		float c2r = fft2[Index].x; \n"
"		float c2i = fft2[Index].y; \n"
"		float denom2 = sqrt( compensation*compensation   * (c1r*c1r*c2r*c2r + c1i*c1i*c2i*c2i + c1r*c1r*c2i*c2i + c2r*c2r*c1i*c1i) \n" 
"		+ 0.001 * 0.001 + 2 * 0.001 * compensation * (c1r*c2r + c1i*c2i)); \n"
"		fftresult[Index].x = (frequency<=pcpcfkmax)*compensation*(c1r*c2r + c1i*c2i)/denom2; \n"
"		fftresult[Index].y = (frequency<=pcpcfkmax)*compensation*(c2i*c1r - c1i*c2r)/denom2; \n"
"	}	\n"
"}		\n"
;

const char* PadCropsource2 = 
"__kernel void clPadCrop(__global const float2* Input, __global float2* Output, int fullwidth, int fullheight, int padLeft, int padRight, int padTop, int padBottom, float xShift, float yShift, int newwidth, int newheight, int iTop, int iLeft) \n"
"{ \n"
"	//Get the work items ID \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	if(xid < newwidth && yid < newheight) \n"
"	{	\n"
"		int Index = xid + yid*newwidth; \n"
"		int xs2 = floor(xShift); \n"
"		int ys2 = floor(yShift); \n"
"		float subx = xShift - xs2; \n"
"		float suby = yShift - ys2; \n"
"		if((xid < padLeft || xid >= (newwidth - padRight)) || (yid < padTop || yid >= (newheight - padBottom))) \n"
"		{ \n"
"			Output[Index].x = 0; \n"
"			Output[Index].y = 0; \n"
"		} else { \n"
"			float v1 = (1-subx)*(1-suby)*Input[iLeft + xs2 + xid + (iTop+ys2+yid)*fullwidth].x; \n"
"			float v2 = (subx)*(1-suby)*Input[iLeft + xs2 + xid + 1 + (iTop+ys2+yid)*fullwidth].x; \n"
"			float v3 = (1-subx)*(suby)*Input[iLeft + xs2 + xid + (iTop+ys2+yid + 1)*fullwidth].x; \n"
"			float v4 = (subx)*(suby)*Input[iLeft + xs2 + xid + 1 +(iTop+ys2+yid + 1)*fullwidth].x; \n"
"			Output[Index].x = v1 + v2 + v3 + v4; \n"
"			Output[Index].y = 0.0f; \n"
"		} \n"
"	}	\n"
"}	\n"
;

const char* wavetransferfunctionsource2 = 
"__kernel void clWaveTransferFunction(__global float2* clw, __global const float* CLxFrequencies, __global const float* CLyFrequencies, int sizeX, int sizeY, float wavelength, float beta, float delta, float A1r, float A1i, float C1, float C3, float objap)	\n"
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
"		float SpatCohPart = fabs( wavelength * 2 * 3.141592654f * C1 * frequency + 2 * 3.141592654f * wavelength * wavelength * wavelength * C3 * frequency * frequency * frequency );	\n"
"		float CohEnv = exp( -(( beta * beta )/( 4 * wavelength * wavelength )) * SpatCohPart * SpatCohPart ) *	\n"
"						exp( -(0.5f * delta * delta)*(fabs( 3.141592654f * wavelength * frequency * frequency ) * fabs( 3.141592654f * wavelength * frequency * frequency )));	\n"
"		float gamma = ( .166666666f * 3.1415927f * wavelength ) * ( 6 * A1r * kx * kx + 6 * C1 * kx * kx - 6 * A1r * ky * ky	\n"
"				  + 6 * C1 * ky * ky + 3 * C3 * wavelength * wavelength * kx * kx * kx * kx + 6 * C3 * wavelength * wavelength * kx * kx * ky * ky	\n"
"				  + 3 * C3 * wavelength * wavelength * ky * ky * ky * ky + 12 * A1i * kx * ky) ;	\n"
"		clw[Index].x = (frequency<=objap) * CohEnv * cos(gamma);	\n"
"		clw[Index].y = (frequency<=objap) * CohEnv * -1 * sin(gamma);	\n"
"		if(Index==0)	\n"
"		{\n"
"			clw[Index].x = 0.0f; \n" 
"			clw[Index].y = 0; \n"
"		}\n"
"	}	\n"
"}	\n"
;

const char* wavetransferfunctionminussource2 = 
"__kernel void clWaveTransferFunctionMinus(__global float2* clw, __global const float* CLxFrequencies, __global const float* CLyFrequencies, int sizeX, int sizeY, float wavelength, float beta, float delta, float A1r, float A1i, float C1, float C3, float objap)	\n"
"{	\n"
"	//Get the work items ID \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	\n"
"	if(xid<sizeX&&yid<sizeY) \n"
"	{	\n"
"		int Index = xid + yid*sizeX; \n"
"		float frequency = sqrt(CLxFrequencies[xid]*CLxFrequencies[xid] + CLyFrequencies[yid]*CLyFrequencies[yid]);	\n"
"		float kx = -CLxFrequencies[xid];	\n"
"		float ky = -CLyFrequencies[yid];	\n"
"		float SpatCohPart = fabs( wavelength * 2 * 3.141592654f * C1 * frequency + 2 * 3.141592654f * wavelength * wavelength * wavelength * C3 * frequency * frequency * frequency );	\n"
"		float CohEnv = exp( -(( beta * beta )/( 4 * wavelength * wavelength )) * SpatCohPart * SpatCohPart ) *	\n"
"						exp( -(0.5f * delta * delta)*(fabs( 3.141592654f * wavelength * frequency * frequency ) * fabs( 3.141592654f * wavelength * frequency * frequency )));	\n"
"		float gamma = ( .166666666 * 3.1415927f * wavelength ) * ( 6 * A1r * kx * kx + 6 * C1 * kx * kx - 6 * A1r * ky * ky	\n"
"				  + 6 * C1 * ky * ky + 3 * C3 * wavelength * wavelength * kx * kx * kx * kx + 6 * C3 * wavelength * wavelength * kx * kx * ky * ky	\n"
"				  + 3 * C3 * wavelength * wavelength * ky * ky * ky * ky + 12 * A1i * kx * ky) ;	\n"
"		clw[Index].x = (frequency<=objap) * CohEnv * cos(gamma);	\n"
"		clw[Index].y = (frequency<=objap) * CohEnv * -1 * sin(gamma);	\n"
"		if(Index==0)	\n"
"		{\n"
"			clw[Index].x = 0.0f; \n" 
"			clw[Index].y = 0; \n"
"		}\n"
"	}	\n"
"}	\n"
;


const char* wavetransferfunctionmtfsource2 = 
"__kernel void clWaveTransferFunctionMTF(__global float2* clw, __global const float* CLxFrequencies, __global const float* CLyFrequencies, int sizeX, int sizeY, \n"
" float wavelength, float beta, float delta, float A1r, float A1i, float C1, float C3, float objap, __global const float* clMTF, int mtflength, float scaleMTFx, float scaleMTFy)	\n"
"{	\n"
"	//Get the work items ID \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	\n"
"	if(xid<sizeX&&yid<sizeY) \n"
"	{	\n"
"		int Index = xid + yid*sizeX; 																																					\n"
"		float frequency = sqrt(CLxFrequencies[xid]*CLxFrequencies[xid] + CLyFrequencies[yid]*CLyFrequencies[yid]);																		\n"
"		float kx = CLxFrequencies[xid];																																					\n"
"		float ky = CLyFrequencies[yid];																																					\n"
"																																														\n"
"		int xposmtf;																																									\n"
"		int yposmtf;																																									\n"
"																																														\n"
"		if(xid < sizeX/2)																																								\n"
"			xposmtf = xid;																																								\n"
"		else																																											\n"
"			xposmtf = (sizeX-xid);																																						\n"
"																																														\n"
"		if(yid < sizeY/2)																																								\n"
"			yposmtf = yid;																																								\n"
"		else																																											\n"
"			yposmtf = sizeY-yid;																																						\n"
"																																														\n"
"		float mtfposition = sqrt(scaleMTFy*yposmtf*scaleMTFy*yposmtf + scaleMTFx*xposmtf*scaleMTFx*xposmtf);																			\n"
"																																														\n"
"		int mtfpos = floor(sqrt(scaleMTFy*yposmtf*scaleMTFy*yposmtf + scaleMTFx*xposmtf*scaleMTFx*xposmtf));																			\n"
"																																														\n"		
"		float interp = mtfposition - floor(mtfposition);																																\n"
"																																														\n"		
"		if(mtfpos>mtflength-2)																																							\n"
"			mtfpos=mtflength-2;																																							\n"
"																																														\n"
"		float mtf = (1-interp)*clMTF[mtfpos] + interp*clMTF[mtfpos+1];																													\n"
"		float SpatCohPart = fabs( wavelength * 2 * 3.141592654f * C1 * frequency + 2 * 3.141592654f * wavelength * wavelength * wavelength * C3 * frequency * frequency * frequency );	\n"
"		float CohEnv = exp( -(( beta * beta )/( 4 * wavelength * wavelength )) * SpatCohPart * SpatCohPart ) *																			\n"
"						exp( -(0.5 * delta * delta)*(fabs( 3.141592654f * wavelength * frequency * frequency ) * fabs( 3.141592654f * wavelength * frequency * frequency )));			\n"
"		float gamma = ( .166666666f * 3.1415927f * wavelength ) * ( 6 * A1r * kx * kx + 6 * C1 * kx * kx - 6 * A1r * ky * ky															\n"
"				  + 6 * C1 * ky * ky + 3 * C3 * wavelength * wavelength * kx * kx * kx * kx + 6 * C3 * wavelength * wavelength * kx * kx * ky * ky										\n"
"				  + 3 * C3 * wavelength * wavelength * ky * ky * ky * ky + 12 * A1i * kx * ky) ;																						\n"
"		clw[Index].x = (frequency<=objap) * mtf * CohEnv * cos(gamma);																													\n"
"		clw[Index].y = (frequency<=objap) * mtf * CohEnv * -1 * sin(gamma);																												\n"
"		if(Index==0)																																									\n"
"		{\n"
"			clw[Index].x = 0.0f;																																						\n" 
"			clw[Index].y = 0;																																							\n"
"		}\n"
"	} \n"
"} \n"
;


const char* wavetransferfunctionminusmtfsource2 = 
"__kernel void clWaveTransferFunctionMinusMTF(__global float2* clw, __global float* CLxFrequencies, __global float* CLyFrequencies, int sizeX, int sizeY, float wavelength, float beta, float delta, float A1r, float A1i, float C1, float C3, float objap, __global float* clMTF, int mtflength, float scaleMTFx, float scaleMTFy)	\n"
"{	\n"
"	//Get the work items ID \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	\n"
"	if(xid<sizeX&&yid<sizeY) \n"
"	{	\n"
"		int Index = xid + yid*sizeX; \n"
"		float frequency = sqrt(CLxFrequencies[xid]*CLxFrequencies[xid] + CLyFrequencies[yid]*CLyFrequencies[yid]);	\n"
"		float kx = -CLxFrequencies[xid];	\n"
"		float ky = -CLyFrequencies[yid];	\n"
"											\n"
"		int xposmtf;						\n"
"		int yposmtf;						\n"
"											\n"
"		if(xid < sizeX/2)					\n"
"			xposmtf = xid;					\n"
"		else								\n"
"			xposmtf = (sizeX-xid);			\n"
"											\n"
"		if(yid < sizeY/2)					\n"
"			yposmtf = yid;					\n"
"		else								\n"
"			yposmtf = sizeY-yid;			\n"
"											\n"
"		float mtfposition = sqrt(scaleMTFy*yposmtf*scaleMTFy*yposmtf + scaleMTFx*xposmtf*scaleMTFx*xposmtf);	\n"
"																												\n"
"		int mtfpos = floor(sqrt(scaleMTFy*yposmtf*scaleMTFy*yposmtf + scaleMTFx*xposmtf*scaleMTFx*xposmtf));	\n"
"																												\n"		
"		float interp = mtfposition - floor(mtfposition);														\n"
"																												\n"		
"		if(mtfpos>mtflength-2)																					\n"
"			mtfpos=mtflength-2;																					\n"
"																												\n"
"		float mtf = (1-interp)*clMTF[mtfpos] + interp*clMTF[mtfpos+1];											\n"
"		float SpatCohPart = fabs( wavelength * 2 * 3.141592654f * C1 * frequency + 2 * 3.141592654f * wavelength * wavelength * wavelength * C3 * frequency * frequency * frequency );	\n"
"		float CohEnv = exp( -(( beta * beta )/( 4 * wavelength * wavelength )) * SpatCohPart * SpatCohPart ) *	\n"
"						exp( -(0.5f * delta * delta)*(fabs( 3.141592654f * wavelength * frequency * frequency ) * fabs( 3.141592654f * wavelength * frequency * frequency )));	\n"
"		float gamma = ( .166666666f * 3.1415927f * wavelength ) * ( 6 * A1r * kx * kx + 6 * C1 * kx * kx - 6 * A1r * ky * ky	\n"
"				  + 6 * C1 * ky * ky + 3 * C3 * wavelength * wavelength * kx * kx * kx * kx + 6 * C3 * wavelength * wavelength * kx * kx * ky * ky	\n"
"				  + 3 * C3 * wavelength * wavelength * ky * ky * ky * ky + 12 * A1i * kx * ky) ;	\n"
"		clw[Index].x = (frequency<=objap) * mtf * CohEnv * cos(gamma);	\n"
"		clw[Index].y = (frequency<=objap) * mtf * CohEnv * -1 * sin(gamma);	\n"
"		if(Index==0)	\n"
"		{\n"
"			clw[Index].x = 0.0f; \n" 
"			clw[Index].y = 0; \n"
"		}\n"
"	}	\n"
"}\n"
;


const char* wienerwsource2 = 
"__kernel void clWienerW(__global float* clW, __global float2* clw, int sizeX, int sizeY, int init)	\n"
"{	\n"
"	//Get the work items ID \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	\n"
"	if(xid<sizeX&&yid<sizeY) \n"
"	{	\n"
"		int Index = xid + yid*sizeX; \n"
"		float absw = sqrt(clw[Index].x*clw[Index].x + clw[Index].y*clw[Index].y);	\n"
"		if(init==1)	\n"
"		{\n"
"			clW[Index] = 0; \n" 
"		}\n"
"		clW[Index] += absw*absw;	\n"
"	}	\n"
"}	\n"
;


const char* wienervsource2 = 
"__kernel void clWienerV(__global float2* clV, __global float2* clw, __global float2* clwminus, int sizeX, int sizeY, int init)	\n"
"{	\n"
"	//Get the work items ID \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	\n"
"	if(xid<sizeX&&yid<sizeY) \n"
"	{	\n"
"		int Index = xid + yid*sizeX; \n"
"		float real = clw[Index].x * clwminus[Index].x - clw[Index].y * clwminus[Index].y;	\n"
"		float imag = clw[Index].x * clwminus[Index].y + clw[Index].y * clwminus[Index].x;	\n"
"		if(init==1)	\n"
"		{\n"
"			clV[Index].x = 0; \n" 
"			clV[Index].y = 0; \n" 
"		}\n"
"		clV[Index].x += real;	\n"
"		clV[Index].y += imag;	\n"
"	}	\n"
"}	\n"
;


const char* wienertsource2 = 
"__kernel void clWienerT(__global float2* clT, __global float2* clw, __global float2* fft, int sizeX, int sizeY, int init)	\n"
"{	\n"
"	//Get the work items ID \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	\n"
"	if(xid<sizeX&&yid<sizeY) \n"
"	{	\n"
"		int Index = xid + yid*sizeX; \n"
"		float real = clw[Index].x*fft[Index].x + clw[Index].y*fft[Index].y;	\n"
"		float imag = clw[Index].x*fft[Index].y - clw[Index].y*fft[Index].x;	\n"
"		if(init==1)	\n"
"		{\n"
"			clT[Index].x = 0; \n" 
"			clT[Index].y = 0; \n" 
"		}\n"
"		clT[Index].x += real;	\n"
"		clT[Index].y += imag;	\n"
"	}	\n"
"}	\n"
;



const char* wienerusource2 = 
"__kernel void clWienerU(__global float2* clU, __global float2* clwminus, __global float2* fft, int sizeX, int sizeY, int init)	\n"
"{	\n"
"	//Get the work items ID \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	\n"
"	if(xid<sizeX&&yid<sizeY) \n"
"	{	\n"
"		int Index = xid + yid*sizeX; \n"
"		float real = clwminus[Index].x*fft[Index].x - clwminus[Index].y*fft[Index].y;	\n"
"		float imag = clwminus[Index].x*fft[Index].y + clwminus[Index].y*fft[Index].x;	\n"
"		if(init==1)	\n"
"		{\n"
"			clU[Index].x = 0; \n" 
"			clU[Index].y = 0; \n" 
"		}\n"
"		clU[Index].x += real;	\n"
"		clU[Index].y += imag;	\n"
"	}	\n"
"}	\n"
;


const char* makerestoredsource2 = 
"__kernel void clMakeRestored(__global float* clW,__global float* clWminus, __global float2* clV,__global float2* clT, __global float2* clU, __global float2* clRestored, int sizeX, int sizeY,float snr)	\n"
"{	\n"
"	//Get the work items ID \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	\n"
"	if(xid<sizeX&&yid<sizeY) \n"
"	{	\n"
"		int Index = xid + yid*sizeX; \n"
"		float real = (clWminus[Index] + snr)*clT[Index].x - clV[Index].x * clU[Index].x - clV[Index].y * clU[Index].y;	\n"
"		float imag = (clWminus[Index] + snr)*clT[Index].y - clV[Index].x * clU[Index].y + clV[Index].y * clU[Index].x;	\n"
"		float denom = ((clWminus[Index] + snr) * (clW[Index] + snr)) - ( clV[Index].x * clV[Index].x + clV[Index].y * clV[Index].y ); \n"
"		clRestored[Index].x = real/denom;	\n"
"		clRestored[Index].y = imag/denom;	\n"
"	}	\n"
"}	\n"
;

const char* makerestoredMTFNPSsource2 = 
"__kernel void clMakeRestoredMTFNPS(__global float* clW,__global float* clWminus, __global float2* clV,__global float2* clT, __global float2* clU, __global float2* clRestored, int sizeX, int sizeY, __global float* clMTF, __global float* clNPS, int mtflength, float scaleMTFx, float scaleMTFy)	\n"
"{											\n"
"	//Get the work items ID					\n"
"	int xid = get_global_id(0);				\n"
"	int yid = get_global_id(1);				\n"
"											\n"
"	if(xid<sizeX&&yid<sizeY)				\n"
"	{										\n"
"		int Index = xid + yid*sizeX;		\n"
"											\n"
"		int xposmtf;						\n"
"		int yposmtf;						\n"
"											\n"
"		if(xid < sizeX/2)					\n"
"			xposmtf = xid;					\n"
"		else								\n"
"			xposmtf = (sizeX-xid);			\n"
"											\n"
"		if(yid < sizeY/2)					\n"
"			yposmtf = yid;					\n"
"		else								\n"
"			yposmtf = sizeY-yid;			\n"
"											\n"
"		float mtfposition = sqrt(scaleMTFy*yposmtf*scaleMTFy*yposmtf + scaleMTFx*xposmtf*scaleMTFx*xposmtf);			\n"
"																														\n"
"		int mtfpos = floor(sqrt(scaleMTFy*yposmtf*scaleMTFy*yposmtf + scaleMTFx*xposmtf*scaleMTFx*xposmtf));			\n"
"																														\n"		
"		float interp = mtfposition - floor(mtfposition);																\n"
"																														\n"		
"		if(mtfpos>mtflength-2)																							\n"
"			mtfpos=mtflength-2;																							\n"
"																														\n"
"		float mtf = (1-interp)*clMTF[mtfpos] + interp*clMTF[mtfpos+1];													\n"
"		if(mtf < 0.001)																									\n"
"			mtf = 0.001;																								\n"
"		float nps = (1-interp)*clNPS[mtfpos] + interp*clNPS[mtfpos+1];													\n"
"		if(nps < 0.001)																									\n"
"			nps = 0.001;																								\n"
"		float snr = nps/(100*mtf);																						\n"
"		float real = (clWminus[Index] + snr) * clT[Index].x - clV[Index].x*clU[Index].x - clV[Index].y*clU[Index].y;	\n"
"		float imag = (clWminus[Index] + snr) * clT[Index].y - clV[Index].x*clU[Index].y + clV[Index].y*clU[Index].x;	\n"
"		float denom = ((clWminus[Index] + snr) * (clW[Index] + snr)) - ( clV[Index].x * clV[Index].x + clV[Index].y * clV[Index].y ); \n"
"		clRestored[Index].x = real/denom;	\n"
"		clRestored[Index].y = imag/denom;	\n"
"	}	\n"
"}	\n"
;

const char* getQsource2 = 
"__kernel void clCalculateQ(__global float* clW, __global float* clWminus, __global float2* clV, __global float2* clQ, int sizeX, int sizeY, float snr)	\n"
"{	\n"
"	//Get the work items ID \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	\n"
"	if(xid<sizeX&&yid<sizeY) \n"
"	{	\n"
"		int Index = xid + yid*sizeX; \n"
"		float wtf  = (clWminus[Index]+snr)*clW[Index] -clV[Index].x*clV[Index].x - clV[Index].y*clV[Index].y;\n"
"		float wtfCr = snr*clV[Index].x;\n"
"		float wtfCi = snr*clV[Index].y;\n"
"		float denom = (clWminus[Index]+snr)*(clW[Index]+snr)-clV[Index].x*clV[Index].x - clV[Index].y*clV[Index].y; \n"
"		clQ[Index].x = wtf*wtf/(denom*denom) - wtfCr*wtfCr/(denom*denom) - wtfCi*wtfCi/(denom*denom);\n"
"		clQ[Index].y = 0;\n"
"	}\n"
"}	\n"
;

const char* minuswavefunctionsource2 = 
"__kernel void clMinusWavefunction( __global float2* clFFT, __global float2* clFFTminus, int sizeX, int sizeY)	\n"
"{	\n"
"	//Get the work items ID \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	\n"
"	if(xid<sizeX&&yid<sizeY) \n"
"	{	\n"
"		int Index = xid + yid*sizeX; \n"
"		int Index2 = sizeX-xid + (sizeY-yid)*sizeX; \n"
"		if(xid==0) \n"
"			Index2-=sizeX; \n"
"		if(yid==0) \n"
"			Index2-=sizeX*sizeY; \n"
"		clFFTminus[Index] = clFFT[Index2]; \n"
"	}\n"
"}	\n"
;

const char* predictedsource = 
"__kernel void clPredicted( __global float2* clFFT, __global float2* clPred, int sizeX, int sizeY)	\n"
"{	\n"
"	//Get the work items ID \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	\n"
"	if(xid<sizeX&&yid<sizeY) \n"
"	{	\n"
"		int Index = xid + yid*sizeX; \n"
"		int Index2 = sizeX-xid + (sizeY-yid)*sizeX; \n"
"		if(xid==0) \n"
"			Index2-=sizeX; \n"
"		if(yid==0) \n"
"			Index2-=sizeX*sizeY; \n"
"		clPred[Index].x = clFFT[Index].x + clFFT[Index2].x; \n"
"		clPred[Index].y = clFFT[Index].y - clFFT[Index2].y; \n"
"	}\n"
"}	\n"
;

// Changed frequencies, were negative for some reason
const char* getPCIsource2 = 
"__kernel void clCalculatePCI(__global float2* clQ, __global float2* clFFT, __global float2* clFFTminus, \n"
"__global float* CLxFrequencies, __global float* CLyFrequencies, __global float2* clPCI, __global float2* clPCIC, \n"
" __global float2* clPCIM, __global float2* clPCIS, int sizeX, int sizeY, float C1, float C3, float A1r, float A1i, float wavelength, float objap)	\n"
"{	\n"
"	//Get the work items ID \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	\n"
"	if(xid<sizeX&&yid<sizeY) \n"
"	{	\n"
"		int Index = xid + yid*sizeX; \n"
"		float freq = sqrt(CLxFrequencies[xid]*CLxFrequencies[xid] + CLyFrequencies[yid]*CLyFrequencies[yid]);	\n"
"		float kx = CLxFrequencies[xid];	\n"
"		float ky = CLyFrequencies[yid];	\n"
"		float gamma = ( .166666666 * 3.1415927f * wavelength ) * ( 6 * A1r * kx * kx + 6 * C1 * kx * kx - 6 * A1r * ky * ky	\n"
"				  + 6 * C1 * ky * ky + 3 * C3 * wavelength * wavelength * kx * kx * kx * kx + 6 * C3 * wavelength * wavelength * kx * kx * ky * ky	\n"
"				  + 3 * C3 * wavelength * wavelength * ky * ky * ky * ky + 12 * A1i * kx * ky) ;	\n"
"		float arg1 = atan2(clFFT[Index].y,clFFT[Index].x);					\n"
"		float arg2 = atan2(clFFTminus[Index].y,clFFTminus[Index].x);		\n"
"		float angle = atan2(ky,kx);										\n"
"		clPCI[Index].x = (freq<=objap)*clQ[Index].x*-1 *cos(arg1+arg2+2*gamma); \n"
"		clPCI[Index].y = 0; \n"
"		clPCIC[Index].x = (freq<=objap)*clQ[Index].x*-1 *cos(arg1+arg2+2*gamma)*cos(2*angle); \n"
"		clPCIC[Index].y = 0; \n"
"		clPCIS[Index].x = (freq<=objap)*clQ[Index].x*-1 *cos(arg1+arg2+2*gamma)*sin(2*angle); \n"
"		clPCIS[Index].y = 0; \n"
"		clPCIM[Index].x = (freq<=objap)*(clPCI[Index].x + sqrt(clPCIC[Index].x * clPCIC[Index].x + clPCIS[Index].x * clPCIS[Index].x)); \n"
"		clPCIM[Index].y = 0; \n"
"	}\n"
"}	\n"
;


const char* sumReductionsource2 = 
"__kernel void clSumReduction(__global const float2* input, __global float2* output, const unsigned int size, __local float2* buffer)	\n"
"{																																		\n"
"	//Get the work items ID																												\n"
"	size_t idx = get_local_id(0);																										\n"
"	size_t stride = get_global_size(0);																									\n"
"	buffer[idx] = 0;																													\n"
"	// Copy data into shared memory buffer, each threads gets multiple values if we launch less threads than size						\n"
"	for(size_t pos = get_global_id(0); pos < size; pos += stride )																		\n"
"		buffer[idx] += input[pos];																										\n"
"	// Make sure every thread in workgroup has finished writing to shared memory before continuing										\n"
"	barrier(CLK_LOCAL_MEM_FENCE);																										\n"
"	// Loop and sum over all values with first thread in workgroup																		\n"
"	float2 sum = 0;																														\n"
"	if(!idx) {																															\n"
"		for(size_t i = 1; i < get_local_size(0); ++i)																					\n"
"			sum += buffer[i];																											\n"
"																																		\n"
"		output[get_group_id(0)] = sum;																									\n"
"	}																																	\n"
"}																																		\n"
;

const char* sumReductionsource3 = 
"__kernel void clSumReductionFloat(__global const float* input, __global float* output, const unsigned int size, __local float* buffer, int offset)	\n"
"{																																		\n"
"	//Get the work items ID																												\n"
"	size_t idx = get_local_id(0);																										\n"
"	size_t stride = get_global_size(0);																									\n"
"	buffer[idx] = 0;																													\n"
"																																		\n"
"	for(size_t pos = get_global_id(0); pos < size; pos += stride )																		\n"
"		buffer[idx] += input[offset+pos];																										\n"
"																																		\n"
"	barrier(CLK_LOCAL_MEM_FENCE);																										\n"
"																																		\n"
"	float sum = 0;																														\n"
"	if(!idx) {																															\n"
"		for(size_t i = 1; i < get_local_size(0); ++i)																					\n"
"			sum += buffer[i];																											\n"
"																																		\n"
"		output[get_group_id(0)] = sum;																									\n"
"	}																																	\n"
"}																																		\n"
;

const char* sumReductionsource4 = 
"__kernel void clSumReductionUint(__global const uint* input, __global uint* output, const unsigned int size, __local uint* buffer, int offset)	\n"
"{																																		\n"
"	//Get the work items ID																												\n"
"	size_t idx = get_local_id(0);																										\n"
"	size_t stride = get_global_size(0);																									\n"
"	buffer[idx] = 0;																													\n"
"																																		\n"
"	for(size_t pos = get_global_id(0); pos < size; pos += stride )																		\n"
"		buffer[idx] += input[offset+pos];																										\n"
"																																		\n"
"	barrier(CLK_LOCAL_MEM_FENCE);																										\n"
"																																		\n"
"	uint sum = 0;																														\n"
"	if(!idx) {																															\n"
"		for(size_t i = 1; i < get_local_size(0); ++i)																					\n"
"			sum += buffer[i];																											\n"
"																																		\n"
"		output[get_group_id(0)] = sum;																									\n"
"	}																																	\n"
"}																																		\n"
;

const char* MaxReductionsource2 = 
"__kernel void clMaxReduction(__global const float* input, __global float* output, const unsigned int size, __local float* buffer)	\n"
"{																																		\n"
"	//Get the work items ID																												\n"
"	size_t idx = get_local_id(0);																										\n"
"	size_t stride = get_global_size(0);																									\n"
"	buffer[idx] = 0;																													\n"
"	// Copy data into shared memory buffer, each threads gets multiple values if we launch less threads than size						\n"
"	for(size_t pos = get_global_id(0); pos < size; pos += stride )																		\n"
"		buffer[idx] = max(buffer[pos],buffer[idx]);																						\n"
"	// Make sure every thread in workgroup has finished writing to shared memory before continuing										\n"
"	barrier(CLK_LOCAL_MEM_FENCE);																										\n"
"	// Loop and sum over all values with first thread in workgroup																		\n"
"	float max = 0;																														\n"
"	if(!idx) {																															\n"
"		for(size_t i = 1; i < get_local_size(0); ++i)																					\n"
"			max = max(sum,buffer[i]);																									\n"
"																																		\n"
"		output[get_group_id(0)] = max;																									\n"
"	}																																	\n"
"}																																		\n"
;

// Do opposite of angle to determine position in rotated image to get data from??
// Do i need opposite of magscale aswell
const char* RotScalesource2 = 
"__kernel void clRotScale(__global const float2* Input, __global float2* Output, int width, int height, float magscale, float rotscale, float df) \n"
"{ \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	int centrex = width/2;	\n"
"	int centrey = height/2; \n"
"	if(xid < width && yid < height) \n"
"	{	\n"
"		int Index = xid + yid*width; \n"
"		float xp = (float)(xid-centrex); \n"
"		float yp = (float)(yid-centrey); \n"
"		float r = hypot( xp , yp); \n"
"		float theta = atan2( yp, xp); \n"
"		float newx = centrex + r*((magscale*df -df) +1.0f) * cos(theta + df*rotscale); \n"
"		float newy = centrey + r*((magscale*df -df) +1.0f) * sin(theta + df*rotscale); \n"
"		int xs2 = floor(newx); \n"
"		int ys2 = floor(newy); \n"
"		if( (xs2  >= width - 1  || xs2 < 0) || (ys2  >= height -1  || ys2 < 0) ) \n"
"		{ \n"
"			Output[Index].x = 0.0f; \n"
"			Output[Index].y = 0.0f; \n"
"		}  else { \n"
"			float subx = newx - xs2; \n"
"			float suby = newy - ys2; \n"
"			float v1 = (1-subx)*(1-suby)*Input[xs2 + ys2*width].x; \n"
"			float v2 = (subx)*(1-suby)*Input[xs2 + 1 + ys2*width].x; \n"
"			float v3 = (1-subx)*(suby)*Input[xs2 + (ys2 + 1)*width].x; \n"
"			float v4 = (subx)*(suby)*Input[xs2 + 1 + (ys2 + 1)*width].x; \n"
"			Output[Index].x = v1 + v2 + v3 + v4; \n"
"			Output[Index].y = 0.0f; \n"
"		} \n"
"	}	\n"
"}	\n"
;

// Rotate Scale Crop Shift and Pad all in one go to avoid errors from interpolating twice....
const char* PadCropRotScalesource2 = 
"__kernel void clPadCropRotScale(__global const float2* Input, __global float2* Output, int fullwidth, int fullheight, int padLeft, int padRight, int padTop, int padBottom, float xShift, float yShift, int newwidth, int newheight, int iTop, int iLeft) \n"
"{ \n"
"	//Get the work items ID \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	int centrex = width/2;	\n"
"	int centrey = height/2; \n"
"	if(xid < newwidth && yid < newheight) \n"
"	{	\n"
"		int Index = xid + yid*newwidth; \n"
"		int xs2 = floor(xShift); \n"
"		int ys2 = floor(yShift); \n"
"		float subx = xShift - xs2; \n"
"		float suby = yShift - ys2; \n"
"		if((xid < padLeft || xid >= (newwidth - padRight)) || (yid < padTop || yid >= (newheight - padBottom))) \n"
"		{ \n"
"			Output[Index].x = 0; \n"
"			Output[Index].y = 0; \n"
"		} else { \n"
"			float v1 = (1-subx)*(1-suby)*Input[iLeft + xs2 + xid + (iTop+ys2+yid)*fullwidth].x; \n"
"			float v2 = (subx)*(1-suby)*Input[iLeft + xs2 + xid + 1 + (iTop+ys2+yid)*fullwidth].x; \n"
"			float v3 = (1-subx)*(suby)*Input[iLeft + xs2 + xid + (iTop+ys2+yid + 1)*fullwidth].x; \n"
"			float v4 = (subx)*(suby)*Input[iLeft + xs2 + xid + 1 +(iTop+ys2+yid + 1)*fullwidth].x; \n"
"			Output[Index].x = v1 + v2 + v3 + v4; \n"
"			Output[Index].y = 0.0f; \n"
"		} \n"
"	}	\n"
"}	\n"
;

// Trying to turn EW into a normal image for PCF. Tried adding one because images normalised by removing 1.
const char* abssource2 = 
"__kernel void clAbs(__global float2* clEW, int sizeX, int sizeY)	\n"
"{	\n"
"	//Get the work items ID \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	\n"
"	if(xid<sizeX&&yid<sizeY) \n"
"	{	\n"
"		int Index = xid + yid*sizeX; \n"
"		float real = clEW[Index].x;	\n"
"		float imag = clEW[Index].y;	\n"
"		clEW[Index].x = hypot(real,imag);	\n"
"		clEW[Index].y = 0;	\n"
"	}	\n"
"}	\n"
;

const char* HanningWindowSource = 
"__kernel void clHanningWindow(__global float2* Image, int sizeX, int sizeY)	\n"
"{	\n"
"	//Get the work items ID \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	\n"
"	if(xid<sizeX&&yid<sizeY) \n"
"	{	\n"
"		int Index = xid + yid*sizeX; \n"
"		float windowvalue = 0.5f*(1-cos(2*3.1415927f*xid/(sizeX-1))) * 0.5f*(1-cos(2*3.1415927f*yid/(sizeY-1))); \n"
"		Image[Index].x *= windowvalue;	\n"
"		Image[Index].y *= windowvalue;	\n"
"	}	\n"
"}	\n"
;