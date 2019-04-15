#include"parsejson.h"
// #define WORKSPACE
struct rib
{
	const char* iban;
	const char* code_bic;
	const char* numero_compte;
	const char* indicatif_agence;
	const char* domiciliation_agence;

};
union client_s
{
	Client client;

	Client clients[2];

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

struct client{

	const char* uuid_client;
	Login client_login;
	int statut;

	Donnees_Personnelles donnees_perso;

	Compte comptes;

};
struct conseiller
{
	const char *uuid_conseiller;
	int statut;

	Donnees_Personnelles info_conseiller;
	Login login_conseiller;

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
struct login
{
    const char* login_id;
    const char* login_key;

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
struct liste_conseiller
{
	Conseiller conseiller;

	lConseiller next_conseiller;
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
#ifdef WORKSPACE

static  void *guaranteed_memset(void *v,int c,size_t n)
{
	volatile char *p=v;
	while (n--)
		*p++=c;
	return v;
}
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
static const char* path_finder(const char* filename)
{
	const char* path=calloc(FILENAME_MAX_, sizeof(char));

	char cmd[64];

	sprintf( cmd, "find ./database -name %s", filename);

	FILE* fp=popen(cmd,"r");

	if(!fp)
	{
		fail("unable to find the JSON file PATH");
		return NULL;
	}

	fscanf(fp, "%216s", path);

	pclose(fp);

	return path;

}

static int json_dump_data_file(json_t* root, const char* file_uuid)
{

	const char *filename = malloc(( strlen(file_uuid) + 5 + 1) * sizeof(char));

	strcat(filename,file_uuid);
	strcat(filename,".json");

	const char* path= path_finder(filename);
	int res=json_dump_file(root, path, JSON_COMPACT);
	if (res==-1)
	{
		json_decref(root);

		fail("json_dump_file returned an invalid error");

		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;

}

static json_t* json_load_data_file(const char* file_uuid)
{
	json_t *json;
	json_error_t error;

	const char *filename = malloc((strlen(file_uuid) + 5 + 1) * sizeof(char));

	strcpy(filename, file_uuid);
	strcat(filename, ".json");

	const char* path=path_finder(filename);

	free(filename);

	json = json_load_file(path, JSON_REJECT_DUPLICATES, &error);
	free(path);
	if (!json)
	{
		fprintf(stderr,"Error type: %s %s, at line: %d, column: %d\n",error.source, error.text, error.line, error.column);
		fail("json_load_file returned an invalid error");
		return NULL;
	}

	return json;


}

Rib parse_rib_struct(json_t *rib_object, size_t flags)
{
	json_error_t error;

	char *iban, *code_bic, *numero_compte, *domiciliation;
	int res, iban_size, code_bic_size, numero_compte_size, indicatif_agence, domiciliation_size;

	res = json_unpack_ex(rib_object, &error, flags, "{s:s%, s:s%, s:s%, s:i, s:s%}", "iban", &iban, &iban_size, "code_bic", &code_bic, &code_bic_size, "numero_compte", &numero_compte, &numero_compte_size, "indicatif_agence", &indicatif_agence, "domiciliation_agence", &domiciliation, &domiciliation_size);

	if( res ||  iban_size != IBAN_SIZE || code_bic_size != CODE_BIC_SIZE || numero_compte_size > NUMERO_COMPTE_SIZE || indicatif_agence > 99999 || domiciliation_size > DOMICILIATION_SIZE )
	{
		json_decref(rib_object);
		fail("json_unpack object Rib: Expected a string but JSON key value is not or JSON string value exceeded length expected or incompatible JSON key value encountered");
		fprintf(stderr,"Error type: %s %s, at line: %d, column: %d\n",error.source, error.text, error.line, error.column);
		return NULL;
	}
	rib_size_t alloc_size;
	alloc_size.iban_size= iban_size+1;
	alloc_size.numero_compte_size=numero_compte_size+1;
	alloc_size.code_bic_size=code_bic_size+1;
	alloc_size.domiciliation_size=domiciliation_size+1;


	return init_rib_arg(alloc_size, iban, code_bic, numero_compte, indicatif_agence, domiciliation);

}

char* parse_operations_csv(json_t *operations_object)
{
	json_error_t error;
	char* uuid_operations;
	int res= json_unpack_ex(operations_object, &error, 0, "{s:s%}", "uuid_operations", &uuid_operations);
	if(res)
	{
		json_decref(operations_object);
		fail("json_unpack object operations: Wrong type, format or incompatible JSON value encountered");
		fprintf(stderr,"Error type: %s %s, at line: %d, column: %d\n",error.source, error.text, error.line, error.column);
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
	char *date_operation, *libelle, *detail, *montant, *devise;

	fputs("Date de l'operation;Libelle;Detail de l'ecriture;Montant de l'operation;Devise", fp);

	json_array_foreach(op_array, index, value)
	{
		if(json_unpack(value, "{s:s, s:s, s:s, s:s, s:s !}", "date_operation", &date_operation, "libelle", &libelle, "detail", &detail, "montant", &montant, "devise", &devise))
		{
			json_decref(operations_object);
			fail("json_unpack object operations: Wrong type, format or incompatible JSON value encountered");
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

	char *uuid_compte, *uuid_operations, *iban;
	int nature_compte, type_compte;

	int res, uuid_size, iban_size, operations_size;

	res=json_unpack_ex(compte_object, &error, 0, "{s:s%, s:i, s:i, s:{s:s%}, s:s%}", "uuid_compte", &uuid_compte, &uuid_size, "nature_compte", &nature_compte, "type_compte", &type_compte, "rib", "iban",&iban, &iban_size, "uuid_operations", &uuid_operations, &operations_size );

	if (res || uuid_size != UUID_SIZE || operations_size != UUID_SIZE || (nature_compte!=INDIVIDUEL && nature_compte!=JOINT) ||  !(type_compte >= LIVRET_A && type_compte<=PEL) )
	{
		json_decref(compte_object);
		fail("json_unpack object Compte: Wrong type, format or incompatible JSON value encountered");
		fprintf(stderr,"Error type: %s %s, at line: %d, column: %d\n",error.source, error.text, error.line, error.column);

		return NULL;
	}

	json_t *titulaire_array=json_object_get(compte_object, "titulaire");

	char *titulaire[4], *uuid_client[2];

	int nom_size1, prenom_size1, nom_size2, prenom_size2, uuid1_size, uuid2_size;

	res=json_unpack_ex(titulaire_array, &error, JSON_ALLOW_NUL, "[{s:s%, s:s%, s:s%}, {s?:s%, s?:s%, s?:s%}]", "uuid_client",&(uuid_client[0]), &uuid1_size, "nom_client", &(titulaire[0]), &nom_size1,"prenom_client", &(titulaire[1]), &prenom_size1,"uuid_client", &(uuid_client[1]), &uuid2_size, "nom_client", &(titulaire[2]), &nom_size2, "prenom_client", &prenom_size2, &(titulaire[3]));


	if(res || prenom_size1>PRENOM_SIZE || nom_size1> NOM_SIZE || uuid1_size!=UUID_SIZE ||  prenom_size2>PRENOM_SIZE || nom_size2> NOM_SIZE || uuid2_size!=UUID_SIZE)
	{
		json_decref(compte_object);
		fail("json_unpack object Compte: Wrong type, format or incompatible JSON value encountered");
		return NULL;
	}
	client_size_t alloc_size;
	if(uuid2_size==0)
	{
		alloc_size.nom_size=nom_size1+1;
		alloc_size.prenom_size=prenom_size1+1;
	}
	Client_s titulaire_s=init_client_s_arg(alloc_size, uuid_client, titulaire);

	return init_compte_arg(uuid_compte, iban, nature_compte, type_compte, uuid_operations, titulaire_s);

}

Donnees_Personnelles parse_donnees_perso_struct(json_t *donnees_object, size_t flags)
{
	json_error_t error;

	char *nom, *prenom, sexe, *date_de_naissance, *email, *adresse, *nom_ville, *complement_ad, *numero_mobile, *numero_fixe, *carte_id, *date_de_creation;
	int code_postale, res, nom_size, prenom_size, sexe_size, date_de_naissance_size, email_size, adresse_size, nom_ville_size, complement_ad_size, numero_mobile_size, numero_fixe_size, carte_id_size, date_de_creation_size;


	res = json_unpack_ex(donnees_object, &error, flags, "{s:s%, s:s%,  s:s%, s:s%, s:s%, s:s%, s:s%, s:i, s:s%, s:s%, s?s%, s:s%, s:s%}", "nom", &nom, &nom_size,"prenom", &prenom, &prenom_size,"sexe", &sexe, &sexe_size,"date_de_naissance", &date_de_naissance, &date_de_naissance_size,"email", &email, &email_size,"adresse", &adresse, &adresse_size,"complement_adresse", &complement_ad, &complement_ad_size,"nom_ville", &nom_ville, &nom_ville_size, "code_postale", &code_postale, "numero_mobile", &numero_mobile, &numero_mobile_size,"numero_fixe", &numero_fixe, &numero_fixe_size, "carte_id", &carte_id, &carte_id_size,"date_de_creation", &date_de_creation, &date_de_creation_size );

	if(res || nom_size > NOM_SIZE || prenom_size > PRENOM || date_de_naissance_size > DATE_DE_NAISSANCE_SIZE || email_size > EMAIL_SIZE || adresse_size> ADRESSE_SIZE || nom_ville_size >  NOM_VILLE_SIZE || complement_ad_size > COMPLEMENT_AD_SIZE || code_postale > 99999 || numero_mobile_size >NUMERO_MOBILE_SIZE || numero_fixe_size > NUMERO_FIXE_SIZE || carte_id_size > CARTE_ID_SIZE || date_de_creation_size > DATE_DE_CREATION_SIZE || sexe_size==1)
	{
		json_decref(donnees_object);
		fail("json_unpack object Donnees_peronnelles: Expected a string but JSON key value is not one or JSON string value exceeded length expected or an incompatible JSON value encountered");
		fprintf(stderr,"Error type: %s %s, at line: %d, column: %d\n",error.source, error.text, error.line, error.column);

		return NULL;
	}
	client_size_t alloc_size;
	alloc_size.adresse_size =adresse_size +1;
	alloc_size.carte_id_size = carte_id_size +1;
	alloc_size.complement_ad_size = complement_ad_size +1;
	alloc_size.date_de_creation_size =date_de_creation_size +1 ;
	alloc_size.date_de_naissance_size = date_de_naissance_size +1 ;
	alloc_size.email_size=email_size +1;
	alloc_size.nom_size = nom_size +1;
	alloc_size.nom_ville_size = nom_ville_size +1;
	alloc_size.numero_fixe_size = numero_fixe_size +1;
	alloc_size.numero_mobile_size = numero_mobile_size +1;
	alloc_size.prenom_size = prenom_size  +1;

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
		json_decref(login_object);
		fail("json_unpack object Login: Wrong type, format or incompatible JSON value encountered");
		fprintf(stderr,"Error type: %s %s, at line: %d, column: %d\n",error.source, error.text, error.line, error.column);

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

	if (res || uuid_size != UUID_SIZE || (statut != 0 && statut != 1))
	{
		json_decref(client_object);
		fail("json_unpack object Client: Expected a string but JSON key value is not one or JSON string value exceeded length expected or an incompatible JSON value encountered");
		fprintf(stderr,"Error type: %s %s, at line: %d, column: %d\n",error.source, error.text, error.line, error.column);
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

	if( res || uuid_conseiller_size != UUID_SIZE || (statut!=0 && statut!=1))
	{
		json_decref(conseiller_object);
		fail("json_unpack object Conseiller: Wrong type, format or incompatible JSON value encountered");
		fprintf(stderr,"Error type: %s %s, at line: %d, column: %d\n",error.source, error.text, error.line, error.column);

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

	if (res || uuid_agence_size != UUID_SIZE || code_bic_size != CODE_BIC_SIZE || indicatif_agence > 99999 || domiciliation_agence_size > DOMICILIATION_SIZE || hash_code_size > HASH_CODE_SIZE)
	{
		json_decref(agence_object);
		fail("json_unpack object Agence: Expected a string but JSON key value is not or JSON string value exceeded length expected or incompatible JSON key value encountered");
		fprintf(stderr,"Error type: %s %s, at line: %d, column: %d\n",error.source, error.text, error.line, error.column);
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

	if (res || uuid_size != UUID_SIZE || nom_ville_size > NOM_VILLE_SIZE || hash_code_size > HASH_CODE_SIZE)
	{
		json_decref(ville_object);
		fail("json_unpack object Ville: Expected a JSON string value or JSON string value exceeded length expected");
		fprintf(stderr,"Error type: %s %s, at line: %d, column: %d\n",error.source, error.text, error.line, error.column);

		return NULL;
	}
	ville_size_t alloc_size;

	alloc_size.uuid_ville=uuid_size + 1;
	alloc_size.nom_ville=nom_ville_size + 1;
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
		json_decref(root);
		fail("parsing basic data: error: root is not an array");
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



void** login_id_parsing(const char* path, Agence agence, Client client, Conseiller conseiller)
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
		json_decref(root_ag);
		fail("parsing data for the login process(1) failed: JSON data format error or invalid JSON file");
		return NULL;
	}

	char* uuid_agence_verify;
	if(json_unpack(root_ag, "{s:s}", "uuid_agence", &uuid_agence_verify))
	{
		json_decref(root_ag);
		fail("parsing data for the login process(2) failed: JSON data format error or invalid JSON file");
		return NULL;
	}

	if(strcmp(uuid_agence_verify,agence->uuid_agence)!=0)
	{
		json_decref(root_ag);
		fail("ERROR-POSSIBLE DATA CONFUSION: JSON data values does NOT corresponds to its file");
		return NULL;
	}

	json_set_alloc_funcs(secure_malloc, secure_free);

	int boolean=(client!=NULL && conseiller==NULL);
	void **results=calloc(2, sizeof(json_t*));
	results[0]=calloc(1, sizeof(json_t));
	results[1]=calloc(1, sizeof(json_t));

	switch(boolean)
	{
		case 1:
		{
			json_t *client_array = json_object_get(root_ag, "liste_client");
			if (!json_is_array(client_array))
			{
				json_decref(root_ag);
				fail("parsing data for the login process(3.1) failed: wrong JSON data type or format");
				return NULL;
			}

			json_t *client_js, *login_js = json_pack("{s:s, s:s}", "login_id", login->login_id, "login_key", login->login_key);
			size_t index;

			json_array_foreach(client_array, index, client_js)
			{
				if (json_equal(json_object_get(json_object_get(client_js, "login_client"),"login_id"), json_object_get(login_js, "login_id")))
				{
					// json_decref(login_js);
					fputs("login client ID was found", stdout);
					// json_decref(client_js);
					// json_decref(client_array);
					// json_decref(root_ag);
					(results[0])=root_ag;
					(results[1])=client_js;
					return results;
				// json_decref(client_js);
				}
			// json_decref(login_js);
			}
			// json_decref(client_js);
			// json_decref(client_array);
			// json_decref(root_ag);

			fputs("login client ID was not found", stdout);

			(results[0]) = root_ag;
			(results[1]) = NULL;
			return results;
		}

		case 0:
		{
			assert(client==NULL && conseiller!=NULL);

			json_t *conseillers_array = json_object_get(root_ag, "liste_conseiller");
			if (!json_is_array(conseillers_array))
			{
				json_decref(root_ag);
				fail("parsing data for the login process(3.2) failed: wrong JSON data type or format");
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
					(results[0]) = root_ag;
					(results[1]) = conseiller_js;
					return results;
				// json_decref(conseiller_js);
				}
			}
			// json_decref(login_js);

			// json_decref(conseiller_js);
			// json_decref(conseillers_array);
			// json_decref(root_ag);
			fputs("login conseiller ID was not found", stdout);

			(results[0]) = root_ag;
			(results[1]) = NULL;
			return results;
		}
		default:
		{
			break;
		}

	}
	json_decref(root_ag);
	fail("IMPOSSIBLE ERROR: check login process");
	return NULL;
}

int parse_ville_data(Ville ville)
{

	json_t *ville_object, *agence_array, *agence_object;

	ville_object=json_load_data_file(ville->uuid_ville);

	if (!json_is_object(ville_object))
	{
		json_decref(ville_object);
		fail("parsing data file ville process(0), check(0) failed: wrong JSON data type or format");
		return EXIT_FAILURE;
	}
	if(!isEqualVille(parse_ville_struct(ville_object), ville))
	{
		json_decref(ville_object);
		fail("parsing data file ville process(0), check(1) failed: POSSIBLE DATA CONFUSION: JSON data values does NOT corresponds to its file");
		return EXIT_FAILURE;
	}

	agence_array=json_object_get(ville_object, "liste_agences");

	if (!json_is_array(agence_array))
	{
		json_decref(ville_object);
		fail("parsing data file ville process(1), check(2) failed: wrong JSON data type or format");
		return EXIT_FAILURE;
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
			json_decref(ville_object);
			fail("parsing data file ville process(2), check(3) failed: wrong JSON data type or format");
			return EXIT_FAILURE;
		}


		if(!addAgence(ville->liste_agences, agence))
		{
			json_decref(ville_object);
			fail("parsing data file ville process(2), check(4) failed: data contains a duplicate");
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

	json_t *agence_object, *client_array, *conseiller_array, *compte_array;
	json_t *client_object, *conseiller_object, *compte_object;

	agence_object = json_load_data_file(agence->uuid_agence);

	if (!json_is_object(agence_object))
	{
		json_decref(agence_object);
		fail("parsing data file agence process(0), check(0) failed: wrong JSON data type or format");
		return EXIT_FAILURE;
	}
	if (!isEqualAgence(parse_agence_struct(agence_object, 0), agence))
	{
		json_decref(agence_object);
		fail("parsing data file agence process(0), check(1) failed: POSSIBLE DATA CONFUSION: JSON data values does NOT corresponds to its file");
		return EXIT_FAILURE;
	}

/*----------------------CLIENT--------------------*/

	client_array = json_object_get(agence_object, "liste_client");

	if (!json_is_array(client_array))
	{
		json_decref(agence_object);
		fail("parsing data file agence process(1), check(2) failed: wrong JSON data type or format");
		return EXIT_FAILURE;
	}

	size_t index, breakout = json_array_size(client_array) - 1;

	agence->liste_client = init_liste_client();
	lClient liste_client= agence->liste_client;
	Client client;

	json_array_foreach(client_array, index, client_object)
	{
		client = parse_client_struct(client_object, 0);

		if (client == NULL)
		{
			json_decref(agence_object);
			fail("parsing data file agence process(1.2), check(3) failed: wrong JSON data type or format");
			return EXIT_FAILURE;
		}

		if (!addClient(agence->liste_client, client))
		{
			json_decref(agence_object);
			fail("parsing data file agence process(1.2), check(4) failed: data contains a duplicate");
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
		json_decref(agence_object);
		fail("parsing data file agence process(2.1), check(2) failed: wrong JSON data type or format");
		return EXIT_FAILURE;
	}

	index=0;
	breakout = json_array_size(compte_array) - 1;

	agence->liste_compte = init_liste_compte();
	lCompte liste_compte = agence->liste_compte;
	Compte compte;

	json_array_foreach(compte_array, index, compte_object)
	{
		compte=parse_compte_struct(compte_object,0);

		if (compte == NULL)
		{
			json_decref(agence_object);
			fail("parsing data file agence process(2.2), check(3) failed: wrong JSON data type or format");
			return EXIT_FAILURE;
		}

		if (!addCompte(agence->liste_compte, compte))
		{
			json_decref(agence_object);
			fail("parsing data file agence process(2.2), check(4) failed: data contains a duplicate");
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
		json_decref(agence_object);
		fail("parsing data file agence process(2.1), check(2) failed: wrong JSON data type or format");
		return EXIT_FAILURE;
	}

	index=0;
	breakout = json_array_size(conseiller_array) - 1;

	agence->liste_conseiller = init_liste_conseiller();
	lConseiller liste_conseiller = agence->liste_conseiller;
	Conseiller conseiller;

	json_array_foreach(conseiller_array, index, conseiller_object)
	{
		conseiller = parse_conseiller_struct(conseiller_object, 0);

		if (conseiller == NULL)
		{
			json_decref(agence_object);
			fail("parsing data file agence process(2.2), check(3) failed: wrong JSON data type or format");
			return EXIT_FAILURE;
		}

		if (!addConseiller(agence->liste_conseiller, conseiller))
		{
			json_decref(agence_object);
			fail("parsing data file agence process(2.2), check(4) failed: data contains a duplicate");
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
	json_t **result;
	strcpy(path, ville->uuid_ville);
	strcat(path, "/");
	strcat(path, agence->uuid_agence);
	switch (option)
	{
		case 1:

			result=(json_t**)(login_id_parsing(path, agence, client, NULL));
			if(result[0]!=NULL && result[1]!=NULL)
			{
				json_t *login_js=json_object_get(result[1], "login_client");
				if(strcpy(client->client_login->login_key,json_string_value(json_object_get(login_js, "login_key")))==0)
				{
					json_decrefp(result);
					fputs("login client is VALID", stdout);
					return EXIT_SUCCESS;
				}
				fputs("login client is INVALID", stdout);
				json_decrefp(result);


				return EXIT_FAILURE;
			}
			break;
		case 2:

			result=(json_t**)(login_id_parsing(path, agence, client, NULL));

			if(result[0]!=NULL && result[1]!=NULL)
			{
				json_t *login_js=json_object_get(result[1], "login_client");

				if(strcpy(client->client_login->login_key,json_string_value(json_object_get(login_js, "login_key")))==0)
				{
					fputs("login client is VALID", stdout);

					if(login==NULL || login->login_id==NULL || login->login_key==NULL)
					{
						json_decrefp(result);
						fail("login changing password process, check failed: data contains an invalid pointer ");
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

			result=(json_t**)(login_id_parsing(path, agence, NULL, conseiller));
			if(result[0]!=NULL && result[1]!=NULL)
			{
				json_t *login_js=json_object_get(result[1], "login_conseiller");
				if(strcpy(conseiller->login_conseiller->login_key,json_string_value(json_object_get(login_js, "login_key")))==0)
				{
					json_decrefp(result);
					fputs("login conseiller is VALID", stdout);
					return EXIT_SUCCESS;
				}
				json_decrefp(result);
				fputs("login conseilller is INVALID", stdout);
				return EXIT_FAILURE;
			}
			break;
		case 4:

			result=(json_t**)(login_id_parsing(path, agence, NULL, conseiller));

			if(result[0]!=NULL && result[1]!=NULL)
			{
				json_t *login_js=json_object_get(result[1], "login_conseiller");

				if(strcpy(conseiller->login_conseiller->login_key,json_string_value(json_object_get(login_js, "login_key")))==0)
				{
					fputs("login conseiller is VALID", stdout);

					if(login==NULL || login->login_id==NULL || login->login_key==NULL)
					{
						json_decrefp(result);
						fail("login changing password process, check failed: data contains an invalid pointer ");
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
			break;

	}
	return EXIT_FAILURE;
}


int parse_compte_data(Compte compte)
{

	json_t *compte_object;

	compte_object= json_load_data_file(compte->uuid_compte);

	if (!json_is_object(compte_object))
	{
		json_decref(compte_object);
		fail("parsing data file compte process(0), check(0) failed: wrong JSON data type or format");
		return EXIT_FAILURE;
	}
	Compte compte_copy=parse_compte_struct(compte_object, JSON_STRICT);
	if (!isEqualCompte(compte_copy, compte))
	{
		json_decref(compte_object);
		fail("parsing data file compte process(0), check(1) failed: POSSIBLE DATA CONFUSION: JSON data values does NOT corresponds to its file");
		return EXIT_FAILURE;
	}
	if(compte->rib!=NULL)
	{
		free(compte->rib);
		free(compte->operations);
	}
	compte->rib =parse_rib_struct(json_object_get(compte_object, "rib"), JSON_STRICT);
	compte->nature_compte=compte_copy->nature_compte;
	compte->type_compte=compte_copy->type_compte;
	compte->operations=compte_copy->operations;

	int statut=json_integer_value(json_object_get(compte_object,"statut"));
	double solde=json_number_value(json_object_get(compte_object,"solde"));
	if((statut!=0 && statut!=1))
	{
		json_decref(compte_object);
		fail("parsing data file compte process(1), check(2) failed: wrong JSON data value type or format");
		return EXIT_FAILURE;
	}
	compte->statut=statut;
	compte->solde=solde;
	compte->titulaire=compte_copy->titulaire;

	return EXIT_SUCCESS;


}
int parse_client_data(Client client)
{
	json_error_t error;

	json_t *client_object, *donnees_object;

	client_object= json_load_data_file(client->uuid_client);

	if (!json_is_object(client_object))
	{
		json_decref(client_object);
		fail("parsing data file client process(0), check(0) failed: wrong JSON data type or format");
		return EXIT_FAILURE;
	}
	Client client_copy=parse_client_struct(client_object, JSON_STRICT);
	if (!isEqualClient(client_copy, client))
	{
		json_decref(client_object);
		fail("parsing data file client process(0), check(1) failed: POSSIBLE DATA CONFUSION: JSON data values does NOT corresponds to its file");
		return EXIT_FAILURE;
	}
	donnees_object=json_object_get(client_object, "donnees_personnelles");
	Donnees_Personnelles donnees_s=parse_donnees_perso_struct(donnees_object, JSON_STRICT);
	if(client->donnees_perso!=NULL)
	{
		free(client->donnees_perso);
	}
	client->donnees_perso=donnees_s;
	if(client->comptes!=NULL)
	{
		free(client->comptes);
	}
	json_t *compte_array=json_object_get(client_object, "comptes");
	if(!json_is_array(compte_array))
	{
		json_decref(client_object);
		fail("parsing data file client process(1), check(2) failed: wrong JSON data type or format");
		return EXIT_FAILURE;
	}
	json_t* value;
	size_t index, uuid_size, breakout=json_array_size(compte_array);
	Compte compte=init_compte(), itercompte=compte;
	json_array_foreach(compte_array, index, value)
	{
		int res= json_unpack_ex(value, &error, JSON_REJECT_DUPLICATES, "{s:s%}", "uuid_compte", &(itercompte->uuid_compte), &uuid_size);
		if(res)
		{
			json_decref(client_object);
			fail("json_unpack object comptes in Client Data: Wrong type, format or incompatible JSON value encountered");
			fprintf(stderr,"Error type: %s %s, at line: %d, column: %d\n",error.source, error.text, error.line, error.column);
			return EXIT_FAILURE;
		}
		if(index < breakout - 1)
		{
			itercompte->next_compte=init_compte();
			itercompte=itercompte->next_compte;
		}
	}
	itercompte=compte;
	do
	{
		if(!parse_compte_data(itercompte))
		{
			json_decref(client_object);
			fail("parsing data file client process(2), check(4) failed: Compte data parsing: wrong JSON data type or format");
			return EXIT_FAILURE;
		}
		itercompte=itercompte->next_compte;
	} while(itercompte!=NULL);

	client->comptes=compte;
	return EXIT_SUCCESS;
}
int parse_conseiller_data(Conseiller conseiller)
{


	json_t *conseiller_object, *donnees_object;

	conseiller_object= json_load_data_file(conseiller->uuid_conseiller);

	if (!json_is_object(conseiller_object))
	{
		json_decref(conseiller_object);
		fail("parsing data file conseiller process(0), check(0) failed: wrong JSON data type or format");
		return EXIT_FAILURE;
	}
	Conseiller conseiller_copy=parse_conseiller_struct(conseiller_object, 0);
	if (!isEqualConseiller(conseiller_copy, conseiller))
	{
		json_decref(conseiller_object);
		fail("parsing data file conseiller process(0), check(1) failed: POSSIBLE DATA CONFUSION: JSON data values does NOT corresponds to its file");
		return EXIT_FAILURE;
	}

	donnees_object=json_object_get(conseiller_object, "donnees_personnelles");
	Donnees_Personnelles donnees_s=parse_donnees_perso_struct(donnees_object, JSON_STRICT);
	if(conseiller->info_conseiller!=NULL)
	{
		free(conseiller->info_conseiller);
	}
	conseiller->info_conseiller=donnees_s;


	return EXIT_SUCCESS;
}


int parse_full_data(lData Data)
{
	if(!parse_valid_basic_data(BASIC_DATA, Data))
	{
		fail("FULL DATA FAILED TO PARSE: process(0)");
		return EXIT_FAILURE;
	}
	assert(Data->ville->uuid_ville!=NULL);

	lData iterdata;

	do
	{
		if(!parse_ville_data(iterdata->ville))
		{
			fail("FULL DATA FAILED TO PARSE: process(1), lVille data parsing");
			return EXIT_FAILURE;
		}

		lAgence iter_agence=iterdata->ville->liste_agences;

		do
		{

			if(!parse_agence_data(iter_agence->agence))
			{
				fail("FULL DATA FAILED TO PARSE: process(2), lAgence data parsing");
				return EXIT_FAILURE;
			}


			lClient iter_client=iter_agence->agence->liste_client;
			do
			{
				if(!parse_client_data(iter_client->client))
				{
					fail("FULL DATA FAILED TO PARSE: process(3.1), lClient data parsing");
					return EXIT_FAILURE;
				}

				iter_client=iter_client->next_client;
			} while (iter_client!=NULL);


			lConseiller iter_conseiller=iter_agence->agence->liste_conseiller;
			do
			{
				if(!parse_conseiller_data(iter_conseiller->conseiller))
				{
					fail("FULL DATA FAILED TO PARSE: process(3.2), lConseiller data parsing");
					return EXIT_FAILURE;
				}

				iter_conseiller=iter_conseiller->next_conseiller;
			} while (iter_conseiller!=NULL);


			lCompte iter_compte=iter_agence->agence->liste_compte;
			do
			{
				if(!parse_compte_data(iter_compte->compte))
				{
					fail("FULL DATA FAILED TO PARSE: process(3.2), lCompte data parsing");
					return EXIT_FAILURE;
				}

				iter_compte=iter_compte->next_compte;
			} while (iter_compte!=NULL);


			iter_agence=iter_agence->next_agence;
		} while (iter_agence!=NULL);

		iterdata=iterdata->next_ville;

	} while (iterdata!=NULL);

	fputs("FULL DATA PARSING WENT SUCCESSFULLY", stdout);

	return EXIT_SUCCESS;

}

int dump_compte_data(Compte compte)
{
	json_t *compte_object;
	json_error_t error;
	int bool0, bool1, bool2, bool3, bool4, bool5, bool6, bool7, bool8;

	bool0=strlen(compte->uuid_compte)==UUID_SIZE;
	bool1=compte->statut==0 || compte->statut==1;
	bool2=compte->nature_compte==JOINT || compte->nature_compte==INDIVIDUEL;
	bool3=compte->type_compte>=LIVRET_A && compte->type_compte<=PEL;
	bool4=strlen(compte->operations)==UUID_SIZE;
	bool5=strlen(compte->rib->iban)==IBAN_SIZE;
	bool6=strlen(compte->rib->code_bic)==CODE_BIC_SIZE;
	bool7=(compte->rib->indicatif_agence>0 && atoi(compte->rib->indicatif_agence)<99999 );
	bool8=strlen(compte->rib->domiciliation_agence)==DOMICILIATION_SIZE;

	if((bool0 && bool1 && bool2 && bool3 && bool4 && bool5 && bool6 && bool7 && bool8)==0)
	{
		fail("Dumping Json failed process(0): Compte struct, wrong format or invalid data");
		return EXIT_FAILURE;
	}

	compte_object=json_pack_ex(&error, JSON_COMPACT, "{s:s, s:i, s:i, s:i, s:f, s:s, s:{s:s, s:s, s:i, s:s}}", "uuid_compte", compte->uuid_compte, "statut", compte->statut, "nature_compte", compte->nature_compte, "type_compte", compte->type_compte, "solde", compte->solde, "uuid_operations", compte->operations, "rib", "iban", compte->rib->iban, "code_bic", compte->rib->code_bic, "indicatif_agence",  compte->rib->indicatif_agence, "domiciliation_agence", compte->rib->domiciliation_agence);

	if(!compte_object)
	{
		fail("json_pack struct Compte: JSON string value exceeded length expected or incompatible JSON key value encountered");
		fprintf(stderr,"Error type: %s %s, at line: %d, column: %d\n",error.source, error.text, error.line, error.column);
		return EXIT_FAILURE;
	}

	json_dump_data_file(compte_object, compte->uuid_compte);

	json_decref(compte_object);

	return EXIT_SUCCESS;

}
int dump_client_data(Client client);
int dump_agence_data(Client client);
int dump_basic_data(Client client);

#endif