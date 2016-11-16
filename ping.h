/*************************************************************************
	> File Name: ping.h
	> Author: 
	> Mail: 
	> Created Time: sam. 12 nov. 2016 18:54:34 CET
 ************************************************************************/

# ifndef _PING_H
# define _PING_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>

# define NUM_ICMP 6;
# define BUF_SIZE 1026
struct icmp
{
    unsigned char type; // unsigned char 8 bit
    unsigned char code;
        
    unsigned short checksum; // unsigned short 16 bit
    unsigned short id;
    unsigned short sequence;
    struct timeval timestamp;
};

struct ip
{
    unsigned char version:4;
    unsigned char headLen:4;

    unsigned char typeServ;
    unsigned short len;
    unsigned short id;          //identifier 标识符
    unsigned short offset;      //id and offset
    unsigned char ttl;
    unsigned char ptcl;         //protocol
    unsigned short checksum;
    struct in_addr ipv4Src;
    struct in_addr ipv4Dst;
};


unsigned short checksum(unsigned short*, int);
float timediff(struct timeval *, struct timeval *);
void package(struct icmp*, int);
int unpack(char *, char *);


#endif /*_PING_H_*/
