#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "client.h"
#include "random.h"



enum allocation_size_compte
{
	IBAN_SIZE=27,
	CODE_BIC_SIZE=8,
	NUMERO_COMPTE_SIZE=11,
	INDICATIF_AGENCE_SIZE=5,
	DOMICILIATION_SIZE=35,
	UUID_SIZE=36,
	OPERATIONS_SIZE=36
};

typedef struct rib_size
{
	 size_t iban_size, code_bic_size, numero_compte_size, indicatif_agence_size, domiciliation_size;
}rib_size_t;

typedef enum
{
	UNINTIALIZED,
	JOINT,
	INDIVIDUEL,
	ACTIVE,
	INACTIVE,
	ENATTENTE,
	LIVRET_A,
	LDD,
	LIVRET_JEUNE,
	COURANT,
	PEL

} compte_t;

typedef enum
{
	IBAN,
	CODE_BIC,
	NUMERO_COMPTE,
	INDICATIF_AGENCE,
	DOMICILIATION,
	STATUT,
	JOINT_OU_INDIVIDUEL,
	TYPE,
	RIB,
	SOLDE,
	UUID_COMPTE
} compte_element_t;


typedef union client_s
{
	Client client;

	Client clients[2];

} Client_s;

typedef struct rib
{
	const char* iban;

	const char* code_bic;
	const char* numero_compte;
	const char* indicatif_agence;
	const char* domiciliation_agence;


} * Rib;

typedef struct compte
{
	const char* uuid_compte;

	compte_t statut; //compte supprime ? i.e actif ou non actif
	compte_t nature_compte;
	compte_t type_compte;//LIVRET...

	double solde;

	const char *operations;//A FILENAME OR MAYBE I WILL CHANGE IT TO A FILE POINTER

	Rib rib;

	Client_s titulaire;



	Compte next_compte;


} *Compte;
Compte init_compte();
Rib init_rib();
Rib init_rib_arg(rib_size_t alloc_size, char* iban, char* code_bic, char* numero_compte, int indicatif_agence, char* domiciliation);

Compte init_compte_arg(char* uuid_compte,int statut, int nature_compte, int type_compte, double solde, char* operations);

Compte Compte_Courant;

Compte request_new_acc(Client client1, Client client2, float solde);

Compte add_new_acc(Admin admin, Client client1, Client client2, float solde);//ADMIN

int modify_acc(Admin admin, Compte compte); //ADMIN

int afficher_acc(Compte compte);

void afficher_liste_acc(Admin admin, Compte liste_comptes,int type);//ADMIN
