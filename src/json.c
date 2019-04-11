#include"json.h"

/*
 * Copyright (c) 2009-2016 Petri Lehtinen <petri@digip.org>
 *
 * Jansson is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include <stdlib.h>
#include <string.h>

#include <jansson.h>

#define BUFFER_SIZE (256 * 1024) /* 256 KB */

#define URL_FORMAT "./database/%s/%s/"
#define URL_SIZE 256

/* Return the offset of the first newline in text or the length of
   text if there's no newline */

static int newline_offset(const char *text)
{
	const char *newline = strchr(text, '\n');
	if (!newline)
		return strlen(text);
	else
		return (int)(newline - text);
}

struct write_result
{
	char *data;
	int pos;
};

static size_t write_response(void *ptr, size_t size, size_t nmemb, void *stream)
{
	struct write_result *result = (struct write_result *)stream;

	if (result->pos + size * nmemb >= BUFFER_SIZE - 1)
	{
		fprintf(stderr, "error: too small buffer\n");
		return 0;
	}

	memcpy(result->data + result->pos, ptr, size * nmemb);
	result->pos += size * nmemb;

	return size * nmemb;
}

static char *request(const char *url)
{
	char *data = NULL;
	long code;

	FILE* fp;

	fp=fopen(url,"r");
	
	if (!fp)
	{
		fprintf(stderr, "error: unable to open file from %s:\n", url);
		return ERROR_MESSAGE;
	}
	data = malloc(BUFFER_SIZE);
	if (!data)
	{
		fprintf(stderr, "error: unable to allocate memory for this file's data %s:\n", url);
		return ERROR_MESSAGE;
	}
	int c; //BECAUSE Char TYPE CANT CONTAIN AN EOF

	int i = 0; //FIRST LINE NUMBER IS 0

	while ((c = fgetc(fp)) != EOF)
	{
		data[i] = c;
		i++;
	}
	/*		CHECKING ERROR WHILE READING THE FILE 				*/
	if (ferror(fp))
		fputs("\nI/O error when reading\n", stderr);

	fclose(fp);

	/* zero-terminate the result */
	data[i] = '\0';

	return data;
}
int parse_code_postale(json_t* code_postale)
{
	/* code here */

	return SUCCESS;
}
int parse_nom_ville(json_t* nom_ville)
{
	/* code here */

	return SUCCESS;
}
int parse_uuid(json_t* uuid,char* message, int i)
{
	if (!json_is_string(uuid))
	{
		fprintf(stderr, "error: the uuid_%s no: %d is not a string\n",message, (int)(i + 1));
		return FAILURE;
	}
	/* code here */

	return SUCCESS;
}
int parse_numero_compte(json_t* numero_compte)
{
	/* code here */

	return SUCCESS;
}
int parse_iban(json_t* iban)
{
	/* code here */

	return SUCCESS;
}
int parse_code_bic(json_t* code_bic)
{
	/* code here */

	return SUCCESS;
}
int parse_indicatif_agence(json_t* indicatif_agence)
{
	/* code here */

	return SUCCESS;
}
int parse_domiciliation_agence(json_t* domiciliation_agence)
{
	/* code here */

	return SUCCESS;
}
int parse_solde(json_t* solde)
{
	/* code here */

	return SUCCESS;
}
int parse_operations(json_t* operations)
{
	/* code here */

	return SUCCESS;
}
int parse_type_compte(json_t* type_compte)
{
	/* code here */

	return SUCCESS;
}
int parse_status_compte(json_t* statut_compte)
{
	/* code here */

	return SUCCESS;
}
int parse_nature_compte(json_t* nature_compte)
{
	/* code here */

	return SUCCESS;
}
json_t** parse_rib(json_t* rib)
{
	json_t* tab[5];
	/* code here */

	return tab;
}

int parse_compte(json_t* compte)
{
	/* code here */

	return SUCCESS;
}
int parse_nom(json_t* nom)
{
	/* code here */

	return SUCCESS;
}
int parse_prenom(json_t* prenom)
{
	/* code here */

	return SUCCESS;
}
int parse_date_de_naissance(json_t* date_de_naissance)
{
	/* code here */

	return SUCCESS;
}
int parse_email(json_t* email)
{
	/* code here */

	return SUCCESS;
}
int parse_adresse(json_t* complement_ad)
{
	/* code here */

	return SUCCESS;
}
int parse_complement_ad(json_t* complement_ad)
{
	/* code here */

	return SUCCESS;
}
int parse_numero_mobile(json_t* numero_mobile)
{
	/* code here */

	return SUCCESS;
}

int parse_numero_fixe(json_t* numero_fixe)
{
	/* code here */

	return SUCCESS;
}
int parse_donnees_personnelles(json_t *donnees_personnelles)
{
	/* code here */

	return SUCCESS;
}
int parse_client(json_t* client)
{
	/* code here */

	return SUCCESS;
}
int parse_liste_client(json_t* liste_client)
{
	/* code here */

	return SUCCESS;
}
int parse_liste_compte(json_t* liste_compte)
{
	/* code here */

	return SUCCESS;
}
int parse_agence(json_t* agence)
{
	/* code here */

	return SUCCESS;
}
int parse_conseiller(json_t* conseiller)
{
	/* code here */

	return SUCCESS;
}
int parse_liste_conseiller(json_t* liste_conseiller)
{
	/* code here */

	return SUCCESS;
}
int parse_login_id(json_t* login_id)
{
	/* code here */

	return SUCCESS;
}
int parse_login_key(json_t* login_key)
{
	/* code here */

	return SUCCESS;
}
int parse_login(json_t* login)
{
	/* code here */

	return SUCCESS;
}
int parse_login_admin(json_t* login_admin)
{
	/* code here */

	return SUCCESS;
}
int parse_liste_admin(json_t* liste_admin)
{
	/* code here */

	return SUCCESS;
}

lData parse_basic_data(const char* url)
{
	size_t i;
	char *text;

	json_t *root;
	json_error_t error;

	text = request(url);

	if (!text)
		return FAILURE;

	root = json_loads(text, 0, &error);

	free(text);

	if (!root)
	{
		fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
		return FAILURE;
	}

	if (!json_is_array(root))
	{
		fprintf(stderr, "error: root is not an array\n");
		json_decref(root);
		return FAILURE;
	}

	lData liste_data = init_data();
	Ville count;

	for (i = 0; i < json_array_size(root); i++)
	{


		json_t *data, *uuid_ville, *code_postale, *nom_ville;

		data = json_array_get(root, i);
		if (!json_is_object(data))
		{
			fprintf(stderr, "error: the %dth element of the basic data is not an object\n", (int)(i + 1));
			json_decref(root);
			return FAILURE;
		}

		uuid_ville = json_object_get(data, "uuid_ville");
		if(!parse_uuid(uuid_ville,"ville",i))
		{
			json_decref(root);
			return FAILURE;
		}
		

		nom_ville = json_object_get(data, "nom_ville");
		if (!parse_nom_ville(nom_ville))
		{
			// fprintf(stderr, "error: the nom_ville no: %d is not a string\n", (int)(i + 1));
			json_decref(root);
			return FAILURE;
		}
		code_postale = json_object_get(data, "code_postale");
		if (!parse_code_postale(code_postale))
		{
			// fprintf(stderr, "error: the code_postale no: %d is not a string\n", (int)(i + 1));
			json_decref(root);
			return FAILURE;
		}
		/* initializing a new struct ville for the main data struct(stack)*/
		count = init_ville();

		/* Filling the struct data from database */
		strcpy(count->uuid_ville , json_string_value(uuid_ville));
		strcpy(count->nom_ville , json_string_value(nom_ville));
		strcpy(count->code_postale , json_string_value(code_postale));


		/* adding it to the stack (struct data) */
		if(addVille(liste_data, count)!=SUCCESS)
		{
			fprintf(stderr, "error when adding ville struct no %d from database to already parsed data\n", (int)(i + 1));
			json_decref(root);
			return FAILURE;
		}
	}
	json_decref(root);
	
	return liste_data;
}

int parse_ville_data(const char* url, Ville ville)
{
	size_t i;
	char *text;

	json_t *root;
	json_error_t error;
	/* adding PATH to the uuid_ville*/
	strcat(url,ville->uuid_ville);
	strcat(url,".json");

	text = request(url);

	if (!text)
		return FAILURE;

	root = json_loads(text, 0, &error);

	free(text);

	if (!root)
	{
		fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
		return FAILURE;
	}

	if (!json_is_object(root))
	{
		fprintf(stderr, "error: root(ville) is not an object\n");
		json_decref(root);
		return FAILURE;
	}


	json_t *data, *uuid_ville, *code_postale, *nom_ville;


	//----------------------Verifying that database matchs argument----------------------------------

	/* getting object */
	uuid_ville = json_object_get(root, "uuid_ville");
	if (!parse_uuid(uuid_ville, "ville", i))
	{
		json_decref(root);
		return FAILURE;
	}

	if(!strcmp(ville->uuid_ville, json_string_value(uuid_ville)))
	{
		fprintf(stderr, "error: the uuid_ville parsed doesnt match the one passed as argument\n", (int)(i + 1));
		json_decref(root);
		return FAILURE;
	}



	/* getting second object */
	nom_ville = json_object_get(root, "nom_ville");
	if (!parse_nom_ville(nom_ville))
	{
		// fprintf(stderr, "error: the nom_ville no: %d is not a string\n", (int)(i + 1));
		json_decref(root);
		return FAILURE;
	}

	/* verifying that the object parsed is equal to one passed as argument  */
	if (!strcmp(ville->nom_ville, json_string_value(nom_ville)))
	{
		fprintf(stderr, "error: the nom_ville parsed doesnt match the one passed as argument\n", (int)(i + 1));
		json_decref(root);
		return FAILURE;
	}



	/* getting third object */
	code_postale = json_object_get(root, "code_postale");
	if (!parse_code_postale(code_postale))
	{
		// fprintf(stderr, "error: the code_postale no: %d is not a string\n", (int)(i + 1));
		json_decref(root);
		return FAILURE;
	}
	/* verifying that the object parsed is equal to one passed as argument  */
	if (!strcmp(ville->code_postale, json_string_value(code_postale)))
	{
		fprintf(stderr, "error: the code_postale parsed doesnt match the one passed as argument\n", (int)(i + 1));
		json_decref(root);
		return FAILURE;
	}

//-------------------------------PARSING LISTE_AGENCE STRUCT--------------------------------------

	if(ville->liste_agences==NULL)
		ville->liste_agences=init_liste_agence();
	
	Agence newAgence;

	data = json_object_get(root, "liste_agence");

	/* verifying that "liste_agence" is an array */
	if (!json_is_array(data))
	{
		fprintf(stderr, "error: the liste_agence element of this data is not an array\n");
		json_decref(root);
		return FAILURE;
	}


	for (i = 0; i < json_array_size(data); i++)
	{
		json_t *uuid_agence, *code_bic, *indicatif_agence, *domiciliation_agence;


		uuid_agence = json_object_get(data, "uuid_agence");
		if(!parse_uuid(uuid_agence,"agence",i))
		{
			json_decref(root);
			return FAILURE;
		}
		

		code_bic = json_object_get(data, "code_bic");
		if (!parse_code_bic(code_bic))
		{
			// fprintf(stderr, "error: the nom_ville no: %d is not a string\n", (int)(i + 1));
			json_decref(root);
			return FAILURE;
		}
		indicatif_agence = json_object_get(data, "indicatif_agence");
		if (!parse_indicatif_agence(indicatif_agence))
		{
			// fprintf(stderr, "error: the code_postale no: %d is not a string\n", (int)(i + 1));
			json_decref(root);
			return FAILURE;
		}
		domiciliation_agence = json_object_get(data, "domiciliation_agence");
		if (!parse_domiciliation_agence(domiciliation_agence))
		{
			// fprintf(stderr, "error: the code_postale no: %d is not a string\n", (int)(i + 1));
			json_decref(root);
			return FAILURE;
		}
		/* initializing a new struct agence for the data struct(stack) ville passsed as arguemnt */
		newAgence = init_agence();

		/* Filling the struct data from database */
		strcpy(newAgence->uuid_agence , json_string_value(uuid_agence));
		strcpy(newAgence->code_bic , json_string_value(code_bic));
		strcpy(newAgence->domiciliation_agence , json_string_value(indicatif_agence));


		/* adding it to the data stack*/
		if(addAgence(ville->liste_agences, newAgence)!=SUCCESS)
		{
			fprintf(stderr, "error when adding agence struct no %d from database to already parsed data\n", (int)(i + 1));
			json_decref(root);
			return FAILURE;
		}
	}
	json_decref(root);
	
	return SUCCESS;
}

int parse_agence_data(const char* url, Agence agence)
{
	size_t i;
	char *text;

	json_t *root;
	json_error_t error;
	/* adding PATH to the uuid_ville*/
	strcat(url,agence->uuid_agence);
	strcat(url,".json");

	text = request(url);

	if (!text)
		return FAILURE;

	root = json_loads(text, 0, &error);

	free(text);

	if (!root)
	{
		fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
		return FAILURE;
	}

	if (!json_is_object(root))
	{
		fprintf(stderr, "error: root(agence) is not an object\n");
		json_decref(root);
		return FAILURE;
	}


	json_t *data, *uuid_agence, *code_bic, *indicatif_agence, *domiciliation_agence;


	//----------------------Verifying that database matchs argument----------------------------------

	/* getting object */
	uuid_agence = json_object_get(root, "uuid_agence");
	if (!parse_uuid(uuid_agence, "agence", i))
	{
		json_decref(root);
		return FAILURE;
	}

	if(!strcmp(agence->uuid_agence, json_string_value(uuid_agence)))
	{
		fprintf(stderr, "error: the uuid_agence parsed doesnt match the one passed as argument\n", (int)(i + 1));
		json_decref(root);
		return FAILURE;
	}



	/* getting second object */
	code_bic = json_object_get(root, "code_bic");
	if (!parse_code_bic(code_bic))
	{
		// fprintf(stderr, "error: the nom_ville no: %d is not a string\n", (int)(i + 1));
		json_decref(root);
		return FAILURE;
	}

	/* verifying that the object parsed is equal to one passed as argument  */
	if (!strcmp(agence->code_bic, json_string_value(code_bic)))
	{
		fprintf(stderr, "error: the code_bic parsed doesnt match the one passed as argument\n", (int)(i + 1));
		json_decref(root);
		return FAILURE;
	}



	/* getting third object */
	indicatif_agence = json_object_get(root, "indicatif_agence");
	if (!parse_indicatif_agence(indicatif_agence))
	{
		// fprintf(stderr, "error: the code_postale no: %d is not a string\n", (int)(i + 1));
		json_decref(root);
		return FAILURE;
	}
	/* verifying that the object parsed is equal to one passed as argument  */
	if (!strcmp(agence->indicatif_agence, json_string_value(indicatif_agence)))
	{
		fprintf(stderr, "error: the indicatif_agence parsed doesnt match the one passed as argument\n", (int)(i + 1));
		json_decref(root);
		return FAILURE;
	}




	/* getting 4th object */
	domiciliation_agence = json_object_get(data, "domiciliation_agence");
	if (!parse_domiciliation_agence(domiciliation_agence))
	{
		// fprintf(stderr, "error: the code_postale no: %d is not a string\n", (int)(i + 1));
		json_decref(root);
		return FAILURE;
	}
	/* verifying that the object parsed is equal to one passed as argument  */
	if (!strcmp(agence->domiciliation_agence, json_string_value(domiciliation_agence)))
	{
		fprintf(stderr, "error: the domiciliation_agence parsed doesnt match the one passed as argument\n", (int)(i + 1));
		json_decref(root);
		return FAILURE;
	}

//-------------------------------PARSING LISTE_COMPTES STRUCT--------------------------------------


	Compte compte;
	if(agence->liste_comptes==NULL)
	{
		agence->liste_comptes=init_liste_compte();
	}

	data = json_object_get(root, "liste_comptes");

	/* verifying that "liste_comptes" is an array */
	if (!json_is_array(data))
	{
		fprintf(stderr, "error: the liste_comptes element of this data is not an array\n");
		json_decref(root);
		return FAILURE;
	}


	for (i = 0; i < json_array_size(data); i++)
	
	{
		json_t *uuid_compte, *rib, *statut, *nature_compte, *type_compte, *solde, *operations;

		uuid_compte = json_object_get(data, "uuid_compte");
		if(!parse_uuid(uuid_compte,"compte",i))
		{
			json_decref(root);
			return FAILURE;
		}
		
		json_t** tab_rib;
		rib = json_object_get(data, "rib");
		if (!(tab_rib=parse_rib(rib)))
		{
			// fprintf(stderr, "error: the nom_ville no: %d is not a string\n", (int)(i + 1));
			json_decref(root);
			return FAILURE;
		}
		statut = json_object_get(data, "statut");
		if (!parse_status_compte(statut))
		{
			// fprintf(stderr, "error: the code_postale no: %d is not a string\n", (int)(i + 1));
			json_decref(root);
			return FAILURE;
		}
		nature_compte = json_object_get(data, "nature_compte");
		if (!parse_nature_compte(nature_compte))
		{
			// fprintf(stderr, "error: the code_postale no: %d is not a string\n", (int)(i + 1));
			json_decref(root);
			return FAILURE;
		}
		type_compte = json_object_get(data, "type_compte");
		if (!parse_type_compte(type_compte))
		{
			// fprintf(stderr, "error: the code_postale no: %d is not a string\n", (int)(i + 1));
			json_decref(root);
			return FAILURE;
		}
		solde = json_object_get(data, "solde");
		if (!parse_solde(solde))
		{
			// fprintf(stderr, "error: the code_postale no: %d is not a string\n", (int)(i + 1));
			json_decref(root);
			return FAILURE;
		}
		operations = json_object_get(data, "operations");
		if (!parse_operations(solde))
		{
			// fprintf(stderr, "error: the code_postale no: %d is not a string\n", (int)(i + 1));
			json_decref(root);
			return FAILURE;
		}
		/* initializing a new struct agence for the data struct(stack) ville passsed as arguemnt */
		compte= init_compte();

		/* Filling the struct data from database */
		strcpy(compte->uuid_compte , json_string_value(uuid_agence));
		strcpy(compte->rib->iban , json_string_value(tab_rib[1]));
		strcpy(compte->rib->code_bic , json_string_value(tab_rib[2]));
		strcpy(compte->rib->numero_compte , json_string_value(tab_rib[3]));
		strcpy(compte->rib->indicatif_agence , json_string_value(tab_rib[4]));
		strcpy(compte->rib->domiciliation_agence , json_string_value(tab_rib[5]));
		strcpy(compte->statut , json_string_value(statut));
		strcpy(compte->nature_compte , json_string_value(nature_compte));
		strcpy(compte->type_compte , json_string_value(type_compte));

		compte->solde=atof(json_string_value(solde));

		strcpy(compte->operations , json_string_value(operations));

		/*Adding it to the data stack*/
		if (addCompte(agence->liste_comptes, compte) != SUCCESS)
		{
			fprintf(stderr, "error when adding compte struct no %d from database to already parsed data\n", (int)(i + 1));
			json_decref(root);
			return FAILURE;
		}
	}
//-------------------------------PARSING LISTE_CLIENTS STRUCT--------------------------------------

	Client client;
	if (agence->liste_clients == NULL)
	{
		agence->liste_clients = init_liste_client();
	}

	data = json_object_get(root, "liste_clients");

	/* verifying that "liste_clients" is an array */
	if (!json_is_array(data))
	{
		fprintf(stderr, "error: the liste_clients element of this data is not an array\n");
		json_decref(root);
		return FAILURE;
	}

	for (i = 0; i < json_array_size(data); i++)

	{
		json_t *uuid_client, *donnees_personnelles, *statut_client;

		uuid_client = json_object_get(data, "uuid_client");
		if (!parse_uuid(uuid_client, "client", i))
		{
			json_decref(root);
			return FAILURE;
		}

		json_t **tab_donnees;
		donnees_personnelles = json_object_get(data, "rib");
		if (!(tab_donnees = parse_donnees_personnelles(donnees_personnelles)))
		{
			// fprintf(stderr, "error: the nom_ville no: %d is not a string\n", (int)(i + 1));
			json_decref(root);
			return FAILURE;
		}
		statut_client = json_object_get(data, "statut_client");
		if (!parse_status_client(statut_client))
		{
			// fprintf(stderr, "error: the code_postale no: %d is not a string\n", (int)(i + 1));
			json_decref(root);
			return FAILURE;
		}
		
		/* initializing a new struct agence for the data struct(stack) ville passsed as arguemnt */
		client = init_client();

		/* Filling the struct data from database */
		strcpy(client->uuid_client, json_string_value(uuid_client));
		strcpy(client->donnees_perso->nom, json_string_value(tab_donnees[1]));
		strcpy(client->donnees_perso->prenom, json_string_value(tab_donnees[2]));
		client->donnees_perso->sexe= json_string_value(tab_donnees[3])[0];
		strcpy(client->donnees_perso->date_de_naissance, json_string_value(tab_donnees[4]));
		strcpy(client->donnees_perso->email, json_string_value(tab_donnees[5]));
		strcpy(client->donnees_perso->adresse, json_string_value(tab_donnees[6]));
		client->donnees_perso->code_postale=atoi(json_string_value(tab_donnees[7]));
		strcpy(client->donnees_perso->nom_ville, json_string_value(tab_donnees[8]));
		strcpy(client->donnees_perso->complement_ad, json_string_value(tab_donnees[9]));
		strcpy(client->donnees_perso->numero_mobile, json_string_value(tab_donnees[10]));
		strcpy(client->donnees_perso->numero_fixe, json_string_value(tab_donnees[11]));
		strcpy(client->statut, json_string_value(statut_client));


		/*Adding it to the data stack*/
		if (addClient(agence->liste_clients, client) != SUCCESS)
		{
			fprintf(stderr, "error when adding client struct no %d from database to already parsed data\n", (int)(i + 1));
			json_decref(root);
			return FAILURE;
		}
	}
	json_decref(root);
	
	return SUCCESS;
}


