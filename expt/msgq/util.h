#ifndef __CLIENT_UTILS
#define __CLIENT_UTILS

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>

#define LENGTH 100
#define ERR -1

#define MSGQ_PATH "/Users/joyp.isahac/Documents/my_tests/net_prog/my_solns/assignment/first/expt/msgq/client.c"

enum role{SENDER, RECIEVER};

struct msgbuf {
    long mtype;
    char mtext[LENGTH];
};

typedef struct msgbuf MBuff;

int getMsgq();
void sendMsg(int id, MBuff msg);
void removeMsgq(int id);
void runtask(enum role r, int id);
int recieveMsg(int id, MBuff* msg);
enum role init();

#endif