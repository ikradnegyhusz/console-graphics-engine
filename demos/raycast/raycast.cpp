#include "../../include/consoleGraphics.h"
#include "def.h"

int main()
{
    BOOL topdown=FALSE;
    xya player={SW/2,SH/2,0.0f};
    graphicConsole gc;
    char text_out[SW]="";
    LINE* map;
    int nlines=readMap("map.txt",&map);
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

        if(topdown)
        {
            draw_line(gc,V{player.x,player.y},V{player.x+RESOLUTION*COS(player.a),player.y+RESOLUTION*SIN(player.a)});
            for(int i=0;i<nlines;i++)
            {
                draw_line(gc,map[i].A,map[i].B);
            }
            
        }

        sprintf(text_out,"angle:\n%f",player.a);
        gc.write(0,0,text_out);
        gc.render();
    }
    return 0;
}