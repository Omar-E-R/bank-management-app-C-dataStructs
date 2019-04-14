#include<stdio.h>
#include<stdlib.h>
#include "client.h"
#include "random.h"

typedef struct agence_size
{
	size_t uuid_agence, code_bic, domiciliation_agence, hash_code;
	size_t indicatif_agence;

} agence_size_t;

typedef struct liste_compte
{
	Compte compte;

	lCompte next_compte;

} *lCompte;

typedef struct liste_client
{
	Client client;
	lClient next_client;

} *lClient;

typedef struct agence
{
	const char* uuid_agence;

	const char *code_bic;
	const char *indicatif_agence;
	const char *domiciliation_agence;
	const char *hash_code;

	lCompte liste_compte;

	lClient liste_client;

	lConseiller liste_conseiller;

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

lCompte init_liste_compte();
lClient init_liste_client();
Conseiller init_conseiller();
Conseiller init_conseiller_arg(char* uuid_conseiller, int statut);
lConseiller init_liste_conseiller();
Agence init_agence();
Agence init_agence_arg(agence_size_t allocation_size,char *uuid_agence, char *code_bic, int indicatif_agence, char *domiciliation_agence, char *hash_code);
Agence new_agence(const char* domiciliation, const char* uuid_ville);
Conseiller new_conseiller(Donnees_Personnelles info_conseiller);
int isEqualConseiller(Conseiller employe1, Conseiller employe2);
int isEqualAgence(Agence agence1, Agence agence2);

int addConseiller(lConseiller liste_conseiller, Conseiller conseiller);
int addCompte(lCompte liste_compte, Compte compte);
int addClient(lClient liste_client, Client client);