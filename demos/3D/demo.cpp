#include "..\..\include\consoleGraphics.h"
#define SW 256
#define SH 144
#define PIXEL_W 4
#define PIXEL_H 4
#include "3D.h"
#include "obj.h"

int main(int argc, char* args[])
{
    //init variables
	init_values();
    char axis='z';
	char rotate = 0;
    float scale=5.0f;
    float ox=50.0f;
    float oy=50.0f;
    float oz=0.0f;

    //init Zbuffer
    zbuffer=(float*)malloc(sizeof(float)*SW*SH);
    resetZbuf();

    //Create and load object
	OBJ testobj;
	if(args[1]) testobj = load_obj(args[1]);
	else testobj = load_obj("objs/cube.obj");
	
	int colors[]={0xFF0000,0x00FF00,0xFFFF00,0x111111,0x000011,0xFF00FF};
	obj_SetColors(&testobj,colors);

    gc.createConsole(SW,SH,PIXEL_W, PIXEL_H);
    while(1)
    {
        if(GetAsyncKeyState(27)) return 0;

        if(GetAsyncKeyState('X')) axis='x';
        else if(GetAsyncKeyState('Y')) axis='y';
        else if(GetAsyncKeyState('Z')) axis='z';

        if(GetAsyncKeyState('E')) scale+=0.5;
        else if(GetAsyncKeyState('Q')) scale-=0.5;
        
        if(GetAsyncKeyState('W')) oy-=1;
        else if(GetAsyncKeyState('S')) oy+=1;
        if(GetAsyncKeyState('A')) ox-=1;
        else if(GetAsyncKeyState('D')) ox+=1;
        
        if(GetAsyncKeyState('R')) oz+=1;
        if(GetAsyncKeyState('F')) oz-=1;
		
        if(GetAsyncKeyState(32)) rotate= !rotate;
		
		if(GetAsyncKeyState(37)) objRotate(&testobj,1,axis);
		if(GetAsyncKeyState(39)) objRotate(&testobj,-1,axis);

        obj3D(testobj,ox,oy,oz,scale);
		if(rotate) objRotate(&testobj,1,axis);
        gc.render();
        resetZbuf();
    }
    return 0;
}