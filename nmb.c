#include <"utils.h">

void die(char* str){
    perror(str);
    exit(0);
}



tcp_call create_tcp_call(int action, void* msg){
	tcp_call tc;
	tc.action = action;
	tc.msg = msg;
	return tc;
}

nmb_t msgget_nmb(){
    int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock < 0)die("socket() failed");
	
    struct sockaddr_in servAddr; 
    unsigned short servPort = TCP_PORT;
    
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family  = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(servIP);   
    servAddr.sin_port        = htons(servPort); 
    if (connect(sock, (struct sockaddr *) &servAddr,
    	sizeof(servAddr)) < 0)
        die("connect() failed");

    return sock;
}

int msgsnd_nmb(nmb_t nmbid, msgbuf msg, 
	size_t msgsz, int msgflg){
	tcp_call tc = create_tcp_call(MSGSND, &msg);
	return send(nmbid, &tc, sizeof(tc), 0);
}

ssize_t msgrcv_nmb(nmb_t nmbid, msgbuf* msgp,
    size_t msgsz, long msgtyp, int msgflg){
    tcp_call tc = create_tcp_call(MSGRCV, msgp);
    
    send(nmbid, &tc, sizeof(tc), 0);
    
    int t = recv(nmbid, &tc, sizeof(tc), 0);
    if(t < 0 )die("recv() failed");
    
    *msgp = tc.msg;
    return sizeof(tc.msg);
}
/***
int msgctl_nmb(nmb_t nmbid, int cmd,
    struct msqid_ds *buf){
    
    if(cmd == IPC_RMID){
        return close(nmbid);
    }
    
}
***/
int msgrem_nmb(nmb_t nmbid){

tcp_call tc = create_tcp_call(MSGCLS, NULL);
send(nmbid,&tc,sizeof(tc),0);

return 0;
}



