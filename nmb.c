#include "utils.h"

void die(char* str){
    perror(str);
    exit(0);
}

MsgBuf dummyMsg(nmb_t nmbid){
    MsgBuf msg;
    msg.mtype = get_my_mtype(nmbid);
    return msg;
}

TcpCall create_tcp_call(int action, MsgBuf msg){
	TcpCall tc;
	tc.action = action;
	tc.msg = msg;
	return tc;
}

nmb_t msgget_nmb(){
    int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock < 0)die("socket() failed");
	
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family  = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servAddr.sin_port        = htons(TCP_PORT); 
    if (connect(sock, (struct sockaddr *) &servAddr,
    	sizeof(servAddr)) < 0)
        die("connect() failed");

    return sock;
}

int msgsnd_nmb(nmb_t nmbid, MsgBuf msg, 
	size_t msgsz, int msgflg){
	TcpCall tc = create_tcp_call(MSGSND, msg);
	send(nmbid, &tc, sizeof(tc), 0);
    return 0;
}

ssize_t msgrcv_nmb(nmb_t nmbid, MsgBuf* msgp,
    size_t msgsz, long msgtyp, int msgflg){
    TcpCall tc = create_tcp_call(MSGRCV, dummyMsg(nmbid));
    
    send(nmbid, &tc, sizeof(tc), 0);
    
    int t = recv(nmbid, &tc, sizeof(tc), 0);
    if(t < 0 )die("recv() failed");
    
    *msgp = tc.msg;
    return sizeof(tc.msg);
}

int msgrem_nmb(nmb_t nmbid){
    close(nmbid);
    return 0;
}

long get_my_mtype(int nmbid){
    return 0;
}

int extract_ip(long type){
    return 0;
}
long get_mtype(char* ip, char* port){
    return 0;
}