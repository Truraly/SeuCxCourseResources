/*导入库文件*/
#pragma comment(lib,"wsock32.lib")
/*加载头文件*/
#include <stdio.h>
#include <winsock2.h>

/*自定义函数*/
int InitSockets(void);

void GetArgument(int argc, char **argv);
void ErrorPrint(x);
void userHelp();

/*定义全局变量*/
unsigned short port;
char *hostName;

/*初始化Winsock函数*/
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
    printf("userHelp:  -h:str -p:int\n");
    printf("           -h:str  The host name \n");
    printf("           -p:int  The Port number to use\n");
    ExitProcess(-1);
}

/*主函数*/
int main(int argc, char **argv)
{
    SOCKET clientfd;
    int err;
    struct sockaddr_in serverAddr;
    struct hostent *ptrHost;
    char response[4096];
    char *msg = "HELLO SERVER";
	GetArgments(argc, argv);
    if (argc != 3) 
	{
      userHelp();
      return 1;
    }
	GetArgments(argc,argv);
    InitSockets();
	/*创建套接字*/
    clientfd = socket(PF_INET, SOCK_STREAM, 0);
	/*如果创建失败*/
    if (clientfd == INVALID_SOCKET)
	{
        ErrorPrint("no more socket resources");
		return 1;
	}
	/*根据IP地址解析主机名*/
    if (atoi(hostName))
	{
        u_long ip_addr = inet_addr(hostName);
        ptrHost = gethostbyaddr((char *)&ip_addr,
            sizeof(u_long), AF_INET);
    }
	/*根据主机名解析IP地址*/
    else
        ptrHost = gethostbyname(hostName);

	/*如果解析失败*/
    if (!ptrHost)
	{
        ErrorPrint("cannot resolve hostname");
		return 1;
	}

    /*设置服务器端地址选项*/
	serverAddr.sin_family = PF_INET;
    memcpy((char *) &(serverAddr.sin_addr),
        ptrHost->h_addr,ptrHost->h_length);
    serverAddr.sin_port = htons(port);

	/*连接服务器*/
    err = connect(clientfd, (struct sockaddr *) &serverAddr,
        sizeof(serverAddr));
	/*连接失败*/
    if (err == INVALID_SOCKET)
	{
		ErrorPrint("cannot connect to server");
		return 1;
	}

	/*连接成功后，输出信息*/
    printf("You are connected to the server\n");
	/*发送消息到服务器端*/
    send (clientfd, msg, strlen(msg)+1, 0);
    memset(response, 0, sizeof(response));
	/*接收来自服务器端的消息*/
    recv(clientfd, response, sizeof(response), 0);
    printf("server says %s\n", response);
	/*关闭套接字*/
    closesocket(clientfd);
	/*释放Winscoket初始化时占用的资源*/
    WSACleanup();
    return 0;
}


