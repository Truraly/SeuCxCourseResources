/*文本编辑器editor源代码*/
#include <stdio.h>
#include <conio.h>
#include <bios.h>
#include <math.h>
#define LEFT 0x4b00   /*←：光标左移*/
#define RIGHT 0x4d00  /*→：光标右移*/
#define DOWN 0x5000   /*↓键：光标下移*/
#define UP 0x4800     /*↑键：光标上移*/
#define ESC 0x011b    /*ESC键：取消菜单打开操作*/
#define ENTER 0x1c0d  /*回车键：换行*/
#define DEL 21248     /*DEL键：删除当前字符*/
#define BACK 3592     /*BackSpace键：删除当前光标位置前一个字符*/
#define CL 29440      /*ctrl+←键：从右至左，选定文本*/
#define CR 29696      /*ctrl+→键：从左到右，选定文本*/
#define Cc 11779      /*ctrl+c键：将选定文本，复制一份到剪贴板中*/
#define Cv 12054      /*ctrl+v键：将剪贴板中的内容复制到当前位置*/
#define Cx 11544      /*ctrl+x键：对选定文本，执行剪切操作*/
#define F1 15104      /*F1键：打开文件菜单*/
#define F2 15360      /*F2键：打开编辑菜单*/
#define F3 15616      /*F3键：打开帮助菜单*/
#define F10 17408     /*F10键:进入文本快速预览模式*/
int value,backup,NUM;
/*value保存有值数组元素的最大下标值，backup保存value的副本，NUM保存当前行中的用户输入的字符个数*/
typedef struct record
 {
       char ch;  /*保存一字符*/
       int col, line;  /*x轴和y轴坐标*/
 }record;
record r[500]; /*定义一个有500个元素的结构体数组，保存选定的文本字符的属性*/

typedef struct node /*定义保存行中的单个字符的结构*/
 {
        char ch; /*数据域：保存一字符*/
        struct node *next; /*指针域：指向下一个结点的指针*/
 }node;/*由此类型节点构成的单链表，命名为：列单链表*/


  typedef struct Hnode /*定义保存所有列单链表首节点的指针的结构*/
 {
         node *next; /*指向列单链表的首节点的地址*/
         struct Hnode *nextl; /*指向下一个节点的指针*/
 }Hnode;/*由此类型节点构成的单链表，命名为：行单链表*/




void drawmain() /*画主窗口函数*/
 {
          int i,j;
          gotoxy(1,1);       /*在文本窗口中设置光标至(1,1)处*/
          textbackground(7); /*选择新的文本背景颜色,7为LIGHTGRAY淡灰色*/
          textcolor(0);      /*在文本模式中选择新的字符颜色0为BLACK黑*/
          insline();         /*在文本窗口的(1,1)位置处中插入一个空行*/
          for(i=1;i<=24;i++)
         {
              gotoxy(1,1+i);     /*（x,y)中x不变，y++*/
              cprintf("%c",196); /*在窗口左边输出-,即画出主窗口的左边界 */
              gotoxy(80,1+i);
              cprintf("%c",196); /*在窗口右边，输出-，即画出主窗口的右边界*/
         }
          for(i=1;i<=79;i++)
         {
               gotoxy(1+i,2);     /*在第2行，第2列开始*/
               cprintf("%c",196); /*在窗口顶端，输出-*/
               gotoxy(1+i,25);    /*在第25行，第2列开始*/
               cprintf("%c",196); /*在窗口底端，输出-*/
         }
          gotoxy(1,1);   cprintf("%c",196); /*在窗口左上角，输出-*/
          gotoxy(1,24);  cprintf("%c",196); /*在窗口左下角，输出-*/
          gotoxy(80,1);  cprintf("%c",196); /*在窗口右上角，输出-*/
          gotoxy(80,24); cprintf("%c",196); /*在窗口右下角，输出-*/
          gotoxy(7,1); cprintf("%c  %c File %c  %c",179,17,16,179);  /* | < > |*/
          gotoxy(27,1); cprintf("%c  %c Edit %c  %c",179,17,16,179); /* | < > |*/
          gotoxy(47,1); cprintf("%c  %c Help %c  %c",179,17,16,179); /* | < > |*/
          gotoxy(5,25); /*跳至窗口底端*/
          textcolor(1);
          cprintf(" Row:1         Col:1");
          gotoxy(68,25);
          cprintf("Version 2.0");
 }




void qview(Hnode *q) /*快速预览文本：开头：#，回车：* */
 {
       void view(Hnode *q); /*view()函数声明*/
       node *p;
       int i;
       window(1,1,80,25); /*定义文本窗口大小*/
       clrscr(); /*清屏*/
       /*循环读取两个单链表中的值：q是一个指向行单链表首节点的指针，
       此单链表数据域的值为实际保存各行字符的列单链表p中的首节点地址*/
       do{
              p=q->next;    /*p指向保存行数据的列单链表的首节点的地址*/
              cprintf("#"); /*每行开头，打印此字符，不管前面是否有回车符*/
              while(p!=NULL) /*循环读取单链表p中的值*/
             {
                    if(p->ch==13)  putch('*'); /*若为回车键，打印出*号*/
                    else
                    putch(p->ch);  /*输出各行中的字符到预览窗口*/
                    p=p->next; /*指向下一个节点*/
             }
             q=q->nextl; /*指向下一个节点*/
             printf("\n");/*输出一个回车*/
         }while(q!=NULL);

       getch();
       clrscr();
       drawmain();/*按任意键后，回到主窗口界面*/
       window(2,2,79,23);
       textbackground(9);
       for(i=0;i<24;i++)
            insline(); /*插入24个空行*/
       window(3,3,78,23);
       textcolor(10);
  }



void view(Hnode *q)  /*按行显示保存在单链表中的文本字符,q为指向行单链表中第一个节点的指针*/
 {
       node *p; /*p为保存列单链表节点元素地址的指针*/
       clrscr(); /*清屏*/
       /*双重循环，读取并显示保存在单链表中字符*/
       do{
              p=q->next;
              while(p!=NULL&&p->ch>=32&&p->ch<127&&p->ch!=13&&p->ch!=-1)  /*指针p不能为空，且数据域必须为常规字符*/
             {
                    putch(p->ch);/*在文本窗口中输出该字符*/
                    p=p->next;  /*指向下一个节点*/
             }
             q=q->nextl; /*指向下一个节点*/
             if((p->ch==13||p->ch==-1)&&q!=NULL)  gotoxy(1,wherey()+1); /*若ch为回车或EOF标记，光标跳至下行的开始处*/
         }while(q!=NULL); /*逐行逐列显示文本字符*/
 }
 
int check(Hnode *Hhead,int m,int n) /*check():在单链表中检查第m行第n列位置的字符，若为常规字符，则返回该字符*/
 {
        int i;
        Hnode *q;
        node *p;
        q=Hhead;
        for(i=1;i<m;i++) /*定位至行单链表中的第m个元素*/
            q=q->nextl;
        p=q->next;/*获取第m个节点的数据域*/
        for(i=1;i<n;i++) /*定位至列单链表中的第n个元素*/
            p=p->next;
        if(p->ch==13)  return -1; /*若第m行，第n列的字符为回车键，则返回-1*/
        if(p->ch>=32&&p->ch<127)  return p->ch; /*若第m行，第n列的字符为常规字符,则返回该字符*/
        else return 0; /*若第m行，第n列的字符既非回车符又非常规字符，则返回0*/
  }



int judge(Hnode *Hhead,int m) /*judge():返回第m行中的常规字符总的个数，不包括回车符*/
  {
        Hnode *q;
        node *p;
        int i,num=0;
        q=Hhead;
        for(i=1;i<m;i++) /*定位至行单链表中的第m个元素*/
            q=q->nextl;

        if(q==NULL)  return -1; /*返回-1,表示第m行不存在*/
        p=q->next;
        while(p->next!=NULL)
       {
                p=p->next;
                num++;       /*统计第m行的字符个数*/
       }
        /*行尾字符还没有判断，接下来判断行尾字符*/
        if(p->ch==13&&num==0)  return 0;    /*返回0,表示当前行只有一个回车字符*/
        if(p->ch>=32&&p->ch<127)  return num+1; /*返回num+1,表示当前行的最后一个字符为常规字符*/
        if(p->ch==13&&num!=0)  return num; /*返回num,表示当前行的最后一个字符为回车符，不计算在内*/
        else return 1;/*返回num,表示当前行中只有一个字符，且没有回车符*/
  }






  int del(Hnode *Hhead,int m,int n)  /*del():删除第m行，第n列位置的字符*/
 {
        Hnode *q,*q1;
        node *p1,*p2,*tail;
        int i,num=0,j,flag=0;
        q=Hhead;
        if(n==0&&m==1)  return; /*第1行，第0列不存在*/
        if(n==0&&m>1) /*若为第0列字符，但行必须大于1,执行向上行移处理*/
         {
               n=76;
               m=m-1;
               gotoxy(n,m);/*移至第m-1行，第76列*/
               flag=1; /*移位的标志置1*/
         }

        for(i=1;i<m;i++) /*定位至行单链表中的第m个元素*/
            q=q->nextl;
        p1=q->next;

        for(i=1;i<n-1;i++) /*定位至列单链表中的第n-1个元素*/
            p1=p1->next;
        p2=p1->next; /*p2指向列单链表中的第n个元素*/

             if(n==1) /*若是删除第m行第1列的字符*/
               {
                    q->next=p1->next;
                    free(p1);
               }
                else
               {
                    p1->next=p2->next; /*在单链表中删除第m行第n列的元素*/
                    free(p2);
               }

                /*删除掉第m行第n列的元素后，处理行单链表中第m个节点后的数据向前移的任务*/
                 while((num=judge(Hhead,m++))>0) /*执行一次judge(Head,m)后，m才加1.这里必须满足行常规字符数不为0的条件*/
                {
                         p1=q->next; q1=q;
                         if(p1!=NULL) /*若当前行非空*/
                        {
                                while(p1->next!=NULL)
                                    p1=p1->next;
                                tail=p1;/*tail保存列单链表最后一个元素的地址*/
                                q=q->nextl; /*指向下一行的元素的地址*/
                                p1=p2=q->next;
                                tail->next=p1; /*tail的指针域指向下一行的第一个元素的地址*/
                         }
                          else  /*若当前行的字符个数为0,即删除该字符后，只剩下回车符，则将下一个行单链表中节点的数据域移至前一下节点的数据域*/
                         {
                                 q=q->nextl;   p1=p2=q->next;
                                 q1->next=p1;/*q1->next指向下一行的第一个元素的地址*/
                          }

                         for(i=0;i<76-num;i++)
                         /*当前行还有76-num个空位没有字符，在下一行的单链表中读取字符，直至遇到回车符为止*/
                        {
                                p1=p2; /*p1指向p2的前一个节点,p2指向行单链表中下一个节点*/
                                p2=p2->next;
                                if(p2->ch==13)  break; /*若为回车，跳出循环*/
                         }
                          q->next=p2;   /*在列单链表中去掉移至上行的元素*/
                          p1->next=NULL;/*下行移至上行的最后一个元素，指针置空*/
                }
           return flag; /*返回0：表示没有换位，返回1：表示有换位*/
  }

 /*执行insert()后，检验第n行及后面的数据，使其满足规则*/
 int test(Hnode *Hhead,int n)
 {
       int i=0,num1=1;
       node *p1,*p2,*tail,*temp1,*temp2;
       Hnode *q;
       q=Hhead;
       for(i=1;i<n;i++) /*定位至行单链表中的第n个元素*/
            q=q->nextl;
       tail=p1=q->next;
       if(p1==NULL) return; /*若此行没有任何字符，则返回*/
       while(tail->next!=NULL) /*定位至列单链表中的最后一个元素*/
            tail=tail->next;

       /*若此单链表中没有回车符且有超过76个节点时，则p1会指向此列单链表中的第76个节点*/
       for(i=0;i<75;i++)
      {
             if(p1->ch==13||p1->next==NULL)  break;
             p1=p1->next;
      }

       p2=p1->next;
       p1->next=NULL; /*在此行的最后一个字符的前一个字符处断行,因为插入在此行插入了一个新的字符*/
       if(tail->ch!=13) /*若此行行尾不是回车键*/
      {
             if(p1->ch==13&&q->nextl==NULL)/*若p1的数据域为回车符且行单链表中只有n个节点*/
            {
                       q->nextl=(Hnode *)malloc(sizeof(Hnode)); /*新建一个行单链表节点，相当于添加一个新行*/
                       q->nextl->nextl=NULL;
                       tail->next=(node *)malloc(sizeof(node));/*在tail所指节点位置开始继续准备添加字符*/
                       tail->next->ch=13;   tail->next->next=NULL;
                       q->nextl->next=p2; /*新行单链表节点保存此行多出的字符*/
             }
             else /*若此行行尾和行中都没有回车键,或者q->nextl不为空*/
            {
                   q=q->nextl;/*q->nextl有可能为空*/
                   tail->next=q->next;/*将多出的字符与下一行的字符相连*/
                   q->next=p2;/**/
                   if(q!=NULL)  test(Hhead,++n); /*若行单链表第n个节点后还有节点，继续test()的相同处理*/
            }

      }
       else  /*若此列单链表最后一个元素为回车符*/
      {
            temp2=p2; /*p2指向第77个字符，或者为空(为空表示此行插入一个字符后，没有超出范围*/
            while(q!=NULL&&p2!=NULL) /*q指向行列表中的第n个节点.条件:行单链表中第n个节点存中且有第77个字符*/
             {  /*条件：在行单链表中只有n个节点,且字符超过了一行规定的76个，且num1标志为1*/
                 if((q->nextl==NULL)&&(p1!=tail||p2!=NULL)&&(num1==1))
                {
                       num1++;
                       q->nextl=(Hnode *)malloc(sizeof(Hnode)); /*新建一个行单链表节点,准备存储此行中多出的字符*/
                       q->nextl->nextl=NULL;   q->nextl->next=NULL; /*初始化值*/

                }
                /*行单链表中第n+1个节点已经存在，下面为在行单链表中插入一个新的节点*/
                 q=q->nextl; /*q指向行列表中的第n+1个节点*/
                 temp1=q->next;
                 q->next=temp2; /*q的数据域为此行中多出的字符所在的列单链表中的节点地址*/
                 temp2=temp1;
           }
      }
 }

void insert(Hnode *Hhead,int m,int n, char a) /*第m行，第n列的位置之前一个位置，插入单字符*/
 {
        int i;
        Hnode *q;
        node *p,*p1,*p2;
        q=Hhead;
        for(i=1;i<m;i++) /*定位至行单链表中的第m个元素*/
            q=q->nextl;
        p1=q->next;
        for(i=1;i<n-1;i++) /*定位至列单链表中的第n-1个元素*/
            p1=p1->next;
        p=(node *)malloc(sizeof(node)); /*创建一个新的列单链表节点*/
        p->ch=a; /*给此节点的数据域赋值*/
        if(n==1) /*插入之前，若只有一个字符在行中，则插在此节点之前*/
       {
               p->next=q->next;
               q->next=p;
       }
        else
       {
                 p->next=p1->next; /*在第m行，第n列的字符前，插入一字符*/
                 p1->next=p;
       }
    test(Hhead,m); /*在插入新元素后，检验并处理单链表中第m行开始的元素，使其满足规则*/

 }

/*对控制键进行响应，A：按键的整数值，Hhead:行单链表的首地址*/
void control(int A, Hnode *Hhead)
   {
           void colorview(Hnode *,int,int); /*函数声明*/
           int x,y,flag=0;
           x=wherex();   y=wherey(); /*得到当前光标的坐标值*/
           if((A==CL)&&(x!=1)) /*ctrl+←，当前光标不是在行首,光标移动*/
                gotoxy(wherex()-1,wherey());

           if((A==CL)&&(x==1)) /*ctrl+←，在行首*/
                gotoxy(abs(judge(Hhead,wherey()-1)),wherey()-1); /*judge(Hhead,wherey()-1)上一行的字符个数作为x值，光标移动*/

           if((A==CR)&&check(Hhead,wherey(),wherex())>0) /*ctrl+→,当前光标的右边有字符，光标移动*/
          {    flag=1;  gotoxy(wherex()+1,wherey()); }

           if((A==CR)&&check(Hhead,wherey()+1,1)>0&&check(Hhead,y,x)==0) /*ctrl+→,当前光标处没有字符但下一行的第一列有字符，光标移动*/
          {    flag=1;  gotoxy(1,wherey()+1);  }

           if((A==CR)&&x==76) /*ctrl+→，当前光标在当前行的行尾，光标移动*/
          {    flag=1;      gotoxy(1,wherey()+1);  }

           if(A==CR&&flag==1) /*ctrl+→，光标已经跳至新处，将当前光标所在位置的字符的坐标和值保存在r数组中*/
          {
             r[abs(value)].col=wherex();
             r[abs(value)].line=wherey();
             r[abs(value)].ch=check(Hhead,r[abs(value)].line,r[abs(value)].col);
             if(r[abs(value)].ch==-1)   r[abs(value)].ch=13; /*若第line行，第col列的字符为回车键，则返回-1*/
             value--;

          }

           if(A==CL&&(x!=1||y!=1))   /*ctrl+←,当前光标并不在窗口左上角,将当前光标所在位置的字符的坐标和值保存在r数组中*/
          {

             r[abs(value)].col=wherex();
             r[abs(value)].line=wherey();
             r[abs(value)].ch=check(Hhead,r[abs(value)].line,r[abs(value)].col);
             value++;
          }

           colorview(Hhead,wherex(),wherey());

  }

/*用不同的前背景色显示选择的字符*/
void colorview(Hnode *Hhead,int x,int y)
  {

           int i;
           view(Hhead);/*重新显示所有文本字符*/
           for(i=0;i<abs(value);i++)   /*value为数组下标*/
          {
                    gotoxy(r[i].col,r[i].line);
                    textbackground(7);
                    textcolor(0);
                    if(r[i].ch!=13&&r[i].ch!=-1)
                        cprintf("%c",r[i].ch);
                    if(r[i].ch==13||r[i].ch==-1)
                        cprintf(" ");
          }

           gotoxy(x,y);

  }

void drawmenu(int m,int n) /*画菜单,m:第几项菜单，n：第m项的第n个子菜单*/
 {
       int i;
      if(m%3==0) /*画File菜单项*/
     {
            window(8,2,19,9);
            textcolor(0);
            textbackground(7);
            for(i=0;i<7;i++) /*在上面定义的文本窗口中先输出7个空行*/
           {
                gotoxy(1,1+i);
                insline();
           }
            window(1,1,80,25);
            gotoxy(7,1);
            for(i=1;i<=7;i++)
           {
                gotoxy(8,1+i);
                cprintf("%c",179); /*窗口内文本的输出函数，在窗口左边输出 | */
                gotoxy(19,1+i);
                cprintf("%c",179); /*窗口内文本的输出函数，在窗口右边输出 | */
            }
            for(i=1;i<=11;i++)
           {
                  gotoxy(8+i,2);
                  cprintf("%c",196);  /*窗口内文本的输出函数，在窗口上边输出 - */
                  gotoxy(8+i,9);
                  cprintf("%c",196);  /*窗口内文本的输出函数，在窗口下边输出 - */
            }
            textbackground(0);
            gotoxy(10,10); cprintf("            "); /*输出下边的阴影效果*/
            for(i=0;i<9;i++)
           {
                  gotoxy(20,2+i);
                  cprintf("   "); /*输出右边的阴影效果*/
           }
            /*以上为显示菜单项的外观*/
            textbackground(7);
            gotoxy(8,2);  cprintf("%c",218); /*输出四个边角表格符*/
            gotoxy(8,9);  cprintf("%c",192);
            gotoxy(19,2); cprintf("%c",191);
            gotoxy(19,9); cprintf("%c",217);
            gotoxy(9,3);  cprintf(" New    ");
            gotoxy(9,4);  cprintf(" Open   ");
            gotoxy(9,5);  cprintf(" Save   ");
            gotoxy(9,6);  cprintf(" Save as");
            for(i=1;i<=10;i++)
           {
                  gotoxy(8+i,7);
                  cprintf("%c",196); /*在Save as下输出一行分隔符*/
           }
            gotoxy(9,8);  cprintf(" Exit");
            textcolor(15);  textbackground(0);
            gotoxy(7,1);
            cprintf("%c  %c File %c  %c",179,17,16,179);
            switch(n%5)
           {
                case 0:gotoxy(9,3);  cprintf(" New      "); break;
                case 1:gotoxy(9,4);  cprintf(" Open     "); break;
                case 2:gotoxy(9,5);  cprintf(" Save     "); break;
                case 3:gotoxy(9,6);  cprintf(" Save as  "); break;
                case 4:gotoxy(9,8);  cprintf(" Exit     "); break;
           }
      }

     /********************************************************/
     if(m%3==1) /*画Edit菜单项*/
    {
            window(28,2,38,7);
            textcolor(0);
            textbackground(7);
            for(i=0;i<5;i++)
           {
                gotoxy(1,1+i);
                insline();
            }
            window(1,1,80,25);
            gotoxy(27,1);
            for(i=1;i<=5;i++)
           {
                gotoxy(28,1+i);
                cprintf("%c",179);
                gotoxy(39,1+i);
                cprintf("%c",179);
            }
            for(i=1;i<=11;i++)
           {
                  gotoxy(28+i,2);
                  cprintf("%c",196);
                  gotoxy(28+i,7);
                  cprintf("%c",196);
            }

            textbackground(0);
            gotoxy(30,8); cprintf("            ");
            for(i=0;i<7;i++)
           {
                  gotoxy(40,2+i);
                  cprintf("   ");
           }
            textbackground(7);
            gotoxy(28,2);  cprintf("%c",218);
            gotoxy(28,7);  cprintf("%c",192);
            gotoxy(39,2);  cprintf("%c",191);
            gotoxy(39,7);  cprintf("%c",217);
            gotoxy(29,3);  cprintf("  Cut     ");
            gotoxy(29,4);  cprintf("  Copy    ");
            gotoxy(29,5);  cprintf("  Paste   ");
            gotoxy(29,6);  cprintf("  Clear   ");
            textcolor(15);  textbackground(0);
            gotoxy(27,1);
            cprintf("%c  %c Edit %c  %c",179,17,16,179);
            switch(n%4)
            {
                    case 0:gotoxy(29,3);  cprintf("  Cut     "); break;
                    case 1:gotoxy(29,4);  cprintf("  Copy    "); break;
                    case 2:gotoxy(29,5);  cprintf("  Paste   "); break;
                    case 3:gotoxy(29,6);  cprintf("  Clear   "); break;
            }
      }

    /*********************************************************/
     if(m%3==2) /*画Help菜单项3*/
    {
            window(48,2,48,6);
            textcolor(0);
            textbackground(7);
            for(i=0;i<3;i++)
            {
                    gotoxy(1,1+i);
                    insline();
            }
            window(1,1,80,25);
            gotoxy(47,1);
            for(i=1;i<=5;i++)
            {
                    gotoxy(48,1+i);
                    cprintf("%c",179);
                    gotoxy(59,1+i);
                    cprintf("%c",179);
            }
            for(i=1;i<=11;i++)
            {
                    gotoxy(48+i,2);
                    cprintf("%c",196);
                    gotoxy(48+i,6);
                    cprintf("%c",196);
            }

            textbackground(0);
            gotoxy(50,7); cprintf("            ");
            for(i=0;i<6;i++)
           {
                  gotoxy(60,2+i);
                  cprintf("   ");
           }
            textbackground(7);
            gotoxy(48,2);   cprintf("%c",218);
            gotoxy(48,6);   cprintf("%c",192);
            gotoxy(59,2);   cprintf("%c",191);
            gotoxy(59,6);   cprintf("%c",217);
            gotoxy(49,3);   cprintf("Help...   ");
            gotoxy(49,5);   cprintf("About...  ");
            for(i=1;i<=10;i++)
            {
                    gotoxy(48+i,4);
                    cprintf("%c",196);
            }
            textcolor(15);  textbackground(0);
            gotoxy(47,1);
            cprintf("%c  %c Help %c  %c",179,17,16,179);
            switch(n%2)
            {
                    case 0:gotoxy(49,3);  cprintf("Help...   "); break;
                    case 1:gotoxy(49,5);  cprintf("About...  "); break;
            }
    }
 }

int menuctrl(Hnode *Hhead,int A) /*菜单控制*/
  {
        int x,y,i,B,value,flag=100,a,b;
        x=wherex();  y=wherey();
        if(A==F1) {  drawmenu(0,flag);   value=300;  } /*显示File及其子菜单,并将光带显示在第一个子菜单上*/
        if(A==F2) {  drawmenu(1,flag);   value=301;  } /*显示Edit及其子菜单,并将光带显示在第一个子菜单上*/
        if(A==F3) {  drawmenu(2,flag);   value=302;  } /*显示Help及其子菜单,并将光带显示在第一个子菜单上*/

        if(A==F1||A==F2||A==F3)
       {
                while((B=bioskey(0))!=ESC) /*选择用户按键*/
               {
                      if(flag==0)   flag=100;
                      if(value==0)  value=300;  /*此value为局部变量*/

                      if(B==UP)     drawmenu(value,--flag); /*循环上下移*/
                      if(B==DOWN)   drawmenu(value,++flag); /*循环上下移*/

                      if(B==LEFT) /*菜单项之间循环选择（左移）*/
                     {
                            flag=100;
                            drawmain();
                            window(2,2,79,23);
                            textbackground(9);
                            for(i=0;i<24;i++)
                                insline();
                            window(3,3,78,23);
                            textcolor(10);
                            view(Hhead);
                            drawmenu(--value,flag);

                      }
                       if(B==RIGHT)/*菜单项之间循环选择（右移）*/
                     {
                            flag=100;
                            drawmain();
                            window(2,2,79,23);
                            textbackground(9);
                            for(i=0;i<24;i++)
                                insline();
                            window(3,3,78,23);
                            textcolor(10);
                            view(Hhead);
                            drawmenu(++value,flag);

                     }
                       if(B==ENTER) /*选中某主菜单项的子菜单项（选中某项）*/
                      {
                            if(value%3==0)  b=5; /*File下有5个子菜单项*/
                            if(value%3==1)  b=4; /*Edit下有4个子菜单项*/
                            if(value%3==2)  b=2; /*Help下有2个子菜单项*/
                            a=(value%3)*10+flag%b;/*a表示选择子菜单的编号*/
                            drawmain();
                            window(2,2,79,23);
                            textbackground(9);
                            for(i=0;i<24;i++)
                                   insline();
                            window(3,3,78,23);
                            textcolor(10);
                            view(Hhead);
                            gotoxy(x,y);
                            if(a==0)   return 100; /*New*/
                            if(a==1)   return 101; /*Open*/
                            if(a==2)   return 102; /*Save*/
                            if(a==3)   return 103; /*Save As*/
                            if(a==4)   exit(0);    /*Exit*/

                            if(a==10)  return Cx; /*Cut*/
                            if(a==11)  return Cc; /*Copy*/
                            if(a==12)  return Cv; /*Paste*/
                            if(a==13)  return DEL;/*Clear*/

                            if(a==20)  return 120; /*Help... */
                            if(a==21)  return 121; /*About...*/
                      }

                       gotoxy(x+2,y+2);

              }
              /*若按键非F1、F2、F3*/
                drawmain();
                window(2,2,79,23);
                textbackground(9);
                for(i=0;i<24;i++)
                     insline();
                window(3,3,78,23);
                textcolor(10);
                view(Hhead);
                gotoxy(x,y);



       }
        return A;


  }


/*将head所指的行单链表中所指的各个列单链表中的数据域的值写入文件，文件路径和文件名由用户指定*/
void save(Hnode *head)
{
FILE* fp;
Hnode *q;
node *p;
int count=0,x,y;
char filename[10]; /*保存文件名*/
q=head;
clrscr();/*清屏*/
printf("Enter infile name,for example [c:\\wb.txt]:");/*输入文件名格式*/
scanf("%s",filename); /*输入文件名*/
fp=fopen(filename,"w");
if(fp==NULL) /*打开文件失败*/
{
  printf("\n=====>open file error!\n");
  getchar();
  return ;
}
       do{
           p=q->next; /*指向node类型的数据*/
           while(p!=NULL)
             {      if((int)p->ch==13)
                    {
                      fputc('\n',fp);p=p->next; count++;
                     }
                    else
                    {fputc(p->ch, fp);
                     p=p->next;
                     count++;}
             }
           q=q->nextl;
         }while(q!=NULL);

fclose(fp); /*关闭此文件*/
return ;
}

/*文件另存为:将head所指的行单链表中所指的各个列单链表中的数据域的值写入文件，文件路径和文件名由用户指定*/
void saveas(Hnode *head)
{
FILE* fp;
Hnode *q;
node *p;
int count=0,x,y;
char filename[10]; /*保存文件名*/
q=head;
clrscr();/*清屏*/
printf("Enter infile name,for example [c:\\wb.txt]:");/*输入文件名格式*/
scanf("%s",filename); /*输入文件名*/
fp=fopen(filename,"w");
if(fp==NULL) /*打开文件失败*/
{
  printf("\n=====>open file error!\n");
  getchar();
  return ;
}
       do{
           p=q->next; /*指向node类型的数据*/
           while(p!=NULL)
             {       if((int)p->ch==13)
                    {
                      fputc('\n',fp);p=p->next; count++;
                     }
                    else
                    {fputc(p->ch, fp);
                     p=p->next;
                     count++;}

             }
           q=q->nextl;
         }while(q!=NULL);

fclose(fp); /*关闭此文件*/
return ;
}

/*从任意文本文件中读取文件内容，保存至行单链表和列单链表形式的数据结构中*/
void opens(Hnode *Hp)
{
FILE* fp;
Hnode *q11,*q22;
node *p11,*p22,*hp;
char temp;
int count=0,flags=1;
char filename[10]; /*保存文件名*/
clrscr();/*清屏*/
printf("Enter infile name,for example [c:\\wb.txt]:");/*输入文件名格式*/
scanf("%s",filename); /*输入文件名*/
fp=fopen(filename,"r");/*以只读方式打开文件，filename必须要存在*/
if(fp==NULL)/*打开文件失败*/
{  textbackground(2);
   textcolor(13);
   cprintf("open file error!");
   getchar();
   exit(0) ;
}
q11=Hp;
while(!feof(fp))
{   count=0;flags=1;
    q22=(Hnode *)malloc(sizeof(Hnode));/*新建一个行单链表中的节点*/
    p11=(node *)malloc(sizeof(node));  /*新建一个列单链表中的节点*/
    while((temp=fgetc(fp))!=10&&count<=76&&!feof(fp)) /*循环结束，表示在单链表中一行处理完毕，开始新行*/
    {  p22=(node *)malloc(sizeof(node));/*新建一个列单链表中的节点*/
       if(flags==1) {hp=p22;flags=0;} /*hp保存列单链表中的首节点的地址*/
       p22->ch=temp;  p22->next=NULL;
       p11->next=p22; p11=p22;
       count++;
    }
    if(temp==10){ /*若为换行符，将其转换为回车符，因为在程序中，是按回车符处理的*/
       p22=(node *)malloc(sizeof(node));p22->ch=13; p22->next=NULL;
       p11->next=p22; p11=p22;
    }
    if(!feof(fp))/*若没此条件，文件最后一行会处理两次.*/
    {q22->next=hp;q22->nextl=NULL; /*将存储了字符的新列单链表与行单链表中的新节点建立关联*/
     q11->nextl=q22;q11=q22;}
}
fclose(fp);
Hp=Hp->nextl;/*因为Hp的所在节点的数据域为空，所以Hp=Hp->nextl*/
return ;
}


void main()
 {
          char a;
          int i,A,x,y,flag=0,b;
          Hnode *Hhead,*q;
          node *p1,*p2;
          Hhead=(Hnode *)malloc(sizeof(Hnode)); /*为行单链表中首节点分配内存空间*/
          q=Hhead;      Hhead->nextl=NULL;
          p1=p2=q->next=(node *)malloc(sizeof(node)); /*为列单链表中首节点分配内存空间*/
          p1->ch=13; p1->next=NULL;
          drawmain();     /*显示主窗口*/
          window(2,2,79,23);
          textbackground(9);
          for(i=0;i<24;i++)
                insline();
          window(3,3,78,23);
          textcolor(10);

          while(1)
         {
                while(bioskey(1)==0) continue; /*等待用户按键*/
                a=A=bioskey(0); /*返回输入的字符的键值*/
                if(a>=32&&a<127) /*若输入为常规字符或回车键*/
               {

                     if(check(Hhead,wherey(),wherex())<=0)/*当前位置没有字符且输入是常规字符，则执行添加字符操作*/
                    {
                            NUM++;
                            p2->ch=a;
                            putch(a);
                            if(NUM==76) /*连续输入满行，分别生成一个新的行单链表和列单链表节点*/
                           {
                                p2->next=NULL;
                                q->nextl=(Hnode *)malloc(sizeof(Hnode));
                                q=q->nextl;   q->nextl=NULL;  q->next=NULL;
                                p1=p2=q->next=(node *)malloc(sizeof(node));
                                p1->ch=13;  p1->next=NULL;
                                NUM=0;
                           }
                            else /*连续输入未满一行,生成一个新的列单链表节点*/
                           {
                                 p2->next=(node *)malloc(sizeof(node));
                                 p2=p2->next;
                                 p2->ch=13;
                                 p2->next=NULL;

                           }
                     }

                      else /*当前位置有字符且输入是常规字符,则执行插入字符操作*/
                     {
                             x=wherex();  y=wherey();
                             insert(Hhead,wherey(),wherex(),a);
                             NUM++;
                             view(Hhead);
                             gotoxy(x,y);
                     }



                }

                /*若输入为回车键*/
                if(a==13)
               {
                       gotoxy(1,wherey()+1);
                       q->nextl=(Hnode *)malloc(sizeof(Hnode));
                       q=q->nextl;    q->nextl=NULL;   q->next=NULL;
                       p1=p2=q->next=(node *)malloc(sizeof(node));
                       p1->ch=13; p1->next=NULL;
                       NUM=0;
               }

                 x=wherex();   y=wherey();
                /*文本窗口中左移,当前光标不在窗口的第1列*/
                if((A==LEFT)&&(x!=1))  gotoxy(wherex()-1,wherey());
                /*文本窗口中左移,当前光标在窗口的第1列*/
                if((A==LEFT)&&(x==1))  gotoxy(abs(judge(Hhead,wherey()-1)),wherey()-1);
                /*文本窗口中右移,若当前光标的右边一位有字符*/
                if((A==RIGHT)&&check(Hhead,wherey(),wherex())>0)  gotoxy(wherex()+1,wherey());
                /*文本窗口中右移至下行的第1列,若当前光标位置没有字符且下行的第1列有字符*/
                if((A==RIGHT)&&check(Hhead,wherey()+1,1)!=0&&check(Hhead,y,x)<=0)  gotoxy(1,wherey()+1);
                 /*右移*/
                if((A==RIGHT)&&x==76)    gotoxy(1,wherey()+1);
                 /*上移*/
                if((A==UP)&&check(Hhead,wherey()-1,wherex())!=0)  gotoxy(wherex(),wherey()-1);
                 /*上移*/
                if((A==UP)&&check(Hhead,wherey()-1,wherex())<=0)
               {
                    if(judge(Hhead,wherey()-1)==0)
                        gotoxy(-judge(Hhead,wherey()-1)+1,wherey()-1);
                    else
                        gotoxy(-judge(Hhead,wherey()-1),wherey()-1);
               }

                /*下移*/
                if((A==DOWN)&&check(Hhead,wherey()+1,wherex())!=0)
                    gotoxy(wherex(),wherey()+1);

                /*处理BackSpace键*/
                if(A==BACK) /*处理BackSpace键*/
               {
                       flag=del(Hhead,wherey(),wherex()-1);
                       x=wherex()-1;    y=wherey();
                       view(Hhead);
                       if(flag==0)
                      {
                            if(x!=0)  gotoxy(x,y);
                            else  gotoxy(x+1,y);
                      }
                       if(flag==1)
                      {
                           gotoxy(x+1,y);
                           flag=0;
                      }
               }
               /*处理菜单按键F1 F2 F3*/
               if((A==F1)||(A==F2)||(A==F3)||(a<32||a>127))
               {   A=menuctrl(Hhead,A);
                  if(A==100){main();} /*新建文件*/

                  if(A==101){ /*打开文件*/
                    Hhead=(Hnode *)malloc(sizeof(Hnode));
                    opens(Hhead);
                    getchar();clrscr();gotoxy(3,3);view(Hhead);
                    }
                /*保存文件*/
                 if(A==102){save(Hhead);clrscr();cprintf("save successfully!");getch();gotoxy(3,3);view(Hhead);}
                /*文件另存为*/
                 if(A==103){saveas(Hhead);clrscr();cprintf("save as successfully!");getch();gotoxy(3,3);view(Hhead);}
                /*帮助*/
                 if(A==120){clrscr();cprintf("<Help> F1:File  F2:Edit F3:Help ");
                           getch();gotoxy(3,3);view(Hhead);}
                 if(A==121){clrscr();cprintf("Abort:Version 2.0 Tel:XXXXXXXXXX");getch();gotoxy(3,3);view(Hhead);}
               }

                /*处理DEL键,删除当前位置的单个字符*/
               if(A==DEL)
               {
                      x=wherex();    y=wherey();
                      del(Hhead,wherey(),wherex());
                      view(Hhead);
                      gotoxy(x,y);
               }
               /*处理已经选定文本字符后，按DEL键的情况*/
               if(A==DEL&&value!=0)
                {
                      if(value>0)
                            x=wherex(),    y=wherey();
                      else
                            x=r[0].col, y=r[0].line;
                      for(i=0;i<abs(value);i++)
                     {
                           if(value>0)
                              del(Hhead,r[i].line,r[i].col);
                           if(value<0)
                              del(Hhead,r[abs(value)-1-i].line,r[abs(value)-1-i].col);
                     }
                      value=0; /*此value为全局变量*/
                      view(Hhead);
                      gotoxy(x,y);
               }
              /*处理Ctrl+x按键*/
              if(A==Cx&&value!=0)
               {
                      if(value>0)
                            x=wherex(),    y=wherey();
                      else
                            x=r[0].col, y=r[i].line;
                      for(i=0;i<abs(value);i++)
                     {
                           if(value>0)
                              del(Hhead,r[i].line,r[i].col);
                           if(value<0)
                              del(Hhead,r[abs(value)-1-i].line,r[abs(value)-1-i].col);
                     }
                           backup=value; /*保存r数组的有值元素的最大下标值*/
                           value=0; /*此value为全局变量*/
                           view(Hhead);
                           gotoxy(x,y);
               }

               /*处理Ctrl+c按键*/
                if(A==Cc&&value!=0)
               {
                     x=wherex();      y=wherey();
                     backup=value;    value=0; /*此value为全局变量*/
                     view(Hhead);
                     gotoxy(x,y);
               }

               /*处理Ctrl+v按键*/
                if(A==Cv&&backup!=0)
               {
                     x=wherex();    y=wherey();
                     if(backup<0) /*Ctrl+右移键选定的文本，贴切此当前位置*/
                            for(i=0;i<abs(backup);i++)
                                insert(Hhead,y,x+i,r[i].ch);/*逐个插入*/


                     if(backup>0) /*Ctrl+左移键选定的文本，贴切此当前位置*/
                            for(i=0;i<backup;i++)
                                insert(Hhead,y,x+i,r[backup-1-i].ch);

                     view(Hhead);
                     gotoxy(x,y);
               }
               /*快速预览*/
                if(A==F10)
               {
                        qview(Hhead);
                        view(Hhead);
                        gotoxy(x,y);
               }

                /*处理Ctrl+左移键或右移键*/
                if(A==CL||A==CR)    control(A,Hhead);
                /*显示当前行列号*/
                x=wherex();   y=wherey();
                window(1,1,80,25);
                textcolor(0);
                textbackground(7);
                gotoxy(10,25); /*第25行，第10列,输出当前行号wherey()*/
                cprintf("%-3d",y);
                gotoxy(24,25); /*第25行，第24列*/
                cprintf("%-3d",x);
                window(3,3,78,23);
                textcolor(10);
                gotoxy(x,y);
                textcolor(10);
                textbackground(1);
        }
  }
