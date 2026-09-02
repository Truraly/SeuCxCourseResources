/*导入库文件*/
#pragma comment(lib,"wsock32.lib")
/*加载头文件*/
#include <stdio.h>
#include <winsock2.h>
/*自定义函数原型*/
void initial();
int InitSockets(void);

void GetArgments(int argc, char **argv);
void ErrorPrint(x);
void userHelp();

int LoopControl(SOCKET listenfd, int isMultiTasking);

void Service(LPVOID lpv);

/*定义常量*/
#define MAX_SER 10
/*定义全局变量*/
char *hostName;
unsigned short maxService;
unsigned short  port;

/*初始化全局变量函数*/
void initial()
{
    hostName = "127.0.0.1";
    maxService = 3;
    port = 9999;
}

/*初始化Winsocket函数*/
int InitSockets(void)
{
    WSADATA wsaData;
    WORD sockVersion;
    int err;

    /*设置Winsock版本号*/
    sockVersion = MAKEWORD( 2, 2 );
    /*初始化Winsock*/
    err = WSAStartup( sockVersion, &wsaData );
    /*如果初始化失败*/
    if ( err != 0 )
    {
      printf("Error %d: Winsock not available\n", err);
      return 1;
    }
    return 0;
}


/*获取选项函数*/
void GetArgments(int argc, char **argv)
{
    int i;
    for(i=1; i < argc ;i++)
    {
        /*参数的第一个字符若是“-”*/
        if (argv[i][0] == '-')
        {
            /*转换成小写*/
            switch (tolower(argv[i][1]))
            {
                /*若是端口号*/
                case 'p': 
                    if (strlen(argv[i]) > 3)
                        port = atoi(&argv[i][3]);
                    break;
               /*若是主机名*/
               case 'h': 
                    hostName = &argv[i][3];
                    break;
                /*最多服务次数*/
                case 'n': 
                    maxService = atoi(&argv[i][3]);
                    break;
                /*其他情况*/
                default:
                    userHelp();
                    break;
            }
        }
    }
    return;
}

/*错误输出函数*/
void ErrorPrint(x)
{ 
    printf("Error %d: %s\n", WSAGetLastError(), x);
}

/*用户帮助函数*/
void userHelp()
{
    printf("userHelp:  -h:str -p:int -n:int\n");
    printf("           -h:str  The host name \n");
    printf("                   The default host is 127.0.0.1\n");
    printf("           -p:int  The Port number to use\n");
    printf("                   The default port is 9999\n");
    printf("           -n:int  The number of service,below MAX_SER \n");
    printf("                   The default number is 3\n");
    ExitProcess(-1);
}

/*循环控制函数*/
int LoopControl(SOCKET listenfd, int isMultiTasking)
{
    SOCKET acceptfd;
    struct sockaddr_in clientAddr;
    int err;
    int nSize;
    int serverNum = 0;
    HANDLE handles[MAX_SER];
    int myID;
    
    /*服务次数小于最大服务次数*/
    while (serverNum < maxService)
    {
      nSize = sizeof(clientAddr);
      /*接收客户端请求*/
      acceptfd = accept(listenfd, (struct sockaddr *)
                         &clientAddr, &nSize);
      /*如果接收失败*/
      if (acceptfd == INVALID_SOCKET)
      {
          ErrorPrint("Error: accept failed\n");
          return 1;
      }
      /*接收成功*/
      printf("Accepted connection from client at %s\n",
             inet_ntoa(clientAddr.sin_addr));
      /*如果允许多任务执行*/
      if (isMultiTasking) 
      {
          /*创建一个新线程来执行任务，新线程的初始堆栈大小为1000，线程执行函数
          是Service()，传递给Service()的参数为acceptfd*/
          handles[serverNum] = CreateThread(NULL, 1000,
                      (LPTHREAD_START_ROUTINE)Service,
                      (LPVOID) acceptfd, 0, &myID);

       }
      else
          /*直接调用服务客户端的函数*/
          Service((LPVOID) acceptfd);
      serverNum++;
    }

    if (isMultiTasking)
    {
        /*在一个线程中等待多个事件，当所有对象都被通知时函数才会返回，并且等待没有时间限制*/
        err = WaitForMultipleObjects(maxService, handles, TRUE, INFINITE);
        printf("Last thread to finish was thread #%d\n", err);

     }
    return 0;
}

/*服务函数*/
void Service(LPVOID lpv)
{
    SOCKET acceptfd = (SOCKET) lpv;
    const char *msg = "HELLO CLIENT";
    char response[4096];

    /*用0初始化response[4096]数组*/
    memset(response, 0, sizeof(response));
    /*接收数据，存入response中*/
    recv(acceptfd, response, sizeof(response), 0);

    /*如果接收到的数据和预定义的数据不同*/
    if (strcmp(response, "HELLO SERVER")) 
    {
        printf("Application:  client not using expected "
            "protocol %s\n", response);
    }
    else
        /*发送服务器端信息到客户端*/
        send (acceptfd, msg, strlen(msg)+1, 0);
    /*关闭套接字*/
    closesocket(acceptfd);
}


/*主函数*/
int main(int argc, char **argv)
{
    SOCKET listenfd;
    int err;
    struct sockaddr_in serverAddr;
    struct hostent *ptrHost;
    initial();
    GetArgments(argc,argv);
    InitSockets();
    /*创建TCP流套接字，在domain参数为PF_INET的SOCK_STREAM套接口中，protocol参数为0意味
    着告诉内核选择 IPPRPTP_TCP，这也意味着套接口将使用TCP/IP协议*/
    listenfd = socket(PF_INET, SOCK_STREAM, 0);
    /*如果创建套接字失败*/
    if (listenfd == INVALID_SOCKET)
    {
        printf("Error: out of socket resources\n");
        return 1;
    }

    /*如果是IP地址*/
    if (atoi(hostName)) 
    {
        /*将IP地址转换成32二进制表示法，返回32位二进制的网络字节序*/
        u_long ip_addr = inet_addr(hostName);
        /*根据IP地址找到与之匹配的主机名*/
        ptrHost = gethostbyaddr((char *)&ip_addr,
                     sizeof(u_long), AF_INET);
     }
    /*如果是主机名*/
    else
        /*根据主机名获取一个指向hosten的指针，该结构中包含了该主机所有的IP地址*/
        ptrHost = gethostbyname(hostName);

    /*如果解析失败*/
    if (!ptrHost)
    {
        ErrorPrint("cannot resolve hostname");
        return 1;
    }
    
    /*设置服务器地址*/
    /*设置地址族为PF_INET*/
    serverAddr.sin_family = PF_INET;
    /*将一个通配的Internet地址转换成无符号长整型的网络字节序数*/
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    /*将端口号转换成无符号短整型的网络字节序数*/
    serverAddr.sin_port = htons(port);
    
    /*将套接字与服务器地址绑定*/
    err = bind(listenfd, (const struct sockaddr *) &serverAddr,
               sizeof(serverAddr));
    /*如果绑定失败*/
    if (err == INVALID_SOCKET)
    {
        ErrorPrint("Error: unable to bind socket\n");
        return 1;
    }
    
    /*开始侦听,设置等待连接的最大队列长度为SOMAXCONN，默认值为5个*/
    err = listen(listenfd, SOMAXCONN);
    /*如果侦听失败*/
    if (err == INVALID_SOCKET)
    {
        ErrorPrint("Error: listen failed\n");
        return 1;
    }
    
    LoopControl(listenfd, 1);
    printf("Server is down\n");
    /*释放Winscoket初始化时占用的资源*/
    WSACleanup();
    return 0;
}




