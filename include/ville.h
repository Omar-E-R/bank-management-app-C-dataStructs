#include <stdio.h>
#include <stdlib.h>
#include "agence.h"

typedef enum
{
    Toulouse=31
    //on peut en rajouter d'autres
}Nom_Ville;

typedef struct ville *Ville;
//Pour authentificer les logins, les logins
Client decrypt_ville(Login user);//uses agence.h/decrypt_client

Compte decrypt_ville_ad(Login_Admin user);//uses agence.h/decrypt_client
