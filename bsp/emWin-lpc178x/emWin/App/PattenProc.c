#include "PattenProc.h"


#define M_PI 3.14159265359
#define PRECISION 10

static double sqrt(double b)
{
 	double x = 1;
	int step = 0;

 	while ((x*x-b<-0.000000001 || x*x-b>0.000000001) && step<50)
 	{
		x = (b/x+x)/2.0;
		step++;
	}
 	return x;
}

static double arctan(double x)
{
  double x2, result, addPart = 0;
	int half_theta = 0;
	int negative_sign = 0;
	
	if ((x > 1) || (x < -1))//use half-angle formula of the arctan
	{
		x = x / (1 + sqrt(1+x*x));
		half_theta = 1;
	}
	// now |x| < 1
	
	if (x < 0)//because arctan function is odd function
	{
		x = -x;
		negative_sign = 1;
	}
	//now 0<= x <=1
	
	if (x > 0.75)//
	{
		addPart = 0.643501108;//arctan(0.75)
		x = (x-0.75) / (1+0.75*x);
	}
	else if (x > 0.5)
	{
		addPart = 0.463647609;//arctan(0.5)
		x = (x-0.5) / (1+0.5*x);
	}
	else if (x > 0.25)
	{
		addPart = 0.244978663;//arctan(0.25)
		x = (x-0.25) / (1+0.25*x);
	}
	//now 0<= x <=0.25
	
	x2 = x * x;
	result = 0;
	
#if (PRECISION >= 10)
    result -= 1./15.;
    result *= x2;
#endif
#if (PRECISION >= 8)
    result += 1./13.;
    result *= x2;
#endif
#if (PRECISION >= 7)
    result -= 1./11.;
    result *= x2;
#endif
#if (PRECISION >= 6)
    result += 1./9.;
    result *= x2;
#endif
#if (PRECISION >= 5)
    result -= 1./7.;
    result *= x2;
#endif
    result += 1./5;
    result *= x2;

    result -= 1./3;
    result *= x2;

    result += 1;
	result *= x;
		
	result += addPart;
	if (negative_sign == 1)
		result = -result;
	if (half_theta == 1)
		result = 2 * result;
		
	return result;
}


static double arccos(double x)
{
	if (x == -1){
		return M_PI;
	}else if(x < 0){ 
		return M_PI-arctan(sqrt(1-x*x)/(-x));
	}else if(x == 0){
		return M_PI/2;
	}else if (x < 1){ 
		return arctan(sqrt(1-x*x)/x);
	}else{ 
		return 0;
	} 
}


// double arccos(double x)//the x is smaller,the precision of result is higher
// {
//     double x2, result;
// 	
// 	  if (x >= 1)
// 			return 0;
// 		if (x <= -1)
// 			return M_PI;

// 	  /*raccosx = PI/2 - racsinx, calculate the racsinx first*/
//     x2 = x * x;
//     result = 0;
// 		
//     result += (1.*3*5*7*9*11*13*15*17)/(2.*4*6*8*10*12*14*16*18*19);
//     result *= x2;
// 		
//     result += (1.*3*5*7*9*11*13*15)/(2.*4*6*8*10*12*14*16*17);
//     result *= x2;
// 		
//     result += (1.*3*5*7*9*11*13)/(2.*4*6*8*10*12*14*15);
//     result *= x2;

//     result += (1.*3*5*7*9*11)/(2.*4*6*8*10*12*13);
//     result *= x2;

//     result += (1.*3*5*7*9)/(2.*4*6*8*10*11);
//     result *= x2;

//     result += (1.*3*5*7)/(2.*4*6*8*9);
//     result *= x2;
// 		
//     result += (1.*3*5)/(2.*4*6*7);
//     result *= x2;

//     result += (1.*3)/(2.*4*5);
//     result *= x2;

//     result += 1./(1.*2*3);
//     result *= x2;

// 		result += 1;
//     result *= x;

//     result = M_PI/2 - result;

//     return result;
// }

/*在屏幕坐标系中（x轴向右，y轴向下），计算patten一段弧线的起始角度和终止角度，前者必须小于后者
该数据可以直接用于emWin绘图，{{这里输入数据f是在屏幕坐标系中}}*/
void CalculateArcScreen(PattenFragment_t *f, float *pR, float *pDegSt, float *pDegEnd)
{
	double k, theta;
	float deg1, deg2;
	//Attention : This calculation is excutting in screen coordinates system

	*pR = sqrt((f->x1-f->xo)*(f->x1-f->xo)+(f->y1-f->yo)*(f->y1-f->yo));
	k = (f->x1 - f->xo) / sqrt((f->x1-f->xo)*(f->x1-f->xo)+(f->y1-f->yo)*(f->y1-f->yo));
	theta = arccos(k);
	deg1 = theta * 180 / M_PI;
	if (f->y1-f->yo > 0)//lower half-plane,because it's in screen coordinate system
	{
		deg1 = -deg1;
	}
	k = (f->x2 - f->xo) / sqrt((f->x2-f->xo)*(f->x2-f->xo)+(f->y2-f->yo)*(f->y2-f->yo));
	theta = arccos(k);
	deg2 = theta * 180 / M_PI;
	if (f->y2-f->yo > 0)//lower half-plane,because it's in screen coordinate system
	{
		deg2 = -deg2;
	}
	
	if ((deg1>=0) && (deg2>=0))//the same sign
	{
		if (f->cDir == 0){//left or down
			if (deg1 > deg2){
				*pDegSt = deg1 - 360;
				*pDegEnd = deg2;
			}else{
				*pDegSt = deg2 - 360;
				*pDegEnd = deg1;
			}
		}else{            //right or up
			if (deg1 > deg2){
				*pDegSt = deg2;
				*pDegEnd = deg1;
			}else{
				*pDegSt = deg1;
				*pDegEnd = deg2;
			}
		}
	}
	else if ((deg1<0) && (deg2<0))
	{
		if (f->cDir == 0){//left or down
			if (deg1 > deg2){
				*pDegSt = deg2;
				*pDegEnd = deg1;
			}else{
				*pDegSt = deg1;
				*pDegEnd = deg2;
			}					
		}else{            //right or up
			if (deg1 > deg2){
				*pDegSt = deg1;
				*pDegEnd = deg2 + 360;
			}else{
				*pDegSt = deg2;
				*pDegEnd = deg1 + 360;
			}
		}
	}
	else if ((deg1>=0) && (deg2<0))
	{
		if (f->cDir == 0){//left or down
				*pDegSt = deg1;
				*pDegEnd = deg2 + 360;				
		}else{            //right or up
				*pDegSt = deg2;
				*pDegEnd = deg1;
		}
	}
	else if ((deg1<0) && (deg2>=0))
	{
		if (f->cDir == 0){//left or down
				*pDegSt = deg2;
				*pDegEnd = deg1 + 360;				
		}else{            //right or up
				*pDegSt = deg1;
				*pDegEnd = deg2;
		}
	}
}

/*在数学中直角坐标系中（x轴向右，y轴向上），这也是制图软件使用的坐标系，计算patten一段弧线的起始角度和终止角度，
前者必须小于后者，该数据不可以直接用于emWin绘图，和在屏幕坐标绘图的主要区别是
对于arccos计算角度处于上半平面还是下半平面的区分，{{这里输入数据f是在数学直角坐标系中}}*/
void CalculateArcMath(PattenFragment_t *f, float *pR, float *pDegSt, float *pDegEnd)
{
	double k, theta;
	float deg1, deg2;

	*pR = sqrt((f->x1-f->xo)*(f->x1-f->xo)+(f->y1-f->yo)*(f->y1-f->yo));
	k = (f->x1 - f->xo) / sqrt((f->x1-f->xo)*(f->x1-f->xo)+(f->y1-f->yo)*(f->y1-f->yo));
	theta = arccos(k);
	deg1 = theta * 180 / M_PI;
	if (f->y1-f->yo < 0)//lower half-plane
	{
		deg1 = -deg1;
	}
	k = (f->x2 - f->xo) / sqrt((f->x2-f->xo)*(f->x2-f->xo)+(f->y2-f->yo)*(f->y2-f->yo));
	theta = arccos(k);
	deg2 = theta * 180 / M_PI;
	if (f->y2-f->yo < 0)//lower half-plane
	{
		deg2 = -deg2;
	}

	if ((deg1>=0) && (deg2>=0))//the same sign
	{
		if (f->cDir == 0){//left or down
			if (deg1 > deg2){
				*pDegSt = deg1 - 360;
				*pDegEnd = deg2;
			}else{
				*pDegSt = deg2 - 360;
				*pDegEnd = deg1;
			}
		}else{            //right or up
			if (deg1 > deg2){
				*pDegSt = deg2;
				*pDegEnd = deg1;
			}else{
				*pDegSt = deg1;
				*pDegEnd = deg2;
			}
		}
	}
	else if ((deg1<0) && (deg2<0))
	{
		if (f->cDir == 0){//left or down
			if (deg1 > deg2){
				*pDegSt = deg2;
				*pDegEnd = deg1;
			}else{
				*pDegSt = deg1;
				*pDegEnd = deg2;
			}					
		}else{            //right or up
			if (deg1 > deg2){
				*pDegSt = deg1;
				*pDegEnd = deg2 + 360;
			}else{
				*pDegSt = deg2;
				*pDegEnd = deg1 + 360;
			}
		}
	}
	else if ((deg1>=0) && (deg2<0))
	{
		if (f->cDir == 0){//left or down
			*pDegSt = deg1;
			*pDegEnd = deg2 + 360;				
		}else{            //right or up
			*pDegSt = deg2;
			*pDegEnd = deg1;
		}
	}
	else if ((deg1<0) && (deg2>=0))
	{
		if (f->cDir == 0){//left or down
			*pDegSt = deg2;
			*pDegEnd = deg1 + 360;				
		}else{            //right or up
			*pDegSt = deg1;
			*pDegEnd = deg2;
		}
	}
}
/************************************************************************/
/* 这里输入数据f已经处于{{屏幕坐标系}}中，直接绘制                        */
/************************************************************************/
void DrawPatten(PattenFragment_t frag[], int fragNum)
{
	int i;
	U8 style;
	float r, deg_start, deg_end;
	PattenFragment_t *f;
	for (i=0; i<fragNum; i++)
	{
		f = &frag[i];
		if (f->type == PATTEN_LINE)
		{
			GUI_DrawLine(f->x1, f->y1, f->x2, f->y2);
		}
		else if (f->type == PATTEN_ARC)
		{
			CalculateArcScreen(f, &r, &deg_start, &deg_end);
			GUI_DrawArc(f->xo, f->yo, r+1, r+1, (int)deg_start, (int)deg_end);
		}
		else if (f->type == PATTEN_DASH)
		{
			style = GUI_GetLineStyle();
			GUI_SetLineStyle(GUI_LS_DOT);
			GUI_DrawLine(f->x1, f->y1, f->x2, f->y2);
			GUI_SetLineStyle(style);
		}
	}
	
}
/************************************************************************/
/* 计算花样边界，输出边界保存在bd中，此处花样中坐标处于{{数学坐标系}}中
*/
/************************************************************************/
void CalculateBoundary(PattenFragment_t frag[], int fragNum, GUI_RECT *bd)
{
	int i;
	int bdLeft=65535, bdRight=0, bdUp=0, bdDown=65535;
	float r, degSt, degEnd;
	PattenFragment_t *f;
	for (i=0; i<fragNum; i++)
	{
		f = &frag[i];
//		if ((f->type == PATTEN_LINE) || (f->type == PATTEN_DASH) || (f->type == PATTEN_ARC))
		{
			if (f->x1 < bdLeft)
				bdLeft = f->x1;
			if (f->x2 < bdLeft)
				bdLeft = f->x2;
			if (f->x1 > bdRight)
				bdRight = f->x1;
			if (f->x2 > bdRight)
				bdRight = f->x2;
			if (f->y1 < bdDown)
				bdDown = f->y1;
			if (f->y2 < bdDown)
				bdDown = f->y2;
			if (f->y1 > bdUp)
				bdUp = f->y1;
			if (f->y2 > bdUp)
				bdUp = f->y2;
		}
		if (f->type == PATTEN_ARC)
		{
			CalculateArcMath(f, &r, &degSt, &degEnd);
			if (((-90 >= degSt) && (-90 <= degEnd)) || ((270 >= degSt) && (270 <= degEnd)))
			{
				if (f->yo-(int)r-1 < bdDown)
					bdDown = f->yo-(int)r-1;
			}
			if ((0 >= degSt) && (0 <= degEnd))
			{
				if (f->xo+(int)r+1 > bdRight)
					bdRight = f->xo+(int)r+1;
			}
			if ((90 >= degSt) && (90 <= degEnd))
			{
				if (f->yo+(int)r+1 > bdUp)
					bdUp = f->yo+(int)r+1;
			}
			if ((180 >= degSt) && (180 <= degEnd))
			{
				if (f->xo-(int)r-1 < bdLeft)
					bdLeft = f->xo-(int)r-1;
			}
		}
	}
	bd->x0 = bdLeft;
	bd->y0 = bdUp;
	bd->x1 = bdRight;
	bd->y1 = bdDown;
	return;
}
/************************************************************************/
/* 根据给定的边界矩形框bd，将{{数学坐标系}}花样frag所有坐标减去左边边界和下边边界，
相当于将花样边界左下点移到原点处
*/
/************************************************************************/
void MoveToOrigin(PattenFragment_t frag[], int fragNum, GUI_RECT *bd)
{
	int i;
	PattenFragment_t *f;
	int bdLeft = bd->x0;
	int bdDown = bd->y1;
	
	for (i=0; i<fragNum; i++)
	{
		f = &frag[i];
		f->x1 -= bdLeft;
		f->x2 -= bdLeft;
		f->xo -= bdLeft;
		f->y1 -= bdDown;
		f->y2 -= bdDown;
		f->yo -= bdDown;
	}

	return;
}
/************************************************************************/
/* 这个函数中花样frag被从数学坐标系中转换为屏幕坐标系中，结果保存在frag中，posx和posy是
花样边界左下点相对于canvasRect左下点的偏移位置，即posx和posy是数学坐标系下值
*/
/************************************************************************/
void ChangePosition(PattenFragment_t frag[], int fragNum, GUI_RECT *canvasRect, int posx, int posy)
{
	int i;
	PattenFragment_t *f;
	for (i=0; i<fragNum; i++)
	{
		f = &frag[i];
		f->x1 = (f->x1 + posx) + canvasRect->x0;
		f->x2 = (f->x2 + posx) + canvasRect->x0;
		f->xo = (f->xo + posx) + canvasRect->x0;
		f->y1 = canvasRect->y1 - (f->y1 + posy);
		f->y2 = canvasRect->y1 - (f->y2 + posy);
		f->yo = canvasRect->y1 - (f->yo + posy);
	}
}
/************************************************************************/
/* 在数学坐标系中，从原点处为中心开始放大，最好事先将花样左下点移到原点处
*/
/************************************************************************/
void MagnifyPatten(PattenFragment_t frag[], int fragNum, float mag)
{
	int i;
	PattenFragment_t *f;
	for (i=0; i<fragNum; i++)
	{
		f = &frag[i];
		f->x1 = (int)(f->x1 * mag);
		f->x2 = (int)(f->x2 * mag);
		f->xo = (int)(f->xo * mag);
		f->y1 = (int)(f->y1 * mag);
		f->y2 = (int)(f->y2 * mag);
		f->yo = (int)(f->yo * mag);
	}
}

//这里dstRect是在屏幕坐标系中左上点和右下点，srcRect是在数学坐标系中左上点和右下点
float CalculateMagFactor(GUI_RECT *dstRect, GUI_RECT *srcRect)
{
	int dstWidth = dstRect->x1 - dstRect->x0;
	int dstHeight = dstRect->y1 - dstRect->y0;
	int srcWidth = srcRect->x1 - srcRect->x0;
	int srcHeight = srcRect->y0 - srcRect->y1;
	if ((float)dstWidth/dstHeight >= (float)srcWidth/srcHeight)
	{
		return (float)dstHeight/srcHeight;
	} 
	else
	{
		return (float)dstWidth/srcWidth;
	}
}