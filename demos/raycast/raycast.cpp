#include "../../include/consoleGraphics.h"
#include "def.h"
#define RAYNUM 120
#define FOV 60

int main()
{
    BOOL topdown=FALSE;
    xya player={SW/2,SH/2,0.0f};
    graphicConsole gc;
    char text_out[SW]="";
    LINE* map;
    int nlines=readMap("map.txt",&map);
    RAY* rays=(RAY*)malloc(sizeof(RAY)*RAYNUM);
    float segment_width=(float)SW/(float)FOV;


    gc.createConsole(SW,SH,13-RESOLUTION,13-RESOLUTION);
    gc.setColor(0x00FF00);
    while(1)
    {
        if(gc.key(27)) return 0;
        if(gc.key(9)) topdown=TRUE;
        else topdown=FALSE;
        //controls
        if(gc.key(37)) player.a-=2;
        if(gc.key(39)) player.a+=2;
        if(gc.key(38)){
            player.x+=COS(player.a);
            player.y+=SIN(player.a);
        } 
        if(gc.key(40)){
            player.x-=COS(player.a);
            player.y-=SIN(player.a);
        }
        player.a=angle_limit(player.a);
        //cast rays
        float anglemod=-(FOV/2);
        for(int i=0;i<RAYNUM;i++)
        {
            RAY r=cast_ray(player.x,player.y,player.a+anglemod,map,nlines);
            rays[i]=r;
            anglemod+=(float)FOV/(float)RAYNUM;
        }
        
        //render
        if(topdown)
        {
             //topdown 2D view
            draw_line(gc,V{player.x,player.y},V{player.x+RESOLUTION*COS(player.a),player.y+RESOLUTION*SIN(player.a)});
            for(int i=0;i<nlines;i++)
            {
                draw_line(gc,map[i].A,map[i].B);
            }
            for(int i=0;i<RAYNUM;i++)
            {
                draw_line(gc,V{player.x,player.y},V{rays[i].x,rays[i].y});
            }
            
        }
        else
        {
            //3D view
            for(int i=0;i<RAYNUM;i++)
            {
                draw_rect(gc,i*(segment_width-1),rays[i].d/2,segment_width,SH-rays[i].d);
            }
        }

        sprintf(text_out,"player pos:\n(%f,%f)",player.x,player.y);
        gc.write(0,0,text_out);
        gc.render();
    }
    return 0;
}