#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "client.h"
#include "generateRand.h"

#define BANK_NUMBER 40305

typedef enum
{
	UNINTIALIZED,
    JOINT,
    NONJOINT,
    ACTIVE,
    DESACTIVE,
    ENATTENTE,
    LIVRETA,
    DEPOT,
    PEL

} type_compte;

typedef enum 
{
    MATCH,
    NO_MATCH,
    EMPTY_STACK,
    SUCCESS,
    FAILURE,
    ERROR,
    UNINTIALIZED
    //on peut en rajouter d'autres
} return_type;

typedef enum 
{
    IBAN,
    CODE_BIC,
    NUMERO_COMPTE,
    INDICATIF_AGENCE,
    DOMICILIATION,
    STATUT,
    JOINT_OU_NONJOINT,
    TYPE,
    RIB,
    SOLDE,
    UUID_COMPTE
} type_element;
typedef enum allocation_size_element
{
    IBAN_SIZE=28,
    CODE_BIC_SIZE=9,
    NUMERO_COMPTE_SIZE=12,
    INDICATIF_AGENCE_SIZE=6,
    DOMICILIATION_SIZE=35,
    UUID_SIZE=37
} allocation_size_element;

typedef union client_s {
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

    Rib rib;

    Client_s titulaire;

    type_compte statut; //compte supprime ? i.e actif ou non actif
    type_compte nature_compte;
    type_compte type_compte;//LIVRET...

    double solde;

    const char *operations;//A FILENAME OR MAYBE I WILL CHANGE IT TO A FILE POINTER

    Compte next_compte;


} *Compte;



Compte Compte_Courant;

Compte request_new_acc(Client client1, Client client2, float solde);

Compte add_new_acc(Admin admin, Client client1, Client client2, float solde);//ADMIN

int modify_acc(Admin admin, Compte compte); //ADMIN

int afficher_acc(Compte compte);

void afficher_liste_acc(Admin admin, Compte liste_comptes,int type);//ADMIN

int get_operations(); //uses ville.h/decrypt_ville

int get_solde();

