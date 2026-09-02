#include <dos.h> 
#include <stdio.h> 
#include <ctype.h> 
#include <conio.h> 
#include <bios.h> 
#include <alloc.h> 

typedef struct winer  
{ 
    int x,y; 
    struct winer *p; 
}winer; 

char status [20][20]; 
char far *printScreen=(char far* )0xB8000000; 

void putoutChar(int y,int x,char ch,char fc,char bc);
void printWall(int x, int y);
void printBox(int x, int y);
void printBoxDes(int x, int y);
void printDestination(int x, int y);
void printDestination1(int x,int y,winer **win,winer **pw);
void printMan(int x, int y);
void init();
winer *initStep1();
winer *initStep2();
winer *initStep3();
void moveBoxSpacetoSpace(int x ,int y, char a);
void moveBoxDestoSpace(int x ,int y, char a) ;
void moveBoxSpacetoDes(int x, int y, char a);
void moveBoxDestoDes(int x, int y, char a);
int judge(int x, int y);
void move(int x, int y, char a);
void reset(int i);


void putoutChar(int y,int x,char ch,char fc,char bc) 
{
    printScreen[(x*160)+(y<<1)+0]=ch; 
    printScreen[(x*160)+(y<<1)+1]=(bc*16)+fc; 
} 
void printWall(int x,int y) 
{ 
    putoutChar(y-1,x-1,219,GREEN,BLACK); 
    status[x][y]='w'; 
} 
 
void printBox(int x,int y) 
{ 
    putoutChar(y-1,x-1,10,WHITE,BLACK); 
    status[x][y]='b'; 
} 
 
void printDestination1(int x,int y,winer **win,winer **pw) 
{ 
    winer *qw; 
    putoutChar(y-1,x-1,003,YELLOW,BLACK); 
    status[x][y]='m'; 
    if(*win==NULL) 
    {
    *win=*pw=qw=(winer* )malloc(sizeof(winer)); 
    (*pw)->x=x;
        (*pw)->y=y;
        (*pw)->p=NULL; 
    }
    else
    {
    qw=(winer* )malloc(sizeof(winer)); 
    qw->x=x;
    qw->y=y;
    (*pw)->p=qw;
    (*pw)=qw;qw->p=NULL; 
    }          
} 
 
void printDestination(int x,int y) 
{ 
    putoutChar(y-1,x-1,003,YELLOW,BLACK); 
    status[x][y]='m'; 
} 
 
void printMan(int x,int y) 
{ 
    gotoxy(y,x); 
    _AL=02;
    _CX=01;
    _AH=0xa; 
    geninterrupt(0x10); 
} 
 
void printBoxDes(int x,int y) 
{ 
    putoutChar(y-1,x-1,10,YELLOW,BLACK); 
    status[x][y]='i'; 
} 
 
void init() 
{ 
    int i,j; 
    for(i=0;i<20;i++) 
    for(j=0;j<20;j++) 
        status[i][j]=0; 
    _AL=3; 
    _AH=0; 
    geninterrupt(0x10); 
    gotoxy(40,4); 
    printf("Welcome to the box world!"); 
    gotoxy(40,6); 
    printf("You can use up, down, left,"); 
    gotoxy(40,8); 
    printf("right key to control it, or");
    gotoxy(40,10); 
    printf("you can press Esc to quit it."); 
    gotoxy(40,12); 
    printf("Press space to reset the game."); 
    gotoxy(40,14); 
    printf("Wish you have a good time !"); 
    gotoxy(40,16); 
    printf("April , 2007"); 

} 
winer *initStep1() 
{ 
    int x;
    int y; 
    winer *win=NULL;
    winer *pw; 
    for(x=1,y=5;y<=20;y++)
    printWall(x+4,y+10); 

    for(y=5,x=2;x<=5;x++)
    printWall(x+4,y+10); 

    for(y=20,x=2;x<=5;x++)
    printWall(x+4,y+10); 

    for(y=1,x=3;x<=8;x++)
    printWall(x+4,y+10);
     
    for(x=3,y=3;x<=5;x++)
    printWall(x+4,y+10); 

    for(x=5,y=19;x<=15;x++)
    printWall(x+4,y+10); 

    for(x=7,y=4;x<=15;x++)
    printWall(x+4,y+10); 

    for(x=15,y=5;y<=18;y++)
    printWall(x+4,y+10); 

    for(x=8,y=2;y<=3;y++)
    printWall(x+4,y+10);

    
    for(x=5,y=7;y<=15;y++)
    printWall(x+4,y+10);

    for(y=8,x=6;x<=10;x++)
    printWall(x+4,y+10);

    printWall(5+4,4+10);

    printWall(3+4,2+10); 

    printBox(3+4,16+10);
    printBox(3+4,17+10);
    printBox(4+4,17+10);
    printDestination1(4+4,2+10,&win,&pw); 
    printDestination1(5+4,2+10,&win,&pw); 
    printDestination1(6+4,2+10,&win,&pw); 
    printMan(2+4,8+10); 
    return win; 
} 
 
winer *initStep2() 
{ 
    int x;
    int y; 
    winer *win=NULL;
    winer *pw; 
    for(x=1,y=4;y<=14;y++)
    printWall(x+4,y+10);
    for(x=2,y=2;y<=4;y++)
    printWall(x+4,y+10); 
    for(x=2,y=14;x<=4;x++)
    printWall(x+4,y+10); 
    for(x=4,y=1;x<=15;x++)
    printWall(x+4,y+10); 
    for(x=15,y=2;y<=15;y++)
    printWall(x+4,y+10); 
    for(x=4,y=15;x<=15;x++)
    printWall(x+4,y+10); 
    for(x=4,y=13;x<=10;x++)
    printWall(x+4,y+10);
    for(x=3,y=2;x<=4;x++)
    printWall(x+4,y+10); 
    for(x=4,y=11;x<=10;x++)
    printWall(x+4,y+10);
    for(x=12,y=9;y<=13;y++)
    printWall(x+4,y+10); 
    for(x=9,y=7;x<=14;x++)
    printWall(x+4,y+10); 

    printBox(11+4,12+10);
    printBox(8+4,12+10);
    printBox(13+4,3+10);
    printDestination1(5+4,14+10,&win,&pw);
    printDestination1(6+4,14+10,&win,&pw);
    printDestination1(7+4,14+10,&win,&pw);
    printMan(2+4,6+10); 
    return win; 
}  
winer *initStep3() 
{ 
    int x;
    int y; 
    winer *win=NULL;
    winer *pw; 
    for(x=1,y=2;y<=8;y++)
    printWall(x+4,y+10); 
    for(x=2,y=2;x<=4;x++)
    printWall(x+4,y+10); 
    for(x=4,y=1;y<=3;y++)
    printWall(x+4,y+10); 
    for(x=5,y=1;x<=15;x++)
    printWall(x+4,y+10); 
    for(x=15,y=2;y<=5;y++)
    printWall(x+4,y+10); 
    for(x=5,y=5;x<=14;x++)
    printWall(x+4,y+10); 
    for(x=14,y=6;y<=15;y++)
    printWall(x+4,y+10);
    for(x=3,y=15;x<=14;x++)
    printWall(x+4,y+10);
    for(x=3,y=6;y<=15;y++)
    printWall(x+4,y+10);
    for(x=5,y=8;y<=14;y++)
    printWall(x+4,y+10);
    for(x=8,y=3;x<=14;x++)
    printWall(x+4,y+10); 
    for(x=7,y=7;x<=13;x++)
    printWall(x+4,y+10); 

    printWall(2+4,8+10);
    printBox(6+4,3+10);
    printBox(4+4,4+10); 
    printBox(5+4,6+10); 
    printDestination1(2+4,5+10,&win,&pw); 
    printDestination1(2+4,6+10,&win,&pw); 
    printDestination1(2+4,7+10,&win,&pw); 
    printMan(2+4,4+10); 
    return win; 
} 

void moveBoxSpacetoSpace(int x,int y,char a)
{ 
    switch(a)
    {
    case 'u':
    status[x-1][y]=0;
    printf(" "); 
    printBox(x-2,y);
    printMan(x-1,y); 
    status[x-2][y]='b';
    break; 
    case 'd':
    status[x+1][y]=0;
    printf(" "); 
    printBox(x+2,y);
    printMan(x+1,y); 
    status[x+2][y]='b';
    break;
    case 'l':
    status[x][y-1]=0;
    printf(" "); 
    printBox(x,y-2);
    printMan(x,y-1); 
    status[x][y-2]='b';
    break;
    case 'r':
    status[x][y+1]=0;
    printf(" "); 
    printBox(x,y+2);
    printMan(x,y+1); 
    status[x][y+2]='b';
    break;
    default:
    break;
    }
}     
 
 
void moveBoxDestoSpace(int x,int y,char a) 
{ 
    switch(a)
    {
    case 'u':
    status[x-1][y]='m';
    printf(" "); 
    printBox(x-2,y);
    printMan(x-1,y); 
    status[x-2][y]='b';
    break;
    case 'd':
    status[x+1][y]='m';
    printf(" "); 
    printBox(x+2,y);
    printMan(x+1,y); 
    status[x+2][y]='b';
    break; 
    case 'l':
    status[x][y-1]='m';
    printf(" "); 
    printBox(x,y-2);
    printMan(x,y-1); 
    status[x][y-2]='b';
    break; 
    case 'r':
    status[x][y+1]='m';
    printf(" "); 
    printBox(x,y+2);
    printMan(x,y+1); 
    status[x][y+2]='b';
    break; 
    default:
    break; 
    }
} 
 
 
 
 
void moveBoxSpacetoDes(int x,int y,char a) 
{ 
    switch(a)
    {
    case 'u':
    status[x-1][y]=0;
    printf(" "); 
    printBoxDes(x-2,y);
    printMan(x-1,y); 
    status[x-2][y]='i';
    break;
    case 'd':
    status[x+1][y]=0;
    printf(" "); 
    printBoxDes(x+2,y);
    printMan(x+1,y); 
    status[x+2][y]='i';
    break; 
    case 'l':
    status[x][y-1]=0;
    printf(" "); 
    printBoxDes(x,y-2);
    printMan(x,y-1); 
    status[x][y-2]='i';
    break; 
    case 'r':
    status[x][y+1]=0;
    printf(" "); 
    printBoxDes(x,y+2);
    printMan(x,y+1); 
    status[x][y+2]='i';
    break; 
    default:
    break; 
    }
} 
 
 
void moveBoxDestoDes(int x,int y,char a) 
{ 
    switch(a) 
    {
    case 'u':
    status[x-1][y]='m';
    printf(" "); 
    printBoxDes(x-2,y);
    printMan(x-1,y); 
    status[x-2][y]='i';
    break; 
    case 'd':
    status[x+1][y]='m';
    printf(" "); 
    printBoxDes(x+2,y);
    printMan(x+1,y); 
    status[x+2][y]='i';
    break; 
    case 'l':
    status[x][y-1]='m';
    printf(" "); 
    printBoxDes(x,y-2);
    printMan(x,y-1); 
    status[x][y-2]='i';
    break; 
    case 'r':
    status[x][y+1]='m';
    printf(" "); 
    printBoxDes(x,y+2);
    printMan(x,y+1); 
    status[x][y+2]='i';
    break; 
    default:
    break; 
    }
}
 
 
int judge(int x,int y) 
{ 
    int i; 
    switch(status[x][y]) 
    {
    case 0:
    i=1;
    break; 
    case 'w':
    i=0;
    break; 
    case 'b':
    i=2;
    break; 
    case 'i':
    i=4;
    break; 
    case 'm':
    i=3;
    break; 
    default:
    break;
    }
    return i; 
} 
 
 
void move(int x,int y,char a) 
{ 
    switch(a) 
    {
    case 'u':
    if(!judge(x-1,y))
    {
        gotoxy(y,x); 
        break; 
    }
    else if(judge(x-1,y)==1||judge(x-1,y)==3) 
    {
        if(judge(x,y)==3) 
        {
        printDestination(x,y); 
        printMan(x-1,y); 
        break; 
        }
        else 
        {
        printf(" "); 
        printMan(x-1,y); 
        break; 
        }
    }
    else if(judge(x-1,y)==2) 
    {
        if(judge(x-2,y)==1) 
        {
        moveBoxSpacetoSpace(x,y,'u'); 
        if(judge(x,y)==3)  
            printDestination(x,y);  
        gotoxy(y,x-1); 
        }
        else if(judge(x-2,y)==3) 
        {
        moveBoxSpacetoDes(x,y,'u'); 
        if(judge(x,y)==3) 
            printDestination(x,y); 
        gotoxy(y,x-1); 
        }
        else 
                gotoxy(y,x); 
        break;
    }
    else if(judge(x-1,y)==4) 
    {
        if(judge(x-2,y)==1) 
        {
        moveBoxDestoSpace(x,y,'u'); 
        if(judge(x,y)==3) 
            printDestination(x,y); 
        gotoxy(y,x-1); 
        }
        else if(judge(x-2,y)==3) 
        {
        moveBoxDestoDes(x,y,'u'); 
        if(judge(x,y)==3) 
            printDestination(x,y); 
        gotoxy(y,x-1); 
        }
        else 
        gotoxy(y,x); 
        break; 
    }
    case 'd':
    if(!judge(x+1,y)) 
    {
        gotoxy(y,x); 
        break; 
    }
    else if(judge(x+1,y)==1||judge(x+1,y)==3) 
    {
        if(judge(x,y)==3) 
        {
        printDestination(x,y); 
        printMan(x+1,y); 
        break; 
        }
        else
        {
        printf(" "); 
        printMan(x+1,y); 
        break; 
        }
    } 
    else if(judge(x+1,y)==2) 
    {
        if(judge(x+2,y)==1) 
        {
        moveBoxSpacetoSpace(x,y,'d'); 
        if(judge(x,y)==3) 
            printDestination(x,y); 
        gotoxy(y,x+1); 
        }
        else if(judge(x+2,y)==3) 
        {
        moveBoxSpacetoDes(x,y,'d'); 
        if(judge(x,y)==3)  
            printDestination(x,y); 
        gotoxy(y,x+1); 
        } 
        else
        gotoxy(y,x); 
        break; 
    }
    else if(judge(x+1,y)==4) 
    {  
        if(judge(x+2,y)==1) 
        { 
        moveBoxDestoSpace(x,y,'d'); 
        if(judge(x,y)==3) 
            printDestination(x,y); 
        gotoxy(y,x+1); 
        } 
        else if(judge(x+2,y)==3) 
        { 
        moveBoxDestoDes(x,y,'d'); 
        if(judge(x,y)==3)  
            printDestination(x,y); 
        gotoxy(y,x+1); 
        } 
        else 
        gotoxy(y,x); 
        break; 
    } 
    case 'l':
    if(!judge(x,y-1)) 
    { 
        gotoxy(y,x); 
        break; 
    } 
    else if(judge(x,y-1)==1||judge(x,y-1)==3) 
    { 
        if(judge(x,y)==3) 
        { 
        printDestination(x,y); 
        printMan(x,y-1); 
        break; 
        } 
        else 
        { 
        printf(" "); 
        printMan(x,y-1); 
        break; 
        } 
    } 
    else if(judge(x,y-1)==2) 
    { 
        if(judge(x,y-2)==1) 
        { 
        moveBoxSpacetoSpace(x,y,'l'); 
        if(judge(x,y)==3) 
            printDestination(x,y); 
        gotoxy(y-1,x); 
        } 
        else if(judge(x,y-2)==3) 
        { 
        moveBoxSpacetoDes(x,y,'l'); 
        if(judge(x,y)==3)  
            printDestination(x,y); 
        gotoxy(y-1,x); 
        } 
        else 
        gotoxy(y,x); 
        break; 
    } 
    else if(judge(x,y-1)==4) 
    {
        if(judge(x,y-2)==1) 
        {
        moveBoxDestoSpace(x,y,'l'); 
        if(judge(x,y)==3)
            printDestination(x,y); 
        gotoxy(y-1,x); 
        } 
        else if(judge(x,y-2)==3) 
        {
        moveBoxDestoDes(x,y,'l'); 
        if(judge(x,y)==3) 
            printDestination(x,y); 
        gotoxy(y-1,x); 
        } 
        else 
        gotoxy(y,x); 
        break; 
    } 
    case 'r':
    if(!judge(x,y+1))    
    {
        gotoxy(y,x);
        break;
    }
    else if(judge(x,y+1)==1||judge(x,y+1)==3) 
    {
        if(judge(x,y)==3) 
        {
        printDestination(x,y);
        printMan(x,y+1);
        break;
        } 
        else 
        {
        printf(" ");
        printMan(x,y+1);
        break;
        } 
    }
    else if(judge(x,y+1)==2) 
    {
        if(judge(x,y+2)==1) 
        {    
        moveBoxSpacetoSpace(x,y,'r'); 
        if(judge(x,y)==3) 
            printDestination(x,y); 
        gotoxy(y+1,x); 
        } 
        else if(judge(x,y+2)==3) 
        {
        moveBoxSpacetoDes(x,y,'r'); 
        if(judge(x,y)==3) 
            printDestination(x,y); 
        gotoxy(y+1,x); 
        } 
        else 
        gotoxy(y,x);     
        break; 
    } 
    else if(judge(x,y+1)==4) 
    { 
        if(judge(x,y+2)==1) 
        {    
        moveBoxDestoSpace(x,y,'r'); 
        if(judge(x,y)==3) 
            printDestination(x,y); 
        gotoxy(y+1,x); 
        } 
        else if(judge(x,y+2)==3) 
        {
        moveBoxDestoDes(x,y,'r'); 
        if(judge(x,y)==3)
            printDestination(x,y);
        gotoxy(y+1,x); 
        } 
        else 
        gotoxy(y,x); 
        break; 
    } 
    default:
    break; 
    } 
} 

 
 
void reset(int i) 
{ 
    switch(i) 
    {
    case 0:
    init(); 
    initStep1();
    break; 
    case 1:
    init(); 
    initStep2();
    break; 
    case 2:
    init(); 
    initStep3();
    break;
    default:
    break; 
    } 
} 
 

void main() 
{ 
    int key;
    int x;
    int y;
    int s;
    int i=0;
    winer *win;
    winer *pw; 
    _AL=3;
    _AH=0; 
    geninterrupt(0x10); 
    init(); 
    win=initStep1();
    do{
    _AH=3; 
    geninterrupt(0x10); 
    x=_DH+1;
    y=_DL+1; 
    while(bioskey(1)==0);
    key=bioskey(0); 
    switch(key) 
    {
    case 0x4800:
        move(x,y,'u');
        break;
    case 0x5000:
        move(x,y,'d');
        break;
    case 0x4b00:
        move(x,y,'l');
        break;
    case 0x4d00:
        move(x,y,'r');
        break;  
    case 0x3920:
        reset(i);
        break;
    default:
        break; 
    }    
    s=0; 
    pw=win; 
    while(pw) 
    { 
        if(status[pw->x][pw->y]=='m')
        s++; 
        pw=pw->p; 
    }
    if(s==0) 
    { 
        free(win); 
        gotoxy(25,2); 
        printf("congratulate! You have done this step!"); 
        getch(); 
        i++; 
        switch(i) 
        {
        case 1:
        init();
        win=initStep2();
        break;
        case 2:
        init(); 
        win=initStep3();
        break;
        case 3:
        gotoxy(20,21);
        printf("Congratulation! \n");
                gotoxy(15,23);
                printf("You have done all the steps, Welcome to play again!"); 
        key=0x011b;
        getch();
        break;
        default:
        break; 
        } 
    } 
 
    }while(key!=0x011b); 
    _AL=3; 
    _AH=0; 
    geninterrupt(0x10); 
}
