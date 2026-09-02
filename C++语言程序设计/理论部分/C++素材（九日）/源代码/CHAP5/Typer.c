/**************************打字游戏*******************************/
/*加载头文件*/
# include <stdlib.h>
# include <stdio.h>
# include <graphics.h>
# include <dos.h>
/* 定义一些通用的宏*/
# define  BYTE         unsigned char
# define  BOOL         BYTE
# define  WORD         unsigned int
# define  DWORD        unsigned long

# define  TRUE         1
# define  FALSE        !TRUE

# define  KEY_ESC      27
# define  KEY_SPACE    32

# define  MAX_LETTER   30     /* 最大的字符数目 */
# define  MAX_HEIGHT   400    /* 字符下落的最大高度 */

# define  MAXTIMER     10    /* 系统可用定时器的最大数目 */
# ifndef  NULL
#   define NULL        0
# endif
/*****************定时器相关数据结构与函数定义*********************/
struct TIMER   /* 定时器结构 */
{
  DWORD Interval;              /*      间隔       */
  DWORD LastTimer;             /* 上次事件发生时间*/
  BOOL  Enable;                /*      活动       */
  BOOL  Used;                  /*      可用       */
  void  (*Pointer)();          /* 计算器void类型指针,用于指向系统时间的句柄 */
};
struct TIMER tmTM[MAXTIMER+1]; /*全局结构数据，存储定时器个数*/
int    TimerUsed=0; /*全局变量，存储定时器当前已用个数*/
/*处理定时器相关函数*/
DWORD BiosTimer(void);      /* 获取BIOS计数器数值 */
void TimerEvent();          /* 事件定时器超时更新*/
int CreateTimer(DWORD Interval,void (*Pointer)());
/* 创建一个定时器（若成功返回时钟的句柄，否则返回NULL） */
void KillTimer(int *TimerID);  /* 删除一个定时器 */
void KillAllTimer();  /* 删除所有定时器 */
/***********************************************************/

/****************字符对象数据结构及处理函数定义*****************/
/* 定义字符对象结构*/
typedef struct
{
    int  x;              /*  字符出现的坐标X   */
    int  y;              /*  字符出现的坐标Y   */
    BYTE val;            /*  字符ASCll码       */
    BOOL Used;           /* 当前字符是否已经使用  */
}LETTER;

LETTER Letter[MAX_LETTER];    /* 字符对象数组 */
int    Step=1;                /* 字符下落步长 */
DWORD  Hits=0;                /* 击键次数 */
DWORD  Right=0;               /* 正确个数 */
int    Interval=18;           /* 字符产生间隔,系统时钟中断大约每秒钟发生18.2次*/
int    LetterUsed=0;          /* 已用的字符个数 */

/*处理字符相关函数*/
BYTE AscllKey();             /* 取按键的ASCLL码(不等待) */
void ShowBar();            /* 显示状态栏图 */
void KillLetter();         /* 删除字符 */
void SelectLevel();        /* 选择难度 */
void CreateLetter();       /* 创建字符 */
void SetGraphMode();       /* 设置图形模式 */
void Check(BYTE Key);      /* 检测是否击中 */
void CloseGraphMode();     /* 关闭图形模式 */
void MoveLetter(void);     /* 移动字符 */
void HideLetter(int ID);   /* 隐藏字符*/
void ShowLetter(int ID);   /* 显示字符 */
/***********************************************************/


/* 获取BIOS计数器数值 */
DWORD BiosTimer(void)
{
  DWORD BIOSTIMER=0;
  /*dos.h中的peek函数用于读取指定存储单元中的内容*/
  /*原型：int peek(int segment, unsigned offset)*/
  BIOSTIMER=peek(0x0,0x46e);
  BIOSTIMER<<=8; /*左移8位,空出位置用于拼接下一个数值*/
  BIOSTIMER+=peek(0x0,0x46c);
  /*先从0x46e处读出一个字节，然后从0x46c处读一个字节，然后拼成一个16位的数(系统时间）*/
  /*注：Dos系统中，规定此处固定地存放系统时间*/
  return (BIOSTIMER);
}

/* 事件定时器超时更新*/
void TimerEvent()
{
  int   i;
  DWORD TimerDiff;
  for (i=1;i<=MAXTIMER;i++) /*遍历每一个事件定时器*/
  {
      if (tmTM[i].Used&&tmTM[i].Enable) /*若定时器可用且是可活动的*/
      {
         TimerDiff=BiosTimer()-tmTM[i].LastTimer;
         /*计算单个事件发生时间与当前系统的时间间隔*/
        if (tmTM[i].Interval<=TimerDiff) /*若此事件定时器超时*/
         {
            tmTM[i].Pointer(); /*更新指向系统时间的句柄*/
            tmTM[i].LastTimer=BiosTimer(); /*更新事件发生时间为当前时间*/
         }
      }
  }
}

/* 启动一个定时器（若成功返回时钟的句柄，否则返回NULL） */
int CreateTimer(DWORD Interval,void (*Pointer)())
{
  int i=0;
  if (TimerUsed==MAXTIMER) return NULL; /*定时器个数已用完*/
  while (tmTM[++i].Used); /*检测可用定时器，一时某定时器可用退出循环*/
    /*更新定时器各参数值*/
    tmTM[i].Pointer=Pointer;
    tmTM[i].Interval=Interval;
    tmTM[i].Enable=TRUE;
    tmTM[i].Used=TRUE;
    tmTM[i].LastTimer=BiosTimer();
  TimerUsed++; /*已用定时器个数增加1*/
  return i;
}


/* 删除一个定时器 */
void KillTimer(int *TimerID)
{
  if (tmTM[*TimerID].Used)
  {
     TimerUsed--; /*已用定时器个数减1*/
     tmTM[*TimerID].Used=FALSE;
  }
  *TimerID=0;
}

/* 删除所有定时器 */
void KillAllTimer()
{
  int i;
  for (i=0;i<=MAXTIMER;i++) tmTM[i].Used=FALSE;
  TimerUsed=0;
}


/* 设置系统图形模式 */
void SetGraphMode()
{
  int Device=VGA,Mode=VGAHI;
  initgraph(&Device,&Mode,"");
  /*初始化屏幕为图形模式,表示图形驱动器和模式分别为VGA与VGAHI*/
  settextstyle(TRIPLEX_FONT,HORIZ_DIR,1);
  /*设置输出字符的字型为TRIPLEX_FONT、方向为HORIZ_DIR和字体大小为1*/
  /*其中TRIPLEX_FONT为三倍比划字体HORIZ_DIR表示从左向右输出字符*/
  setfillstyle(SOLID_FILL,0);
  /*设定填充方式:填充模式为SOLID_FILL实填充,填充颜色为0*/
  setcolor(7);
  /*设置输出文本的颜色，color表示要设置的颜色，此处为7*/
}

void CloseGraphMode()
{
  restorecrtmode(); /*将屏幕模式恢复为先前默认的initgraph设置，一般为文本模式*/
}

/* 从内存中的键盘缓冲区中直接读出ASCLL码（不等待） */
/* BIOS程序驻留在ROM中，但它使用了256个字节的RAM作为数据区*/
/* 绝对地址在0x0400-0x04FF中*/
BYTE AscllKey(void)
{
  int  start,end;
  WORD key=0;
  start=peek(0,0x41a); /*0表示参考点，0x41a为偏移值*/
  /*0x41a即十进制的1050,表示从内存单元1050(0x41a)中读入一个字节的按键码值*/
  /*按键码值的高四位为扫描码，低四位为对应的ASCLL码*/
  end=peek(0,0x41c); /*0x41c即1052*/
  /*一般最后一个按键保存在1052处*/
  if (start==end) return(0); /*表示当前没有新的按键*/
  else
  {
    key=peek(0x40,start);  /*0x40即十进制64*/
    start+=2;
    if (start==0x3e) start=0x1e;
    poke(0x40,0x1a,start);/*将start的值写回指定内存区域0x40+0x1a处*/
    return(key&0xff);
  }
}

/* 创建一个字符*/
void CreateLetter()
{
  int  i=0;
  int  x;
  BYTE val;
  if (LetterUsed==MAX_LETTER) return;   /* 无字符可用则返回 */
  while (Letter[++i].Used);     /* 找到第一个空闲的字符对象，产生一个字符对象 */
  x=i;
  Letter[i].x=x*640/MAX_LETTER; /*x表示字符出现的列位置*/
  Letter[i].y=0;                /*y表示在第0行出现*/
  Letter[i].val=random(26)+'A'; /*random(26)随机产生一个0-25之间的数*/
  Letter[i].Used=TRUE; /*标记该字符已经被使用*/
  LetterUsed++; /*使用的字符总数加1*/
}

/* 隐藏一个字符*/
void HideLetter(int ID)
{
  /* 用填充矩形来消隐字符 */
  bar(Letter[ID].x,Letter[ID].y,Letter[ID].x+16,Letter[ID].y+20);
  /*先画一个以（x，y）为左上角、（x+16，y+20）为右下角的矩形窗口，再按规定模式和颜色填充*/
}

/*显示一个字符*/
void ShowLetter(int ID)
{
  char str[2]={0,0};
  str[0]=Letter[ID].val;
  setcolor(15);
  outtextxy(Letter[ID].x,Letter[ID].y,str);
}

/* 检测是否击中 */
void Check(BYTE Key)
{
  int  i;
  char str[6];
  Hits++; /*击键总次数加1*/
  for (i=0;i<MAX_LETTER;i++)
      /* 击中判断，toupper()把字符转换成大写字母 */
      if (Letter[i].Used&&Letter[i].val==toupper(Key))
      {
       sound(1000); /*以指定频率1000打开PC扬声器*/
       delay(10); /*将程序的执行暂停10毫秒,即扬声器发声10ms*/
       KillLetter(i); /* 删除该字符*/
       Right++; /*总正确数增加1*/
       nosound();
      }
  /* 显示击键总次数及正确数*/
  setfillstyle(SOLID_FILL,5);
  bar(260,430,320,450);
  bar(410,430,470,450);
  setcolor(13);
  sprintf(str," %4ld",Hits);
  outtextxy(260,432,str);
  sprintf(str," %4ld",Right);
  outtextxy(410,432,str);
  setcolor(7);
  setfillstyle(SOLID_FILL,0);
}

/* 字符向下移动*/
void MoveLetter(void)
{
  int  i;
  /*遍历每个字符*/
  for (i=0;i<MAX_LETTER;i++)
  {
      if (Letter[i].Used)  /*若该字符已经使用*/
      {
         HideLetter(i); /* 隐藏该字符*/
         Letter[i].y+=Step; /*y控制行，增加一个步长*/
         ShowLetter(i); /*在新位置重新显示*/
         /* 字符对象下落到最底部时，该字符消失*/
         if (Letter[i].y>MAX_HEIGHT) KillLetter(i);
      }
  }
}

/*字符删除*/
void KillLetter(int LetterID)
{
  if (Letter[LetterID].Used)
  {
     Letter[LetterID].Used=FALSE; /*字符变为当前没有被使用状态*/
     LetterUsed--; /*已经字符数减1*/
     HideLetter(LetterID);
     /*隐藏该字符*/
  }
  /* 删除字符后马上再创建一个 */
  CreateLetter();
}

/*显示分数状态栏条*/
void ShowBar()
{
  FILE *bmp;
  BYTE r,g,b,t;
  int  i,x,y;
  bmp=fopen("bar.bmp","rb");
  /*以只读的方式打一个二进制图像文件bar.bmp*/
  fseek(bmp,54,SEEK_SET);
  /*fseek()函数用以实现改变位置指针所指向的位置，其调用一般形式为
    fseek（文件类型指针，位移量，起始点）
    起始点有三个值，分别是文件开始（名字SEEK_SET，用数字0表示）、
    文件当前位置（名字SEEK_CUR，用数字1表示）和文件末尾（名字SEEK_END，用数字2表示)*/
  for (i=0;i<16;i++)
  {
      setpalette(i,i); /*改变调色板的颜色*/
      b=fgetc(bmp)>>2; /*fgetc()函数用于从一个指定的文件中读出一个字符*/
      g=fgetc(bmp)>>2;
      r=fgetc(bmp)>>2;
      t=fgetc(bmp)>>2;
      setrgbpalette(i,r,g,b); /*定义IBM8514图形卡的颜色*/
  }
  for (y=0;y<80;y++)
      for (x=0;x<320;x++)
      {
      t=fgetc(bmp);
      putpixel(x*2,479-y,t>>4); /*putpixel在指定位置画一像素*/
      putpixel(x*2+1,479-y,t&15);
      }
  fclose(bmp); /*关闭文件*/
}

/*选择等级界面*/
void SelectLevel()
{
  int  Steps[3]={1,2,4}; /*根据等级不同，字符的下降步长不同*/
  int  Intervals[3]={18,9,5}; /*根据等级不同，字符的出现的时间间隔不同*/
  int  Sel=0;
  FILE *bmp;
  BYTE r,g,b,t,Key;
  int  i,x,y;

  /*显示图形界面,与ShowBar类同*/
  bmp=fopen("welcome.bmp","rb");
  fseek(bmp,54,SEEK_SET);
  for (i=0;i<16;i++)
  {
      setpalette(i,i);
      b=fgetc(bmp)>>2;
      g=fgetc(bmp)>>2;
      r=fgetc(bmp)>>2;
      t=fgetc(bmp)>>2;
      setrgbpalette(i,r,g,b);
  }
  for (y=0;y<200;y++)
      for (x=0;x<160;x++)
      {
      t=fgetc(bmp);
      putpixel(x*2+160,339-y,t>>4);
      putpixel(x*2+161,339-y,t&15);
      }
  fclose(bmp);
  /*根据用户按键，确定玩家等级*/
  while (TRUE)
  {
    Key=toupper(AscllKey());
    if (Key=='1') Sel=1;
    if (Key=='2') Sel=2;
    if (Key=='3') Sel=3;
    if (Sel) break;
  }
  Step=Steps[Sel-1];
  Interval=Intervals[Sel-1];
  cleardevice();
}

void main()
{
  BOOL bQuit=FALSE;                 /* 是否退出变量初始化为假 */
  BOOL bPause=FALSE;                /* 是否暂停变量初始化为假 */
  int  tm1,tm2;
  BYTE Key;

  randomize();                    /* 初始化随机数种子 */
  SetGraphMode();                 /* 设置图形模式参数*/
  SelectLevel();                  /* 选择等级*/
  ShowBar();                      /*显示状态栏及成绩*/
  tm1=CreateTimer(1,MoveLetter);            /* 创建移动字符定时器 */
  tm2=CreateTimer(Interval,CreateLetter);   /* 创建产生字符定时器*/
  CreateLetter(); /*创建字符*/
  /*读取按键值*/
  Key=AscllKey();
  while (!bQuit)   /*若没有按下ESC键退出*/
  {
    TimerEvent(); /* 事件定时器超时更新*/
    switch (Key)  /* 判定键值*/
    {
      case NULL:  /* 若为0则跳出switch语句*/
        break;
      case KEY_ESC: /*若为ESC键，退出游戏*/
       bQuit=TRUE;
       break;
      case KEY_SPACE: /*若为SPACE键，暂停游戏*/
       bPause=!bPause;
       tmTM[tm1].Enable=!bPause;
       tmTM[tm2].Enable=!bPause;
       break;
      default: /*若上述都不满足，则进行击中判断处理*/
       if (!bPause) Check(Key);
    }
    Key=AscllKey(); /*继续读按键*/
  }
  CloseGraphMode(); /*关键图形模式退出*/
}
