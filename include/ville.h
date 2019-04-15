#ifndef VILLE_H
#define VILLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"agence.h"

typedef struct ville_size
{
	size_t uuid_ville, code_postale, nom_ville, hash_code;

} ville_size_t;

#define FILENAME_MAX_ 216
#define DATABASE "./database"
#define BASIC_DATA "basic_data.json"


typedef struct liste_agence *lAgence;

typedef struct ville *Ville;

typedef struct data *lData;

typedef struct admin *Admin;



lAgence init_liste_agence();

Ville init_ville();

Ville init_ville_arg(ville_size_t allocation_size,char* uuid_ville,int code_postale,char* nom_ville,char* hash_code);

lData init_data();

Admin init_admin();

int isEqualVille(Ville ville1, Ville ville2);

int isEqualAdmin(Admin admin1, Admin admin2);

int addAgence(lAgence liste_agence, Agence agence);

int addVille(lData liste_ville, Ville ville);

int addAdmin(Admin lAdmin, Admin admin);

#endif
