#include "util.h"

int main(){
	int msgq_id = getMsgq();
	printf("msgq_id: %d\n", msgq_id);
	enum role r = init();
	
	while(1){
		printf("role: %d\n", r);
		runtask(r, msgq_id);
		printf("role iiii: %d\n", r);
	}
	printf("I'm out! %d", r);
    removeMsgq(msgq_id);
    return 0;
}

enum role init(){
	pid_t pid;
	if ((pid = fork ()) < 0)
    {
      perror ("fork error");
    }
    if(pid  == 0){
    	return SENDER;
    }
    return RECIEVER;
}

void runtask(enum role r, int id){
	MBuff msg;
	if(r == SENDER){
		msg.mtype = 1;
      	gets(msg.mtext);
      	sendMsg(id, msg);
	}else{
		if(recieveMsg(id, &msg) == 1){
			printf("recieved text: %s\n", msg.mtext);
		}
	}
}

int getMsgq(){
  	key_t key = ftok (MSGQ_PATH, 'B');
  	if(key == ERR){
  		perror("ftok");
  		exit(0);
  	}
  	int id = msgget (key, 0644 | IPC_CREAT);
  	if(id == ERR){
  		perror("msgget");
  		exit(0);
  	}
  	return id;
}

void sendMsg(int id, MBuff msg){
	if (msgsnd (id, &(msg.mtype), sizeof (msg), IPC_NOWAIT) == ERR){
		perror ("msgsnd");
    }
}

int recieveMsg(int id, MBuff* msg){
	msg->mtype = 1;
	if (msgrcv (id, &(msg->mtype), sizeof (*msg), 0, IPC_NOWAIT) != ERR)
		return 1;
	return 0;
}

void removeMsgq(int id){
	if (msgctl (id, IPC_RMID, NULL) == ERR)
    {
      perror ("msgctl");
      exit (1);
    }
}