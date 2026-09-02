#include "bios.h"     /*基本输入输出系统函数库*/
#include "math.h"     /*数学运算函数库*/
#include "stdio.h"   /*标准输入输出函数库*/
#include "stdlib.h"  /*标准函数库*/
#include "string.h"  /*字符串函数库*/
#include "conio.h"    /*屏幕操作函数库*/
#include "dos.h"      /*与日期获得有关*/
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
/*与Course菜单选项有关的宏定义*/
#define ADD_COURSE 100
#define QUERY_COURSE 101
#define MODIFY_COURSE 102
#define DEL_COURSE 103
#define SORT_COURSE 104
#define COUNT_COURSE 105
/*与Student菜单选项有关的宏定义*/
#define ADD_STUDENT 200
#define QUERY_STUDENT 201
#define MODIFY_STUDENT 202
#define DEL_STUDENT 203
#define SORT_STUDENT 204
#define COUNT_STUDENT 205
/*与E&E菜单选项有关的宏定义*/
#define ELECTIVE_COURSE 300
#define HELP            301
#define EXIT            302
/*与课程记录格式化输出有关的宏定义*/
#define HEADER1 "|  Number  |  Name  |Type|Period|        Student_Number        | \n"
#define FORMAT1  "|%-10s|%-8s|%4d|%-6d|%-30s|\n"
#define DATA1  lll->c_num,lll->c_name,lll->c_type,lll->c_period,lll->student_num
/*与学生记录格式输出有关的宏定义*/
#define HEADER2 "|   Number   |   Name   | Sex|Age |        Course_Number         | \n"
#define FORMAT2  "|%-12s|%-10s|%-4s|%4d|%-30s|\n"
#define DATA2  lll->data.s_num,lll->data.s_name,lll->data.s_sex,lll->data.s_age,lll->data.course_num

#define N 200      /*定义课程的最大数量，用户可自行设置*/
int saveflag=0;  /*是否需要存盘的全局标志变量*/
struct date sysTime; /*系统结构体，用于存储系统日期。该结构体有三个成员,即a_year,da_mon和da_day*/
int currentYear;  /*保存当前年份*/
int currentMonth; /*保存当前月份*/
int currentDay;  /*保存当前日期*/

/*定义与课程有关的数据结构*/
typedef struct course      /*标记为course*/
{
char   c_num[15];          /*课程编号*/
char   c_name[15];         /*课程名称*/
int    c_type;             /*保存课程的类型，其中1表示必修课，2表示选修课*/
int    c_period;           /*保存课程的学时*/
char   student_num[200];   /*保存当前已选该门课程的学生的学号*/
}COURSE;

/*定义与学生有关的数据结构*/
typedef struct student      /*标记为student*/
{
char s_num[15];      /*学生编号*/
char s_name[15];     /*学生姓名*/
char s_sex[4];       /*学生性别M或F,Male:男性，Female:女性*/
int  s_age;            /*学生年龄*/
char course_num[100]; /*保存当前学生已选课程的编号列表*/
};

/*定义每条学生记录的数据结构，标记为：student_node*/
typedef struct student_node
{
struct student data;          /*数据域*/
struct student_node *next;    /*指针域*/
}Student_Node,*Student_Link;   /*Student_Node为student_node类型的结构变量，*Student_Link为student_node类型的指针变量*/


void drawmain() /*画主窗口函数*/
 {
          int i,j;
          gotoxy(1,1);       /*在文本窗口中设置光标至(1,1)处*/
          textbackground(7); /*选择新的文本背景颜色,7为LIGHTGRAY淡灰色*/
          textcolor(4);      /*在文本模式中选择新的字符颜色4为RED红*/
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
          gotoxy(1,1);
          cprintf("%c",196); /*在窗口左上角，输出-*/
          gotoxy(1,24);
          cprintf("%c",196); /*在窗口左下角，输出-*/
          gotoxy(80,1);
          cprintf("%c",196); /*在窗口右上角，输出-*/
          gotoxy(80,24);
          cprintf("%c",196); /*在窗口右下角，输出-*/
          gotoxy(7,1);
          cprintf("%c  %c Course %c  %c",179,17,16,179);  /* | < > |*/
          gotoxy(27,1);
          cprintf("%c  %c Student %c  %c",179,17,16,179); /* | < > |*/
          gotoxy(47,1);
          cprintf("%c  %c E&E %c  %c",179,17,16,179); /* | < > |*/
          gotoxy(5,25); /*跳至窗口底端*/
          textcolor(4);
          cprintf(" Student Elective Course Management System");
          gotoxy(68,25);
          getdate(&sysTime);
          currentYear=sysTime.da_year;
          currentMonth=sysTime.da_mon;
          currentDay=sysTime.da_day ;
          cprintf("%4d-%d-%d",currentYear,currentMonth,currentDay);
 }


 void drawmenu(int m,int n) /*画菜单,m:第几项菜单，n：第m项的第n个子菜单*/
 {
       int i;
      if(m%3==0) /*画Course菜单项*/
     {
            window(8,2,19,9);
            textcolor(4);
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
            cprintf("%c  %c Course %c  %c",179,17,16,179);
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
     if(m%3==1) /*画Student菜单项*/
    {
            window(28,2,39,9);
            textcolor(4);
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
            cprintf("%c  %c Student %c  %c",179,17,16,179);
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
     if(m%3==2) /*画E&E菜单项3*/
    {
            window(48,2,59,8);
            textcolor(4);
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
            gotoxy(49,3);   cprintf(" Elective ");
            gotoxy(49,5);   cprintf(" Help   ");
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
            cprintf("%c  %c R&R %c  %c",179,17,16,179);
            switch(n%3)
            {
                    case 0:gotoxy(49,3);  cprintf(" Elective  "); break;
                    case 1:gotoxy(49,5);  cprintf(" Help    "); break;
                    case 2:gotoxy(49,7);  cprintf(" Exit      "); break;


            }
    }
 }


int menuctrl(int A) /*菜单控制*/
  {
        int x,y,i,B,value,flag=36,a,b;
        x=wherex();  y=wherey();
        if(A==F1) {  drawmenu(0,flag);   value=300;  } /*显示Course及其子菜单,并将光带显示在第一个子菜单上*/
        if(A==F2) {  drawmenu(1,flag);   value=301;  } /*显示Student及其子菜单,并将光带显示在第一个子菜单上*/
        if(A==F3) {  drawmenu(2,flag);   value=302;  } /*显示E&E及其子菜单,并将光带显示在第一个子菜单上*/

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
                            if(value%3==0)  b=6; /*Course下有6个子菜单项*/
                            if(value%3==1)  b=6; /*Student下有6个子菜单项*/
                            if(value%3==2)  b=3; /*R&R下有3个子菜单项*/
                            a=(value%3)*10+flag%b;/*a表示选择子菜单的编号*/
                            drawmain();
                            window(2,2,79,23);
                            textbackground(9);
                            for(i=0;i<24;i++)
                                   insline();
                            window(3,3,78,23);
                            textcolor(10);
                            gotoxy(x,y);
                            if(a==0)   return ADD_COURSE;
                            if(a==1)   return QUERY_COURSE;
                            if(a==2)   return MODIFY_COURSE;
                            if(a==3)   return DEL_COURSE;
                            if(a==4)   return SORT_COURSE;
                            if(a==5)   return COUNT_COURSE;

                            if(a==10)   return ADD_STUDENT;
                            if(a==11)   return QUERY_STUDENT;
                            if(a==12)   return MODIFY_STUDENT;
                            if(a==13)   return DEL_STUDENT;
                            if(a==14)   return SORT_STUDENT;
                            if(a==15)   return COUNT_STUDENT;

                            if(a==20)   return ELECTIVE_COURSE;
                            if(a==21)   return HELP;
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

/*******************************************************************
作用：用于定位数组中符合要求的记录，并返回保存该记录的数组元素下标值
参数：findmess[]保存要查找的具体内容; nameornum[]保存按什么在数组中查找;
********************************************************************/
int LocateCourse(COURSE tp[],int n,char findmess[],char nameornum[])
{
int i=0;
if(strcmp(nameornum,"num")==0) /*按课程编号查询*/
{
  while(i<n)
   {
   if(strcmp(tp[i].c_num,findmess)==0) /*若找到findmess课程编号的课程记录*/
    return i;
    i++;
    }
}
else if(strcmp(nameornum,"name")==0)  /*按课程名称查询课程*/
{
  while(i<n)
   {
   if(strcmp(tp[i].c_name,findmess)==0) /*若找到findmess的课程记录*/
    return i;
    i++;
    }
}
return -1; /*若未找到，返回一个整数-1*/
}

/*************************************************************
作用：用于定位学生链表中符合要求的节点，并返回指向该节点的指针
参数：findmess[]保存要查找的具体内容; nameornum[]保存按什么查找;
      在单链表l中查找;
**************************************************************/
 Student_Node* LocateStudent(Student_Link l,char findmess[],char nameornum[])
  {
  Student_Node *r;
  if(strcmp(nameornum,"num")==0) /*按学生学号查询*/
  {
  r=l->next;
  while(r)
  {
   if(strcmp(r->data.s_num,findmess)==0) /*若找到findmess值的学生编号*/
    return r;
    r=r->next;
  }
 }
 else if(strcmp(nameornum,"name")==0)  /*按学生姓名查询*/
  {
  r=l->next;
  while(r)
  {
   if(strcmp(r->data.s_name,findmess)==0)    /*若找到findmess值的学生姓名*/
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

 /*增加课程记录*/
int AddCourse(COURSE tp[],int n)
{
  char ch,flag=0,num[15];
  int i;
  clrscr();
  while(1) /*一次可输入多条记录，直至输入课程编号为00的记录结束添加操作*/
  {
    while(1) /*输入课程编号，保证该课程编号没有被使用，若输入课程编号为00，则退出添加记录操作*/
     {
      clrscr();
      gotoxy(2,2);
      stringinput(num,15,"Please input course number(press '00'return menu):"); /*格式化输入课程编号并检验*/
      flag=0;

      if(strcmp(num,"00")==0) /*输入为00，则退出添加操作，返回主界面*/
      {return n;}
     i=0;
     while(i<n) /*查询该课程编号是否已经存在，若存在则要求重新输入一个未被占用的课程编号*/
      {
      if(strcmp(tp[i].c_num,num)==0)
      {
       flag=1;
       break;
       }
       i++;
      }

  if(flag==1) /*提示用户是否重新输入*/

     {  gotoxy(2,3);
        getchar();
        printf("=====>The number %s is existing,please try again(y/n)?",num);
        scanf("%c",&ch);
        if(ch=='y'||ch=='Y')
         continue;
       else
         return n;
      }
     else
      {break;}
  }

  /*给课程记录赋值*/
  strcpy(tp[n].c_num,num); /*将字符串num拷贝到tp[n].c_num中*/
  gotoxy(2,3);
  printf("Course name:");
  scanf("%s",tp[n].c_name);
  gotoxy(2,4);
  printf("Course type(1,2):");
  scanf("%d",&tp[n].c_type);
  gotoxy(2,5);
  printf("Course period(1-4):");
  scanf("%d",&tp[n].c_period);
  strcpy(tp[n].student_num,"");
  saveflag=1; /*标记有新的修改，需要存盘*/
  n++;
  gotoxy(2,6);printf(">>>>press any key to start next record!");
  getchar(); getchar();
  }
  return n;
}

void QueryCourse(COURSE tp[],int n) /*按课程编号或课程名称查询*/
{
  int select; /*1:按课程编号查，2：按课程名称查，其他：返回主界面（菜单）*/
  char searchinput[20]; /*保存用户输入的查询内容*/
  int p=0;
  if(n<=0) /*若课程记录为空*/
  {
   clrscr();
   gotoxy(2,2);
   printf("\n=====>No Course Record!\n");
   getchar();getchar();
   return;
  }
  clrscr();
  gotoxy(2,2);
  cprintf("=====>1 Search by course number  =====>2 Search by course name");
  gotoxy(2,3);
  cprintf("please choice[1,2]:");
  scanf("%d",&select);
  if(select==1)   /*按课程编号查询*/
  {
   gotoxy(2,4);
   stringinput(searchinput,15,"input the existing course number:");
   p=LocateCourse(tp,n,searchinput,"num");/*在数组tp中查找编号为searchinput值的元素，并返回该数组元素的下标值*/
   if(p!=-1) /*若p!=-1*/
   {
    gotoxy(2,5);printf("---------------------------------------------");
    gotoxy(2,6);printf("Course Number:%s",tp[p].c_num);
    gotoxy(2,7);printf("Course Name:%s",tp[p].c_name);
    gotoxy(2,8);printf("Course Type:%d",tp[p].c_type);
    gotoxy(2,9);printf("Course Period:%d",tp[p].c_period);
    gotoxy(2,10);printf("Student Number:%s",tp[p].student_num);
    gotoxy(2,11);printf("---------------------------------------------");
    gotoxy(2,12);printf("press any key to return");
    getchar(); getchar();
   }
  else
   {gotoxy(2,5);printf("=====>Not find this course!\n");getchar();getchar();}
  }
 else if(select==2) /*按课程名称查询*/
  {
  gotoxy(2,4);
  stringinput(searchinput,15,"input the existing client name:");
  p=LocateCourse(tp,n,searchinput,"name");
  if(p!=-1)
  {
    gotoxy(2,5);printf("---------------------------------------------");
    gotoxy(2,6);printf("Course Number:%s",tp[p].c_num);
    gotoxy(2,7);printf("Course Name:%s",tp[p].c_name);
    gotoxy(2,8);printf("Course Type:%d",tp[p].c_type);
    gotoxy(2,9);printf("Course Period:%d",tp[p].c_period);
    gotoxy(2,10);printf("Student Number:%s",tp[p].student_num);
    gotoxy(2,11);printf("---------------------------------------------");
    gotoxy(2,12);printf("press any key to return");
    getchar(); getchar();
  }
  else
   {gotoxy(2,5);printf("=====>Not find this course registered by %s!",searchinput);getchar();getchar();}
  }
 else
   {gotoxy(2,5);;printf("*****Error:input has wrong! press any key to continue******");
    getchar();getchar();}
}

/*删除课程记录：先找到保存该课程记录的数组元素，然后删除该元素*/
int DelCourse(COURSE tp[],int n)
 {
  int i=0,p=0;
  char findmess[20];
  if(n<=0)
  { clrscr();
    gotoxy(2,2);
    printf("\n=====>No course record!\n");
    getchar();
    return 0;
   }
  clrscr();
  gotoxy(2,2);
  stringinput(findmess,10,"input the existing course number:");
  p=LocateCourse(tp,n,findmess,"num");
  if(p!=-1)
  {
    for(i=p+1;i<n;i++) /*删除此记录，后面记录向前移*/
    {
     strcpy(tp[i-1].c_num,tp[i].c_num);
     strcpy(tp[i-1].c_name,tp[i].c_name);
     tp[i-1].c_type=tp[i].c_type;
     tp[i-1].c_period=tp[i].c_period;
     strcpy(tp[i-1].student_num,tp[i].student_num);
    }
     gotoxy(2,6);
     printf("=====>delete success!");
     getchar();getchar();
     saveflag=1;
     n--;
    }
  else
   {gotoxy(2,6);printf("=====>Not find this course!\n");getchar();getchar();}
  return n;
 }


/*修改课程记录。先按输入的课程编号查询到该记录，然后提示用户修改课程编号之外的用户输入字段*/
void ModifyCourse(COURSE tp[],int n)
{
  int p=0;
  char findmess[20];
  if(n<=0)
  { clrscr();
    gotoxy(2,1);
    printf("\n=====>No course record!\n");
    getchar();
    return;
  }
  clrscr();
  gotoxy(2,1);
  stringinput(findmess,10,"input the existing course number:"); /*输入并检验该课程编号*/
  p=LocateCourse(tp,n,findmess,"num"); /*查询到该节点*/
  if(p!=-1) /*表明已经找到该记录*/
  { gotoxy(2,2);printf("---------------------------------------------");
    gotoxy(2,3);printf("Course Number:%s",tp[p].c_num);
    gotoxy(2,4);printf("Course Name:%s",tp[p].c_name);
    gotoxy(2,5);printf("Course Type:%d",tp[p].c_type);
    gotoxy(2,6);printf("Course Period:%d",tp[p].c_period);
    gotoxy(2,7);printf("Student Number:%s",tp[p].student_num);
    gotoxy(2,8);printf("---------------------------------------------");
    gotoxy(2,9);printf("please modify course recorder:");
    gotoxy(2,10);printf(" Course Number:%s",tp[p].c_num);
    gotoxy(2,11);printf(" Course Name:");scanf("%s",tp[p].c_name);
    gotoxy(2,12);printf(" Course Type(1,2):");scanf("%d",&tp[p].c_type);
    gotoxy(2,13);printf(" Course Period(1-4):");scanf("%d",&tp[p].c_period);
    gotoxy(2,14);printf("---------------------------------------------");
    gotoxy(2,15);printf("=====>modify success!"); getchar();getchar();
    saveflag=1;
   }
  else
  {
   gotoxy(2,4);printf("=====>Not find this course!\n");getchar();getchar();
  }
}



/*统计总课程数,必修课门数及选修课门数*/
void CountCourse(COURSE tp[],int n)
 {
  int i=0,b1=0,x1=0;
  if(n<=0)  /*若没有课程记录*/
  { clrscr();
    gotoxy(2,1);
    printf("=====>Not course record!");
    getchar();
    return ;
  }
  i=0;
  while(i<n) /*统计必修课与选修课门数*/
  {
   if(tp[i].c_type==1)  {b1++;i=i+1;continue;}   /*必修课*/
   else
   {x1++;i=i+1;continue;}    /*必修课*/
  }
 clrscr();
 gotoxy(2,3);
 printf("--------------------------the TongJi Result of Course-------------------------");
 gotoxy(2,4);
 printf("Total number of courses:%d",n);
 gotoxy(2,5);
 printf("Total total number of BiXiu:%d",b1);
 gotoxy(2,6);
 printf("Total total number of XuanXiu:%d",x1);
 gotoxy(2,7);
 printf("----------------------------------------------------------------------------");
 getchar();getchar();
}

/*系统利用冒泡排序法实现数组的按课程编号字段的降序排序，从小到大*/
void SortCourse(COURSE tp[],int n)
{

   int i=0,j=0,k=0,flag=0;
   int x,y; /*保存当前光标所在位置的坐标值*/
   COURSE newinfo;
   COURSE* lll;
   clrscr();
   if(n<=0)
   { clrscr();
     gotoxy(2,1);
     printf("=====>Not course record!");
     getchar();getchar();
     return;
   }
   /*显示排序前的所有记录*/
   textcolor(10);textbackground(9);
   gotoxy(2,1);
   printf(HEADER1);/*输出表格头部*/
   gotoxy(2,1);
   x=wherex();y=wherey();k=0;
   i=0;
   while(i<n)    /*逐条输出数组中存储的课程信息*/
   {
    k++;
    gotoxy(x,k+y);   /*换行*/
    lll=&tp[i];
    printf(FORMAT1,DATA1); /*见头部的宏定义*/
    i++;
   }
   getchar();getchar();
   gotoxy(2,k+2);
   printf("=====>sort..............");
  /******排序****************/
  for(i=0;i<n;i++)
   {
    flag=0;
    for(j=0;j<n-1;j++)
    if((tp[j].c_num>tp[j+1].c_num))
     { flag=1;
        /*利用结构变量newinfo实现数组元素的交换*/
       strcpy(newinfo.c_num,tp[j].c_num);
       strcpy(newinfo.c_name,tp[j].c_name);
       newinfo.c_type=tp[j].c_type;
       newinfo.c_period=tp[j].c_period;
       strcpy(newinfo.student_num,tp[j].student_num);

       strcpy(tp[j].c_num,tp[j+1].c_num);
       strcpy(tp[j].c_name,tp[j+1].c_name);
       tp[j].c_type=tp[j+1].c_type;
       tp[j].c_period=tp[j+1].c_period;
       strcpy(tp[j].student_num,tp[j+1].student_num);


       strcpy(tp[j+1].c_num,newinfo.c_num);
       strcpy(tp[j+1].c_name,newinfo.c_name);
       tp[j+1].c_type=newinfo.c_type;
       tp[j+1].c_period=newinfo.c_period;
       strcpy(tp[j+1].student_num,newinfo.student_num);
      }
      if(flag==0) break;/*若标记flag=0，意味着没有交换了，排序已经完成*/
     }

      /*显示排序后的所有记录*/
      gotoxy(x,y+k+1);
      x=wherex();y=wherey();k=0;
       i=0;
      while(i<n)    /*逐条输出数组中存储的职工信息*/
      {
       k++;
       gotoxy(x,k+y);   /*换行*/
       lll=&tp[i];
       printf(FORMAT1,DATA1); /*见头部的宏定义*/
       i++;
      }
      saveflag=1;
      gotoxy(2,wherey()+2);
      printf("=====>sort complete!\n");
      getchar();getchar();
      return;
   }

  /*数据存盘,若用户对数据有修改，在退出系统时，系统会自动存盘*/
 void SaveCourse(COURSE tp[],int n)
 {
  FILE* fp;
  int i=0;
  int count=0;
  fp=fopen("c:\\course","wt");/*以只写方式打开文本文件*/
  if(fp==NULL) /*打开文件失败*/
  {
  clrscr();
  gotoxy(2,2);
  printf("=====>open file error!\n");
  getchar();
  return ;
  }
  i=0;
  while(i<n)
  {
    if(fwrite(&tp[i],sizeof(COURSE),1,fp)==1)/*每次写一条记录或一个节点信息至文件*/
     {
      count++;
      i++;
      continue;
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
  printf("=====>save course,total saved's record number is:%d\n",count);
  getchar();
  saveflag=0;
 }
 else
 {clrscr();
  gotoxy(2,3);
  printf("the current is empty,no record is saved!\n");
  getchar();
 }
 fclose(fp); /*关闭course文件*/
 }


 /***************与学生相关的函数*******************/
 /*增加学生记录*/
void AddStudent(Student_Link l)
{
  Student_Node *p,*r,*s;  /*实现添加操作的临时的结构体指针变量*/
  char ch,flag=0,num[10];
  int temp;
  r=l;
  s=l->next;
  clrscr();
  while(r->next!=NULL)
  r=r->next; /*将指针移至于链表最末尾，准备添加记录*/
  while(1) /*一次可输入多条记录，直至输入学生编号为00的记录结点添加操作*/
  {
    while(1) /*输入学生编号，保证该学生编号没有被使用，若输入学生编号为00，则退出添加记录操作*/
     {
      clrscr();
      gotoxy(2,2);
      stringinput(num,12,"Input Student Number(press '00'return menu):"); /*格式化输入学生编号并检验*/
      flag=0;

      if(strcmp(num,"00")==0) /*输入为00，则退出添加操作，返回主界面*/
      {return;}
      s=l->next;
      while(s) /*查询该学生编号是否已经存在，若存在则要求重新输入一个未被占用的学生编号*/
      {
      if(strcmp(s->data.s_num,num)==0)
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
  
  p=(Student_Node *)malloc(sizeof(Student_Node)); /*申请内存空间*/
  if(!p)
   {
      printf("\n allocate memory failure "); /*如没有申请到，打印提示信息*/
      return ;             /*返回主界面*/
   }
  /*给学生记录赋值*/
  strcpy(p->data.s_num,num); /*将字符串num拷贝到p->data.s_num中*/
  gotoxy(2,3);stringinput(p->data.s_name,15,"Student Name:");
  gotoxy(2,4);stringinput(p->data.s_sex,4,"Student Sex(M or F)):");
  gotoxy(2,5);printf("Student Age:");scanf("%d",&temp); p->data.s_age=temp;
  strcpy(p->data.course_num,"");
  gotoxy(2,6);printf(">>>>press any key to start next record!");
  getchar(); getchar();
  /********************/
  p->next=NULL; /*表明这是链表的尾部结点*/
  r->next=p;  /*将新建的结点加入链表尾部中*/
  r=p;
  saveflag=1;
  }
     return ;
}

void QueryStudent(Student_Link l) /*按学生编号或学生姓名查询*/
{
  int select; /*1:按学生学号查，2：按学生姓名查，其他：返回主界面（菜单）*/
  char searchinput[20]; /*保存用户输入的查询内容*/
  Student_Node *p;
  if(!l->next) /*若链表为空*/
  {
   clrscr();
   gotoxy(2,2);
   printf("\n=====>No Student Record!\n");
   getchar();getchar();
   return;
  }
  clrscr();
  gotoxy(2,2);
  cprintf("=====>1 Search by Student number  =====>2 Search by Student name");
  gotoxy(2,3);
  cprintf("please choice[1,2]:");
  scanf("%d",&select);
  if(select==1)   /*按学生编号查询*/
  {
   gotoxy(2,4);
   stringinput(searchinput,15,"input the existing Student number:");
   p=LocateStudent(l,searchinput,"num");/*在l中查找学生编号为searchinput值的节点，并返回节点的指针*/
   if(p) /*若p!=NULL*/
   {gotoxy(2,5);printf("---------------------------------------------");
    gotoxy(2,6);printf("Student Number:%s",p->data.s_num);
    gotoxy(2,7);printf("Student Name:%s",p->data.s_name);
    gotoxy(2,8);printf("Student Sex:%s",p->data.s_sex);
    gotoxy(2,9);printf("Student Age:%d",p->data.s_age);
    gotoxy(2,10);printf("Course Number:%s",p->data.course_num);
    gotoxy(2,11);printf("---------------------------------------------");
    gotoxy(2,12);printf("press any key to return");
    getchar(); getchar();
   }
  else
   {gotoxy(2,5);printf("=====>Not find this Student!\n");getchar();getchar();}
  }
 else if(select==2) /*按学生姓名查询*/
  {
  gotoxy(2,4);
  stringinput(searchinput,15,"input the existing Student name:");
  p=LocateStudent(l,searchinput,"name");
  if(p)/*若p!=NULL*/
  { gotoxy(2,5);printf("---------------------------------------------");
    gotoxy(2,6);printf("Student Number:%s",p->data.s_num);
    gotoxy(2,7);printf("Student Name:%s",p->data.s_name);
    gotoxy(2,8);printf("Student Sex:%s",p->data.s_sex);
    gotoxy(2,9);printf("Student Age:%d",p->data.s_age);
    gotoxy(2,10);printf("Course Number:%s",p->data.course_num);
    gotoxy(2,11);printf("---------------------------------------------");
    gotoxy(2,12);printf("press any key to return");
    getchar(); getchar();
  }
  else
   {gotoxy(2,5);printf("=====>Not find this Student!\n");getchar();getchar();}
  }
 else
   {gotoxy(2,5);;printf("*****Error:input has wrong! press any key to continue******");
    getchar();getchar();}
}

/*删除学生记录：先找到保存该学生记录的节点，然后删除该节点*/
void DelStudent(Student_Link l)
 {
  int sel;
  Student_Node *p,*r;
  char findmess[20];
  if(!l->next)
  { clrscr();
    gotoxy(2,2);
    printf("\n=====>No Student record!\n");
    getchar();
    return;
   }
  clrscr();
  gotoxy(2,2);
  printf("=====>1 Delete by Student number       =====>2 Delete by Student name");
  gotoxy(2,3);
  printf("please choice[1,2]:");
  scanf("%d",&sel);
  if(sel==1)
  { gotoxy(2,4);
    stringinput(findmess,10,"input the existing Student number:");
    p=LocateStudent(l,findmess,"num");
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
   {gotoxy(2,6);printf("=====>Not find this Student!\n");getchar();getchar();}
 }
 else if(sel==2) /*先按学生姓名查询到该记录所在的节点*/
  {
  stringinput(findmess,15,"input the existing Student name:");
  p=LocateStudent(l,findmess,"name");
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
   {gotoxy(2,6);printf("=====>Not find this Student!\n");getchar();getchar();}
 }
 else
  {gotoxy(2,6);printf("*****Error:input has wrong! press any key to continue******");
    getchar();getchar();}
}

/*修改学生记录。先按输入的学生编号查询到该记录，然后提示用户修改学生编号之外的用户输入字段*/
void ModifyStudent(Student_Link l)
{
  Student_Node *p;
  char findmess[20];
  int temp;
  if(!l->next)
  { clrscr();
    gotoxy(2,1);
    printf("\n=====>No Student record!\n");
    getchar();
    return;
  }
  clrscr();
  gotoxy(2,1);
  stringinput(findmess,10,"input the existing Student Num:"); /*输入并检验该学生编号*/
  p=LocateStudent(l,findmess,"num"); /*查询到该节点*/
  if(p) /*若p!=NULL,表明已经找到该节点*/
  { gotoxy(2,2);printf("---------------------------------------------");
    gotoxy(2,3);printf("Student Number:%s",p->data.s_num);
    gotoxy(2,4);printf("Student Name:%s",p->data.s_name);
    gotoxy(2,5);printf("Student Sex:%s",p->data.s_sex);
    gotoxy(2,6);printf("Student Age:%d",p->data.s_age);
    gotoxy(2,7);printf("Course  Number:%s",p->data.course_num);
    gotoxy(2,9);printf("---------------------------------------------");
    gotoxy(2,10);printf("please modify Student recorder:");
    gotoxy(2,11);printf("Student Number:%s",p->data.s_num);
    gotoxy(2,12);stringinput(p->data.s_name,15,"Student Name:");
    gotoxy(2,13);stringinput(p->data.s_sex,4,"Student Sex(M or F)):");
    gotoxy(2,14);printf("Student Age:");scanf("%d",&temp); p->data.s_age=temp;
    gotoxy(2,15);printf("---------------------------------------------");
    gotoxy(2,16);printf("=====>modify success!"); getchar();getchar();
    saveflag=1;
   }
  else
  {gotoxy(2,4);printf("=====>Not find this Student!\n");getchar();getchar();}
 }


/*统计学生的总数量，男学生和女学生的数量*/
void CountStudent(Student_Link l)
 {
  Student_Node *r=l->next;
  int countc=0,countm=0,countf=0;
  char Studentname[15];
  if(!r)
  { clrscr();
    gotoxy(2,1);
    printf("=====>Not Student record!");
    getchar();
    return ;
  }
  while(r)
  {
   countc++; /*统计学生数量*/
   if (strcmp(r->data.s_sex,"M")==0 || strcmp(r->data.s_sex,"m")==0 )
      countm++;   /*统计男性学生数*/
   else
      countf++;  /*统计女性学生数*/
   r=r->next;
 }
 clrscr();
 gotoxy(2,3);
 printf("------------------------------the TongJi result--------------------------------");
 gotoxy(2,4);
 printf("Total number of Students:%d",countc);
 gotoxy(2,5);
 printf("Total number of Male:%d, Female:%d",countm,countf);
 gotoxy(2,6);
 printf("-------------------------------------------------------------------------------");
 getchar();getchar();
}

/*利用直接选择排序法实现按学生编号字段的升序排序，从小到大*/
void SortStudent(Student_Link l)
{
  Student_Link lll; /*临时指针*/
  Student_Node *p,*q,*r,*s,*h1;  /*临时指针*/
  int x,y; /*保存当前光标所在位置的坐标值*/
  int i=0;
  clrscr();
  if(l->next==NULL)
  { clrscr();
    gotoxy(2,1);
    printf("=====>Not Student record!");
    getchar();
    return ;
  }

  h1=p=(Student_Node*)malloc(sizeof(Student_Node)); /*用于创建新的头节点*/
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
   p->next=l->next; /*l所指节点为不存有任何记录的节点，下一个节点才有学生记录*/
   while(p->next!=NULL) /*外层循环决定待排序位置*/
   {
    q=p->next;
    r=p;
     while(q->next!=NULL)/*内循环找到当前关键字最小节点*/
      {
      if (strcmp(q->next->data.s_num,r->next->data.s_num)<0) r=q; /*r为记录当前最小节点的前驱节点的指针变量*/
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

     /*显示排序后的所有学生记录*/
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
 void SaveStudent(Student_Link l)
 {
  FILE* fp;
  Student_Node *p;
  int count=0;
  fp=fopen("c:\\student","wt");/*以只写方式打开二进制文件*/
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
   if(fwrite(p,sizeof(Student_Node),1,fp)==1)/*每次写一条记录或一个节点信息至文件*/
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
  printf("=====>save student,total saved's record number is:%d\n",count);
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
 fclose(fp); /*关闭student文件*/
 }

 /**************************选课********************************/

  void ElectiveCourse(COURSE tp[],int n,Student_Link ll)
  {
    Student_Node *p;   /*定义学生记录指针变量*/
    int p1; /*定位课程记录的下标*/
    char studentnum[15],coursenum[15],ch;
    /*输入学号，定位学生记录*/
    clrscr();
    gotoxy(2,1);
    stringinput(studentnum,15,"input the existing Student Number:"); /*输入并检验该学生编号*/
    p=LocateStudent(ll,studentnum,"num"); /*查询到该节点*/
    if(!p) /*若p==NULL,表明未找到该节点*/
    {
    clrscr();gotoxy(2,4);printf("=====>Not find this Student!\n");getchar();getchar();return;
    }
     /*输入课程号，定位课程记录*/
    gotoxy(2,5);
    stringinput(coursenum,15,"input the existing course Number:");
    p1=LocateCourse(tp,n,coursenum,"num");/*在数组tp中查找编号为coursenum值的元素，并返回该数组元素的下标值*/
    if(p1!=-1) /*若p1!=-1表示已经找到相应记录*/
    {
     if(!strstr(tp[p1].student_num,p->data.s_num))  /*检测该学生是否已经选了此课，若还没有则执行选课操作*/
     {
      strcat(tp[p1].student_num," ");
      strcat(tp[p1].student_num,p->data.s_num); /*以追加方式，在课程记录中添加已选课学生的学号*/
      strcat(p->data.course_num," ");
      strcat(p->data.course_num,tp[p1].c_num); /*以追加方式，在学生记录中添加已选课程的编号*/
      gotoxy(2,7);
      printf("Success!! Student Number:%s,Student Name:%s\n",p->data.s_num,p->data.s_name);
      gotoxy(2,8);
      printf(">>>>Course Number:%s,Course Name:%s\n",tp[p1].c_num,tp[p1].c_name);
      getchar();getchar();
      saveflag=1;
     }
     else /*若该学生已经选了该课*/
     {gotoxy(2,8);printf("=====>This student already selected this course!\n");getchar();getchar();return; }
     }
    else  /*若p1==-1表示没有找到相应记录*/
    {gotoxy(2,9);printf("=====>Not find this course!\n");getchar();getchar();return;}
    return;
  }

  /**************************帮助********************************/

  void Help()
  {
    clrscr();
    gotoxy(2,1);
    printf("Welcome using this system, Design:Qiwu,wu;E_mail:bookforc@163.com\n");
    getchar();getchar();
    return;
  }

  /*******************用户登录*******************************/
  void logging(int left,int top,int right,int bottom)
 {
  int i,ii,jj,chint;
  char name[12],pass[12],passtemp[12],ch;
  for(i=top+1;i<bottom;i++)
  { gotoxy(left,i);
    cprintf("%c",179); /*窗口内文本的输出函数，在窗口左边输出 | */
    gotoxy(right,i);
    cprintf("%c",179); /*窗口内文本的输出函数，在窗口右边输出 | */
   }
   for(i=left+1;i<right;i++)
   { gotoxy(i,top);
     cprintf("%c",196); /*窗口内文本的输出函数，在窗口上边输出 - */
     gotoxy(i,bottom);
    cprintf("%c",196); /*窗口内文本的输出函数，在窗口下边输出 - */
    }
   gotoxy(left,top);
   cprintf("%c",218);
   gotoxy(right,top);
   cprintf("%c",191);
   gotoxy(left,bottom);
   cprintf("%c",192);
   gotoxy(right,bottom);
   cprintf("%c",217);
   ii=0;
   textcolor(3);textbackground(7);
   gotoxy(left+12,top+3);
    cprintf("            ");
    gotoxy(left+12,top+5);
    cprintf("            ");
   while(ii<3)
   {
    gotoxy(left+3,top+3);
    cprintf("username");
    gotoxy(left+3,top+5);
    cprintf("password");
    gotoxy(left+12,top+3);
    scanf("%s",name);
    gotoxy(left+12,top+5);
    jj=0;strcpy(pass,"");strcpy(passtemp,"");
    while((chint=ch=getche())!=13)/*13为回车键的十进制值*/
    {if(chint==8) /*backspace*/
     {jj--;gotoxy(left+12+jj,top+5);
      printf("%c",' ');gotoxy(left+12+jj,top+5);
      strcpy(passtemp,pass);
      passtemp[strlen(pass)-1]='\0';/*删除一个字符*/
      strcpy(pass,passtemp);
      continue;
      }
     sprintf(passtemp,"%c",ch);
     strcat(pass,passtemp);
     gotoxy(left+12+jj,top+5);printf("%c",'*');
     jj++;
    }
   if(strcmp("user",name)==0 && strcmp(pass,"123")==0)   {clrscr();return;}
    else
   {gotoxy(left+12,top+3);
    cprintf("            ");
    gotoxy(left+12,top+5);
    cprintf("            ");
   }
   ii++;
  }
  clrscr();
  printf("the user or password is not right!");
  getchar();
  exit(0);
  }


 /***********************主函数***************************/
  void main()
  {
  COURSE GR[N];      /*定义课程结构数组*/
  Student_Link ll;   /*定义学生链表指针*/
  FILE *fp1,*fp2;    /*fp为指向课程的文件指针，fp2为指向学生的文件指针*/
  int i;
  char ch;      /*保存(y,Y,n,N)*/
  int count1=0,count2=0;  /*分别保存课程文件或学生文件中的记录条数*/
  Student_Node *p2,*r2;   /*定义学生记录指针变量*/

  int A;  /*保存用户的按键值*/
  char a; /*保存用户的按键值*/
  int B;  /*保存用户选择的子菜单项*/

  drawmain();     /*显示主窗口*/
  window(2,2,79,23); /*定义活动文本模式窗口*/
  textbackground(0); /*区分登录前后的系统背景色*/
  for(i=0;i<24;i++) insline(); /*输出空行*/
  window(3,3,78,23);
  textcolor(4);

  logging(20,5,50,15);/*用户登录*/

  drawmain();     /*显示主窗口*/
  window(2,2,79,23); /*定义活动文本模式窗口*/
  textbackground(9); /*区分登录前后的系统背景色*/
  for(i=0;i<24;i++) insline(); /*输出空行*/
  window(3,3,78,23);
  textcolor(10);
  /************打开课程文件course,将其调入数组中存储**************************/
  fp1=fopen("C:\\course","at+"); /*以追加方式打开一个文本文件，可读可写，若此文件不存在，会创建此文件*/
  if(fp1==NULL)
  { clrscr();gotoxy(2,3);
    printf("\n=====>can not open file!\n");
    exit(0);
  }
  while(!feof(fp1))
  {
   if(fread(&GR[count1],sizeof(COURSE),1,fp1)==1) /*一次从文件中读取一条课程记录*/
   {
     count1++;
   }
  }
  fclose(fp1); /*关闭文件*/
  /***************************************************************************/
  /************打开课程文件student,将其调入链表中存储**************************/
  ll=(Student_Node*)malloc(sizeof(Student_Node));
  if(!ll)
   {
      printf("\n allocate memory failure "); /*如没有申请到，打印提示信息*/
      return ;             /*返回主界面*/
   }
  ll->next=NULL;
  r2=ll;
  fp2=fopen("C:\\student","at+"); /*以追加方式打开一个文本文件，可读可写，若此文件不存在，会创建此文件*/
  if(fp2==NULL)
  {
    printf("\n=====>can not open file!\n");
    exit(0);
  }
  while(!feof(fp2))
  {
   p2=(Student_Node*)malloc(sizeof(Student_Node));
   if(!p2)
    {
      printf(" memory malloc failure!\n");    /*没有申请成功*/
      exit(0);       /*退出*/
    }
   if(fread(p2,sizeof(Student_Node),1,fp2)==1) /*一次从文件中读取一条课程记录*/
   {
    p2->next=NULL;
    r2->next=p2;
    r2=p2;                            /*r2指针向后移一个位置*/
    count2++;
   }
  }
  fclose(fp2); /*关闭文件*/
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
                    case ADD_COURSE:count1=AddCourse(GR,count1);break;   /*增加课程记录*/
                    case QUERY_COURSE:QueryCourse(GR,count1);break;  /*查询课程记录*/
                    case MODIFY_COURSE:ModifyCourse(GR,count1);break; /*修改课程记录*/
                    case DEL_COURSE:count1=DelCourse(GR,count1);break;   /*删除课程记录*/
                    case SORT_COURSE:SortCourse(GR,count1);break;  /*排序课程记录*/
                    case COUNT_COURSE:CountCourse(GR,count1);break;    /*统计课程记录*/

                    case ADD_STUDENT: AddStudent(ll);break;  /*增加学生记录*/
                    case QUERY_STUDENT:QueryStudent(ll);break;   /*查询学生记录*/
                    case MODIFY_STUDENT:ModifyStudent(ll);break;   /*修改学生记录*/
                    case DEL_STUDENT:DelStudent(ll);break;     /*删除学生记录*/
                    case SORT_STUDENT:SortStudent(ll);break;  /*排序学生记录*/
                    case COUNT_STUDENT:CountStudent(ll);break; /*统计学生记录*/

                    case ELECTIVE_COURSE:ElectiveCourse(GR,count1,ll);break;/*选课*/
                    case HELP:Help();break;  /*帮助*/
                    case EXIT:
                     { clrscr();gotoxy(3,3);
                       cprintf("\n=====>Are you really exit this  Course Management System?(y/n):");
                       scanf("%c",&ch);
                       if(ch=='y'||ch=='Y') {SaveCourse(GR,count1);SaveStudent(ll);exit(0);}
                      }
                }

     }
     clrscr();

   }

 }
