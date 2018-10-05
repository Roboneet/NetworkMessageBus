#include <"utils.h">

int main(){
	int msqid = msgget (IPC_PRIVATE, IPC_CREAT | 0644);
	
	if(fork() == 0){
		udp_listener(msgid);
		exit(0);
	}

	create_tcp(1111);
}

udp_listener(int msgid){
	sock = socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in recvaddr;
	recvaddr.sin_family = AF_INET;
	recvaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	recvaddr.sin_port = htons(atoi(port));
	bind(sock,(struct sockaddr *)&recvaddr,sizeof(recvaddr));

	for (;;) { // Run forever
	// Block until receive message from a client
	msgbuf buf; // I/O buffer
	// Size of received message
	ssize_t numBytesRcvd = recv(sock, &buf, sizeof(buf), 0);
	if (numBytesRcvd < 0)
	  die("recv() failed");
	// put in queue
	put_in_msg_queue(msgid,buf);
	}
}


send_msg_to_udp(msg){
	sock = socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in sendaddr;
	sendaddr.sin_family = AF_INET;
	sendaddr.sin_addr.s_addr = inet_addr(extract_ip(msg.mtype));
	sendaddr.sin_port = htons(1112);
	sendto(sock,msg,sizeof(msg),0,(struct sockaddr *)&sendaddr,sizeof(sendaddr));
}

// Tcp receive from nmb client
create_tcp(port){

	sock = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in listaddr;
  	listaddr.sin_family = AF_INET;
	listaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	listaddr.sin_port = htons(atoi(port));
	bind(sock,(struct sockaddr *)&listaddr,sizeof(listaddr));
	// Blocking
	listen(sock,5);
	int confd;

	struct sockaddr_in cliaddr
	for(;;){
		clilen = sizeof (cliaddr);
		
		connfd = accept (sock, (struct sockaddr *) &cliaddr, &clilen);
		if(confd< 0)die("accept() failed");
		pid_t pid;
		if((pid =fork()) == 0){
			close(sock);
			handle_tcp(connfd);
			exit(0);
		}
		else if (pid < 0){die("fork() failed");}
		close(connfd);
	}

}


//tcp single child connection handler
handle_tcp(connfd){
	for(;;){
		tcp_call tc;
		int t = recv(connfd, &tc, sizeof(tc));
		if(t<0)die("recv() failed");
		if(t == 0){
			// connection closed
			break;
		}
		
		// messnd_nmb
		if(tc.action == 1){
		send_msg_to_udp(tc.msg,udp_socket);
		}
		// msgrcv_nmb 
		else if(tc.action == 2){
		 // check if msg is there
		 while(retrieve_msg_queue(tc.msg.mtype) != 0){
		 sleep(1);
		 }
		 msgbuf msg;
		 retrieve_msg_queue(get_client_details,&msg);
		 send(connfd,&msg,sizeof(msg), 0);
		}
	}
	close (connfd);
    exit(0);
}

// Making UDP server







