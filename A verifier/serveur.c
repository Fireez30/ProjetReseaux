#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char** argv){

if (argc!=1){
	printf("Paramètres incorrects ! ");
	return -1;
}

FILE * fp;
int port=3271;
int taillerecu=0;

int dSock=socket(PF_INET,SOCK_STREAM,0);
if (dSock<0) {
	printf("Erreur lors de la creation de la socket ! ");
	return -1;
}

struct sockaddr_in adR;
adR.sin_family=AF_INET;
adR.sin_port=htons(port);
adR.sin_addr.s_addr = INADDR_ANY;

int res1=bind(dSock,(struct sockaddr *) &adR,sizeof(adR));
if (res1==-1){
	printf("Erreur du bind \n");
	return -1;
}

listen(dSock,2);

struct sockaddr_in adClient;
socklen_t lgA = sizeof(struct sockaddr_in);

int dSClient=accept(dSock,(struct sockaddr *) &adClient,&lgA);

int taillefic=recv(dSClient,&taillefic,sizeof(int),0);
if (taille==-1){
	printf("Erreur lors de la reception de la taille du fichier !");
	return -1;
}
printf("taille du fichier a recevoir: %d\n",taillefic);


int tnm=recv(dSClient,&taillenom,sizeof(int),0);
if (tnm==-1){
	printf("Erreur lors de la reception de la taille du nom !");
	return -1;
}
printf("taille du nom du fichier a recevoir: %d\n",tnm);

char nom[tnm];

int nvnom=recv(dSClient,&nom,taillenom,0);
if (nvnom==-1){
	printf("Erreur lors de la reception du nom !");
	return -1;
}

fp=fopen(nom, "w"); 

char msg[100];

int nbrecu2=0;
while (nbrecu2 < taillefic){
int nbrecu=recv(dSClient,msg,taillefic,0);
if (nbrecu == -1){
	printf("Fail recepetion");
	return -1;
}
	fwrite(msg,1,nbrecu,fp);
	nbrecu2 += nbrecu;
}

fclose(fp);

	return 0;
}