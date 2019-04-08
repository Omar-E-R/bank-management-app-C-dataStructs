#ifndef CLIENT_H
#define CLIENT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compte.h"
#include "login.h"

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
    DATE_DE_NAISSANCE_SIZE=9,
    EMAIL_SIZE=45,
    ADRESSE_SIZE=50,
    CODE_POSTALE_SIZE=6,
    NOM_VILLE_SIZE=30,
    COMPLEMENT_AD_SIZE=50,
    NUMERO_MOBILE_SIZE=11,
    NUMERO_FIXE_SIZE=11

};

typedef struct donnees_personnelles
{

    const char *nom;
    const char *prenom;

    const char* date_de_naissance;

    const char *email;
    const char *adresse;

    int code_postale;
    int MouMme;

    const char *nom_ville;
    const char *complement_ad;
    const char *numero_mobile;
    const char *numero_fixe;

} * Donnees_Personnelles;


typedef struct client{

    int statut;

	const char* uuid_client;

    Donnees_Personnelles donnees_perso;

    Compte comptes;

}* Client;


Client Client_Courant;

type_donnees *equalAllClients(Client client1, Client client2);

Client new_client();

int modify_client(Client client);

int afficher_client(Client client);

void afficher_liste_client(Admin admin, Client liste_clients); //ADMIN

int get_donnees();
int set_donnees();

#endif