/*加载库文件*/
#pragma comment( lib, "ws2_32.lib" )
/*加载头文件*/
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

/*定义多播常量*/
#define MCASTADDR     "224.3.5.8"
#define MCASTPORT     25000
#define BUFSIZE       1024
#define MCOUNT        10

/*定义广播常量*/
#define BCASTPORT     5050
#define BCOUNT        10

/*定义广播全局变量*/
SOCKET             socketBro;
SOCKET             socketRec;
struct sockaddr_in addrBro;
struct sockaddr_in addrRec;
BOOL               broadSendFlag;
BOOL               broadFlag;

DWORD              bCount;
DWORD              bcastAddr;
short              bPort;

/*定义多播全局变量*/
SOCKET             socketMul;
SOCKET             sockJoin;
struct sockaddr_in addrLocal;
struct sockaddr_in addrMul;

BOOL               multiSendFlag;
BOOL               bLoopBack;    
BOOL               multiFlag;

DWORD              dwInterface;  
DWORD              dwMulticastGroup;
DWORD              mCount;         
short              mPort;           

/*自定义函数*/
void initial();
void GetArgments(int argc, char **argv);

void userHelpAll();
void userHelpBro();
void userHelpMul();

void broadcastSend();
void broadcastRec();

void mulControl();
void multicastSend();
void multicastRec();

/*初始化全局变量函数*/
void initial()
{
    /*初始化广播全局变量*/
    bPort = BCASTPORT;
    bCount = BCOUNT;
    bcastAddr = INADDR_BROADCAST;
    broadSendFlag = FALSE;
    broadFlag = FALSE;
    multiFlag = FALSE;

    /*初始化多播全局变量*/
    dwInterface = INADDR_ANY;
    dwMulticastGroup = inet_addr(MCASTADDR);
    mPort = MCASTPORT;
    mCount = MCOUNT;
    multiSendFlag = FALSE;
    bLoopBack = FALSE;
}

/*参数获取函数*/
void GetArgments(int argc, char **argv)
{
    int i;
    /*如果参数个数小于2个*/
    if(argc<=1)
    {
        userHelpAll();
        return ;
    }
    /*获取广播选项*/
    if(argv[1][0]=='-'&&argv[1][1]=='b')
    {
        /*广播标志设置为真*/
        broadFlag = TRUE;
        for(i=2; i < argc ;i++)
    	{
            if (argv[i][0] == '-')
    		{
                switch (tolower(argv[i][1]))
        		{
                    /*如果是发送者*/
                	case 's': 
                        broadSendFlag = TRUE;
            			break;
                    /*广播的地址*/
                	case 'h':
                        if (strlen(argv[i]) > 3)
                            bcastAddr = inet_addr(&argv[i][3]);
            			break;
                    /*广播的端口号*/
                	case 'p':
                        if (strlen(argv[i]) > 3)
                            bPort = atoi(&argv[i][3]);
            			break;
                    /*广播(接收或者发送)的数量*/
                	case 'n': 
                        bCount = atoi(&argv[i][3]);
            			break;
                    /*其他情况显示用户帮助，终止程序*/
                	default:
        				{
                        	userHelpBro();
                        	ExitProcess(-1);
        				}
            			break;
        		}
    		}
    	}
        return ;
    }

    /*获取多播选项*/
    if(argv[1][0]=='-'&&argv[1][1]=='m')
    {
        /*多播标志设置为真*/
        multiFlag = TRUE;
        for(i=2; i < argc ;i++)
    	{
            if (argv[i][0] == '-')
    		{
                switch (tolower(argv[i][1]))
        		{
                    /*如果是发送者*/
                	case 's': 
                        multiSendFlag = TRUE;
            			break;
                    /*多播地址*/
                	case 'h': 
                        if (strlen(argv[i]) > 3)
                            dwMulticastGroup = inet_addr(&argv[i][3]);
            			break;
                    /*本地接口地址*/
                	case 'i': 
                        if (strlen(argv[i]) > 3)
                            dwInterface = inet_addr(&argv[i][3]);
            			break;
                    /*多播端口号*/
                	case 'p': 
                        if (strlen(argv[i]) > 3)
                            mPort = atoi(&argv[i][3]);
            			break;
                    /*环回标志设置为真*/
                	case 'l': 
                        bLoopBack = TRUE;
            			break;
                    /*发送(接收)的数量*/
                	case 'n':
                        mCount = atoi(&argv[i][3]);
            			break;
                    /*其他情况，显示用户帮助，终止程序*/
                	default:
                    	userHelpMul();
            			break;
        		}
    		}
    	}
    
    }
    return;
}

/*全局用户帮助函数*/
void userHelpAll()
{
    printf("Please choose broadcast[-b] or multicast[-m] !\n"); 
    printf("userHelpAll: -b [-s][p][-h][-n] | -m[-s][-h][-p][-i][-l][-n]\n");
    userHelpBro();
    userHelpMul();
}

/*广播用户帮助函数*/
void userHelpBro()
{
    printf("Broadcast: -b -s:str -p:int -h:str -n:int\n");
    printf("           -b     Start the broadcast program.\n");
    printf("           -s     Act as server (send data); otherwise\n");
    printf("                  receive data. Default is receiver.\n");
    printf("           -p:int Port number to use\n ");
    printf("                  The default port is 5050.\n");
    printf("           -h:str The decimal broadcast IP address.\n");
    printf("           -n:int The Number of messages to send/receive.\n");
    printf("                  The default number is 10.\n");
}

/*多播用户帮助函数*/
void userHelpMul()
{
    printf("Multicast: -m -s -h:str -p:int -i:str -l -n:int\n");
    printf("           -m     Start the multicast program.\n");
    printf("           -s      Act as server (send data); otherwise\n");
    printf("                   receive data. Default is receiver.\n");
    printf("           -h:str  The decimal multicast IP address to join\n");
    printf("                   The default group is: %s\n", MCASTADDR);
    printf("           -p:int  Port number to use\n");
    printf("                   The default port is: %d\n", MCASTPORT);
    printf("           -i:str  Local interface to bind to; by default \n");
    printf("                   use INADDRY_ANY\n");
    printf("           -l      Disable loopback\n");
    printf("           -n:int  Number of messages to send/receive\n");
    ExitProcess(-1);
}

/*广播消息发送函数*/
void broadcastSend()
{
    /*设置广播的消息*/
    char *smsg="The message received is from sender!";
    BOOL opt=TRUE;
    int nlen=sizeof(addrBro);
    int ret;
    DWORD i=0;
    
    /*创建UDP套接字*/
    socketBro=WSASocket(AF_INET,SOCK_DGRAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
    /*如果创建失败*/
    if(socketBro==INVALID_SOCKET)
    {
        printf("Create socket failed:%d\n",WSAGetLastError());
        WSACleanup();
        return;
    }
    
    /*设置广播地址各个选项*/
    addrBro.sin_family=AF_INET;
    addrBro.sin_addr.s_addr=bcastAddr;
    addrBro.sin_port=htons(bPort);
    
    /*设置该套接字为广播类型*/
    if (setsockopt(socketBro,SOL_SOCKET,SO_BROADCAST,(char FAR *)&opt,
                                        		sizeof(opt))==SOCKET_ERROR)
    /*如果设置失败*/
    {
        printf("setsockopt failed:%d",WSAGetLastError());
        closesocket(socketBro);
        WSACleanup();
        return;
    }
    /*循环发送消息*/
    while(i<bCount)
    {
        /*延迟1秒*/
        Sleep(1000);
        /*从广播地址发送消息*/
        ret=sendto(socketBro,smsg,256,0,(struct sockaddr*)&addrBro,nlen);
        /*如果发送失败*/
        if(ret==SOCKET_ERROR)
            printf("Send failed:%d",WSAGetLastError());
        /*如果发送成功*/
        else
        {		
            printf("Send message %d!\n",i);	
    	}
        i++;
    }
    /*发送完毕后关闭套接字、释放占用资源*/
    closesocket(socketBro);
    WSACleanup();
}

/*广播消息接收函数*/
void broadcastRec()
{   
    BOOL optval = TRUE;
    int addrBroLen;
    char buf[256];
    DWORD i=0;
    /*该地址用来绑定套接字*/
    addrRec.sin_family=AF_INET;
    addrRec.sin_addr.s_addr=0;
    addrRec.sin_port=htons(bPort);

    /*该地址用来接收网路上广播的消息*/
    addrBro.sin_family=AF_INET;
    addrBro.sin_addr.s_addr=bcastAddr;
    addrBro.sin_port=htons(bPort);
    
    addrBroLen=sizeof(addrBro);
    //创建UDP套接字
    socketRec=socket(AF_INET,SOCK_DGRAM,0);
    /*如果创建失败*/
    if(socketRec==INVALID_SOCKET)
    {
        printf("Create socket error:%d",WSAGetLastError());
        WSACleanup();
        return;
    }

    /*设置该套接字为可重用类型*/
    if(setsockopt(socketRec,SOL_SOCKET,SO_REUSEADDR,(char FAR *)&optval,
                                                			sizeof(optval))==SOCKET_ERROR)
    /*如果设置失败*/
    {
        printf("setsockopt failed:%d",WSAGetLastError());
        closesocket(socketRec);
        WSACleanup();
        return;
    }
    /*绑定套接字和地址*/
    if(bind(socketRec,(struct sockaddr *)&addrRec,
                                sizeof(struct sockaddr_in))==SOCKET_ERROR)
    /*如果绑定失败*/
    {
        printf("bind failed with: %d\n", WSAGetLastError());
        closesocket(socketRec);
        WSACleanup();
        return ;
    }
    /*从广播地址接收消息*/
    while(i<bCount)
    {
        recvfrom(socketRec,buf,256,0,(struct sockaddr FAR *)&addrBro,(int FAR *)&addrBroLen);
        /*延迟2秒钟*/
        Sleep(2000);
        /*输出接收到缓冲区的消息*/
        printf("%s\n",buf);
        /*情况缓冲区*/
        ZeroMemory(buf,256);
        i++;
    }
    /*接收完毕后关闭套接字、释放占用资源*/
    closesocket(socketRec);
    WSACleanup();
}

/*多播控制函数*/
void mulControl()
{
    int optval; 
    /*创建UDP套接字，用于多播*/
    if ((socketMul = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0,
                  WSA_FLAG_MULTIPOINT_C_LEAF 
                  | WSA_FLAG_MULTIPOINT_D_LEAF 
                  | WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
    {
        printf("socket failed with: %d\n", WSAGetLastError());
        WSACleanup();
        return ;
    }
    
    /*设置本地接口地址*/
    addrLocal.sin_family = AF_INET;
    addrLocal.sin_port = htons(mPort);
    addrLocal.sin_addr.s_addr = dwInterface;
    
    /*将UDP套接字绑定到本地地址上*/
    if (bind(socketMul, (struct sockaddr *)&addrLocal, 
                                    sizeof(addrLocal)) == SOCKET_ERROR)
    /*如果绑定失败*/
    {
        printf("bind failed with: %d\n", WSAGetLastError());
        closesocket(socketMul);
        WSACleanup();
        return ;
    }

    /*设置多播地址各个选项*/
    addrMul.sin_family      = AF_INET;
    addrMul.sin_port        = htons(mPort);
    addrMul.sin_addr.s_addr = dwMulticastGroup;

    /*重新设置TTL值*/
    optval = 8;
    /*设置多播数据的TTL(存在时间)值。默认情况下，TTL值是1*/
    if (setsockopt(socketMul, IPPROTO_IP, IP_MULTICAST_TTL, 
        (char *)&optval, sizeof(int)) == SOCKET_ERROR)
    /*如果设置失败*/
    {
        printf("setsockopt(IP_MULTICAST_TTL) failed: %d\n",WSAGetLastError());
        closesocket(socketMul);
        WSACleanup();
        return ;
    }

    /*如果指定了返还选项*/
    if (bLoopBack)
    {
        /*设置返还选项为假，禁止将发送的数据返还给本地接口*/
        optval = 0;
        if (setsockopt(socketMul, IPPROTO_IP, IP_MULTICAST_LOOP,
            (char *)&optval, sizeof(optval)) == SOCKET_ERROR)
        /*如果设置失败*/
    	{
            printf("setsockopt(IP_MULTICAST_LOOP) failed: %d\n",
                WSAGetLastError());
            closesocket(socketMul);
            WSACleanup();
            return ;
        }
    }
    
    /*加入多播组*/
    if ((sockJoin = WSAJoinLeaf(socketMul, (SOCKADDR *)&addrMul, 
                             sizeof(addrMul), NULL, NULL, NULL, NULL, 
                             JL_BOTH)) == INVALID_SOCKET)
    /*如果加入不成功*/
    {
        printf("WSAJoinLeaf() failed: %d\n", WSAGetLastError());
        closesocket(socketMul);
        WSACleanup();
        return ;
    }
}

/*多播消息发送函数*/
void multicastSend()
{
    
    TCHAR  sendbuf[BUFSIZE];
    DWORD i;
    int ret;

    mulControl();
    /*发送mCount条消息*/
    for(i = 0; i < mCount; i++)
    {
        /*将待发送的消息写入发送缓冲区*/
        sprintf(sendbuf, "server 1: This is a test: %d", i);
        ret=sendto(socketMul, (char *)sendbuf, strlen(sendbuf), 0,
                (struct sockaddr *)&addrMul, sizeof(addrMul));
        /*如果发送失败*/
        if(ret==SOCKET_ERROR)
        {
            printf("sendto failed with: %d\n",WSAGetLastError());
            closesocket(sockJoin);
            closesocket(socketMul);
            WSACleanup();
            return ;
        }
        /*如果发送成功*/
        else
            printf("Send message %d\n",i);
         Sleep(500);
     }
    /*关闭套接字、释放占用资源*/
    closesocket(socketMul);
    WSACleanup();
}

/*多播消息接收函数*/
void multicastRec()
{
    DWORD i;
    struct sockaddr_in  from;
    TCHAR recvbuf[BUFSIZE];
    int ret;
    int len = sizeof(struct sockaddr_in);

    mulControl();
    /*接收mCount条消息*/
    for(i = 0; i < mCount; i++)
    {
        /*将接收的消息写入接收缓冲区*/
        if ((ret = recvfrom(socketMul, recvbuf, BUFSIZE, 0,
                (struct sockaddr *)&from, &len)) == SOCKET_ERROR)
        /*如果接收不成功*/
    	{
            printf("recvfrom failed with: %d\n",WSAGetLastError());
            closesocket(sockJoin);
            closesocket(socketMul);
            WSACleanup();
             return ;
        }
        /*接收成功，输出接收的消息*/
        recvbuf[ret] = 0;
        printf("RECV: '%s' from <%s>\n", recvbuf,inet_ntoa(from.sin_addr));
     }
    /*关闭套接字、释放占用资源*/
    closesocket(socketMul);
    WSACleanup();
}

/*主函数*/
int main(int argc, char **argv)
{
    WSADATA wsd;

    initial();
    GetArgments(argc, argv);

    /*初始化Winsock*/
    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
    {
        printf("WSAStartup() failed\n");
        return -1;
    }

    /*如果是执行广播程序*/
    if(broadFlag)
    {
        /*以发送者身份发送消息*/
        if(broadSendFlag)
    	{
            broadcastSend();
            return 0;
    	}
        /*以接收者身份接收消息*/
        else
    	{
            broadcastRec();
            return 0;
    	}
    }

    /*如果是执行多播程序*/
    if(multiFlag)
    {
        /*以发送者身份发送消息*/
        if(multiSendFlag) 
    	{
            multicastSend();
            return 0;
        }
        /*以接收者身份接收消息*/
        else    
    	{
            multicastRec();
            return 0;
    	}
    }
    return 0;
}




