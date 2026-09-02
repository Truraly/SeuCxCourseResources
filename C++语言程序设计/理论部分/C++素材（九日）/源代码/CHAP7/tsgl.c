#include "bios.h"     /*基本输入输出系统函数库*/
#include "math.h"     /*数学运算函数库*/
#include "stdio.h"   /*标准输入输出函数库*/
#include "stdlib.h"  /*标准函数库*/
#include "string.h"  /*字符串函数库*/
#include "conio.h"    /*屏幕操作函数库*/
/*与按键有关的宏定义*/
#define LEFT 0x4b00   /*←：光标左移*/
#define RIGHT 0x4d00  /*→：光标右移*/
#define DOWN 0x5000   /*↓键：光标下移*/
#define UP 0x4800     /*↑键：光标上移*/
#define ESC 0x011b    /*ESC键：取消菜单打开操作*/
#define F1 15104      /*F1键：打开文件菜单*/
#define F2 15360      /*F2键：打开编辑菜单*/
#define F3 15616      /*F3键：打开帮助菜单*/
#define ENTER 0x1c0d  /*回车键：换行*/
/*与Book菜单选项有关的宏定义*/
#define ADD_BOOK 100
#define QUERY_BOOK 101
#define MODIFY_BOOK 102
#define DEL_BOOK 103
#define SORT_BOOK 104
#define COUNT_BOOK 105
/*与Reader菜单选项有关的宏定义*/
#define ADD_READER 200
#define QUERY_READER 201
#define MODIFY_READER 202
#define DEL_READER 203
#define SORT_READER 204
#define COUNT_READER 205
/*与B&R菜单选项有关的宏定义，其中B表示借书，R表示还书*/
#define BORROW_BOOK 300
#define RETURN_BOOK 301
#define EXIT        302
/*与book记录格式化输出有关的宏定义*/
#define HEADER1 "|Number    |Name      |Author    |Publish   |Price| F|Reader    |Tota| \n"
#define FORMAT1  "|%-10s|%-10s|%-10s|%-10s|%.2f|%2d|%-10s|%4d|\n"
#define DATA1  lll->data.num,lll->data.name,lll->data.author,lll->data.publish,lll->data.price,lll->data.borrow_flag,lll->data.reader,lll->data.total_num
/*与reader记录格式输出有关的宏定义*/
#define HEADER2 "|Number      |Name      |Sex |Age |Tele           |Tota| \n"
#define FORMAT2  "|%-12s|%-10s|%-4s|%4d|%15s|%4d|\n"
#define DATA2  lll->data.num,lll->data.name,lll->data.sex,lll->data.age,lll->data.tele,lll->data.total_num

int saveflag=0;  /*是否需要存盘的全局标志变量*/

/*定义与图书有关的数据结构*/
typedef struct book      /*标记为book*/
{
char   num[15];      /*图书编号*/
char   name[15];     /*图书名  */
char   author[15];   /*图书作者*/
char   publish[15];  /*出版社  */
float  price;        /*图书定价*/
int    borrow_flag;  /*图书是否借出，1表示借出，0表示未借出*/
char   reader[12];   /*借阅人编号*/
int    total_num;    /*图书被借次数*/
};

/*定义与读者有关的数据结构*/
typedef struct reader      /*标记为reader*/
{
char num[12];      /*读者编号，如可按注册日期中的顺序，如2010-10-10-1*/
char name[15];     /*读者姓名*/
char sex[4];       /*读者性别M或F,Male:男性，Female:女性*/
int  age;          /*读者年龄*/
char tele[15];     /*读者联系电话*/
int  total_num;    /*读者目前已借图书册数*/
};


/*定义每条图书记录的数据结构，标记为：book_node*/
typedef struct book_node
{
struct book data;       /*数据域*/
struct book_node *next; /*指针域*/
}Book_Node,*Book_Link;  /*Book_Node为book_node类型的结构变量，*Book_Link为book_node类型的指针变量*/

/*定义每条读者记录的数据结构，标记为：reader_node*/
typedef struct reader_node
{
struct reader data;          /*数据域*/
struct reader_node *next;    /*指针域*/
}Reader_Node,*Reader_Link;   /*Reader_Node为reader_node类型的结构变量，*Reader_Link为reader_node类型的指针变量*/


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
          gotoxy(7,1); cprintf("%c  %c Book %c  %c",179,17,16,179);  /* | < > |*/
          gotoxy(27,1); cprintf("%c  %c Reader %c  %c",179,17,16,179); /* | < > |*/
          gotoxy(47,1); cprintf("%c  %c B&R %c  %c",179,17,16,179); /* | < > |*/
          gotoxy(5,25); /*跳至窗口底端*/
          textcolor(1);
          cprintf(" Book Management System");
          gotoxy(68,25);
          cprintf("Version 2.0");
 }


 void drawmenu(int m,int n) /*画菜单,m:第几项菜单，n：第m项的第n个子菜单*/
 {
       int i;
      if(m%3==0) /*画Book菜单项*/
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
            gotoxy(9,3);  cprintf(" Add     ");
            gotoxy(9,4);  cprintf(" Query   ");
            gotoxy(9,5);  cprintf(" Modify  ");
            gotoxy(9,6);  cprintf(" Delete  ");
            gotoxy(9,7);  cprintf(" Sort    ");
            gotoxy(9,8);  cprintf(" Count   ");
            textcolor(15);  textbackground(0);
            gotoxy(7,1);
            cprintf("%c  %c Book %c  %c",179,17,16,179);
            switch(n%6)
           {
                case 0:gotoxy(9,3);  cprintf(" Add      "); break;
                case 1:gotoxy(9,4);  cprintf(" Query    "); break;
                case 2:gotoxy(9,5);  cprintf(" Modify   "); break;
                case 3:gotoxy(9,6);  cprintf(" Delete   "); break;
                case 4:gotoxy(9,7);  cprintf(" Sort     "); break;
                case 5:gotoxy(9,8);  cprintf(" Count    "); break;
           }
      }

     /********************************************************/
     if(m%3==1) /*画Reader菜单项*/
    {
            window(28,2,39,9);
            textcolor(0);
            textbackground(7);
            for(i=0;i<7;i++) /*在上面定义的文本窗口中先输出6个空行*/
           {
                gotoxy(1,1+i);
                insline();
            }
            window(1,1,80,25);
            gotoxy(27,1);
            for(i=1;i<=7;i++)
           {
                gotoxy(28,1+i);
                cprintf("%c",179); /*窗口内文本的输出函数，在窗口左边输出 | */
                gotoxy(39,1+i);
                cprintf("%c",179);  /*窗口内文本的输出函数，在窗口右边输出 | */
            }
            for(i=1;i<=11;i++)
           {
                  gotoxy(28+i,2);
                  cprintf("%c",196); /*窗口内文本的输出函数，在窗口上边输出 - */
                  gotoxy(28+i,9);
                  cprintf("%c",196); /*窗口内文本的输出函数，在窗口下边输出 - */
            }

            textbackground(0);
            gotoxy(30,10); cprintf("            ");
            for(i=0;i<9;i++)    /*输出右边的阴影效果*/
           {
                  gotoxy(40,2+i);
                  cprintf("   ");
           }
            textbackground(7);
            gotoxy(28,2);  cprintf("%c",218);
            gotoxy(28,9);  cprintf("%c",192);
            gotoxy(39,2);  cprintf("%c",191);
            gotoxy(39,9);  cprintf("%c",217);
            gotoxy(29,3);  cprintf(" Add     ");
            gotoxy(29,4);  cprintf(" Query   ");
            gotoxy(29,5);  cprintf(" Modify  ");
            gotoxy(29,6);  cprintf(" Delete  ");
            gotoxy(29,7);  cprintf(" Sort    ");
            gotoxy(29,8);  cprintf(" Count   ");
            textcolor(15);  textbackground(0);
            gotoxy(27,1);
            cprintf("%c  %c Reader %c  %c",179,17,16,179);
            switch(n%6)
            {
                    case 0:gotoxy(29,3);  cprintf(" Add     "); break;
                    case 1:gotoxy(29,4);  cprintf(" Query   "); break;
                    case 2:gotoxy(29,5);  cprintf(" Modify  "); break;
                    case 3:gotoxy(29,6);  cprintf(" Delete  "); break;
                    case 4:gotoxy(29,7);  cprintf(" Sort    "); break;
                    case 5:gotoxy(29,8);  cprintf(" Count   "); break;


            }
      }

    /*********************************************************/
     if(m%3==2) /*画B&R菜单项3*/
    {
            window(48,2,59,8);
            textcolor(0);
            textbackground(7);
            for(i=0;i<7;i++)
            {
                    gotoxy(1,1+i);
                    insline();
            }
            window(1,1,80,25);
            gotoxy(47,1);
            for(i=1;i<=7;i++)
            {
                    gotoxy(48,1+i);
                    cprintf("%c",179);   /*窗口内文本的输出函数，在窗口左边输出 | */
                    gotoxy(59,1+i);
                    cprintf("%c",179);   /*窗口内文本的输出函数，在窗口右边输出 | */
            }
            for(i=1;i<=11;i++)
            {
                    gotoxy(48+i,2);   /*窗口内文本的输出函数，在窗口上边输出 - */
                    cprintf("%c",196);
                    gotoxy(48+i,8);
                    cprintf("%c",196);   /*窗口内文本的输出函数，在窗口下边输出 - */
            }

            textbackground(0);
            gotoxy(50,9); cprintf("            "); /*输出下边的阴影效果*/
            for(i=0;i<8;i++)      /*输出右边的阴影效果*/
           {
                  gotoxy(60,2+i);
                  cprintf("   ");
           }
            textbackground(7);
            gotoxy(48,2);   cprintf("%c",218);
            gotoxy(48,8);   cprintf("%c",192);
            gotoxy(59,2);   cprintf("%c",191);
            gotoxy(59,8);   cprintf("%c",217);
            gotoxy(49,3);   cprintf(" Borrow   ");
            gotoxy(49,5);   cprintf(" Return   ");
            gotoxy(49,7);   cprintf(" Exit     ");
            for(i=1;i<=10;i++)
            {
                    gotoxy(48+i,4);
                    cprintf("%c",196);
            }
            for(i=1;i<=10;i++)
            {
                    gotoxy(48+i,6);
                    cprintf("%c",196);
            }
            textcolor(15);  textbackground(0);
            gotoxy(47,1);
            cprintf("%c  %c B&R %c  %c",179,17,16,179);
            switch(n%3)
            {
                    case 0:gotoxy(49,3);  cprintf(" Borrow  "); break;
                    case 1:gotoxy(49,5);  cprintf(" Return  "); break;
                    case 2:gotoxy(49,7);  cprintf(" Exit    "); break;


            }
    }
 }


int menuctrl(int A) /*菜单控制*/
  {
        int x,y,i,B,value,flag=36,a,b;
        x=wherex();  y=wherey();
        if(A==F1) {  drawmenu(0,flag);   value=300;  } /*显示Book及其子菜单,并将光带显示在第一个子菜单上*/
        if(A==F2) {  drawmenu(1,flag);   value=301;  } /*显示Reader及其子菜单,并将光带显示在第一个子菜单上*/
        if(A==F3) {  drawmenu(2,flag);   value=302;  } /*显示B&R及其子菜单,并将光带显示在第一个子菜单上*/

        if(A==F1||A==F2||A==F3)
       {
                while((B=bioskey(0))!=ESC) /*选择用户按键*/
               {
                      if(flag==0)   flag=36;
                      if(value==0)  value=300;  /*此value为局部变量*/

                      if(B==UP)     drawmenu(value,--flag); /*循环上下移*/
                      if(B==DOWN)   drawmenu(value,++flag); /*循环上下移*/

                      if(B==LEFT) /*菜单项之间循环选择（左移）*/
                     {
                            flag=36;
                            drawmain();
                            window(2,2,79,23);
                            textbackground(9);
                            for(i=0;i<24;i++)
                                insline();
                            window(3,3,78,23);
                            textcolor(10);
                            drawmenu(--value,flag);

                      }
                       if(B==RIGHT)/*菜单项之间循环选择（右移）*/
                     {
                            flag=36;
                            drawmain();
                            window(2,2,79,23);
                            textbackground(9);
                            for(i=0;i<24;i++)
                                insline();
                            window(3,3,78,23);
                            textcolor(10);
                            drawmenu(++value,flag);

                     }
                       if(B==ENTER) /*选中某主菜单项的子菜单项（选中某项）*/
                      {
                            if(value%3==0)  b=6; /*Book下有6个子菜单项*/
                            if(value%3==1)  b=6; /*Reader下有6个子菜单项*/
                            if(value%3==2)  b=3; /*B&R下有3个子菜单项*/
                            a=(value%3)*10+flag%b;/*a表示选择子菜单的编号*/
                            drawmain();
                            window(2,2,79,23);
                            textbackground(9);
                            for(i=0;i<24;i++)
                                   insline();
                            window(3,3,78,23);
                            textcolor(10);
                            gotoxy(x,y);
                            if(a==0)   return ADD_BOOK;
                            if(a==1)   return QUERY_BOOK;
                            if(a==2)   return MODIFY_BOOK;
                            if(a==3)   return DEL_BOOK;
                            if(a==4)   return SORT_BOOK;
                            if(a==5)   return COUNT_BOOK;

                            if(a==10)   return ADD_READER;
                            if(a==11)   return QUERY_READER;
                            if(a==12)   return MODIFY_READER;
                            if(a==13)   return DEL_READER;
                            if(a==14)   return SORT_READER;
                            if(a==15)   return COUNT_READER;

                            if(a==20)   return BORROW_BOOK;
                            if(a==21)   return RETURN_BOOK;
                            if(a==22)   return EXIT;
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
                gotoxy(x,y);



       }
        return A;


  }

/*************************************************************
作用：用于定位链表中符合要求的节点，并返回指向该节点的指针
参数：findmess[]保存要查找的具体内容; nameornum[]保存按什么查找;
      在单链表l中查找;
**************************************************************/
 Book_Node* Locate(Book_Link l,char findmess[],char nameornum[])
  {
  Book_Node *r;
  if(strcmp(nameornum,"num")==0) /*按图书编号查询*/
  {
  r=l->next;
  while(r)
  {
   if(strcmp(r->data.num,findmess)==0) /*若找到findmess值的图书编号*/
    return r;
   r=r->next;
  }
 }
 else if(strcmp(nameornum,"name")==0)  /*按图书名查询*/
  {
  r=l->next;
  while(r)
  {
   if(strcmp(r->data.name,findmess)==0)    /*若找到findmess值的图书名*/
    return r;
   r=r->next;
  }
 }
 return 0; /*若未找到，返回一个空指针*/
 }

/*************************************************************
作用：用于定位读者链表中符合要求的节点，并返回指向该节点的指针
参数：findmess[]保存要查找的具体内容; nameornum[]保存按什么查找;
      在单链表l中查找;
**************************************************************/
 Reader_Node* LocateReader(Reader_Link l,char findmess[],char nameornum[])
  {
  Reader_Node *r;
  if(strcmp(nameornum,"num")==0) /*按读者编号查询*/
  {
  r=l->next;
  while(r)
  {
   if(strcmp(r->data.num,findmess)==0) /*若找到findmess值的读者编号*/
    return r;
   r=r->next;
  }
 }
 else if(strcmp(nameornum,"name")==0)  /*按读者姓名查询*/
  {
  r=l->next;
  while(r)
  {
   if(strcmp(r->data.name,findmess)==0)    /*若找到findmess值的读者姓名*/
    return r;
   r=r->next;
  }
 }
 return 0; /*若未找到，返回一个空指针*/
 }


/*输入字符串，并进行长度验证(长度<lens)*/
void stringinput(char *t,int lens,char *notice)
{
   char n[255];
   int x,y;
   do{
      printf(notice);  /*显示提示信息*/
      scanf("%s",n);  /*输入字符串*/
      if(strlen(n)>lens)
      { x=wherex();  y=wherey();
        gotoxy(x+2,y+1);printf("exceed the required length! \n");
      } /*进行长度校验，超过lens值重新输入*/
     }while(strlen(n)>lens);
   strcpy(t,n); /*将输入的字符串拷贝到字符串t中*/
}

 /*增加图书记录*/
void AddBook(Book_Link l)
{
  Book_Node *p,*r,*s;  /*实现添加操作的临时的结构体指针变量*/
  char ch,flag=0,num[10];
  float temp;
  r=l;
  s=l->next;
  clrscr();
  while(r->next!=NULL)
  r=r->next; /*将指针移至于链表最末尾，准备添加记录*/
  while(1) /*一次可输入多条记录，直至输入图书编号为0的记录结点添加操作*/
  {
    while(1) /*输入图书编号，保证该图书编号没有被使用，若输入图书编号为0，则退出添加记录操作*/
     {
      clrscr();
      gotoxy(2,2);
      stringinput(num,15,"input book number(press '0'return menu):"); /*格式化输入图书编号并检验*/
      flag=0;

      if(strcmp(num,"0")==0) /*输入为0，则退出添加操作，返回主界面*/
      {return;}
      s=l->next;
      while(s) /*查询该图书编号是否已经存在，若存在则要求重新输入一个未被占用的图书编号*/
      {
      if(strcmp(s->data.num,num)==0)
      {
       flag=1;
       break;
       }
       s=s->next;
      }

  if(flag==1) /*提示用户是否重新输入*/

     {  gotoxy(2,3);
        getchar();
        printf("=====>The number %s is existing,please try again(y/n)?",num);
        scanf("%c",&ch);
        if(ch=='y'||ch=='Y')
         continue;
       else
         return;
      }
     else
      {break;}
  }
  
  p=(Book_Node *)malloc(sizeof(Book_Node)); /*申请内存空间*/
  if(!p)
   {
      printf("\n allocate memory failure "); /*如没有申请到，打印提示信息*/
      return ;             /*返回主界面*/
   }
  /*给图书记录赋值*/
  strcpy(p->data.num,num); /*将字符串num拷贝到p->data.num中*/
  gotoxy(2,3);stringinput(p->data.name,15,"Book Name:");
  gotoxy(2,4);stringinput(p->data.author,15,"Book Author:");
  gotoxy(2,5);stringinput(p->data.publish,15,"Book Publishing Company:");
  gotoxy(2,6);printf("Book Price:");scanf("%f",&temp); p->data.price=temp;
  p->data.borrow_flag=0;/*图书初始为未借出,1表示借出*/
  strcpy(p->data.reader,"");
  p->data.total_num=0;/*图书被借次数初始为0*/
  gotoxy(2,8);printf(">>>>press any key to start next record!");
  getchar(); getchar();

  /********************/
  p->next=NULL; /*表明这是链表的尾部结点*/
  r->next=p;  /*将新建的结点加入链表尾部中*/
  r=p;
  saveflag=1;
  }
     return ;
}

void QueryBook(Book_Link l) /*按图书编号或图书名查询*/
{
  int select; /*1:按图书编号查，2：按图书名查，其他：返回主界面（菜单）*/
  char searchinput[20]; /*保存用户输入的查询内容*/
  Book_Node *p;
  if(!l->next) /*若链表为空*/
  {
   clrscr();
   gotoxy(2,2);
   printf("\n=====>No Book Record!\n");
   getchar();getchar();
   return;
  }
  clrscr();
  gotoxy(2,2);
  cprintf("=====>1 Search by book number  =====>2 Search by book name");
  gotoxy(2,3);
  cprintf("please choice[1,2]:");
  scanf("%d",&select);
  if(select==1)   /*按图书编号查询*/
  {
   gotoxy(2,4);
   stringinput(searchinput,15,"input the existing book number:");
   p=Locate(l,searchinput,"num");/*在l中查找图书编号为searchinput值的节点，并返回节点的指针*/
   if(p) /*若p!=NULL*/
   {gotoxy(2,5);printf("---------------------------------------------");
    gotoxy(2,6);printf("Book Number:%s",p->data.num);
    gotoxy(2,7);printf("Book Name:%s",p->data.name);
    gotoxy(2,8);printf("Book Author:%s",p->data.author);
    gotoxy(2,9);printf("Book Publishing Company:%s",p->data.publish);
    gotoxy(2,10);printf("Book Price:%.2f",p->data.price);
    gotoxy(2,11);printf("Book Borrow_Flag(1:borrowed,0:un-borrowed):%d",p->data.borrow_flag);
    gotoxy(2,12);printf("Book Current Reader:%s",p->data.reader);
    gotoxy(2,13);printf("Total Number of Book Borrowed:%d",p->data.total_num);
    gotoxy(2,14);printf("---------------------------------------------");
    gotoxy(2,16);printf("press any key to return");
    getchar(); getchar();
   }
  else
   {gotoxy(2,5);printf("=====>Not find this book!\n");getchar();getchar();}
  }
 else if(select==2) /*按图书名查询*/
  {
  gotoxy(2,4);
  stringinput(searchinput,15,"input the existing book name:");
  p=Locate(l,searchinput,"name");
  if(p)
  { gotoxy(2,5);printf("---------------------------------------------");
    gotoxy(2,6);printf("Book Number:%s",p->data.num);
    gotoxy(2,7);printf("Book Name:%s",p->data.name);
    gotoxy(2,8);printf("Book Author:%s",p->data.author);
    gotoxy(2,9);printf("Book Publishing Company:%s",p->data.publish);
    gotoxy(2,10);printf("Book Price:%.2f",p->data.price);
    gotoxy(2,11);printf("Book Borrow_Flag(1:borrowed,0:un-borrowed):%d",p->data.borrow_flag);
    gotoxy(2,12);printf("Book Current Reader:%s",p->data.reader);
    gotoxy(2,13);printf("Total Number of Book Borrowed:%d",p->data.total_num);
    gotoxy(2,14);printf("---------------------------------------------");
    gotoxy(2,16);printf("press any key to return");
    getchar(); getchar();
  }
  else
   {gotoxy(2,5);printf("=====>Not find this book!\n");getchar();getchar();}
  }
 else
   {gotoxy(2,5);;printf("*****Error:input has wrong! press any key to continue******");
    getchar();getchar();}
}

/*删除图书记录：先找到保存该图书记录的节点，然后删除该节点*/
void DelBook(Book_Link l)
 {
  int sel;
  Book_Node *p,*r;
  char findmess[20];
  if(!l->next)
  { clrscr();
    gotoxy(2,2);
    printf("\n=====>No book record!\n");
    getchar();
    return;
   }
  clrscr();
  gotoxy(2,2);
  printf("=====>1 Delete by book number       =====>2 Delete by book name");
  gotoxy(2,3);
  printf("please choice[1,2]:");
  scanf("%d",&sel);
  if(sel==1)
  { gotoxy(2,4);
    stringinput(findmess,10,"input the existing book number:");
    p=Locate(l,findmess,"num");
    if(p)  /*p!=NULL*/
    {
     r=l;
     while(r->next!=p)
     r=r->next;
     r->next=p->next;/*将p所指节点从链表中去除*/
     free(p); /*释放内存空间*/
     gotoxy(2,6);
     printf("=====>delete success!");
     getchar();getchar();
     saveflag=1;
   }
  else
   {gotoxy(2,6);printf("=====>Not find this book!\n");getchar();getchar();}
 }
 else if(sel==2) /*先按图书名查询到该记录所在的节点*/
  {
  stringinput(findmess,15,"input the existing book name:");
  p=Locate(l,findmess,"name");
  if(p)
  {
   r=l;
   while(r->next!=p)
    r=r->next;
   r->next=p->next;
   free(p);
   gotoxy(2,6);
   printf("=====>delete success!\n");
   getchar();getchar();
   saveflag=1;
  }
  else
   {gotoxy(2,6);printf("=====>Not find this book!\n");getchar();getchar();}
 }
 else
  {gotoxy(2,6);printf("*****Error:input has wrong! press any key to continue******");
    getchar();getchar();}
}

/*修改图书记录。先按输入的图书编号查询到该记录，然后提示用户修改图书编号之外的用户输入字段*/
void ModifyBook(Book_Link l)
{
  Book_Node *p;
  char findmess[20];
  float temp;
  if(!l->next)
  { clrscr();
    gotoxy(2,1);
    printf("\n=====>No book record!\n");
    getchar();
    return;
  }
  clrscr();
  gotoxy(2,1);
  stringinput(findmess,10,"input the existing book number:"); /*输入并检验该图书编号*/
  p=Locate(l,findmess,"num"); /*查询到该节点*/
  if(p) /*若p!=NULL,表明已经找到该节点*/
  { gotoxy(2,2);printf("---------------------------------------------");
    gotoxy(2,3);printf("Book Number:%s",p->data.num);
    gotoxy(2,4);printf("Book Name:%s",p->data.name);
    gotoxy(2,5);printf("Book Author:%s",p->data.author);
    gotoxy(2,6);printf("Book Publishing Company:%s",p->data.publish);
    gotoxy(2,7);printf("Book Price:%.2f",p->data.price);
    gotoxy(2,8);printf("Book Borrow_Flag(1:borrowed,0:un-borrowed):%d",p->data.borrow_flag);
    gotoxy(2,9);printf("Book Current Reader:%s",p->data.reader);
    gotoxy(2,10);printf("Total Number of Book Borrowed:%d",p->data.total_num);
    gotoxy(2,11);printf("---------------------------------------------");
    gotoxy(2,12);printf("please modify book recorder:");
    gotoxy(2,13);stringinput(p->data.name,15,"Book Name:");
    gotoxy(2,14);stringinput(p->data.author,15,"Book Author:");
    gotoxy(2,15);stringinput(p->data.publish,15,"Book Publishing Company:");
    gotoxy(2,16);printf("Book Price:");scanf("%f",&temp); p->data.price=temp;
    gotoxy(2,17);printf("---------------------------------------------");
    gotoxy(2,18);printf("=====>modify success!"); getchar();getchar();
    saveflag=1;
   }
  else
  {gotoxy(2,4);printf("=====>Not find this book!\n");getchar();getchar();}
 }


/*统计图书数量，已借出图书数，借出次数最多的图书名*/
void CountBook(Book_Link l)
 {
  Book_Node *r=l->next;
  int countc=0,countm=0,counte=0;
  char bookname[15];
  if(!r)
  { clrscr();
    gotoxy(2,1);
    printf("=====>Not book record!");
    getchar();
    return ;
  }
 counte=r->data.total_num;
 strcpy(bookname,r->data.name);
 while(r)
  {
   countc++; /*统计图书数量*/
   if (r->data.borrow_flag==1) countm++;  /*统计已借出图书数*/
   /*保存借出次数最多的图书名*/
   if(r->data.total_num>counte) {counte=r->data.total_num;strcpy(bookname,r->data.name); }
   r=r->next;
 }
 clrscr();
 gotoxy(2,3);
 printf("------------------------------the TongJi result--------------------------------");
 gotoxy(2,4);
 printf("Total number of books:%d",countc);
 gotoxy(2,5);
 printf("Total number of borrowed books:%d",countm);
 gotoxy(2,6);
 printf("Book name of maximum borrowed number:%s",bookname);
 gotoxy(2,7);
 printf("-------------------------------------------------------------------------------");
 getchar();getchar();
}

/*利用直接选择排序法实现单链表按图书价格字段的升序排序，从低到高*/
void SortBook(Book_Link l)
{
  Book_Link lll; /*临时指针*/
  Book_Node *p,*q,*r,*s,*h1;  /*临时指针*/
  int x,y; /*保存当前光标所在位置的坐标值*/
  int i=0;
  if(l->next==NULL)
  { clrscr();
    gotoxy(2,1);
    printf("=====>Not book record!");
    getchar();
    return ;
  }

  h1=p=(Book_Node*)malloc(sizeof(Book_Node)); /*用于创建新的头节点*/
  if(!p)
   {
      gotoxy(2,1);
      printf("allocate memory failure "); /*如没有申请到，打印提示信息*/
      return ;             /*返回主界面*/
   }
    /************显示排序前的所有记录*********/
    clrscr();
    gotoxy(2,1);
    printf(HEADER1);
    gotoxy(2,1);
    lll=l->next;
    x=wherex();y=wherey();i=0;
    while(lll!=NULL)  /*当p不为空时，进行下列操作*/
    {
      i++;
      gotoxy(x,i+y);   /*换行*/
      printf(FORMAT1,DATA1); /*见头部的宏定义*/
      lll=lll->next;   /*指针后移*/
    }
    getchar();getchar();
    gotoxy(2,i+2);
    printf("=====>sort..............");
   /********************排序**************************/
   p->next=l->next; /*l所指节点为不存有任何记录的节点，下一个节点才有图书记录*/
   while(p->next!=NULL) /*外层循环决定待排序位置*/
   {
    q=p->next;
    r=p;
     while(q->next!=NULL)/*内循环找到当前关键字最小节点*/
      {
      if (q->next->data.price<r->next->data.price) r=q; /*r为记录当前最小节点的前驱节点的指针变量*/
      q=q->next; /*移至下一个节点*/
      }

     if (r!=p)
     /*表示原来的第1个节点不是关键字最小的节点，改变指针的关系，
     将关键字最小的节点与本轮循环的首节点进行位置互换*/
      {
       s=r->next; /*s指向最小节点*/
       r->next=s->next; /*r的指针域指向最小节点的下一个节点*/
       s->next=p->next; /*s的指针域指向当前p指针所指的下一个节点*/
       p->next=s; /*p的指针域指向本次循环结束后关键字最小的节点*/
       }
      p=p->next;/*移至下一个节点，过行外循环控制*/
      }/*外层while*/
     /*********************************************/
     l->next=h1->next;/*将排序好的链表首节点地址赋给原来链表的指针域*/

     /*显示排序后的所有图书记录*/
    lll=l->next;
    gotoxy(x,y+i+1);
    x=wherex();y=wherey();i=0;
    while(lll!=NULL)  /*当p不为空时，进行下列操作*/
    {
      i++;
      gotoxy(x,i+y);
      printf(FORMAT1,DATA1);
      lll=lll->next;   /*指针后移*/
    }
      free(h1);
      saveflag=1;
      gotoxy(2,wherey()+2);
      printf("=====>sort complete!\n");
      getchar();getchar();
      return;
   }


  /*数据存盘,若用户对数据有修改，在退出系统时，系统会自动存盘*/
 void SaveBook(Book_Link l)
 {
  FILE* fp;
  Book_Node *p;
  int count=0;
  fp=fopen("c:\\book","wb");/*以只写方式打开二进制文件*/
  if(fp==NULL) /*打开文件失败*/
  {
  clrscr();
  gotoxy(2,2);
  printf("=====>open file error!\n");
  getchar();
  return ;
  }
  p=l->next;
  while(p)
  {
   if(fwrite(p,sizeof(Book_Node),1,fp)==1)/*每次写一条记录或一个节点信息至文件*/
  { 
    p=p->next;
    count++;
   }
  else
  {
    break;
   }
  }
 if(count>0)
  {
  clrscr();
  gotoxy(4,8);
  printf("=====>save book,total saved's record number is:%d\n",count);
  getchar();
  saveflag=0;
 }
 else
 {clrscr();
  gotoxy(2,3);
  printf("the current link is empty,no record is saved!\n");
  getchar();
 }
 fclose(fp); /*关闭book文件*/
 }


 /***************与读者相关的函数*******************/
 /*增加读者记录*/
void AddReader(Reader_Link l)
{
  Reader_Node *p,*r,*s;  /*实现添加操作的临时的结构体指针变量*/
  char ch,flag=0,num[10];
  float temp;
  r=l;
  s=l->next;
  clrscr();
  while(r->next!=NULL)
  r=r->next; /*将指针移至于链表最末尾，准备添加记录*/
  while(1) /*一次可输入多条记录，直至输入读者编号为0的记录结点添加操作*/
  {
    while(1) /*输入读者编号，保证该读者编号没有被使用，若输入读者编号为0，则退出添加记录操作*/
     {
      clrscr();
      gotoxy(2,2);
      stringinput(num,12,"Input Reader Number(press '00'return menu):"); /*格式化输入读者编号并检验*/
      flag=0;

      if(strcmp(num,"00")==0) /*输入为0，则退出添加操作，返回主界面*/
      {return;}
      s=l->next;
      while(s) /*查询该读者编号是否已经存在，若存在则要求重新输入一个未被占用的读者编号*/
      {
      if(strcmp(s->data.num,num)==0)
      {
       flag=1;
       break;
       }
       s=s->next;
      }

  if(flag==1) /*提示用户是否重新输入*/

     {  gotoxy(2,3);
        getchar();
        printf("=====>The number %s is existing,please try again(y/n)?",num);
        scanf("%c",&ch);
        if(ch=='y'||ch=='Y')
         continue;
       else
         return;
      }
     else
      {break;}
  }
  
  p=(Reader_Node *)malloc(sizeof(Reader_Node)); /*申请内存空间*/
  if(!p)
   {
      printf("\n allocate memory failure "); /*如没有申请到，打印提示信息*/
      return ;             /*返回主界面*/
   }
  /*给读者记录赋值*/
  strcpy(p->data.num,num); /*将字符串num拷贝到p->data.num中*/
  gotoxy(2,3);stringinput(p->data.name,15,"Reader Name:");
  gotoxy(2,4);stringinput(p->data.sex,4,"Reader Sex(M or F)):");
  gotoxy(2,5);printf("Reader Age:");scanf("%f",&temp); p->data.age=temp;
  gotoxy(2,6);stringinput(p->data.tele,15,"Reader Telephone:");
  p->data.total_num=0;/*读者目前已借图书册数初始化为0*/
  gotoxy(2,8);printf(">>>>press any key to start next record!");
  getchar(); getchar();
  /********************/
  p->next=NULL; /*表明这是链表的尾部结点*/
  r->next=p;  /*将新建的结点加入链表尾部中*/
  r=p;
  saveflag=1;
  }
     return ;
}

void QueryReader(Reader_Link l) /*按读者编号或读者名查询*/
{
  int select; /*1:按读者编号查，2：按读者名查，其他：返回主界面（菜单）*/
  char searchinput[20]; /*保存用户输入的查询内容*/
  Reader_Node *p;
  if(!l->next) /*若链表为空*/
  {
   clrscr();
   gotoxy(2,2);
   printf("\n=====>No Reader Record!\n");
   getchar();getchar();
   return;
  }
  clrscr();
  gotoxy(2,2);
  cprintf("=====>1 Search by Reader number  =====>2 Search by Reader name");
  gotoxy(2,3);
  cprintf("please choice[1,2]:");
  scanf("%d",&select);
  if(select==1)   /*按读者编号查询*/
  {
   gotoxy(2,4);
   stringinput(searchinput,15,"input the existing Reader number:");
   p=LocateReader(l,searchinput,"num");/*在l中查找读者编号为searchinput值的节点，并返回节点的指针*/
   if(p) /*若p!=NULL*/
   {gotoxy(2,5);printf("---------------------------------------------");
    gotoxy(2,6);printf("Reader Number:%s",p->data.num);
    gotoxy(2,7);printf("Reader Name:%s",p->data.name);
    gotoxy(2,8);printf("Reader Sex:%s",p->data.sex);
    gotoxy(2,9);printf("Reader Age:%d",p->data.age);
    gotoxy(2,10);printf("Reader Tele:%s",p->data.tele);
    gotoxy(2,11);printf("Total Number of Reader Borrowed:%d",p->data.total_num);
    gotoxy(2,12);printf("---------------------------------------------");
    gotoxy(2,14);printf("press any key to return");
    getchar(); getchar();
   }
  else
   {gotoxy(2,5);printf("=====>Not find this Reader!\n");getchar();getchar();}
  }
 else if(select==2) /*按读者名查询*/
  {
  gotoxy(2,4);
  stringinput(searchinput,15,"input the existing Reader name:");
  p=LocateReader(l,searchinput,"name");
  if(p)
  { gotoxy(2,5);printf("---------------------------------------------");
    gotoxy(2,6);printf("Reader Number:%s",p->data.num);
    gotoxy(2,7);printf("Reader Name:%s",p->data.name);
    gotoxy(2,8);printf("Reader Sex:%s",p->data.sex);
    gotoxy(2,9);printf("Reader Age:%d",p->data.age);
    gotoxy(2,10);printf("Reader Tele:%s",p->data.tele);
    gotoxy(2,11);printf("Total Number of Reader Borrowed:%d",p->data.total_num);
    gotoxy(2,12);printf("---------------------------------------------");
    gotoxy(2,14);printf("press any key to return");
    getchar(); getchar();
  }
  else
   {gotoxy(2,5);printf("=====>Not find this Reader!\n");getchar();getchar();}
  }
 else
   {gotoxy(2,5);;printf("*****Error:input has wrong! press any key to continue******");
    getchar();getchar();}
}

/*删除读者记录：先找到保存该读者记录的节点，然后删除该节点*/
void DelReader(Reader_Link l)
 {
  int sel;
  Reader_Node *p,*r;
  char findmess[20];
  if(!l->next)
  { clrscr();
    gotoxy(2,2);
    printf("\n=====>No Reader record!\n");
    getchar();
    return;
   }
  clrscr();
  gotoxy(2,2);
  printf("=====>1 Delete by Reader number       =====>2 Delete by Reader name");
  gotoxy(2,3);
  printf("please choice[1,2]:");
  scanf("%d",&sel);
  if(sel==1)
  { gotoxy(2,4);
    stringinput(findmess,10,"input the existing Reader number:");
    p=LocateReader(l,findmess,"num");
    if(p)  /*p!=NULL*/
    {
     r=l;
     while(r->next!=p)
     r=r->next;
     r->next=p->next;/*将p所指节点从链表中去除*/
     free(p); /*释放内存空间*/
     gotoxy(2,6);
     printf("=====>delete success!");
     getchar();getchar();
     saveflag=1;
   }
  else
   {gotoxy(2,6);printf("=====>Not find this Reader!\n");getchar();getchar();}
 }
 else if(sel==2) /*先按读者名查询到该记录所在的节点*/
  {
  stringinput(findmess,15,"input the existing Reader name:");
  p=LocateReader(l,findmess,"name");
  if(p)
  {
   r=l;
   while(r->next!=p)
    r=r->next;
   r->next=p->next;
   free(p);
   gotoxy(2,6);
   printf("=====>delete success!\n");
   getchar();getchar();
   saveflag=1;
  }
  else
   {gotoxy(2,6);printf("=====>Not find this Reader!\n");getchar();getchar();}
 }
 else
  {gotoxy(2,6);printf("*****Error:input has wrong! press any key to continue******");
    getchar();getchar();}
}

/*修改读者记录。先按输入的读者编号查询到该记录，然后提示用户修改读者编号之外的用户输入字段*/
void ModifyReader(Reader_Link l)
{
  Reader_Node *p;
  char findmess[20];
  float temp;
  if(!l->next)
  { clrscr();
    gotoxy(2,1);
    printf("\n=====>No Reader record!\n");
    getchar();
    return;
  }
  clrscr();
  gotoxy(2,1);
  stringinput(findmess,10,"input the existing Reader Name:"); /*输入并检验该读者编号*/
  p=LocateReader(l,findmess,"name"); /*查询到该节点*/
  if(p) /*若p!=NULL,表明已经找到该节点*/
  { gotoxy(2,2);printf("---------------------------------------------");
    gotoxy(2,3);printf("Reader Number:%s",p->data.num);
    gotoxy(2,4);printf("Reader Name:%s",p->data.name);
    gotoxy(2,5);printf("Reader Sex:%s",p->data.sex);
    gotoxy(2,6);printf("Reader Age:%d",p->data.age);
    gotoxy(2,7);printf("Reader Tele:%s",p->data.tele);
    gotoxy(2,8);printf("Total Number of Reader Borrowed:%d",p->data.total_num);
    gotoxy(2,9);printf("---------------------------------------------");
    gotoxy(2,10);printf("please modify Reader recorder:");
    gotoxy(2,11);stringinput(p->data.name,15,"Reader Name:");
    gotoxy(2,12);stringinput(p->data.sex,4,"Reader Sex(M or F)):");
    gotoxy(2,13);printf("Reader Age:");scanf("%f",&temp); p->data.age=temp;
    gotoxy(2,14);stringinput(p->data.tele,15,"Reader Telephone:");
    gotoxy(2,15);printf("---------------------------------------------");
    gotoxy(2,16);printf("=====>modify success!"); getchar();getchar();
    saveflag=1;
   }
  else
  {gotoxy(2,4);printf("=====>Not find this Reader!\n");getchar();getchar();}
 }


/*统计读者的总数量，男性读者和女性读者的数量，统计目前借书数最多的读者名*/
void CountReader(Reader_Link l)
 {
  Reader_Node *r=l->next;
  int countc=0,countm=0,countf=0,counte=0;
  char Readername[15];
  if(!r)
  { clrscr();
    gotoxy(2,1);
    printf("=====>Not Reader record!");
    getchar();
    return ;
  }
 counte=r->data.total_num;
 strcpy(Readername,r->data.name);
 while(r)
  {
   countc++; /*统计读者数量*/
   if (strcmp(r->data.sex,"M")==0 || strcmp(r->data.sex,"m")==0 )
      countm++;   /*统计男性读者数*/
   else
      countf++;  /*统计女性读者数*/
   /*保存借出次数最多的读者名*/
   if(r->data.total_num>counte) {counte=r->data.total_num;strcpy(Readername,r->data.name); }
   r=r->next;
 }
 clrscr();
 gotoxy(2,3);
 printf("------------------------------the TongJi result--------------------------------");
 gotoxy(2,4);
 printf("Total number of Readers:%d",countc);
 gotoxy(2,5);
 printf("Total number of Male:%d, Female:%d",countm,countf);
 gotoxy(2,6);
 printf("Reader name of maximum borrowed number:%s",Readername);
 gotoxy(2,7);
 printf("-------------------------------------------------------------------------------");
 getchar();getchar();
}

/*利用直接选择排序法实现按读者编号字段的升序排序，从低到高*/
void SortReader(Reader_Link l)
{
  Reader_Link lll; /*临时指针*/
  Reader_Node *p,*q,*r,*s,*h1;  /*临时指针*/
  int x,y; /*保存当前光标所在位置的坐标值*/
  int i=0;
  if(l->next==NULL)
  { clrscr();
    gotoxy(2,1);
    printf("=====>Not Reader record!");
    getchar();
    return ;
  }

  h1=p=(Reader_Node*)malloc(sizeof(Reader_Node)); /*用于创建新的头节点*/
  if(!p)
   {
      gotoxy(2,1);
      printf("allocate memory failure "); /*如没有申请到，打印提示信息*/
      return ;             /*返回主界面*/
   }
    /************显示排序前的所有记录*********/
    clrscr();
    gotoxy(2,1);
    printf(HEADER2);
    gotoxy(2,1);
    lll=l->next;
    x=wherex();y=wherey();i=0;
    while(lll!=NULL)  /*当p不为空时，进行下列操作*/
    {
      i++;
      gotoxy(x,i+y);   /*换行*/
      printf(FORMAT2,DATA2); /*见头部的宏定义*/
      lll=lll->next;   /*指针后移*/
    }
    getchar();getchar();
    gotoxy(2,i+2);
    printf("=====>sort..............");
   /********************排序**************************/
   p->next=l->next; /*l所指节点为不存有任何记录的节点，下一个节点才有读者记录*/
   while(p->next!=NULL) /*外层循环决定待排序位置*/
   {
    q=p->next;
    r=p;
     while(q->next!=NULL)/*内循环找到当前关键字最小节点*/
      {
      if (strcmp(q->next->data.num,r->next->data.num)<0) r=q; /*r为记录当前最小节点的前驱节点的指针变量*/
         q=q->next; /*移至下一个节点*/
      }

     if (r!=p)
     /*表示原来的第1个节点不是关键字最小的节点，改变指针的关系，
     将关键字最小的节点与本轮循环的首节点进行位置互换*/
      {
       s=r->next; /*s指向最小节点*/
       r->next=s->next; /*r的指针域指向最小节点的下一个节点*/
       s->next=p->next; /*s的指针域指向当前p指针所指的下一个节点*/
       p->next=s; /*p的指针域指向本次循环结束后关键字最小的节点*/
       }
      p=p->next;/*移至下一个节点，过行外循环控制*/
      }/*外层while*/
     /*********************************************/
     l->next=h1->next;/*将排序好的链表首节点地址赋给原来链表的指针域*/

     /*显示排序后的所有读者记录*/
    lll=l->next;
    gotoxy(x,y+i+1);
    x=wherex();y=wherey();i=0;
    while(lll!=NULL)  /*当p不为空时，进行下列操作*/
    {
      i++;
      gotoxy(x,i+y);
      printf(FORMAT2,DATA2);
      lll=lll->next;   /*指针后移*/
    }
      free(h1);
      saveflag=1;
      gotoxy(2,wherey()+2);
      printf("=====>sort complete!\n");
      getchar();getchar();
      return;
   }


  /*数据存盘,若用户对数据有修改，在退出系统时，系统会自动存盘*/
 void SaveReader(Reader_Link l)
 {
  FILE* fp;
  Reader_Node *p;
  int count=0;
  fp=fopen("c:\\reader","wb");/*以只写方式打开二进制文件*/
  if(fp==NULL) /*打开文件失败*/
  {
  clrscr();
  gotoxy(2,2);
  printf("=====>open file error!\n");
  getchar();
  return ;
  }
  p=l->next;
  while(p)
  {
   if(fwrite(p,sizeof(Reader_Node),1,fp)==1)/*每次写一条记录或一个节点信息至文件*/
  { 
    p=p->next;
    count++;
   }
  else
  {
    break;
   }
  }
 if(count>0)
  {
  gotoxy(4,9);
  printf("=====>save reader,total saved's record number is:%d\n",count);
  gotoxy(4,14);
  printf("*****************Thank you for your useness!*******************");
  getchar();
  saveflag=0;
 }
 else
 {clrscr();
  gotoxy(2,3);
  printf("the current link is empty,no record is saved!\n");
  getchar();
 }
 fclose(fp); /*关闭reader文件*/
 }

 /**************************借书*********************************/

  void BorrowBook(Book_Link l,Reader_Link ll)
  {
    Book_Node *p1;   /*定义图书记录指针变量*/
    Reader_Node *p2;   /*定义读者记录指针变量*/
    char readernum[15],bookname[15];
    int flag=0;
    p1=l->next;
    p2=ll->next;
    clrscr();
    gotoxy(2,2);stringinput(readernum,15,"Reader Number:");
    while(p2) /*查询该读者编号是否已经存在，若不存在则不允许执行借书操作*/
     {
      if(strcmp(p2->data.num,readernum)==0)
      {
       flag=1;
       break;
       }
       p2=p2->next;
     }
     if (flag==0)
      {
       gotoxy(2,3);
       printf("The Reader Number %s is not existing!",readernum);
       getchar();getchar();
       return;
      }
     if (p2->data.total_num>=19) /*每人共限借20本*/
      {
       gotoxy(2,3);
       printf("The number of reader allowed borrowed book can't be more than 20!",readernum);
       getchar();getchar();
       return;
      }

      gotoxy(2,3);stringinput(bookname,15,"Book Name:");
      while(p1) /*查询该图书编号是否存在且该图书是否为可借状态，若任意条件不成立则不允许执行借书操作*/
      {
      if(strcmp(p1->data.name,bookname)==0)
      {
         if(p1->data.borrow_flag==0) /*0表示未借出，1表示已借出*/
         {
           p1->data.borrow_flag=1; /*标记此图书已借出*/
           strcpy(p1->data.reader,readernum); /*借阅人编号*/
           p1->data.total_num++;  /*图书被借次数*/
           p2->data.total_num++;  /*读者目前已借图书册数增1*/
           gotoxy(2,4);
           printf("The book %s is borrowed by %s (Num:%s) Successfully!",bookname, p2->data.name, p2->data.num);
           getchar();getchar();
           return;
         }
         else
         {
          gotoxy(2,3);
          printf("The book %s can't be borrowed currently!",bookname);
          getchar();getchar();
          return;
          }
        }
       else
        { p1=p1->next;}
      }

    gotoxy(2,3);
    printf("The book %s is not existing !",bookname);
    getchar();getchar();
    return;
    }

  /**************************还书*********************************/

  void ReturnBook(Book_Link l,Reader_Link ll)
  {
    Book_Node *p1;   /*定义图书记录指针变量*/
    Reader_Node *p2;   /*定义读者记录指针变量*/
    char readernum[15],bookname[15];
    int flag=0;
    p1=l->next;
    p2=ll->next;
    clrscr();
    gotoxy(2,2);stringinput(readernum,15,"Reader Number:");
    while(p2) /*查询该读者编号是否已经存在，若不存在则不允许执行还书操作*/
     {
      if(strcmp(p2->data.num,readernum)==0)
      {
       flag=1;
       break;
       }
       p2=p2->next;
     }
     if (flag==0)
      {
       gotoxy(2,3);
       printf("The Reader Number %s is not existing!",readernum);
       getchar();getchar();
       return;
      }
      gotoxy(2,3);stringinput(bookname,15,"Book Name:");
      while(p1) /*查询该图书编号是否存在且该图书是否为已借状态，同时读者编号一致，若任意条件不满足则不允许执行还书操作*/
      {
      if(strcmp(p1->data.name,bookname)==0)
      {
         if(p1->data.borrow_flag==1 && strcmp(p1->data.reader,readernum)==0) /*0表示未借出，1表示已借出*/
         {
           p1->data.borrow_flag=0; /*标记此图书已还*/
           strcpy(p1->data.reader,""); /*在book文件中借阅人编号置空*/
           p2->data.total_num--;  /*读者目前已借图书册数减1*/
           gotoxy(2,4);
           printf("The book %s is returned by %s (Num:%s) Successfully!",bookname, p2->data.name, p2->data.num);
           getchar();getchar();
           return;
         }
         else
         {
          gotoxy(2,3);
          printf("The book %s is not borrroed,or the number of reader is different!",bookname);
          getchar();getchar();
          return;
          }
        }
       else
        { p1=p1->next;}
      }

    gotoxy(2,3);
    printf("The book %s is not existing !",bookname);
    getchar();getchar();
    return;
    }

 /***********************主控函数***************************/
  void main()
 {
  Book_Link l;      /*定义图书链表*/
  Reader_Link ll;   /*定义读者链表*/
  FILE *fp1,*fp2;    /*fp为指向图书的文件指针，fp2为指向读者的文件指针*/
  int i;
  char ch;      /*保存(y,Y,n,N)*/
  int count1=0,count2=0;  /*分别保存图书文件或读者文件中的记录条数*/
  Book_Node *p,*r;   /*定义图书记录指针变量*/
  Reader_Node *p2,*r2;   /*定义读者记录指针变量*/

  int A;  /*保存用户的按键值*/
  char a; /*保存用户的按键值*/
  int B;  /*保存用户选择的子菜单项*/
  drawmain();     /*显示主窗口*/
  window(2,2,79,23); /*定义活动文本模式窗口*/
  textbackground(9);
  for(i=0;i<24;i++) insline(); /*输出空行*/
  window(3,3,78,23);
  textcolor(10);

  /************打开图书文件book,将其调入链表中存储**************************/
  l=(Book_Node*)malloc(sizeof(Book_Node));
  if(!l)
   {  clrscr();gotoxy(2,3);
      printf("\n allocate memory failure "); /*如没有申请到，打印提示信息*/
      return ;             /*返回主界面*/
   }
  l->next=NULL;
  r=l;
  fp1=fopen("C:\\book","ab+"); /*以追加方式打开一个二进制文件，可读可写，若此文件不存在，会创建此文件*/
  if(fp1==NULL)
  { clrscr();gotoxy(2,3);
    printf("\n=====>can not open file!\n");
    exit(0);
  }
  while(!feof(fp1))
  {
   p=(Book_Node*)malloc(sizeof(Book_Node));
   if(!p)
    { clrscr();gotoxy(2,3);
      printf(" memory malloc failure!\n");    /*没有申请成功*/
      exit(0);       /*退出*/
    }
   if(fread(p,sizeof(Book_Node),1,fp1)==1) /*一次从文件中读取一条图书记录*/
   {
    p->next=NULL;
    r->next=p;
    r=p;                            /*r指针向后移一个位置*/
    count1++;
   }
  }
  fclose(fp1); /*关闭文件*/
  printf("\n=====>open file sucess,the total records number is : %d.\n",count1);
  p=r;
  /***************************************************************************/
  /************打开图书文件reader,将其调入链表中存储**************************/
  ll=(Reader_Node*)malloc(sizeof(Reader_Node));
  if(!ll)
   {
      printf("\n allocate memory failure "); /*如没有申请到，打印提示信息*/
      return ;             /*返回主界面*/
   }
  ll->next=NULL;
  r2=ll;
  fp2=fopen("C:\\reader","ab+"); /*以追加方式打开一个二进制文件，可读可写，若此文件不存在，会创建此文件*/
  if(fp2==NULL)
  {
    printf("\n=====>can not open file!\n");
    exit(0);
  }
  while(!feof(fp2))
  {
   p2=(Reader_Node*)malloc(sizeof(Reader_Node));
   if(!p2)
    {
      printf(" memory malloc failure!\n");    /*没有申请成功*/
      exit(0);       /*退出*/
    }
   if(fread(p2,sizeof(Reader_Node),1,fp2)==1) /*一次从文件中读取一条图书记录*/
   {
    p2->next=NULL;
    r2->next=p2;
    r2=p2;                            /*r指针向后移一个位置*/
    count2++;
   }
  }
  fclose(fp2); /*关闭文件*/
  printf("\n=====>open file sucess,the total records number is : %d.\n",count2);
  p2=r2;
  /***************************************************************************/

while(1)
{
   while(bioskey(1)==0) continue; /*等待用户按键*/
    a=A=bioskey(0); /*返回用户的按键值*/
    if((A==F1)||(A==F2)||(A==F3))
      {   B=menuctrl(A);
              switch(B)
                {
                    case ADD_BOOK:AddBook(l);break;   /*增加记录*/
                    case QUERY_BOOK:QueryBook(l);break;
                    case MODIFY_BOOK:ModifyBook(l);break;
                    case DEL_BOOK:DelBook(l);break;
                    case SORT_BOOK:SortBook(l);break;
                    case COUNT_BOOK:CountBook(l);break;
                    case ADD_READER: AddReader(ll);break;
                    case QUERY_READER:QueryReader(ll);break;
                    case MODIFY_READER:ModifyReader(ll);break;
                    case DEL_READER:DelReader(ll);break;
                    case SORT_READER:SortReader(ll);break;
                    case COUNT_READER:CountReader(ll);break;

                    case BORROW_BOOK:BorrowBook(l,ll);break;
                    case RETURN_BOOK:ReturnBook(l,ll);break;
                    case EXIT:
                     { clrscr();gotoxy(3,3);
                       cprintf("\n=====>Are you really exit the Book Management System?(y/n):");
                       scanf("%c",&ch);
                       if(ch=='y'||ch=='Y') {SaveBook(l);SaveReader(ll);exit(0);}
                      }
                }

     }
     clrscr();

   }

 }
