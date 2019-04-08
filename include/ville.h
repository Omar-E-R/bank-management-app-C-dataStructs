#include <stdio.h>
#include <stdlib.h>
#include "agence.h"

typedef enum
{
    Toulouse=31000
    //on peut en rajouter d'autres
}Nom_Ville;

typedef struct liste_agence
{
	Agence agence;
	lAgence next_agence;
}*lAgence;

typedef struct ville
{
	const char *uuid_ville;

	const char *code_postale;
	const char* nom_ville;

	lAgence liste_agences;

} * Ville;

struct data
{
	Ville ville;

	lData next_ville;
};

typedef struct admin
{
	Login_Admin login_admin;
	const char* uuid_admin;
	Admin next_admin;
}*Admin;

typedef struct data *lData;
//Pour authentificer les logins, les logins
Client decrypt_agence(Login user);//uses agence.h/decrypt_client

Compte decrypt_agence_ad(Login_Admin user);//uses agence.h/decrypt_client
