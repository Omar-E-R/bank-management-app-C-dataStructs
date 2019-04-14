#include "ville.h"

/*


permet de decrypter les donnees des agences



*/

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


Ville init_ville()
{
	Ville ville = (Ville)calloc(1, sizeof(struct ville));

	ville->uuid_ville = (const char *)calloc(UUID_SIZE, sizeof(char));
	ville->code_postale = (const char *)calloc(CODE_POSTALE_SIZE, sizeof(char));
	ville->nom_ville = (const char *)calloc(NOM_VILLE_SIZE, sizeof(char));
	ville->hash_code = (const char *)calloc(HASH_CODE_SIZE, sizeof(char));

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

Admin new_admin()
{
	Admin admin = init_admin();

	uuid_gen(admin->uuid_admin);

	return admin;
}
lAgence init_liste_agence()
{
	lAgence liste_agence=(lAgence)calloc(1, sizeof(struct liste_agence));

	return liste_agence;


}
Ville new_ville(const char* nom_ville, const char* code_postale)
{
	Ville ville = init_ville();

	uuid_gen(ville->uuid_ville);
	strcpy(ville->code_postale, code_postale);
	strcpy(ville->nom_ville, nom_ville);

	return ville;
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
			var->next_agence= init_agence();
		}
		var=var->next_agence;

	}
	return EXIT_SUCCESS;
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

int addVille(lData liste_ville, Ville ville)
{
	if (liste_ville == NULL || ville == NULL)
	{
		return EXIT_FAILURE;
	}
	if (liste_ville->ville== NULL && ville != NULL)
	{
		liste_ville->ville=ville;
		return EXIT_SUCCESS;
	}
	if (liste_ville->next_ville == NULL && isEqualVille(liste_ville->ville, ville) != EXIT_SUCCESS)
	{
		liste_ville->next_ville=init_data();
		liste_ville->next_ville->ville = ville;

		return EXIT_SUCCESS;
	}
	return addVille(liste_ville->next_ville, ville);
}
int addAdmin(Admin lAdmin, Admin admin)
{
	if (lAdmin == NULL || admin == NULL)
	{
		return EXIT_FAILURE;
	}
	if (lAdmin->next_admin == NULL && isEqualAdmin(lAdmin, admin) != EXIT_SUCCESS)
	{
		lAdmin->next_admin = admin;

		return EXIT_SUCCESS;
	}
	return addAdmin(lAdmin->next_admin, admin);
}

Ville decrypt_agence(Login user)
{


}