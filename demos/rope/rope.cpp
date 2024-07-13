#include "..\console_graphics\consoleGraphics.h"
#define SW 128
#define SH 72
graphicConsole gc;
#include "math.h"
float Hyp(float x, float y)
{
    return sqrt(x*x+y*y);
}

typedef struct POINT_2D
{
    float x,y;
}POINT_2D;

typedef struct ROPE
{
    POINT_2D* bits;
    int length;
}ROPE;

ROPE CreateRope(int l)
{
    ROPE r;
    r.bits=(POINT_2D*)malloc(sizeof(POINT_2D)*l);
    for(int i=0;i<l;i++)
    {
        r.bits[i].x=0;
        r.bits[i].y=i;
    }
    r.length=l;
    return r;
}

void renderRope(ROPE r,int x,int y)
{
    for(int i=0;i<r.length;i++)
    {
        gc.pixel(r.bits[i].x+x,r.bits[i].y+y);
    }
}

char RopeIntact(ROPE r)
{
    for(int i=0;i<r.length-1;i++)
    {
        float distx,disty;
        float hyp;
        distx=r.bits[i+1].x - r.bits[i].x;
        disty=r.bits[i+1].y - r.bits[i].y;
        hyp=Hyp(distx,disty);
        if((int)hyp>1)
        {
            return 0;
        }
    }
    return 1;
}

void RopeCorrect(ROPE* r, int index)
{
    while(!RopeIntact(*r))
    {
        float distx,disty;
        float hyp;
        //check before index
        for(int i=0;i<index;i++)
        {
            distx=r->bits[i+1].x - r->bits[i].x;
            disty=r->bits[i+1].y - r->bits[i].y;
            hyp=Hyp(distx,disty);
            if(hyp>1)
            {
                r->bits[i].x+=distx-distx/hyp;
                r->bits[i].y+=disty-disty/hyp;
            }
        }
        //check after index
        for(int i=index+1;i<r->length;i++)
        {
            distx=r->bits[i-1].x - r->bits[i].x;
            disty=r->bits[i-1].y - r->bits[i].y;
            hyp=Hyp(distx,disty);
            if(hyp>1)
            {
                r->bits[i].x+=distx-distx/hyp;
                r->bits[i].y+=disty-disty/hyp;
            }
        }
    }
}

void RopeDisplace(ROPE* r,int index,float dx,float dy)
{
    r->bits[index].x+=dx;
    r->bits[index].y+=dy;
    RopeCorrect(r,index);
}

void RopePlace(ROPE* r,int index, float x, float y)
{
    r->bits[index].x=x;
    r->bits[index].y=y;
    RopeCorrect(r,index);
}

void RopeGravity(ROPE* r)
{
    for(int i=0;i<r->length;i++)
    {
        RopeDisplace(r,i,0,0.5);
    }
}

int main()
{
    ROPE testrope;
    testrope=CreateRope(30);
    gc.createConsole(SW,SH,5,5);
    gc.setColor(0xFF0000);
    while(1)
    {
        if(GetAsyncKeyState(27)) return 0;
        if(GetAsyncKeyState('D'))
            RopeDisplace(&testrope,20,5,0);
        else if(GetAsyncKeyState('A'))
            RopeDisplace(&testrope,20,-5,0);
        if(GetAsyncKeyState('W'))
            RopeDisplace(&testrope,20,0,-5);
        else if(GetAsyncKeyState('S'))
            RopeDisplace(&testrope,20,0,5);

        RopeGravity(&testrope);
        RopePlace(&testrope,0,0,0);
        renderRope(testrope,(int)SW/2,20);
        gc.render();
    }
    return 0;
}