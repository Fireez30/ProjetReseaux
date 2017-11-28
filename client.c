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

char* nomfichier=argv[3];
nomfichier[strlen(nomfichier)]='\0';

printf("Nom du fichier à envoyer : %s\n",nomfichier);

int dSock=socket(PF_INET,SOCK_STREAM,0);
if (dSock<0) {
	printf("Erreur lors de la creation de la socket ! ");
	return -1;
}

struct sockaddr_in adR;
adR.sin_family=AF_INET;
adR.sin_port=htons(atoi(argv[2]));

int res =inet_pton(AF_INET,argv[1],&(adR.sin_addr));
if (res!=1) {
	printf("Erreur lors du stockage de l'adresse !");
	return -1;
}

FILE * fp;
char adressefic[4096];
adressefic[4095]='\0';

strcat(adressefic,"envoi/");
strcat(adressefic,nomfichier);

fp =fopen(adressefic,"r");

struct stat sb;//pour recuperer la taille précise du fichier
if (stat(adressefic,&sb)==-1) {
    perror("stat");
    exit(EXIT_FAILURE);
}

long taille=(long) sb.st_size;
printf("Taille du fichiers en octets: %ld\n",taille);

char buffer[taille];
fread(buffer,taille,1,fp);
fclose(fp);
printf("Lu : %s\n", buffer);

socklen_t lgAd=sizeof(adR);
int conn=connect(dSock,(struct sockaddr *) &adR,lgAd);
if (conn == -1){
	printf("Erreur lors de la connection !");
	return -1;
}

int res1=send(dSock,&taille,sizeof(int),0);
if (res1 == -1){
	printf("Erreur lors de l'envoi !");
	return -1;
}
printf("Taille de la donnee envoyee (taille fichier) : %d\n",res1);

int taille1=(int)(strlen(nomfichier));//taille du nom du fichier a envoyer

int res4=send(dSock,&taille1,sizeof(int),0);
if (res4 == -1){
	printf("Erreur lors de l'envoi de la taille du nom du fic ! ");
	return -1;
}

int res2=send(dSock,buffer,taille,0);//Envoi du contenu
if (res2==-1){
	printf("Erreur lors de l'envoi ! ");
	return -1;
}
printf("Taille envoyée : %d\n",res2);

int res3=send(dSock,nomfichier,strlen(nomfichier),0);
if (res3 == -1){
	printf("Erreur lors de l'envoi du nom !");
	return -1;
}

close(dSock);

	return 0;
}