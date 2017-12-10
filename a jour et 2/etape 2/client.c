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


DIR *dir;
struct dirent *ent;
char ** files;
int acc=0;
if ((dir = opendir ("envoi\\")) != NULL) {
  while ((ent = readdir (dir)) != NULL) {
    printf ("%s\n", ent->d_name);
    files[acc]=ent->d_name;
    acc++;
  }
  closedir (dir);
} else {
  /* could not open directory */
  perror ("");
  return EXIT_FAILURE;
}


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


socklen_t lgAd=sizeof(adR);
int conn=connect(dSock,(struct sockaddr *) &adR,lgAd);// je me connecte a la socket du server 
if (conn == -1){
	printf("Erreur lors de la connection !");
	return -1;
}

for (int i=0;i<acc;i++){

char* nomfichier=files[i];
nomfichier[strlen(nomfichier)]='\0';

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

fclose(fp);//je ferme mon fichier

}

close(dSock);//je ferme la socket car communication terminée

	return 0;
}