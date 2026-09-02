/*电子时钟源代码*/
#include<graphics.h>
#include<stdio.h>
#include<math.h>
#include<dos.h>
#define PI 3.1415926 /*定义常量*/
#define UP 0x4800   /*上移↑键：修改时间*/
#define DOWN 0x5000 /*下移↓键：修改时间*/
#define ESC 0x11b   /*ESC键 ： 退出系统*/
#define TAB 0xf09   /*TAB键 ： 移动光标*/
/*函数声明*/
int keyhandle(int,int); /*键盘按键判断，并调用相关函数处理*/
int timeupchange(int);  /*处理上移按键*/
int timedownchange(int); /*处理下移按键*/
int digithour(double); /*将double型的小时数转换成int型*/
int digitmin(double);  /*将double型的分钟数转换成int型*/
int digitsec(double);   /*将double型的秒钟数转换成int型*/
void digitclock(int,int,int ); /*在指定位置显示时钟或分钟或秒钟数*/
void drawcursor(int); /*绘制一个光标*/
void clearcursor(int);/*消除前一个光标*/
void clockhandle(); /*时钟处理*/

double h,m,s; /*全局变量:小时，分，秒*/
double x,x1,x2,y,y1,y2; /*全局变量:坐标值*/
struct time t[1];/*定义一个time结构类型的数组*/
main()
{
 int driver, mode=0,i,j;
 driver=DETECT; /*自动检测显示设备*/
 initgraph(&driver, &mode, "");/*初始化图形系统*/
 setlinestyle(0,0,3); /*设置当前画线宽度和类型:设置三点宽实线*/
 setbkcolor(0);/*用调色板设置当前背景颜色*/
 setcolor(9); /*设置当前画线颜色*/
 line(82,430,558,430);
 line(70,62,70,418);
 line(82,50,558,50);
 line(570,62,570,418);
 line(70,62,570,62);
 line(76,56,297,56);
 line(340,56,564,56);       /*画主体框架的边直线*/
 /*arc(int x, int y, int stangle, int endangle, int radius)*/
 arc(82,62,90,180,12);
 arc(558,62,0,90,12);
 setlinestyle(0,0,3);
 arc(82,418,180,279,12);
 setlinestyle(0,0,3);
 arc(558,418,270,360,12);   /*画主体框架的边角弧线*/
 setcolor(15);
 outtextxy(300,53,"CLOCK"); /*显示标题*/
 setcolor(7);
 rectangle(342,72,560,360); /*画一个矩形,作为时钟的框架*/

 setwritemode(0); /*规定画线的方式。mode=0, 则表示画线时将所画位置的原来信息覆盖*/
 setcolor(15);
 outtextxy(433,75,"CLOCK");/*时钟的标题*/
 setcolor(7);
 line(392,310,510,310);
 line(392,330,510,330);
 arc(392,320,90,270,10);
 arc(510,320,270,90,10); /*绘制电子动画时钟下的数字时钟的边框架*/
 /*绘制数字时钟的时分秒的分隔符*/
 setcolor(5);
 for(i=431;i<=470;i+=39)
   for(j=317;j<=324;j+=7){
   setlinestyle(0,0,3);
   circle(i,j,1); /*以(i, y)为圆心,1为半径画圆*/
  }
 setcolor(15);
 line(424,315,424,325); /*在运行电子时钟前先画一个光标*/
 /*绘制表示小时的圆点*/
 for(i=0,m=0,h=0;i<=11;i++,h++){
    x=100*sin((h*60+m)/360*PI)+451;
    y=200-100*cos((h*60+m)/360*PI);
    setlinestyle(0,0,3);
    circle(x,y,1);
 }
 /*绘制表示分钟或秒钟的圆点*/
 for(i=0,m=0;i<=59;m++,i++){
    x=100*sin(m/30*PI)+451;
    y=200-100*cos(m/30*PI);
    setlinestyle(0,0,1);
    circle(x,y,1);
 }
 /*在电子表的左边打印帮助提示信息*/
 setcolor(4);
 outtextxy(184,125,"HELP");
 setcolor(15);
 outtextxy(182,125,"HELP");
 setcolor(5);
 outtextxy(140,185,"TAB : Cursor move");
 outtextxy(140,225,"UP  : Time ++");
 outtextxy(140,265,"DOWN: Time --");
 outtextxy(140,305,"ESC : Quit system!");
 outtextxy(140,345,"Version : 2.0");
 setcolor(12);
 outtextxy(150,400,"Nothing is more important than time!");
 clockhandle();/*开始调用时钟处理程序*/
 closegraph(); /*关闭图形系统*/
 return 0; /*表示程序正常结束,向操作系统返回一个0值*/
 }

void clockhandle()
{
 int k=0,count;
 setcolor(15);
 gettime(t);/*取得系统时间,保存在time结构类型的数组变量中*/
 h=t[0].ti_hour;
 m=t[0].ti_min;
 x=50*sin((h*60+m)/360*PI)+451; /*时针的x坐标值*/
 y=200-50*cos((h*60+m)/360*PI); /*时针的y坐标值*/
 line(451,200,x,y);/*在电子表中绘制时针*/

 x1=80*sin(m/30*PI)+451; /*分针的x坐标值*/
 y1=200-80*cos(m/30*PI); /*分针的y坐标值*/
 line(451,200,x1,y1); /*在电子表中绘制分针*/

 digitclock(408,318,digithour(h)); /*在数字时钟中，显示当前的小时值*/
 digitclock(446,318,digitmin(m)); /*在数字时钟中，显示当前的分钟值*/
 setwritemode(1);
 /*规定画线的方式,如果mode=1,则表示画线时用现在特性的线
与所画之处原有的线进行异或(XOR)操作,实际上画出的线是原有线与现在规定
的线进行异或后的结果。因此, 当线的特性不变, 进行两次画线操作相当于没有 
画线，即在当前位置处清除了原来的画线*/
 for(count=2;k!=ESC;){ /*开始循环，直至用户按下ESC键结束循环*/
   setcolor(12);/*淡红色*/
   sound(500);/*以指定频率打开PC扬声器,这里频率为500Hz*/
   delay(700);/*发一个频率为500Hz的音调,维持700毫秒*/
   sound(200);/*以指定频率打开PC扬声器,这里频率为200Hz*/
   delay(300);
   /*以上两种不同频率的音调，可仿真钟表转动时的嘀哒声*/
   nosound(); /*关闭PC扬声器*/
   s=t[0].ti_sec;
   m=t[0].ti_min;
   h=t[0].ti_hour;

   x2=98*sin(s/30*PI)+451; /*秒针的x坐标值*/
   y2=200-98*cos(s/30*PI); /*秒针的y坐标值*/
   line(451,200,x2,y2);
   /*绘制秒针*/

   /*利用此循环,延时一秒*/
   while(t[0].ti_sec==s&&t[0].ti_min==m&&t[0].ti_hour==h)
   {  gettime(t);/*取得系统时间*/
      if(bioskey(1)!=0){
       k=bioskey(0);
       count=keyhandle(k,count);
      if(count==5) count=1;
      }
     }
   setcolor(15);
   digitclock(485,318,digitsec(s)+1);/*数字时钟增加１秒*/

   setcolor(12); /*淡红色*/
   x2=98*sin(s/30*PI)+451;
   y2=200-98*cos(s/30*PI);
   line(451,200,x2,y2);
  /*用原来的颜色在原来位置处再绘制秒针，以达到清除当前秒针的目的*/

   /*分钟处理*/
  if(t[0].ti_min!=m){ /*若分钟有变化*/
         /*消除当前分针*/
         setcolor(15); /*白色*/
         x1=80*sin(m/30*PI)+451;
         y1=200-80*cos(m/30*PI);
         line(451,200,x1,y1);
         /*绘制新的分针*/
         m=t[0].ti_min;
         digitclock(446,318,digitmin(m)); /*在数字时钟中显示新的分钟值*/
         x1=80*sin(m/30*PI)+451;
         y1=200-80*cos(m/30*PI);
         line(451,200,x1,y1);
  }

  /*小时处理*/
  if((t[0].ti_hour*60+t[0].ti_min)!=(h*60+m)){  /*若小时数有变化*/
         /*消除当前时针*/
          setcolor(15); /*白色*/
          x=50*sin((h*60+m)/360*PI)+451;/*50:时钟的长度（单位：像素），451:圆心的x坐标值*/
          y=200-50*cos((h*60+m)/360*PI);
          line(451,200,x,y);
          /*绘制新的时针*/
          h=t[0].ti_hour;
          digitclock(408,318,digithour(h));
          x=50*sin((h*60+m)/360*PI)+451;
          y=200-50*cos((h*60+m)/360*PI);
          line(451,200,x,y);
     }
   }
 }


int keyhandle(int key,int count)   /*键盘控制 */
{ switch(key)
   {case UP: timeupchange(count-1); /*因为count的初始值为２，所以此处减１*/
    break;
    case DOWN:timedownchange(count-1); /*因为count的初始值为２，所以此处减１*/
     break;
    case TAB:setcolor(15);
    clearcursor(count); /*清除原来的光标*/
    drawcursor(count);  /*显示一个新的光标*/
    count++;
    break;
  }
  return count;
 }

int timeupchange(int count) /*处理光标上移的按键*/
{
     if(count==1){
        t[0].ti_hour++;
        if(t[0].ti_hour==24) t[0].ti_hour=0;
        settime(t); /*设置新的系统时间*/
        }

     if(count==2){
        t[0].ti_min++;
        if(t[0].ti_min==60) t[0].ti_min=0;
        settime(t); /*设置新的系统时间*/
        }

    if(count==3){
        t[0].ti_sec++;
        if(t[0].ti_sec==60) t[0].ti_sec=0;
        settime(t); /*设置新的系统时间*/
        }
 }

int timedownchange(int count) /*处理光标下移的按键*/
{
   if(count==1) {
        t[0].ti_hour--;
        if(t[0].ti_hour==0) t[0].ti_hour=23;
        settime(t);/*设置新的系统时间*/
        }
   if(count==2) {
        t[0].ti_min--;
        if(t[0].ti_min==0) t[0].ti_min=59;
        settime(t);/*设置新的系统时间*/
        }

  if(count==3) {
        t[0].ti_sec--;
        if(t[0].ti_sec==0) t[0].ti_sec=59;
        settime(t);/*设置新的系统时间*/
        }
 }


 int digithour(double h)/*将double型的小时数转换成int型*/
{int i;
 for(i=0;i<=23;i++)
  {if(h==i) return i;}
 }

int digitmin(double m)/*将double型的分钟数转换成int型*/
{int i;
 for(i=0;i<=59;i++)
  {if(m==i) return i;}
}

int digitsec(double s) /*将double型的秒钟数转换成int型*/
{int i;
 for(i=0;i<=59;i++)
  {if(s==i) return i;}
}


void digitclock(int x,int y,int clock)/*在指定位置显示数字时钟:时\分\秒*/
{char buffer1[10];
 setfillstyle(0,2);
 bar(x,y,x+15,328);
 if(clock==60) clock=0;
 sprintf(buffer1,"%d",clock);
 outtextxy(x,y,buffer1);
 }

void drawcursor(int count)    /*根据count的值，画一个光标*/
{switch(count)
 {
  case 1:line(424,315,424,325);break;
  case 2:line(465,315,465,325);break;
  case 3:line(505,315,505,325);break;
  }
 }

void clearcursor(int count)    /*根据count的值，清除前一个光标*/
{switch(count)
 {
  case 2:line(424,315,424,325);break;
  case 3:line(465,315,465,325);break;
  case 1:line(505,315,505,325);break;
  }
 }

