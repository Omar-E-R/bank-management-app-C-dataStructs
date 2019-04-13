#ifndef CLIENT_H
#define CLIENT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compte.h"
#include "login.h"

typedef struct client_size
{
	size_t nom_size,
		prenom_size,
		date_de_naissance_size,
		email_size,
		adresse_size,
		code_postale_size,
		nom_ville_size,
		complement_ad_size,
		numero_mobile_size,
		numero_fixe_size,
		carte_id_size, date_de_creation_size;
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
    NULL_,
    NOT_NULL_,
    COLLUSION


} type_donnees;

enum allocations
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

typedef struct donnees_personnelles
{

    const char *nom;
    const char *prenom;
    char sexe;

    const char* date_de_naissance;

    const char *email;
    const char *adresse;


    int code_postale;
    const char *nom_ville;
    const char *complement_ad;
    const char *numero_mobile;
    const char *numero_fixe;

	const char* carte_identite;
	const char* date_de_creation;

} * Donnees_Personnelles;


typedef struct client{

	const char* uuid_client;
	Login user_login;
	int statut;

    Donnees_Personnelles donnees_perso;

    Compte comptes;

}* Client;

Donnees_Personnelles init_donnees_perso_arg(client_size_t alloc_size, char sexe, char *nom, char *prenom, char *date_de_naissance, char *email, char* adresse, int code_postale, char* nom_ville, char* complement_ad, char* numero_mobile, char* numero_fixe, char* carte_id, char* date_de_creation);

Client Client_Courant;

type_donnees *equalAllClients(Client client1, Client client2);

Client new_client();

int modify_client(Client client);

int afficher_client(Client client);

void afficher_liste_client(Admin admin, Client liste_clients); //ADMIN

int get_donnees();
int set_donnees();

#endif