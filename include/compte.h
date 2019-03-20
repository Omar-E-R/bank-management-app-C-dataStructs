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
    PEL
    //on peut en rajouter d'autres
};


typedef struct compte
{
    int statut; //supprime ou pas, i.e actif ou pas
    
    int joint;
    
    int type;

    float solde;

    int numero;

    Titulaire_Compte[2] titulaires;

    Compte next_compte;


} * Comptes;


Compte new_acc(Client client);

int modify_acc(Compte compte);

int afficher_acc(Compte compte);

void afficher_liste_acc(Compte liste_comptes);