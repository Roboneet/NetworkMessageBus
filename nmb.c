#include <"utils.h">

void die(char* str){
    perror(str);
    exit(0);
}

nmb_t msgget_nmb(){
    int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock < 0)die("socket() failed");
	
    struct sockaddr_in servAddr; 
    unsigned short servPort = TCP_PORT;
    
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family  = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(servIP);   
    servAddr.sin_port        = htons(echoServPort); 
    if (connect(sock, (struct sockaddr *) &servAddr,
    	sizeof(servAddr)) < 0)
        die("connect() failed");

    return sock;
}

int msgsnd_nmb(nmb_t nmbid, const void *msgp, 
	size_t msgsz, int msgflg){
	return send(nmbid, msgp, msgsz, 0);
}

ssize_t msgrcv_nmb(nmb_t nmbid, void *msgp,
    size_t msgsz, long msgtyp, int msgflg){
    return recv(nmbid, msgp, msgz, 0);
}

int msgctl_nmb(nmb_t nmbid, int cmd,
    struct msqid_ds *buf){
    
    if(cmd == IPC_RMID){
        return close(nmbid);
    }
}
    
