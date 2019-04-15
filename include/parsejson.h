#ifndef PARSEJSON_H
#define PARSEJSON_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include <assert.h>

#include"ville.h"
#include"client.h"



#define failhdr fprintf(stderr, "%s:%d: ", __FILE__, __LINE__)

#define fail(msg)                     \
	do                                \
	{                                 \
		failhdr;                      \
		fprintf(stderr, "%s\n", msg); \
		exit(EXIT_FAILURE);                      \
	} while (0)



Rib parse_rib_struct(json_t *rib_object, size_t flags);

char* parse_operations_csv(json_t *operations_object);

Compte parse_compte_struct(json_t *compte_object, size_t flags);

Donnees_Personnelles parse_donnees_perso_struct(json_t *donnees_object, size_t flags);

Login parse_login_struct(json_t *login_object);

Client parse_client_struct(json_t *client_object, size_t flags);

Conseiller parse_conseiller_struct(json_t *conseiller_object, size_t flags);

Agence parse_agence_struct(json_t *agence_object, size_t flags);

Ville parse_ville_struct(json_t *ville_object);

int parse_valid_basic_data(const char* basic_data_filename, lData basic_data);

void** login_id_parsing(const char* path, Agence agence, Client client, Conseiller conseiller);

int parse_ville_data(Ville ville);

int parse_agence_data(Agence agence);

int login_key_parsing(Login login, Client client, Conseiller conseiller, Ville ville, Agence agence, int option);

int parse_compte_data(Compte compte);

int parse_client_data(Client client);

int parse_conseiller_data(Conseiller conseiller);

int parse_full_data(lData Data);

int dump_compte_data(Compte compte);

int dump_client_data(Client client);

int dump_agence_data(Client client);

int dump_basic_data(Client client);


#endif
