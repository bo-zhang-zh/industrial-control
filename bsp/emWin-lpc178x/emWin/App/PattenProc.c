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

/*����Ļ����ϵ�У�x�����ң�y�����£�������pattenһ�λ��ߵ���ʼ�ǶȺ���ֹ�Ƕȣ�ǰ�߱���С�ں���
�����ݿ���ֱ������emWin��ͼ��{{������������f������Ļ����ϵ��}}*/
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

/*����ѧ��ֱ������ϵ�У�x�����ң�y�����ϣ�����Ҳ����ͼ���ʹ�õ�����ϵ������pattenһ�λ��ߵ���ʼ�ǶȺ���ֹ�Ƕȣ�
ǰ�߱���С�ں��ߣ������ݲ�����ֱ������emWin��ͼ��������Ļ�����ͼ����Ҫ������
����arccos����Ƕȴ����ϰ�ƽ�滹���°�ƽ������֣�{{������������f������ѧֱ������ϵ��}}*/
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
/* ������������f�Ѿ�����{{��Ļ����ϵ}}�У�ֱ�ӻ���                        */
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
/* ���㻨���߽磬����߽籣����bd�У��˴����������괦��{{��ѧ����ϵ}}��
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
/* ���ݸ����ı߽���ο�bd����{{��ѧ����ϵ}}����frag���������ȥ��߽߱���±߽߱磬
�൱�ڽ������߽����µ��Ƶ�ԭ�㴦
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
/* ��������л���frag������ѧ����ϵ��ת��Ϊ��Ļ����ϵ�У����������frag�У�posx��posy��
�����߽����µ������canvasRect���µ��ƫ��λ�ã���posx��posy����ѧ����ϵ��ֵ
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
/* ����ѧ����ϵ�У���ԭ�㴦Ϊ���Ŀ�ʼ�Ŵ�������Ƚ��������µ��Ƶ�ԭ�㴦
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

//����dstRect������Ļ����ϵ�����ϵ�����µ㣬srcRect������ѧ����ϵ�����ϵ�����µ�
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