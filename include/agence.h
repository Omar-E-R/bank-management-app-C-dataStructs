#include<stdio.h>
#include<stdlib.h>
#include "compte.h"
#include "client.h"

typedef struct agence
{
    int code_agence;
    
    char* nom_agence;

    Compte liste_comptes;

    Titulaire_Compte liste_clients;

    Admin admins;

}*Agence;

typedef struct admin
{
    int code_agence;

    char* nom_admin;

    int login;

    int password; 

}*Admin;

