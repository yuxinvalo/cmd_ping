/*************************************************************************
	> File Name: ping.c
	> Author: 
	> Mail: 
	> Created Time: sam. 12 nov. 2016 19:16:40 CET
    > Model:
    >PING 8.8.8.8 (8.8.8.8) 56(84) bytes of data.
    >64 bytes from 8.8.8.8: icmp_seq=1 ttl=57 time=3.99 ms
 ************************************************************************/

#include "ping.h"
//=============Error Handle============================
enum err{argErr, sockErr, getHostNameErr, sendErr, recErr, unpErr};
void printErr(int flag)
{
    switch (flag)
    {
        case argErr:
            puts("Arguments number error.");
            puts("Usage: ping www.xxxx.com or IP addr.");
            break;
        case sockErr:
            puts("Socket init error.");
            break;
        case getHostNameErr:
            puts("Get host name error, can't find it.");
            break;
        case sendErr:
            puts("sendto() socket error, continue.");
            break;
        case recErr:
            puts("recvfrom() socket error, continue.");
            break;
        case unpErr:
            puts("unpack() error, continue.");
            break;
        default: puts("Error inexpected!");

    }
}


//=============CHECKSUM================================
unsigned short checksum(unsigned short* addr, int len_struct)
{
    unsigned int sumRe = 0;
    for (len_struct; len_struct > 1; len_struct-=2)
    {
        sumRe = sumRe + *addr++;
    }
    
    if (len_struct == 1)
    {
        sumRe = sumRe + *(unsigned char *)addr;
    }

    sumRe = (sumRe >> 16) + (sumRe & 0xffff);
    sumRe = sumRe + (sumRe >> 16);
    return (unsigned short) ~sumRe;
}

//===========Time back and forth====================== 
float timediff(struct timeval* begin, struct timeval* end)
{
    long int diff = 0;
    //unit: usec, sec = usec * 1000000
    diff = (end->tv_sec - begin->tv_sec) * 1000000 + 
            (end->tv_usec - begin->tv_usec);
    return (float)(diff / 1000);
}

//============Pack info and unpack info===============
void package(struct icmp* stIcmp, int index)
{
    stIcmp->type = ICMP_SEND;
    stIcmp->code = 0;
    stIcmp->checksum = 0;
    stIcmp->id = getpid();
    stIcmp->sequence = index;
    gettimofday(&stIcmp->timestamp, 0);
    stIcmp->checksum = checksum((unsigned short*)stIcmp, 
                                sizeof(stIcmp));
    
}

int unpackage(char* bufRec, int len_struct, char* addr)
{
    int re = -1;
    return re;
}

/*Check if argument 1 is an IP addr or a domain Name*/ 
int checkType(char *argv)
{

    //0->IP, 1 DNS
    int type = 0;
    in_addr_t inAddr;
    /*该函数的作用是将用点字符串格式表示的IP地址转换成32big endian
     * 失败时返回INADDR_NONE
    */
    if ((inAddr = inet_addr(argv)) != INADDR_NONE)
    {
        printf("IP addr detected.\n");
        return type;
    } 
    else
    {
        printf("domain name.\n");
        type = 1;
    }
    return type;
}


int main(int argc, char* argv[])
{
    struct hostent *host;
    struct icmp sendIcmp;

    struct sockaddr_in from;
    struct sockaddr_in to;

    int sockFd = 0;

    char buf[BUF_SIZE];

    memset(&from, '\0', sizeof(struct sockaddr_in));
    memset(&to, '\0', sizeof(struct sockaddr_in));

    printf("Start to ping %s....\n", argv[1]);
    if(argc != 2)
    {
        printErr(argErr);
        exit(1);
    }

    /*Create socket and init
     *- We use an origin socket-> SOCK_RAW
     *- We create our ICMP package and send it
     */
    if ((sockFd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
    {
        printErr(sockErr);
        exit(1);
    }

    to.sin_family = AF_INET;

    //check domain name or ip addr
    if ((host = gethostbyname(argv[1]) == NULL))
    {
        printErr(getHostNameErr);
        exit(1);
    }
    int type = checkType(argv[1]);
    if (type == 0)//ip addr 
    {
        to.sin_addr.s_addr = inet_addr(argv[1]);
    }
    else //host name 
    {
        host = gethostbyname(argv[1]);
        for (int i = 0; host->h_addr_list[i]; ++i)
        {
            printf("IP addr list  of domain:%s, %lu bytes data. \n", 
                   inet_ntoa(*(struct in_addr *)host->h_addr_list[i]),
                   sizeof(struct icmp));
        }  
    }
    printf("Start to ping : %s, %lu bytes data \n...",
        inet_ntoa(*(struct in_addr *)host->h_addr_list[0]),
         sizeof(struct icmp));
    
    int nbRe = 0;
    int nbSend = 1;
    int nbRecv = 1;
    for (int i = 0 ; i < NUM_ICMP; ++i)
    {
        memset(&sendIcmp, '\0', sizeof(struct icmp));
        package(&sendIcmp, nbRe + 1);
        printf("send socket index: %d", nbSend + 1); 

        //Send ping package
        nbSend++;
        if((nbRe = sendto(sockFd, &sendIcmp, sizeof(struct icmp), 0, (struct sockaddr_in*)&to, sizeof(to)) == -1)){
        printErr(sendErr);
        continue;
        }

        //Receive ping package
        nbRecv++;
        if((nbRe = recvfrom(sockFd, &sendIcmp, sizeof(struct icmp), 0,
            (struct sockaddr_in*)&from, sizeof(to))) == -1)
        {
            printErr(recErr);
            continue;
        }
        printf("receive socket index: %d, and unpack...", nbRecv + 1); 
        if(unpackage(buf, nbRe, inet_ntoa(from.sin_addr)) == -1)
        {
            printErr(unpErr);
        } 
        sleep(0.5);
    }


    return 0;

}
