//REQUIRES 3D.h
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#ifndef FUNCTIONS_H
#include "functions.h"
#endif
/*
float limit(float x,float min,float max)
{
	if (x<min) return min;
	else if (x>max) return max;
	return x;
}
*/
int count(char* str,char c)
{
	int i=0;
	int counter=0;
	while(str[i]!='\0')
	{
		if(str[i]==c) counter++;
		i++;
	}
	return counter;
}

int len(char* str)
{
	int i=0;
	while(str[i]!='\0') i++;
	return i;
}

void clearstr(char* str)
{
	for(int i=0;i<len(str);i++)
	{
		str[i]='\0';
	}
}

typedef struct OBJ
{
    std::vector<POINT_3D> v;
    std::vector< std::vector<int> > f;
    WORD* colors;
    int vcount,fcount;
}OBJ;

OBJ load_obj(std::string path)
{
    OBJ obj;
    std::ifstream file;
    file.open(path);
    std::string line;
    char buffer[256]="";
    int bi=0;
    char mode=' ';
    
    int vcount=1;
    int fcount=1;
    while (std::getline(file, line))
    {
        mode=' ';
        for(int i=0; i<line.length(); i++)
        {
            if(line[i]=='#') break;
            else if(line[i]=='v' && line[i+1]==' ') {mode='v';continue;}
            else if(line[i]=='f' && line[i+1]==' ') {mode='f';continue;}

            buffer[bi]=line[i];
            bi++;
        }
        //process the buffer
        if(mode=='v')
        {
            //obj.v=(POINT_3D*)realloc(obj.v,sizeof(POINT_3D)*vcount);
			POINT_3D point;
            sscanf(buffer, "%f %f %f", &point.x,&point.y,&point.z);
            obj.v.push_back(point);
			//std::cout << obj.v[vcount-1].x << " " <<obj.v[vcount-1].y << " " << obj.v[vcount-1].z<<"\n";
            vcount+=1;
        }
        else if(mode=='f')
        {
            //obj.f=(std::vector<int>*)realloc(obj.f,sizeof(std::vector<int>)*fcount);
            
			char val[24]="";
			int val_index=0;
			std::vector<int> values;
			for (int i=0;i<len(buffer);i++)
			{
				val[val_index]=buffer[i];
				val_index++;
				if((buffer[i]==' '&&i!=0) || i==len(buffer)-1)
				{
					int val_abstract;
					sscanf(val, "%d/*/*",&val_abstract);
					values.push_back(val_abstract);
					clearstr(val);
					val_index=0;
					continue;
				}
			}
			obj.f.push_back(values);
            //std::cout << obj.f[fcount-1].x << " " <<obj.f[fcount-1].y << " " << obj.f[fcount-1].z<<"\n";
            fcount+=1;
        }
        for(int i=0;i<256;i++) buffer[i]='\0';
        bi=0;
    }
    file.close();
    obj.vcount=vcount-1;
    obj.fcount=fcount-1;
    obj.colors=(WORD*)malloc(sizeof(WORD)*fcount);
    return obj;
}

void obj_SetColors(OBJ* obj,int* colors)
{
    for(int i=0;i<obj->fcount;i++)
    {
        obj->colors[i]=colors[i];
    }
}

void objRotate(OBJ* obj,float angle,char axis)
{
    for(int i=0;i<obj->vcount;i++)
    {
        rotate3D(&obj->v[i],angle,axis);
    }
}

void obj3D(OBJ obj,float x,float y,float z,float scale)
{
	
    for(int i=0;i<obj.fcount;i++)
    {
		int size=(obj.f[i].size())-1;
        for(int k=0;k<size;k+=2)
		{
			POINT_3D A;
			POINT_3D B;
			POINT_3D C;
			
			int k1=list_overflow(k,obj.f[i].size());
			int k2=list_overflow(k+1,obj.f[i].size());
			int k3=list_overflow(k+2,obj.f[i].size());
			
			//printf("%d %d %d\n",k1,k2,k3);
			
			//0,1,2    2,3,0
			A.x=obj.v[ obj.f[i][k1] - 1].x*scale+x;
			A.y=obj.v[ obj.f[i][k1] - 1].y*scale+y;
			A.z=obj.v[ obj.f[i][k1] - 1].z*scale+z;

			B.x=obj.v[ obj.f[i][k2] - 1].x*scale+x;
			B.y=obj.v[ obj.f[i][k2] - 1].y*scale+y;
			B.z=obj.v[ obj.f[i][k2] - 1].z*scale+z;

			C.x=obj.v[ obj.f[i][k3] - 1].x*scale+x;
			C.y=obj.v[ obj.f[i][k3] - 1].y*scale+y;
			C.z=obj.v[ obj.f[i][k3] - 1].z*scale+z;
			
			//color shading
			/*
			if(light_value==0)
			{
				//light_value = (A.x+B.x+C.x)/3-200.0f;
				light_value = limit( (A.z+B.z+C.z)/3+90.0f, 0.0f,100.0f ); //between 0.0f 100.0f
				int char_index = abs((int)round((light_value/10.0f))-5);
				ch = char_set[char_index];
				if(light_value>55.0f) attr=FG_BLACK;
				else if(light_value<45.0f) attr=FG_WHITE;
				else ch=L' ';
				gc.setDrawChar(ch,obj.colors[i]|attr);
				
			}*/
			//gc.setDrawChar(L' ',obj.colors[i]);
			//gc.drawChar.Char.UnicodeChar=L' ';
			//gc.drawChar.Attributes=getColor(obj.colors[i]);
			gc.setColor(obj.colors[i]);
			triangle3D(A,B,C);
			//break;
		}
    }
}