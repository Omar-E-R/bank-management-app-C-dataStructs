#ifndef CLIENT_H
#define CLIENT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"compte.h"
#include"login.h"
#include"random.h"

enum allocations_size_client
{
	NOM_SIZE=30,
	PRENOM_SIZE=30,
	DATE_DE_NAISSANCE_SIZE=8,
	EMAIL_SIZE=45,
	ADRESSE_SIZE=50,
	CODE_POSTALE_SIZE=5,
	NOM_VILLE_SIZE=30,
	COMPLEMENT_AD_SIZE=50,
	NUMERO_MOBILE_SIZE=10,
	NUMERO_FIXE_SIZE=10,
	HASH_CODE_SIZE=20,
	CARTE_ID_SIZE=20,
	DATE_DE_CREATION_SIZE=8

};

typedef struct client_size
{
	size_t nom_size,
		prenom_size,
		email_size,
		adresse_size,
		nom_ville_size,
		complement_ad_size,
		carte_id_size;

} client_size_t;

typedef enum
{
	UUID_CLIENT,
	NOM,
	PRENOM,
	DATE_DE_NAISSANCE,
	EMAIL,
	ADRESSE,
	CODE_POSTALE,
	NOM_VILLE,
	COMPLEMENT_AD,
	NUMERO_MOBILE,
	NUMERO_FIXE,

} donnees_perso_t;


typedef struct donnees_personnelles *Donnees_Personnelles;
typedef struct client *Client;
typedef union client_s Client_s;

Donnees_Personnelles init_donnees_perso();

Donnees_Personnelles init_donnees_perso_arg(client_size_t alloc_size, char sexe, char *nom, char *prenom, char *date_de_naissance, char *email, char* adresse, int code_postale, char* nom_ville, char* complement_ad, char* numero_mobile, char* numero_fixe, char* carte_id, char* date_de_creation);

Client init_client();

Client init_client_arg(char* uuid_client, int statut);

Client_s init_client_s_arg(client_size_t alloc_size,char** uuid_client, char** titulaire);

int isEqualClient(Client client1, Client client2);

#endif