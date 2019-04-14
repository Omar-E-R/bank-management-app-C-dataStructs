#include<json.h>



static void *secure_malloc(size_t size)
{
	/* Store the memory area size in the beginning of the block */
	void *ptr = malloc(size + 8);
	*((size_t *)ptr) = size;
	return ptr + 8;
}

static void secure_free(void *ptr)
{
	size_t size;

	ptr -= 8;
	size = *((size_t *)ptr);

	guaranteed_memset(ptr, 0, size + 8);
	free(ptr);
}

static int json_dump_data_file(json_t* root, const char* file_uuid)
{
	json_error_t error;
	char *path = malloc((strlen(WORKSPACE) + strlen(DATABASE) + strlen(file_uuid) + 5 + 1) * sizeof(char));

	strcpy(path,WORKSPACE);
	strcat(path,DATABASE);
	strcat(path,file_uuid);
	strcat(path,".json");

	int res=json_dump_file(root, path, &error);
	if (res=-1)
	{
		fprintf(stderr, "error: in file:%s on line %d: %s\n", error.source, error.line, error.text);

		fail("json_dump_file returned an invalid error");

		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;


}
static json_t* json_load_data_file(const char* file_uuid)
{
	json_t *json;
	json_error_t error;
	char *path = malloc((strlen(WORKSPACE) + strlen(DATABASE) + strlen(file_uuid) + 5 + 1) * sizeof(char));

	strcpy(path,WORKSPACE);
	strcat(path,DATABASE);
	strcat(path,file_uuid);
	strcat(path,".json");

	json = json_load_file(path, JSON_REJECT_DUPLICATES, &error);

	if (!json)
	{
		fprintf(stderr, "error: in file:%s on line %d: %s\n", error.source, error.line, error.text);
		fail("json_load_file returned an invalid error");

		return NULL;
	}

	return json;


}

Rib parse_rib_struct(json_t *rib_object, size_t flags)
{
	json_error_t error;

	char* iban, code_bic, numero_compte, indicatif_agence, domiciliation;
	int res, iban_size, code_bic_size, numero_compte_size, indicatif_agence, domiciliation_size;

	res = json_unpack_ex(rib_object, &error, flags, "{s:s%, s:s%, s:s%, s:i, s:s%}", "iban", &iban, &iban_size, "code_bic", &code_bic, &code_bic_size, "numero_compte", &numero_compte, &numero_compte_size, "indicatif_agence", &indicatif_agence, "domiciliation_agence", &domiciliation, &domiciliation_size);

	if( iban_size > IBAN_SIZE || code_bic_size > CODE_BIC_SIZE || numero_compte_size > NUMERO_COMPTE_SIZE || indicatif_agence > 99999 || domiciliation_size > DOMICILIATION_SIZE )
	{
		fail("json_unpack object Rib: Expected a string but JSON key value is not or JSON string value exceeded length expected or incompatible JSON key value encountered");
		check_error(json_error_wrong_type, "Wrong type, format of Rib is {s:s, s:s, s:s, s:i, s:s}", "<validation>", error.line, error.column, error.position);
		json_decref(rib_object);
		return NULL;
	}
	rib_size_t alloc_size;
	alloc_size.iban_size= iban_size;
	alloc_size.numero_compte_size=numero_compte_size;
	alloc_size.code_bic_size=code_bic_size;
	alloc_size.domiciliation_size=domiciliation_size;


	return init_rib_arg(alloc_size, iban, code_bic, numero_compte, indicatif_agence, domiciliation);

}

char* parse_operations_csv(json_t *operations_object)
{
	json_error_t error;
	char* uuid_operations;
	int res= json_unpack_ex(operations_object, &error, 0, "{s:s%}", "uuid_operations", &uuid_operations);
	if(res)
	{
		fail("json_unpack object operations: Wrong type, format or incompatible JSON value encountered");
		check_error(json_error_wrong_type, "Wrong type, format", "<validation>", error.line, error.column, error.position);
		json_decref(operations_object);
		return NULL;
	}
	char* path=malloc((strlen(uuid_operations)+strlen(WORKSPACE)+strlen(DATABASE)+1+4)*sizeof(char));
	strcpy(path, WORKSPACE);
	strcat(path, DATABASE);
	strcat(path, uuid_operations);
	strcat(path, ".csv");

	FILE* fp=fopen(path,"w");
	json_t *op_array=json_object_get(operations_object, "operations");
	json_t *value;

	size_t index;
	char* date_operation, libelle, detail, montant, devise;

	fputs("Date de l'operation;Libelle;Detail de l'ecriture;Montant de l'operation;Devise", fp);

	json_array_foreach(op_array, index, value)
	{
		if(json_unpack(value, "{s:s, s:s, s:s, s:s, s:s !}", "date_operation", &date_operation, "libelle", &libelle, "detail", &detail, "montant", &montant, "devise", &devise))
		{
			fail("json_unpack object operations: Wrong type, format or incompatible JSON value encountered");
			json_decref(operations_object);
			return NULL;
		}

		fprintf(fp, "%s;%s;%s;%s;%s\n",date_operation, libelle, detail, montant, devise);

		json_decref(value);
	}

	fclose(fp);

	return path;

}

Compte parse_compte_struct(json_t *compte_object, size_t flags)
{
	json_error_t error;
	char *uuid_compte, *operations;
	double solde;
	int statut, nature_compte, type_compte;

	int res, uuid_size, operations_size;

	res=json_unpack_ex(compte_object, &error, flags, "{s:s%, s:i, s:i, s:i, s:f, s:s%}", "uuid_compte", &uuid_compte, &uuid_size, "statut", &statut, "nature_compte", &nature_compte, "type_compte", &type_compte , "solde", &solde, "operations", &operations );

	if (res || uuid_size > UUID_SIZE || operations_size > OPERATIONS_SIZE || (statut != 0 && statut != 1) || (nature_compte!=INDIVIDUEL && nature_compte!=JOINT) ||  !(type_compte >= LIVRET_A && type_compte<=PEL) || solde <0 )
	{
		fail("json_unpack object Compte: Wrong type, format or incompatible JSON value encountered");
		check_error(json_error_wrong_type, "Wrong type, format of Compte struct", "<validation>", error.line, error.column, error.position);
		json_decref(compte_object);

		return NULL;
	}

	return init_compte_arg(uuid_compte, statut, nature_compte, type_compte, solde, operations);

}

Donnees_Personnelles parse_donnees_perso_struct(json_t *donnees_object, size_t flags)
{
	json_error_t error;

	char* nom, prenom, sexe, date_de_naissance, email, adresse, nom_ville, complement_ad, numero_mobile, numero_fixe, carte_id, date_de_creation;
	int code_postale, res, nom_size, prenom_size, sexe_size, date_de_naissance_size, email_size, adresse_size, nom_ville_size, complement_ad_size, numero_mobile_size, numero_fixe_size, carte_id_size, date_de_creation_size;


	res = json_unpack_ex(donnees_object, &error, flags, "{s:s%, s:s%,  s:s%, s:s%, s:s%, s:s%, s:s%, s:i, s:s%, s:s%, s?s%, s:s%, s:s%}", "nom", &nom, &nom_size,"prenom", &prenom, &prenom_size,"sexe", &sexe, &sexe_size,"date_de_naissance", &date_de_naissance, &date_de_naissance,"email", &email, &email_size,"adresse", &adresse, &adresse_size,"complement_adresse", &complement_ad, &complement_ad_size,"nom_ville", &nom_ville, &nom_ville_size, "code_postale", &code_postale, "numero_mobile", &numero_mobile, &numero_mobile_size,"numero_fixe", &numero_fixe, &numero_fixe_size, "carte_id", &carte_id, &carte_id_size,"date_de_creation", &date_de_creation, &date_de_creation_size );

	if(res || nom_size > NOM_SIZE || prenom_size > PRENOM || date_de_naissance_size > DATE_DE_NAISSANCE_SIZE || email_size > EMAIL_SIZE || adresse_size> ADRESSE_SIZE || nom_ville_size >  NOM_VILLE_SIZE || complement_ad_size > COMPLEMENT_AD_SIZE || code_postale > 99999 || numero_mobile_size >NUMERO_MOBILE_SIZE || numero_fixe_size > NUMERO_FIXE_SIZE || carte_id_size > CARTE_ID_SIZE || date_de_creation_size > DATE_DE_CREATION_SIZE || sexe_size==1)
	{
		fail("json_unpack object Donnees_peronnelles: Expected a string but JSON key value is not one or JSON string value exceeded length expected or an incompatible JSON value encountered");
		check_error(json_error_wrong_type, "Wrong type, format of Donnees_personnelles is {s:s, s:s, s:s, s:s, s:s, s:s, s:s, s:i, s:s, s:s, s:s, s:s, s:s}", "<validation>", error.line, error.column, error.position);

		json_decref(donnees_object);

		return NULL;
	}
	client_size_t alloc_size;
	alloc_size.adresse_size =adresse_size  ;
	alloc_size.carte_id_size = carte_id_size ;
	alloc_size.complement_ad_size = complement_ad_size ;
	alloc_size.date_de_creation_size =date_de_creation_size  ;
	alloc_size.date_de_naissance_size = date_de_naissance_size  ;
	alloc_size.email_size=email_size;
	alloc_size.nom_size = nom_size ;
	alloc_size.nom_ville_size = nom_ville_size ;
	alloc_size.numero_fixe_size = numero_fixe_size ;
	alloc_size.numero_mobile_size = numero_mobile_size ;
	alloc_size.prenom_size = prenom_size  ;

	return init_donnees_perso_arg(alloc_size, sexe, nom, prenom, date_de_naissance, email, adresse, code_postale, nom_ville, complement_ad, numero_mobile, numero_fixe, carte_id, date_de_creation);
}

Login parse_login_struct(json_t *login_object)
{

	json_set_alloc_funcs(secure_malloc, secure_free);

	json_error_t error;

	char *id, *key;
	int res, id_size, key_size;

	res = json_unpack_ex(login_object, &error, JSON_STRICT, "{ s:s%, s:s% }", "login_id", &id, &id_size, "login_key", &key, &key_size);

	if (res || id_size > LOGIN_ID_SIZE || key_size > LOGIN_KEY_SIZE)
	{
		fail("json_unpack object Login: Wrong type, format or incompatible JSON value encountered");
		check_error(json_error_wrong_type, "Wrong type, format of Login struct", "<validation>", error.line, error.column, error.position);
		json_decref(login_object);

		return NULL;
	}
	login_size_t alloc_size;
	alloc_size.id=id_size+1;
	alloc_size.key=key_size+1;

	return init_login_arg(alloc_size, id, key);
}

Client parse_client_struct(json_t *client_object, size_t flags)
{
	json_error_t error;

	char *uuid_client;
	int statut, res, uuid_size;

	res = json_unpack_ex(client_object, &error, 0, "{ s:s%, s:i }", "uuid_client", &uuid_client, &uuid_size, "statut", &statut);

	if (res || uuid_size > UUID_SIZE || (statut != 0 && statut != 1))
	{
		fail("json_unpack object Client: Expected a string but JSON key value is not one or JSON string value exceeded length expected or an incompatible JSON value encountered");
		check_error(json_error_wrong_type, "Wrong type, format of Client is {s:s, s:i, s:{s:s, s:s}}", "<validation>", error.line, error.column, error.position);
		json_decref(client_object);
		return NULL;
	}

	return init_client_arg(uuid_client, statut);
}


Conseiller parse_conseiller_struct(json_t *conseiller_object, size_t flags)
{
	json_error_t error;

	char* uuid_conseiller;

	int res, statut, uuid_conseiller_size;

	res=json_unpack_ex(conseiller_object, &error, flags, "{s:s%, s:i}", "uuid_conseiller", &uuid_conseiller, &uuid_conseiller_size, "statut", &statut);

	if( res || uuid_conseiller_size > UUID_SIZE || (statut!=0 && statut!=1))
	{
		fail("json_unpack object Conseiller: Wrong type, format or incompatible JSON value encountered");
		check_error(json_error_wrong_type, "Wrong type, format of  conseiller", "<validation>", error.line, error.column, error.position);
		json_decref(conseiller_object);

		return NULL;
	}

	return init_conseiller_arg( uuid_conseiller, statut);
}


Agence parse_agence_struct(json_t *agence_object, size_t flags)
{
	json_error_t error;
	char *uuid_agence, *code_bic, *domiciliation_agence, *hash_code;
	int uuid_agence_size, code_bic_size, indicatif_agence, domiciliation_agence_size, hash_code_size;

	int res;

	res = json_unpack_ex(agence_object, &error, flags, "{s:s%, s:s%, s:i, s:s%, s:s%}", "uuid_agence",&uuid_agence, &uuid_agence_size, "code_bic", &code_bic, &code_bic_size, "indicatif_agence", &indicatif_agence, "domiciliation_agence", &domiciliation_agence, &domiciliation_agence_size, "hash_code", &hash_code, &hash_code_size);

	if (res || uuid_agence_size > UUID_SIZE || code_bic_size > CODE_BIC_SIZE || indicatif_agence > 99999 || domiciliation_agence_size > DOMICILIATION_SIZE || hash_code_size > HASH_CODE_SIZE)
	{
		fail("json_unpack object Agence: Expected a string but JSON key value is not or JSON string value exceeded length expected or incompatible JSON key value encountered");
		check_error(json_error_wrong_type, "Wrong type, format of Ville->Agence is {s:s, s:i, s:s, s:s}", "<validation>", error.line, error.column, error.position);
		json_decref(agence_object);
		return NULL;
	}

	agence_size_t alloc_size;

	alloc_size.uuid_agence=uuid_agence_size + 1;
	alloc_size.code_bic=code_bic_size + 1;
	alloc_size.domiciliation_agence=domiciliation_agence_size + 1;
	alloc_size.hash_code=hash_code_size + 1;

	return init_agence_arg(alloc_size, uuid_agence, code_bic, indicatif_agence, domiciliation_agence, hash_code);
}

Ville parse_ville_struct(json_t *ville_object)
{
	json_error_t error;

	int code_postale, uuid_size, nom_ville_size, hash_code_size;

	char *uuid, *nom_ville, *hash_code;

	int res;

	res = json_unpack_ex(ville_object, &error, 0, "{s:s%, s:i, s:s%, s:s%}", "uuid_compte", &uuid, &uuid_size, "code_postale", &code_postale, "nom_ville", &nom_ville, &nom_ville_size, "hash_code", &hash_code, &hash_code_size);

	if (res || uuid_size > UUID_SIZE || nom_ville_size > NOM_VILLE_SIZE || hash_code_size > hash_code_size)
	{
		fail("json_unpack object Ville: Expected a JSON string value or JSON string value exceeded length expected");
		check_error(json_error_wrong_type, "Wrong type, format of lData->ville is {s:s, s:i, s:s, s:s}", "<validation>", error.line, error.column, error.position);
		json_decref(ville_object);

		return NULL;
	}
	ville_size_t alloc_size;

	alloc_size.uuid_ville=uuid_size + 1;
	alloc_size.nom_ville=nom_ville + 1;
	alloc_size.hash_code=hash_code_size + 1;

	return init_ville_arg(alloc_size, uuid, code_postale, nom_ville, hash_code);
}

int parse_valid_basic_data(const char* basic_data_filename, lData basic_data)
{

	if (!basic_data_filename)
		return EXIT_FAILURE;

	json_t *root;

	root = json_load_data_file(basic_data_filename);

	if(!json_is_array(root))
	{
		fail("parsing basic data: error: root is not an array");
		json_decref(root);
		return EXIT_FAILURE;
	}
	size_t index;
	json_t *value;

	Ville ville;
	json_array_foreach(root, index, value)
	{

		if ((ville=parse_ville_struct(value))==NULL || !addVille(basic_data,ville))
		{
			json_decref(root);
			fprintf(stderr, "error adding data to lData struct: a possible error is that uuid_ville element has a duplicate\n");
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;

}



json_t** login_id_parsing(const char* path, Agence agence, Client client, Conseiller conseiller)
{
	Login login;

	if(path==NULL || (client == NULL && conseiller == NULL))
	{
		fail("parsing data for the login process(0) failed: Passing NULL or INVALID  pointers");

		return NULL;
	}else
	{
		if(client!=NULL)
		{
			login=client->client_login;
		}else
		{
			login=conseiller->login_conseiller;

		}

		assert(login != NULL && login->login_id != NULL && login->login_key != NULL);
	}

	json_t *root_ag=json_load_data_file(path);


	if (!json_is_object(root_ag))
	{
		fail("parsing data for the login process(1) failed: JSON data format error or invalid JSON file");
		json_decref(root_ag);
		return NULL;
	}

	char* uuid_agence_verify;
	if(json_unpack(root_ag, "{s:s}", "uuid_agence", &uuid_agence_verify))
	{
		fail("parsing data for the login process(2) failed: JSON data format error or invalid JSON file");
		json_decref(root_ag);
		return NULL;
	}

	if(strcmp(uuid_agence_verify,agence->uuid_agence)!=0)
	{
		fail("ERROR-POSSIBLE DATA CONFUSION: JSON data values does NOT corresponds to its file");
		json_decref(root_ag);
		return NULL;
	}

	json_set_alloc_funcs(secure_malloc, secure_free);

	int boolean=(client!=NULL && conseiller==NULL);
	json_t *results[2];

	switch(boolean)
	{
		case 1:
		{
			json_t *client_array = json_object_get(root_ag, "liste_client");
			if (!json_is_array(client_array))
			{
				fail("parsing data for the login process(3.1) failed: wrong JSON data type or format");
				json_decref(root_ag);
				return NULL;
			}

			json_t *client_js, *login_js = json_pack("{s:s, s:s}", "login_id", login->login_id, "login_key", login->login_key);
			size_t index;

			json_array_foreach(client_array, index, client_js)
			{
				if (json_equal(json_object_get(json_object_get(client_js, "login_client"),"login_id"), json_object_get(login_js, "login_id")))
				{
					fputs("login client ID was found", stdout);
					// json_decref(login_js);
					// json_decref(client_js);
					// json_decref(client_array);
					// json_decref(root_ag);
					results[0]=root_ag;
					results[1]=client_js;
					return results;
				}
				// json_decref(client_js);
			}
			// json_decref(login_js);
			// json_decref(client_js);
			// json_decref(client_array);
			// json_decref(root_ag);

			fputs("login client ID was not found", stdout);

			results[0] = root_ag;
			results[1] = NULL;
			return results;
		}

		case 0:
		{
			assert(client==NULL && conseiller!=NULL);

			json_t *conseillers_array = json_object_get(root_ag, "liste_conseiller");
			if (!json_is_array(conseillers_array))
			{
				fail("parsing data for the login process(3.2) failed: wrong JSON data type or format");
				json_decref(root_ag);
				return NULL;
			}

			json_t *conseiller_js, *login_js = json_pack("{s:s, s:s}", "login_id", login->login_id, "login_key", login->login_key);
			size_t index;

			json_array_foreach(conseillers_array, index, conseiller_js)
			{
				if (json_equal(json_object_get(json_object_get(conseiller_js, "login_conseiller"), "login_id"), json_object_get(login_js, "login_id")))
				{
					fputs("login conseiller ID was found", stdout);

					// json_decref(login_js);
					// json_decref(conseiller_js);
					// json_decref(conseillers_array);
					// json_decref(root_ag);
					results[0] = root_ag;
					results[1] = conseiller_js;
					return results;
				}
				// json_decref(conseiller_js);
			}

			// json_decref(login_js);
			// json_decref(conseiller_js);
			// json_decref(conseillers_array);
			// json_decref(root_ag);
			fputs("login conseiller ID was not found", stdout);

			results[0] = root_ag;
			results[1] = NULL;
			return results;
		}
		default:
		{
			break;
		}

	}
	fail("IMPOSSIBLE ERROR: check login process");
	json_decref(root_ag);
	return NULL;
}

int parse_ville_data(Ville ville)
{
	json_error_t error;

	json_t *ville_object, *agence_array, *agence_object;

	ville_object=json_load_data_file(ville->uuid_ville);

	if (!json_is_object(ville_object))
	{
		fail("parsing data file ville process(0), check(0) failed: wrong JSON data type or format");
		json_decref(ville_object);
		return NULL;
	}
	if(!isEqualVille(parse_ville_struct(ville_object), ville))
	{
		fail("parsing data file ville process(0), check(1) failed: POSSIBLE DATA CONFUSION: JSON data values does NOT corresponds to its file");
		json_decref(ville_object);
		return NULL;
	}

	agence_array=json_object_get(ville_object, "liste_agences");

	if (!json_is_array(agence_array))
	{
		fail("parsing data file ville process(1), check(2) failed: wrong JSON data type or format");
		json_decref(ville_object);
		return NULL;
	}

	size_t index, breakout=json_array_size(agence_array)-1;

	ville->liste_agences= init_liste_agence();

	lAgence liste_agence=ville->liste_agences;

	Agence agence;

	json_array_foreach(agence_array, index, agence_object)
	{
		agence=parse_agence_struct(agence_object, JSON_STRICT);

		if (agence == NULL)
		{
			fail("parsing data file ville process(2), check(3) failed: wrong JSON data type or format");
			json_decref(ville_object);
			return EXIT_FAILURE;
		}


		if(!addAgence(ville->liste_agences, agence))
		{
			fail("parsing data file ville process(2), check(4) failed: data contains a duplicate");
			json_decref(ville_object);
			return EXIT_FAILURE;
		}

		if(index < breakout )
		{
			liste_agence=liste_agence->next_agence;
		}

	}

	liste_agence->next_agence=ville->liste_agences; //circular stack

	json_decref(ville_object);

	return EXIT_SUCCESS;
}



int parse_agence_data(Agence agence)
{
	json_error_t error;

	json_t *agence_object, *client_array, *conseiller_array, *compte_array;

	agence_object = json_load_data_file(agence->uuid_agence);

	if (!json_is_object(agence_object))
	{
		fail("parsing data file agence process(0), check(0) failed: wrong JSON data type or format");
		json_decref(agence_object);
		return NULL;
	}
	if (!isEqualAgence(parse_agence_struct(agence_object, 0), agence))
	{
		fail("parsing data file agence process(0), check(1) failed: POSSIBLE DATA CONFUSION: JSON data values does NOT corresponds to its file");
		json_decref(agence_object);
		return NULL;
	}

/*----------------------CLIENT--------------------*/

	client_array = json_object_get(agence_object, "liste_client");

	if (!json_is_array(client_array))
	{
		fail("parsing data file agence process(1), check(2) failed: wrong JSON data type or format");
		json_decref(agence_object);
		return NULL;
	}

	size_t index, breakout = json_array_size(client_array) - 1;

	agence->liste_client = init_liste_client();
	lClient liste_client= agence->liste_client;
	Client client;

	json_array_foreach(client_array, index, agence_object)
	{
		client = parse_client_struct(agence_object, 0);

		if (client == NULL)
		{
			fail("parsing data file agence process(1.2), check(3) failed: wrong JSON data type or format");
			json_decref(agence_object);
			return EXIT_FAILURE;
		}

		if (!addClient(agence->liste_client, client))
		{
			fail("parsing data file agence process(1.2), check(4) failed: data contains a duplicate");
			json_decref(agence_object);
			return EXIT_FAILURE;
		}

		if (index < breakout)
		{
			liste_client = liste_client->next_client;
		}
	}

	liste_client->next_client = agence->liste_client; //circular stack

/*-----------------------COMPTE---------------------------*/

	compte_array = json_object_get(agence_object, "liste_compte");

	if (!json_is_array(compte_array))
	{
		fail("parsing data file agence process(2.1), check(2) failed: wrong JSON data type or format");
		json_decref(agence_object);
		return NULL;
	}

	size_t index, breakout = json_array_size(compte_array) - 1;

	agence->liste_compte = init_liste_compte();
	lCompte liste_compte = agence->liste_compte;
	Compte compte=init_compte();

	json_array_foreach(compte_array, index, agence_object)
	{
		char* uuid_compte=json_string_value(json_object_get(agence_object, "uuid_compte"));
		char* iban=json_string_value(json_object_get((json_object_get(agence_object, "rib")), "iban"));

		compte->type_compte=json_integer_value(json_object_get(agence_object, "type_compte"));

		compte->rib->iban=(const char*)calloc(IBAN_SIZE, sizeof(char));

		strcpy(compte->rib->iban, iban);
		strcpy(compte->uuid_compte, uuid_compte);

		if (compte == NULL)
		{
			fail("parsing data file agence process(2.2), check(3) failed: wrong JSON data type or format");
			json_decref(agence_object);
			return EXIT_FAILURE;
		}

		if (!addCompte(agence->liste_compte, compte))
		{
			fail("parsing data file agence process(2.2), check(4) failed: data contains a duplicate");
			json_decref(agence_object);
			return EXIT_FAILURE;
		}

		if (index < breakout)
		{
			liste_compte = liste_compte->next_compte;
		}
	}

	liste_compte->next_compte = agence->liste_compte; //circular stack


/*------------------------CONSEILLERS--------------------------*/
	conseiller_array = json_object_get(agence_object, "liste_conseiller");

	if (!json_is_array(conseiller_array))
	{
		fail("parsing data file agence process(2.1), check(2) failed: wrong JSON data type or format");
		json_decref(agence_object);
		return NULL;
	}

	size_t index, breakout = json_array_size(conseiller_array) - 1;

	agence->liste_conseiller = init_liste_conseiller();
	lConseiller liste_conseiller = agence->liste_conseiller;
	Conseiller conseiller;

	json_array_foreach(conseiller_array, index, agence_object)
	{
		conseiller = parse_conseiller_struct(agence_object, 0);

		if (conseiller == NULL)
		{
			fail("parsing data file agence process(2.2), check(3) failed: wrong JSON data type or format");
			json_decref(agence_object);
			return EXIT_FAILURE;
		}

		if (!addConseiller(agence->liste_conseiller, conseiller))
		{
			fail("parsing data file agence process(2.2), check(4) failed: data contains a duplicate");
			json_decref(agence_object);
			return EXIT_FAILURE;
		}

		if (index < breakout)
		{
			liste_conseiller = liste_conseiller->next_conseiller;
		}
	}

	liste_conseiller->next_conseiller = agence->liste_conseiller; //circular stack

	json_decref(agence_object);

	return EXIT_SUCCESS;
}



int login_key_parsing(Login login, Client client, Conseiller conseiller, Ville ville, Agence agence, int option)
{
	const char *path = malloc((strlen(ville->uuid_ville)+1+strlen(agence->uuid_agence)+1)*sizeof(char));

	strcpy(path, ville->uuid_ville);
	strcat(path, "/");
	strcat(path, agence->uuid_agence);
	switch (option)
	{
	case 1:
		json_t ** result=login_id_parsing(path, agence, client, NULL);
		if(result[0]!=NULL && result[1]!=NULL)
		{
			json_t *login_js=json_object_get(result[1], "login_client");
			if(strcpy(client->client_login->login_key,json_string_value(json_object_get(login_js, "login_key")))==0)
			{
				fputs("login client is VALID", stdout);
				json_decrefp(result);
				return EXIT_SUCCESS;
			}
			fputs("login client is INVALID", stdout);

			json_decrefp(result);

			return EXIT_FAILURE;
		}
		break;
	case 2:
		json_t ** result=login_id_parsing(path, agence, client, NULL);

		if(result[0]!=NULL && result[1]!=NULL)
		{
			json_t *login_js=json_object_get(result[1], "login_client");

			if(strcpy(client->client_login->login_key,json_string_value(json_object_get(login_js, "login_key")))==0)
			{
				fputs("login client is VALID", stdout);

				if(login==NULL || login->login_id==NULL || login->login_key==NULL)
				{
					fail("login changing password process, check failed: data contains an invalid pointer ");
					json_decrefp(result);
					return EXIT_FAILURE;
				}
				fputs("Changing password...", stdout);

				json_t *new_login=json_pack("{s:s, s:s}", "login_id", login->login_id, "login_key", login->login_key);

				json_object_set_new(result[1], "login_client", new_login);

				json_dump_data_file(result[0], path);

				fputs("Password change is done...", stdout);

				json_decrefp(result);

				return EXIT_SUCCESS;
			}
			fputs("login client is INVALID", stdout);

			json_decrefp(result);

			return EXIT_FAILURE;
		}
		break;
	case 3:
		json_t ** result=login_id_parsing(path, agence, NULL, conseiller);
		if(result[0]!=NULL && result[1]!=NULL)
		{
			json_t *login_js=json_object_get(result[1], "login_conseiller");
			if(strcpy(conseiller->login_conseiller->login_key,json_string_value(json_object_get(login_js, "login_key")))==0)
			{
				fputs("login conseiller is VALID", stdout);
				json_decrefp(result);
				return EXIT_SUCCESS;
			}
			fputs("login conseilller is INVALID", stdout);
			json_decrefp(result);
			return EXIT_FAILURE;
		}
		break;
	case 4:
		json_t ** result=login_id_parsing(path, agence, NULL, conseiller);

		if(result[0]!=NULL && result[1]!=NULL)
		{
			json_t *login_js=json_object_get(result[1], "login_conseiller");

			if(strcpy(conseiller->login_conseiller->login_key,json_string_value(json_object_get(login_js, "login_key")))==0)
			{
				fputs("login conseiller is VALID", stdout);

				if(login==NULL || login->login_id==NULL || login->login_key==NULL)
				{
					fail("login changing password process, check failed: data contains an invalid pointer ");
					json_decrefp(result);
					return EXIT_FAILURE;
				}
				fputs("Changing password...", stdout);

				json_t *new_login=json_pack("{s:s, s:s}", "login_id", login->login_id, "login_key", login->login_key);

				json_object_set_new(result[1], "login_conseiller", new_login);

				json_dump_data_file(result[0], path);

				fputs("Password change is done...", stdout);

				json_decrefp(result);

				return EXIT_SUCCESS;
			}
			fputs("login conseiller is INVALID", stdout);

			json_decrefp(result);

			return EXIT_FAILURE;
		}
		break;///////////////////////////////////////////////////////////////////////////

	default:
		fputs("INVALID logon", stdout);

		json_decrefp(result);

		return EXIT_FAILURE;

	}
}


int parse_compte_data(Compte compte);
int parse_client_data(Client client);
int parse_conseiller_data(Conseiller conseiller);
int parse_full_data(lData Data);

int dump_compte_data(Compte compte);
int dump_client_data(Client client);
int dump_agence_data(Client client);
int dump_basic_data(Client client);


