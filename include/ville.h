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

	const char* nom_ville;
	
	const char *code_postale;

	lAgence liste_agences;

} * Ville;

typedef struct data
{
	Ville ville;

	lData next_ville;
}*lData;

typedef struct admin
{
	Login_Admin login_admin;
	const char* uuid_admin;
	Admin next_admin;
}*Admin;

Ville init_ville();
lData init_data();
Admin init_admin();
Admin new_admin();
Ville new_ville(const char* nom_ville, const char* code_postale);
int addAgence(lAgence liste_agence, Agence agence);
return_type isEqualVille(Ville agence1, Ville agence2);
return_type isEqualAdmin(Admin admin1, Admin admin2);
int addVille(lData liste_ville, Ville ville);
int addAdmin(Admin lAdmin, Admin admin);



//Pour authentificer les logins, les logins
Client decrypt_agence(Login user);//uses agence.h/decrypt_client

Compte decrypt_agence_ad(Login_Admin user);//uses agence.h/decrypt_client
