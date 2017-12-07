#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


int main(int argc,char** argv){
if (argc!=4){
	printf("Nombre de parametres incorrects ! Utilisation : nomprog ip port nomfihier\n");
	return -1;
}

char* nomfichier=argv[3];//le nom du fichier en parametre
nomfichier[strlen(nomfichier)]='\0';

printf("Nom du fichier à envoyer : %s\n",nomfichier);

int dSock=socket(PF_INET,SOCK_STREAM,0);// je creer la socket de mon coté
if (dSock<0) {
	printf("Erreur lors de la creation de la socket ! ");
	return -1;
}

struct sockaddr_in adR;// création de ladresse
adR.sin_family=AF_INET;
adR.sin_port=htons(atoi(argv[2])); // le port du serveur est a passé en parametre

int res =inet_pton(AF_INET,argv[1],&(adR.sin_addr));//On va stocker l'adresse sous forme réseau 
if (res!=1) {
	printf("Erreur lors du stockage de l'adresse !");
	return -1;
}

FILE * fp;
char adressefic[4096];
adressefic[4095]='\0';

strcat(adressefic,"envoi/");
strcat(adressefic,nomfichier);// adresse du fichier a envoyer

fp =fopen(adressefic,"r");

struct stat sb;//pour recuperer la taille précise du fichier
if (stat(adressefic,&sb)==-1) {
    perror("stat");
    exit(EXIT_FAILURE);
}

long taille=(long) sb.st_size;//stockage de la taille du fichier pour l'envoyer
printf("Taille du fichiers en octets: %ld\n",taille);



socklen_t lgAd=sizeof(adR);
int conn=connect(dSock,(struct sockaddr *) &adR,lgAd);// je me connecte a la socket du server 
if (conn == -1){
	printf("Erreur lors de la connection !");
	return -1;
}

int res1=send(dSock,&taille,sizeof(int),0);// j'envoie la taille totale du fichier au serveur
if (res1 == -1){
	printf("Erreur lors de l'envoi !");
	return -1;
}
printf("Taille de la donnee envoyee (taille fichier) : %d\n",res1);

int taille1=(int)(strlen(nomfichier));//taille du nom du fichier a envoyer 

int res4=send(dSock,&taille1,sizeof(int),0);//j'envoie la taille du nom
if (res4 == -1){
	printf("Erreur lors de l'envoi de la taille du nom du fic ! ");
	return -1;
}

int res3=send(dSock,nomfichier,strlen(nomfichier),0);//j'envoie le nom du fichier au serveur
if (res3 == -1){
	printf("Erreur lors de l'envoi du nom !");
	return -1;
}

char buffer[100];
int acclus = 0;

while (acclus < taille){// tant qu'on peut lire dans le fichier

	int nblus = fread(&buffer,1,100,pf);// je lis jusqua 100 caractere dans le fichier
	if (nblus == -1){
		printf("Erreur lors de la lecture dans le fichier !"),
		return -1;
	}
	printf("Lu : %s\n", buffer);

	int res2=send(dSock,buffer,sizeof(buffer),0);//j'envoi ce que j'ai lu au serveur
	if (res2 == -1){
		printf("Erreur lors de l'envoi partie !");
		return -1;
	}

	acclus += res2;//pour arreter le while
	res2 = 0;
}

printf("Fichier envoyé ! ");

fclose(fp);//je ferme mon fichier
close(dSock);//je ferme la socket car communication terminée

	return 0;
}