#include <graphics.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <dos.h>
#include <bios.h>
#include <math.h>
#include <alloc.h>

/*定义常量*/
/*向上翻页移键*/
#define PAGEUP 0x4900
/*向下翻页移键*/
#define PAGEDOWN 0x5100
/*Escape键*/
#define ESC 0x011b 
/*左移键*/
#define LEFT 0x4b00
/*右移键*/
#define RIGHT 0x4d00 
/*下移键*/
#define DOWN 0x5000 
/*上移键*/
#define UP 0x4800
/*空格键*/
#define SPACE 0x3920

#define   NO_PRESSED    0
#define   LEFT_PRESSED  1
#define   RIGHT_PRESSED 2
#define   pi            3.1415926

/*定义全局变量*/
int Rx,Ry,R;
int TOPx,TOPy,BOTTOMx,BOTTOMy;
int Centx,Centy;
int lineStartx,lineStarty,lineEndx,lineEndy;
int linePoint_x[20],linePoint_y[20];


/*这里的字模数组均由“点阵字模工具”生成，你可以用你自己需要的点阵信息来
替换示例中的字模信息，注意字模大小要一致，否则显示会出问题。*/
char zhi16K[]={
/* 以下是 '直' 的 16点阵楷体_GB2312 字模，32 byte */
  0x01,0x00,0x01,0x00,0x01,0xF0,0x1E,0x00,
  0x02,0x00,0x07,0xC0,0x08,0x40,0x0F,0x40,
  0x08,0x40,0x0F,0x40,0x08,0x40,0x0F,0x40,
  0x08,0x40,0x0F,0xFC,0x70,0x00,0x00,0x00,
};

char xian16K[]={
/* 以下是 '线' 的 16点阵楷体_GB2312 字模，32 byte */
  0x00,0x80,0x00,0x90,0x08,0x88,0x10,0x80,
  0x24,0xF0,0x45,0x80,0x78,0xB0,0x11,0xC0,
  0x2C,0x88,0x70,0x50,0x04,0x60,0x18,0xA4,
  0x63,0x14,0x00,0x0C,0x00,0x04,0x00,0x00,
};

char ju16K[]={
/* 以下是 '矩' 的 16点阵楷体_GB2312 字模，32 byte */
  0x00,0x00,0x08,0x00,0x08,0x78,0x10,0x80,
  0x1E,0x80,0x28,0xF8,0x48,0x88,0x0E,0x88,
  0xF8,0xF0,0x08,0x80,0x14,0x80,0x12,0x9E,
  0x20,0xE0,0x40,0x00,0x00,0x00,0x00,0x00,
};

char xing16K[]={
/* 以下是 '形' 的 16点阵楷体_GB2312 字模，32 byte */
  0x00,0x00,0x07,0x88,0x3A,0x08,0x12,0x10,
  0x12,0x20,0x17,0x48,0xFA,0x10,0x12,0x20,
  0x12,0xC8,0x12,0x08,0x22,0x10,0x42,0x20,
  0x00,0x40,0x00,0x80,0x03,0x00,0x00,0x00,
};

char yuan16K[]={
/* 以下是 '圆' 的 16点阵楷体_GB2312 字模，32 byte */
  0x00,0xF8,0x3F,0x08,0x23,0x88,0x24,0x88,
  0x27,0x08,0x21,0xC8,0x2E,0x48,0x29,0x48,
  0x29,0x48,0x22,0x88,0x24,0x48,0x28,0x08,
  0x3F,0xE8,0x00,0x10,0x00,0x00,0x00,0x00,
};

char qing16K[]={
/* 以下是 '清' 的 16点阵楷体_GB2312 字模，32 byte */
  0x00,0x80,0x00,0xE0,0x33,0x80,0x10,0xE0,
  0x03,0x80,0x40,0xFC,0x2F,0x00,0x01,0xE0,
  0x12,0x20,0x13,0xA0,0x22,0x20,0x63,0xA0,
  0x42,0x20,0x02,0x60,0x00,0x20,0x00,0x00,
};

char ping16K[]={
/* 以下是 '屏' 的 16点阵楷体_GB2312 字模，32 byte */
  0x00,0xF0,0x0F,0x30,0x08,0x60,0x0F,0x80,
  0x0A,0x20,0x09,0x40,0x08,0xF8,0x17,0x20,
  0x11,0x3E,0x2F,0xE0,0x21,0x20,0x42,0x20,
  0x82,0x20,0x04,0x20,0x08,0x20,0x00,0x00,
};

char bao16K[]={
/* 以下是 '保' 的 16点阵楷体_GB2312 字模，32 byte */
  0x00,0x00,0x09,0xF0,0x0A,0x10,0x12,0x10,
  0x13,0xE0,0x30,0x80,0x50,0xFC,0x9F,0x80,
  0x11,0xC0,0x12,0xA0,0x14,0x98,0x18,0x8E,
  0x10,0x80,0x10,0x80,0x00,0x00,0x00,0x00,
};

char cun16K[]={
/* 以下是 '存' 的 16点阵楷体_GB2312 字模，32 byte */
  0x01,0x00,0x01,0x00,0x01,0xF0,0x1E,0x00,
  0x02,0x70,0x05,0x90,0x08,0x20,0x08,0x40,
  0x18,0x7E,0x2B,0xA0,0xC8,0x20,0x08,0x20,
  0x08,0x20,0x08,0xA0,0x00,0x40,0x00,0x00,
};

char jia16K[]={
/* 以下是 '加' 的 16点阵楷体_GB2312 字模，32 byte */
  0x00,0x00,0x08,0x00,0x08,0x00,0x08,0x00,
  0x0F,0x00,0x79,0x3C,0x09,0x44,0x11,0x44,
  0x11,0x44,0x22,0x44,0x22,0x78,0x4A,0x00,
  0x84,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

char zai16K[]={
/* 以下是 '载' 的 16点阵楷体_GB2312 字模，32 byte */
  0x00,0x80,0x08,0xA0,0x08,0x90,0x0E,0x80,
  0x38,0xF0,0x0F,0x80,0x78,0x50,0x0E,0x50,
  0x34,0x20,0x1E,0x20,0x34,0x50,0x0E,0x92,
  0x75,0x0A,0x04,0x06,0x04,0x02,0x00,0x00,
};

char bang16K[]={
/* 以下是 '帮' 的 16点阵楷体_GB2312 字模，32 byte */
  0x04,0x00,0x07,0x38,0x1C,0x48,0x06,0x50,
  0x1C,0x50,0x07,0x48,0x78,0x58,0x11,0x40,
  0x21,0xF0,0x4F,0x10,0x09,0x10,0x09,0x50,
  0x09,0x20,0x01,0x00,0x01,0x00,0x00,0x00,
};


char zhu16K[]={
/* 以下是 '助' 的 16点阵楷体_GB2312 字模，32 byte */
  0x00,0x00,0x00,0x20,0x0C,0x20,0x34,0x20,
  0x24,0x20,0x34,0x38,0x25,0xC8,0x34,0x48,
  0x24,0x48,0x26,0x88,0x38,0x88,0xE1,0x28,
  0x02,0x10,0x04,0x00,0x00,0x00,0x00,0x00,
};

/*自定义函数*/
void outChinese(char *mat,int matsize,int x,int y,int color);
void fill(int startx,int starty,int endx,int endy,int color);
void showHelp();

void save();
void load();

int mouseStatus(int* x,int* y);
int setMousePos(int x, int y);
void DrawMouse(float x,float y);

void DrawLine();
void DrawRectangle();
void LineToCircle(int x0,int y0,int r);
void DrawCircle();
long factorial(int n);
float berFunction(int i,int n,double t);
void DrawBezier();


/*根据点阵信息显示中文函数*/
void outChinese(char *mat,int matsize,int x,int y,int color)
/*依次：字模指针、点阵大小、起始坐标(x,y)、颜色*/
{
  int i, j, k, n;
  n = (matsize - 1) / 8 + 1;
  for(j = 0; j < matsize; j++)
    for(i = 0; i < n; i++)
      for(k = 0;k < 8; k++)
        if(mat[j * n + i] & (0x80 >> k))
          /*测试为1的位则显示*/
          putpixel(x + i * 8 + k, y + j, color);
}

/*填充函数*/
void fill(int startx,int starty,int endx,int endy,int color)
{
    int i,j;
        for(i=startx;i<=endx;i++)
            for(j=starty;j<=endy;j++)
            /*在指定位置以指定颜色画一像素*/
            putpixel(i,j,color);

} 

/*显示用户帮助函数*/
void showHelp()
{
    setcolor(14);
    outtextxy(45,50,"Line:");
    setcolor(WHITE);
    outtextxy(45,50,"      1 Press left button to start until to line end.");
    outtextxy(45,65,"      2 Use UP,DOWN,LEFT,RIGHT keys to move it.");
    outtextxy(45,80,"      3 Use PAGEUP key to enlarge it, and PAGEDOWN key to shrink it.");
    outtextxy(45,95,"      4 Use SPACE key to rotate it.");

    setcolor(14);
    outtextxy(45,120,"Rectangle:");
    setcolor(WHITE);
    outtextxy(45,120,"           1 Press left button to start until to right corner.");
    outtextxy(45,135,"           2 Use UP,DOWN,LEFT,RIGHT keys to move it.");
    outtextxy(45,150,"           3 Use PAGEUP key to enlarge it, and PAGEDOWN key to shrink it.");


    setcolor(14);
    outtextxy(45,170,"Circle:");
    setcolor(WHITE);
    outtextxy(45,170,"        1 Press left button to start until to end.");
    outtextxy(45,185,"        2 Use PAGEUP key to enlarge it, and PAGEDOWN key to shrink it.");


    setcolor(14);
    outtextxy(45,205,"Bezier:");
    setcolor(WHITE);
    outtextxy(45,205,"        Press left button to start, and right button to end.");


    outtextxy(45,230,"Press ESC key to stop the operation function.");
    outtextxy(45,245,"Press right button to end the drawing works.");
    outtextxy(45,260,"Press any key to continue......");
    getch();
    fill(40,40,625,270,0);

}

/*保存函数*/
void save()
{
    int i,j; 
    FILE *fp;
    char fileName[20];

    fill(0,447,630,477,2);
    gotoxy(1,25);
    printf("\n\n\n\n  Input the file name[.dat]:");
    scanf("%s",fileName);
    fill(0,447,630,477,2);

    /*以读写的方式打开文件*/
    if((fp=fopen(fileName,"w+"))==NULL)
    {
        outtextxy(260,455,"Failed to open file!");
        exit(0); 
    } 
    outtextxy(280,455,"saving...");

    /*保存像素到文件*/
    for(i=5;i<630;i++)
        for(j=30;j<=445;j++)
            fputc(getpixel(i,j),fp);
    fclose(fp);

    fill(0,447,630,477,2);
    outtextxy(260,455,"save over!");
} 

/*打开函数*/
void load()
{ 
    int i,j; 
    char fileName[20];
    FILE *fp;

    fill(0,447,630,477,2);
    gotoxy(1,25); 
    printf("\n\n\n\n  Input the file name[.dat]:");
    scanf("%s",fileName);

    /*打开指定的文件*/
    if((fp=fopen(fileName,"r+"))!=NULL)
    { 
        fill(0,447,630,477,2);
        outtextxy(280,455,"loading...");

        /*从文件中读出像素*/
        for(i=5;i<630;i++)
            for(j=30;j<=445;j++)
                putpixel(i,j,fgetc(fp));
        fill(0,447,630,477,2);
        outtextxy(280,455,"loading over !");
    }
    /*打开失败*/
    else
    {
        fill(0,447,630,477,2);
        outtextxy(260,455,"Failed to open file!");

    }
    fclose(fp);
}

/*获取鼠标状态函数*/
int mouseStatus(int* x,int* y)
{
    /*定义两个寄存器变量，分别存储入口参数和出口参数*/
    union REGS inregs,outregs;
    int status;
    status=NO_PRESSED;

    /*入口参数AH＝3，读取鼠标位置及其按钮状态*/
    inregs.x.ax=3;
    int86(0x33,&inregs,&outregs);
    /*CX表示水平位置，DX表示垂直位置*/
    *x=outregs.x.cx;
    *y=outregs.x.dx;

    /*BX表示按键状态*/
    if(outregs.x.bx&1)
        status=LEFT_PRESSED;
    else if(outregs.x.bx&2)
        status=RIGHT_PRESSED;
    return (status);
}

/*设置鼠标指针位置函数*/
int setMousePos(int x,int y)
{
    union REGS inregs,outregs;

    /*入口参数AH＝4，设置鼠标指针位置*/
    inregs.x.ax=4;
    inregs.x.cx=x;
    inregs.x.dx=y;
    int86(0x33,&inregs,&outregs);
}

/*绘制鼠标函数*/
void DrawMouse(float x,float y)
{
    line(x,y,x+5,y+15);
    line(x,y,x+15,y+5);
    line(x+5,y+15,x+15,y+5);
    line(x+11,y+9,x+21,y+19);
    line(x+9,y+11,x+19,y+21);
    line(x+22,y+19,x+20,y+21);
}


/*绘制直线函数*/
void DrawLine()
{
   int x0,y0,x1,y1;
   int last_x=0,last_y=0;
   int endFlag=0;
   int key;
   int temStartx,temStarty,temEndx,temEndy;
   int increment_x,increment_y,angle;

   DrawMouse(last_x,last_y);
   while(1)
   {
        /*右键结束画直线*/
        while((mouseStatus(&x1,&y1)==RIGHT_PRESSED))
            endFlag=1;
        if(endFlag==1)
            break;
        /*鼠标移动，没有单击，仅仅画移动的鼠标*/
        while(mouseStatus(&x1,&y1) == NO_PRESSED)
        {
            if(last_x!=x1||last_y!=y1)
            {
                DrawMouse(last_x,last_y);
                DrawMouse(x1,y1);
                last_x=x1;
                last_y=y1;
            }
        }
        /*单击左键后，开始画直线*/
        if(mouseStatus(&x0,&y0)==LEFT_PRESSED)
        {
            DrawMouse(last_x,last_y);
            line(x0,y0,x1,y1);
            last_x=x1;
            last_y=y1;
            /*拉动过程中，画直线和鼠标*/
            while(mouseStatus(&x1, &y1)==LEFT_PRESSED)
            {
                if(last_x!=x1||last_y!=y1)
                {
                    line(x0,y0,last_x,last_y);
                    line(x0,y0,x1,y1);
                    last_x=x1;
                    last_y=y1;
                 }
            }
            /*松开左键后，画直线完成，记录直线的起始位置*/
            lineStartx=x0;
            lineStarty=y0;
            lineEndx=x1;
            lineEndy=y1;

            while(1)
            {
                /*从键盘获取键值，开始操作（移动、放大、缩小、旋转）直线*/
                key=bioskey(0);
                /*ESC键，退出操作*/
                if(key==ESC)
                    break;

                /*旋转*/
                if(key==SPACE)
                {   
                    /*计算旋转中心*/
                    /*如果直线示倾斜的*/
                    if((lineStarty!=lineEndy)&& (lineStartx!=lineEndx))
                    {
                        Centx=(lineEndx-lineStartx)/2+lineStartx;
                        Centy=(lineEndy-lineStarty)/2+lineStarty;
                    }
        
                    /*如果直线是竖直的*/
                    if(lineStarty==lineEndy)
                    {
                        Centx=(lineEndx-lineStartx)/2+lineStartx;
                        Centy=lineStarty;
                    }

                    /*如果直线是水平的*/
                    if(lineStartx==lineEndx)
                    {
                        Centx=lineStartx;
                        Centy=(lineEndy-lineStarty)/2+lineStarty;
                    }

                    temStartx=lineStartx;
                    temStarty=lineStarty;
                    temEndx=lineEndx;
                    temEndy=lineEndy;

                    /*旋转不能超过边界*/
                    if(lineStartx>=10 && lineStarty>=40 && lineEndx <=620 && lineEndy <=445)
                    {
                        /*清除原有的直线*/
                        setwritemode(XOR_PUT);
                        line(lineStartx,lineStarty,lineEndx,lineEndy);

                        /*计算旋转30度后的起点坐标*/
                        lineStartx=(temStartx-Centx)*cos(pi/6)-(temStarty-Centy)*sin(pi/6)+Centx;
                        lineEndx=(temEndx-Centx)*cos(pi/6)-(temEndy-Centy)*sin(pi/6)+Centx;

                        /*计算旋转30度后的终点坐标*/
                        lineStarty=(temStartx-Centx)*sin(pi/6)+(temStarty-Centy)*cos(pi/6)+Centy;
                        lineEndy=(temEndx-Centx)*sin(pi/6)+(temEndy-Centy)*cos(pi/6)+Centy;
    
                        temStartx=lineStartx;
                        temStarty=lineStarty;
                        temEndx=lineEndx;
                        temEndy=lineEndy;

                        /*绘制旋转后的直线*/
                        line(lineStartx,lineStarty,lineEndx,lineEndy);
                    }   
                }
                /*左移直线*/
                if(key==LEFT)
                {
                    if(lineStartx>=10 && lineStarty>=40 && lineEndx <=620 && lineEndy <=445)
                    {
                        setwritemode(XOR_PUT);
                        line(lineStartx,lineStarty,lineEndx,lineEndy);
                        /*起始的横坐标减小*/
                        lineStartx-=5;
                        lineEndx-=5;
                        line(lineStartx,lineStarty,lineEndx,lineEndy);
                    }
                }

                /*右移直线*/
                if(key==RIGHT)
                {
                    if(lineStartx>=10 && lineStarty>=40 && lineEndx <=620 && lineEndy <=445)
                    {
                        setwritemode(XOR_PUT);
                        line(lineStartx,lineStarty,lineEndx,lineEndy);
                        /*起始的横坐标增加*/
                        lineStartx+=5;
                        lineEndx+=5;
                        line(lineStartx,lineStarty,lineEndx,lineEndy);
                    }
                }

                /*下移直线*/
                if(key==DOWN)
                {
                    if(lineStartx>=10 && lineStarty>=40 && lineEndx <=620 && lineEndy <=445)
                    {
                        setwritemode(XOR_PUT);
                        line(lineStartx,lineStarty,lineEndx,lineEndy);
                        /*起始的纵坐标增加*/
                        lineStarty+=5;
                        lineEndy+=5;
                        line(lineStartx,lineStarty,lineEndx,lineEndy);
                    }
                }

                /*上移直线*/
                if(key==UP)
                {
                    if(lineStartx>=10 && lineStarty>=40 && lineEndx <=620 && lineEndy <=445)
                    {
                        setwritemode(XOR_PUT);
                        line(lineStartx,lineStarty,lineEndx,lineEndy);
                        /*起始的纵坐标减小*/
                        lineStarty-=5;
                        lineEndy-=5;
                        line(lineStartx,lineStarty,lineEndx,lineEndy);
                    }
                }
                /*放大直线*/
                if(key==PAGEUP)
                {   
                    if(lineStartx>=10 && lineStarty>=40 && lineEndx <=620 && lineEndy <=445)
                    {
                        setwritemode(XOR_PUT);
                        line(lineStartx,lineStarty,lineEndx,lineEndy);

                        /*如果直线是倾斜的*/
                        if((lineStarty!=lineEndy)&& (lineStartx!=lineEndx))
                        {
                            /*计算直线的倾角*/
                            angle=atan((fabs(lineEndy-lineStarty))/(fabs(lineEndx-lineStartx)));
                            /*计算水平增量*/
                            increment_x=cos(angle)*2;
                            /*计算垂直增量*/
                            increment_y=sin(angle)*2;

                            /*计算放大后的起始坐标*/
                            if(lineStartx<lineEndx)
                            {
                                lineStartx-=increment_x;
                                lineStarty-=increment_y;
                                lineEndx+=increment_x;
                                lineEndy+=increment_y;
                            }
                            if(lineStartx>lineEndx)
                            {
                                lineEndx-=increment_x;
                                lineEndy-=increment_y;
                                lineStartx+=increment_x;
                                lineStarty+=increment_y;
                            }

                        }

                        /*如果直线竖直的*/
                        if(lineStarty==lineEndy)
                        {
                            lineStartx-=5;
                            lineEndx+=5;
                        }

                        /*如果直线是水平的*/
                        if(lineStartx==lineEndx)
                        {
                            lineStarty-=5; 
                            lineEndy+=5;
                        }
                        line(lineStartx,lineStarty,lineEndx,lineEndy);
                    }
                }
                /*缩小直线*/
                if(key==PAGEDOWN)
                {
                    if(lineStartx>=10 && lineStarty>=40 && lineEndx <=620 && lineEndy <=445)
                    {
                        setwritemode(XOR_PUT);
                        line(lineStartx,lineStarty,lineEndx,lineEndy);

                        /*如果直线是倾斜的*/
                        if((lineStarty!=lineEndy)&& (lineStartx!=lineEndx))
                        {
                            /*计算直线的倾角*/
                            angle=atan((fabs(lineEndy-lineStarty))/(fabs(lineEndx-lineStartx)));
                            /*计算水平减少量*/
                            increment_x=cos(angle)*2;
                            /*计算垂直减少量*/
                            increment_y=sin(angle)*2;
                            /*计算缩小后的起始坐标*/
                            if(lineStartx<lineEndx)
                            {
                                lineStartx+=increment_x;
                                lineStarty+=increment_y;
                                lineEndx-=increment_x;
                                lineEndy-=increment_y;
                            }
                            if(lineStartx>lineEndx)
                            {
                                lineEndx+=increment_x;
                                lineEndy+=increment_y;
                                lineStartx-=increment_x;
                                lineStarty-=increment_y;
                            }
                        }

                        /*如果直线竖直的*/
                        if(lineStarty==lineEndy)
                        {
                            lineStartx+=5;
                            lineEndx-=5;
                        }

                        /*如果直线是水平的*/
                        if(lineStartx==lineEndx)
                        {
                            lineStarty+=5;
                            lineEndy-=5;
                        }
                        line(lineStartx,lineStarty,lineEndx,lineEndy);
                    }
                }
            }
            DrawMouse(x1,y1);
        }
    }
    DrawMouse(last_x,last_y);
}


/*绘制矩形函数*/
void DrawRectangle()
{
   int x0,y0,x1,y1;
   int last_x=0,last_y=0;
   int endFlag=0;
   int key;

   DrawMouse(last_x,last_y);
   while(1)
   {    
        /*单击右键，结束绘制矩形*/
        while((mouseStatus(&x1,&y1)==RIGHT_PRESSED))
            endFlag=1;
        if(endFlag==1)
            break;

        /*移动鼠标，仅仅绘制鼠标即可*/
        while(mouseStatus(&x1,&y1) == NO_PRESSED)
        {
            if(last_x!=x1||last_y!=y1)
            {
                DrawMouse(last_x,last_y);
                DrawMouse(x1,y1);
                last_x=x1;
                last_y=y1;
            }
        }

        /*单击左键开始绘制矩形*/
        if(mouseStatus(&x0,&y0)==LEFT_PRESSED)
        {
            DrawMouse(last_x,last_y);
            rectangle(x0,y0,x1,y1);
            last_x=x1;
            last_y=y1;

            /*按着鼠标左键不动，绘制矩形*/
            while(mouseStatus(&x1,&y1)==LEFT_PRESSED)
            {
                if(last_x!=x1||last_y!=y1)
                {
                    rectangle(x0,y0,last_x,last_y);
                    rectangle(x0,y0,x1,y1);
                    last_x=x1;
                    last_y=y1;
                }
            }

            /*绘制结束后，记录左上角和右下角的坐标*/
            TOPx=x0;
            TOPy=y0;
            BOTTOMx=x1;
            BOTTOMy=y1;

            while(1)
            {
                key=bioskey(0);
                if(key==ESC)
                    break;

                /*放大矩形*/
                if(key==PAGEUP)
                {
                    if(TOPx>=10 && TOPy>=40 && BOTTOMx <=620 &&BOTTOMy <=445)
                    {
                        /*清除原有的直线*/
                        setwritemode(XOR_PUT);
                        rectangle(TOPx,TOPy,BOTTOMx,BOTTOMy);
                        /*左上角坐标减小*/
                        TOPx-=5;
                        TOPy-=5;
                        /*右下角坐标增加*/
                        BOTTOMx+=5;
                        BOTTOMy+=5;
                        /*绘制放大后的矩形*/
                        rectangle(TOPx,TOPy,BOTTOMx,BOTTOMy);
                    }
                }

              /*缩小矩形*/
                if(key==PAGEDOWN)
                {
                    if(TOPx>=10 && TOPy>=40 && BOTTOMx <=620 &&BOTTOMy <=445)
                    {
                        setwritemode(XOR_PUT);
                        rectangle(TOPx,TOPy,BOTTOMx,BOTTOMy);
                        /*左上角坐标增加*/
                        TOPx+=5;
                        TOPy+=5;
                        /*右下角坐标减小*/
                        BOTTOMx-=5;
                        BOTTOMy-=5;
                        /*绘制缩小后的矩形*/
                        rectangle(TOPx,TOPy,BOTTOMx,BOTTOMy);
                    }
                }

                /*左移矩形*/
                if(key==LEFT)
                {
                    if(TOPx>=10 && TOPy>=40 && BOTTOMx <=620 &&BOTTOMy <=445)
                    {
                        setwritemode(XOR_PUT);
                        rectangle(TOPx,TOPy,BOTTOMx,BOTTOMy);
                        /*横坐标减小*/
                        TOPx-=5;
                        BOTTOMx-=5;
                        rectangle(TOPx,TOPy,BOTTOMx,BOTTOMy);
                    }
                }

                /*右移矩形*/
                if(key==RIGHT)
                {
                    if(TOPx>=10 && TOPy>=40 && BOTTOMx <=620 &&BOTTOMy <=445)
                    {
                        setwritemode(XOR_PUT);
                        rectangle(TOPx,TOPy,BOTTOMx,BOTTOMy);
                        /*横坐标增加*/
                        TOPx+=5;
                        BOTTOMx+=5;
                        rectangle(TOPx,TOPy,BOTTOMx,BOTTOMy);
                     }
                }

                /*下移矩形*/
               if(key==DOWN)
               {
                   if(TOPx>=10 && TOPy>=40 && BOTTOMx <=620 &&BOTTOMy <=445)
                   {
                        setwritemode(XOR_PUT);
                        rectangle(TOPx,TOPy,BOTTOMx,BOTTOMy);
                        /*纵坐标增加*/
                        TOPy+=5;
                        BOTTOMy+=5;
                        rectangle(TOPx,TOPy,BOTTOMx,BOTTOMy);
                   }
                }

                /*上移矩形*/
                if(key==UP)
                {
                     if(TOPx>=10 && TOPy>=40 && BOTTOMx <=620 &&BOTTOMy <=445)
                     {
                        setwritemode(XOR_PUT);
                        rectangle(TOPx,TOPy,BOTTOMx,BOTTOMy);
                        /*纵坐标减小*/
                        TOPy-=5;
                        BOTTOMy-=5;
                        rectangle(TOPx,TOPy,BOTTOMx,BOTTOMy);
                     }
                }
            }
            DrawMouse(x1,y1);
        }
   }
   DrawMouse(last_x,last_y);
}

/*用直线法生成圆*/
void LineToCircle(int x0,int y0,int r)
{
   int angle;
   int x1,y1,x2,y2;

   angle=0;
   x1=r*cos(angle*pi/180);
   y1=r*sin(angle*pi/180);

   while(angle<45)
   {
      angle+=5;
      x2=r*cos(angle*pi/180);
      y2=r*sin(angle*pi/180);
      while(x2==x1)
        x2++;
      while(y2==y1)
        y2++;
      line(x0+x1,y0+y1,x0+x2,y0+y2);
      line(x0-x1,y0+y1,x0-x2,y0+y2);
      line(x0+x1,y0-y1,x0+x2,y0-y2);
      line(x0-x1,y0-y1,x0-x2,y0-y2);
      line(x0+y1,y0-x1,x0+y2,y0-x2);
      line(x0+y1,y0+x1,x0+y2,y0+x2);
      line(x0-y1,y0-x1,x0-y2,y0-x2);
      line(x0-y1,y0+x1,x0-y2,y0+x2);
      x1=x2+1;
      y1=y2+1;
    }
}

/*绘制圆函数*/
void DrawCircle()
{
   int x0,y0,x1,y1,r,oldr;
   int last_x,last_y;
   int endFlag;
   int key;

   last_x=0;
   last_y=0;
   endFlag=0;

   DrawMouse(last_x,last_y);
   while(1)
   {
        /*单击右键，绘制圆结束*/
        while((mouseStatus(&x1,& y1)==RIGHT_PRESSED))
        {
            endFlag=1;
        }
        if(endFlag==1)
            break;

        /*移动鼠标，仅绘制鼠标即可*/
        while(mouseStatus(&x1,&y1) == NO_PRESSED)
        {
            if(last_x!=x1||last_y!=y1)
            {
                DrawMouse(last_x,last_y);
                DrawMouse(x1,y1);
                last_x=x1;
                last_y=y1;
            }
        }

        /*单击左键，开始绘制圆*/
        if(mouseStatus(&x0,&y0)==LEFT_PRESSED)
        {
            /*计算半径*/
            r=sqrt((x0-x1)*(x0-x1)+(y0-y1)*(y0-y1));
            DrawMouse(last_x,last_y);
            LineToCircle(x0,y0,r);
            last_x=x1;
            last_y=y1;
            oldr=r;

            /*按住鼠标左键不动，拖动鼠标绘制圆*/
            while(mouseStatus(&x1,&y1)==LEFT_PRESSED)
            { 
                if(last_x!=x1||last_y!=y1)
                {
                    r=sqrt((x0-x1)*(x0-x1)+(y0-y1)*(y0-y1));
                    LineToCircle(x0,y0,oldr);
                    LineToCircle(x0,y0,r);
                    last_x=x1;
                    last_y=y1;
                    oldr=r;
                }          
            }
            /*绘制结束后，记录圆的圆心和半径*/
            Rx=x0;
            Ry=y0;
            R=r;

         while(1)
        {
            key=bioskey(0);
            if(key==ESC)
               break;
            /*放大圆*/
            if(key==PAGEUP)
                {
                    if(Rx-R>10 && Ry-R>40 && Rx+R<620 && Ry+R<445)
                    {
                        /*如果半径和初始状态一样大，则保留原来的圆*/
                        if(R==r)
                        {
                            setcolor(WHITE);
                            R+=10;
                            circle(Rx,Ry,R);
                        }
                        else
                        {
                            setcolor(BLACK);
                            /*用背景色画圆，覆盖原有的*/
                            circle(Rx,Ry,R);
                            /*增加半径*/
                            R+=10;
                            setcolor(WHITE);
                            /*绘制新圆*/
                            circle(Rx,Ry,R);
                        }
                     }
                }
            /*缩小圆*/
             if(key==PAGEDOWN)
                {
                    if(Rx-R>10 && Ry-R>40 && Rx+R<620 && Ry+R<445)
                    {
                        /*如果半径和初始状态一样大，则保留原来的圆*/
                        if(R==r)
                        {
                            setcolor(WHITE);
                            R-=10;
                            circle(Rx,Ry,R);

                        }
                        else
                        {
                            setcolor(BLACK);
                            /*用背景色画圆，覆盖原有的*/
                            circle(Rx,Ry,R);
                            setcolor(WHITE);
                            /*减小半径*/
                            R-=10;
                            circle(Rx,Ry,R);
                        }

                     }
                }

            }
            DrawMouse(x1,y1);
        }
   }
   DrawMouse(last_x,last_y);
}       

/*求阶乘函数*/
long factorial(int n)
{ 
    long s=1;
    if(n==0)
        return 1;

    while(n>0)
    {
        s*=n;
        n--;
    }
    return s;
}

/*伯恩斯坦基函数*/
float berFunction(int i,int n,double t)
{
    if(i==0&&t==0||t==1&&i==n)
        return  1;
    else if(t==0||t==1)
        return 0;
    return  factorial(n)/(factorial(i)*factorial(n-i))*pow(t,i)*pow(1-t,n-i);

}

/*绘制Bezier曲线函数*/
void DrawBezier()
{
    int x,y,x0,y0,x1,y1;
    float j,t,dt;
    int i,n;
    int endFlag=0;
    int last_x=0,last_y=0;
    n=0;

    DrawMouse(last_x,last_y);
    while(mouseStatus(&x1,&y1)==LEFT_PRESSED);
    while(1)
    {
            while((mouseStatus(&x1,&y1)==RIGHT_PRESSED))
                endFlag=1;
            if(endFlag==1)
                break;
            /*如果有两个以上的点，则将其连接，即画直线*/ 
            if(n>1)
               line(linePoint_x[n-1],linePoint_y[n-1],linePoint_x[n-2],linePoint_y[n-2]);

            /*移动鼠标*/
            while(mouseStatus(&x1,&y1) == NO_PRESSED)
            {
                if(last_x!=x1||last_y!=y1)
                {
                    DrawMouse(last_x,last_y);
                    DrawMouse(x1,y1);
                    last_x=x1;
                    last_y=y1;
                }
            }

            /*单击左键时，绘制点*/
            while(mouseStatus(&x0,&y0)==LEFT_PRESSED);
            putpixel(x0,y0,14);
            /*记录每次鼠标左键单击的点坐标*/
            linePoint_x[n]=x0;
            linePoint_y[n]=y0;
            n++;
      }
      DrawMouse(x1,y1);

      dt=1.0/10;
      setwritemode(0);
      for(j=0;j<=10;j+=0.5)
      {
        t=j*dt;
        x=0;
        y=0;
        i=0;
        while(i<n-1)
        {
            x+=berFunction(i,n-2,t)*linePoint_x[i];
            y+=berFunction(i,n-2,t)*linePoint_y[i];
            i++;
        }
        if(j==0)
           moveto(x,y);

        lineto(x,y);

      }
      setwritemode(1);
}

void main()
{
    int gdriver,gmode;
    int x0,y0,x1,y1;
    int last_x,last_y;
    int i;

    x0=250;
    y0=250;
    gdriver=DETECT;
    while( 1)
    {
        initgraph(&gdriver,&gmode,"");
        setbkcolor(0);
        setcolor(14);
        /*绘制画布*/
        rectangle(5,30,630,445);
        setfillstyle(1,2);
        /*填充画布以外的颜色，画布仍呈背景色*/
        floodfill(10,10,14);

        /*绘制按钮框*/
        for(i=0;i<=7;i++)
        {
            setcolor(RED);
            line(60*i+1,2,60*i+1,25);
            line(60*i+1,2,60*i+55,2);
            setcolor(RED);
            line(60*i+1,25,60*i+55,25);
            line(60*i+55,2,60*i+55,25);
        }

        setcolor(RED);
        line(0,446,639,446);
        line(0,478,639,478);

        setcolor(8);
        /*绘制退出按钮框*/
        rectangle(570,2,625,25);
        setfillstyle(1,RED);
        floodfill(620,5,8);
        setcolor(WHITE);
        outtextxy(585,10,"EXIT");

        /*显示“直线”*/
        outChinese(zhi16K, 16, 10,6, WHITE);
        outChinese(xian16K, 16, 28,6, WHITE);

        /*显示“矩形”*/
        outChinese(ju16K, 16, 70,6, WHITE);
        outChinese(xing16K, 16, 88,6, WHITE);

        /*显示“圆形”*/
        outChinese(yuan16K, 16, 130,6, WHITE);
        outChinese(xing16K, 16, 148,6, WHITE);

        outtextxy(185,10,"Bezier");

        /*显示“清屏”*/
        outChinese(qing16K, 16, 250,6, WHITE);
        outChinese(ping16K, 16, 268,6, WHITE);

        /*显示“保存”*/
        outChinese(bao16K, 16, 310,6, WHITE);
        outChinese(cun16K, 16, 328,6, WHITE);

        /*显示“加载”*/
        outChinese(jia16K, 16, 370,6, WHITE);
        outChinese(zai16K, 16, 388,6, WHITE);

        /*显示“帮助”*/
        outChinese(bang16K, 16, 430,6, WHITE);
        outChinese(zhu16K, 16, 448,6, WHITE);

        setMousePos(x0,y0);
        setwritemode(1);
        DrawMouse(x0,y0);

        last_x=x0;
        last_y=y0;
        while(!((mouseStatus(&x1,&y1)==NO_PRESSED) && x1>240 &&x1<295&&y1>1&&y1<25))
        {
            /*单击退出按钮*/
            if((mouseStatus(&x1,&y1)==NO_PRESSED) && x1>570 &&x1<625&&y1>1&&y1<25)
                exit(0);
            /*鼠标移动*/
            while(mouseStatus(&x1,&y1) == NO_PRESSED||y1>25)
            {
                if(last_x!=x1 && last_y!=y1)
                {
                    DrawMouse(last_x,last_y);
                    DrawMouse(x1,y1);
                    last_x=x1;
                    last_y=y1;
                }
            }

            DrawMouse(last_x,last_y);
            /*在按钮框中单击左键后*/
            while(mouseStatus(&x1,&y1)==LEFT_PRESSED);
            /*绘制直线*/
            if(x1>0 && x1<60 && y1>1 && y1<25)
            {
                setwritemode(0);
                setcolor(8);
                /*呈凹陷状态*/
                line(1,2,1,25);
                line(1,2,55,2);
                setcolor(15);
                line(1,25,55,25);
                line(55,2,55,25);
                setwritemode(1);

                DrawLine();

                setwritemode(0);
                setcolor(RED);
                /*还原成初始状态*/
                rectangle(1,2,55,25);
                setcolor(15);
                setwritemode(1);

                DrawMouse(last_x,last_y);
            }

            /*绘制矩形*/
            if(x1>60 && x1<115 && y1>1 && y1<25)
            {
                setwritemode(0);
                setcolor(8);
                line(61,2,61,25);
                line(61,2,115,2);
                setcolor(15);
                line(61,25,115,25);
                line(115,2,115,25);
                setwritemode(1);

                DrawRectangle();

                setwritemode(0);
                setcolor(RED);
                rectangle(61,2,115,25);
                setcolor(15);
                setwritemode(1);

                DrawMouse(last_x,last_y);
            }

            /*绘制圆形*/
            if(x1>120 && x1<175 && y1>1 && y1<25)
            {
                setwritemode(0);
                setcolor(8);
                line(121,2,121,25);
                line(121,2,175,2);
                setcolor(15);
                line(121,25,175,25);
                line(175,2,175,25);
                setwritemode(1);

                DrawCircle();

                setwritemode(0);
                setcolor(RED);
                rectangle(121,2,175,25);
                setcolor(15);
                setwritemode(1);

                DrawMouse(last_x,last_y);
            }

            /*绘制Bezier曲线*/
            if(x1>180 && x1<235 && y1>1 && y1<25)
            {
                setwritemode(0);
                setcolor(8);
                line(181,2,181,25);
                line(181,2,235,2);
                setcolor(15);
                line(181,25,235,25);
                line(235,2,235,25);
                setwritemode(1);

                DrawBezier();

                setwritemode(0);
                setcolor(RED);
                rectangle(181,2,235,25);
                setcolor(15);
                setwritemode(1);
                DrawMouse(last_x,last_y);
            }

            /*保存文件*/
            if(x1>300 && x1<355 && y1>1 && y1<25)
            {
                setwritemode(0);
                setcolor(8);
                line(301,2,301,25);
                line(301,2,355,2);
                setcolor(15);
                line(301,25,355,25);
                line(355,2,355,25);
                setwritemode(1);

                save();

                setwritemode(0);
                setcolor(RED);
                rectangle(301,2,355,25);
                setcolor(15);
                setwritemode(1);
                DrawMouse(last_x,last_y);
            }

            /*加载已有的文件*/
           if(x1>360 && x1<415 && y1>1 && y1<25)
           {
                setwritemode(0);
                setcolor(8);
                line(361,2,361,25);
                line(361,2,415,2);
                setcolor(15);
                line(361,25,415,25);
                line(415,2,415,25);
                setwritemode(1);

                load();

                setwritemode(0);
                setcolor(RED);
                rectangle(361,2,415,25);
                setcolor(15);
                setwritemode(1);
                DrawMouse(last_x,last_y);
            }

            /*显示用户帮助*/
            if(x1>420 && x1<475 && y1>1 && y1<25)
            {
                setwritemode(0);
                setcolor(8);
                line(421,2,421,25);
                line(421,2,475,2);
                setcolor(15);
                line(421,25,475,25);
                line(475,2,475,25);
                setwritemode(1);

                showHelp();

                setwritemode(0);
                setcolor(RED);
                rectangle(421,2,475,25);
                setcolor(15);
                setwritemode(1);
                DrawMouse(last_x,last_y);
            }

        }
        closegraph();
    }
}
