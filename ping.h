# ifndef _PING_H_
# define _PING_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>

#define ICMP_SIZE (sizeof(struct icmp))
#define ICMP_ECHO 8
#define BUF_SIZE 1024
#define NUM   5   

#define UCHAR  unsigned char
#define USHORT unsigned short
#define UINT   unsigned int 

//ICMP structure
struct icmp{
    UCHAR           type;      
    UCHAR           code;      
    USHORT          checksum;  
    USHORT          id;        
    USHORT          sequence;  
    struct timeval  timestamp; 
};



// IP structure
struct ip{
    // Check big endian or little endian
    #if __BYTE_ORDER == __LITTLE_ENDIAN
    UCHAR   hlen:4;       
    UCHAR   version:4;     
    #endif
    #if __BYTE_ORDER == __BIG_ENDIAN
    UCHAR   version:4;       
    UCHAR   hlen:4;    
    #endif    
    UCHAR   tos;             
    USHORT  len;             
    USHORT  id;                
    USHORT  offset;            
    UCHAR   ttl;            
    UCHAR   protocol;     
    USHORT  checksum;     
    struct in_addr ipSrc;    
    struct in_addr ipDst;   
};


char buf[BUF_SIZE] = {0};

USHORT checkSum(USHORT *, int); // Calculate checksum
float timediff(struct timeval *, struct timeval *); 
void icmpInit(struct icmp *, int);  
int unpack(char *, int, char *);   

#endif /*_PING_H_*/
