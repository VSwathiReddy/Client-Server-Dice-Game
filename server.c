//Team members: Adarsh Sai Gupta M S and Swathi Reddy Vanga
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/shm.h>
#include <time.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<string.h>
void servicePlayer(int toto,int titi);
int main(int argc, char *argv[]){
int sd, sd1,client1, client2, portNumber;
socklen_t len;
struct sockaddr_in servAdd;
if(argc != 2){
printf("Call model: %s <Port #>\n", argv[0]);
exit(0);
}
//Creation of Socket
if ((sd=socket(AF_INET,SOCK_STREAM,0))<0)//creates a communication and returns a file descriptor
{
fprintf(stderr, "Cannot create socket\n");
exit(1);
}
//Assigning values 
servAdd.sin_family = AF_INET;  //domain for internet address family
servAdd.sin_addr.s_addr = htonl(INADDR_ANY);  //host  to network for long
sscanf(argv[1], "%d", &portNumber);  //reading the format string rather than from int
servAdd.sin_port = htons((uint16_t)portNumber);  //host to network for short
bind(sd,(struct sockaddr*)&servAdd,sizeof(servAdd));//it assigns a protocol to specific socket address
listen(sd, 6);//is called by server to accept connections from client 
while(1)
{
client1=accept(sd,(struct sockaddr*)NULL,NULL);//to extract first connection on queue of pending connections
//printf("Got 1st client %d\n",client1);
client2=accept(sd,(struct sockaddr*)NULL,NULL);
printf("Got 2 Players.. SO we can start the dice game \n");

if(!fork())
  servicePlayer(client1,client2);
}
}
void servicePlayer(int toto,int titi){
time_t start_t, end_t;
struct tm * timeinfo;
double diff_t;
time(&start_t);
timeinfo = localtime ( &start_t );
printf("Starting Time of the game %s...\n",asctime(timeinfo));

char message[255]="You can now play";
int player1score=0,player1total=0,player2score=0,player2total=0;
int i;
printf("-----------GAME STARTS------------\n");
while(1){
printf("-----------NEW SET------------\n");
printf("Referee: Now its TOTO's turn \n");
write(toto, message, strlen(message)+1);
if(!read(toto, &player1score, sizeof(player1score))){	
close(toto);
fprintf(stderr,"Error happened in Read\n");
exit(0);
}
printf("Points Earned in this turn :%d\n",player1score);
player1total+=player1score;
printf("Total Points earned : %d\n",player1total);
if(player1total>=100)
{
	printf("****TOTO WINS THE DICE GAME***** \n");
	write(toto,"Game over: you won the game",50);
	close(toto);
	write(titi,"Game over: you lost the game",50);
	close(titi);
	time(&end_t);
	timeinfo = localtime ( &end_t );
	printf("Ending Time of the game %s...\n",asctime(timeinfo));
	diff_t = difftime(end_t, start_t);
	printf("Total Execution time taken for the game is: %f seconds\n",diff_t);
	exit(0);
}
sleep(2);
printf("Referee: Now its TITI's turn \n");
write(titi, message, strlen(message)+1);
if(!read(titi, &player2score, sizeof(player2score))){	
close(titi);
fprintf(stderr,"Error happened in Read\n");
exit(0);
}
printf("Points Earned in this turn :%d\n",player2score);
player2total+=player2score;
printf("Total Points earned : %d\n",player2total);
if(player2total>=100)
{
	printf("****TITI WINS THE DICE GAME***** ");
	write(titi,"Game over: you won the game",50);
	close(titi);
	write(toto,"Game over: you lost the game",50);
	close(toto);
	time(&end_t);
	timeinfo = localtime ( &end_t );
	printf("Ending Time of the game %s...\n",asctime(timeinfo));
	diff_t = difftime(end_t, start_t);
	printf("Total Execution time taken for the game is: %f  seconds\n",diff_t);
	exit(0);
}
sleep(3); 
}}
