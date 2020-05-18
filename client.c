//Team members: 1. Adarsh Sai Gupta M S and Swathi Reddy Vanga
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h> 
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>
int main(int argc, char *argv[]){
char message[255];
// using current time as seed for random number geneartion
srand(time(0));
int server, portNumber,dice;
socklen_t len;
long int score=0;
struct sockaddr_in servAdd;
// General Prototype of call Model
if(argc != 3){
printf("Call model:%s <IP> <Port#>\n",argv[0]);
exit(0);
}
//Creation of Socket
if((server = socket(AF_INET, SOCK_STREAM, 0))<0){
fprintf(stderr, "Cannot create socket\n");
exit(1);
}
//Defnitions of parameters
servAdd.sin_family = AF_INET;
sscanf(argv[2], "%d", &portNumber);
servAdd.sin_port = htons((uint16_t)portNumber);
if(inet_pton(AF_INET,argv[1],&servAdd.sin_addr)<0){
fprintf(stderr, " inet_pton() has failed\n");
exit(2);
}
//Establishing Connection
if(connect(server,(struct sockaddr *)&servAdd,sizeof(servAdd))<0){
fprintf(stderr, "connect() has failed, exiting\n");
exit(3);
}
while(1){
//Reading message from server
if(read(server, message, 255)<0){
fprintf(stderr, "read() error\n");
exit(3);
}
//Player's turn
if(!strcasecmp(message, "You can now play")){
//dice =(int) time(&score)%6 + 1;
// Dice Generating random numbers from 1 to 6 
dice=rand()%6+1;
printf("Points earned in this turn: %d\n", dice);
printf("\n");	
write(server, &dice, sizeof(dice));
}
//If player has lost the game
else if(!strcasecmp(message, "Game over: you lost the game")){		
printf( "\n I lost the game");
close(server);
exit(4);
}
//If player has won the game
 else if(!strcasecmp(message, "Game over: you won the game")){		
printf( "\n I won the game");
close(server);
exit(3);
}

}
}
