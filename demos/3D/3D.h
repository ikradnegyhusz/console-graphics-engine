/*
FOR IT TO WORK:
define these values in main:

    #define SW 256
    #define SH 144
    #define PIXEL_W 4
    #define PIXEL_H 4
    #define ZBUF_DELTA_DIV 2

*/
#define ZBUF_DELTA_DIV 2
#include "math.h"
#ifndef FUNCTIONS_H
#include "functions.h"
#endif
#define PI 3.14159265359
graphicConsole gc;
float* zbuffer;
float zbuf_base=20.0f;
float z_ax;

float axes_list[SW*SH];
float cos_axes[360];
float sin_axes[360];

float rad(float x)
{
    return PI*x/180;
}

typedef struct POINT_3D
{
    float x, y, z;
}POINT_3D;

typedef struct POINT_3Di
{
    int x, y, z;
}POINT_3Di;

typedef struct POINT_2D
{
    float x,y;
}POINT_2D;

float Hyp(float x, float y, float z)
{
    return sqrt(x*x+y*y+z*z);
}

char inBounds(float x, float y)
{
    return (int)x<SW && (int)x>=0 && (int)y<SH && (int)y>=0;
}

float limit(float x,float min,float max)
{
	if (x<min) return min;
	else if (x>max) return max;
	return x;
}

void init_values()
{
	//perspective
	float mid_x = SW/2;
	float mid_y = SH/2;		
	for(int i=0;i<SW;i++)
	{
		for(int j=0;j<SH;j++)
		{
			
			float x=(float)i;
			float y=(float)j;
			/*
			float dx = mid_x-x;
			float dy = mid_y-y;
			float hyp = Hyp(dx,dy,0);
			if(hyp!=0)
			{
				dx/=hyp;
				dy/=hyp;
			}
			float adx=x+1-dx;
			float ady=y-dy;
			float a = Hyp(adx,ady,0);
			float k = (a-2)/(-2);
			if(k<0) k=0;
			float alpha = acos(rad(k));
			if(alpha<0) alpha=360+alpha;
			axes_list[i+j*SW]=alpha;
			*/
			//axes_list[i+j*SW] = 45.0f+(x/SW)*90.0f + (y/SH) * ((x/SW)*180.0f-90.0f);
			//axes_list[i+j*SW] = 45.0f+(x/SW)*90.0f + ((x/SW)-0.5) * 90.0f * (y/SH);
			//printf("%f\n",axes_list[i+j*SW]);
			axes_list[i+j*SW]=45.0f;
		}
	}
	//sine and cosine for all axes (0 - 360)
	for(int i=0;i<360;i++)
	{
		cos_axes[i]=cos(rad(i));
		sin_axes[i]=sin(rad(i));
	}
	//light values
	
}

void rotate3D(POINT_3D* point, float angle, char axis)
{
    float x=point->x;
    float y=point->y;
    float z=point->z;
	int angle_index=list_overflow(list_negative((int)angle,360),360);
    //float cosa=cos(rad(angle));
    //float sina=sin(rad(angle));
	float cosa=cos_axes[angle_index];
    float sina=sin_axes[angle_index];
    
    switch(axis)
    {
        case 'z':
            point->x = x*cosa - y*sina;
            point->y = y*cosa + x*sina;
            break;
        case 'y':
            point->x = x*cosa + z*sina;
            point->z = z*cosa - x*sina;
            break;
        case 'x':
            point->y = y*cosa - z*sina;
            point->z = z*cosa + y*sina;
            break;
    }
}

void line3D(POINT_3D A, POINT_3D B)
{
	float resolution=2.0f;
    POINT_3D PIXEL={A.x,A.y,A.z};
    POINT_2D drawPIXEL;
    //from A to B
    float dx=B.x-A.x;
    float dy=B.y-A.y;
    float dz=B.z-A.z;
    float hyp=Hyp(dx,dy,dz);
    //normalize
    dx/=hyp;
    dy/=hyp;
    dz/=hyp;
    //loop
    //float cos_a=cos(rad(z_ax));
    //float sin_a=sin(rad(z_ax));
    float cos_a,sin_a;
	
	wchar_t char_set[6]={L' ',L'.',L'+',176,177,178};
	wchar_t ch;
	WORD attr=0;
	WORD color=gc.drawChar.Attributes;
	while(1)
    {
		int angle_index=list_overflow(list_negative((int)z_ax,360),360);
		cos_a=cos_axes[angle_index];
		sin_a=sin_axes[angle_index];
		
        //draw the pixel
        drawPIXEL.x=PIXEL.x+PIXEL.z*cos_a*0.5;
        drawPIXEL.y=PIXEL.y+PIXEL.z*sin_a*0.5;
        int drawIndex=(int)drawPIXEL.x+(int)drawPIXEL.y*SW;
        if(!inBounds(drawPIXEL.x,drawPIXEL.y)) goto check_end;
        //draw if z value is smaller than in z buffer
        if(PIXEL.z <= zbuffer[drawIndex])
        {
            gc.pixel(drawPIXEL.x, drawPIXEL.y);
            zbuffer[drawIndex]=PIXEL.z;
        }
        //check end
        check_end:
        float line_dx = PIXEL.x-A.x;
        float line_dy = PIXEL.y-A.y;
        float line_dz = PIXEL.z-A.z;
        if( Hyp(line_dx,line_dy, line_dz) >= hyp ) break;
        //change PIXEL values
        PIXEL.x+=dx/resolution;
        PIXEL.y+=dy/resolution;
        PIXEL.z+=dz/resolution;
    }
}

void triangle3D(POINT_3D A, POINT_3D B, POINT_3D C)
{
    POINT_3D END={B.x,B.y,B.z};
    //line start from A
    float dx=C.x-B.x;
    float dy=C.y-B.y;
    float dz=C.z-B.z;
    float hyp=Hyp(dx,dy,dz);
    //normalize
    dx/=hyp;
    dy/=hyp;
    dz/=hyp;
    //loop
    while(1)
    {
        //draw line
        line3D(A,END);
        //check for end
        float line_dx=END.x-B.x;
        float line_dy=END.y-B.y;
        float line_dz=END.z-B.z;
        if( Hyp(line_dx,line_dy, line_dz) >= hyp) break;
        //change END point
        END.x+=dx/ZBUF_DELTA_DIV;
        END.y+=dy/ZBUF_DELTA_DIV;
        END.z+=dz/ZBUF_DELTA_DIV;
    }
}

POINT_3D shift3D(POINT_3D point, float x, float y,float z)
{
    POINT_3D ret;
    ret.x=point.x+x;
    ret.y=point.y+y;
    ret.z=point.z+z;
    return ret;
}

void resetZbuf()
{
    for(int i=0;i<SW*SH;i++)
        zbuffer[i]=zbuf_base;
}