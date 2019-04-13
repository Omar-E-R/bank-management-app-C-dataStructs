#include<stdio.h>
#include<stdlib.h>
#include "client.h"
#include "generateRand.h"

typedef struct agence_size
{
	size_t uuid_agence, code_bic, domiciliation_agence, hash_code;
	size_t indicatif_agence;

} agence_size_t;

typedef struct liste_compte
{
    Compte compte;

    lComptes next_compte;

} *lComptes;

typedef struct liste_clients
{
    Client client;
    lClients next_client;

} *lClients;

typedef struct agence
{
    const char* uuid_agence;

	const char *code_bic;
	const char *indicatif_agence;
	const char *domiciliation_agence;
	const char *hash_code;

    lComptes liste_comptes;

    lClients liste_clients;

    lConseiller liste_conseillers;

}*Agence;

typedef struct conseiller
{
	const char *uuid_conseiller;
	int statut;

	Donnees_Personnelles info_conseiller;
	Login login_conseiller;

}*Conseiller;

typedef struct liste_conseiller
{
    Conseiller conseiller;
	
	lConseiller next_conseiller;
}*lConseiller;

Agence init_agence_arg(agence_size_t allocation_size, char *uuid_agence, char *code_bic, int indicatif_agence, char *domiciliation_agence, char *hash_code);
lClients init_liste_clients();
int addClient(lClients liste_client, Client client);
	Client decrypt_client();
Admin decrypt_admin();
