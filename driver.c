#include "utils.h"

enum states{SEND=1, RCV, PRINT_DETAILS, SHOW_MENU, QUIT, WAIT};

void print_banner();

int main(){
	print_banner();
	nmb_t nmbid = msgget_nmb();

	enum states state = SHOW_MENU;
	char ip[INET_ADDRSTRLEN], port[10], text[100];
	struct msgbuf msg;
	int type;

	long my_type = get_my_mtype(nmbid);
	uint32_t my_ip_;
	char my_ip[INET_ADDRSTRLEN];
	int my_port;
	extract(my_type, &my_ip_, &my_port);
	ip_to_string(my_ip_, my_ip);
	printf("My IP: %s\n", my_ip);
	printf("My Port: %d\n", my_port);


	while(1){
		printf("     __________________________     \n\n");
		switch(state){
			case SHOW_MENU:
				printf("Choose an option:\n");
				printf("1 Send a message\n");
				printf("2 Read a message\n");
				printf("3 Print Details\n");
				printf("4 Show menu\n");
				printf("5 Quit\n");
				state = WAIT;
				break;
			case SEND:
				printf("Enter IP:\n");
				scanf("%s", ip);

				printf("Enter Port:\n");
				scanf("%s", port);
				
				printf("Enter message:\n");
				scanf("%s", text);
				msg.mtype = get_mtype(ip, atoi(port));
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
				break;
			case PRINT_DETAILS:
				printf("My IP: %s\n", my_ip);
				printf("My Port: %d\n", my_port);
				state = SHOW_MENU;
			case WAIT:
				scanf("%d", &state);
				break;
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
