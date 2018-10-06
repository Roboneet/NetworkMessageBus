#ifndef __NMB_UTILS
#define __NMB_UTILS

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h>  

#include <string.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h> 

#define TCP_PORT 1111
#define UDP_PORT 1112
#define RCVBUFSIZE 32   

#define MSGSND 1
#define MSGRCV 2

#define LENGTH 100

struct msgbuf {
    long mtype;    
    char mtext[LENGTH];  
};

typedef struct msgbuf MsgBuf;

struct tcp_call{
	int action;
	struct msgbuf msg;
};

typedef struct tcp_call TcpCall;
typedef int nmb_t;

nmb_t msgget_nmb();
int msgsnd_nmb(nmb_t nmbid, MsgBuf msg, 
	size_t msgsz, int msgflg);
ssize_t msgrcv_nmb(nmb_t nmbid, MsgBuf* msgp,
    size_t msgsz, long msgtyp, int msgflg);
int msgrem_nmb(nmb_t nmbid);

void die(char* str);
MsgBuf dummyMsg();
void ip_to_string(uint32_t addr, char* str);
long get_mtype(char* ip, int port);
void extract(long type, uint32_t* ip, int* port);
void get_my_port(nmb_t nmbid, int* p);

#endif
