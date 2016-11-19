/*************************************************************************
	> File Name: ping.c
 	>	Author: 
	>	Mail:
 	>	Created Time: sam. 12 nov. 2016 19:16:40 CET
	>
*************************************************************************/
#include "ping.h"
int main(int argc, char * argv[]){
    struct hostent *host;
    struct icmp sendicmp;
    struct sockaddr_in from;
    struct sockaddr_in to;
    int fromlen = 0;
    int sockfd;
    int nsend = 0;
    int nreceived = 0;
    int i, n;
    in_addr_t inaddr;

    memset(&from, 0, sizeof(struct sockaddr_in));
    memset(&to, 0, sizeof(struct sockaddr_in));

	printf("Size of struct ip : %ld, size of struct icmp: %ld\n", sizeof(struct ip), sizeof(struct icmp) );
    if(argc < 2){
        printf("Usage : %s  hostname/IP address \n", argv[0]);
        exit(1);
    }

    // generate a origin socket
    if((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1){
        printf("socket() error \n");
        exit(1);
    }

    
    to.sin_family = AF_INET;

    // Detect argument if its an ip or a domain name
    if(inaddr = inet_addr(argv[1]) == INADDR_NONE){
        // domain name
        if((host = gethostbyname(argv[1])) == NULL){
            printf("gethostbyname() error \n");
            exit(1);
        }
        to.sin_addr = *(struct in_addr *)host->h_addr_list[0];
    }else{
        // IP address
        to.sin_addr.s_addr = inaddr;
    }

    // Output ip info
    printf("ping %s (%s) : %d bytes of data.\n", argv[1], inet_ntoa(to.sin_addr), (int)ICMP_SIZE);

    //Send socket and receive socket loop
    for(i = 0; i < NUM; i++){
        nsend++;  
        memset(&sendicmp, 0, ICMP_SIZE);
        icmpInit(&sendicmp, nsend);

        printf("Send socket index %d \n", nsend);
        if(sendto(sockfd, &sendicmp, ICMP_SIZE, 0, (struct sockaddr *)&to, sizeof(to)) == -1){
            printf("sendto() error \n");
            continue;
        }

        //n is nb of bits in buf return.
        if((n = recvfrom(sockfd, buf, BUF_SIZE, 0, (struct sockaddr *)&from, &fromlen)) < 0){
            printf("recvform() error \n");
            continue;
        }
        nreceived++;  
	    printf("Receive socket index : %d and unpack .\n", nreceived);
        if(unpack(buf, n, inet_ntoa(from.sin_addr)) == -1){
            printf("unpack() error \n");
        }

        sleep(1);
    }

    // Statistics info
    printf("---  %s ping statistics ---\n", argv[1]);
    printf("%d packets transmitted, %d received, %%%d packet loss\n", nsend, nreceived, 
            (nsend - nreceived) / nsend * 100);

    return 0;
}


USHORT checkSum(USHORT *addr, int len){
    UINT sum = 0;  
    while(len > 1){
        sum += *addr++;
        len -= 2;
    }

    if(len == 1){
        sum += *(UCHAR *)addr;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);

    return (USHORT) ~sum;
}


float timediff(struct timeval *begin, struct timeval *end){
    int n;

    n = ( end->tv_sec - begin->tv_sec ) * 1000000
        + ( end->tv_usec - begin->tv_usec );

    return (float) (n / 1000);
}


void icmpInit(struct icmp * icmp, int sequence){
    icmp->type = ICMP_ECHO;
    icmp->code = 0;
    icmp->checksum = 0;
    icmp->id = getpid();
    icmp->sequence = sequence;
    gettimeofday(&icmp->timestamp, 0);
    icmp->checksum = checkSum((USHORT *)icmp, ICMP_SIZE);
}

/**
 * buf  point to buffer msg
 * len  nb of bits return from recvfrom()
 * addr IP destination
 */ 
int unpack(char * buf, int len, char * addr){
   int ipheadlen;
   struct ip * ip;
   struct icmp * icmp;
   float rtt;          
   struct timeval end; 

   ip = (struct ip *)buf;

   // Calculate ip msg length, just ip->headlen x 4
   ipheadlen = ip->hlen << 2;
	printf("length of msg : %d \n", len);
   // pass to ip msg, its icmp 
   icmp = (struct icmp *)(buf + ipheadlen);

   //length of icmp msg
   len -= ipheadlen;

   if(len < 8){
        printf("ICMP packets\'s length is less than 8 \n"); 
        return -1;
   }

//Make sure it's the socket that we send to..
   if(/*icmp->type != ICMP_ECHOREPLY ||*/
           icmp->id != getpid()){    
       printf("ICMP packets are not send by us \n");
       return -1;
   }

   // Calculate time.
   gettimeofday(&end, 0);
   rtt = timediff(&icmp->timestamp, &end);

   printf("%d bytes from %s : icmp_seq=%u ttl=%d rtt=%fms \n",
           len, addr, icmp->sequence, ip->ttl, rtt);

   return 0;
}
