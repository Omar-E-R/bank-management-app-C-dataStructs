#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "client.h"


enum nature_compte
{
    Vierge,
    Supprime,
    NonJoint,
    Joint,
    LivretA,
    Depot,
    PEL,
    Actif,
    EnAttente
    //on peut en rajouter d'autres
};


typedef struct compte
{
    int statut; //supprime ou pas, i.e actif ou pas
    
    int joint;
    
    int type;

    float solde;

    int numero;

    char* operations;

    Client clients[2];

    Compte next_compte;


} * Compte;

Compte request_new_acc(Client client1, Client client2, float solde);

Compte add_new_acc(Admin admin, Client client1, Client client2, float solde);//ADMIN

int modify_acc(Admin admin, Compte compte); //ADMIN

int afficher_acc(Compte compte);

void afficher_liste_acc(Admin admin, Compte liste_comptes,int type);//ADMIN

int get_operations(); //uses ville.h/decrypt_ville

int get_solde();
