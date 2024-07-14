#pragma once
#include <windows.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <stdlib.h>
#include <algorithm>

//GetAsyncKeyState

int getColor(int hexValue)
{
	//first determine the composition of the colors in a binary form
	int composition[3]={0,0,0};
	
	int rgb[3] = {((hexValue >> 16) & 0xFF),((hexValue >> 8) & 0xFF),((hexValue) & 0xFF)};
	int RGB[3] = {((hexValue >> 16) & 0xFF),((hexValue >> 8) & 0xFF),((hexValue) & 0xFF)};
	
	int max_index;
	int absolute_max_value;
	int range;
	for(int i=0;i<3;i++)
	{
		max_index = std::distance(RGB, std::max_element(RGB, RGB + 3));
		if(i==0)
		{
			composition[max_index]=1;
			absolute_max_value=RGB[max_index];
			range=(int)(absolute_max_value/2);
		}
		else if((RGB[max_index]>absolute_max_value-range) && (RGB[max_index]<absolute_max_value+range))
		{
			composition[max_index]=1;
		}
		RGB[max_index]=-1;
	}
	//each composition corresponds to a specific color the command line can visualize
	//have to determine which color it is
	int color;
	int colors[9][4]={{0,0,0,0x0000},
		{0,0,1,0x0010},
		{0,1,0,0x0020},
		{0,1,1,0x0030},
		{1,0,0,0x0040},
		{1,0,1,0x0050},
		{1,1,0,0x0060},
		{1,1,1,0x0070}};
	for(int i=0;i<9;i++)
	{
		if(colors[i][0]==composition[0] and colors[i][1]==composition[1] and colors[i][2]==composition[2])
		{
			color=colors[i][3];
		}
	}
	return color;
}

using TimePoint = std::chrono::time_point<std::chrono::system_clock, 
    std::chrono::duration<double>>;

class graphicConsole
{
    public:
		CHAR_INFO drawChar;
        graphicConsole()
        {
            w=128;
            h=72;
            wxh=w*h;
            appName=L"console app";
            console = GetStdHandle(STD_OUTPUT_HANDLE);
            consoleIn = GetStdHandle(STD_INPUT_HANDLE);
        };

        int createConsole(int width, int height, int fontw, int fonth)
        {
            w=width;
            h=height;
            wxh=w*h;
            COORD whcoord = {(short)w,(short)h};
            console=CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,0,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
            SetConsoleScreenBufferSize(console,whcoord);
            SetConsoleActiveScreenBuffer(console);
            //cursor
            CONSOLE_CURSOR_INFO cursorinfo;
            cursorinfo.bVisible = FALSE;
            cursorinfo.dwSize=1;
            SetConsoleCursorInfo(console, &cursorinfo);
            //font
            CONSOLE_FONT_INFOEX cfi;
            cfi.cbSize = sizeof(cfi);
            cfi.nFont = 0;
            cfi.dwFontSize.X = fontw;
            cfi.dwFontSize.Y = fonth;
            cfi.FontFamily = FF_DONTCARE;
            cfi.FontWeight = FW_NORMAL;
            wcscpy_s(cfi.FaceName, L"Consolas");
            SetCurrentConsoleFontEx(console, false, &cfi);
            //console window
            windowrect={ 0, 0, (short)(w-1), (short)(h-1)};
            SetConsoleWindowInfo(console, TRUE, &windowrect);
            //input
            SetConsoleMode(consoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
            //screen
            screen=new CHAR_INFO[wxh];
            memset(screen, 0, sizeof(CHAR_INFO) * wxh);
            return 1;
        }

        void pixel(int x, int y)
        {
            screen[x+y*w].Char.UnicodeChar=drawChar.Char.UnicodeChar;
            screen[x+y*w].Attributes=drawChar.Attributes;
        }
		
		void character(int x, int y, char c)
		{
			screen[x+y*w].Char.UnicodeChar=c;
            screen[x+y*w].Attributes=0x000F; //FG white
		}

        void write(int x, int y, const char* str)
        {
            int i=0;
            while(str[i]!='\0')
            {
                character(x+i,y,str[i]);
                i+=1;
            }
        }
		
        float getFPS()
        {
            tp2 = std::chrono::system_clock::now();
			std::chrono::duration<float> SecPerFrame = tp2 - tp1;
            tp1=tp2;
            int sleepAmount=sleepAmount=(int)((1.0f/FPS_LIMIT - SecPerFrame.count())*1000);
            if(sleepAmount>=0) Sleep(sleepAmount);
            return 1.0f/SecPerFrame.count();
        }

        void clear(int val)
        {
            memset(screen, val, sizeof(CHAR_INFO) * wxh);
        }

        int render()
        { 
            float FPS=getFPS();
            wchar_t s[256];
			swprintf_s(s, 256, L"%s - FPS: %f", appName.c_str(),FPS);
			SetConsoleTitleW(s);
            WriteConsoleOutput(console, screen, { (short)w, (short)h }, { 0,0 }, &windowrect);
			clear(0);
            return 1;
        }
		
		void setColor(int hexValue)
		{
			int color=getColor(hexValue);
			int rgb[3] = {((hexValue >> 16) & 0xFF),((hexValue >> 8) & 0xFF),((hexValue) & 0xFF)};
			int RGB[3] = {((hexValue >> 16) & 0xFF),((hexValue >> 8) & 0xFF),((hexValue) & 0xFF)};
			//now to determine the brightness of the color, add the RGB values together
			int brightness = rgb[0]+rgb[1]+rgb[2] - 383; //goes from -383 to 382
			wchar_t amplifying_levels[6]={L' ',L'.',L'+',176,177,178};
			char bright = (brightness>=0);
			int deviation_from_mid=std::abs(brightness);
			if(bright)
			{
				drawChar.Attributes=color|0x000F; //foreground white
			}
			else
			{
				drawChar.Attributes=color; //foreground black
			}
			int level = (int)(deviation_from_mid/63);
			drawChar.Char.UnicodeChar=amplifying_levels[level];
		}
		
		BOOL key(int value)
		{
			if (GetAsyncKeyState(value)) return TRUE;
			else return FALSE;
		}

    protected:
        HANDLE console;
        HANDLE consoleIn;
        CHAR_INFO *screen;
        SMALL_RECT windowrect;
        std::wstring appName;
        int w,h,wxh;
        TimePoint tp2;
        TimePoint tp1;
        float FPS_LIMIT=100.0f;
};