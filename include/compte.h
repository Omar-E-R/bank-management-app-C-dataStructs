#ifndef COMPTE_H
#define COMPTE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include"client.h"

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

typedef struct rib *Rib;

typedef struct compte *Compte;
typedef union client_s Client_s;
int isEqualCompte(Compte c1, Compte c2);
Compte init_compte();
int add_compte(Compte comptes, Compte c);
Rib init_rib();
Rib init_rib_arg(rib_size_t alloc_size, char* iban, char* code_bic, char* numero_compte, int indicatif_agence, char* domiciliation);
Compte init_compte_arg(char* uuid_compte, char* iban, int nature_compte, int type_compte, char* operations, Client_s titulaire);
#endif