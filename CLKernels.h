
// input + (multi * input2)
const char* additionSubtractionSource = 
"__kernel void clAdditionSubtraction(__global const float2* Input, __global const float2* Input2, __global float2* Output, int width, int height, float multi) \n"
"{		\n"
"	//Get the work items ID \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	if(xid<width&&yid<height) \n"
"	{	\n"
"		int Index = xid + yid*width; \n"
"		Output[Index].x = Input[Index].x + multi * sqrt(Input2[Index].x * Input2[Index].x + Input2[Index].y * Input2[Index].y); \n"
"		Output[Index].y = Input[Index].y; \n"
"		//Output[Index].x = Input[Index].x + multi * Input2[Index].x; \n"
"		//Output[Index].y = Input[Index].y + multi * Input2[Index].y; \n"
"	}	\n"
"}		\n"
;

// input * multi * input2
const char* multiplicationSource = 
"__kernel void clMultiplication(__global const float2* Input, __global const float2* Input2, __global float2* Output, int width, int height, float multi) \n"
"{		\n"
"	//Get the work items ID \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	if(xid<width&&yid<height) \n"
"	{	\n"
"		int Index = xid + yid*width; \n"
"		Output[Index].x = Input[Index].x * multi *Input2[Index].x; \n"
"		Output[Index].y = Input[Index].y * multi *Input2[Index].y; \n"
"	}	\n"
"}		\n"
;

// input * multi * input2
const char* multiConjSource = 
"__kernel void clMultiConj(__global const float2* Input, __global float2* Input2, __global float2* Ouput, int width, int height) \n"
"{		\n"
"	//Get the work items ID \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	if(xid<width&&yid<height) \n"
"	{	\n"
"		int Index = xid + yid*width; \n"
"		Ouput[Index].x = Input[Index].x * Input2[Index].x + Input[Index].y * Input2[Index].y; \n"
"		Ouput[Index].y = Input2[Index].x * Input[Index].y - Input[Index].x * Input2[Index].y; \n"
"	}	\n"
"}		\n"
;

// Polar Transform around centre point maybe too many args
const char* Polar1Source = 
"__kernel void clTransform(__global const float2* Input, __global float2* Output, int width, int height, int polarwidth, int polarheight, float rscale) \n"
"{ \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	int centrex = width/2;	\n"
"	int centrey = height/2; \n"
"	if(xid < polarwidth && yid < polarheight) \n"
"	{	\n"
"		float ph = polarheight; \n"
"		float theta = (2 * yid * 3.141592f ) / ph; \n"
"		float xpos = rscale * (xid+30) * cos(theta); \n"
"		float ypos = rscale * (xid+30) * sin(theta); \n"
"		int Index = xid + yid*polarwidth; \n"
"		int xpos2 = floor(xpos); \n"
"		int ypos2 = floor(ypos); \n"
"		float interpx = xpos - xpos2; \n"
"		float interpy = ypos - ypos2; \n"
"		Output[Index].x = (1 - interpx)*(1 - interpy) * Input[(centrey + ypos2) * width + centrex + xpos2].x + (interpx)*(1 - interpy) * Input[(centrey + ypos2) * width + centrex  + xpos2 + 1].x \n"
"		+ (1 - interpx)*(interpy)* Input[(centrey + ypos2 + 1) * width + centrex  + xpos2].x + ( interpx)*(interpy)* Input[(centrey + ypos2 + 1) * width + centrex + xpos2 + 1].x; \n"
"		Output[Index].y = (1 - interpx)*(1 - interpy) * Input[(centrey + ypos2) * width + centrex + xpos2].y + (interpx)*(1 - interpy) * Input[(centrey + ypos2) * width + centrex  + xpos2 + 1].y \n"
"		+ (1 - interpx)*(interpy)* Input[(centrey + ypos2 + 1) * width + centrex  + xpos2].y + ( interpx)*(interpy)* Input[(centrey + ypos2 + 1) * width + centrex + xpos2 + 1].y; \n"
"	}	\n"
"} \n"
;

// Polar Transform around centre point maybe too many args
const char* Polar1Sourceflip = 
"__kernel void clTransformFlip(__global const float2* Input, __global float2* Output, int width, int height, int polarwidth, int polarheight, float rscale) \n"
"{ \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	int centrex = width/2;	\n"
"	int centrey = height/2; \n"
"	if(xid < polarwidth && yid < polarheight) \n"
"	{	\n"
"		float ph = polarheight; \n"
"		float theta = (2 * yid * 3.141592f ) / ph; \n"
"		float xpos = rscale * (xid+30) * cos(theta); \n"
"		float ypos = rscale * (xid+30) * sin(theta); \n"
"		ypos *= -1; \n"
"		int Index = xid + yid*polarwidth; \n"
"		int xpos2 = floor(xpos); \n"
"		int ypos2 = floor(ypos); \n"
"		float interpx = xpos - xpos2; \n"
"		float interpy = ypos - ypos2; \n"
"		Output[Index].x = (1 - interpx)*(1 - interpy) * Input[(centrey + ypos2) * width + centrex + xpos2].x + (interpx)*(1 - interpy) * Input[(centrey + ypos2) * width + centrex  + xpos2 + 1].x \n"
"		+ (1 - interpx)*(interpy)* Input[(centrey + ypos2 + 1) * width + centrex  + xpos2].x + ( interpx)*(interpy)* Input[(centrey + ypos2 + 1) * width + centrex + xpos2 + 1].x; \n"
"		Output[Index].y = (1 - interpx)*(1 - interpy) * Input[(centrey + ypos2) * width + centrex + xpos2].y + (interpx)*(1 - interpy) * Input[(centrey + ypos2) * width + centrex  + xpos2 + 1].y \n"
"		+ (1 - interpx)*(interpy)* Input[(centrey + ypos2 + 1) * width + centrex  + xpos2].y + ( interpx)*(interpy)* Input[(centrey + ypos2 + 1) * width + centrex + xpos2 + 1].y; \n"
"	}	\n"
"} \n"
;

// Polar Transform around centre point maybe too many args
const char* Polar2Source = 
"__kernel void clTransform2(__global const float2* Input, __global float2* Output, int width, int height, int newwidth, int newheight, float angle, float rscale) \n"
"{ \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	int centrex = width/2;	\n"
"	int centrey = height/2; \n"
"	if(xid < newwidth && yid < newheight) \n"
"	{	\n"
"		float rad2 = (float)((newwidth/2-xid)*(newwidth/2-xid) + (newheight/2-yid)*(newheight/2-yid)); \n"
"		float rad = sqrt(rad2); \n"
"		float angy = (float)(yid-newheight/2); \n"
"		float angx = (float)(xid-newwidth/2); \n"
"		float theta = atan2(angy,angx)+angle; \n"
"		float xpos = rad * rscale * cos(theta); \n"
"		float ypos = rad * rscale * sin(theta); \n"
"		int Index = xid + yid*newwidth; \n"
"		int xpos2 = floor(xpos); \n"
"		int ypos2 = floor(ypos); \n"
"		float interpx = xpos - xpos2; \n"
"		float interpy = ypos - ypos2; \n"
"		Output[Index].x = (1 - interpx)*(1 - interpy) * Input[(centrey + ypos2) * width + centrex + xpos2].x + (interpx)*(1 - interpy) * Input[(centrey + ypos2) * width + centrex  + xpos2 + 1].x \n"
"		+ (1 - interpx)*(interpy)* Input[(centrey + ypos2 + 1) * width + centrex  + xpos2].x + ( interpx)*(interpy)* Input[(centrey + ypos2 + 1) * width + centrex + xpos2 + 1].x; \n"
"		Output[Index].y = (1 - interpx)*(1 - interpy) * Input[(centrey + ypos2) * width + centrex + xpos2].y + (interpx)*(1 - interpy) * Input[(centrey + ypos2) * width + centrex  + xpos2 + 1].y \n"
"		+ (1 - interpx)*(interpy)* Input[(centrey + ypos2 + 1) * width + centrex  + xpos2].y + ( interpx)*(interpy)* Input[(centrey + ypos2 + 1) * width + centrex + xpos2 + 1].y; \n"
"	}	\n"
"} \n"
;


// This takes the quadrant and writes it to shared/local memory for each workgroup, then proceeds to normalise cross-correlate this with a simulated diffractogram for all the different dfs,astigs
// NOTES: assuming that block has z size of 1 to make it easier.
const char* DiffMatchSourceOrig =
"__kernel void clDiffMatch(__global const float* quadrantData, __global float* result, __local float* localQuadrant, int quadrant, int width, int height, int depth, int quadrantwidth, int quadrantheight, float dfstart, \n"
" float dfstep, float astigxstart, float astigxstep, float astigystart, float astigystep, float pixelscale, float wavelength, float Cs) \n"
"{ \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	int zid = get_global_id(2); \n"
"	int times = ceil(float(quadrantwidth * quadrantheight)/float(get_local_size(0)*get_local_size(1))); \n"
"	if(get_local_id(0) < quadrantwidth && get_local_id(1) < quadrantheight) \n"
"	{ \n"
"		for(int i = 0 ; i < times ; i++) \n"
"			if(get_local_id(0) + get_local_id(1)*get_local_size(0) + i*get_local_size(0)*get_local_size(1) < quadrantwidth*quadrantheight) \n"
"				localQuadrant[get_local_id(0) + get_local_id(1)*get_local_size(0) + i*get_local_size(0)*get_local_size(1)] \n"
"				= quadrantData[get_local_id(0) + get_local_id(1)*get_local_size(0) + i*get_local_size(0)*get_local_size(1)]; \n"
"	} \n"
"	barrier(CLK_LOCAL_MEM_FENCE); \n"
"	if(xid >= width || yid >= height || zid >= depth) \n"
"		return; \n"
"	int Index = xid + yid * width + zid * width * height; \n"
"	float defocus = dfstart + xid*dfstep; \n"
"	float astigx = astigxstart + yid*astigxstep; \n"
"	float astigy = astigystart + zid*astigystep; \n"
"	float astigma = sqrt(astigx*astigx + astigy*astigy); \n"
"	float astigaxis = atan2(astigy+0.000001f,astigx+0.000001f); \n"
"	float s1 = 0.0f; \n"
"	float s2 = 0.0f; \n"
"	float s3 = 0.0f; \n"
"	int quadi = 1; \n"
"	int quadj = 1; \n"
"	if(quadrant==3||quadrant==4) \n"
"		quadi = -1; \n"
"	if(quadrant==2||quadrant==3) \n"
"		quadj = -1; \n"
"	for(int i = 1; i <= quadrantwidth; i++) \n"
"		for(int j = 1; j<= quadrantheight; j++) \n"
"		{ \n"
"			if(((quadrantwidth-i)*(quadrantwidth-i) + (quadrantheight-j)*(quadrantheight-j))> 9) \n"
"			{ \n"
"				if(localQuadrant[(i-1)+quadrantwidth*(j-1)]*localQuadrant[(i-1)+quadrantwidth*(j-1)]!=0) \n"
"				{ \n"
"					float k = sqrt((quadrantwidth-i)*(quadrantwidth-i)*pixelscale*pixelscale + (quadrantheight-j)*(quadrantheight-j)*pixelscale*pixelscale); \n"
"					float val = fabs(-sin(2*3.1415926f*((wavelength/2)*defocus*k*k + (wavelength/2)*astigma*k*k*cos(2*(atan2(quadrantheight-0.99999f - quadj*j, quadrantwidth-0.99999f - quadi*i) - astigaxis)) + ((wavelength*wavelength*wavelength)/4)*Cs*k*k*k*k))); \n"
"					s1 += (val * localQuadrant[(i-1)+quadrantwidth*(j-1)]); \n"
"					s2 +=  val*val; \n"
"					s3 +=  localQuadrant[(i-1)+quadrantwidth*(j-1)]*localQuadrant[(i-1)+quadrantwidth*(j-1)]; \n"
"				} \n"
"			} \n"
"		} \n"
"	\n"
"	result[Index] = s1/sqrt(s2*s3); \n"	
"	} \n"
;

const char* DiffMatchSource = 
"__kernel void clDiffMatch(__global const float* quadrantData, __global float* result, __local float* localQuadrant, int quadrant, int width, int height, int depth, int quadrantwidth, int quadrantheight, float dfstart, \n"
" float dfstep, float astigxstart, float astigxstep, float astigystart, float astigystep, float pixelscale, float wavelength, float Cs, float noincluderegion) \n"
"{ \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	int zid = get_global_id(2); \n"
"	//int times = ceil((float)((quadrantwidth * quadrantheight) /(get_local_size(0) * get_local_size(1)))); \n"
"	int times = ( quadrantwidth * quadrantheight + ( get_local_size(0) * get_local_size(1) ) - 1 ) / ( get_local_size(0) * get_local_size(1) ); \n"
"	if(get_local_id(0) < quadrantwidth && get_local_id(1) < quadrantheight && get_local_id(2)== 0) \n"
"	{ \n"
"		for(int i = 0 ; i < times ; i++) \n"
"			if(get_local_id(0) + get_local_id(1)*get_local_size(0) + i*get_local_size(0)*get_local_size(1) < quadrantwidth*quadrantheight) \n"
"				localQuadrant[get_local_id(0) + get_local_id(1)*get_local_size(0) + i*get_local_size(0)*get_local_size(1)] \n"
"				= quadrantData[get_local_id(0) + get_local_id(1)*get_local_size(0) + i*get_local_size(0)*get_local_size(1)]; \n"
"	} \n"
"	barrier(CLK_LOCAL_MEM_FENCE); \n"
"	if(xid >= width || yid >= height || zid >= depth) \n"
"		return; \n"
"	float defocus = dfstart + xid*dfstep; \n"
"	float astigx = astigxstart + yid*astigxstep; \n"
"	float astigy = astigystart + zid*astigystep; \n"
"	float astigma = sqrt(astigx*astigx + astigy*astigy); \n"
"	float astigaxis = atan2(astigy+0.000001f,astigx+0.000001f); \n"
"	float s1 = 0.0f; \n"
"	float s2 = 0.0f; \n"
"	float s3 = 0.0f; \n"
"	int quadi = 1; \n"
"	int quadj = 1; \n"
"	if(quadrant==3||quadrant==4) \n"
"		quadi = -1; \n"
"	if(quadrant==1||quadrant==4) \n"
"		quadj = -1; \n"
"	for(int i = 1; i <= quadrantwidth; i++) \n"
"		for(int j = 1; j<= quadrantheight; j++) \n"
"		{ \n"
"			if(((quadrantwidth-i)*(quadrantwidth-i) + (quadrantheight-j)*(quadrantheight-j))> noincluderegion*noincluderegion) \n"
"			{ \n"
"				if(localQuadrant[(i-1)+quadrantwidth*(j-1)]*localQuadrant[(i-1)+quadrantwidth*(j-1)]!=0) \n"
"				{ \n"
"					float k = sqrt((quadrantwidth-i)*(quadrantwidth-i)*pixelscale*pixelscale + (quadrantheight-j)*(quadrantwidth-j)*pixelscale*pixelscale); \n"
"					float val = -sin(2*3.1415926f*((wavelength/2.0f)*defocus*k*k + (wavelength/2.0f)*astigma*k*k*cos(2.0f*(atan2(quadj*(quadrantheight+0.001f - j), quadi*(quadrantwidth+0.001f - i)) - astigaxis)) + ((wavelength*wavelength*wavelength)/4)*Cs*k*k*k*k)); \n"
"					float val2 = val*val; \n"
"					s1 += (val2 * localQuadrant[(i-1)+quadrantwidth*(j-1)]); \n"
"					s2 +=  val2*val2; \n"
"					s3 +=  localQuadrant[(i-1)+quadrantwidth*(j-1)]*localQuadrant[(i-1)+quadrantwidth*(j-1)]; \n"
"				} \n"
"			} \n"
"		} \n"
"	\n"
"	result[xid + yid * width + zid * width * height] = s1/sqrt(s2*s3); \n"		
"} \n"
;

const char* RadialFilterSource = 
"__kernel void clRadialFilter(__global const float2* Input, __global float2* Output, int width, int height, float filterlength) \n"
"{		\n"
"	//Get the work items ID \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	if(xid < width && yid < height) \n"
"	{	\n"
"		int Index = xid + yid*width; \n"
"		int xr = xid*(xid < (width/2)) + (width-xid)*(xid >= (width/2)); \n"
"		int yr = yid*(yid < (height/2)) + (height-yid)*(!yid >= (height/2)); \n"
"		Output[Index].x = Input[Index].x * exp(-(xr*xr + yr*yr) / filterlength*filterlength); \n"
"		Output[Index].y = Input[Index].y * exp(-(xr*xr + yr*yr) / filterlength*filterlength); \n"
"	}	\n"
"}	\n"
;

const char* DiffMatchSource2 = 
"__kernel void clDiffMatch2(__global const float* quadrantData, __global float* result, __local float* localQuadrant, int width, int height, int quadrantwidth, int quadrantheight, float dfstart, \n"
" float dfstep, float astigstart, float astigstep, float pixelscale, float wavelength, float Cs, float noincluderegion) \n"
"{ \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	//int times = ceil((float)((quadrantwidth * quadrantheight) /(get_local_size(0) * get_local_size(1)))); \n"
"	int times = ( quadrantwidth * quadrantheight + ( get_local_size(0) * get_local_size(1) ) - 1 ) / ( get_local_size(0) * get_local_size(1) ); \n"
"	if(get_local_id(0) < quadrantwidth && get_local_id(1) < quadrantheight ) \n"
"	{ \n"
"		for(int i = 0 ; i < times ; i++) \n"
"			if(get_local_id(0) + get_local_id(1)*get_local_size(0) + i*get_local_size(0)*get_local_size(1) < quadrantwidth*quadrantheight) \n"
"				localQuadrant[get_local_id(0) + get_local_id(1)*get_local_size(0) + i*get_local_size(0)*get_local_size(1)] \n"
"				= quadrantData[get_local_id(0) + get_local_id(1)*get_local_size(0) + i*get_local_size(0)*get_local_size(1)]; \n"
"	} \n"
"	barrier(CLK_LOCAL_MEM_FENCE); \n"
"	if(xid >= width || yid >= height ) \n"
"		return; \n"
"	float defocus = dfstart + xid*dfstep; \n"
"	float astig = astigstart + yid*astigstep; \n"
"	float s1 = 0.0f; \n"
"	float s2 = 0.0f; \n"
"	float s3 = 0.0f; \n"
"	for(int i = 1; i <= quadrantwidth; i++) \n"
"		for(int j = 1; j<= quadrantheight; j++) \n"
"		{ \n"
"			if(((quadrantwidth-i)*(quadrantwidth-i) + (quadrantheight-j)*(quadrantheight-j))> noincluderegion*noincluderegion) \n"
"			{ \n"
"				if(localQuadrant[(i-1)+quadrantwidth*(j-1)]*localQuadrant[(i-1)+quadrantwidth*(j-1)]!=0) \n"
"				{ \n"
"					float k = sqrt((quadrantwidth-i)*(quadrantwidth-i)*pixelscale*pixelscale + (quadrantheight-j)*(quadrantwidth-j)*pixelscale*pixelscale); \n"
"				//	float val = fabs(-sin(2*3.1415926f*((wavelength/2.0f)*defocus*k*k + (wavelength/2.0f)*astig*k*k*cos(2.0f*(atan2(1*(quadrantheight+0.001f - j), -1*(quadrantwidth+0.001f - i)))) + ((wavelength*wavelength*wavelength)/4)*Cs*k*k*k*k))); \n"
"					float val = sin(2*3.1415926f*((wavelength/2.0f)*defocus*k*k + (wavelength/2.0f)*astig*k*k*cos(2.0f*(atan2(1*(quadrantheight+0.001f - j), -1*(quadrantwidth+0.001f - i)))) + ((wavelength*wavelength*wavelength)/4)*Cs*k*k*k*k)); \n"
"					float val2 = val*val - 0.5f; \n"
"					s1 += (val2 * localQuadrant[(i-1)+quadrantwidth*(j-1)]); \n"
"					s2 +=  val2*val2; \n"
"					s3 +=  localQuadrant[(i-1)+quadrantwidth*(j-1)]*localQuadrant[(i-1)+quadrantwidth*(j-1)]; \n"
"				} \n"
"			} \n"
"		} \n"
"	\n"
"	result[xid + yid * width] = s1/sqrt(s2*s3); \n"		
"} \n"
;

const char* ImageCutSource = 
"__kernel void clImageCut(__global const float* Input, __global float2* Output, int fullwidth, int fullheight, int newwidth, int newheight, int startx, int starty) \n"
"{		\n"
"	//Get the work items ID \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	if(xid < newwidth && yid < newheight) \n"
"	{	\n"
"		int Index = xid + yid*newwidth; \n"
"		int xid2 = startx + xid; \n"
"		int yid2 = starty + yid; \n"
"		int Index2 = xid2 + yid2 * fullwidth; \n"
"		Output[Index].x = Input[Index2]; \n"
"		Output[Index].y = 0; \n"
"	}	\n"
"}	\n"
;

const char* PeriodogramAverageSource = 
"__kernel void clPeriodogramAverage(__global const float2* Input, __global float2* Output, int width, int height, int total, int first) \n"
"{		\n"
"	//Get the work items ID \n"
"	int xid = get_global_id(0);	\n"
"	int yid = get_global_id(1); \n"
"	if(xid < width && yid < height) \n"
"	{	\n"
"		int Index = xid + yid*width; \n"
"		if(first == 1) \n"
"		{ \n"
"			Output[Index].x = 0; \n"
"			Output[Index].y = 0; \n"
"		} \n"
"		Output[Index].x += (Input[Index].x); \n"
"		Output[Index].y += (Input[Index].y); \n"
"	}	\n"
"}	\n"
;

