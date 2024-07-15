#define PI 3.14159265359
#include "math.h"
#include <cmath>
#include "stdio.h"
#define RESOLUTION 5
#define SW 32*RESOLUTION
#define SH 18*RESOLUTION

typedef struct
{
    float x,y,a;
}xya;

typedef struct
{
    float x,y;
}V;

typedef struct
{
    V A,B;
}LINE;

typedef struct
{
    float x,y,d;

}RAY;

float rad(float x)
{
    return x*PI/180;
}

float COS(float x)
{
    return cos(rad(x));
}

float SIN(float x)
{
    return sin(rad(x));
}

float ABS(float x)
{
    if(x<0)return -x;
    else return x;
}

float angle_limit(float a)
{
    int k=(int)ABS(a)%360;
    if(a<0) a=360-k;
    else a=k;
    return a;
}

void draw_line(graphicConsole& gc, V A, V B)
{
    float dx=B.x-A.x;
    float dy=B.y-A.y;
    float len=sqrt(dx*dx+dy*dy);
    if(len==0) return;
    float ax=dx/len;
    float ay=dy/len;
    V temp={A.x,A.y};
    float temp_dx=0;
    float temp_dy=0;
    float temp_len=0;
    while(temp_len<=len)
    {
        gc.pixel(temp.x,temp.y);
        temp.x+=ax;
        temp.y+=ay;
        temp_dx=temp.x-A.x;
        temp_dy=temp.y-A.y;
        temp_len=sqrt(temp_dx*temp_dx+temp_dy*temp_dy);        
    }
}

void draw_rect(graphicConsole& gc,int x, int y, int width, int height)
{
    for(int i=x;i<width+x;i++)
    {
        for(int j=y;j<height+y;j++)
        {
            gc.pixel(i,j);
        }
    }
}

char isnumeric(char c)
{
    if (c>=48&&c<=57) return 1;
    else return 0;
}

void clearstr(char* str)
{
    int i=0;
    while(1)
    {
        str[i]='\0';
        if(str[i+1]=='\0') break;
        i++;
    }
}

int readMap(const char* filepath, LINE** map)
{
    FILE* file=fopen(filepath,"r");
    char c;
    char buffer[64]="";
    int i=0;
    float values[4];
    int vindex=0;
    int linecount=0;
    LINE l;
    while(c!=EOF)
    {
        c=fgetc(file);

        if(c==' ') continue;
        else if(c==':'){i=0;continue;}
        
        if(c==',' || c==';'){
            buffer[i]='\0';
            if(isnumeric(buffer[0])){
                sscanf(buffer,"%f",&values[vindex]);}
            else{
                if(buffer[0]=='w') values[vindex]=SW-1;
                else if(buffer[0]=='h') values[vindex]=SH-1;
            }
            i=0;
            vindex++;
            if(c==',') continue;
        }
        if(c==';')
        {
            if(linecount==0){
                    *map=(LINE*)malloc(sizeof(LINE)*2);
            }
            else{
                *map=(LINE*)realloc((*map),sizeof(LINE)* (linecount+5) );
            }
            l.A.x=values[0];
            l.A.y=values[1];
            l.B.x=values[2];
            l.B.y=values[3];
            (*map)[linecount]=l;
            linecount++;
            vindex=0;
            clearstr(buffer);
            continue;
        }
        buffer[i]=c;
        i++;
    }
    /*for(int i=0;i<linecount;i++)
    {
        printf("%f %f, %f %f\n",map[i].A.x,map[i].A.y,map[i].B.x,map[i].B.y);
    }*/
    return linecount;
}

V intersection(V A, V B, V C, V D) {
    float a1 = B.y - A.y;
    float b1 = A.x - B.x;
    float c1 = a1 * A.x + b1 * A.y;

    float a2 = D.y - C.y;
    float b2 = C.x - D.x;
    float c2 = a2 * C.x + b2 * C.y;

    float determinant = a1 * b2 - a2 * b1;

    if (determinant == 0) {
        return V{-1,-1};
    } else {
        float x = (b2 * c1 - b1 * c2) / determinant;
        float y = (a1 * c2 - a2 * c1) / determinant;
        return V{x, y};
    }
}

int isOnSegment(V A, V B, V P) {
    return (P.x <= std::max(A.x, B.x) && P.x >= std::min(A.x, B.x) &&
            P.y <= std::max(A.y, B.y) && P.y >= std::min(A.y, B.y));
}

RAY cast_ray(float x, float y, float a, LINE* map, int nlines)
{
    V A={x,y};
    V B={x+(SW+SH)*COS(a),y+(SW+SH)*SIN(a)};
    float mindist=SW*SH;
    V P;
    RAY r;
    for(int i=0;i<nlines;i++)
    {
        V C=map[i].A;
        V D=map[i].B;
        V p=intersection(A,B,C,D);
        if(p.x==-1) continue;
        if(!isOnSegment(A,B,p)) continue;
        float dx=p.x-x;
        float dy=p.y-y;
        float dist=sqrt(dx*dx+dy*dy);
        if(dist<mindist)
        {
            mindist=dist;
            P=p;
        }
    }
    r.x=P.x;
    r.y=P.y;
    r.d=mindist;
    return r;
}
