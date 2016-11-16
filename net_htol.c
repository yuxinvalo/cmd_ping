/*************************************************************************
	> File Name: net_htol.c
	> Author: 
	> Mail: 
	> Created Time: sam. 12 nov. 2016 18:48:09 CET
 ************************************************************************/

#include <stdio.h>
#include <arpa/inet.h>

int main(void){
        unsigned short hosts = 0x1234;
        unsigned short nets;
        unsigned long  hostl = 0x12345678;
        unsigned long  netl; 

        nets = htons(hosts);
        netl = htonl(hostl);

        printf("Host ordered short: %#x \n", hosts);
        printf("Network ordered short: %#x \n", nets);

        printf("Host ordered long: %#lx \n", hostl);
        printf("Network ordered long: %#lx \n", netl);

        return 0;

}
