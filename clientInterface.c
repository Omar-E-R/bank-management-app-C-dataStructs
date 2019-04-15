#include<stdio.h>
#include"jsonlib.h"
#include"bankmanagement.h"
#include"userinterface.h"


#include"ville.h"
#include"agence.h"
#include"client.h"
#include"compte.h"
#include"login.h"


struct login
{
    const char* login_id;
    const char* login_key;

};
struct login_admin
{
    const char* code_agence;
    const char* login_id;
    const char* pin;

};

union client_s
{
	Client client;

	Client clients[2];

};

struct rib
{
	const char* iban;
	const char* code_bic;
	const char* numero_compte;
	const char* indicatif_agence;
	const char* domiciliation_agence;

};
struct compte
{
	const char* uuid_compte;

	compte_t statut; //compte supprime ? i.e actif ou non actif
	compte_t nature_compte;
	compte_t type_compte;//LIVRET...

	double solde;

	const char *operations;//A FILENAME OR MAYBE I WILL CHANGE IT TO A FILE POINTER

	Rib rib;

	Compte next_compte;

	Client_s titulaire;
};



struct donnees_personnelles
{

	const char *nom;
	const char *prenom;
	char sexe;

	const char* date_de_naissance;

	const char *email;
	const char *adresse;


	int code_postale;
	const char *nom_ville;
	const char *complement_ad;
	const char *numero_mobile;
	const char *numero_fixe;

	const char* carte_identite;
	const char* date_de_creation;

};

struct client{

	const char* uuid_client;
	Login client_login;
	int statut;

	Donnees_Personnelles donnees_perso;

	Compte comptes;

};

struct liste_compte
{
	Compte compte;

	lCompte next_compte;

};
struct liste_client
{
	Client client;
	lClient next_client;

};
struct conseiller
{
	const char *uuid_conseiller;
	int statut;

	Donnees_Personnelles info_conseiller;
	Login login_conseiller;

};
struct liste_conseiller
{
	Conseiller conseiller;

	lConseiller next_conseiller;
};
struct agence
{
	const char* uuid_agence;

	const char *code_bic;
	const char *indicatif_agence;
	const char *domiciliation_agence;
	const char *hash_code;

	lCompte liste_compte;

	lClient liste_client;

	lConseiller liste_conseiller;

};

struct liste_agence
{
	Agence agence;
	lAgence next_agence;
};
struct ville
{
	const char *uuid_ville;

	const char* nom_ville;

	const char *code_postale;

	const char *hash_code;

	lAgence liste_agences;

};
struct data
{
	Ville ville;

	lData next_ville;

};
struct admin
{
	const char* uuid_admin;

	Login_Admin login_admin;

	Admin next_admin;

};