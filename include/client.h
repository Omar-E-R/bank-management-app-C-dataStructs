#ifndef CLIENT_H
#define CLIENT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compte.h"
#include "login.h"

typedef enum
{
    nom,
    prenom,
    ate_de_naissance,
    email,
    adresse,
    code_postale,
    ville,
    complement_ad,
    numero_mobile,
    numero_fixe

} type_donnees;

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


typedef struct{
    int statut;

    int numero_client;

    Donnees_Personnelles donnees_perso;

    Comptes comptes;

    Client next_client;

    Login user;

}* Client;

Client new_client();

int modify_client(Client client);

int afficher_client(Client client);

void afficher_liste_client(Admin admin, Client liste_clients); //ADMIN

int get_donnees();
int set_donnees();

#endif