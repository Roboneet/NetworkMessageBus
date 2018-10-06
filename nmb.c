#include "utils.h"

void die(char* str){
    perror(str);
    exit(0);
}

MsgBuf dummyMsg(nmb_t nmbid){
    MsgBuf msg;
    int p;
    get_my_port(nmbid, &p);
    msg.mtype = (long)p;
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
    servAddr.sin_addr.s_addr = INADDR_ANY;
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
    
    int t = recv(nmbid, msgp, msgsz, 0);
    if(t < 0 )die("recv() failed");
    
    return sizeof(*msgp);
}

int msgrem_nmb(nmb_t nmbid){
    close(nmbid);
    return 0;
}

void printbits(long i){
    while(i != 0){
        printf("%ld", i % 2);
        i = i/2;
    }
    printf("\n");
}

void extract(long type, uint32_t* ip, int* port){
    *ip = (uint32_t)(type >> 16);
    *port = type & 0xffff;
}

long get_mtype(char* ip, int port){
    return ((long)(inet_addr(ip)) << 16) | (long)port;
}

void ip_to_string(uint32_t addr, char* str){
    inet_ntop(AF_INET, &(addr), str, INET_ADDRSTRLEN);
}

void get_my_port(nmb_t nmbid, int* p){
    struct sockaddr_in me,temp;
    char address[INET_ADDRSTRLEN];
    unsigned int len = sizeof(me);
    getsockname(nmbid, (struct sockaddr*)&me, &len);
    *p = me.sin_port;
}
