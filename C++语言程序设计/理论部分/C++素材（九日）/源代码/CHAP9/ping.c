/*导入库文件*/
#pragma comment( lib, "ws2_32.lib" )
/*加载头文件*/
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*定义常量*/
/*表示要记录路由*/
#define IP_RECORD_ROUTE  0x7
/*默认数据报大小*/
#define DEF_PACKET_SIZE  32 
/*最大的ICMP数据报大小*/
#define MAX_PACKET       1024    
/*最大IP头长度*/
#define MAX_IP_HDR_SIZE  60 
/*ICMP报文类型，回显请求*/      
#define ICMP_ECHO        8
/*ICMP报文类型，回显应答*/ 
#define ICMP_ECHOREPLY   0
/*最小的ICMP数据报大小*/
#define ICMP_MIN         8

/*自定义函数原型*/
void InitPing();
void UserHelp();

void GetArgments(int argc, char** argv); 
USHORT CheckSum(USHORT *buffer, int size);
void FillICMPData(char *icmp_data, int datasize);
void FreeRes();

void DecodeIPOptions(char *buf, int bytes);
void DecodeICMPHeader(char *buf, int bytes, SOCKADDR_IN* from);

void PingTest(int timeout);



/*IP报头字段数据结构*/
typedef struct _iphdr 
{
    unsigned int   h_len:4;        /*IP报头长度*/
    unsigned int   version:4;      /*IP的版本号*/
    unsigned char  tos;            /*服务的类型*/
    unsigned short total_len;      /*数据报总长度*/
    unsigned short ident;          /*惟一的标识符*/
    unsigned short frag_flags;     /*分段标志*/
    unsigned char  ttl;            /*生存期*/
    unsigned char  proto;          /*协议类型(TCP、UDP等)*/
    unsigned short checksum;       /*校验和*/
    unsigned int   sourceIP;       /*源IP地址*/
    unsigned int   destIP;         /*目的IP地址*/
} IpHeader;

/*ICMP报头字段数据结构*/
typedef struct _icmphdr 
{
    BYTE   i_type;				   /*ICMP报文类型*/
    BYTE   i_code;                 /*该类型中的代码号*/
    USHORT i_cksum;                /*校验和*/ 
    USHORT i_id;                   /*惟一的标识符*/
    USHORT i_seq;                  /*序列号*/
    ULONG  timestamp;              /*时间戳*/  
} IcmpHeader;

/*IP选项头字段数据结构*/
typedef struct _ipoptionhdr
{
   
	unsigned char  code;           /*选项类型*/
    unsigned char  len;            /*选项头长度*/
    unsigned char  ptr;            /*地址偏移长度*/
	unsigned long  addr[9];        /*记录的IP地址列表*/
} IpOptionHeader;

/*定义全局变量*/
SOCKET m_socket;
IpOptionHeader IpOption;
SOCKADDR_IN DestAddr;
SOCKADDR_IN SourceAddr;
char *icmp_data;
char *recvbuf;
USHORT seq_no ;
char *lpdest;
int datasize;
BOOL RecordFlag;
double PacketNum;
BOOL SucessFlag;


/*初始化变量函数*/   
void InitPing()
{
  WSADATA wsaData;
  icmp_data = NULL;
  seq_no = 0;
  recvbuf = NULL;
  RecordFlag = FALSE;
  lpdest = NULL;
  datasize = DEF_PACKET_SIZE;
  PacketNum = 5;
  SucessFlag = FALSE;

  /*Winsock初始化*/
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        /*如果初始化不成功则报错，GetLastError()返回发生的错误信息*/
		printf("WSAStartup() failed: %d\n", GetLastError());
        return ;
    }
  m_socket = INVALID_SOCKET;
}

/*显示信息函数*/
void UserHelp()
{
	printf("UserHelp: ping -r <host> [data size]\n");
    printf("          -r           record route\n");
	printf("          -n           record amount\n");
    printf("          host         remote machine to ping\n");
    printf("          datasize     can be up to 1KB\n");
    ExitProcess(-1); 
}


/*获取ping选项函数*/
void GetArgments(int argc,char** argv)
{
   int i;
   int j;
   int exp;
   int len;
   int m;
   /*如果没有指定目的地地址和任何选项*/
   if(argc == 1)
	{
		printf("\nPlease specify the destination IP address and the ping option as follow!\n");
	    UserHelp();
	}

	for(i = 1; i < argc; i++)
    {
        len = strlen(argv[i]);
		if (argv[i][0] == '-')
        {
            /*选项指示要获取记录的条数*/
			if(isdigit(argv[i][1]))
			{
				PacketNum = 0;
				for(j=len-1,exp=0;j>=1;j--,exp++)
					/*根据argv[i][j]中的ASCII值计算要获取的记录条数(十进制数)*/
					PacketNum += ((double)(argv[i][j]-48))*pow(10,exp);
			}
			else
			{
				switch (tolower(argv[i][1]))
				{
					/*选项指示要获取路由信息*/
					case 'r':        
						RecordFlag = TRUE;
						break;
					/*没有按要求提供选项*/
					default:
						UserHelp();
						break;
				}
			}
        }
		/*参数是数据报大小或者IP地址*/
        else if (isdigit(argv[i][0]))
		{
			for(m=1;m<len;m++)
			{
				if(!(isdigit(argv[i][m])))
				{
					/*是IP地址*/
					lpdest = argv[i];
					break;
				}
				/*是数据报大小*/
				else if(m==len-1)
					datasize = atoi(argv[i]);				 
			}
		}
		/*参数是主机名*/
        else
            lpdest = argv[i];
    }
}

/*求校验和函数*/
USHORT CheckSum(USHORT *buffer, int size)
{
	unsigned long cksum=0;
    while (size > 1) 
    {
        cksum += *buffer++;
        size -= sizeof(USHORT);
    }
    if (size) 
    {
        cksum += *(UCHAR*)buffer;
    }
	/*对每个16bit进行二进制反码求和*/
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >>16);
    return (USHORT)(~cksum);
}


/*填充ICMP数据报字段函数*/
void FillICMPData(char *icmp_data, int datasize)
{
    IcmpHeader *icmp_hdr = NULL;
    char      *datapart = NULL;

    icmp_hdr = (IcmpHeader*)icmp_data;
	/*ICMP报文类型设置为回显请求*/
    icmp_hdr->i_type = ICMP_ECHO;        
    icmp_hdr->i_code = 0;
	/*获取当前进程IP作为标识符*/
    icmp_hdr->i_id = (USHORT)GetCurrentProcessId();
    icmp_hdr->i_cksum = 0;
    icmp_hdr->i_seq = 0;
	datapart = icmp_data + sizeof(IcmpHeader);
	/*以数字0填充剩余空间*/
	memset(datapart,'0',datasize-sizeof(IcmpHeader));
}

/*释放资源函数*/
void FreeRes()
{
	/*关闭创建的套接字*/
	if (m_socket != INVALID_SOCKET) 
        closesocket(m_socket);
	/*释放分配的内存*/
    HeapFree(GetProcessHeap(), 0, recvbuf);
    HeapFree(GetProcessHeap(), 0, icmp_data);
	/*注销WSAStartup()调用*/
    WSACleanup();
    return ;
}


/*解读IP选项头函数*/
void DecodeIPOptions(char *buf, int bytes)
{
	IpOptionHeader *ipopt = NULL;
    IN_ADDR inaddr;
    int i;
    HOSTENT *host = NULL;
	/*获取路由信息的地址入口*/
    ipopt = (IpOptionHeader *)(buf + 20);

    printf("RR:   ");
    for(i = 0; i < (ipopt->ptr / 4) - 1; i++)
    {
        inaddr.S_un.S_addr = ipopt->addr[i];
        if (i != 0)
            printf("      ");
		/*根据IP地址获取主机名*/
        host = gethostbyaddr((char *)&inaddr.S_un.S_addr,sizeof(inaddr.S_un.S_addr), AF_INET);
        /*如果获取到了主机名，则输出主机名*/
		if (host)
            printf("(%-15s) %s\n", inet_ntoa(inaddr), host->h_name);
        /*否则输出IP地址*/
		else
            printf("(%-15s)\n", inet_ntoa(inaddr));
    }
    return;
}

/*解读ICMP报头函数*/
void DecodeICMPHeader(char *buf, int bytes, SOCKADDR_IN *from)
{
	IpHeader *iphdr = NULL;
    IcmpHeader *icmphdr = NULL;
    unsigned short iphdrlen;
    DWORD tick;
    static int icmpcount = 0;

    iphdr = (IpHeader *)buf;
    /*计算IP报头的长度*/
    iphdrlen = iphdr->h_len * 4;
    tick = GetTickCount();
	
	/*如果IP报头的长度为最大长度(基本长度是20字节)，则认为有IP选项，需要解读IP选项*/
    if ((iphdrlen == MAX_IP_HDR_SIZE) && (!icmpcount))
		/*解读IP选项，即路由信息*/
        DecodeIPOptions(buf, bytes);

	/*如果读取的数据太小*/
    if (bytes < iphdrlen + ICMP_MIN) 
    {
        printf("Too few bytes from %s\n", 
            inet_ntoa(from->sin_addr));
    }
    icmphdr = (IcmpHeader*)(buf + iphdrlen);

	/*如果收到的不是回显应答报文则报错*/
    if (icmphdr->i_type != ICMP_ECHOREPLY) 
    {
        printf("nonecho type %d recvd\n", icmphdr->i_type);
        return;
    }
    /*核实收到的ID号和发送的是否一致*/
    if (icmphdr->i_id != (USHORT)GetCurrentProcessId()) 
    {
        printf("someone else's packet!\n");
        return ;
    }
	SucessFlag = TRUE;
	/*输出记录信息*/
    printf("%d bytes from %s:", bytes, inet_ntoa(from->sin_addr));
    printf(" icmp_seq = %d. ", icmphdr->i_seq);
    printf(" time: %d ms", tick - icmphdr->timestamp);
    printf("\n");

    icmpcount++;
    return;
}

/*ping函数*/
void PingTest(int timeout)
{   
	int ret;
	int readNum;
	int fromlen;
	struct hostent *hp = NULL;
	/*创建原始套接字，该套接字用于ICMP协议*/
	m_socket = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0,WSA_FLAG_OVERLAPPED);
	/*如果套接字创建不成功*/
    if (m_socket == INVALID_SOCKET) 
    {
        printf("WSASocket() failed: %d\n", WSAGetLastError());
        return ;
    }

	/*若要求记录路由选项*/
    if (RecordFlag)
    {
        /*IP选项每个字段用0初始化*/
        ZeroMemory(&IpOption, sizeof(IpOption));
		/*为每个ICMP包设置路由选项*/
        IpOption.code = IP_RECORD_ROUTE; 
        IpOption.ptr  = 4;               
        IpOption.len  = 39;              
  
        ret = setsockopt(m_socket, IPPROTO_IP, IP_OPTIONS,(char *)&IpOption, sizeof(IpOption));
        if (ret == SOCKET_ERROR)
        {
            printf("setsockopt(IP_OPTIONS) failed: %d\n",WSAGetLastError());
        }
    }

    /*设置接收的超时值*/
    readNum = setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO,(char*)&timeout, sizeof(timeout));
    if(readNum == SOCKET_ERROR) 
    {
        printf("setsockopt(SO_RCVTIMEO) failed: %d\n",WSAGetLastError());
        return ;
    }
	/*设置发送的超时值*/
    timeout = 1000;
    readNum = setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO,(char*)&timeout, sizeof(timeout));
    if (readNum == SOCKET_ERROR) 
    {
        printf("setsockopt(SO_SNDTIMEO) failed: %d\n",WSAGetLastError());
        return ;
    }

	/*用0初始化目的地地址*/
    memset(&DestAddr, 0, sizeof(DestAddr));
	/*设置地址族，这里表示使用IP地址族*/
	DestAddr.sin_family = AF_INET;
    if ((DestAddr.sin_addr.s_addr = inet_addr(lpdest)) == INADDR_NONE)
    {   
        	
        /*名字解析，根据主机名获取IP地址*/
		if ((hp = gethostbyname(lpdest)) != NULL)
        {
            /*将获取到的IP值赋给目的地地址中的相应字段*/
			memcpy(&(DestAddr.sin_addr), hp->h_addr, hp->h_length);
			/*将获取到的地址族值赋给目的地地址中的相应字段*/
            DestAddr.sin_family = hp->h_addrtype;
            printf("DestAddr.sin_addr = %s\n", inet_ntoa(DestAddr.sin_addr));
        }
        /*获取不成功*/
		else
        {
            printf("gethostbyname() failed: %d\n",WSAGetLastError());
            return ;
        }
    }        

	/*数据报文大小需要包含ICMP报头*/
    datasize += sizeof(IcmpHeader);  
	/*根据默认堆句柄，从堆中分配MAX_PACKET内存块，新分配内存的内容将被初始化为0*/
    icmp_data =(char*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,MAX_PACKET);
    recvbuf =(char*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,MAX_PACKET);

	/*如果分配内存不成功*/
    if (!icmp_data) 
    {
        printf("HeapAlloc() failed: %d\n", GetLastError());
        return ;
    }
	/* 创建ICMP报文*/
    memset(icmp_data,0,MAX_PACKET);
    FillICMPData(icmp_data,datasize);

    
    while(1) 
    {
        static int nCount = 0;
        int writeNum;
        /*超过指定的记录条数则退出*/        
        if (nCount++ == PacketNum) 
            break;

         /*计算校验和前要把校验和字段设置为0*/       
        ((IcmpHeader*)icmp_data)->i_cksum = 0;
		/*获取操作系统启动到现在所经过的毫秒数，设置时间戳*/
        ((IcmpHeader*)icmp_data)->timestamp = GetTickCount();
		/*设置序列号*/
        ((IcmpHeader*)icmp_data)->i_seq = seq_no++;
		/*计算校验和*/
        ((IcmpHeader*)icmp_data)->i_cksum = CheckSum((USHORT*)icmp_data, datasize);
		/*开始发送ICMP请求 */
        writeNum = sendto(m_socket, icmp_data, datasize, 0,(struct sockaddr*)&DestAddr, sizeof(DestAddr));
        
		/*如果发送不成功*/
		if (writeNum == SOCKET_ERROR)
        {
            /*如果是由于超时不成功*/
			if (WSAGetLastError() == WSAETIMEDOUT) 
            {
                printf("timed out\n");
				continue;
            }
			/*其他发送不成功原因*/
            printf("sendto() failed: %d\n", WSAGetLastError());
            return ;
        }

		/*开始接收ICMP应答 */
        fromlen = sizeof(SourceAddr);
        readNum = recvfrom(m_socket, recvbuf, MAX_PACKET, 0,(struct sockaddr*)&SourceAddr, &fromlen);
        /*如果接收不成功*/
		if (readNum == SOCKET_ERROR)
        {
            /*如果是由于超时不成功*/
			if (WSAGetLastError() == WSAETIMEDOUT) 
            {
                printf("timed out\n");
				continue;
            }
			/*其他接收不成功原因*/
            printf("recvfrom() failed: %d\n", WSAGetLastError());
            return ;
        }
		/*解读接收到的ICMP数据报*/
        DecodeICMPHeader(recvbuf, readNum, &SourceAddr);
    }
}


int main(int argc, char* argv[])
{   
    InitPing();  
    GetArgments(argc, argv); 
    PingTest(1000);
	/*延迟1秒*/
    Sleep(1000);
	if(SucessFlag)
		printf("\nPing end, you have got %.0f records!\n",PacketNum);
	else
		printf("Ping end, no record!");	
    FreeRes();
    getchar();
    return 0;
    
}
