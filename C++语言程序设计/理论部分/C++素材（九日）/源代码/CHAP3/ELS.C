/*
 *俄罗斯方块源程序
 */
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <graphics.h>  /*图形函数库*/

/*定义按键码*/
#define VK_LEFT  0x4b00
#define VK_RIGHT 0x4d00
#define VK_DOWN  0x5000
#define VK_UP    0x4800
#define VK_ESC   0x011b
#define TIMER 0x1c      /*设置中断号*/

/*定义常量*/
#define MAX_BOX 19          /*总共有19种各形态的方块*/
#define BSIZE 20            /*方块的边长是20个象素*/
#define Sys_x 160           /*显示方块界面的左上角x座标*/
#define Sys_y 25            /*显示方块界面的左上角y座标*/
#define Horizontal_boxs 10  /*水平的方向以方块为单位的长度*/
#define Vertical_boxs 15    /*垂直的方向以方块为单位的长度,也就说长是15个方块*/
#define Begin_boxs_x Horizontal_boxs/2  /*产生第一个方块时出现的起始位置*/

#define FgColor 3       /*前景颜色,如文字.2-green*/
#define BgColor 0       /*背景颜色.0-blac*/

#define LeftWin_x Sys_x+Horizontal_boxs*BSIZE+46        /*右边状态栏的x座标*/

#define false 0
#define true 1
/*移动的方向*/
#define MoveLeft 1
#define MoveRight 2
#define MoveDown 3
#define MoveRoll 4
/*以后坐标的每个方块可以看作是像素点是BSIZE*BSIZE的正方形*/
/*定义全局变量*/
int current_box_numb;   /*保存当前方块编号*/
int Curbox_x=Sys_x+Begin_boxs_x*BSIZE,Curbox_y=Sys_y;/*x,y是保存方块的当前坐标的*/
int flag_newbox=false;  /*是否要产生新方块的标记0*/
int speed=0;            /*下落速度*/
int score=0;            /*总分*/
int speed_step=30;       /*每等级所需要分数*/
void interrupt (*oldtimer)(void);/* 指向原来时钟中断处理过程入口的中断处理函数指针 */

struct BOARD            /*游戏底板结构,表示每个点所具有的属性*/
{
    int var;            /*当前状态 只有0和1,1表示此点已被占用*/
    int color;          /*颜色,游戏底板的每个点可以拥有不同的颜色.增强美观*/
}Table_board[Vertical_boxs][Horizontal_boxs];

/*方块结构*/
struct SHAPE
{
    char box[2];            /*一个字节等于8位,每4位来表示一个方块的一行
                              如:box[0]="0x88",box[1]="0xc0"表示的是:
                              1000
                              1000
                              1100
                              0000*/
    int color;              /*每个方块的颜色*/
    int next;               /*下个方块的编号*/
};


/*初始化方块内容.即定义MAX_BOX个SHAPE类型的结构数组，并初始化*/
struct SHAPE shapes[MAX_BOX]=
{
/*
 *   口     口口口   口口      口
 *   口     口         口  口口口
 *   口口              口        
 */

    {0x88,  0xc0,   CYAN,   1},
    {0xe8,  0x0,    CYAN,   2},
    {0xc4,  0x40,   CYAN,   3},
    {0x2e,  0x0,    CYAN,   0},
/*
 *   口        口口 口口口
 *   口 口     口       口
 * 口口 口口口 口    
 */
    {0x44,  0xc0,   MAGENTA,  5},
    {0x8e,  0x0,    MAGENTA,  6},
    {0xc8,  0x80,   MAGENTA,  7},
    {0xe2,  0x0,    MAGENTA,  4},

/*
 *   口
 *   口口         口口
 *     口       口口
 */
    {0x8c,  0x40,   YELLOW, 9},
    {0x6c,  0x0,    YELLOW, 8},

/*
 *   口        口口
 * 口口          口口
 * 口
 */
    {0x4c,  0x80,   BROWN,  11},
    {0xc6,  0x0,    BROWN,  10},

/*
 *   口       口                口
 * 口口口     口口   口口口   口口
 *            口       口       口
 */
    {0x4e,  0x0,    WHITE,  13},
    {0x8c,  0x80,   WHITE,  14},
    {0xe4,  0x0,    WHITE,  15},
    {0x4c,  0x40,   WHITE,  12},

/* 口
 * 口
 * 口       口口口口
 * 口
 */
    {0x88,  0x88,   RED,    17},
    {0xf0,  0x0,    RED,    16},

/*
 * 口口
 * 口口
 */

    {0xcc,  0x0,    BLUE,   18}

};


unsigned int TimerCounter=0;  /*定时计数器变量*/

/* 新的时钟中断处理函数 */
void interrupt newtimer(void)
{
    (*oldtimer)();   /* call the old routine */
    TimerCounter++;/* increase the global counter */
}

/* 设置新的时钟中断处理过程 */
void SetTimer(void interrupt(*IntProc)(void))
{
    oldtimer=getvect(TIMER); /*获取中断号为TIMER的中断处理函数的入口地址*/
    disable(); /* 设置新的时钟中断处理过程时，禁止所有中断 */
    setvect(TIMER,IntProc);
    /*将中断号为TIMER的中断处理函数的入口地址改为IntProc()函数的入口地址
    即中断发生时，将调用IntProc()函数。*/
    enable();  /* 开启中断 */
}

/* 恢复原有的时钟中断处理过程 */
void KillTimer()
{
    disable();
    setvect(TIMER,oldtimer);
    enable();
}


void show_intro(int xs,int ys)
{
 char stemp[50];
 setcolor (15);
 rectangle(xs,ys,xs+239,ys+100);
 sprintf(stemp," -Roll  -Downwards");
 stemp[0]=24;
 stemp[8]=25;
 setcolor(14);
 outtextxy(xs+40,ys+30,stemp);

 sprintf(stemp," -Turn Left   -Turn Right");
 stemp[0]=27;
 stemp[13]=26;

 outtextxy(xs+40,ys+45,stemp);
 outtextxy(xs+40,ys+60,"Esc-Exit ");
 setcolor(FgColor);
}

/*显示分数*/
void ShowScore(int score)
{
    int x,y;
    char score_str[5];/*保存游戏得分*/
    setfillstyle(SOLID_FILL,BgColor);
    x=LeftWin_x;
    y=100;
    bar(x-BSIZE,y,x+BSIZE*3,y+BSIZE*3);
    sprintf(score_str,"%3d",score);
    outtextxy(x,y,"SCORE");
    outtextxy(x,y+10,score_str);
}

/*显示速度*/
void ShowSpeed(int speed)
{
    int x,y;
    char speed_str[5];/*保存速度值*/
    setfillstyle(SOLID_FILL,BgColor);
    x=LeftWin_x;
    y=150;
    bar(x-BSIZE,y,x+BSIZE*3,y+BSIZE*3);
    /*确定一个以(x1,y1)为左上角,(x2,y2)为右下角的矩形窗口, 再按规定图模和颜色填充。*/
    sprintf(speed_str,"%3d",speed+1);
    outtextxy(x,y,"Level");
    outtextxy(x,y+10,speed_str);
    /*输出字符串指针speed_str所指的文本在规定的(x, y)位置*/
    outtextxy(x,y+50,"Nextbox");
}


/**********初始化界面*******
 *参数说明:
 *      x,y为左上角坐标
 *      m,n对应于Vertical_boxs,Horizontal_boxs
 *      分别表示纵横方向上方块的个数(以方块为单位)
 *      BSIZE Sys_x Sys_y
 **********************************/
void initialize(int x,int y,int m,int n)
{

    int i,j,oldx;
    oldx=x;
    for(j=0;j<n;j++)
    {
        for(i=0;i<m;i++)
        {

            Table_board[j][i].var=0;
            Table_board[j][i].color=BgColor;
            line(x,y,x+BSIZE,y);
            line(x,y,x,y+BSIZE);
            line(x,y+BSIZE,x+BSIZE,y+BSIZE);
            line(x+BSIZE,y,x+BSIZE,y+BSIZE);
            x+=BSIZE;
        }
        y+=BSIZE;
        x=oldx;
    }
    Curbox_x=x;
    Curbox_y=y;/*x,y是保存方块的当前坐标的*/
    flag_newbox=false;  /*是否要产生新方块的标记0*/
    speed=0;            /*下落速度*/
    score=0;            /*总分*/
    ShowScore(score);
    ShowSpeed(speed);

}


/*   删除一行满的情况
 *   这里的y为具体哪一行为满
 */
int DelFullRow(int y)
{
    /*该行游戏板往下移一行*/
    int n,top=0;                /*top保存的是当前最高点,出现一行全空就表示为最点了,移动是到最高点结束*/
    register m,totoal;
    for(n=y;n>=0;n--)/*从当前行往上看*/
    {
        totoal=0;
        for(m=0;m<Horizontal_boxs;m++)
        {
            if(!Table_board[n][m].var)totoal++;             /*没占有方格+1*/
            if(Table_board[n][m].var!=Table_board[n-1][m].var)  /*上行不等于下行就把上行传给下行 xor关系*/
            {
                Table_board[n][m].var=Table_board[n-1][m].var;
                Table_board[n][m].color=Table_board[n-1][m].color;
            }
        }
        if(totoal==Horizontal_boxs)                     /*发现上面有连续的空行提前结束*/
        {
            top=n;
            break;
        }
    }
    return(top);                /*返回最高点*/
}


/*找到一行满的情况*/
void setFullRow(int t_boardy)
{
    int n,full_numb=0,top=0;            /*top保存的是当前方块的最高点*/
    register m;
/*
t_boardy 口       5
         口       6
   口口口口口口   7
n  口口口口口口   8
 */
    for(n=t_boardy+3;n>=t_boardy;n--)
    {
        if(n<0 || n>=Vertical_boxs ){continue;} /*超过低线了*/
        for(m=0;m<Horizontal_boxs;m++)          /*水平的方向*/
        {
            if(!Table_board[n+full_numb][m].var)break;      /*发现有一个是空就跳过该行*/
        }
        if(m==Horizontal_boxs)                  /*找到满行了*/
        {
            if(n==t_boardy+3)                   /*第一次献给了n,最高的*/
                top=DelFullRow(n+full_numb);        /*清除游戏板里的该行,并下移数据*/
            else
                DelFullRow(n+full_numb);
            full_numb++;                        /*统计找到的行数*/

        }
    }
    if(full_numb)
    {
        int oldx,x=Sys_x,y=BSIZE*top+Sys_y;
        oldx=x;
        score=score+full_numb*10;              /*加分数*/
        /*这里相当于重显调色板*/
        for(n=top;n<t_boardy+4;n++)
        {
            if(n>=Vertical_boxs)continue;   /*超过低线了*/
            for(m=0;m<Horizontal_boxs;m++)  /*水平的方向*/
            {
                if(Table_board[n][m].var)
                    setfillstyle(SOLID_FILL,Table_board[n][m].color);/*Table_board[n][m].color*/
                else
                    setfillstyle(SOLID_FILL,BgColor);

                bar(x,y,x+BSIZE,y+BSIZE);
                line(x,y,x+BSIZE,y);
                line(x,y,x,y+BSIZE);
                line(x,y+BSIZE,x+BSIZE,y+BSIZE);
                line(x+BSIZE,y,x+BSIZE,y+BSIZE);
                x+=BSIZE;
            }
            y+=BSIZE;
            x=oldx;
        }
        ShowScore(score);
        if(speed!=score/speed_step)
          {speed=score/speed_step; ShowSpeed(speed);}
        else
          {ShowSpeed(speed);}
    }
}


/*
 * 将新形状的方块放置在游戏板上，并返回此方块号
 */
int MkNextBox(int box_numb)
{

    int mask=128,t_boardx,t_boardy,n,m;
    t_boardx=(Curbox_x-Sys_x)/BSIZE;
    t_boardy=(Curbox_y-Sys_y)/BSIZE;
    for(n=0;n<4;n++)
    {
        for(m=0;m<4;m++)
        {
            if( ((shapes[current_box_numb].box[n/2]) & mask) )
            {
                Table_board[t_boardy+n][t_boardx+m].var=1;/*这里设置游戏板*/
                Table_board[t_boardy+n][t_boardx+m].color=shapes[current_box_numb].color;/*这里设置游戏板*/
            }
            mask=mask/(2);
            if(mask==0)mask=128;
        }
    }
    setFullRow(t_boardy);
    Curbox_x=Sys_x+Begin_boxs_x*BSIZE,Curbox_y=Sys_y;/*再次初始化座标*/
    if(box_numb==-1) box_numb=rand()%MAX_BOX;
    current_box_numb=box_numb;
    flag_newbox=false;
    return(rand()%MAX_BOX);
}

/*
 *  擦除(x,y)位置开始的编号为box_numb的box.
 */
void EraseBox(int x,int y,int box_numb)
{
    int mask=128,t_boardx,t_boardy,n,m;
    setfillstyle(SOLID_FILL,BgColor);
    for(n=0;n<4;n++)
    {
        for(m=0;m<4;m++)    /*看最左边四个单元*/
        {
            if( ((shapes[box_numb].box[n/2]) & mask) )/*最左边有方块并且当前游戏板也有方块*/
            {
                bar(x+m*BSIZE,y+n*BSIZE,x+m*BSIZE+BSIZE,y+n*BSIZE+BSIZE);
                line(x+m*BSIZE,y+n*BSIZE,x+m*BSIZE+BSIZE,y+n*BSIZE);
                line(x+m*BSIZE,y+n*BSIZE,x+m*BSIZE,y+n*BSIZE+BSIZE);
                line(x+m*BSIZE,y+n*BSIZE+BSIZE,x+m*BSIZE+BSIZE,y+n*BSIZE+BSIZE);
                line(x+m*BSIZE+BSIZE,y+n*BSIZE,x+m*BSIZE+BSIZE,y+n*BSIZE+BSIZE);
            }
            mask=mask/(2);
            if(mask==0)mask=128;
        }
    }

}



 void ErasePreBox(int x,int y,int box_numb)
{
    int mask=128,t_boardx,t_boardy,n,m;
    setfillstyle(SOLID_FILL,BgColor);
    for(n=0;n<4;n++)
    {
        for(m=0;m<4;m++)    /*看最左边四个单元*/
        {
            if( ((shapes[box_numb].box[n/2]) & mask) )/*最左边有方块并且当前游戏板也有方块*/
            {
                bar(x+m*BSIZE,y+n*BSIZE,x+m*BSIZE+BSIZE,y+n*BSIZE+BSIZE);

            }
            mask=mask/(2);
            if(mask==0)mask=128;
        }
    }

}

/* 判断是否可以移动
 * x,y为当前方块位置
 * box_numb为方块号
 * direction 方向标志 
 *  返回true 和false
 #define MoveLeft -1
 #define MoveRight 1
 #define MoveDown 0
 */
int MoveAble(int x,int y,int box_numb,int direction)
{
    int n,m,t_boardx,t_boardy;          /*t_boardx 当前方块最左边在游戏板的位置*/
    int mask;
    if(direction==MoveLeft)             /*如果向左移*/
    {
        mask=128;
        x-=BSIZE;
        t_boardx=(x-Sys_x)/BSIZE;
        t_boardy=(y-Sys_y)/BSIZE;
        for(n=0;n<4;n++)
        {
            for(m=0;m<4;m++)    /*看最左边四个单元*/
            {
                if((shapes[box_numb].box[n/2]) & mask)/*最左边有方块并且当前游戏板也有方块*/
                {
                    if((x+BSIZE*m)<Sys_x)return(false);/*碰到最左边了*/
                    else if(Table_board[t_boardy+n][t_boardx+m].var) /*左移一个方块后，此4*4的区域与游戏板有冲突*/
                    {
                        return(false);
                    }
                }
                mask=mask/(2);
                if(mask==0)mask=128;
            }
        }
        return(true);
    }
    else if(direction==MoveRight)       /*如果向右移*/
    {
        x+=BSIZE;
        t_boardx=(x-Sys_x)/BSIZE;
        t_boardy=(y-Sys_y)/BSIZE;
        mask=128;
        for(n=0;n<4;n++)
        {
            for(m=0;m<4;m++)    /*看最右边四个单元*/
            {
                if((shapes[box_numb].box[n/2]) & mask)/*最右边有方块并且当前游戏板也有方块*/
                {
                    if((x+BSIZE*m)>=(Sys_x+BSIZE*Horizontal_boxs) )return(false);/*碰到最右边了*/
                    else if( Table_board[t_boardy+n][t_boardx+m].var)
                    {
                        return(false);
                    }
                }
                mask=mask/(2);
                if(mask==0)mask=128;
            }
        }
        return(true);
    }
    else if(direction==MoveDown)        /*如果向下移*/
    {
        y+=BSIZE;
        t_boardx=(x-Sys_x)/BSIZE;
        t_boardy=(y-Sys_y)/BSIZE;
        mask=128;
        for(n=0;n<4;n++)
        {
            for(m=0;m<4;m++)    /*看最下边四个单元*/
            {
                if((shapes[box_numb].box[n/2]) & mask)/*最下边有方块并且当前游戏板也有方块*/
                {
                    if((y+BSIZE*n)>=(Sys_y+BSIZE*Vertical_boxs) ||  Table_board[t_boardy+n][t_boardx+m].var)
                    {
                        flag_newbox=true;
                        break;
                    }
                }
                mask=mask/(2);
                /*mask依次为:10000000,01000000,00100000,00010000
                             00001000,00000100,00000010/00000001
                 */
                if(mask==0)mask=128;
            }
        }
        if(flag_newbox)
        {
            return(false);
        }
        else
            return(true);
    }
    else if(direction==MoveRoll)        /*转动*/
    {
        t_boardx=(x-Sys_x)/BSIZE;
        t_boardy=(y-Sys_y)/BSIZE;
        mask=128;
        for(n=0;n<4;n++)
        {
            for(m=0;m<4;m++)    /*看最下边四个单元*/
            {
                if((shapes[box_numb].box[n/2]) & mask)/*最下边有方块并且当前游戏板也有方块*/
                {
                    if((y+BSIZE*n)>=(Sys_y+BSIZE*Vertical_boxs) )return(false);/*碰到最下边了*/
                    if((x+BSIZE*n)>=(Sys_x+BSIZE*Horizontal_boxs) )return(false);/*碰到最左边了*/
                    if((x+BSIZE*m)>=(Sys_x+BSIZE*Horizontal_boxs) )return(false);/*碰到最右边了*/

                    else if( Table_board[t_boardy+n][t_boardx+m].var)
                    {
                        return(false);
                    }
                }
                mask=mask/(2);
                if(mask==0)mask=128;
            }
        }
        return(true);
    }
    else
    {
        return(false);
    }

}

/*
 *显示指定的方块
 */
void show_box(int x,int y,int box_numb,int color)
{
    int i,ii,ls_x=x;
    if(box_numb<0 || box_numb>=MAX_BOX)/*指定的方块不存在*/
        box_numb=MAX_BOX/2;
    setfillstyle(SOLID_FILL,color);
/********************************* 
 *   移位来判断第哪一位是1
 *   方块是每1行用半个字节来表示
 *   128d=1000 0000b
 *********************************/
    for(ii=0;ii<2;ii++) 
    {
        int mask=128;
        for(i=0;i<8;i++)
        {
            if(i%4==0 && i!=0)              /*表示转到方块的下一行了*/
            {
                y+=BSIZE;
                x=ls_x;
            }
                       if((shapes[box_numb].box[ii])&mask)
            {
                bar(x,y,x+BSIZE,y+BSIZE);
                line(x,y,x+BSIZE,y);
                line(x,y,x,y+BSIZE);
                line(x,y+BSIZE,x+BSIZE,y+BSIZE);
                line(x+BSIZE,y,x+BSIZE,y+BSIZE);
            }
            x+=BSIZE;
            mask/=2;
        }
        y+=BSIZE;
        x=ls_x;
    }
}




void main()
{
    int GameOver=0;
    int key,nextbox; 
    int Currentaction=0;/*标记当前动作状态*/
    int gd=VGA,gm=VGAHI,errorcode;

    initgraph(&gd,&gm,"");
    errorcode = graphresult(); 
    if (errorcode != grOk)  
    {
        printf("\nNotice:Graphics error: %s\n", grapherrormsg(errorcode));
        printf("Press any key to quit!");
        getch(); 
        exit(1);            
    }
    setbkcolor(BgColor);
    setcolor(FgColor);
    randomize();
    SetTimer(newtimer);

    initialize(Sys_x,Sys_y,Horizontal_boxs,Vertical_boxs);/*初始化*/


    nextbox=MkNextBox(-1);

    show_box(Curbox_x,Curbox_y,current_box_numb,shapes[current_box_numb].color);
    show_box(LeftWin_x,Curbox_y+200,nextbox,shapes[nextbox].color);
    show_intro(Sys_x,Curbox_y+320);
    getch();
    while(1)
    {
        /* Currentaction=0;
        flag_newbox=false;
        检测是否有按键*/
        if (bioskey(1)){key=bioskey(0);     }
        else            {           key=0;      }

        switch(key)
        {
            case VK_LEFT:
                if(MoveAble(Curbox_x,Curbox_y,current_box_numb,MoveLeft))
                {EraseBox(Curbox_x,Curbox_y,current_box_numb);Curbox_x-=BSIZE;Currentaction=MoveLeft;}
                break;
            case VK_RIGHT:
                if(MoveAble(Curbox_x,Curbox_y,current_box_numb,MoveRight))
                {EraseBox(Curbox_x,Curbox_y,current_box_numb);Curbox_x+=BSIZE;Currentaction=MoveRight;}
                break;
            case VK_DOWN:
                if(MoveAble(Curbox_x,Curbox_y,current_box_numb,MoveDown))
                {EraseBox(Curbox_x,Curbox_y,current_box_numb);Curbox_y+=BSIZE;Currentaction=MoveDown;}
                else flag_newbox=true;
                break;
            case VK_UP:/*旋转方块*/
                if(MoveAble(Curbox_x,Curbox_y,shapes[current_box_numb].next,MoveRoll))
                {EraseBox(Curbox_x,Curbox_y,current_box_numb);current_box_numb=shapes[current_box_numb].next;
                    Currentaction=MoveRoll;
                }
                break;
            
            case VK_ESC:
                GameOver=1;
                break;
            default:
                break;
        }

        if(Currentaction)
        {   /*表示当前有动作,移动或转动*/
            show_box(Curbox_x,Curbox_y,current_box_numb,shapes[current_box_numb].color);
            Currentaction=0;
        }

            /*按了往下键，但不能下移,就产生新方块*/
        if(flag_newbox)
        {
            /*这时相当于方块到底部了,把其中出现点满一行的清去，置0*/
            ErasePreBox(LeftWin_x,Sys_y+200,nextbox);
            nextbox=MkNextBox(nextbox);
            show_box(LeftWin_x,Curbox_y+200,nextbox,shapes[nextbox].color);
            if(!MoveAble(Curbox_x,Curbox_y,current_box_numb,MoveDown))/*刚一开始,游戏结束*/
            {
                show_box(Curbox_x,Curbox_y,current_box_numb,shapes[current_box_numb].color);
                GameOver=1;
            }
            else
            {
                flag_newbox=false;
            }
            Currentaction=0;
        }
        else    /*自由下落*/
        {
            if (Currentaction==MoveDown || TimerCounter> (20-speed*2))
            {
                if(MoveAble(Curbox_x,Curbox_y,current_box_numb,MoveDown))
                {
                    EraseBox(Curbox_x,Curbox_y,current_box_numb);Curbox_y+=BSIZE;
                    show_box(Curbox_x,Curbox_y,current_box_numb,shapes[current_box_numb].color);
                }
                TimerCounter=0;
            }
        }

        if(GameOver )/*|| flag_newbox==-1*/
        {
            printf("game over,thank you! your score is %d",score);
            getch();
            break;
        }
    }
    getch();
    KillTimer();
    closegraph();
}
