#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char* nom;
	int taille;
} fichier;

typedef struct {
	char* adresse;
	short port;
	int nbfichiers;
	fichier* fichiers
} client;

int main(int argc,char** argv){
	if (argc!=1){
	printf("Paramètres incorrects ! ");
	return -1;
}

int nbclient;
client* clients;

int dSock=socket(PF_INET,SOCK_STREAM,0);//Crée la socket, si ca ne marche pas s'arrete
if (dSock<0) {
	printf("Erreur lors de la creation de la socket ! ");
	return -1;
}

struct sockaddr_in adR;// tout ca sert a bien ta socket correctement  , adR est la structure de l'adresse
adR.sin_family=AF_INET;// ca doit rester comme ca
adR.sin_port=htons(port);// passe le port en version reseau et le stock
adR.sin_addr.s_addr = INADDR_ANY; // ladresse est déterminée par le systeme

int res1=bind(dSock,(struct sockaddr *) &adR,sizeof(adR));//bind la socket 
if (res1==-1){
	printf("Erreur du bind \n");
	return -1;
}

listen(dSock,2);//met la socket en attente de requete endors le serveur

struct sockaddr_in adClient;//si un client arrivé je vais devoir stocker son adresse
socklen_t lgA = sizeof(struct sockaddr_in);

int dSClient=accept(dSock,(struct sockaddr *) &adClient,&lgA);// jaccepte la demande du client et je stocke son adresse dans adClient

struct client c1;
c1.port= ntohs(adClient.sin_port);
printf("Port du client = %d\n",ntohs(adClient.sin_port));
clients[nbclient]=c1;
nbclient++;

return 0;
}