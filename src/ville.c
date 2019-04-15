#include "ville.h"

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

lAgence init_liste_agence()
{
	lAgence liste_agence = (lAgence)calloc(1, sizeof(struct liste_agence));

	return liste_agence;
}

Ville init_ville()
{
	Ville ville = (Ville)calloc(1, sizeof(struct ville));

	ville->uuid_ville = (const char *)calloc(UUID_SIZE, sizeof(char));
	ville->code_postale = (const char *)calloc(CODE_POSTALE_SIZE, sizeof(char));
	ville->nom_ville = (const char *)calloc(NOM_VILLE_SIZE, sizeof(char));
	ville->hash_code = (const char *)calloc(HASH_CODE_SIZE, sizeof(char));

	return ville;
}

Ville init_ville_arg(ville_size_t allocation_size,char* uuid_ville,int code_postale,char* nom_ville,char* hash_code)
{
	Ville ville = (Ville)calloc(1, sizeof(struct ville));

	ville->uuid_ville = (const char *)calloc(allocation_size.uuid_ville, sizeof(char));

	ville->code_postale = (const char *)calloc(CODE_POSTALE_SIZE, sizeof(char));

	ville->nom_ville = (const char *)calloc(allocation_size.nom_ville, sizeof(char));

	ville->hash_code = (const char *)calloc(allocation_size.hash_code, sizeof(char));

	strcpy(ville->uuid_ville, uuid_ville);
	strcpy(ville->nom_ville, nom_ville);
	strcpy(ville->hash_code, hash_code);

	sprintf(ville->code_postale, "%05d", code_postale);

	return ville;
}

lData init_data()
{
	lData data = (lData)calloc(1, sizeof(struct data));


	return data;
}

Admin init_admin()
{
	Admin admin = (Admin)calloc(1, sizeof(struct admin));

	admin->uuid_admin = (const char *)calloc(UUID_SIZE, sizeof(char));

	admin->next_admin=NULL;

	return admin;
}

int isEqualVille(Ville ville1, Ville ville2)
{
	if (strcmp(ville1->uuid_ville, ville2->uuid_ville) == 0 && strcmp(ville1->code_postale, ville2->code_postale) == 0 && strcmp(ville1->nom_ville, ville2->nom_ville) == 0 && strcmp(ville1->hash_code, ville2->hash_code) == 0)
		return EXIT_SUCCESS;
	return EXIT_FAILURE;
}

int isEqualAdmin(Admin admin1, Admin admin2)
{
	if (strcmp(admin1->uuid_admin, admin2->uuid_admin))
		return EXIT_SUCCESS;
	return EXIT_FAILURE;
}

int addAgence(lAgence liste_agence, Agence agence)
{
	lAgence var = liste_agence;

	while (var!=NULL)
	{
		if(var->agence!=NULL)
		{
			if (isEqualAgence((var->agence), agence)==EXIT_SUCCESS)
			{
				return EXIT_FAILURE;
			}

		}else
		{
			var->agence=agence;
			return EXIT_SUCCESS;
		}
		if(var->next_agence==NULL)
		{
			var->next_agence= init_liste_agence();
		}
		var=var->next_agence;

	}
	return EXIT_SUCCESS;
}

int addVille(lData liste_ville, Ville ville)
{
	lData var = liste_ville;

	while (var!=NULL)
	{
		if(var->ville!=NULL)
		{
			if (isEqualVille(var->ville, ville)==EXIT_SUCCESS)
			{
				return EXIT_FAILURE;
			}

		}else
		{
			var->ville=ville;
			return EXIT_SUCCESS;
		}
		if(var->next_ville==NULL)
		{
			var->next_ville= init_data();
		}
		var=var->next_ville;

	}
	return EXIT_SUCCESS;
}

int addAdmin(Admin lAdmin, Admin admin)
{
	Admin var = lAdmin;

	while (var!=NULL)
	{

		if (isEqualAdmin(var, admin)==EXIT_SUCCESS)
		{
			return EXIT_FAILURE;
		}

		if(var->next_admin==NULL)
		{
			var->next_admin= admin;
		}
		var=var->next_admin;

	}
	return EXIT_SUCCESS;
}



/*
Admin new_admin()
{
	Admin admin = init_admin();

	uuid_gen(admin->uuid_admin);

	return admin;
}

Ville new_ville(const char *nom_ville, const char *code_postale)
{
	Ville ville = init_ville();

	uuid_gen(ville->uuid_ville);
	strcpy(ville->code_postale, code_postale);
	strcpy(ville->nom_ville, nom_ville);

	return ville;
}

*/
