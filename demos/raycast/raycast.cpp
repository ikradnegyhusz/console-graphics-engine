#include "../../include/consoleGraphics.h"
#include "def.h"
#define SW 128
#define SH 72

int main()
{
    BOOL topdown=FALSE;
    xya player={SW/2,SH/2,0.0f};
    graphicConsole gc;
    
    gc.createConsole(SW,SH,6,6);
    gc.setColor(0x00FF00);
    while(1)
    {
        if(gc.key(27)) return 0;
        if(gc.key(9)) topdown=TRUE;
        else topdown=FALSE;
        //controls
        if(gc.key(37)) player.a-=1;
        if(gc.key(39)) player.a+=1; 

        gc.write(0,0,"test");

        if(topdown)
        {
            gc.pixel(player.x,player.y);
            gc.pixel(player.x+COS(player.a),player.y+SIN(player.a));
        }
        gc.render();
    }
    return 0;
}