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

File * fp;
int port=3271;
int taillerecu=0;

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

int taillefic=recv(dSClient,&taillefic,sizeof(int),0);// le client m'envoie la taille du fichier total 
if (taille==-1){
	printf("Erreur lors de la reception de la taille du fichier !");
	return -1;
}
printf("taille du fichier a recevoir: %d\n",taillefic);


int tnm=recv(dSClient,&taillenom,sizeof(int),0);//Je dois récuperer le nom du fichier donc je recupere la taille du nom pour faire le buffer
if (tnm==-1){
	printf("Erreur lors de la reception de la taille du nom !");
	return -1;
}

printf("taille du nom du fichier a recevoir: %d\n",tnm);

char nom[tnm];// buffer qui stockera le nom du fihchier

int resn=recv(dSClient,&tnm,sizeof(nom),0);//buffer pret je peux recevoir le nom de mon fichier
if (resn==-1){
	printf("Erreur lors de la reception du nom !");
	return -1;
}



fp = fopen(nom,"w");//je créer le fichier du coté du serveur, avec son nom , qui va etre rempli après
int sumreception = 0;

while (sumreception < taillefic){// tant que j'ai pas recu l'ensemble du fichier :
	char[100] buff2;
	int partial = recv(dSClient,&buff2,sizeof(buff2),0);//je recois jusqua 100 octets du fichier 
	if (partial == -1){
		printf("Erreur lors de la reception partielle ");
		return -1;
	}
	fwrite(&buff2,1,partial,fp);//jecris les octets recus dans le fichier de mon coté
	sumreception += partial;//accumulateur pour arret boucle
	partial = 0;
}

printf("Transfert terminé !");
fclose(fp);// je ferme le fichier
	
	return 0;
}