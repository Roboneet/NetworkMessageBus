#include "utils.h"

enum states{SHOW_MENU, SEND, RCV, QUIT};

void print_banner();

int main(){
	print_banner();
	nmb_t nmbid = msgget_nmb();

	enum states state = SHOW_MENU;
	char *ip, *port, *text;
	struct msgbuf msg;
	int type;

	while(1){
		printf("     __________________________     \n");
		switch(state){
			case SHOW_MENU:
				printf("Choose an option:\n");
				printf("1 Send a message\n");
				printf("2 Read a message\n");
				printf("3 Quit\n");
				scanf("%d", &state);
				break;
			case SEND:
				printf("Enter IP:\n");
				scanf("%s\n", ip);
				printf("Enter Port:\n");
				scanf("%s\n", port);
				
				printf("Enter message:\n");
				scanf("%s\n", text);
				msg.mtype = get_mtype(ip, port);
				strcpy(msg.mtext, text);
				printf("Sending message...\n");
				if(msgsnd_nmb(nmbid, msg, sizeof(msg), 0) < 0){
					die("msgsnd_nmb() failed");
				}
				printf("Message sent...\n");
				state = SHOW_MENU;
				break;
			case RCV:
				printf("Reading a message...\n");
				if(msgrcv_nmb(nmbid, &msg, sizeof(msg), get_my_mtype(nmbid), 0) < 0){
					die("msgsnd_nmb() failed...");
				}
				printf("Here you go:\n");
				printf("%s\n", msg.mtext);;
				state = SHOW_MENU;
			case QUIT:
				msgrem_nmb(nmbid);
				printf("Bye Bye...");
				exit(0);
			default:
				printf("Bad choice, try again...\n");
				state = SHOW_MENU;
		}
	}
	return 0;
}


void print_banner(){
	// tadaaa
	printf("\n");
	printf("      **  **   **   **   ****       \n");
	printf("      *** **   **   **   ** **      \n");
	printf("      ******   *** ***   ****       \n");
	printf("      ** ***   ** * **   ** **      \n");
	printf("      **  ** * **   ** * ****       \n");
	printf("     __________________________     \n");
	printf("\n");

}	
