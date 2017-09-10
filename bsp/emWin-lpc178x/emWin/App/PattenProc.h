#ifndef __PATTENPROC_H__
#define __PATTENPROC_H__
#include "GUI.h"

#define PATTEN_LINE  0
#define PATTEN_ARC   1
#define PATTEN_DASH  2
#define PATTEN_END   -1


typedef struct _fragment
{
	signed short type;
	signed short cDir;//the direction of arc
	int x1, y1;//start point
	int x2, y2;//end point
	int xo, yo;//the center of the arc
}PattenFragment_t;



void CalculateArcScreen(PattenFragment_t *f, float *pR, float *pDegSt, float *pDegEnd);
void CalculateArcMath(PattenFragment_t *f, float *pR, float *pDegSt, float *pDegEnd);
void CalculateBoundary(PattenFragment_t frag[], int fragNum, GUI_RECT *bd);
void MoveToOrigin(PattenFragment_t frag[], int fragNum, GUI_RECT *bd);
void ChangePosition(PattenFragment_t frag[], int fragNum, GUI_RECT *canvasRect, int posx, int posy);
void MagnifyPatten(PattenFragment_t frag[], int fragNum, float mag);
float CalculateMagFactor(GUI_RECT *dstRect, GUI_RECT *srcRect);

void DrawPatten(PattenFragment_t frag[], int fragNum);



#endif