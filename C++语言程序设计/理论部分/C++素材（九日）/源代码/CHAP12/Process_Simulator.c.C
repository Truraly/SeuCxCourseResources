/**************************************************************/
#include "bios.h"     /*»ù±¾ÊäÈëÊä³öÏµÍ³º¯Êı¿â*/
#include "math.h"     /*ÊıÑ§ÔËËãº¯Êı¿â*/
#include "stdio.h"   /*±ê×¼ÊäÈëÊä³öº¯Êı¿â*/
#include "stdlib.h"  /*±ê×¼º¯Êı¿â*/
#include "string.h"  /*×Ö·û´®º¯Êı¿â*/
#include "dos.h"    /*dos½Ó¿Úº¯Êı*/
#include "conio.h"   /*ÆÁÄ»²Ù×÷º¯Êı*/
#define  RUN 1       /*ÔËĞĞ×´Ì¬*/
#define  READY 2     /*¾ÍĞ÷×´Ì¬*/
#define  WAIT  3     /*µÈ´ı×´Ì¬»ò×èÈû×´Ì¬*/
#define  FINISH  4   /*Íê³É×´Ì¬*/
#define  timeslice 1  /*Ê±¼äÆ¬´óĞ¡*/
#define  Ready_N 3    /*¾ÍĞ÷¶ÓÁĞÔÊĞíµÄ×î¶à½ø³ÌÊı*/
#define  Wait_N  10   /*µÈ´ı¶ÓÁĞ»ò×èÈû¶ÓÁĞÔÊĞíµÄ×î¶à½ø³ÌÊı*/
#define  N       Ready_N+Wait_N   /*ÏµÍ³ÔÊĞíµÄ×î¶à½ø³ÌÊı*/
/**************************************************************/
/*¶¨Òå½ø³Ì¿ØÖÆ¿é*/
typedef struct pcb
   { char name[10];    /*½ø³Ì±êÊ¶·û*/
     int  status;  /*½ø³Ì×´Ì¬*/
     int  priority;/*½ø³ÌÓÅÏÈÊı*/
     int  runtime; /*½ø³ÌÔÚCPUÖĞµÄÒÑÔËĞĞÊ±¼ä*/
     int  needtime;/*½ø³ÌĞèÒªÔËĞĞÊ±¼ä*/
   }PCB;

/*¶¨Òå½ø³Ì½Úµã*/
typedef struct queue_node{
                PCB data;
                struct queue_node *next;
         }Queue_Node;

Queue_Node *head;  /*¹«¹²¶ÓÁĞÖ¸Õë*/
Queue_Node *ready; /*¾ÍĞ÷¶ÓÁĞÖ¸Õë*/
Queue_Node *run;   /*ÔËĞĞ¶ÓÁĞÖ¸Õë*/
Queue_Node *wait;  /*µÈ´ı¶ÓÁĞÖ¸Õë*/
Queue_Node *finish;/*Íê³É¶ÓÁĞÖ¸Õë*/
/**************************************************************
  ½ø³Ì³·Ïú£º½«ÔËĞĞÍêµÄ½ø³ÌÌŞ³ı³ö¶ÓÁĞ£¬±£´æÔÚÍê³É¶ÓÁĞÖĞ¡£
**************************************************************/
void Destroy_Process(Queue_Node *run)
{
 Queue_Node *q,*p;
 Queue_Node *r,*f,*w;
 ready=head;
 q=ready->next;f=finish;r=ready;
 while(f->next!=NULL)
  f=f->next; /*½«Ö¸ÕëÒÆÖÁÓÚÍê³É¶ÓÁĞ×îÄ©Î²£¬×¼±¸Ìí¼Ó½ø³Ì*/
 while(q!=NULL)/*É¨Ãè½ø³Ì¶ÓÁĞ£¬ÕÒµ½Ö´ĞĞÍêÁËµÄ½ø³Ì*/
  {
   if(q->data.name==run->data.name)/*ÅĞ¶ÏÊÇ²»ÊÇÒÑÍê³ÉµÄ½ø³Ì*/
    {
     if(q->next!=NULL)
        r->next=q->next; /*¶ÓÁĞÖĞÉ¾³ıÒÑÍê³É½ø³Ì*/
     else
        r->next=NULL; /*¶ÓÁĞÖĞÉ¾³ıÒÑÍê³É½ø³Ì*/
     q->data.priority=0; /*ÓÅÏÈÊıÇå0*/
     q->data.status=FINISH; /*¸ü¸Ä½ø³Ì×´Ì¬*/
     q->next=NULL;
     f->next=q;/*½«ÒÑÍê³É½ø³Ì±£´æÖÁÍê³É¶ÓÁĞ¶ÓÎ²*/
     break;
     }
   r=q;
   q=q->next;
  }
 return;
}

/**************************************************************/
/*É¾³ıËùÓĞÍê³É½ø³ÌºÍ¶ÓÁĞÇå¿Õ*/
/**************************************************************/
void Delete_AllProcess()
{ Queue_Node *f;
  f=finish->next;
  while(f!=NULL) /*É¾³ıÍê³É¶ÓÁĞÖĞµÄ½ø³Ì*/
 { free(f);
   f=f->next;
  }
 ready->next=NULL;
 wait->next=NULL;
 finish->next=NULL;
 run->next=NULL;
 head->next=NULL;
 return;
}

/***************************************************
 ÏÔÊ¾µ±Ç°ËùÓĞ½ø³ÌµÄ×´Ì¬
 ***************************************************/
 void Display_Process()
 {
 Queue_Node *r,*w,*f,*t;
 ready=wait=head;
 t=r=ready->next;w=wait->next;f=finish->next;
 while(f){f->data.status=FINISH;f=f->next;}
 f=finish->next;
 printf("                                                      \n");
 printf("                                                      \n");
 printf("           -----------------PROCESS----------------------  \n");
 printf("          |   name   |status  |Priority|Run_Time|Needtime|\n");
 printf("          |-------- -|--------|--------|--------|--------|\n");
 while(t!=NULL ) /*Êä³ö´¦ÓÚÖ´ĞĞÌ¬µÄ½ø³Ì*/
 { if(t->data.status==RUN)
   {printf("         *|%-10s|%-8d|%-8d|%-8d|%-8d|\n",t->data.name,t->data.status,
   t->data.priority,t->data.runtime,t->data.needtime);
   }
   t=t->next;
  }
  while(r!=NULL ) /*Êä³ö¹«¹²¶ÓÁĞÖĞµÄ¾ÍĞ÷½ø³Ì*/
 { if(r->data.status==READY)
   {printf("          |%-10s|%-8d|%-8d|%-8d|%-8d|\n",r->data.name,r->data.status,
   r->data.priority,r->data.runtime,r->data.needtime);
   }
   r=r->next;
  }
  while(w!=NULL) /*Êä³ö¹«¹²¶ÓÁĞÖĞµÄµÈ´ı½ø³Ì*/
 { if(w->data.status==WAIT)
   {printf("          |%-10s|%-8d|%-8d|%-8d|%-8d|\n",w->data.name,w->data.status,
   w->data.priority,w->data.runtime,w->data.needtime);
   }
   w=w->next;
  }
  while(f!=NULL) /*Êä³öÍê³É¶ÓÁĞÖĞµÄ½ø³Ì*/
 { printf("          |%-10s|%-8d|%-8d|%-8d|%-8d|\n",f->data.name,f->data.status,
   f->data.priority,f->data.runtime,f->data.needtime);
   f=f->next;
  }
  printf("           ----------------------------------------------  \n");
  printf("           1:RUN,2:READY,3:WAIT,4:FINISH\n");
  getchar();
  return;
 }

/**************************************************************/
/*´Ëº¯ÊıÓÃÓÚÔÚ¹«¹²¶ÓÁĞÖĞÕÒµ½Ò»¸öÓÅÏÈÊı×î´óµÄ¾ÍĞ÷½ø³Ì*/
/**************************************************************/
Queue_Node *get_maxprinode(Queue_Node *head)
{
  Queue_Node *p,*q,*t;
  int max,flags=0;
  t=head->next;
  while(t!=NULL)
  { if(t->data.status==READY) /*³õÊ¼maxÎªµÚÒ»¸ö¾ÍĞ÷½ø³ÌµÄÓÅÏÈÊı*/
    {q=t;
     max=t->data.priority;
     flags=1;
     break;
     }
    t=t->next;
   }
  if(flags==0) {return NULL;} /*µ±Ç°Ã»ÓĞ¾ÍĞ÷½ø³Ì£¬·µ»ØNULL*/
  p=t->next;
  while(p)
  {
   if(p->data.priority>max && p->data.status==READY)/*ÖğÒ»±È½Ï¾ÍĞ÷½ø³Ì£¬Ñ¡³öÓÅÏÈÊı×î´óµÄ½ø³Ì*/
   {max=p->data.priority;
    q=p;
    }
   p=p->next;
  }
  return q;
}


/**************************************************************
 Ê±¼äÆ¬ÂÖ×ªµ÷¶È: °´Ë³Ğò½«CPUÊ±¼äÆ¬¸³¸ø¾ÍĞ÷ÁĞ¶ÓÖĞÃ¿Ò»¸ö½ø³Ì£¬¼´½ø³ÌÂÖÁ÷Õ¼ÓĞCPU£¬
 µ±Ê±¼äÆ¬ÓÃÍêÊ±£¬½ø³ÌÎ´Ö´ĞĞÍê±Ï,ÔòÏµÍ³°ş¶á¸Ã½ø³ÌµÄCPU£¬½«RUN×´Ì¬±ä³ÉREADY×´Ì¬£¬¬
 Í¬Ê±°´Ë³ĞòÑ¡ÔñÁíÒ»¸ö½ø³ÌÔËĞĞ.µ±Ê±¼äÆ¬ÓÃÍêÊ±£¬½ø³ÌÖ´ĞĞÍê±Ï,ÔòÏú»Ù´Ë½ø³Ì¡££
 **************************************************************/
void Timeslice_Round(Queue_Node *head)
 { Queue_Node *q,*p,*w;
   ready=wait=head;
   w=q=ready->next; /*readyÎª¾ÍĞ÷¶ÓÁĞÍ·Ö¸Õë*/
   q->data.status=RUN; /*½«¾ÍĞ÷¶ÓÁĞµÄµÚÒ»¸ö½ø³ÌÖÃÎªÔËĞĞÌ¬*/
   clrscr();
   while(q) /*q!=NULL*/
     { if(q->data.status==RUN)
       {  Display_Process();/*ÏÔÊ¾ËùÓĞ½ø³ÌµÄĞÅÏ¢*/
          q->data.needtime=q->data.needtime-1; /*ĞèÒªÖ´ĞĞµÄÊ±¼ä¼õ£±*/
          q->data.runtime=q->data.runtime+1; /*cpuÕ¼ÓÃµÄÊ±¼äÔö£±*/
          sleep(timeslice);/*Ö´ĞĞÒ»¸öÊ±¼äÆ¬*/
          getchar();
          if(q->data.needtime<=0)
           {
              Destroy_Process(q); /*Ïú»Ù´Ë½ø³Ì*/
              w=head->next;
              while(w!=NULL && w->data.status!=WAIT) {w=w->next;}
              if(w!=NULL) w->data.status=READY; /*Ïú»ÙÒ»½ø³Ìºó£¬Á¢¼´½«Ò»µÈ´ı½ø³Ì¾ÍĞ÷*/
            }
         q->data.status=READY; /*ÔËĞĞÍêÒ»¸öÊ±¼äÆ¬ºó£¬ÓÉRUN×´Ì¬±äÎªREADY*/
         if(q->next!=NULL) /*q->next!=NULL£¬ÂÖ×ªÖÁÏÂÒ»¸ö½ø³Ì*/
         {  q=q->next;
            q->data.status=RUN;
         }
         else
         {   q=ready->next;/*Ñ­»·ÌøÖÁµÚÒ»¸öÃ»ÓĞÔËĞĞÍêµÄ½ø³Ì*/
             q->data.status=RUN;
          }
       }/*endif*/
     if(q==NULL) {Display_Process();Delete_AllProcess();return ;}
   }/*end while*/
  Delete_AllProcess();return ;
 }

/********************************************************************
 »ùÓÚ¶¯Ì¬ÓÅÏÈÊıµÄµ÷¶È:ÔÚÊ±¼äÆ¬ÄÚÔËĞĞÒ»´Î½ø³Ìºó£¬ÏàÓ¦½ø³ÌµÄÓÅÏÈÊı¼õ£±£¬
 È»ºóÔÙ´Ó¾ÍĞ÷½ø³ÌÖĞÑ¡³öÒ»¸öÓÅÏÈÊı×î´óµÄ½ø³ÌÓë¸ÕÔËĞĞÒ»´ÎµÄ½ø³ÌµÄÓÅÏÈÊı±È½Ï£¬
 Èç¹ûµ±Ç°Î´ÔËĞĞÍêµÄ½ø³ÌµÄÓÅÏÈÊıµÍÓÚÑ¡³öµÄ¾ÍĞ÷½ø³ÌµÄÓÅÏÈÊı£¬ÔòÏÈÔËĞĞÑ¡³öµÄ¾ÍĞ÷½ø³Ì£¬
 ·ñÔòÔÚÏÂÒ»¸öÊ±¼äÄÚ¼ÌĞøÔËĞĞÎ´ÔËĞĞÍêµÄ½ø³Ì,Ö±ÖÁÓÅÏÈÊıµÍÓÚÖØĞÂÑ¡³öµÄ¾ÍĞ÷½ø³ÌµÄÓÅÏÈÊı¡£
 ********************************************************************/
void Priority_First(Queue_Node *head)
 { Queue_Node *q,*p,*w;
   ready=wait=head;
   q=get_maxprinode(head);
   q->data.status=RUN; /*½«¾ÍĞ÷¶ÓÁĞÖĞÓÅÏÈÊı×î´óµÄ½ø³ÌÖÃÎªÔËĞĞÌ¬*/
   clrscr();
   while(q) /*q!=NULL*/
     { if(q->data.status==RUN)
       {
          Display_Process();/*ÏÔÊ¾ËùÓĞ½ø³ÌµÄĞÅÏ¢*/
          q->data.needtime=q->data.needtime-1; /*ĞèÒªÖ´ĞĞµÄÊ±¼ä¼õ£±*/
          q->data.runtime=q->data.runtime+1; /*cpuÕ¼ÓÃµÄÊ±¼äÔö£±*/
          q->data.priority=q->data.priority-1; /*¶¯Ì¬¼ÆËãÓÅÏÈÊı*/
          sleep(timeslice);/*Ö´ĞĞÒ»¸öÊ±¼äÆ¬*/
          getchar();
          if(q->data.needtime<=0) /*½ø³ÌÒÑ¾­ÔËĞĞÍê*/
           {
              Destroy_Process(q); /*Ïú»Ù´Ë½ø³Ì*/
              w=ready->next;
              while(w!=NULL && w->data.status!=WAIT) {w=w->next;}
              if(w!=NULL)
              {w->data.status=READY;} /*Ïú»ÙÒ»½ø³Ìºó£¬Á¢¼´½«Ò»µÈ´ı½ø³Ì¾ÍĞ÷*/
              p=get_maxprinode(head);
              if(p!=NULL)
              {
               p->data.status=RUN;
               q=p;
               }
              else
               {q=p;}

            }
         else /*½ø³ÌÎ´ÔËĞĞÍê*/
          {
           p=get_maxprinode(head); /*´Ó¾ÍĞ÷½ø³Ì¶ÓÁĞÖĞÌôÑ¡Ò»¸öÓÅÏÈÊı×î´óµÄ½ø³ÌÀ´ÔËĞĞ*/
           if(strcmp(p->data.name,q->data.name)==0) continue;
           if(p!=NULL && q->data.priority<p->data.priority)
            /*Èç¹ûµ±Ç°Î´ÔËĞĞÍêµÄ½ø³ÌµÄÓÅÏÈÊıµÍÓÚ¾ÍĞ÷½ø³ÌµÄÓÅÏÈÊı*/
            { q->data.status=READY;  /*ÓÉRUN×´Ì¬±äÎªREADY*/
              p->data.status=RUN;
              q=p;
            }
          }
       }/*endif*/
    }/*end while*/
   Display_Process();
   Delete_AllProcess();
   return ;
 }

/**************************************************************
¡¡´´½¨½ø³Ì£ºÈô¾ÍĞ÷½ø³ÌÎ´Âú£¬Ê×ÏÈ½«¾ÍĞ÷½ø³ÌÌí¼ÓÖÁ¹«¹²¶ÓÁĞ£¬²¢±ê¼ÇÎªREADY×´Ì¬£»
¡¡¡¡¡¡¡¡¡¡¡¡Èô¾ÍĞ÷¶ÓÁĞÒÑÂú£¬È»ºó½«µÈ´ı½ø³ÌÌí¼ÓÖÁ¹«¹²¶ÓÁĞ£¬²¢±ê¼ÇÎªWAIT×´Ì¬¡£
 **************************************************************/
void Create_Process(Queue_Node *head)
{
    Queue_Node *p,*r,*w; /*ÊµÏÖÌí¼Ó²Ù×÷µÄÁÙÊ±µÄ½á¹¹ÌåÖ¸Õë±äÁ¿*/
    Queue_Node *head1;
    int num=0,i=0,time=0;
    int count1=0,count2=0;
    char name[10];
    r=wait=ready=head;
    clrscr();
    do{
     printf("Input the process number[<=%d]:",N);
     scanf("%d",&num);
     }while(num>N);
    for(i=1;i<=num;i++) /*ÖğÌõÊäÈëPCBÄÚÈİ*/
     {
      printf("**************************************\n");
      printf("Name of process %d:",i);
      scanf("%s",name);
      head1=head->next;
      while(head1!=NULL) /*¼ì²é¾ÍĞ÷¶ÓÁĞºÍµÈ´ı¶ÓÁĞÖĞÊÇ·ñÓĞÍ¬±êÊ¶½ø³Ì*/
      {
        if(strcmp(head1->data.name,name)==0)
         {
          printf("Name %s has been used ,Please use another name:",name);
          scanf("%s",&name);
          }
        head1=head1->next;
      }
      printf("Process needtime:");
      scanf("%d",&time);
      p=(Queue_Node *)malloc(sizeof(Queue_Node)); /*ÉêÇëÄÚ´æ¿Õ¼ä*/
      if(!p)
      {
      printf("\n allocate memory failure "); /*ÈçÃ»ÓĞÉêÇëµ½£¬´òÓ¡ÌáÊ¾ĞÅÏ¢*/
      return ;             /*·µ»ØÖ÷½çÃæ*/
      }
      strcpy(p->data.name,name); /*½ø³Ì±êÊ¶·û*/
      p->data.runtime=0;       /*½ø³ÌÔÚCPUÖĞµÄÒÑÔËĞĞÊ±¼ä*/
      p->data.needtime=time;    /*½ø³ÌĞèÒªÔËĞĞÊ±¼ä*/
      p->data.priority=100-time;  /*½ø³ÌÓÅÏÈÊı*/
      if(i<=Ready_N) /*Ã»ÓĞ³¬¹ıÔÊĞíµÄ¾ÍĞ÷½ø³ÌÊı*/
      {
        p->data.status=READY; /*½ø³Ì×´Ì¬*/
        p->next=NULL;
        r->next=p;  /*½«ĞÂ½¨µÄ½áµã¼ÓÈë¶ÓÁĞ¶ÓÎ²*/
        r=p;
        count1++; /*Í³¼Æ¶ÓÁĞÖĞµÄ¾ÍĞ÷½ø³ÌÊı*/
       }
      else
      { p->data.status=WAIT; /*½ø³Ì×´Ì¬*/
        p->next=NULL;
        r->next=p;  /*½«ĞÂ½¨µÄ½áµã¼ÓÈë¶ÓÁĞ¶ÓÎ²*/
        r=p;
        count2++; /*Í³¼ÆµÈ´ı½ø³ÌÊı*/
       }
     }
   printf("**************************************\n");
   printf("All process has been created!\nReady_Process:%d,Wait_Process:%d\n",count1,count2);
   getchar();getchar();
   return ;
}

/**************************************************************/
/*²Ëµ¥*/
/*************************************************************/
int main_menu()
{
   int  i;
   int key=0; /*¼ÇÂ¼¼üÖµ*/
   int select=0;
   char *menu[]= {     /*¶¨Òå²Ëµ¥×Ö·û´®Êı×é*/
   "***************MENU*************", /*²Ëµ¥µÄ±êÌâĞĞ*/
   " 1. Create   Process", /*´´½¨½ø³Ì*/
   " 2. Display  Process \n",  /*ÏÔÊ¾ËùÓĞ½ø³Ì*/
   " 3. Schedule by Timeslice_Round", /*°´Ê±¼äÆ¬ÂÖ×ªµ÷¶È*/
   " 4. Schedule by Priority_First", /*°´ÓÅÏÈÊıµ÷¶È*/
   " 5. Exit", /*ÍË³ö½ø³ÌÄ£ÄâÆ÷*/
   "********************************"};
   clrscr();  /*ÇåÆÁ*/
   textcolor(14); /*ÉèÖÃÎÄ±¾ÑÕÉ«Îª»ÆÉ«*/
   textbackground(1);
   gotoxy(10,2);
   cprintf("%c",218); /*Êä³ö×óÉÏ½Ç±ß¿ò©³*/
   for(i=1;i<51;i++)
   cprintf("%c",196); /*Êä³öÉÏ±ß¿òË®Æ½Ïß*/
   cprintf("%c",191);  /*Êä³öÓÒÉÏ½Ç±ß¿ò©·*/
   for(i=3;i<22;i++)/*Êä³ö×óÓÒÁ½±ßµÄ´¹Ö±Ïß*/
   {
      gotoxy(10,i);cprintf("%c",179);
      gotoxy(61,i);cprintf("%c",179);
   }
   gotoxy(10,22);cprintf("%c",192); /*Êä³ö×óÉÏ½Ç±ß¿ò©»*/
   for(i=1;i<51;i++)
      cprintf("%c",196);;  /*Êä³öÏÂ±ß¿òË®Æ½Ïß*/
    cprintf("%c",217);  /*Êä³öÓÒÏÂ½Ç±ß¿ò©¿*/
   window(11,3,60,21);
   clrscr();   /*ÇåÆÁ*/
   gotoxy(15,4);printf("%s"," PROCESS SIMULATOR");
   for(i=0;i<7;i++)
   {
      gotoxy(10,i+5);
      cprintf("%s",menu[i]); /*Êä³ö²Ëµ¥ÏîÊı×é*/
   }
   i=1;
   gotoxy(10,6);  /*ÉèÖÃÄ¬ÈÏÑ¡ÏîÔÚµÚÒ»Ïî*/
   textbackground(11);
   cprintf("%s",menu[1]);  /*Êä³ö²Ëµ¥Ïî£¬±íÊ¾Ñ¡ÖĞ*/
   gotoxy(10,6);  /*ÒÆ¶¯¹â±êµ½²Ëµ¥µÄµÚÒ»Ïî*/
   while(key!=13)   /*13»Ø³µ¼ü*/
   {
      while(bioskey(1)==0);
      key=bioskey(0);
      key=key&0xff?key&0xff:key>>8; /*¶Ô°´¼ü½øĞĞÅĞ¶Ï*/
      gotoxy(10,i+5);
      textbackground(1);
      cprintf("%s",menu[i]);  /*Êä³ö²Ëµ¥Ïî*/
      if(key==72) i=(i==1?5:i-1); /*Èç°´ÏòÉÏ¼ü¡ü£¬i¼õ1£¬ÈçÒÑµ½µÚÒ»ĞĞÔÙÉÏÒÆ£¬Ôòµ½×îºóÒ»ĞĞ*/
      if(key==80)i=(i==5?1:i+1); /*Èç°´ÏòÏÂ¼ü¡ı£¬i¼Ó1£¬ÈçÒÑµ½×îºóÒ»ĞĞÔÙÏÂÒÆ£¬Ôòµ½µÚÒ»ĞĞ*/
      gotoxy(10,i+5); /*¹â±êÒÆ¶¯iµÄÏÂÒ»Ïî*/
      textbackground(11);
      cprintf("%s",menu[i]);  /*Êä³ö²Ëµ¥Ïî*/
      select=i;  /*¸ø´ú±í²Ëµ¥Ñ¡ÏîµÄÕûÊı¸³Öµ*/
   }
   textbackground(0);
   window(1,1,80,25);  /*»Ö¸´Ô­´°¿Ú´óĞ¡*/
   return select;
}
/**************************************************************/
 main()
 {
   head=(Queue_Node *)malloc(sizeof(Queue_Node));
   head->next=NULL;
   run=wait=ready=head;/*Ö´ĞĞ½ø³Ì¡¢¾ÍĞ÷½ø³ÌºÍµÈ´ı½ø³Ì¹²Ïí¶ÓÁĞ£¬Ò²³ÆÎª¹«¹²¶ÓÁĞ*/
   finish=(Queue_Node *)malloc(sizeof(Queue_Node));
   finish->next=NULL;
   clrscr();
   while(1)
   {
      switch(main_menu())   /*µ÷ÓÃ²Ëµ¥º¯Êı·µ»ØÒ»¸öÕûÊıÖµ*/
      {
      case 1:clrscr();Create_Process(head);break;   /*´´½¨½ø³Ì*/
      case 2:clrscr();Display_Process();break;      /*ÏÔÊ¾ËùÓĞ½ø³Ì*/
      case 3:clrscr();Timeslice_Round(head);break;  /*°´Ê±¼äÆ¬ÂÖ×ª½ø³Ìµ÷¶È*/
      case 4:clrscr();Priority_First(head);break;  /*°´ÓÅÏÈÊı½øĞĞ½ø³Ìµ÷¶È*/
      case 5:exit(0);break;              /*ÍË³ö*/
      }
   }
}



