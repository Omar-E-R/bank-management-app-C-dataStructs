
#include <stdio.h>
#include <stdlib.h>
#include "compte.h"
#include "login.h"

typedef struct donnees_personnelles
{

    char *nom;
    char *prenom;
    int date_de_naissance;
    char *email;
    char *adresse;
    int code_postale;
    char *ville;
    char *complement_ad;
    char *numero_mobile;
    char *numero_fixe;

} * Donnees_Personnelles;


typedef struct titulaire_compte
{
    int statut;

    Donnees_Personnelles donnees_perso;

    Comptes comptes;

    int key_a;
    int key_b;

} * Titulaire_Compte;


typedef struct client
{
    int statut;

    int numero_client;

    Donnees_Personnelles donnees_perso;

    Comptes comptes;

    Login username;

}* Client;

Client new_client();

int modify_client(Client client);

int afficher_client(Client client);

int afficher_titulaire(Client client);

void afficher_liste_client(Client liste_clients);
