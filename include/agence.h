#include<stdio.h>
#include<stdlib.h>
#include "client.h"
#include "generateRand.h"

typedef struct liste_compte
{
    Compte compte;

    lComptes next_compte;

} *lComptes;

typedef struct liste_clients
{
    Client client;
    Login user;
    lClients next_client;

} *lClients;

typedef struct agence
{
    const char* uuid_agence;

	const char *code_bic;
	const char *indicatif_agence;
	const char *domiciliation_agence;

    lComptes liste_comptes;

    lClients liste_clients;

    lConseiller liste_conseillers;

}*Agence;

typedef struct conseiller
{
	const char *uuid_conseiller;

	Donnees_Personnelles info_conseiller;

}*Conseiller;

typedef struct liste_conseiller
{
    Login login_conseiller;
    Conseiller conseiller;
	
	lConseiller next_conseiller;
}*lConseiller;


Client decrypt_client();
Admin decrypt_admin();
