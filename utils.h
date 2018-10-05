#ifndef __NMB_UTILS
#define __NMB_UTILS

#include <stdio.h>
#include <stdlib.h>


#include <sys/socket.h> 
#include <arpa/inet.h>  

#include <string.h>     
#include <unistd.h> 

#define TCP_PORT 1111
#define RCVBUFSIZE 32   

#define MSGSND 1
#define MSGRCV 2
#define MSGCLS 3

struct msgbuf {
    long mtype;    
    char mtext[100];  
};

struct tcp_call{
	int action;
	struct msgbuf msg;
}

typedef nmb_t int;

nmb_t msgget_nmb();
int msgsnd_nmb(nmb_t nmbid, const void *msgp, 
	size_t msgsz, int msgflg);
ssize_t msgrcv_nmb(nmb_t nmbid, void *msgp,
	size_t msgsz, long msgtyp, int msgflg);
int msgctl_nmb(nmb_t nmbid, int cmd,
	struct msqid_ds *buf);

void die(char* str);
struct msgbuf dummyMsg();

#endif
