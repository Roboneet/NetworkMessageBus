#include "utils.h"

void udp_listener(int msgid);
int send_msg_to_udp(MsgBuf msg);
void create_tcp(int msgq);
void handle_tcp(int connfd, int msgq);
int put_in_msg_queue(int msgid, MsgBuf buf);
int retrieve_msg_queue(int msgq, long type, MsgBuf* msgp);

int main(){
	int msgqid = msgget (IPC_PRIVATE, IPC_CREAT | 0644);
	if(msgqid < 0) die("msgget() failed");	

	pid_t p = fork();
	if(p < 0)
		die("fork() failed");
	else if(p == 0){
		udp_listener(msgqid);	
		exit(0);
	}
	create_tcp(msgqid);
	return 0;
}


int put_in_msg_queue(int msgid, MsgBuf buf){
	return msgsnd(msgid, &(buf.mtype), sizeof(buf.mtext), 0);
}

int retrieve_msg_queue(int msgq, long type, MsgBuf* msgp){
	return msgrcv(msgq, msgp, sizeof(msgp->mtext), type, 0);
}


void udp_listener(int msgid){
	printf("udp_listener(%d)\n", msgid);
	int sock = socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in recvaddr;
	recvaddr.sin_family = AF_INET;
	recvaddr.sin_addr.s_addr = INADDR_ANY;
	recvaddr.sin_port = htons(UDP_PORT);
	bind(sock,(struct sockaddr *)&recvaddr,sizeof(recvaddr));

	for (;;) { 
		MsgBuf buf;
		ssize_t t = recv(sock, &buf, sizeof(buf), 0);
		if (t < 0)
			die("recv() failed");
		if(t == 0){
			break;		
		}
		// put in queue
		printf("mtext: %s\n", buf.mtext);
		put_in_msg_queue(msgid, buf);
	}
	close(sock);
	return;
}

// Tcp receive from nmb client
void create_tcp(int msgq){
	printf("create_tcp(%d)\n", msgq);
	int connfd, sock;
	struct sockaddr_in listaddr, cliaddr;
	pid_t pid;
	unsigned int clilen;

	sock = socket(AF_INET,SOCK_STREAM,0);
	listaddr.sin_family = AF_INET;
	listaddr.sin_addr.s_addr = INADDR_ANY;
	listaddr.sin_port = htons(TCP_PORT);
	bind(sock,(struct sockaddr *)&listaddr,sizeof(listaddr));
	listen(sock,5);
	
	for(;;){
		clilen = sizeof (cliaddr);
		connfd = accept (sock, (struct sockaddr *) &cliaddr, &clilen);
		if(connfd< 0)die("accept() failed");
		
		if((pid =fork()) == 0){
			close(sock);
			handle_tcp(connfd, msgq);
			exit(0);
		}
		else if (pid < 0){die("fork() failed");}
		close(connfd);
	}
	close(sock);
}


int send_msg_to_udp(MsgBuf msg){
	int sock = socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in sendaddr;
	sendaddr.sin_family = AF_INET;
	int p;
	extract(msg.mtype, &(sendaddr.sin_addr.s_addr), &p);
	sendaddr.sin_port = htons(UDP_PORT);
	msg.mtype = (long)p;
	
	
	return sendto(sock, &(msg.mtype), sizeof(msg), 0, (struct sockaddr *)&sendaddr,sizeof(sendaddr));
}

//tcp single child connection handler
void handle_tcp(int connfd, int msgq){
	TcpCall tc;
	int t, action;
	long type;
	MsgBuf msg;

	for(;;){
		t = recv(connfd, &tc, sizeof(tc), 0);
		
		if(t < 0) die("recv() failed");
		if(t == 0) break; // connection closed
			
		// messnd_nmb
		action = tc.action;
		switch(action){
			case MSGSND:
				send_msg_to_udp(tc.msg);
				break;
			case MSGRCV:
				// check if msg is there
				type = tc.msg.mtype;
				t = retrieve_msg_queue(msgq, type, &msg);
			
				if( t < 0 ){
					die("retrieve_msg_queue >> msgrcv() failed");
				}
				send(connfd,&(msg.mtype),sizeof(msg), 0);
				break;
			default:
				printf("wrong action type\n");
		}
	}
	close (connfd);
	exit(0);
}








