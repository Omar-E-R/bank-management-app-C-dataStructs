#include<stdio.h>
#include<stdlib.h>
#include "client.h"

typedef struct
{
    int code_agence;
    
    char* nom_agence;

    Compte liste_comptes;

    Client liste_clients;

    Admin admins;

}*Agence;

typedef struct
{
    int code_agence;

    char* nom_admin;

    int login_id;

    int Key;

    int date_de_naissance;//utilise pour encrypter le mots de pass


     

}*Admin;


Client decrypt_client();
Admin decrypt_admin();
