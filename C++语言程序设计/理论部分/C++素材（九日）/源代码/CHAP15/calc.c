#include "stdio.h"    /* 标准输入输出库      */
#include "string.h"   /* 字符串操作函数库    */
#include "math.h"     /* 数学函数库          */
#include "stdlib.h"   /* 标准函数库          */
#include "graphics.h" /* 图形函数库          */
#include "bios.h"     /* 基本输入输出系统函数库*/
#include "dos.h"      /* dos函数库           */

#define NUM0       0x5230    /* 小键盘区上数字键0  */
#define NUM1       0x4f31    /* 小键盘区上数字键1  */
#define NUM2       0x5032    /* 小键盘区上数字键2  */
#define NUM3       0x5133    /* 小键盘区上数字键3  */
#define NUM4       0x4b34    /* 小键盘区上数字键4  */
#define NUM5       0x4c35    /* 小键盘区上数字键5  */
#define NUM6       0x4d36    /* 小键盘区上数字键6  */
#define NUM7       0x4737    /* 小键盘区上数字键7  */
#define NUM8       0x4838    /* 小键盘区上数字键8  */
#define NUM9       0x4939    /* 小键盘区上数字键9  */
#define NUMPNT     0x532e    /* 小键盘区上 . 键    */
#define NUMADD     0x4e2b    /* 小键盘区上 + 键    */
#define NUMSUB     0x4a2d    /* 小键盘区上 - 键    */
#define NUMMUL     0x372a    /* 小键盘区上 * 键    */
#define NUMDIV     0x352f    /* 小键盘区上 / 键    */
#define NUMEQU     0x1c0d    /* 小键盘区上 = 键    */

#define KEY0       0xb30     /* 主键盘区上数字键0  */
#define KEY1       0x231     /* 主键盘区上数字键1  */
#define KEY2       0x332     /* 主键盘区上数字键2  */
#define KEY3       0x433     /* 主键盘区上数字键3  */
#define KEY4       0x534     /* 主键盘区上数字键4  */
#define KEY5       0x635     /* 主键盘区上数字键5  */
#define KEY6       0x736     /* 主键盘区上数字键6  */
#define KEY7       0x837     /* 主键盘区上数字键7  */
#define KEY8       0x938     /* 主键盘区上数字键8  */
#define KEY9       0xa39     /* 主键盘区上数字键9  */
#define KEYPNT     0x342e    /* 主键盘区上 . 键    */
#define KEYSUB     0xc2d     /* 主键盘区上 - 键    */
#define KEYMUL     0x92a     /* 主键盘区上 * 键    */
#define KEYEQU     0xd3d     /* 主键盘区上 = 键    */

#define SQR        0x340     /* @ 键,求平方根 */
#define KEYR       0x1372    /* r 键,取倒数 */
#define PERCENT    0x625     /* % 键,求百分数 */
#define DEL        0x5300    /* DEL 键  */
#define ESC        0x11b     /* ESC 键  */
#define BACKSPACE  0xe08     /* 退格 键  */
#define F9         0x4300    /* F9 键，正负数变换  */
#define CTRL_L     0x260c    /* Ctrl 键+L键，清除记忆器中的数值 */
#define CTRL_R     0x1312    /* Ctrl 键+R键，读取记忆器中的数值 */
#define CTRL_M     0x320d    /* Ctrl 键+M键，将当前数值写入记忆器中*/
#define CTRL_P     0x1910    /* Ctrl 键+P键，将当前数值和记忆器中保存的数值相加*/
#define ALT_X      0x2d00    /* Alt 键+X键  */

#define TRUE  1     /* 为十进制1表示为真 true  */
#define FALSE 0     /* 为十进制0表示为假 false */

/*计算器界面结构体*/
struct Block
    {    int  left,top,width,height;        /* 左上坐标,宽,高          */
         char caption[50];                  /* 标题                    */
         int  fontcolor,fontsize,status;    /* 字体颜色,字体大小,状态  */
    };

void Form(struct Block form);      /* 构造和显示主窗口*/
void TextBox(struct Block txtbox); /* 构造和显示文本输入框*/
void Label(struct Block label);    /* 构造和显示标签，若记忆功能开启，标签标记为：M*/

void CommandButton(struct Block cmdbutton); /*显示cmdbutton[i]命令按钮*/
void CommandButton_KeyboardDown(int i); /*定义按下编号为i的键所进行的操作，如按钮的状态的改变*/
void CommandButton_KeyboardUp(int i);   /*定义松开编号为i的按键所进行的操作，如按钮的状态*/


void InitApp(); /*初始化程序*/
void Showme();  /*显示计算器界面*/
void Load();    /*初始化默认值*/

void CommandButton_Click(int key);  /*按键盘时所做的操作*/
void DoubleRun(); /*四则运算:加减乘除*/
void SingleRun(int operatoror); /*单运算*/
void Resetbuf(); /*重置缓冲区*/
void StoreSet(int key); /*定义记忆存储操作*/
void Unload(); /*退出系统时的一些恢复操作*/


struct Block frmmain,txtscreen,lblstore,cmdbutton[28]; /*定义主窗口,文本输入框,记忆标签,28[0-27]个按钮*/
int clickflag;  /*clickflag:按键标志，*/
int top,pointflag,digitkeyhit; /*top:保存缓冲区中的当前位数，pointflag:小数点标记，digitkeyhit:数字键按键标记*/
int operatoror,runflag,ctnflag; /*operatoror:操作符，runflag:运算标记，ctnflag:运算符标记*/
int errorflag; /*错误标记*/
double num1,num2,store; /*num1:操作数1,num2：操作数2,store：记忆变量*/
char strbuf[33];  /*字符缓冲区,用于保存一个操作数*/


void main()
    {

         int key;  /*保存此计算器上定义的按键编号*/
         InitApp();/*初始化程序，进入图形模式*/
         Showme(); /*显示计算器窗口*/
         Load();   /*初始化默认值*/
         while(1)
             {
                    if(bioskey(1)==0) continue; /*直到有键按下时，才返回非0值，否则返回0值*/
                    key=bioskey(0); /*返回上条语句的按键值*/
                    switch(key)    /*捕获相应的键盘按键并匹配到计算器按键上*/
                        {    case NUM0:case KEY0:          key=10;break;
                             case NUM1:case KEY1:          key=1;break;
                             case NUM2:case KEY2:          key=2;break;
                             case NUM3:case KEY3:          key=3;break;
                             case NUM4:case KEY4:          key=4;break;
                             case NUM5:case KEY5:          key=5;break;
                             case NUM6:case KEY6:          key=6;break;
                             case NUM7:case KEY7:          key=7;break;
                             case NUM8:case KEY8:          key=8;break;
                             case NUM9:case KEY9:          key=9;break;
                             case F9:                      key=11;break;
                             case NUMPNT:case KEYPNT:      key=12;break;
                             case NUMADD:                  key=13;break;
                             case NUMSUB:case KEYSUB:      key=14;break;
                             case NUMMUL:case KEYMUL:      key=15;break;
                             case NUMDIV:                  key=16;break;
                             case SQR:                     key=17;break;
                             case PERCENT:                 key=18;break;
                             case KEYR:                    key=19;break;
                             case NUMEQU:case KEYEQU:      key=20;break;
                             case CTRL_L:                  key=21;break;
                             case CTRL_R:                  key=22;break;
                             case CTRL_M:                  key=23;break;
                             case CTRL_P:                  key=24;break;
                             case BACKSPACE:               key=25;break;
                             case DEL:                     key=26;break;
                             case ESC:                     key=27;break;
                             case ALT_X:                   key=0; break;
                             default:                      key=-1;break;
                     }
             if(key<0) continue; /*若对应的按键返回负数，则返回到while(1)处执行*/
             CommandButton_KeyboardDown(key);/*为了在计算器上显示按键效果，在此函数中改变一些按钮的状态值*/
             CommandButton_Click(key); /*根据key的值，进行相关操作*/
             delay(300000);    /*为了突出按键效果，此处延时300000毫秒*/
             CommandButton_KeyboardUp(key); /*在处理完此按键后，要恢复按钮的状态值*/
        }
    }

void InitApp()   /*初始化程序*/
    {
         int driver=DETECT,mode; /* 显示设备驱动为自动检测显示器模式 */
         initgraph(&driver,&mode,"");   /* 初始化图形显示系统  */
         if(driver!=VGA&&driver!=EGA)  /* 如果不能初始化  */
             {  printf("\n\nERROR!Can't initialize the graphics system!"); /* 显示错误信息"不能初始化图形系统"  */
                    closegraph();   /* 关闭图形接口*/
                    exit(0);       /* 直接退出系统 */
             }
         setbkcolor(9);     /* 设置背景颜色为青色  */


         /*主窗口的属性设置*/
         frmmain.left=200; frmmain.top=100; frmmain.width=230;  frmmain.height=235; frmmain.fontcolor=BLACK; frmmain.fontsize=1;   strcpy(frmmain.caption,"   << Calculator >>");frmmain.status=1;
         /*文本框的属性设置*/
         txtscreen.left=10;txtscreen.top=25;txtscreen.width=210;txtscreen.height=30;txtscreen.fontcolor=BLACK;txtscreen.fontsize=1;strcpy(txtscreen.caption,"0.");           txtscreen.status=1;
         /*标签的属性设置*/
         lblstore.left=190; lblstore.top=62; lblstore.width=30;  lblstore.height=25; lblstore.fontcolor=YELLOW; lblstore.fontsize=1;strcpy(lblstore.caption,"");              lblstore.status=1;
         /*命令按钮的属性设置*/
         cmdbutton[1].left=50-35;cmdbutton[1].top=165;cmdbutton[1].width=30;cmdbutton[1].height=25;cmdbutton[1].fontcolor=BLUE;cmdbutton[1].fontsize=1;strcpy(cmdbutton[1].caption,"1");cmdbutton[1].status=1;
         cmdbutton[2].left=85-35;cmdbutton[2].top=165;cmdbutton[2].width=30;cmdbutton[2].height=25;cmdbutton[2].fontcolor=BLUE;cmdbutton[2].fontsize=1;strcpy(cmdbutton[2].caption,"2");cmdbutton[2].status=1;
         cmdbutton[3].left=120-35;cmdbutton[3].top=165;cmdbutton[3].width=30;cmdbutton[3].height=25;cmdbutton[3].fontcolor=BLUE;cmdbutton[3].fontsize=1;strcpy(cmdbutton[3].caption,"3");cmdbutton[3].status=1;
         cmdbutton[4].left=50-35;cmdbutton[4].top=130;cmdbutton[4].width=30;cmdbutton[4].height=25;cmdbutton[4].fontcolor=BLUE;cmdbutton[4].fontsize=1;strcpy(cmdbutton[4].caption,"4");cmdbutton[4].status=1;
         cmdbutton[5].left=85-35;cmdbutton[5].top=130;cmdbutton[5].width=30;cmdbutton[5].height=25;cmdbutton[5].fontcolor=BLUE;cmdbutton[5].fontsize=1;strcpy(cmdbutton[5].caption,"5");cmdbutton[5].status=1;
         cmdbutton[6].left=120-35;cmdbutton[6].top=130;cmdbutton[6].width=30;cmdbutton[6].height=25;cmdbutton[6].fontcolor=BLUE;cmdbutton[6].fontsize=1;strcpy(cmdbutton[6].caption,"6");cmdbutton[6].status=1;
         cmdbutton[7].left=50-35;cmdbutton[7].top=95;cmdbutton[7].width=30;cmdbutton[7].height=25;cmdbutton[7].fontcolor=BLUE;cmdbutton[7].fontsize=1;strcpy(cmdbutton[7].caption,"7");cmdbutton[7].status=1;
         cmdbutton[8].left=85-35;cmdbutton[8].top=95;cmdbutton[8].width=30;cmdbutton[8].height=25;cmdbutton[8].fontcolor=BLUE;cmdbutton[8].fontsize=1;strcpy(cmdbutton[8].caption,"8");cmdbutton[8].status=1;
         cmdbutton[9].left=120-35;cmdbutton[9].top=95;cmdbutton[9].width=30;cmdbutton[9].height=25;cmdbutton[9].fontcolor=BLUE;cmdbutton[9].fontsize=1;strcpy(cmdbutton[9].caption,"9");cmdbutton[9].status=1;
         cmdbutton[10].left=50-35;cmdbutton[10].top=200;cmdbutton[10].width=30;cmdbutton[10].height=25;cmdbutton[10].fontcolor=BLUE;cmdbutton[10].fontsize=1;strcpy(cmdbutton[10].caption,"0");cmdbutton[10].status=1;
         cmdbutton[11].left=85-35;cmdbutton[11].top=200;cmdbutton[11].width=30;cmdbutton[11].height=25;cmdbutton[11].fontcolor=BLUE;cmdbutton[11].fontsize=1;strcpy(cmdbutton[11].caption,"+/-");cmdbutton[11].status=1;
         cmdbutton[12].left=120-35;cmdbutton[12].top=200;cmdbutton[12].width=30;cmdbutton[12].height=25;cmdbutton[12].fontcolor=BLUE;cmdbutton[12].fontsize=1;strcpy(cmdbutton[12].caption,".");cmdbutton[12].status=1;
         cmdbutton[13].left=155-35;cmdbutton[13].top=95;cmdbutton[13].width=30;cmdbutton[13].height=25;cmdbutton[13].fontcolor=RED;cmdbutton[13].fontsize=1;strcpy(cmdbutton[13].caption,"+");cmdbutton[13].status=1;
         cmdbutton[14].left=155-35;cmdbutton[14].top=130;cmdbutton[14].width=30;cmdbutton[14].height=25;cmdbutton[14].fontcolor=RED;cmdbutton[14].fontsize=1;strcpy(cmdbutton[14].caption,"-");cmdbutton[14].status=1;
         cmdbutton[15].left=155-35;cmdbutton[15].top=165;cmdbutton[15].width=30;cmdbutton[15].height=25;cmdbutton[15].fontcolor=RED;cmdbutton[15].fontsize=1;strcpy(cmdbutton[15].caption,"*");cmdbutton[15].status=1;
         cmdbutton[16].left=155-35;cmdbutton[16].top=200;cmdbutton[16].width=30;cmdbutton[16].height=25;cmdbutton[16].fontcolor=RED;cmdbutton[16].fontsize=1;strcpy(cmdbutton[16].caption,"/");cmdbutton[16].status=1;
         cmdbutton[17].left=190-35;cmdbutton[17].top=95;cmdbutton[17].width=30;cmdbutton[17].height=25;cmdbutton[17].fontcolor=BLUE;cmdbutton[17].fontsize=1;strcpy(cmdbutton[17].caption,"sqr");cmdbutton[17].status=1;
         cmdbutton[18].left=190-35;cmdbutton[18].top=130;cmdbutton[18].width=30;cmdbutton[18].height=25;cmdbutton[18].fontcolor=BLUE;cmdbutton[18].fontsize=1;strcpy(cmdbutton[18].caption,"%");cmdbutton[18].status=1;
         cmdbutton[19].left=190-35;cmdbutton[19].top=165;cmdbutton[19].width=30;cmdbutton[19].height=25;cmdbutton[19].fontcolor=BLUE;cmdbutton[19].fontsize=1;strcpy(cmdbutton[19].caption,"1/x");cmdbutton[19].status=1;
         cmdbutton[20].left=190-35;cmdbutton[20].top=200;cmdbutton[20].width=30;cmdbutton[20].height=25;cmdbutton[20].fontcolor=RED;cmdbutton[20].fontsize=1;strcpy(cmdbutton[20].caption,"=");cmdbutton[20].status=1;
         cmdbutton[21].left=190;cmdbutton[21].top=95;cmdbutton[21].width=30;cmdbutton[21].height=25;cmdbutton[21].fontcolor=RED;cmdbutton[21].fontsize=1;strcpy(cmdbutton[21].caption,"MC");cmdbutton[21].status=1;
         cmdbutton[22].left=190;cmdbutton[22].top=130;cmdbutton[22].width=30;cmdbutton[22].height=25;cmdbutton[22].fontcolor=RED;cmdbutton[22].fontsize=1;strcpy(cmdbutton[22].caption,"MR");cmdbutton[22].status=1;
         cmdbutton[23].left=190;cmdbutton[23].top=165;cmdbutton[23].width=30;cmdbutton[23].height=25;cmdbutton[23].fontcolor=RED;cmdbutton[23].fontsize=1;strcpy(cmdbutton[23].caption,"MS");cmdbutton[23].status=1;
         cmdbutton[24].left=190;cmdbutton[24].top=200;cmdbutton[24].width=30;cmdbutton[24].height=25;cmdbutton[24].fontcolor=RED;cmdbutton[24].fontsize=1;strcpy(cmdbutton[24].caption,"M+");cmdbutton[24].status=1;
         cmdbutton[25].left=50-35;cmdbutton[25].top=60;cmdbutton[25].width=53;cmdbutton[25].height=25;cmdbutton[25].fontcolor=RED;cmdbutton[25].fontsize=1;strcpy(cmdbutton[25].caption,"<-");cmdbutton[25].status=1;
         cmdbutton[26].left=108-35;cmdbutton[26].top=60;cmdbutton[26].width=53;cmdbutton[26].height=25;cmdbutton[26].fontcolor=RED;cmdbutton[26].fontsize=1;strcpy(cmdbutton[26].caption,"Del");cmdbutton[26].status=1;
         cmdbutton[27].left=166-35;cmdbutton[27].top=60;cmdbutton[27].width=53;cmdbutton[27].height=25;cmdbutton[27].fontcolor=RED;cmdbutton[27].fontsize=1;strcpy(cmdbutton[27].caption,"Esc");cmdbutton[27].status=1;
         /* 设置计算器界面 */
    }

void Showme()   /*显示计算器界面*/
    {    int i;
         Form(frmmain); /*显示主窗口，frmmain为主窗口的结构变量名*/
         TextBox(txtscreen); /*显示文本框*/
         Label(lblstore); /*显示记忆器的状态标签*/
         for(i=1;i<28;i++) /*显示27个按钮在计算器主窗口中*/
             CommandButton(cmdbutton[i]);
    }

void Load() /*初始化默认值*/
    {    num1=num2=0;
         Resetbuf();
         ctnflag=FALSE;
         operatoror=0;
         runflag=FALSE;
         errorflag=FALSE;
         store=0;
         clickflag=FALSE;

         strcpy(txtscreen.caption,"0.");
         TextBox(txtscreen);/*文本框中初始显示为0.的字符*/

         strcpy(lblstore.caption,"");
         Label(lblstore);
    }

void Unload() /*退出系统*/
    {
         cleardevice(); /*清除图形屏幕*/
         closegraph();  /*关闭图形系统*/
         exit(0);
    }

void Resetbuf()
    {    strbuf[0]=' ';
         strbuf[1]=0;
         top=1;
         digitkeyhit=FALSE;
         pointflag=FALSE;
    }


void Form(struct Block form)   /* 构造和显示主窗口*/
    {    int x1=form.left; /*窗口左上角的横坐标值*/
         int y1=form.top;  /*窗口左上角的纵坐标值*/
         int x2=form.width+x1-1;/*窗口右下角的横坐标值*/
         int y2=form.height+y1-1; /*窗口右下角的纵坐标值*/

         setfillstyle(SOLID_FILL,LIGHTGRAY); /*设置填充模式和颜色  */
                bar(x1+1,y1+1,x2-1,y2-1);    /*画一个淡灰色的填充窗口，作为主窗口,但此函数不画出边框*/
         setcolor(WHITE);                    /*设置当前画线颜色*/
                line(x1,y1,x2,y1);
                line(x1,y1,x1,y2);    /*用白线画边框左边和上边的线，美化主窗口*/
         setcolor(DARKGRAY);          /*设置填充模式和颜色  */
                line(x2,y1,x2,y2);
                line(x1,y2,x2,y2);    /*用深灰色画边框右边和下边的线，美化主窗口*/
         setfillstyle(SOLID_FILL,RED);
                bar(x1+2,y1+2,x2-2,y1+15);    /*设置标题栏颜色为红色*/
         settextjustify(LEFT_TEXT,CENTER_TEXT); /*设置文本的对齐方式为左中对齐*/
         settextstyle(DEFAULT_FONT,0,form.fontsize); /*设置文本显示字体为默认字体,大小为主窗体字体大小*/
         setcolor(form.fontcolor);
                outtextxy(x1+3,y1+10,form.caption);  /*用主窗体的颜色显示标题在标题栏*/

         if(form.status&1)     /*判断窗口是否可用,未使用ALT+X键,右上角的Ｘ按钮*/
             {      cmdbutton[0].left=form.width-15;
                    cmdbutton[0].top=3;
                    cmdbutton[0].width=12;
                    cmdbutton[0].height=12;
                    cmdbutton[0].status=1;
                    cmdbutton[0].caption[0]=0;
                    CommandButton(cmdbutton[0]);
                    x1=cmdbutton[0].left+form.left;
                    y1=cmdbutton[0].top+form.top;
                    x2=cmdbutton[0].width+x1-1;
                    y2=cmdbutton[0].height+y1-1;
                    setfillstyle(SOLID_FILL,LIGHTGRAY);
                         bar(x1+1,y1+1,x2-1,y2-1);
                    setcolor(DARKGRAY);
                         line(x1+2,y1+2,x2-2,y2-2);
                         line(x2-2,y1+2,x1+2,y2-2);
             }

    }

void TextBox(struct Block txtbox)     /* 设置和显示输入框*/
    {    /*(x1,y1),(x2,y2)为主窗口中的相对坐标*/
         int x1=txtbox.left+frmmain.left;
         int y1=txtbox.top+frmmain.top;
         int x2=txtbox.width+x1-1;
         int y2=txtbox.height+y1-1;

         setfillstyle(SOLID_FILL,WHITE);
                bar(x1+1,y1+1,x2-1,y2-1);
         /*因bar()不画出边框，所以接下来画这个方本框的边框线*/
         setcolor(LIGHTGRAY);
                rectangle(x1+1,y1+1,x2-1,y2-1);
         setcolor(DARKGRAY);
                line(x1,y1,x2,y1);
                line(x1,y1,x1,y2);
                setcolor(WHITE);
                line(x2,y1,x2,y2);
                line(x1,y2,x2,y2);
         settextjustify(RIGHT_TEXT,CENTER_TEXT); /*为图形函数设置文本的对齐方式,第一参数为水平对齐方式，第二参数为垂直对齐方式*/
         settextstyle(DEFAULT_FONT,0,txtbox.fontsize); /*为图形输出设置当前的文本属性:字体，方向，大小*/
         setcolor(txtbox.fontcolor); /*设置方本框的字体颜色*/
         outtextxy(x2-10,(y1+y2)/2,txtbox.caption);  /*在指定位置显示txtbox.caption的字符串值*/

    }

void Label(struct Block label) /*设置和显示计算器记忆功能的使用状态，默认为空*/
    {    int x1=label.left+frmmain.left;
         int y1=label.top+frmmain.top;
         int x2=label.width+x1-1;
         int y2=label.height+y1-1;

         setfillstyle(SOLID_FILL,LIGHTGRAY);
                bar(x1+1,y1+1,x2-1,y2-1);
         setcolor(DARKGRAY);
                line(x1,y1,x2,y1);
                line(x1,y1,x1,y2);
         setcolor(WHITE);
                line(x2,y1,x2,y2);
                line(x1,y2,x2,y2);
         settextjustify(CENTER_TEXT,CENTER_TEXT);
         settextstyle(DEFAULT_FONT,0,label.fontsize);
         setcolor(label.fontcolor);
                outtextxy((x1+x2)/2,(y1+y2)/2,label.caption);

    }

void CommandButton(struct Block cmdbutton)  /*设置和显示单个按钮*/
    {     /*(x1,y1),(x2,y2)为主窗口中的相对坐标*/
         int x1=cmdbutton.left+frmmain.left;
         int y1=cmdbutton.top+frmmain.top;
         int x2=cmdbutton.width+x1-1;
         int y2=cmdbutton.height+y1-1;
         int c1,c2;
         /*按钮的初始状态为1,若有键按下后，其状态变为0,处理完按键操作后，又恢复为状态1*/
         if(cmdbutton.status) /*根据按钮的当前状态值来，分别用不同的颜色边框来重绘此按钮*/
             {      c1=WHITE;  /*白色*/
                    c2=DARKGRAY; /*深灰色*/
             }
         else /*若刚有键按下*/
             {      c1=DARKGRAY;
                    c2=WHITE;
             }

         setcolor(c1);
                line(x1,y1,x2,y1);
                line(x1,y1,x1,y2);
         setcolor(c2);
                line(x2,y1,x2,y2);
                line(x1,y2,x2,y2);
         settextjustify(CENTER_TEXT,CENTER_TEXT);
         settextstyle(DEFAULT_FONT,0,cmdbutton.fontsize);
         outtextxy((x1+x2)/2,(y1+y2)/2,cmdbutton.caption);

    }

void CommandButton_KeyboardDown(int i) /*按下某键时所做的操作*/
    {    clickflag=TRUE;      /*键盘点击标志*/
         cmdbutton[i].status=0;
         CommandButton(cmdbutton[i]); /*用与初始时不同的边框颜色色，重绘此命令按钮*/
    }



void CommandButton_KeyboardUp(int key) /*当按键处理完后，恢复按钮状态，重绘此命令按钮*/
    {  clickflag=FALSE;
         cmdbutton[key].status=1;
         CommandButton(cmdbutton[key]);
    }



void CommandButton_Click(int key)      /*处理相应按键操作*/
    {
        if(errorflag==TRUE)  return;
         switch(key)
             {  case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:   /*1-9*/
                        if(top<15) /*单个操作数小于15位*/
                            {    strbuf[top++]='0'+key; /*'0'+key表示是字符1,若没有'0'+则存储的是Ascll码为key为字符*/
                                 strbuf[top]=0; /*存储的是ASCLL码为0的表示的字符Ctrl+@*/
                                 digitkeyhit=TRUE; /*表示已有数字键按下*/
                                 strcpy(txtscreen.caption,strbuf); /*在文本框中显示当前的*/
                            }
                        runflag=FALSE; /*运算标记为假*/
                        if(ctnflag==FALSE) operatoror=0;
                        break;
                    case 10:     /* 0输入*/
                        if(top<15&&top!=1)
                            {    strbuf[top++]='0';
                                 strbuf[top]=0;
                                 strcpy(txtscreen.caption,strbuf);
                            }
                        digitkeyhit=TRUE;
                        runflag=FALSE;
                        if(ctnflag==FALSE) operatoror=0;
                        break;
                    case 11:    /*正负互换*/
                        if(digitkeyhit==TRUE)
                            {    strbuf[0]=strbuf[0]==' '?'-':' ';
                                 strcpy(txtscreen.caption,strbuf);
                            }
                        else if(runflag==TRUE) /*表示没有新的输入，就是在之前的计算结果上正负转换*/
                            {    num1=-num2;
                                 sprintf(txtscreen.caption,"%G",num1);
                            }
                        else   /*原数的正负互换*/
                            {    num1=-num1;
                                 sprintf(txtscreen.caption,"%G",num1);
                            }
                        runflag=FALSE;
                        if(ctnflag==FALSE) operatoror=0;
                        break;
                    case 12:   /*输入一个小数点*/
                        if(top==1) /*表示还没有输入数,保持0.状态*/
                            {  strbuf[top++]='0';
                                 strbuf[top++]='.';
                                 strbuf[top]=0;
                                 strcpy(txtscreen.caption,strbuf);
                                 digitkeyhit=TRUE;
                                 runflag=FALSE;
                                 pointflag=TRUE;
                                 if(ctnflag==FALSE) operatoror=0;
                            }
                        else if(top<15&&pointflag==FALSE)
                            {  strbuf[top++]='.';
                                 strbuf[top]=0;
                                 strcpy(txtscreen.caption,strbuf);
                                 digitkeyhit=TRUE;
                                 runflag=FALSE;
                                 pointflag=TRUE;
                                 if(ctnflag==FALSE) operatoror=0;
                            }
                        break;
                    case 13:case 14:case 15:case 16:   /* 四则运算加减乘除运算符 */
                        if(digitkeyhit) /*若此运算符之前已经输入了一个数*/
                            num1=atof(strbuf);
                        if(ctnflag) /*之前的输入中，已有运算符的输入*/
                            if(digitkeyhit==TRUE) /*如1+2+的情况*/
                                 DoubleRun(); /*先计算出1+2*/
                            else
                                 ;
                        else /*之前的输入中，没有运算符的输入,如:1+的情况*/
                            if(operatoror==0)
                                 num2=num1;
                            else
                                 ;
                        Resetbuf();
                        operatoror=key;
                        ctnflag=TRUE;
                        runflag=TRUE;
                        break;
                    case 17:case 18:case 19:   /*单运算 开方,百分比,倒数*/
                        if(digitkeyhit)
                            num1=atof(strbuf); /*num1保存当前操作数*/

                        SingleRun(key);
                        Resetbuf();
                        ctnflag=FALSE;
                        operatoror=0;
                        runflag=FALSE;
                        break;
                    case 20:    /*获取运算结果及等于操作*/
                        if(digitkeyhit) num1=atof(strbuf);
                        if(operatoror)
                            DoubleRun(); /*第一个操作数,保存在num2中*/
                        else
                            num2=num1;
                        Resetbuf();
                        ctnflag=FALSE;
                        runflag=TRUE;
                        break;
                    case 21:case 22:case 23:case 24: /*值的保存的一些操作*/
                        if(digitkeyhit) num1=atof(strbuf);
                        StoreSet(key);
                        Resetbuf();
                        break;
                    case 25:    /*删除数字的整数部分的最后一位数(BackSpace键)*/
                        if(top>1)
                            if(strbuf[--top]=='.')
                                {  if(strbuf[1]=='0'&&strbuf[2]=='.')
                                         strbuf[--top]=0;
                                     else
                                         strbuf[top]=0;
                                     pointflag=FALSE;
                                }
                            else
                                strbuf[top]=0;
                        operatoror=0;
                        ctnflag=FALSE;
                        runflag=FALSE;
                        strcpy(txtscreen.caption,strbuf);
                        break;
                    case 26:   /*清除当前显示的值(Del键)*/
                        Resetbuf();
                        num1=0;
                        strcpy(txtscreen.caption,strbuf);
                        TextBox(txtscreen);
                        break;
                    case 27:   /*清除所有的值,包括存储的,已经运算了的(ESC键）*/
                        Resetbuf();
                        num1=num2=0;
                        ctnflag=FALSE;
                        operatoror=0;
                        runflag=FALSE;
                        errorflag=FALSE;

                        strcpy(txtscreen.caption,"0.");
                        TextBox(txtscreen);

                        break;
                    case 0:
                        Unload();
                        break;
             }
         if(errorflag==FALSE)
             {  if(atof(txtscreen.caption)==0)
                        strcpy(txtscreen.caption,"0");
                    if(strchr(txtscreen.caption,'.')==NULL)
                        strcat(txtscreen.caption,".");
             }
         TextBox(txtscreen); /*显示txtscreen结构变量的值*/
    }

void DoubleRun() /*四则运算*/
    {  switch(operatoror)
             {      case 13: num2+=num1;break;  /*加*/
                    case 14: num2-=num1;break; /*减*/
                    case 15: num2*=num1;break; /*乘*/
                    case 16: if(num1==0)     /*除*/
                                         errorflag=TRUE;
                                     else
                                         num2/=num1;
                                     break;
             }
         if(errorflag)
             strcpy(txtscreen.caption,"Can't divide by zero!");
         else
             sprintf(txtscreen.caption,"%G",num2);
    }

void SingleRun(int key) /*单运算*/
    {  switch(key)
             {  case 17:  /*求开方*/
                        if(num1<0)
                             errorflag=TRUE;
                        else
                             num1=sqrt(num1);
                        break;
                    case 18:  /*求百分比*/
                             num1/=100;
                        break;
                    case 19: /*求倒数*/
                        if(num1==0)
                             errorflag=TRUE;
                        else
                             num1=1/num1;
                        break;
                }
         if(errorflag==TRUE)
             if(num1<0)
                 strcpy(txtscreen.caption,"Can't blower than zero!");
             else
                 strcpy(txtscreen.caption,"Can't equal to zero!");
         else
             sprintf(txtscreen.caption,"%G",num1);
    }

void StoreSet(int key)  /*记忆存储操作*/
   {  switch(key)
             {  case 21:   /*保存清除*/
                        store=0;
                        lblstore.caption[0]=0;
                        break;
                case 22:  /*取出保存的值*/
                        num1=store;
                        sprintf(txtscreen.caption,"%G",store);
                        runflag=FALSE;
                        if(ctnflag==FALSE) operatoror=0;
                        break;
                case 23:  /*保存当前数字*/
                        store=num1;
                        strcpy(lblstore.caption,"M");
                        break;
               case 24:  /*保存值与当前数字相加*/
                        store+=num1;
                        strcpy(lblstore.caption,"M");
                        break;
            }
         Label(lblstore);
    }


