#ifndef AGENCE_H
#define AGENCE_H
#include<stdio.h>
#include<stdlib.h>
#include"client.h"
#include"compte.h"

typedef struct agence_size
{
	size_t uuid_agence, code_bic, domiciliation_agence, hash_code;
	size_t indicatif_agence;

} agence_size_t;


typedef struct liste_compte *lCompte;

typedef struct liste_client *lClient;

typedef struct conseiller *Conseiller;

typedef struct liste_conseiller *lConseiller;

typedef struct agence *Agence;

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

#endif